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
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Totem.h"

namespace
{
// Exercise the real Totem::InitStats without loading world database templates.
class TestTotem : public Totem
{
public:
    TestTotem(SummonPropertiesEntry const* properties, Player* owner, uint32 entry, uint32 type,
        uint32 displayId, float scale) : Totem(properties, owner->GetGUID())
    {
        Object::_Create(1, entry, HighGuid::Unit);
        SetEntry(entry);
        m_originalEntry = entry;
        _template.Entry = entry;
        _template.type = type;
        m_creatureInfo = &_template;
        SetMap(owner->GetMap());
        SetNativeDisplayId(displayId);
        SetDisplayId(displayId, scale);
    }

private:
    CreatureTemplate _template{};
};

class TotemDisplayTest : public IntegrationTestFixture
{
protected:
    void CheckTemplateDisplay(Races race, uint32 entry, uint32 type, uint32 displayId, float scale)
    {
        TestPlayer* owner = CreateTestPlayer(++_guid);
        owner->SetByteValue(UNIT_FIELD_BYTES_0, 0, uint8(race));
        owner->SetByteValue(UNIT_FIELD_BYTES_0, 1, CLASS_CULTIST);
        owner->SetLevel(10);
        owner->SetFaction(TEST_FACTION_HOSTILE_TO_MONSTERS);
        HashMapHolder<Player>::Insert(owner);

        // SummonProperties.dbc 63, used by Cultist Old God tentacles.
        SummonPropertiesEntry properties{63, SUMMON_CATEGORY_ALLY, 0, SUMMON_TYPE_TOTEM, SUMMON_SLOT_TOTEM_FIRE, 2};
        TestTotem summon(&properties, owner, entry, type, displayId, scale);
        summon.InitStats(30000);

        EXPECT_EQ(summon.GetDisplayId(), displayId);
        EXPECT_EQ(summon.GetNativeDisplayId(), displayId);
        EXPECT_FLOAT_EQ(summon.GetObjectScale(), scale);
        EXPECT_EQ(summon.GetLevel(), owner->GetLevel());
        EXPECT_EQ(summon.GetOwnerGUID(), owner->GetGUID());
        EXPECT_EQ(summon.GetTotemDuration(), 30000u);
        EXPECT_EQ(owner->m_SummonSlot[SUMMON_SLOT_TOTEM_FIRE], summon.GetGUID());

        owner->m_SummonSlot[SUMMON_SLOT_TOTEM_FIRE].Clear();
        owner->SetMinion(&summon, false);
        HashMapHolder<Player>::Remove(owner);
    }

private:
    ObjectGuid::LowType _guid = 0;
};
}

TEST_F(TotemDisplayTest, CultistTentaclesKeepTheirTemplateModelsAndScales)
{
    for (Races race : {RACE_HUMAN, RACE_ORC})
    {
        CheckTemplateDisplay(race, 50272, CREATURE_TYPE_ELEMENTAL, 15788, 0.7f);
        CheckTemplateDisplay(race, 501464, CREATURE_TYPE_ELEMENTAL, 28813, 0.5f);
        CheckTemplateDisplay(race, 500465, CREATURE_TYPE_ELEMENTAL, 28815, 0.5f);
        CheckTemplateDisplay(race, 50096, CREATURE_TYPE_ELEMENTAL, 28814, 0.5f);
    }
}

TEST_F(TotemDisplayTest, MissingRacialTotemModelKeepsValidTemplateDisplay)
{
    EXPECT_EQ(sObjectMgr->GetModelForTotem(SUMMON_SLOT_TOTEM_FIRE, RACE_HUMAN), 0u);
    CheckTemplateDisplay(RACE_HUMAN, 15439, CREATURE_TYPE_TOTEM, 19074, 1.0f);
}
