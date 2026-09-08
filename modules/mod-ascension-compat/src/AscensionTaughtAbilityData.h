/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU
 * AGPL v3 license:
 * https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#ifndef ASCENSION_TAUGHT_ABILITY_DATA_H
#define ASCENSION_TAUGHT_ABILITY_DATA_H

#include <array>
#include <cstdint>

namespace AscensionCompatData
{
struct TaughtAbility
{
    std::uint8_t ClassId;
    std::uint32_t SpecId;
    std::uint8_t RequiredLevel;
    std::uint32_t ParentSpellId;
    std::uint32_t SpellId;
};

// Explicit teaches clauses; ordinary CoA passives cannot contain LEARN_SPELL.
inline constexpr std::array<TaughtAbility, 6> TaughtAbilities =
{{
    { 31, 59, 10, 92148, 574301 }, // Spirit Beast -> Harness Animal Spirit
    { 31, 59, 40, 573365, 573364 }, // Spirit Beast Communion -> Beastmaster's Recall
    { 31, 59, 40, 573365, 573310 }, // Spirit Beast Communion -> Spirit Stable
    { 28, 51, 30, 524834, 524835 }, // Beacon Charging -> Overcharge
    { 28, 51, 10, 92140, 524840 },  // Build Z.I.G.G.I. identity -> active ability
    { 28, 50, 10, 92141, 801384 }   // Build Mechsuit identity -> active ability
}};
}

#endif
