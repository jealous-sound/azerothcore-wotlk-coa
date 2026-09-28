/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include <algorithm>

namespace
{
enum RunemasterRunicTalentSpells : uint32
{
    SPELL_EARTH_TATTOO = 801094,
    SPELL_GRANITE_SHIELD = 520822,
    SPELL_GUARDING_RUNE = 500464
};

constexpr uint32 ASCENSION_EFFECT_REDUCE_COOLDOWN_PCT = 192;

bool HasOwnEarthTattoo(Unit const* unit)
{
    for (auto const& [key, application] : unit->GetAppliedAuras())
    {
        Aura const* aura = application->GetBase();
        if (aura->GetCasterGUID() == unit->GetGUID() &&
            sSpellMgr->GetFirstSpellInChain(aura->GetId()) == SPELL_EARTH_TATTOO)
            return true;
    }
    return false;
}

class aura_ascension_runemaster_granite_shield : public AuraScript
{
    PrepareAuraScript(aura_ascension_runemaster_granite_shield);

    bool Validate(SpellInfo const* info) override
    {
        return info->Effects[EFFECT_0].TriggerSpell == SPELL_GRANITE_SHIELD &&
            ValidateSpellInfo({SPELL_EARTH_TATTOO, SPELL_GRANITE_SHIELD});
    }

    void RequireEarthTattoo(AuraEffect const*)
    {
        if (!HasOwnEarthTattoo(GetTarget()))
            PreventDefaultAction();
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(aura_ascension_runemaster_granite_shield::RequireEarthTattoo,
            EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

class spell_ascension_runemaster_protective_warding : public SpellScript
{
    PrepareSpellScript(spell_ascension_runemaster_protective_warding);

    bool Validate(SpellInfo const* info) override
    {
        SpellEffectInfo const& effect = info->Effects[EFFECT_0];
        return effect.Effect == ASCENSION_EFFECT_REDUCE_COOLDOWN_PCT &&
            effect.MiscValue == int32(SPELL_GUARDING_RUNE) && ValidateSpellInfo({SPELL_GUARDING_RUNE});
    }

    void ReduceGuardingRuneCooldown(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);
        Player* player = GetHitUnit() ? GetHitUnit()->ToPlayer() : nullptr;
        if (!player)
            return;
        uint32 const root = sSpellMgr->GetFirstSpellInChain(uint32(GetSpellInfo()->Effects[effIndex].MiscValue));
        int32 const percent = std::clamp(GetEffectValue(), 0, 100);
        for (auto const& [id, entry] : player->GetSpellMap())
            if (player->HasSpell(id) && sSpellMgr->GetFirstSpellInChain(id) == root)
                if (uint32 remaining = player->GetSpellCooldownDelay(id))
                    player->ModifySpellCooldown(id, -int32(CalculatePct(remaining, percent)));
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_ascension_runemaster_protective_warding::ReduceGuardingRuneCooldown,
            EFFECT_0, SPELL_EFFECT_ANY);
    }
};
}

void AddSC_AscensionRunemasterRunicTalents()
{
    RegisterSpellScript(aura_ascension_runemaster_granite_shield);
    RegisterSpellScript(spell_ascension_runemaster_protective_warding);
}
