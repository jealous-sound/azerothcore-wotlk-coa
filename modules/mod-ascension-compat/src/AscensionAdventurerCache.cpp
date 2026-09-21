/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "Item.h"
#include "ItemScript.h"
#include "LootMgr.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include <algorithm>
#include <array>
#include <vector>

namespace
{
enum CacheItems : uint32
{
    AdventurerSatchel = 1397884,
    AdventurerCache = 1397885,
    AdventurerRareCache = 1397886
};

bool IsAdventurerReward(uint32 entry)
{
    return entry == AdventurerSatchel || entry == AdventurerCache || entry == AdventurerRareCache;
}

enum RewardKind : uint8
{
    Food,
    Potion,
    Material,
    Armor,
    RewardKindCount
};

void OpenCache(Player* player, Item* item)
{
    player->SendEquipError(EQUIP_ERR_NONE, item, nullptr);
    if (!player->IsAlive() || player->IsInCombat())
        return;

    Loot loot;
    loot.containerGUID = item->GetGUID();
    loot.FillLoot(item->GetEntry(), LootTemplates_Item, player, true, true);

    std::vector<LootItem const*> rewards;
    ItemPosCountVec reserved;
    uint32 const slots = loot.GetMaxSlotInLootFor(player);
    for (uint32 slot = 0; slot < slots; ++slot)
    {
        LootItem const* reward = loot.LootItemInSlot(slot, player);
        if (!reward)
            continue;
        InventoryResult space =
            player->CanStoreNewItem(NULL_BAG, NULL_SLOT, reserved, reward->itemid, reward->count);
        if (space != EQUIP_ERR_OK)
        {
            player->SendEquipError(space, nullptr, nullptr, reward->itemid);
            return;
        }
        rewards.push_back(reward);
    }

    uint32 count = 1;
    player->DestroyItemCount(item, count, true);
    for (LootItem const* reward : rewards)
    {
        ItemPosCountVec dest;
        if (player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, reward->itemid, reward->count) != EQUIP_ERR_OK)
            continue;
        if (Item* stored = player->StoreNewItem(dest, reward->itemid, true, reward->randomPropertyId))
            player->SendNewItem(stored, reward->count, false, false, true);
    }
}

class item_ascension_adventurer_cache : public ItemScript
{
public:
    item_ascension_adventurer_cache() : ItemScript("item_ascension_adventurer_cache") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const&) override
    {
        if (!IsAdventurerReward(item->GetEntry()))
            return false;
        OpenCache(player, item);
        return true;
    }
};

class adventurer_cache_open : public ServerScript
{
public:
    adventurer_cache_open() : ServerScript("adventurer_cache_open", {SERVERHOOK_CAN_PACKET_RECEIVE}) { }

    bool CanPacketReceive(WorldSession* session, WorldPacket const& packet) override
    {
        if (packet.GetOpcode() != CMSG_OPEN_ITEM || packet.size() < 2)
            return true;
        Player* player = session ? session->GetPlayer() : nullptr;
        if (!player || player->m_mover != player)
            return true;
        Item* item = player->GetItemByPos(packet.read<uint8>(0), packet.read<uint8>(1));
        if (!item || !IsAdventurerReward(item->GetEntry()))
            return true;

        ObjectGuid const cache = item->GetGUID();
        OpenCache(player, item);
        player->SendLootRelease(cache);
        return false;
    }
};

class adventurer_cache_loot : public GlobalScript
{
public:
    adventurer_cache_loot() : GlobalScript("adventurer_cache_loot",
        {GLOBALHOOK_ON_BEFORE_LOOT_EQUAL_CHANCED}) { }

    bool OnBeforeLootEqualChanced(Player const* player, std::list<LootStoreItem*> entries,
        Loot& loot, LootStore const& store) override
    {
        if (!player || &store != &LootTemplates_Item)
            return true;
        Item const* container = player->GetItemByGuid(loot.containerGUID);
        if (!container || !IsAdventurerReward(container->GetEntry()))
            return true;

        std::array<std::vector<LootStoreItem*>, RewardKindCount> rewards;
        std::array<uint32, RewardKindCount> bestLevel{};
        for (LootStoreItem* entry : entries)
        {
            ItemTemplate const* item = sObjectMgr->GetItemTemplate(entry->itemid);
            if (!item || entry->reference)
                continue;
            RewardKind kind;
            if (item->Class == ITEM_CLASS_TRADE_GOODS)
                kind = Material;
            else if (item->Class == ITEM_CLASS_ARMOR)
                kind = Armor;
            else if (item->Class == ITEM_CLASS_CONSUMABLE && item->SubClass == ITEM_SUBCLASS_FOOD)
                kind = Food;
            else if (item->IsPotion())
                kind = Potion;
            else
                continue;

            uint32 level = kind == Material ? item->ItemLevel : item->RequiredLevel;
            uint32 ceiling = player->GetLevel() + (kind == Material ? 5 : 0);
            if (level > ceiling || (kind != Material && player->CanUseItem(item) != EQUIP_ERR_OK) ||
                (kind == Armor && item->GetSkill() && !player->GetSkillValue(item->GetSkill())))
                continue;
            rewards[kind].push_back(entry);
            bestLevel[kind] = std::max(bestLevel[kind], level);
        }

        std::vector<uint8> kinds;
        for (uint8 kind = 0; kind < RewardKindCount; ++kind)
        {
            std::erase_if(rewards[kind], [kind, &bestLevel](LootStoreItem const* entry)
            {
                ItemTemplate const* item = sObjectMgr->GetItemTemplate(entry->itemid);
                uint32 level = kind == Material ? item->ItemLevel : item->RequiredLevel;
                return level + 10 < bestLevel[kind];
            });
            if (!rewards[kind].empty())
                kinds.push_back(kind);
        }
        if (!kinds.empty())
        {
            auto const& choices = rewards[kinds[urand(0, uint32(kinds.size() - 1))]];
            loot.AddItem(*choices[urand(0, uint32(choices.size() - 1))]);
        }
        return false;
    }
};
}

void AddSC_AscensionAdventurerCache()
{
    new item_ascension_adventurer_cache();
    new adventurer_cache_open();
    new adventurer_cache_loot();
}
