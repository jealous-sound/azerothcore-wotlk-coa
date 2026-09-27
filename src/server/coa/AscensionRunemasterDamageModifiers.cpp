/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionRunemasterDamageModifiers.h"
#include "SpellInfo.h"
#include "SpellDefines.h"

namespace
{
constexpr uint32 SPELL_ELEMENTAL_ACUITY = 706671;
constexpr uint32 SPELL_AIR_ENGRAVING = 653223;
constexpr int32 PRIVATE_ATTACK_POWER_COEFFICIENT = 32;

void ApplyElementalAcuityRunebladeScaling(SpellInfo* info)
{
    flag96 const runebladeFamilyFlags(0, 0, 262144);
    SpellEffectInfo& scaling = info->Effects[EFFECT_1];
    if (info->Id != SPELL_ELEMENTAL_ACUITY || !scaling.IsAura(SPELL_AURA_ADD_FLAT_MODIFIER) ||
        scaling.MiscValue != PRIVATE_ATTACK_POWER_COEFFICIENT || scaling.SpellClassMask != runebladeFamilyFlags ||
        scaling.BasePoints != 24 || scaling.DieSides != 1 || scaling.TargetA.GetTarget() != TARGET_UNIT_CASTER)
        return;

    scaling.ApplyAuraName = SPELL_AURA_OVERRIDE_CLASS_SCRIPTS;
    scaling.MiscValue = ASCENSION_DIRECT_AP_COEFFICIENT_FLAT;
}

void ApplyAirEngravingHurricaneScope(SpellInfo* info)
{
    flag96 const hurricaneStrikeFamilyFlags(0, 0, 4);
    SpellEffectInfo& hurricane = info->Effects[EFFECT_1];
    if (info->Id != SPELL_AIR_ENGRAVING || !hurricane.IsAura(SPELL_AURA_ADD_PCT_MODIFIER) ||
        hurricane.MiscValue != SPELLMOD_DAMAGE || hurricane.SpellClassMask != flag96() ||
        hurricane.BasePoints != -1 || hurricane.DieSides != 1 || hurricane.TargetA.GetTarget() != TARGET_UNIT_CASTER)
        return;

    hurricane.SpellClassMask = hurricaneStrikeFamilyFlags;
}
}

void ApplyAscensionRunemasterDamageModifierContracts(SpellInfo* info)
{
    if (info && info->SpellFamilyName == uint32(CLASS_SPIRIT_MAGE) + 6)
    {
        ApplyElementalAcuityRunebladeScaling(info);
        ApplyAirEngravingHurricaneScope(info);
    }

    if (!info || info->SpellFamilyName != uint32(CLASS_SPIRIT_MAGE) + 6 ||
        info->SpellFamilyFlags != flag96() || info->Effects[EFFECT_2].Effect)
        return;

    SpellEffectInfo& damage = info->Effects[EFFECT_0];
    if (damage.Effect != SPELL_EFFECT_APPLY_AURA || damage.ApplyAuraName != SPELL_AURA_ADD_PCT_MODIFIER ||
        damage.MiscValue != SPELLMOD_DAMAGE || damage.DieSides != 1)
        return;

    if (info->Id == 807172 && damage.BasePoints == 19 && damage.SpellClassMask == flag96(0, 4096, 262144) &&
        !info->Effects[EFFECT_1].Effect)
    {
        damage.SpellClassMask = flag96(32, 4096, 262144);
    }
    else if (info->Id == 807379 && damage.BasePoints == 29 && damage.SpellClassMask == flag96(134348800, 4096, 0))
    {
        SpellEffectInfo const& periodic = info->Effects[EFFECT_1];
        if (periodic.Effect == SPELL_EFFECT_APPLY_AURA && periodic.ApplyAuraName == SPELL_AURA_ADD_PCT_MODIFIER &&
            periodic.MiscValue == SPELLMOD_DOT && periodic.BasePoints == 29 && periodic.DieSides == 1 &&
            periodic.SpellClassMask == flag96(134348800, 4096, 262144))
            damage.SpellClassMask = flag96(134348832, 4096, 0);
    }
}
