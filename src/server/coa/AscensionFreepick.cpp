/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionFreepick.h"
#include "AscensionFreepickRules.h"
#include "AscensionWildcard.h"
#include "Battleground.h"
#include "Config.h"
#include "Log.h"
#include "Map.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellInfo.h"
#include "Tokenize.h"
#include "World.h"
#include <algorithm>
#include <unordered_set>

namespace AscensionFreepick
{
Realm ReadRealm()
{
    Realm realm;
    std::string const types = sConfigMgr->GetOption<std::string>("CoA.RealmType", "live");
    for (std::string_view type : Acore::Tokenize(types, ' ', false))
    {
        realm.Live = realm.Live || type == "live";
        realm.Seasonal = realm.Seasonal || type == "seasonal";
        realm.League = realm.League || type == "league";
        realm.Ptr = realm.Ptr || type == "ptr";
        realm.Development = realm.Development || type == "development";
    }
    if (!realm.Live && !realm.Seasonal && !realm.League && !realm.Ptr && !realm.Development)
        realm.Live = true;
    std::string const model = sConfigMgr->GetOption<std::string>("CoA.ClassModel", "coa");
    realm.ConquestOfAzeroth = model == "coa";
    realm.WarcraftReborn = model == "wcr";
    uint32 const maxLevel = sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL);
    realm.Ruleset = maxLevel <= 60 ? 0 : maxLevel <= 70 ? 1 : 2;
    return realm;
}

namespace
{
constexpr char BUILD_SETTING[] = "core.freepick";
constexpr std::uint32_t RANK_FACTOR = 10;

Catalog Loaded;
Realm CurrentRealm;
bool Classless = false;
bool Reborn = false;

std::vector<Entry> StoredEntries(Player const* player)
{
    std::vector<Entry> entries;
    PlayerSettingVector const* stored = player->FindPlayerSettings(BUILD_SETTING);
    if (!stored || stored->empty())
        return entries;
    std::size_t const count = std::min<std::size_t>((*stored)[0].value, stored->size() - 1);
    for (std::size_t index = 1; index <= count; ++index)
    {
        uint32 const value = (*stored)[index].value;
        Entry const entry{ value / RANK_FACTOR, value % RANK_FACTOR };
        if (entry.Rank && Loaded.Find(entry.EntryId))
            entries.push_back(entry);
    }
    return entries;
}

void Store(Player* player, std::vector<Entry> const& entries)
{
    std::size_t previous = 0;
    if (PlayerSettingVector const* stored = player->FindPlayerSettings(BUILD_SETTING))
        previous = stored->size();
    player->UpdatePlayerSetting(BUILD_SETTING, 0, uint32(entries.size()));
    for (std::size_t index = 0; index < entries.size(); ++index)
        player->UpdatePlayerSetting(BUILD_SETTING, uint32(index + 1),
            entries[index].EntryId * RANK_FACTOR + entries[index].Rank);
    for (std::size_t index = entries.size() + 1; index < previous; ++index)
        player->UpdatePlayerSetting(BUILD_SETTING, uint32(index), 0);
}

UnitCheck UnitRules(Player const* player)
{
    return [player](std::uint32_t slot, Row const& row)
    {
        switch (slot)
        {
            case LEARN_NOT_IN_BATTLEGROUNDS:
            {
                if (row.Type == ENTRY_TALENT || !player->GetMap()->IsBattlegroundOrArena())
                    return true;
                if (player->GetMap()->IsBattleArena())
                    return false;
                Battleground const* battleground = player->GetBattleground();
                return !battleground || battleground->GetStatus() != STATUS_IN_PROGRESS;
            }
            case LEARN_NOT_IN_COMBAT:
                return !player->IsInCombat() || (row.Type == ENTRY_TALENT && player->GetMap()->IsDungeon());
            case LEARN_INVULNERABLE:
                return !player->HasAuraType(SPELL_AURA_SCHOOL_IMMUNITY);
            case LEARN_NOT_WHILE_DEAD:
                return player->IsAlive();
            default:
                return true;
        }
    };
}

std::unordered_set<uint32> GrantedSpells(std::vector<Entry> const& entries)
{
    std::unordered_set<uint32> spells;
    for (Entry const& entry : entries)
        if (Row const* row = Loaded.Find(entry.EntryId); row && entry.Rank && entry.Rank <= row->MaxRank())
            spells.insert(row->Spells[entry.Rank - 1]);
    return spells;
}

void RemoveFromActionBars(Player* player, std::unordered_set<uint32> const& spells)
{
    bool removed = false;
    for (uint8 button = 0; button < MAX_ACTION_BUTTONS; ++button)
    {
        ActionButton const* action = player->GetActionButton(button);
        if (!action || action->GetType() != ACTION_BUTTON_SPELL || !spells.contains(action->GetAction()))
            continue;
        player->removeActionButton(button);
        removed = true;
    }
    if (removed)
        player->SendActionButtons(1);
}

void SyncSpells(Player* player, std::vector<Entry> const& before, std::vector<Entry> const& after)
{
    std::unordered_set<uint32> const granted = GrantedSpells(after);
    std::unordered_set<uint32> removed;
    for (Entry const& entry : before)
        if (Row const* row = Loaded.Find(entry.EntryId))
            for (std::uint32_t rank = row->MaxRank(); rank > 0; --rank)
            {
                uint32 const spellId = row->Spells[rank - 1];
                if (granted.contains(spellId) || !player->HasSpell(spellId))
                    continue;
                player->removeSpell(spellId, SPEC_MASK_ALL, false);
                removed.insert(spellId);
            }
    for (uint32 spellId : granted)
        if (!player->HasSpell(spellId))
            player->learnSpell(spellId);
    RemoveFromActionBars(player, removed);
}
}

