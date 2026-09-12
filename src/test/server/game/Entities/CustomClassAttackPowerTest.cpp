/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "IntegrationTestFixture.h"

namespace
{
class CustomClassAttackPowerTest : public IntegrationTestFixture
{
protected:
    TestPlayer* CreatePlayerWithStats(Classes playerClass, uint8 level, int32 strength, int32 agility)
    {
        TestPlayer* player = CreateTestPlayer(++_guid);
        player->SetByteValue(UNIT_FIELD_BYTES_0, 1, uint8(playerClass));
        player->SetLevel(level);
        player->SetStat(STAT_STRENGTH, strength);
        player->SetStat(STAT_AGILITY, agility);
        return player;
    }

private:
    ObjectGuid::LowType _guid = 0;
};
}

TEST_F(CustomClassAttackPowerTest, BarbarianMatchesWarriorAtStartingAndMaximumLevel)
{
    struct StatsCase
    {
        uint8 level;
        int32 strength;
        int32 agility;
        int32 expectedAttackPower;
    };

    for (StatsCase const& stats : {StatsCase{1, 27, 26, 37}, StatsCase{80, 200, 300, 620}})
    {
        TestPlayer* barbarian = CreatePlayerWithStats(CLASS_BARBARIAN, stats.level, stats.strength, stats.agility);
        TestPlayer* warrior = CreatePlayerWithStats(CLASS_WARRIOR, stats.level, stats.strength, stats.agility);
        barbarian->UpdateAttackPowerAndDamage();
        warrior->UpdateAttackPowerAndDamage();

        EXPECT_EQ(barbarian->GetInt32Value(UNIT_FIELD_ATTACK_POWER), stats.expectedAttackPower);
        EXPECT_EQ(barbarian->GetInt32Value(UNIT_FIELD_ATTACK_POWER), warrior->GetInt32Value(UNIT_FIELD_ATTACK_POWER));
    }
}

TEST_F(CustomClassAttackPowerTest, BarbarianScalesWithStrengthAndLevelWithoutBaseAgilityAttackPower)
{
    TestPlayer* player = CreatePlayerWithStats(CLASS_BARBARIAN, 79, 200, 20);
    player->UpdateAttackPowerAndDamage();
    EXPECT_EQ(player->GetInt32Value(UNIT_FIELD_ATTACK_POWER), 617);

    player->SetStat(STAT_AGILITY, 700);
    player->UpdateAttackPowerAndDamage();
    EXPECT_EQ(player->GetInt32Value(UNIT_FIELD_ATTACK_POWER), 617);

    player->SetStat(STAT_STRENGTH, 210);
    player->SetLevel(80);
    player->UpdateAttackPowerAndDamage();
    EXPECT_EQ(player->GetInt32Value(UNIT_FIELD_ATTACK_POWER), 640);
}

TEST_F(CustomClassAttackPowerTest, PreservesBaseFlatAndTotalAttackPowerModifiers)
{
    for (Classes playerClass : {CLASS_BARBARIAN, CLASS_SON_OF_ARUGAL})
    {
        TestPlayer* player = CreatePlayerWithStats(playerClass, 80, 200, 300);
        player->SetStatPctModifier(UNIT_MOD_ATTACK_POWER, BASE_PCT, 1.5f);
        player->SetStatFlatModifier(UNIT_MOD_ATTACK_POWER, TOTAL_VALUE, 40.0f);
        player->SetStatPctModifier(UNIT_MOD_ATTACK_POWER, TOTAL_PCT, 1.1f);
        player->UpdateAttackPowerAndDamage();

        EXPECT_EQ(player->GetInt32Value(UNIT_FIELD_ATTACK_POWER), playerClass == CLASS_BARBARIAN ? 930 : 960);
        EXPECT_EQ(player->GetInt32Value(UNIT_FIELD_ATTACK_POWER_MODS), 40);
        EXPECT_FLOAT_EQ(player->GetTotalAttackPowerValue(BASE_ATTACK),
            playerClass == CLASS_BARBARIAN ? 1067.0f : 1100.0f);
    }
}

TEST_F(CustomClassAttackPowerTest, BloodmageMatchesRogueAtStartingAndMaximumLevel)
{
    struct StatsCase
    {
        uint8 level;
        int32 strength;
        int32 agility;
        int32 expectedAttackPower;
    };

    for (StatsCase const& stats : {StatsCase{1, 19, 20, 21}, StatsCase{80, 200, 300, 640}})
    {
        TestPlayer* bloodmage = CreatePlayerWithStats(CLASS_SON_OF_ARUGAL, stats.level, stats.strength, stats.agility);
        TestPlayer* rogue = CreatePlayerWithStats(CLASS_ROGUE, stats.level, stats.strength, stats.agility);
        bloodmage->UpdateAttackPowerAndDamage();
        rogue->UpdateAttackPowerAndDamage();

        EXPECT_EQ(bloodmage->GetInt32Value(UNIT_FIELD_ATTACK_POWER), stats.expectedAttackPower);
        EXPECT_EQ(bloodmage->GetInt32Value(UNIT_FIELD_ATTACK_POWER), rogue->GetInt32Value(UNIT_FIELD_ATTACK_POWER));
    }
}

TEST_F(CustomClassAttackPowerTest, BloodmageGainsAttackPowerFromBothStatsAndLevel)
{
    TestPlayer* player = CreatePlayerWithStats(CLASS_SON_OF_ARUGAL, 79, 200, 300);
    player->UpdateAttackPowerAndDamage();
    EXPECT_EQ(player->GetInt32Value(UNIT_FIELD_ATTACK_POWER), 638);

    player->SetStat(STAT_AGILITY, 310);
    player->UpdateAttackPowerAndDamage();
    EXPECT_EQ(player->GetInt32Value(UNIT_FIELD_ATTACK_POWER), 648);

    player->SetStat(STAT_STRENGTH, 210);
    player->SetLevel(80);
    player->UpdateAttackPowerAndDamage();
    EXPECT_EQ(player->GetInt32Value(UNIT_FIELD_ATTACK_POWER), 660);
}

TEST_F(CustomClassAttackPowerTest, PreservesRogueMeleeAttackPowerAndOtherCompatibility)
{
    TestPlayer* rogue = CreatePlayerWithStats(CLASS_ROGUE, 80, 200, 300);
    rogue->UpdateAttackPowerAndDamage();

    EXPECT_EQ(rogue->GetInt32Value(UNIT_FIELD_ATTACK_POWER), 640);
    EXPECT_EQ(GetLegacyClassForCustomClass(CLASS_BARBARIAN), CLASS_ROGUE);
    EXPECT_EQ(GetLegacyClassForCustomClass(CLASS_SON_OF_ARUGAL), CLASS_DRUID);
}
