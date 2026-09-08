/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionPrimalistEarthshaping.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include <array>

namespace
{
constexpr uint32 PRIMALIST_FAMILY = uint32(CLASS_WILDWALKER) + 6;
constexpr uint32 SPELL_EARTHSHAPING = 680441;
constexpr uint32 SPELL_STONESHARD_MODIFIER = 680846;
constexpr uint32 SPELL_EARTHQUAKE_MODIFIER = 532562;
constexpr uint32 SPELL_ERUPTION_MODIFIER = 680450;
constexpr uint32 SPELL_BLESSING_OF_THERAZANE = 680439;
constexpr std::array<uint32, 3> EARTHSHAPING_HELPERS =
    {SPELL_STONESHARD_MODIFIER, SPELL_EARTHQUAKE_MODIFIER, SPELL_ERUPTION_MODIFIER};

bool IsEarthshapingOwner(Unit* owner)
{
    return owner && owner->IsPlayer() && owner->getClass() == CLASS_WILDWALKER;
}

void SynchronizeEarthshapingHelpers(Unit* owner, bool remove)
{
    if (!IsEarthshapingOwner(owner))
        return;

    Aura const* main = remove ? nullptr : owner->GetAura(SPELL_EARTHSHAPING, owner->GetGUID());
    for (uint32 spellId : EARTHSHAPING_HELPERS)
    {
        if (!main)
        {
            owner->RemoveAurasDueToSpell(spellId, owner->GetGUID());
            continue;
        }

        Aura* helper = owner->GetAura(spellId, owner->GetGUID());
        if (!helper)
            helper = owner->AddAura(spellId, owner);
        if (helper)
        {
            // SetStackAmount also recalculates Blessing's effect-2 modifier.
            // The visible aura alone owns expiry, including no-refresh gains.
            helper->SetStackAmount(main->GetStackAmount());
            helper->SetDuration(-1);
        }
    }
}

bool ValidateEarthshapingHelpers()
{
    SpellInfo const* stoneshard = sSpellMgr->GetSpellInfo(SPELL_STONESHARD_MODIFIER);
    SpellInfo const* earthquake = sSpellMgr->GetSpellInfo(SPELL_EARTHQUAKE_MODIFIER);
    SpellInfo const* eruption = sSpellMgr->GetSpellInfo(SPELL_ERUPTION_MODIFIER);
    if (!stoneshard || !earthquake || !eruption)
        return false;

    for (SpellInfo const* helper : {stoneshard, earthquake, eruption})
        if (!helper->HasAttribute(SPELL_ATTR0_CU_AURA_CANNOT_BE_SAVED) ||
            helper->HasAttribute(SPELL_ATTR0_CU_FORCE_AURA_SAVING))
            return false;

    // Refuse the old wildcard damage modifier if the metadata correction was
    // not installed. Every other effect keeps its native calculation path.
    return stoneshard->Effects[EFFECT_0].IsAura(SPELL_AURA_DUMMY) &&
        stoneshard->Effects[EFFECT_1].IsAura(SPELL_AURA_ADD_PCT_MODIFIER) &&
        stoneshard->Effects[EFFECT_1].SpellClassMask == flag96(0, 512, 0) &&
        earthquake->Effects[EFFECT_1].IsAura(SPELL_AURA_MOD_CRIT_DAMAGE_BONUS) &&
        earthquake->Effects[EFFECT_2].IsAura(SPELL_AURA_ADD_PCT_MODIFIER) &&
        earthquake->Effects[EFFECT_2].MiscValue == SPELLMOD_DOT &&
        earthquake->Effects[EFFECT_2].SpellClassMask == flag96(0, 0, 268435456) &&
        eruption->Effects[EFFECT_2].IsAura(SPELL_AURA_ADD_FLAT_MODIFIER) &&
        eruption->Effects[EFFECT_2].MiscValue == SPELLMOD_DURATION &&
        eruption->Effects[EFFECT_2].SpellClassMask == flag96(0, 0, 2097152);
}

class spell_ascension_primalist_earthshaping : public AuraScript
{
    PrepareAuraScript(spell_ascension_primalist_earthshaping);

