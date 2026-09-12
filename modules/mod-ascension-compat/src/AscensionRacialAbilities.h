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

    // Only default racial grants, using the authored CoA variants rather than legacy-class fallbacks.
    return ability.AcquireMethod == SKILL_LINE_ABILITY_LEARNED_ON_SKILL_LEARN &&
        ability.MinSkillLineRank <= 1 && !ability.SupercededBySpell &&
        (!ability.RaceMask || (ability.RaceMask & (uint32(1) << (raceId - 1)))) &&
        (!ability.ClassMask || (ability.ClassMask & (uint32(1) << (classId - 1))));
}
}

#endif
