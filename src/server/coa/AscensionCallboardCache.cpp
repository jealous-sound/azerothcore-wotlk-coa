/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionCacheRewards.h"
#include "Chat.h"
#include "Config.h"
#include "DatabaseEnv.h"
#include "Item.h"
#include "ItemScript.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "PlayerScript.h"
#include "Random.h"
#include "ScriptMgr.h"
#include "World.h"
#include <algorithm>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace
{
constexpr uint32 CallboardGeneric = 978050;
constexpr uint32 CallboardGenericOld = 1378050;
constexpr uint32 CallboardGenericVoucher = 1615000;
constexpr uint32 CallboardBonus = 1478050;

std::vector<std::vector<uint32>> const CallboardTiers = {
    { 1615001, 1615002 },
    { 1615003 },
    { 1615004 },
    { 1615005 },
    { 1615006 },
    { 1615007 },
    { 1615008 },
    { 1615009 }
};

using CallboardPool = std::vector<AscensionCacheRewards::Reward>;

std::mutex g_poolLock;
std::unordered_map<uint32, CallboardPool> g_pools;

uint8 g_releaseStage = 7;
uint32 g_itemLevelAllowance = 6;
bool g_previousStageOnly = false;

void LoadCallboardCachePools()
{
    g_releaseStage = uint8(sConfigMgr->GetOption<uint32>("Ascension.CallboardCache.ReleaseStage", 7));
    g_itemLevelAllowance = sConfigMgr->GetOption<uint32>("Ascension.CallboardCache.ItemLevelAllowance", 6);
    g_previousStageOnly = sConfigMgr->GetOption<bool>("Ascension.CallboardCache.PreviousStageOnly", false);

    std::unordered_map<uint32, CallboardPool> pools;
    if (QueryResult result = WorldDatabase.Query(
            "SELECT `CacheItemId`, `RewardItemId`, `RewardItemLevel`, `StatType`, `ArmorClass` "
            "FROM `ascension_callboard_cache_reward`"))
    {
        do
        {
            Field* fields = result->Fetch();
            pools[fields[0].Get<uint32>()].push_back(
                { fields[1].Get<uint32>(), fields[2].Get<uint16>(), fields[3].Get<uint8>(),
                  fields[4].Get<uint8>() });
        } while (result->NextRow());
    }
    else
    {
        LOG_WARN("coa",
            "ascension_callboard_cache_reward is missing: Callboard Caches will not open.");
    }

    std::lock_guard<std::mutex> guard(g_poolLock);
    g_pools = std::move(pools);
}

CallboardPool const* GetPool(uint32 cacheItemId)
{
    std::lock_guard<std::mutex> guard(g_poolLock);
    auto it = g_pools.find(cacheItemId);
    return it == g_pools.end() ? nullptr : &it->second;
}

uint32 HighestItemLevel(CallboardPool const& pool)
{
    uint32 highest = 0;
    for (AscensionCacheRewards::Reward const& reward : pool)
        highest = std::max<uint32>(highest, reward.itemLevel);
    return highest;
}

uint8 HighestStage(uint32 cacheItemId)
{
    uint8 highest = g_releaseStage;
    if (!CallboardTiers.empty() && highest > CallboardTiers.size() - 1)
        highest = uint8(CallboardTiers.size() - 1);
    if ((g_previousStageOnly || cacheItemId == CallboardGenericOld) && highest > 0)
        --highest;
    return highest;
}

uint32 ResolveGenericTier(Player* player, uint32 cacheItemId)
{
    uint8 const highest = HighestStage(cacheItemId);
    uint32 averageItemLevel = uint32(player->GetAverageItemLevel());
    uint32 chosen = 0;

    for (uint8 index = 0; index <= highest; ++index)
    {
        for (uint32 candidate : CallboardTiers[index])
        {
            CallboardPool const* pool = GetPool(candidate);
            if (!pool || pool->empty())
                continue;
            if (HighestItemLevel(*pool) <= averageItemLevel + g_itemLevelAllowance)
                chosen = candidate;
        }
    }

    if (!chosen)
    {
        for (uint8 index = 0; index <= highest && !chosen; ++index)
            for (uint32 candidate : CallboardTiers[index])
                if (CallboardPool const* pool = GetPool(candidate))
                    if (!pool->empty())
                    {
                        chosen = candidate;
                        break;
                    }
    }
    return chosen;
}

bool PickReward(Player* player, CallboardPool const& pool, uint32 averageItemLevel,
    AscensionCacheRewards::Reward& out)
{
    std::vector<AscensionCacheRewards::Reward const*> upgrades;
    std::vector<AscensionCacheRewards::Reward const*> reachable;
    std::vector<AscensionCacheRewards::Reward const*> usable;
    for (AscensionCacheRewards::Reward const& reward : pool)
    {
        ItemTemplate const* proto = sObjectMgr->GetItemTemplate(reward.itemId);
        if (!proto || player->CanUseItem(proto) != EQUIP_ERR_OK)
            continue;
        usable.push_back(&reward);
        if (reward.itemLevel > averageItemLevel + g_itemLevelAllowance)
            continue;
        reachable.push_back(&reward);
        if (reward.itemLevel >= averageItemLevel)
            upgrades.push_back(&reward);
    }

    std::vector<AscensionCacheRewards::Reward const*> const& candidates =
        !upgrades.empty() ? upgrades : (!reachable.empty() ? reachable : usable);
    if (candidates.empty())
        return false;

    out = *candidates[urand(0, uint32(candidates.size() - 1))];
    return true;
}

bool OpenCallboardCache(Player* player, Item* item)
{
    uint32 cacheItemId = item->GetEntry();
    if (cacheItemId == CallboardGeneric || cacheItemId == CallboardGenericOld ||
        cacheItemId == CallboardGenericVoucher || cacheItemId == CallboardBonus)
    {
        cacheItemId = ResolveGenericTier(player, cacheItemId);
    }

    CallboardPool const* pool = GetPool(cacheItemId);
    if (!pool)
        return false;

    player->SendEquipError(EQUIP_ERR_NONE, item, nullptr);

    AscensionCacheRewards::Reward reward;
    if (!PickReward(player, *pool, uint32(player->GetAverageItemLevel()), reward))
    {
        ChatHandler(player->GetSession()).SendSysMessage(
            "The cache holds nothing this character can use.");
        return true;
    }

    std::vector<AscensionCacheRewards::Reward> payout = { reward };
    AscensionCacheRewards::Deliver(player, payout, item);

    return true;
}

class item_ascension_callboard_cache : public ItemScript
{
public:
    item_ascension_callboard_cache() : ItemScript("item_ascension_callboard_cache") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const&) override
    {
        return OpenCallboardCache(player, item);
    }
};

class ascension_callboard_cache_open : public PlayerScript
{
public:
    ascension_callboard_cache_open()
        : PlayerScript("ascension_callboard_cache_open", { PLAYERHOOK_ON_BEFORE_OPEN_ITEM }) { }

    bool OnPlayerBeforeOpenItem(Player* player, Item* item) override
    {
        if (!item || !OpenCallboardCache(player, item))
            return true;
        return false;
    }
};

class ascension_callboard_cache_pools : public WorldScript
{
public:
    ascension_callboard_cache_pools()
        : WorldScript("ascension_callboard_cache_pools",
              { WORLDHOOK_ON_STARTUP, WORLDHOOK_ON_AFTER_CONFIG_LOAD }) { }

    void OnStartup() override
    {
        LoadCallboardCachePools();
    }

    void OnAfterConfigLoad(bool reload) override
    {
        if (reload)
            LoadCallboardCachePools();
    }
};
}

void AddSC_AscensionCallboardCache()
{
    new item_ascension_callboard_cache();
    new ascension_callboard_cache_open();
    new ascension_callboard_cache_pools();
}
