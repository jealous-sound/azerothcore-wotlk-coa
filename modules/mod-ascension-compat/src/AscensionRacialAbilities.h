/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#ifndef ASCENSION_RACIAL_ABILITIES_H
#define ASCENSION_RACIAL_ABILITIES_H

#include "DBCStructure.h"
#include "SharedDefines.h"
#include <array>

namespace AscensionRacialAbilities
{
enum AdditionalRacialSkills
{
    SKILL_ORC_RACIAL_LEGACY = 11125,
    SKILL_DRAENEI_RACIAL_COA = 11760
};

enum RacialSpells
{
    SPELL_ARCANE_TORRENT_ALL_RESOURCES = 28730,
    SPELL_GIFT_OF_THE_NAARU_HYBRID = 814282
};

struct RacialSkill
{
    uint8 RaceId;
    uint32 SkillId;
};

// The copied SkillLineAbility DBC splits Orc and Draenei racials across two skill lines.
inline constexpr std::array<RacialSkill, 12> Skills =
{{
    {RACE_HUMAN, SKILL_RACIAL_HUMAN},
    {RACE_ORC, SKILL_ORC_RACIAL},
    {RACE_ORC, SKILL_ORC_RACIAL_LEGACY},
    {RACE_DWARF, SKILL_RACIAL_DWARVEN},
    {RACE_NIGHTELF, SKILL_RACIAL_NIGHT_ELF},
    {RACE_UNDEAD_PLAYER, SKILL_RACIAL_UNDED},
    {RACE_TAUREN, SKILL_RACIAL_TAUREN},
    {RACE_GNOME, SKILL_RACIAL_GNOME},
    {RACE_TROLL, SKILL_RACIAL_TROLL},
    {RACE_BLOODELF, SKILL_RACIAL_BLOODELF},
    {RACE_DRAENEI, SKILL_RACIAL_DRAENEI},
    {RACE_DRAENEI, SKILL_DRAENEI_RACIAL_COA}
}};

constexpr uint8 GetRace(uint32 skillId)
{
    for (RacialSkill const& skill : Skills)
        if (skill.SkillId == skillId)
            return skill.RaceId;
    return RACE_NONE;
}

inline bool CanLearn(SkillLineAbilityEntry const& ability, uint8 raceId, uint8 classId)
{
    if (!raceId || GetRace(ability.SkillLine) != raceId || !IsAscensionClass(classId))
        return false;

    // Witch Hunter was excluded from Blood Elf class masks. The existing multi-resource
    // Torrent restores its Mana and Rage; none of the single-resource CoA variants covers both.
    bool const witchHunterTorrent = classId == CLASS_WITCH_HUNTER &&
        ability.SkillLine == SKILL_RACIAL_BLOODELF && ability.Spell == SPELL_ARCANE_TORRENT_ALL_RESOURCES;
    // Sun Cleric was excluded from every Draenei Gift of the Naaru class mask. It uses both spell power and
    // attack power, so it receives the hybrid variant the other hybrid classes already have.
    bool const sunClericGift = classId == CLASS_SUN_CLERIC && ability.SkillLine == SKILL_DRAENEI_RACIAL_COA &&
        ability.Spell == SPELL_GIFT_OF_THE_NAARU_HYBRID;
    // Preserve the authored variants for every other race/class combination.
    return ability.AcquireMethod == SKILL_LINE_ABILITY_LEARNED_ON_SKILL_LEARN &&
        ability.MinSkillLineRank <= 1 && !ability.SupercededBySpell &&
        (!ability.RaceMask || (ability.RaceMask & (uint32(1) << (raceId - 1)))) &&
        (witchHunterTorrent || sunClericGift || !ability.ClassMask || (ability.ClassMask & (uint32(1) << (classId - 1))));
}
}

#endif
