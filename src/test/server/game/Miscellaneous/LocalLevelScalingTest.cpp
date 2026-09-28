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

#include "LocalLevelScaling.h"
#include "gtest/gtest.h"

using LocalLevelScaling::ScaleCreatureLevelForViewer;
using LocalLevelScaling::ScaleDungeonCreatureLevelForViewer;

TEST(LocalLevelScalingTest, OpenWorldCreatureAboveViewerKeepsItsLevel)
{
    EXPECT_EQ(ScaleCreatureLevelForViewer(38, 20, 3), 38);
}

TEST(LocalLevelScalingTest, CathedralCreatureComesDownToLowLevelViewer)
{
    EXPECT_EQ(ScaleDungeonCreatureLevelForViewer(36, 20, 3), 23);
    EXPECT_EQ(ScaleDungeonCreatureLevelForViewer(38, 22, 3), 25);
    EXPECT_EQ(ScaleDungeonCreatureLevelForViewer(38, 33, 3), 36);
}

TEST(LocalLevelScalingTest, DungeonCreatureInsideViewerBandKeepsItsLevel)
{
    EXPECT_EQ(ScaleDungeonCreatureLevelForViewer(34, 33, 3), 34);
    EXPECT_EQ(ScaleDungeonCreatureLevelForViewer(30, 33, 3), 30);
}

TEST(LocalLevelScalingTest, DungeonCreatureBelowViewerIsStillLifted)
{
    EXPECT_EQ(ScaleDungeonCreatureLevelForViewer(18, 40, 3), 37);
}

TEST(LocalLevelScalingTest, DungeonCeilingFollowsTheOffset)
{
    EXPECT_EQ(ScaleDungeonCreatureLevelForViewer(40, 20, 0), 20);
    EXPECT_EQ(ScaleDungeonCreatureLevelForViewer(40, 20, 5), 25);
    EXPECT_EQ(ScaleDungeonCreatureLevelForViewer(63, 60, 3), 63);
}
