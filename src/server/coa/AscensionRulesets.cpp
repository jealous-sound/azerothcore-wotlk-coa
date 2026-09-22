/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"

namespace
{
enum RulesetSpells : uint32
{
    SPELL_SELECT_WAR_MODE = 84420,
    SPELL_SELECT_HIGH_RISK = 84421,
    SPELL_SELECT_PVE = 84422,
    SPELL_HIGH_RISK = 1004019,
    SPELL_WAR_MODE = 1004119,
    SPELL_PVE = 9931032,
    SPELL_MERCENARY = 9930874
};

uint32 RulesetAura(uint32 selectionId)
{
    switch (selectionId)
    {
        case SPELL_SELECT_HIGH_RISK:
            return SPELL_HIGH_RISK;
        case SPELL_SELECT_PVE:
            return SPELL_PVE;
        default:
            return SPELL_WAR_MODE;
    }
}

void ApplyRuleset(Player* player, uint32 selectionId)
{
    player->RemoveAurasDueToSpell(SPELL_HIGH_RISK);
    player->RemoveAurasDueToSpell(SPELL_WAR_MODE);
    player->RemoveAurasDueToSpell(SPELL_PVE);
    player->RemoveAurasDueToSpell(SPELL_MERCENARY);
    player->CastSpell(player, RulesetAura(selectionId), true);
}

class spell_ascension_ruleset_select : public SpellScript
{
    PrepareSpellScript(spell_ascension_ruleset_select);

    bool Validate(SpellInfo const*) override
    {
        return ValidateSpellInfo({SPELL_HIGH_RISK, SPELL_WAR_MODE, SPELL_PVE});
    }

    bool Load() override { return GetCaster()->ToPlayer() != nullptr; }

    SpellCastResult CheckCast()
    {
        Player* player = GetCaster()->ToPlayer();
        return player && player->HasPlayerFlag(PLAYER_FLAGS_RESTING) ? SPELL_CAST_OK : SPELL_FAILED_NOT_HERE;
    }

    void Select(SpellEffIndex)
    {
        Player* player = GetCaster()->ToPlayer();
        uint32 id = GetSpellInfo()->Id;
        if (!player || (id != SPELL_SELECT_WAR_MODE && id != SPELL_SELECT_HIGH_RISK && id != SPELL_SELECT_PVE))
            return;

        ApplyRuleset(player, id);
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_ascension_ruleset_select::CheckCast);
        OnEffectHit += SpellEffectFn(spell_ascension_ruleset_select::Select, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

class ruleset_aura_metadata : public GlobalScript
{
public:
    ruleset_aura_metadata() : GlobalScript("ruleset_aura_metadata", {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* info) override
    {
        if (info->Id == SPELL_HIGH_RISK || info->Id == SPELL_WAR_MODE || info->Id == SPELL_PVE)
            info->AttributesEx3 |= SPELL_ATTR3_ALLOW_AURA_WHILE_DEAD;
    }
};

class ruleset_player_spells : public PlayerScript
{
public:
    ruleset_player_spells() : PlayerScript("ruleset_player_spells", {PLAYERHOOK_ON_LOGIN}) { }

    void OnPlayerLogin(Player* player) override
    {
        for (uint32 id : {SPELL_SELECT_WAR_MODE, SPELL_SELECT_HIGH_RISK, SPELL_SELECT_PVE})
            if (!player->HasSpell(id))
                player->learnSpell(id, false);

        if (!sConfigMgr->GetOption<bool>("AscensionCompat.RulesetLoginDefault", true))
            return;

        if (!player->IsInWorld())
            return;

        if (!player->HasAura(SPELL_HIGH_RISK) && !player->HasAura(SPELL_WAR_MODE) && !player->HasAura(SPELL_PVE))
        {
            ApplyRuleset(player, SPELL_SELECT_PVE);
            return;
        }

        if (player->HasAura(SPELL_PVE))
            player->RemoveAurasDueToSpell(SPELL_WAR_MODE);
    }
};
}

void AddSC_AscensionRulesets()
{
    RegisterSpellScript(spell_ascension_ruleset_select);
    new ruleset_aura_metadata();
    new ruleset_player_spells();
}
