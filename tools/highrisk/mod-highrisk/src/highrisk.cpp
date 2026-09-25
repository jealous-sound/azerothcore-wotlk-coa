#include "highrisk_policy.h"
#include "Bag.h"
#include "Chat.h"
#include "Config.h"
#include "DatabaseEnv.h"
#include "Errors.h"
#include "GameObject.h"
#include "Item.h"
#include "LootMgr.h"
#include "Map.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "Random.h"
#include "ScriptMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include <algorithm>
#include <atomic>
#include <map>
#include <memory>
#include <mutex>

namespace HighRisk
{
struct StoredItem
{
    uint32 guid = 0;
    uint32 entry = 0;
    uint32 count = 0;
    int32 property = 0;
    uint32 suffix = 0;
    bool claimed = false;
};

struct Chest
{
    uint64 id = 0;
    uint32 owner = 0;
    uint32 map = 0;
    uint32 phase = 1;
    Position position;
    uint32 gold = 0;
    ObjectGuid object;
    std::map<uint8, StoredItem> items;

    bool Empty() const
    {
        return !gold && std::all_of(items.begin(), items.end(),
            [](auto const& row) { return row.second.claimed; });
    }
};

std::mutex mutex;
std::map<uint64, Chest> chests;
uint64 nextId = 0;
std::atomic_bool loaded{false};
std::atomic_bool enabled{false};
std::atomic<float> gearChance{50.0f};
std::atomic<float> resourceChance{50.0f};
std::atomic<float> goldChance{50.0f};

bool OpenWorld(Player const* player)
{
    return player->GetMap() && !player->GetMap()->Instanceable();
}

uint32 Mode(Player const* player)
{
    auto* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_HIGHRISK_MODE);
    stmt->SetData(0, player->GetGUID().GetCounter());
    PreparedQueryResult result = CharacterDatabase.Query(stmt);
    return result ? result->Fetch()[0].Get<uint8>() : 0;
}

void Commit(CharacterDatabaseTransaction transaction)
{
    // Do not expose the chest or acknowledge a claim before its inventory and
    // escrow changes have committed together. On database failure, stop before
    // later character saves can persist an inventory inconsistent with escrow.
    auto result = CharacterDatabase.AsyncCommitTransaction(transaction);
    if (!result.m_future.get())
        ABORT("High Risk inventory transaction failed; refusing to continue");
}

bool Transferable(Item const* item)
{
    if (!item || item->IsBag() || item->IsSoulBound() || item->IsBoundAccountWide()
        || item->IsInTrade() || item->IsWrapped()
        || item->IsRefundable() || item->IsBOPTradable() || item->GetCount() > 255)
        return false;

    auto const* info = item->GetTemplate();
    if (info->StartQuest || info->Duration || info->Bonding == BIND_QUEST_ITEM
        || info->HasFlag(ITEM_FLAG_CONJURED)
        || item->GetEnchantmentDuration(TEMP_ENCHANTMENT_SLOT))
        return false;

    return info->Class == ITEM_CLASS_ARMOR || info->Class == ITEM_CLASS_WEAPON
        || info->Class == ITEM_CLASS_TRADE_GOODS;
}

void FillLoot(GameObject* object, Chest const& chest)
{
    auto& loot = object->loot;
    loot.clear();
    loot.sourceWorldObjectGUID = object->GetGUID();
    loot.loot_type = LOOT_CORPSE;
    loot.gold = chest.gold;
    if (!chest.items.empty())
        loot.items.resize(chest.items.rbegin()->first + 1);

    for (auto& item : loot.items)
        item.is_looted = true;

    for (auto const& [slot, stored] : chest.items)
    {
        auto& item = loot.items[slot];
        item.itemid = stored.entry;
        item.itemIndex = slot;
        item.count = stored.count;
        item.randomPropertyId = stored.property;
        item.randomSuffix = stored.suffix;
        item.is_looted = stored.claimed;
        item.is_underthreshold = true;
        item.is_counted = true;
        if (!stored.claimed)
            ++loot.unlootedCount;
    }
    object->SetLootState(GO_ACTIVATED);
}