bool RealmIsClassless()
{
    return Classless;
}

bool IsFreepickHero(Player const* player)
{
    return Classless && player->getClass() == CLASS_HERO && !AscensionWildcard::IsWildcardHero(player);
}

bool IsRebornCharacter(Player const* player)
{
    uint8 const classId = player->getClass();
    return Reborn && classId >= CLASS_WARRIOR && classId <= CLASS_DRUID && classId != CLASS_HERO;
}

bool HasFreepickBuild(Player const* player)
{
    return IsFreepickHero(player) || IsRebornCharacter(player);
}

std::vector<AscensionCoATalentState::KnownEntry> KnownEntries(Player const* player)
{
    std::vector<AscensionCoATalentState::KnownEntry> known;
    std::vector<Entry> const entries = StoredEntries(player);
    for (std::size_t index = 0; index < entries.size(); ++index)
        known.push_back({ entries[index].EntryId, entries[index].Rank, entries[index].Rank, false, uint32(index + 1) });
    return known;
}

UploadResult ApplyUpload(Player* player, std::vector<AscensionCoATalentState::KnownEntry> const& upload)
{
    std::vector<Entry> wanted;
    for (AscensionCoATalentState::KnownEntry const& entry : upload)
        wanted.push_back({ entry.EntryId, entry.Rank });

    Build const base(Loaded, CurrentRealm, player->GetLevel(), StoredEntries(player), player->getClass());
    UnitCheck const unit = UnitRules(player);
    ApplyCheck const check = CheckApply(base, wanted, unit,
        { player->GetMoney(), player->GetItemCount(MARK_OF_ASCENSION_ITEM, false) });
    if (check.Result != UPDATE_OK)
    {
        LOG_INFO("coa", "Refused free-pick upload of {} record(s) from {}: {} {} entry {} rank {}", upload.size(),
            player->GetName(), UPDATE_RESULTS[check.Result], LEARN_RESULTS[check.Learn], check.Failed.EntryId,
            check.Failed.Rank);
        return { UPDATE_RESULTS[check.Result], check.Learn ? LEARN_RESULTS[check.Learn] : "", check.Failed.EntryId,
            check.Failed.Rank };
    }

    Build next(base);
    next.SetEntries(check.Entries);
    next.AutoLearn(unit);
    if (check.Marks)
        player->DestroyItemCount(MARK_OF_ASCENSION_ITEM, check.Marks, true);
    if (check.Money)
        player->ModifyMoney(-int32(check.Money));
    Store(player, next.Entries());
    SyncSpells(player, base.Entries(), next.Entries());
    LOG_INFO("coa", "Applied free-pick build of {}: {} entries, {} AE and {} TE spent, charged {} copper and {} marks",
        player->GetName(), next.Entries().size(), next.GlobalAE(0), next.GlobalTE(0), check.Money, check.Marks);
    return {};
}

