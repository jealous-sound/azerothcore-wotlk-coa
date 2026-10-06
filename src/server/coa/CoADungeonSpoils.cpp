/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "Item.h"
#include "ItemScript.h"
#include "LootMgr.h"
#include "Player.h"

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

void AddSC_CoADungeonSpoils()
{
    new item_coa_dungeon_spoils();
}