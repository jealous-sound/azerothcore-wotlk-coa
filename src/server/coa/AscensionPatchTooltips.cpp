/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AchievementMgr.h"
#include "AscensionPatchAchievements.h"
#include "AscensionPatchSpells.h"
#include "AscensionPatchSpellStore.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "Configuration/Config.h"
#include "Player.h"
#include "ScriptMgr.h"
#include <atomic>
#include <unordered_set>
#include <utility>

namespace
{
std::atomic<bool> TooltipEnabled{false};
std::atomic<bool> AchievementPatchesEnabled{false};
std::atomic<bool> AchievementCriteriaPatchesEnabled{false};
std::atomic<bool> AchievementCategoryPatchesEnabled{false};
std::vector<WorldPacket> CachedSpellPatches;
std::vector<WorldPacket> CachedAchievementPatches;
std::unordered_set<uint32> PatchedSpellIds;

void LoadSpellPatches()
{
    CachedSpellPatches.clear();
    PatchedSpellIds.clear();
    auto statement = WorldDatabase.GetPreparedStatement(WORLD_SEL_PATCH_SPELL);
    if (PreparedQueryResult result = WorldDatabase.Query(statement))
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 spellId = fields[0].Get<uint32>();
            AscensionPatchSpells::TextOverrides overrides;
            for (std::size_t index = 0; index < overrides.size(); ++index)
                if (!fields[index + 1].IsNull())
                    overrides[index] = fields[index + 1].Get<std::string>();
            auto packet = AscensionPatchSpells::BuildSpellPatch(spellId, overrides);
            if (!packet)
            {
                LOG_ERROR("module.ascension.patch",
                    "Skipped patch_spell {}: no baseline record or invalid text (NUL or over 4096 bytes)", spellId);
                continue;
            }
            CachedSpellPatches.push_back(std::move(*packet));
            PatchedSpellIds.insert(spellId);
        } while (result->NextRow());
    }
    LOG_INFO("module.ascension.patch", "Loaded {} patch_spell packets for login", CachedSpellPatches.size());
}

void LoadAchievementPatches()
{
    CachedAchievementPatches.clear();
    auto loadTable = [](WorldDatabaseStatements statementId, AscensionPatchAchievements::Table table,
        std::size_t columnCount, std::size_t stringCount)
    {
        auto statement = WorldDatabase.GetPreparedStatement(statementId);
        if (PreparedQueryResult result = WorldDatabase.Query(statement))
        {
            do
            {
                Field* fields = result->Fetch();
                uint32 id = fields[0].Get<uint32>();
                AscensionPatchAchievements::TextOverrides overrides(stringCount);
                for (std::size_t index = 0; index < columnCount; ++index)
                    if (!fields[index + 1].IsNull())
                        overrides[index] = fields[index + 1].Get<std::string>();
                auto packet = AscensionPatchAchievements::BuildPatch(table, id, overrides);
                if (!packet)
                {
                    LOG_ERROR("module.ascension.patch",
                        "Skipped SQL achievement patch {} for opcode {}: missing baseline or invalid text",
                        id, static_cast<uint16>(table));
                    continue;
                }
                CachedAchievementPatches.push_back(std::move(*packet));
            } while (result->NextRow());
        }
    };
    loadTable(WORLD_SEL_PATCH_ACHIEVEMENT, AscensionPatchAchievements::Table::Achievement, 3, 3);
    loadTable(WORLD_SEL_PATCH_ACHIEVEMENT_CRITERIA, AscensionPatchAchievements::Table::Criteria, 1, 16);
    loadTable(WORLD_SEL_PATCH_ACHIEVEMENT_CATEGORY, AscensionPatchAchievements::Table::Category, 1, 16);
    LOG_INFO("module.ascension.patch", "Loaded {} SQL achievement patch packets for login",
        CachedAchievementPatches.size());
}

class ascension_patch_config : public WorldScript
{
public:
    ascension_patch_config() : WorldScript("ascension_patch_config",
        {WORLDHOOK_ON_AFTER_CONFIG_LOAD, WORLDHOOK_ON_STARTUP}) { }

    void OnAfterConfigLoad(bool) override
    {
        TooltipEnabled.store(sConfigMgr->GetOption<bool>("AscensionPatch.Tooltips", false));
        AchievementPatchesEnabled.store(sConfigMgr->GetOption<bool>("AscensionPatch.Achievements", false));
        AchievementCriteriaPatchesEnabled.store(
            sConfigMgr->GetOption<bool>("AscensionPatch.AchievementCriteria", false));
        AchievementCategoryPatchesEnabled.store(
            sConfigMgr->GetOption<bool>("AscensionPatch.AchievementCategories", false));
    }

    void OnStartup() override
    {
        LoadSpellPatches();
        LoadAchievementPatches();
    }
};

class ascension_patch_login : public PlayerScript
{
public:
    ascension_patch_login() : PlayerScript("ascension_patch_login",
        {PLAYERHOOK_ON_SEND_INITIAL_PACKETS_BEFORE_ADD_TO_MAP}) { }

    void OnPlayerSendInitialPacketsBeforeAddToMap(Player* player, WorldPacket&) override
    {
        AscensionPatchSpells::SendSpellPatches(player);
        if (!AscensionPatchSpells::TooltipPatchesEnabled() || !player || !player->GetSession())
            return;
        bool sentAchievementPatch = false;
        for (WorldPacket const& packet : CachedAchievementPatches)
        {
            bool enabled = false;
            switch (static_cast<AscensionPatchAchievements::Table>(packet.GetOpcode()))
            {
                case AscensionPatchAchievements::Table::Achievement:
                    enabled = AchievementPatchesEnabled.load();
                    break;
                case AscensionPatchAchievements::Table::Criteria:
                    enabled = AchievementCriteriaPatchesEnabled.load();
                    break;
                case AscensionPatchAchievements::Table::Category:
                    enabled = AchievementCategoryPatchesEnabled.load();
                    break;
            }
            if (enabled)
            {
                LOG_INFO("module.ascension.patch", "Sending experimental achievement patch opcode {} to {}",
                    uint16(packet.GetOpcode()), player->GetName());
                player->SendDirectMessage(&packet);
                sentAchievementPatch = true;
            }
        }
        if (sentAchievementPatch)
            player->GetAchievementMgr()->SendAllAchievementData();
    }
};
}

namespace AscensionPatchSpells
{
bool TooltipPatchesEnabled() { return TooltipEnabled.load(); }

bool IsTooltipPatchEnabled(uint32 spellId)
{
    return TooltipPatchesEnabled() && PatchedSpellIds.find(spellId) != PatchedSpellIds.end();
}

void SendSpellPatches(Player* player)
{
    if (!TooltipPatchesEnabled() || !player || !player->GetSession())
        return;
    for (WorldPacket const& packet : CachedSpellPatches)
        player->SendDirectMessage(&packet);
}
}

void AddAscensionPatchTooltipScripts()
{
    new ascension_patch_config();
    new ascension_patch_login();
}