void Spawn(Map* map, Chest& chest)
{
    if (chest.Empty() || chest.map != map->GetId() || map->GetInstanceId())
        return;
    if (chest.object && map->GetGameObject(chest.object))
        return;

    // Keep distant deposits in the database rather than holding entire world
    // grids active indefinitely. Recreate the object when someone returns.
    bool nearby = false;
    for (auto const& reference : map->GetPlayers())
        if (Player* player = reference.GetSource())
            if (player->GetExactDist(&chest.position) < 200.0f)
            {
                nearby = true;
                break;
            }
    if (!nearby)
        return;

    auto object = std::make_unique<GameObject>();
    auto const& p = chest.position;
    if (!object->Create(map->GenerateLowGuid<HighGuid::GameObject>(), ChestEntry, map, chest.phase,
        p.GetPositionX(), p.GetPositionY(), p.GetPositionZ(), p.GetOrientation(),
        G3D::Quat(0, 0, 0, 1), 255, GO_STATE_READY))
        return;

    object->SetSpawnedByDefault(false);
    object->SetRespawnTime(0);
    FillLoot(object.get(), chest);
    if (map->AddToMap(object.get()))
    {
        chest.object = object->GetGUID();
        object.release();
    }
}

Chest* Find(GameObject const* object)
{
    if (!object || object->GetEntry() != ChestEntry)
        return nullptr;
    for (auto& [id, chest] : chests)
        if (chest.map == object->GetMapId() && chest.object == object->GetGUID())
            return &chest;
    return nullptr;
}

bool Access(Player* player, GameObject* object)
{
    return player && object && player->IsAlive() && OpenWorld(player)
        && player->InSamePhase(object) && object->IsWithinDistInMap(player, INTERACTION_DISTANCE)
        && !player->HasPlayerFlag(PLAYER_FLAGS_NO_PLAY_TIME)
        && !player->GetSession()->IsBot() && player->GetLevel() >= MinimumLevel && Mode(player) == 3;
}

void Close(Player* player, GameObject* object)
{
    object->loot.RemoveLooter(player->GetGUID());
    player->SetLootGUID(ObjectGuid::Empty);
    player->RemoveUnitFlag(UNIT_FLAG_LOOTING);
    player->SendLootRelease(object->GetGUID());
}

void Open(Player* player, GameObject* object)
{
    if (!Access(player, object))
    {
        ChatHandler(player->GetSession()).SendSysMessage(
            "You must be alive, in High-Risk mode and close to the chest to recover its contents.");
        LOG_INFO("server.worldserver", "High Risk chest access denied for {}: alive {}, mode {}, distance {}",
            player->GetGUID().ToString(), player->IsAlive(), Mode(player), player->GetDistance(object));
        player->SendLootError(object->GetGUID(), LOOT_ERROR_DIDNT_KILL);
        return;
    }
    if (player->GetLootGUID())
        player->GetSession()->DoLootRelease(player->GetLootGUID());

    std::lock_guard<std::mutex> lock(mutex);
    Chest* chest = Find(object);
    if (!chest || chest->Empty())
    {
        player->SendLootRelease(object->GetGUID());
        return;
    }
    player->RemoveAurasByType(SPELL_AURA_MOD_STEALTH);
    player->RemoveAurasByType(SPELL_AURA_MOD_INVISIBILITY);
    player->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);
    player->SetLootGUID(object->GetGUID());
    WorldPacket packet(SMSG_LOOT_RESPONSE, 80);
    packet << object->GetGUID() << uint8(LOOT_CORPSE) << LootView(object->loot, player, ALL_PERMISSION);
    player->SendDirectMessage(&packet);
    object->loot.AddLooter(player->GetGUID());
    player->SetUnitFlag(UNIT_FLAG_LOOTING);
    LOG_INFO("server.worldserver", "High Risk chest {} opened by {}", chest->id, player->GetGUID().ToString());
}

