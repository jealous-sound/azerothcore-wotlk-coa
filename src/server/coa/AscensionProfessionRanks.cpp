/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "Player.h"
#include "AscensionProfessionPolicy.h"
#include "Config.h"
#include "Item.h"
#include "Mail.h"
#include "ObjectMgr.h"
#include "Trainer.h"
#include "World.h"
#include "ScriptMgr.h"
#include "SpellMgr.h"
#include "WorldSession.h"
#include <algorithm>
#include <map>
#include <vector>

namespace
{
constexpr uint16 SkillValuePerStep = 75;
bool AllAtStart = false;
bool UnrestrictedGathering = false;
thread_local bool Reconciling = false;

bool Human(Player const* player)
{
    return player && player->GetSession() && !player->GetSession()->IsBot();
}

bool ProfessionRank(SpellInfo const* info)
{
    if (!info)
        return false;
    for (SpellEffectInfo const& effect : info->GetEffects())
    {
        if ((effect.IsEffect(SPELL_EFFECT_SKILL_STEP) || effect.IsEffect(SPELL_EFFECT_SKILL)) &&
            (IsProfessionSkill(effect.MiscValue) || effect.MiscValue == 505 || effect.MiscValue == SKILL_WOODCUTTING ||
                effect.MiscValue == SKILL_WOODWORKING))
            return true;
        if (effect.IsEffect(SPELL_EFFECT_LEARN_SPELL))
            if (SpellLearnSkillNode const* node = sSpellMgr->GetSpellLearnSkill(effect.TriggerSpell))
                if (IsProfessionSkill(node->skill) || node->skill == 505 || node->skill == SKILL_WOODCUTTING ||
                    node->skill == SKILL_WOODWORKING)
                    return true;
    }
    return false;
}

void ReconcileProfessions(Player* player)
{
    if (!AllAtStart || !Human(player) || Reconciling)
        return;
    Trainer::Trainer const* trainer = sObjectMgr->GetTrainer(57500);
    if (!trainer)
        return;
    Reconciling = true;
    for (uint32 pass = 0; pass < 6; ++pass)
        for (Trainer::Spell const& row : trainer->GetSpells())
            if (ProfessionRank(sSpellMgr->GetSpellInfo(row.SpellId)) && trainer->CanTeachSpell(player, &row))
            {
                if (row.IsCastable())
                    player->CastSpell(player, row.SpellId, true);
                else
                    player->learnSpell(row.SpellId);
            }
    if (!player->HasSkill(9200))
        player->SetSkill(9200, 1, 1, 300);
    Reconciling = false;
}

bool HasTool(Player* player, uint32 tool)
{
    if (player->HasItemCount(tool, 1, true))
        return true;
    for (Mail const* mail : player->GetMails())
        if (mail->state != MAIL_STATE_DELETED)
            for (MailItemInfo const& item : mail->items)
                if (item.item_template == tool)
                    return true;
    return false;
}

void GrantTools(Player* player)
{
    if (!AllAtStart || !Human(player))
        return;
    constexpr uint32 tools[] = { 5956, 20815, 39505, 2901, 7005, 6256, 6954, 6218 };
    for (uint32 index = 0; index < std::size(tools); ++index)
    {
        uint32 tool = tools[index];
        if (player->GetPlayerSetting("core.hxc.profession.tools", index).value)
            continue;
        if (HasTool(player, tool) || player->AddItem(tool, 1))
        {
            player->UpdatePlayerSetting("core.hxc.profession.tools", index, 1);
            continue;
        }
        Item* item = Item::CreateItem(tool, 1, player);
        if (!item)
            continue;
        CharacterDatabaseTransaction transaction = CharacterDatabase.BeginTransaction();
        item->SaveToDB(transaction);
        MailDraft draft("Profession tools", "Your starter profession tool did not fit in your bags.");
        draft.AddItem(item);
        draft.SendMailTo(transaction, player, MailSender(MAIL_CREATURE, 57500));
        CharacterDatabase.CommitTransaction(transaction);
        player->UpdatePlayerSetting("core.hxc.profession.tools", index, 1);
    }
}

class hxc_professions_world : public WorldScript
{
public:
    hxc_professions_world() : WorldScript("hxc_professions_world", { WORLDHOOK_ON_AFTER_CONFIG_LOAD }) { }

