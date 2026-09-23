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

#include "Battleground.h"
#include "BattlegroundUtils.h"
#include "DBCStructure.h"
#include "ScriptMgr.h"
#include "ScriptDefines/AllBattlegroundScript.h"
#include "WorldMock.h"
#include "gtest/gtest.h"

class BracketLevelCapTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        ScriptRegistry<BGScript>::InitEnabledHooksIfNeeded(ALLBATTLEGROUNDHOOK_END);

        previousWorld_ = std::move(sWorld);
        worldMock_ = new ::testing::NiceMock<WorldMock>();
        ON_CALL(*worldMock_, getIntConfig(::testing::_)).WillByDefault(::testing::Return(0));
        sWorld.reset(worldMock_);
    }

    void TearDown() override
    {
        sWorld = std::move(previousWorld_);
    }

    void SetLevelCap(uint32 levelCap)
    {
        ON_CALL(*worldMock_, getIntConfig(CONFIG_MAX_PLAYER_LEVEL)).WillByDefault(::testing::Return(levelCap));
    }

    ::testing::NiceMock<WorldMock>* worldMock_ = nullptr;
    std::unique_ptr<IWorld> previousWorld_;
};

TEST_F(BracketLevelCapTest, LevelSixtyRealmPlaysTheSixtiesBracketAsSixtyToSixty)
{
    SetLevelCap(60);
    PvPDifficultyEntry bracket(489, 5, 60, 69, 0);

    Battleground bg;
    bg.SetBracket(&bracket);

    EXPECT_EQ(bg.GetMinLevel(), 60u);
    EXPECT_EQ(bg.GetMaxLevel(), 60u);
    EXPECT_TRUE(bg.isMaxLevel());
    EXPECT_EQ(GetBracketMaxLevel(&bracket), 60u);
}

TEST_F(BracketLevelCapTest, BracketsBelowTheLevelCapKeepTheirRange)
{
    SetLevelCap(60);
    PvPDifficultyEntry bracket(489, 4, 50, 59, 0);

    Battleground bg;
    bg.SetBracket(&bracket);

    EXPECT_EQ(bg.GetMinLevel(), 50u);
    EXPECT_EQ(bg.GetMaxLevel(), 59u);
}

TEST_F(BracketLevelCapTest, LevelEightyRealmKeepsTheSixtiesBracket)
{
    SetLevelCap(80);
    PvPDifficultyEntry bracket(489, 5, 60, 69, 0);

    Battleground bg;
    bg.SetBracket(&bracket);

    EXPECT_EQ(bg.GetMaxLevel(), 69u);
    EXPECT_FALSE(bg.isMaxLevel());
}