void ClaimItem(Player* player, GameObject* object, uint8 slot)
{
    if (!Access(player, object))
    {
        Close(player, object);
        return;
    }
    std::lock_guard<std::mutex> lock(mutex);
    Chest* chest = Find(object);
    if (!chest)
        return;
    auto found = chest->items.find(slot);
    if (found == chest->items.end() || found->second.claimed)
        return;
    if (slot >= object->loot.items.size()
        || !object->loot.items[slot].AllowedForPlayer(player, object->GetGUID()))
        return;

    auto* select = CharacterDatabase.GetPreparedStatement(CHAR_SEL_HIGHRISK_ITEM);
    select->SetData(0, chest->id);
    select->SetData(1, slot);
    PreparedQueryResult result = CharacterDatabase.Query(select);
    if (!result)
        return;

    Field* fields = result->Fetch();
    auto item = std::make_unique<Item>();
    if (!item->LoadFromDB(fields[11].Get<uint32>(), ObjectGuid::Empty, fields, fields[12].Get<uint32>()))
        return;
    ItemPosCountVec destination;
    InventoryResult error = player->CanStoreItem(NULL_BAG, NULL_SLOT, destination, item.get(), false);
    if (error != EQUIP_ERR_OK)
    {
        player->SendEquipError(error, item.get(), nullptr);
        return;
    }

    uint32 entry = item->GetEntry();
    uint32 count = item->GetCount();
    item->SetState(ITEM_UNCHANGED);
    auto transaction = CharacterDatabase.BeginTransaction();
    player->MoveItemToInventory(destination, item.release(), true);
    player->SaveInventoryAndGoldToDB(transaction);
    auto* claim = CharacterDatabase.GetPreparedStatement(CHAR_UPD_HIGHRISK_ITEM_CLAIM);
    claim->SetData(0, player->GetGUID().GetCounter());
    claim->SetData(1, chest->id);
    claim->SetData(2, slot);
    transaction->Append(claim);
    Commit(transaction);
    found->second.claimed = true;
    if (!destination.empty())
        if (Item* received = player->GetItemByPos(destination.back().pos))
            player->SendNewItem(received, count, false, false, true);
    object->loot.items[slot].is_looted = true;
    --object->loot.unlootedCount;
    object->loot.NotifyItemRemoved(slot);
    LOG_INFO("server.worldserver", "High Risk chest {}: {} recovered item {} x{} (original GUID {})",
        chest->id, player->GetGUID().ToString(), entry, count, found->second.guid);
    if (chest->Empty())
    {
        Close(player, object);
        object->AddObjectToRemoveList();
    }
}

void ClaimGold(Player* player, GameObject* object)
{
    if (!Access(player, object))
    {
        Close(player, object);
        return;
    }
    std::lock_guard<std::mutex> lock(mutex);
    Chest* chest = Find(object);
    if (!chest || !chest->gold)
        return;
    if (uint64(player->GetMoney()) + chest->gold > MAX_MONEY_AMOUNT)
    {
        ChatHandler(player->GetSession()).SendSysMessage("You cannot carry any more gold.");
        return;
    }

    uint32 gold = chest->gold;
    auto transaction = CharacterDatabase.BeginTransaction();
    player->ModifyMoney(gold);
    player->SaveInventoryAndGoldToDB(transaction);
    auto* claim = CharacterDatabase.GetPreparedStatement(CHAR_UPD_HIGHRISK_GOLD_CLAIM);
    claim->SetData(0, player->GetGUID().GetCounter());
    claim->SetData(1, chest->id);
    transaction->Append(claim);
    Commit(transaction);
    chest->gold = 0;
    object->loot.gold = 0;
    object->loot.NotifyMoneyRemoved();
    WorldPacket packet(SMSG_LOOT_MONEY_NOTIFY, 5);
    packet << gold << uint8(1);
    player->SendDirectMessage(&packet);
    if (chest->Empty())
    {
        Close(player, object);
        object->AddObjectToRemoveList();
    }
}

