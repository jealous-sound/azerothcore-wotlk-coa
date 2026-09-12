/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "../../../../../modules/mod-ascension-compat/src/AscensionRacialAbilities.h"
#include "gtest/gtest.h"

namespace
{
SkillLineAbilityEntry RacialAbility(uint32 skillId, uint32 raceMask = 0, uint32 classMask = 0)
{
    SkillLineAbilityEntry ability{};
    ability.SkillLine = skillId;
    ability.RaceMask = raceMask;
    ability.ClassMask = classMask;
    ability.MinSkillLineRank = 1;
    ability.AcquireMethod = SKILL_LINE_ABILITY_LEARNED_ON_SKILL_LEARN;
    return ability;
}
}

TEST(AscensionRacialAbilitiesTest, CoversAllTenRacesAndTwentyOneCustomClasses)
{
    constexpr uint32 racialSkills[] = {754, 125, 101, 126, 220, 124, 753, 733, 756, 760};
    constexpr uint8 races[] = {1, 2, 3, 4, 5, 6, 7, 8, 10, 11};
    for (uint8 index = 0; index < 10; ++index)
    {
        auto ability = RacialAbility(racialSkills[index], uint32(1) << (races[index] - 1), 0xFFFFFA00u);
        EXPECT_EQ(AscensionRacialAbilities::GetRace(racialSkills[index]), races[index]);
        for (uint8 classId = CLASS_BARBARIAN; classId < MAX_CLASSES; ++classId)
            EXPECT_TRUE(AscensionRacialAbilities::CanLearn(ability, races[index], classId));
    }
}

TEST(AscensionRacialAbilitiesTest, RaceComesFromSkillEvenWhenAbilityMaskIsUnrestricted)
{
    // Gemcutting and Blood Elf passives have RaceMask = 0 in the copied DBC.
    auto gemcutting = RacialAbility(SKILL_RACIAL_DRAENEI, 0, 0xFFFFFA00u);
    EXPECT_TRUE(AscensionRacialAbilities::CanLearn(gemcutting, RACE_DRAENEI, CLASS_FLESHWARDEN));
    EXPECT_FALSE(AscensionRacialAbilities::CanLearn(gemcutting, RACE_HUMAN, CLASS_FLESHWARDEN));
    auto magicResistance = RacialAbility(SKILL_RACIAL_BLOODELF, 0, 0xFFFFFA00u);
    EXPECT_TRUE(AscensionRacialAbilities::CanLearn(magicResistance, RACE_BLOODELF, CLASS_NECROMANCER));
    EXPECT_FALSE(AscensionRacialAbilities::CanLearn(magicResistance, RACE_ORC, CLASS_NECROMANCER));
}

TEST(AscensionRacialAbilitiesTest, DraeneiCustomAbilitiesUseTheirAdditionalSkillLine)
{
    auto gift = RacialAbility(11760, 1024, 3909427200u); // Gift of the Naaru 814282.
    EXPECT_TRUE(AscensionRacialAbilities::CanLearn(gift, RACE_DRAENEI, CLASS_FLESHWARDEN));
    EXPECT_FALSE(AscensionRacialAbilities::CanLearn(gift, RACE_DRAENEI, CLASS_GUARDIAN));
    EXPECT_FALSE(AscensionRacialAbilities::CanLearn(gift, RACE_HUMAN, CLASS_FLESHWARDEN));
}

TEST(AscensionRacialAbilitiesTest, PreservesAuthoredResourceVariants)
{
    auto manaTorrent = RacialAbility(SKILL_RACIAL_BLOODELF, 512, 2376105984u);
    auto necromancerTorrent = RacialAbility(SKILL_RACIAL_BLOODELF, 512, 4194304);
    auto energyTorrent = RacialAbility(SKILL_RACIAL_BLOODELF, 512, 401408);
    EXPECT_TRUE(AscensionRacialAbilities::CanLearn(manaTorrent, RACE_BLOODELF, CLASS_SPIRIT_MAGE));
    EXPECT_FALSE(AscensionRacialAbilities::CanLearn(manaTorrent, RACE_BLOODELF, CLASS_NECROMANCER));
    EXPECT_TRUE(AscensionRacialAbilities::CanLearn(necromancerTorrent, RACE_BLOODELF, CLASS_NECROMANCER));
    EXPECT_FALSE(AscensionRacialAbilities::CanLearn(energyTorrent, RACE_BLOODELF, CLASS_NECROMANCER));
    auto attackPowerFury = RacialAbility(SKILL_ORC_RACIAL, 2, 1181696);
    auto spellPowerFury = RacialAbility(SKILL_ORC_RACIAL, 2, 12619776);
    EXPECT_TRUE(AscensionRacialAbilities::CanLearn(attackPowerFury, RACE_ORC, CLASS_BARBARIAN));
    EXPECT_FALSE(AscensionRacialAbilities::CanLearn(spellPowerFury, RACE_ORC, CLASS_BARBARIAN));
    EXPECT_TRUE(AscensionRacialAbilities::CanLearn(spellPowerFury, RACE_ORC, CLASS_NECROMANCER));
}

TEST(AscensionRacialAbilitiesTest, RejectsUnrelatedSkillsClassesAndNonDefaultAbilities)
{
    auto ability = RacialAbility(SKILL_RACIAL_HUMAN, 1, 0xFFFFFA00u);
    EXPECT_FALSE(AscensionRacialAbilities::CanLearn(ability, RACE_HUMAN, CLASS_WARRIOR));
    EXPECT_FALSE(AscensionRacialAbilities::CanLearn(ability, RACE_NONE, CLASS_BARBARIAN));
    EXPECT_FALSE(AscensionRacialAbilities::CanLearn(ability, RACE_HUMAN, MAX_CLASSES));
    ability.SkillLine = SKILL_SWORDS;
    EXPECT_FALSE(AscensionRacialAbilities::CanLearn(ability, RACE_HUMAN, CLASS_BARBARIAN));
    ability.SkillLine = SKILL_RACIAL_HUMAN;
    ability.AcquireMethod = SKILL_LINE_ABILITY_LEARNED_ON_SKILL_VALUE;
    EXPECT_FALSE(AscensionRacialAbilities::CanLearn(ability, RACE_HUMAN, CLASS_BARBARIAN));
    ability.AcquireMethod = SKILL_LINE_ABILITY_LEARNED_ON_SKILL_LEARN;
    ability.MinSkillLineRank = 2;
    EXPECT_FALSE(AscensionRacialAbilities::CanLearn(ability, RACE_HUMAN, CLASS_BARBARIAN));
    ability.MinSkillLineRank = 1;
    ability.SupercededBySpell = 123;
    EXPECT_FALSE(AscensionRacialAbilities::CanLearn(ability, RACE_HUMAN, CLASS_BARBARIAN));
}