    void OnAfterConfigLoad(bool) override
    {
        AllAtStart = sConfigMgr->GetOption<bool>("CoANeeds.Professions.AllAtStart", false);
        UnrestrictedGathering = sConfigMgr->GetOption<bool>("CoANeeds.Professions.UnrestrictedGathering", false);
        if (AllAtStart)
            sWorld->setIntConfig(CONFIG_MAX_PRIMARY_TRADE_SKILL, 32);
    }
};

struct KnownRank
{
    uint32 spellId;
    SpellLearnSkillNode const* node;
};

std::map<uint16, std::vector<KnownRank>> KnownProfessionRanks(Player const* player)
{
    std::map<uint16, std::vector<KnownRank>> ranks;
    for (auto const& [spellId, playerSpell] : player->GetSpellMap())
    {
        if (playerSpell->State == PLAYERSPELL_REMOVED)
            continue;

        SpellLearnSkillNode const* node = sSpellMgr->GetSpellLearnSkill(spellId);
        if (node && IsProfessionSkill(node->skill))
            ranks[node->skill].push_back({ spellId, node });
    }
    return ranks;
}

void LowerProfessionToStep(Player* player, uint16 skill, std::vector<KnownRank> ranks, uint16 maxStep)
{
    auto const aboveBegin = std::partition(ranks.begin(), ranks.end(),
        [maxStep](KnownRank const& rank) { return rank.node->step <= maxStep; });
    auto const highest = std::max_element(ranks.begin(), aboveBegin,
        [](KnownRank const& left, KnownRank const& right) { return left.node->step < right.node->step; });
    bool const rankAbove = aboveBegin != ranks.end();
    if (highest == aboveBegin
        || (!rankAbove && player->GetPureMaxSkillValue(skill) <= maxStep * SkillValuePerStep))
        return;

    uint16 const value = player->GetPureSkillValue(skill);
    for (auto rank = aboveBegin; rank != ranks.end(); ++rank)
        player->removeSpell(rank->spellId, SPEC_MASK_ALL, false);

    auto active = player->GetSpellMap().find(highest->spellId);
    if (active != player->GetSpellMap().end() && !active->second->Active)
    {
        active->second->Active = true;
        player->SendLearnPacket(highest->spellId, true);
    }

    uint16 const maxValue = highest->node->maxvalue;
    player->SetSkill(skill, highest->node->step, std::min(value, maxValue), maxValue);
}

class ascension_profession_ranks_player : public PlayerScript
{
public:
    ascension_profession_ranks_player() : PlayerScript("ascension_profession_ranks_player", { PLAYERHOOK_ON_LOGIN,
        PLAYERHOOK_ON_LEVEL_CHANGED, PLAYERHOOK_ON_UPDATE_SKILL }) { }

    void OnPlayerLogin(Player* player) override
    {
        uint16 const maxStep = GetMaxProfessionSkillStep(player->GetSession()->Expansion());
        for (auto const& [skill, ranks] : KnownProfessionRanks(player))
            LowerProfessionToStep(player, skill, ranks, maxStep);
        ReconcileProfessions(player);
        GrantTools(player);
    }

    void OnPlayerLevelChanged(Player* player, uint8) override
    {
        ReconcileProfessions(player);
    }

    void OnPlayerUpdateSkill(Player* player, uint32 skill, uint32, uint32, uint32, uint32) override
    {
        if (IsProfessionSkill(skill) || skill == 505)
            ReconcileProfessions(player);
    }
};
}

bool HxcProfessions::AllowGathering(Player const* player, uint32 skill)
{
    return UnrestrictedGathering && Human(player) && player->HasSkill(skill) &&
        (skill == SKILL_MINING || skill == SKILL_HERBALISM || skill == SKILL_SKINNING ||
            skill == SKILL_WOODCUTTING);
}

void AddSC_AscensionProfessionRanks()
{
    new ascension_profession_ranks_player();
    new hxc_professions_world();
}