void Drop(Player* player)
{
    if (!enabled || !loaded || !OpenWorld(player) || player->GetLevel() < MinimumLevel
        || player->GetSession()->IsBot())
        return;
    if (!Eligible(player->GetLevel(), Mode(player), !OpenWorld(player), player->GetSession()->IsBot()))
        return;

    std::vector<Item*> gear;
    std::vector<Item*> resources;
    auto add = [&](Item* item)
    {
        if (!Transferable(item))
            return;
        if (item->GetTemplate()->Class == ITEM_CLASS_TRADE_GOODS)
            resources.push_back(item);
        else
            gear.push_back(item);
    };
    for (uint8 slot = EQUIPMENT_SLOT_START; slot < INVENTORY_SLOT_ITEM_END; ++slot)
        add(player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot));
    for (uint8 bag = INVENTORY_SLOT_BAG_START; bag < INVENTORY_SLOT_BAG_END; ++bag)
        if (Bag* container = player->GetBagByPos(bag))
            for (uint32 slot = 0; slot < container->GetBagSize(); ++slot)
                add(container->GetItemByPos(slot));

    std::vector<Item*> losses;
    auto choose = [&](std::vector<Item*>& pool)
    {
        if (pool.empty())
            return;
        uint32 index = urand(0, pool.size() - 1);
        losses.push_back(pool[index]);
        pool.erase(pool.begin() + index);
    };
    if (roll_chance_f(gearChance.load()))
        for (unsigned i = 0, count = urand(1, GearLimit(player->GetLevel())); i < count; ++i)
            choose(gear);
    if (roll_chance_f(resourceChance.load()))
        choose(resources);
    uint32 gold = roll_chance_f(goldChance.load()) ? GoldLoss(player->GetMoney(), urand(1, 5)) : 0;
    // An eligible death must not silently produce no chest just because every
    // category roll missed. Choose one available category without bypassing
    // item protections or a category explicitly disabled in the configuration.
    if (losses.empty() && !gold)
    {
        std::vector<unsigned> available;
        if (!gear.empty() && gearChance > 0.0f)
            available.push_back(0);
        if (!resources.empty() && resourceChance > 0.0f)
            available.push_back(1);
        if (player->GetMoney() && goldChance > 0.0f)
            available.push_back(2);
        if (!available.empty())
            switch (available[urand(0, available.size() - 1)])
            {
                case 0:
                    choose(gear);
                    break;
                case 1:
                    choose(resources);
                    break;
                case 2:
                    gold = std::max(uint32(1), GoldLoss(player->GetMoney(), urand(1, 5)));
                    break;
            }
    }
    if (losses.empty() && !gold)
    {
        ChatHandler(player->GetSession()).SendSysMessage(
            "No High-Risk chest was created: you have no eligible items or money to lose.");
        LOG_INFO("server.worldserver", "High Risk death for {}: no eligible losses; map {}, level {}",
            player->GetGUID().ToString(), player->GetMapId(), player->GetLevel());
        return;
    }

    std::lock_guard<std::mutex> lock(mutex);
    Chest chest;
    chest.id = ++nextId;
    chest.owner = player->GetGUID().GetCounter();
    chest.map = player->GetMapId();
    chest.phase = player->GetPhaseMask();
    chest.position.Relocate(player);
    chest.gold = gold;
    auto transaction = CharacterDatabase.BeginTransaction();
    auto* create = CharacterDatabase.GetPreparedStatement(CHAR_INS_HIGHRISK_CHEST);
    create->SetData(0, chest.id);
    create->SetData(1, chest.owner);
    create->SetData(2, chest.map);
    create->SetData(3, chest.phase);
    create->SetData(4, chest.position.GetPositionX());
    create->SetData(5, chest.position.GetPositionY());
    create->SetData(6, chest.position.GetPositionZ());
    create->SetData(7, chest.position.GetOrientation());
    create->SetData(8, gold);
    create->SetData(9, gold);
    transaction->Append(create);

    uint8 slot = 0;
    for (Item* item : losses)
    {
        StoredItem stored{item->GetGUID().GetCounter(), item->GetEntry(), item->GetCount(),
            item->GetItemRandomPropertyId(), item->GetItemSuffixFactor(), false};
        player->MoveItemFromInventory(item->GetBagSlot(), item->GetSlot(), true);
        item->DeleteFromInventoryDB(transaction);
        if (item->GetState() == ITEM_UNCHANGED)
            item->FSetState(ITEM_CHANGED);
        item->SetOwnerGUID(ObjectGuid::Empty);
        item->SaveToDB(transaction);
        auto* insert = CharacterDatabase.GetPreparedStatement(CHAR_INS_HIGHRISK_ITEM);
        insert->SetData(0, chest.id);
        insert->SetData(1, slot);
        insert->SetData(2, stored.guid);
        insert->SetData(3, stored.entry);
        insert->SetData(4, stored.count);
        transaction->Append(insert);
        chest.items.emplace(slot++, stored);
    }
    player->ModifyMoney(-int32(gold));
    player->SaveInventoryAndGoldToDB(transaction);
    Commit(transaction);
    for (Item* item : losses)
        delete item;
    auto [found, inserted] = chests.emplace(chest.id, std::move(chest));
    Spawn(player->GetMap(), found->second);
    ChatHandler(player->GetSession()).SendSysMessage(
        "Your High-Risk losses are in a chest at your death location. Any High-Risk player can recover them.");
    LOG_INFO("server.worldserver", "High Risk chest {} created for {}: {} items, {} copper",
        found->first, player->GetGUID().ToString(), losses.size(), gold);
}

