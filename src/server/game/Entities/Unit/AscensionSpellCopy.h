/*
 * CoA repack: vanilla classes learn Ascension's Bronzebeard spells, which are copies of the stock WotLK spells
 * at id + 1100000. Class scripts look up auras and spells by their stock ids (HasAura(SPELL_DRUID_...),
 * GetAuraOfRankedSpell, HasSpell...), so a lookup of a stock id that finds nothing retries with the copy.
 */

#ifndef ASCENSION_SPELL_COPY_H
#define ASCENSION_SPELL_COPY_H

#include "Define.h"

constexpr uint32 ASCENSION_BRONZEBEARD_SPELL_OFFSET = 1100000;

/// Bronzebeard copy of a stock spell id, or 0 when the id is not a stock spell.
inline uint32 GetAscensionSpellCopy(uint32 spellId)
{
    return (spellId && spellId < 100000) ? spellId + ASCENSION_BRONZEBEARD_SPELL_OFFSET : 0;
}

#endif
