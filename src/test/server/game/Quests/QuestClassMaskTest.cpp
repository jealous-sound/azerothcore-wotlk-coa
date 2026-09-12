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

#include "SharedDefines.h"
#include "gtest/gtest.h"

TEST(QuestClassMaskTest, LegacyClassQuestsAllowMatchingCustomClasses)
{
    // Quest 397 uses the Warlock mask; Necromancers share that class family.
    EXPECT_EQ(ExpandLegacyClassMask(256), 256u | (uint32(1) << (CLASS_NECROMANCER - 1)));

    // Quest 63 uses the Shaman mask, including Runemaster's unsigned class-32 bit after expansion.
    uint32 shamanFamily = 64u | (uint32(1) << (CLASS_WITCH_DOCTOR - 1)) |
        (uint32(1) << (CLASS_STORMBRINGER - 1)) | (uint32(1) << (CLASS_PROPHET - 1)) |
        (uint32(1) << (CLASS_SPIRIT_MAGE - 1));
    EXPECT_EQ(ExpandLegacyClassMask(64), shamanFamily);

    // There is no custom class mapped to Death Knight.
    EXPECT_EQ(ExpandLegacyClassMask(32), 32u);
}

TEST(QuestClassMaskTest, MultiClassQuestsKeepAllAndOnlyTheirClassFamilies)
{
    // Quest 9238 permits Priest, Mage and Warlock (400).
    uint32 expected = 400u | (uint32(1) << (CLASS_CHRONOMANCER - 1)) |
        (uint32(1) << (CLASS_SUN_CLERIC - 1)) | (uint32(1) << (CLASS_PYROMANCER - 1)) |
        (uint32(1) << (CLASS_NECROMANCER - 1));
    EXPECT_EQ(ExpandLegacyClassMask(400), expected);
}

TEST(QuestClassMaskTest, UnrestrictedQuestsStayUnrestricted)
{
    // SatisfyQuestClass treats zero as no class restriction.
    EXPECT_EQ(ExpandLegacyClassMask(0), 0u);
    EXPECT_EQ(ExpandLegacyClassMask(1535), CLASSMASK_ALL_PLAYABLE);
    EXPECT_EQ(ExpandLegacyClassMask(CLASSMASK_ALL_PLAYABLE), CLASSMASK_ALL_PLAYABLE);
    EXPECT_EQ(ExpandLegacyClassMask(uint32(-1)), uint32(-1));
}

TEST(QuestClassMaskTest, ExplicitCustomQuestMasksStayUnchanged)
{
    uint32 necromancer = uint32(1) << (CLASS_NECROMANCER - 1);
    uint32 runemaster = uint32(1) << (CLASS_SPIRIT_MAGE - 1);
    EXPECT_EQ(ExpandLegacyClassMask(necromancer), necromancer);
    EXPECT_EQ(ExpandLegacyClassMask(runemaster), runemaster);
    EXPECT_EQ(ExpandLegacyClassMask(necromancer | 64u), necromancer | 64u);
}
