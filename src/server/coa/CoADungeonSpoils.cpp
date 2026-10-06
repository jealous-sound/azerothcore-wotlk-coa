/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "DungeonHealth.h"
#include "GlobalScript.h"
#include "Item.h"
#include "ItemScript.h"
#include "LootMgr.h"
#include "Map.h"
#include "Player.h"

namespace
{
    constexpr uint32 MarkOfTriumph = 1414502;
}

class item_coa_dungeon_spoils final : public ItemScript
{
public:
    item_coa_dungeon_spoils() : ItemScript("item_coa_dungeon_spoils") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const&) override
    {
        if (!player->IsAlive() || player->IsInCombat() || !item->GetTemplate()->HasFlag(ITEM_FLAG_HAS_LOOT))
            return false;
        player->SendLoot(item->GetGUID(), LOOT_CORPSE);
        return true;
    }
};

class CoADungeonFinalBossMark final : public GlobalScript
{
public:
    CoADungeonFinalBossMark() : GlobalScript("CoADungeonFinalBossMark", { GLOBALHOOK_ON_AFTER_UPDATE_ENCOUNTER_STATE }) { }

    void OnAfterUpdateEncounterState(Map* map, EncounterCreditType, uint32, Unit*, Difficulty,
        std::list<DungeonEncounter const*> const*, uint32 dungeonCompleted, bool) override
    {
        if (!dungeonCompleted || !map || !map->IsNonRaidDungeon() || !DungeonHealth::IsVanillaDungeon(map->GetId()))
            return;
        if (map->GetSpawnMode() != 1 && map->GetSpawnMode() != 2)
            return;
        map->DoForAllPlayers([](Player* player) { player->AddItem(MarkOfTriumph, 1); });
    }
};

void AddSC_CoADungeonSpoils()
{
    new item_coa_dungeon_spoils();
    new CoADungeonFinalBossMark();
}
