/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionStormbringer.h"
#include "SpellDefines.h"

namespace
{
enum StormbringerContractSpells : uint32
{
    SPELL_CLOUDSURFER = 806414,
    SPELL_FLOW_OF_WRATH = 801855
};

constexpr uint32 SHOCK_FAMILY_MASK = 2048;
constexpr uint32 BRINE_FAMILY_MASK = 128;
constexpr uint32 DROWN_FAMILY_MASK = 0x08000000;
}

namespace AscensionStormbringer
{
void ApplyContracts(SpellInfo* info)
{
    if (!info || info->SpellFamilyName != 22)
        return;
    if (info->Id == SPELL_CLOUDSURFER && info->Effects[EFFECT_0].ApplyAuraName == SPELL_AURA_ADD_PCT_MODIFIER &&
        info->Effects[EFFECT_0].MiscValue == SPELLMOD_DAMAGE &&
        info->Effects[EFFECT_0].SpellClassMask == flag96(0, BRINE_FAMILY_MASK, 0))
        info->Effects[EFFECT_0].SpellClassMask = flag96(SHOCK_FAMILY_MASK, BRINE_FAMILY_MASK, 0);
    if (info->Id == SPELL_FLOW_OF_WRATH && info->Effects[EFFECT_0].ApplyAuraName == SPELL_AURA_ADD_FLAT_MODIFIER &&
        info->Effects[EFFECT_0].MiscValue == SPELLMOD_EFFECT2 &&
        info->Effects[EFFECT_0].SpellClassMask == flag96(0, 0, DROWN_FAMILY_MASK))
        info->Effects[EFFECT_0].SpellClassMask = flag96(0, DROWN_FAMILY_MASK, 0);
}
}