void Synchronize(Player* player)
{
    if (!HasFreepickBuild(player))
        return;
    std::vector<Entry> const stored = StoredEntries(player);
    Build build(Loaded, CurrentRealm, player->GetLevel(), stored, player->getClass());
    if (build.AutoLearn(UnitRules(player)))
        Store(player, build.Entries());
    SyncSpells(player, stored, build.Entries());
}

class AscensionFreepickPlayer final : public PlayerScript
{
public:
    AscensionFreepickPlayer() : PlayerScript("AscensionFreepickPlayer", { PLAYERHOOK_ON_LOGIN }) { }

    void OnPlayerLogin(Player* player) override
    {
        Synchronize(player);
    }
};

class AscensionFreepickWorld final : public WorldScript
{
public:
    AscensionFreepickWorld() : WorldScript("AscensionFreepickWorld", { WORLDHOOK_ON_STARTUP }) { }

    void OnStartup() override
    {
        CurrentRealm = ReadRealm();
        Classless = sConfigMgr->GetOption<std::string>("CoA.ClassModel", "coa") == "hero";
        Reborn = CurrentRealm.WarcraftReborn;
        if ((Classless || Reborn) && !LoadCatalog(Loaded))
            LOG_ERROR("coa", "Free-pick Character Advancement is unavailable: its client DBCs did not load");
    }
};
}

void ApplyAscensionPathPassiveContract(SpellInfo* spellInfo)
{
    constexpr int32 TWO_HANDED_WEAPONS = 0x1562;
    constexpr int32 ONE_HANDED_MELEE_WEAPONS = 0xA091;
    constexpr uint32 AGILE_STRIKES = 986201;
    switch (spellInfo->Id)
    {
        case 986202: case 986200: case 92839: case 92842: case 129245:
            spellInfo->EquippedItemClass = ITEM_CLASS_WEAPON;
            spellInfo->EquippedItemSubClassMask = TWO_HANDED_WEAPONS;
            break;
        case 986203: case AGILE_STRIKES: case 92840: case 92843: case 129246:
            spellInfo->EquippedItemClass = ITEM_CLASS_WEAPON;
            spellInfo->EquippedItemSubClassMask = ONE_HANDED_MELEE_WEAPONS;
            break;
        default:
            return;
    }
    if (spellInfo->Id != AGILE_STRIKES)
        return;
    SpellEffectInfo& cost = spellInfo->Effects[EFFECT_0];
    if (cost.ApplyAuraName != SPELL_AURA_DUMMY)
    {
        LOG_ERROR("coa", "Skipped unexpected Agile Strikes record {}", spellInfo->Id);
        return;
    }
    cost.ApplyAuraName = SPELL_AURA_MOD_POWER_COST_SCHOOL_PCT;
    cost.MiscValue = SPELL_SCHOOL_MASK_NORMAL;
    cost.BasePoints = -9;
}

void AddAscensionFreepickScripts()
{
    new AscensionFreepick::AscensionFreepickPlayer();
    new AscensionFreepick::AscensionFreepickWorld();
}