    bool Validate(SpellInfo const* spellInfo) override
    {
        return spellInfo && spellInfo->Id == SPELL_EARTHSHAPING &&
            spellInfo->SpellFamilyName == PRIMALIST_FAMILY &&
            spellInfo->Effects[EFFECT_0].IsAura(SPELL_AURA_HASTE_SPELLS) && ValidateEarthshapingHelpers();
    }

    bool Load() override
    {
        return IsEarthshapingOwner(GetTarget()) && GetCasterGUID() == GetTarget()->GetGUID();
    }

    void Apply(AuraEffect const*, AuraEffectHandleModes)
    {
        SynchronizeEarthshapingHelpers(GetTarget(), false);
    }

    void Remove(AuraEffect const*, AuraEffectHandleModes)
    {
        SynchronizeEarthshapingHelpers(GetTarget(), true);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_ascension_primalist_earthshaping::Apply,
            EFFECT_0, SPELL_AURA_HASTE_SPELLS, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
        AfterEffectRemove += AuraEffectRemoveFn(spell_ascension_primalist_earthshaping::Remove,
            EFFECT_0, SPELL_AURA_HASTE_SPELLS, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_ascension_primalist_therazane_update : public AuraScript
{
    PrepareAuraScript(spell_ascension_primalist_therazane_update);

    bool Validate(SpellInfo const* spellInfo) override
    {
        return spellInfo && spellInfo->Id == SPELL_BLESSING_OF_THERAZANE &&
            spellInfo->SpellFamilyName == PRIMALIST_FAMILY &&
            spellInfo->Effects[EFFECT_0].IsAura(SPELL_AURA_ADD_FLAT_MODIFIER) &&
            spellInfo->Effects[EFFECT_0].MiscValue == SPELLMOD_EFFECT2 &&
            spellInfo->Effects[EFFECT_0].SpellClassMask == flag96(0, 65536, 0) &&
            ValidateEarthshapingHelpers();
    }

    bool Load() override
    {
        return IsEarthshapingOwner(GetTarget()) && GetCasterGUID() == GetTarget()->GetGUID();
    }

    void Update(AuraEffect const*, AuraEffectHandleModes)
    {
        SynchronizeEarthshapingHelpers(GetTarget(), false);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_ascension_primalist_therazane_update::Update,
            EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
        AfterEffectRemove += AuraEffectRemoveFn(spell_ascension_primalist_therazane_update::Update,
            EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
    }
};
}

void ApplyAscensionPrimalistEarthshapingContracts(SpellInfo* spellInfo)
{
    if (!spellInfo || spellInfo->SpellFamilyName != PRIMALIST_FAMILY)
        return;

    for (uint32 spellId : EARTHSHAPING_HELPERS)
        if (spellInfo->Id == spellId && spellInfo->StackAmount == 15)
            spellInfo->AttributesCu |= SPELL_ATTR0_CU_AURA_CANNOT_BE_SAVED;

    if (spellInfo->Id != SPELL_STONESHARD_MODIFIER)
        return;

    SpellEffectInfo& effect = spellInfo->Effects[EFFECT_0];
    if (effect.Effect == SPELL_EFFECT_APPLY_AURA &&
        (effect.ApplyAuraName == SPELL_AURA_ADD_PCT_MODIFIER || effect.ApplyAuraName == SPELL_AURA_DUMMY) &&
        effect.BasePoints == 1 && effect.DieSides == 1 && effect.MiscValue == SPELLMOD_DAMAGE &&
        effect.SpellClassMask == flag96(0, 0, 0))
        effect.ApplyAuraName = SPELL_AURA_DUMMY;
}

void AddSC_AscensionPrimalistEarthshaping()
{
    RegisterSpellScript(spell_ascension_primalist_earthshaping);
    RegisterSpellScript(spell_ascension_primalist_therazane_update);
}
