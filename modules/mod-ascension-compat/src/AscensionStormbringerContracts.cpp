/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionStormbringer.h"
#include "SpellDefines.h"

namespace
{
enum StormbringerContractSpells : uint32
{
    SPELL_CLOUDSURFER = 806414
};

constexpr uint32 SHOCK_FAMILY_MASK = 2048;
constexpr uint32 BRINE_FAMILY_MASK = 128;
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
}
}
