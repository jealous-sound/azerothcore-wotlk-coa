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
    // Carries the movement-speed and SPELL_AURA_FLY effects that the tooltip of Wasp Form quotes ($805142s2).
    // Wasp Form itself only has the shapeshift, the transform and a flight speed modifier, none of which grants flight.
    SPELL_WASP_FORM_FLIGHT = 805142
};

class aura_ascension_venomancer_wasp_form : public AuraScript
{
    PrepareAuraScript(aura_ascension_venomancer_wasp_form);

    void Apply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        if (!target->HasAura(SPELL_WASP_FORM_FLIGHT))
            target->CastSpell(target, SPELL_WASP_FORM_FLIGHT, true);
    }

    void Remove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->RemoveAurasDueToSpell(SPELL_WASP_FORM_FLIGHT);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(aura_ascension_venomancer_wasp_form::Apply, EFFECT_0,
            SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(aura_ascension_venomancer_wasp_form::Remove, EFFECT_0,
            SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
    }
};
}

void AddSC_AscensionVenomancerWasp()
{
    RegisterSpellScript(aura_ascension_venomancer_wasp_form);
}