class RiskWorld final : public WorldScript
{
public:
    RiskWorld() : WorldScript("highrisk_world", {WORLDHOOK_ON_STARTUP, WORLDHOOK_ON_AFTER_CONFIG_LOAD}) { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        enabled = sConfigMgr->GetOption<bool>("HighRisk.Losses", false);
        gearChance = std::clamp(sConfigMgr->GetOption<float>("HighRisk.GearChance", 50.0f), 0.0f, 100.0f);
        resourceChance = std::clamp(sConfigMgr->GetOption<float>("HighRisk.ResourceChance", 50.0f), 0.0f, 100.0f);
        goldChance = std::clamp(sConfigMgr->GetOption<float>("HighRisk.GoldChance", 50.0f), 0.0f, 100.0f);
    }

    void OnStartup() override
    {
        OnAfterConfigLoad(false);
        std::lock_guard<std::mutex> lock(mutex);
        auto result = CharacterDatabase.Query(CharacterDatabase.GetPreparedStatement(CHAR_SEL_HIGHRISK_MAX_CHEST));
        ASSERT(result, "High Risk chest schema is missing");
        nextId = result->Fetch()[0].Get<uint64>();
        result = CharacterDatabase.Query(CharacterDatabase.GetPreparedStatement(CHAR_SEL_HIGHRISK_CHESTS));
        if (result)
        {
            do
            {
                Field* f = result->Fetch();
                Chest chest;
                chest.id = f[0].Get<uint64>();
                chest.owner = f[1].Get<uint32>();
                chest.map = f[2].Get<uint16>();
                chest.phase = f[3].Get<uint32>();
                chest.position.Relocate(f[4].Get<float>(), f[5].Get<float>(), f[6].Get<float>(), f[7].Get<float>());
                chest.gold = f[8].Get<uint32>();
                auto* select = CharacterDatabase.GetPreparedStatement(CHAR_SEL_HIGHRISK_ITEMS);
                select->SetData(0, chest.id);
                if (auto items = CharacterDatabase.Query(select))
                {
                    do
                    {
                        Field* i = items->Fetch();
                        auto* load = CharacterDatabase.GetPreparedStatement(CHAR_SEL_HIGHRISK_ITEM);
                        load->SetData(0, chest.id);
                        load->SetData(1, i[0].Get<uint8>());
                        auto instance = CharacterDatabase.Query(load);
                        if (!instance)
                        {
                            LOG_ERROR("module.highrisk", "Chest {} slot {}: escrow inconsistency; new losses disabled",
                                chest.id, i[0].Get<uint8>());
                            enabled = false;
                            continue;
                        }
                        Item item;
                        if (!item.LoadFromDB(i[1].Get<uint32>(), ObjectGuid::Empty, instance->Fetch(), i[2].Get<uint32>()))
                        {
                            LOG_ERROR("module.highrisk", "Chest {}: item {} cannot load; new losses disabled",
                                chest.id, i[1].Get<uint32>());
                            enabled = false;
                            continue;
                        }
                        chest.items.emplace(i[0].Get<uint8>(), StoredItem{i[1].Get<uint32>(), i[2].Get<uint32>(),
                            i[3].Get<uint32>(), i[4].Get<int32>(), item.GetItemSuffixFactor(), false});
                    } while (items->NextRow());
                }
                chests.emplace(chest.id, std::move(chest));
            } while (result->NextRow());
        }
        loaded = true;
        LOG_INFO("server.worldserver", "Restored {} High Risk chests; losses {}", chests.size(), enabled.load());
    }
};

