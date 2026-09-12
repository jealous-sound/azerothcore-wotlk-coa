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
constexpr uint32 InnerDemonSpell = 804216;
constexpr ShapeshiftForm InnerDemonForm = ShapeshiftForm(56);
constexpr uint32 MetamorphosisSpell = 47241;
constexpr uint32 MetamorphosisModel = 25277;

class ShapeshiftModelTest : public IntegrationTestFixture
{
protected:
    void SetUp() override
    {
        IntegrationTestFixture::SetUp();

        // Mirror the copied DBC: Inner Demon has no body model, Metamorphosis does.
        for (ShapeshiftForm form : {InnerDemonForm, FORM_METAMORPHOSIS})
            if (!sSpellShapeshiftFormStore.LookupEntry(form))
            {
                auto* entry = new SpellShapeshiftFormEntry{};
                entry->ID = form;
                entry->modelID_A = form == FORM_METAMORPHOSIS ? MetamorphosisModel : 0;
                sSpellShapeshiftFormStore.SetEntry(form, entry);
                _addedForms.push_back(form);
            }

        for (Races race : {RACE_HUMAN, RACE_ORC})
            if (!sChrRacesStore.LookupEntry(race))
            {
                auto* entry = new ChrRacesEntry{};
                entry->RaceID = race;
                entry->TeamID = race == RACE_HUMAN ? 7 : 1;
                sChrRacesStore.SetEntry(race, entry);
                _addedRaces.push_back(race);
            }
    }

    void TearDown() override
    {
        IntegrationTestFixture::TearDown();
        for (uint32 form : _addedForms)
            sSpellShapeshiftFormStore.SetEntry(form, nullptr);
        for (uint32 race : _addedRaces)
            sChrRacesStore.SetEntry(race, nullptr);
    }

    TestPlayer* CreatePlayer(Classes playerClass, Races race, Gender gender)
    {
        TestPlayer* player = CreateTestPlayer(++_guid);
        player->SetByteValue(UNIT_FIELD_BYTES_0, 0, uint8(race));
        player->SetByteValue(UNIT_FIELD_BYTES_0, 1, uint8(playerClass));
        player->SetByteValue(PLAYER_BYTES_3, PLAYER_BYTES_3_OFFSET_GENDER, uint8(gender));
        return player;
    }

private:
    ObjectGuid::LowType _guid = 0;
    std::vector<uint32> _addedForms;
    std::vector<uint32> _addedRaces;
};
}

TEST_F(ShapeshiftModelTest, InnerDemonDoesNotReplaceTheBodyForEitherRaceOrGender)
{
    for (Races race : {RACE_HUMAN, RACE_ORC})
        for (Gender gender : {GENDER_MALE, GENDER_FEMALE})
        {
            TestPlayer* player = CreatePlayer(CLASS_DEMON_HUNTER, race, gender);
            // Zero tells the shapeshift aura handler to leave the current display intact.
            EXPECT_EQ(player->GetModelForForm(InnerDemonForm, InnerDemonSpell), 0u);
        }
}

TEST_F(ShapeshiftModelTest, WarlockMetamorphosisStillUsesItsDemonModel)
{
    for (Races race : {RACE_HUMAN, RACE_ORC})
        for (Gender gender : {GENDER_MALE, GENDER_FEMALE})
        {
            TestPlayer* player = CreatePlayer(CLASS_WARLOCK, race, gender);
            EXPECT_EQ(player->GetModelForForm(FORM_METAMORPHOSIS, MetamorphosisSpell), MetamorphosisModel);
        }
}
