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

#include "LFGMgr.h"
#include "gtest/gtest.h"

namespace
{
lfg::LFGDungeonData MakeDungeon(uint32 id, uint8 type, uint8 expansion, uint8 minLevel, uint8 maxLevel)
{
    lfg::LFGDungeonData dungeon;
    dungeon.id = id;
    dungeon.type = type;
    dungeon.expansion = expansion;
    dungeon.minlevel = minLevel;
    dungeon.maxlevel = maxLevel;
    return dungeon;
}

lfg::LFGDungeonContainer MakeDungeons()
{
    lfg::LFGDungeonContainer dungeons;
    dungeons[258] = MakeDungeon(258, lfg::LFG_TYPE_RANDOM, EXPANSION_CLASSIC, 15, 59);
    dungeons[1258] = MakeDungeon(1258, lfg::LFG_TYPE_RANDOM, EXPANSION_CLASSIC, 60, 60);
    dungeons[417] = MakeDungeon(417, lfg::LFG_TYPE_RANDOM, EXPANSION_THE_BURNING_CRUSADE, 58, 70);
    dungeons[465] = MakeDungeon(465, lfg::LFG_TYPE_RANDOM, EXPANSION_WRATH_OF_THE_LICH_KING, 70, 79);
    dungeons[6] = MakeDungeon(6, lfg::LFG_TYPE_DUNGEON, EXPANSION_CLASSIC, 15, 59);
    return dungeons;
}
}

TEST(LevelCapDungeonsTest, LevelCapSelectsTheExpansionItsDungeonsBelongTo)
{
    EXPECT_EQ(lfg::LFGMgr::GetExpansionForLevelCap(60), EXPANSION_CLASSIC);
    EXPECT_EQ(lfg::LFGMgr::GetExpansionForLevelCap(70), EXPANSION_THE_BURNING_CRUSADE);
    EXPECT_EQ(lfg::LFGMgr::GetExpansionForLevelCap(80), EXPANSION_WRATH_OF_THE_LICH_KING);
}

TEST(LevelCapDungeonsTest, RandomClassicDungeonReachesLevelSixtyOnALevelSixtyRealm)
{
    lfg::LFGDungeonContainer dungeons = MakeDungeons();

    lfg::LFGMgr::ExtendRandomDungeonsToLevelCap(dungeons, 60);

    EXPECT_EQ(dungeons[258].maxlevel, 60);
    EXPECT_EQ(dungeons[1258].maxlevel, 60);
    EXPECT_EQ(dungeons[417].maxlevel, 70);
    EXPECT_EQ(dungeons[465].maxlevel, 79);
    EXPECT_EQ(dungeons[6].maxlevel, 59);
}

TEST(LevelCapDungeonsTest, LevelEightyRealmKeepsDungeonFinderLevels)
{
    lfg::LFGDungeonContainer dungeons = MakeDungeons();

    lfg::LFGMgr::ExtendRandomDungeonsToLevelCap(dungeons, 80);

    EXPECT_EQ(dungeons[258].maxlevel, 59);
    EXPECT_EQ(dungeons[465].maxlevel, 79);
}
