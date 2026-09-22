/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "Unit.h"
#include "Util.h"

namespace
{
enum IonicAuraSpells : uint32
{
    IonicAura = 707652
};

class aura_ascension_ionic_aura : public AuraScript
{
    PrepareAuraScript(aura_ascension_ionic_aura);

    void RefreshUnencodedPools(AuraEffect const* effect, bool apply)
    {
        Unit* target = GetTarget();
        float multiplier = 1.0f;
        for (AuraEffect const* applied : target->GetAuraEffectsByType(SPELL_AURA_MOD_INCREASE_ENERGY_PERCENT))
            if (applied != effect && applied->GetId() == IonicAura && applied->GetMiscValue() == POWER_MANA)
                AddPct(multiplier, applied->GetAmount());

        if (apply)
            AddPct(multiplier, effect->GetAmount());

        for (Powers power : {POWER_FOCUS, POWER_ENERGY, POWER_RUNIC_POWER})
            target->SetStatPctModifier(UnitMods(static_cast<uint16>(UNIT_MOD_POWER_START) + power),
                TOTAL_PCT, multiplier);
    }

    void Apply(AuraEffect const* effect, AuraEffectHandleModes)
    {
        RefreshUnencodedPools(effect, true);
    }

    void Remove(AuraEffect const* effect, AuraEffectHandleModes)
    {
        RefreshUnencodedPools(effect, false);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(aura_ascension_ionic_aura::Apply,
            EFFECT_0, SPELL_AURA_MOD_INCREASE_ENERGY_PERCENT, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(aura_ascension_ionic_aura::Remove,
            EFFECT_0, SPELL_AURA_MOD_INCREASE_ENERGY_PERCENT, AURA_EFFECT_HANDLE_REAL);
    }
};
}

void AddSC_AscensionStormbringerIonicAura()
{
    RegisterSpellScript(aura_ascension_ionic_aura);
}
