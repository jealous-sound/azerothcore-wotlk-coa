/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellScript.h"

namespace
{
enum BarrierSpells : uint32
{
    RockBarrier = 503630,
    BarrierModifiers = 504853,
    EarthmotherProtection = 560298
};

class aura_ascension_earthmother_protection_link : public AuraScript
{
    PrepareAuraScript(aura_ascension_earthmother_protection_link);

    bool Validate(SpellInfo const*) override
    {
        return ValidateSpellInfo({RockBarrier, BarrierModifiers, EarthmotherProtection});
    }

    bool Load() override
    {
        return GetUnitOwner()->IsPlayer() && GetUnitOwner()->getClass() == CLASS_WILDWALKER;
    }

    void Applied(AuraEffect const*, AuraEffectHandleModes)
    {
        Unit* owner = GetTarget();
        Aura* barrier = owner->GetAura(RockBarrier, owner->GetGUID());
        if (!barrier || !owner->HasAura(EarthmotherProtection))
            return;
        if (Aura* helper = owner->AddAura(BarrierModifiers, owner))
        {
            // The unused SLS record lasts eight seconds; the visible defense
            // owns the actual duration, including any duration modifiers.
            helper->SetMaxDuration(barrier->GetMaxDuration());
            helper->SetDuration(barrier->GetDuration());
        }
    }

    void Removed(AuraEffect const*, AuraEffectHandleModes)
    {
        GetTarget()->RemoveAurasDueToSpell(BarrierModifiers, GetTarget()->GetGUID());
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(aura_ascension_earthmother_protection_link::Applied,
            EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        AfterEffectRemove += AuraEffectRemoveFn(aura_ascension_earthmother_protection_link::Removed,
            EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

class primalist_barrier_metadata : public GlobalScript
{
public:
    primalist_barrier_metadata() : GlobalScript("primalist_barrier_metadata",
        {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* info) override
    {
        if (info->Id == BarrierModifiers && info->SpellFamilyName == 37)
        {
            info->AttributesCu &= ~SPELL_ATTR0_CU_FORCE_AURA_SAVING;
            info->AttributesCu |= SPELL_ATTR0_CU_AURA_CANNOT_BE_SAVED;
        }
    }
};
}

void AddSC_AscensionPrimalistBarrier()
{
    RegisterSpellScript(aura_ascension_earthmother_protection_link);
    new primalist_barrier_metadata();
}
