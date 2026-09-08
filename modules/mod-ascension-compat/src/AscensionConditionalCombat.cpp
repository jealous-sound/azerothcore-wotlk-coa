/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionConditionalCombat.h"
#include "SpellInfo.h"
#include "SpellDefines.h"
#include "AscensionConditionalCombatData.h"

namespace
{
constexpr uint32 SPELL_EVIL_DOERS_BEWARE = 681328;

bool HasExpectedEvilDoersLegacy(SpellInfo const* spellInfo)
{
    SpellEffectInfo const& effect = spellInfo->Effects[EFFECT_0];
    return spellInfo->HasAttribute(SPELL_ATTR0_PASSIVE) &&
        spellInfo->TargetAuraState == AURA_STATE_HEALTHLESS_20_PERCENT &&
        effect.IsAura(SPELL_AURA_ADD_FLAT_MODIFIER) && effect.MiscValue == SPELLMOD_CRITICAL_CHANCE &&
        !effect.MiscValueB && effect.BasePoints == 9 && effect.DieSides == 1 &&
        effect.SpellClassMask == flag96(0, 0, 1024) &&
        effect.TargetA.GetTarget() == TARGET_UNIT_CASTER && !effect.TargetB.GetTarget() &&
        !spellInfo->Effects[EFFECT_2].Effect;
}
}

void ApplyAscensionConditionalCombatContracts(SpellInfo* spellInfo)
{
    if (!spellInfo)
        return;

    for (AscensionConditionalCombatRule const& rule : AscensionConditionalCombatRules)
    {
        if (spellInfo->Id != rule.SpellId || spellInfo->SpellFamilyName != rule.Family)
            continue;

        SpellEffectInfo& effect = spellInfo->Effects[rule.Index];
        if (effect.Effect != SPELL_EFFECT_APPLY_AURA || effect.ApplyAuraName != rule.OldAura ||
            effect.MiscValue != rule.OldSelector || effect.MiscValueB != rule.OldCondition ||
            effect.BasePoints != rule.BasePoints || effect.DieSides != rule.DieSides ||
            effect.SpellClassMask != flag96(rule.Mask[0], rule.Mask[1], rule.Mask[2]) ||
            effect.TargetA.GetTarget() != rule.OldTargetA || effect.TargetB.GetTarget())
            continue;

        if (spellInfo->Id == SPELL_EVIL_DOERS_BEWARE)
        {
            if (!HasExpectedEvilDoersLegacy(spellInfo))
                continue;

            // The level passive is always present. Its guarantee checks the
            // enemy's health when Stake hits; there is no unconditional +10%.
            spellInfo->TargetAuraState = 0;
            spellInfo->Effects[EFFECT_0].Effect = 0;
        }

        effect.ApplyAuraName = SPELL_AURA_OVERRIDE_CLASS_SCRIPTS;
        effect.MiscValue = rule.Selector;
        effect.MiscValueB = rule.Condition;
        effect.TargetA = SpellImplicitTargetInfo(rule.TargetA);
    }
}
