/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionRunemasterDamageModifiers.h"
#include "SpellInfo.h"
#include "SpellDefines.h"

void ApplyAscensionRunemasterDamageModifierContracts(SpellInfo* info)
{
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
        // E142 forwards effect amounts before DAMAGE modifiers. Admit the one
        // current family38/flag32 child so native damage scaling runs once.
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