class RiskPlayer final : public PlayerScript
{
public:
    RiskPlayer() : PlayerScript("highrisk_death", {PLAYERHOOK_ON_PLAYER_JUST_DIED, PLAYERHOOK_ON_LOGIN}) { }
    void OnPlayerJustDied(Player* player) override { Drop(player); }

    void OnPlayerLogin(Player* player) override
    {
        // Refresh the old lockless template retained in the client's WDB cache.
        // Reuse the native serializer, including localized names and lock data.
        if (player->GetSession()->IsBot())
            return;
        WorldPacket query(CMSG_GAMEOBJECT_QUERY, 12);
        query << uint32(ChestEntry) << ObjectGuid::Empty;
        player->GetSession()->HandleGameObjectQueryOpcode(query);
    }
};

class RiskMaps final : public AllMapScript
{
public:
    RiskMaps() : AllMapScript("highrisk_maps", {ALLMAPHOOK_ON_MAP_UPDATE}) { }
    void OnMapUpdate(Map* map, uint32 diff) override
    {
        if (!loaded || map->Instanceable())
            return;
        std::lock_guard<std::mutex> lock(mutex);
        uint32& elapsed = refresh[map->GetId()];
        elapsed += diff;
        if (elapsed < 1000)
            return;
        elapsed = 0;
        for (auto it = chests.begin(); it != chests.end();)
        {
            if (it->second.Empty())
                it = chests.erase(it);
            else
            {
                Spawn(map, it->second);
                ++it;
            }
        }
    }

private:
    std::map<uint32, uint32> refresh;
};

class RiskChest final : public GameObjectScript
{
public:
    RiskChest() : GameObjectScript("highrisk_chest") { }
    bool OnGossipHello(Player* player, GameObject* object) override
    {
        Open(player, object);
        return true;
    }
};

class RiskPackets final : public ServerScript
{
public:
    RiskPackets() : ServerScript("highrisk_loot", {SERVERHOOK_CAN_PACKET_RECEIVE}) { }
    bool CanPacketReceive(WorldSession* session, WorldPacket const& packet) override
    {
        if (!session || !session->GetPlayer())
            return true;
        Player* player = session->GetPlayer();
        if (!player->IsInWorld())
            return true;

        auto opcode = packet.GetOpcode();
        if (opcode == CMSG_LOOT || opcode == CMSG_GAMEOBJ_USE || opcode == CMSG_GAMEOBJ_REPORT_USE)
        {
            if (packet.size() < 8)
                return true;
            ObjectGuid guid(packet.read<uint64>(0));
            if (!guid.IsGameObject())
                return true;
            GameObject* object = player->GetMap()->GetGameObject(guid);
            if (!object || object->GetEntry() != ChestEntry)
                return true;
            Open(player, object);
            return false;
        }
        if (opcode != CMSG_AUTOSTORE_LOOT_ITEM && opcode != CMSG_LOOT_MONEY
            && opcode != CMSG_LOOT_MASTER_GIVE && opcode != CMSG_LOOT_RELEASE)
            return true;
        ObjectGuid guid = player->GetLootGUID();
        if (!guid.IsGameObject())
            return true;
        GameObject* object = player->GetMap()->GetGameObject(guid);
        if (!object || object->GetEntry() != ChestEntry)
            return true;
        if (opcode == CMSG_AUTOSTORE_LOOT_ITEM && packet.size())
            ClaimItem(player, object, packet.read<uint8>(0));
        else if (opcode == CMSG_LOOT_MONEY)
            ClaimGold(player, object);
        else if (opcode == CMSG_LOOT_RELEASE)
            Close(player, object);
        return false;
    }
};
}

void AddHighRiskScripts()
{
    new HighRisk::RiskWorld();
    new HighRisk::RiskPlayer();
    new HighRisk::RiskMaps();
    new HighRisk::RiskChest();
    new HighRisk::RiskPackets();
}
