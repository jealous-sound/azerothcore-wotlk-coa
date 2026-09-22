/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellScript.h"
#include "Unit.h"

namespace
{
enum WaspFormSpells : uint32
{
    SPELL_WASP_FORM = 805141,
    SPELL_WASP_FORM_FLIGHT = 805142
};

class aura_ascension_venomancer_wasp_form : public AuraScript
{
    PrepareAuraScript(aura_ascension_venomancer_wasp_form);

    void GrantFlight(AuraEffect const*, AuraEffectHandleModes)
    {
        Unit* target = GetTarget();
        if (!target->HasAura(SPELL_WASP_FORM_FLIGHT))
            target->CastSpell(target, SPELL_WASP_FORM_FLIGHT, true);
    }

    void RemoveFlight(AuraEffect const*, AuraEffectHandleModes)
    {
        GetTarget()->RemoveAurasDueToSpell(SPELL_WASP_FORM_FLIGHT);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(aura_ascension_venomancer_wasp_form::GrantFlight, EFFECT_0,
            SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(aura_ascension_venomancer_wasp_form::RemoveFlight, EFFECT_0,
            SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
    }
};
}

void AddSC_AscensionVenomancerWasp()
{
    RegisterSpellScript(aura_ascension_venomancer_wasp_form);
}
