/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionCacheRewards.h"
#include "AscensionCompatOpcodes.h"
#include "Chat.h"
#include "Config.h"
#include "DatabaseEnv.h"
#include "Item.h"
#include "ItemScript.h"
#include "Log.h"
#include "Mail.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "PlayerScript.h"
#include "QuestDef.h"
#include "Random.h"
#include "ScriptMgr.h"
#include "StringConvert.h"
#include "Tokenize.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include <algorithm>
#include <mutex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace
{
constexpr uint32 CallboardGeneric = 978050;
constexpr uint32 CallboardGenericOld = 1378050;
constexpr uint32 CallboardGenericVoucher = 1615000;
constexpr uint32 CallboardBonus = 1478050;

constexpr uint16 SMSG_CALLBOARD_CACHE_CONFIG = 0x0730;
constexpr uint16 CMSG_QUERY_CALLBOARD_QUEST_POINTS = 0x0731;
constexpr uint16 SMSG_CALLBOARD_QUEST_POINTS = 0x0732;
constexpr uint16 SMSG_CALLBOARD_TOKEN_UPDATE = 0x0670;
constexpr char CallboardPointsToken[] = "TOKEN_TYPE_CALLBOARD_CACHE_POINTS";
constexpr char CallboardPointsSetting[] = "core.callboard.points";

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

std::unordered_map<uint32, uint32> g_questPoints;

uint8 g_releaseStage = 7;
uint32 g_itemLevelAllowance = 6;
bool g_previousStageOnly = false;
std::vector<uint32> const DefaultStageCachePoints = { 3, 5, 10, 10, 15, 15, 15, 15 };
std::vector<uint32> g_stageCachePoints = DefaultStageCachePoints;

std::vector<uint32> ParseStageCachePoints(std::string const& list)
{
    std::vector<uint32> points;
    for (std::string_view token : Acore::Tokenize(list, ' ', false))
        points.push_back(std::max<uint32>(1, Acore::StringTo<uint32>(token).value_or(1)));

    if (points.empty())
        return DefaultStageCachePoints;

    points.resize(CallboardTiers.size(), points.back());
    return points;
}

void LoadCallboardQuestPoints()
{
    std::unordered_map<uint32, uint32> questPoints;
    if (QueryResult result = WorldDatabase.Query(
            "SELECT `QuestId`, `Points` FROM `ascension_callboard_quest_points`"))
    {
        do
        {
            Field* fields = result->Fetch();
            questPoints[fields[0].Get<uint32>()] = fields[1].Get<uint16>();
        } while (result->NextRow());
    }
    else
    {
        LOG_WARN("coa",
            "ascension_callboard_quest_points is missing: Callboard quests will not fill the cache progress bar.");
    }

    std::lock_guard<std::mutex> guard(g_poolLock);
    g_questPoints = std::move(questPoints);
}

void LoadCallboardCachePools()
{
    g_releaseStage = uint8(sConfigMgr->GetOption<uint32>("Ascension.CallboardCache.ReleaseStage", 7));
    g_itemLevelAllowance = sConfigMgr->GetOption<uint32>("Ascension.CallboardCache.ItemLevelAllowance", 6);
    g_previousStageOnly = sConfigMgr->GetOption<bool>("Ascension.CallboardCache.PreviousStageOnly", false);
    g_stageCachePoints = ParseStageCachePoints(
        sConfigMgr->GetOption<std::string>("Ascension.CallboardCache.StageCachePoints", "3 5 10 10 15 15 15 15"));
    LoadCallboardQuestPoints();

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

uint8 ReleasedStage()
{
    return std::min<uint8>(g_releaseStage, uint8(CallboardTiers.size() - 1));
}

uint8 HighestStage(uint32 cacheItemId)
{
    uint8 highest = ReleasedStage();
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

uint32 QuestPoints(uint32 questId)
{
    std::lock_guard<std::mutex> guard(g_poolLock);
    auto it = g_questPoints.find(questId);
    return it == g_questPoints.end() ? 0 : it->second;
}

uint32 OpenTierCache()
{
    return CallboardTiers[ReleasedStage()].front();
}

uint32 PointsPerCache()
{
    return g_stageCachePoints[ReleasedStage()];
}

uint32 StoredPoints(Player const* player)
{
    PlayerSettingVector const* stored = player->FindPlayerSettings(CallboardPointsSetting);
    return stored && !stored->empty() ? stored->front().value : 0;
}

void SendPoints(Player* player, uint32 points)
{
    WorldPacket update(SMSG_CALLBOARD_TOKEN_UPDATE, sizeof(CallboardPointsToken) + 2 * sizeof(uint32));
    update << CallboardPointsToken << points << uint32(0);
    player->SendDirectMessage(&update);
}

void SetPoints(Player* player, uint32 points)
{
    player->UpdatePlayerSetting(CallboardPointsSetting, 0, points);
    SendPoints(player, points);
}

void SendCacheConfig(Player* player)
{
    WorldPacket config(SMSG_CALLBOARD_CACHE_CONFIG, 7 * sizeof(uint32));
    config << uint32(1);
    config << uint32(1) << uint32(0) << uint32(0) << uint32(0) << OpenTierCache() << PointsPerCache();
    player->SendDirectMessage(&config);
}

void GrantCache(Player* player, uint32 cacheItemId)
{
    ItemPosCountVec dest;
    if (player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, cacheItemId, 1) == EQUIP_ERR_OK)
    {
        if (Item* item = player->StoreNewItem(dest, cacheItemId, true))
        {
            player->SendNewItem(item, 1, true, false);
            return;
        }
    }

    Item* item = Item::CreateItem(cacheItemId, 1, player);
    if (!item)
        return;

    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
    item->SaveToDB(trans);
    MailDraft("Callboard Cache", "Your bags were full, so your Callboard Cache was sent by mail.")
        .AddItem(item)
        .SendMailTo(trans, MailReceiver(player), MailSender(player));
    CharacterDatabase.CommitTransaction(trans);
}

void AwardQuestPoints(Player* player, uint32 questId)
{
    uint32 const award = QuestPoints(questId);
    if (!award)
        return;

    uint32 points = StoredPoints(player) + award;
    SetPoints(player, points);

    uint32 const cacheItemId = OpenTierCache();
    if (!sObjectMgr->GetItemTemplate(cacheItemId))
        return;

    uint32 const threshold = PointsPerCache();
    while (points >= threshold)
    {
        GrantCache(player, cacheItemId);
        points -= threshold;
        SetPoints(player, points);
    }
}

bool HandleQuestPointsQuery(WorldSession* session, WorldPacket const& packet)
{
    if (!session || packet.size() < sizeof(uint32))
        return true;

    uint32 const questId = packet.read<uint32>(0);
    WorldPacket reply(SMSG_CALLBOARD_QUEST_POINTS, 3 * sizeof(uint32));
    reply << questId << QuestPoints(questId) << uint32(0);
    session->SendPacket(&reply);
    return true;
}

void SendProgressState(Player* player)
{
    SendCacheConfig(player);
    SendPoints(player, StoredPoints(player));
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

class ascension_callboard_cache_progress : public PlayerScript
{
public:
    ascension_callboard_cache_progress()
        : PlayerScript("ascension_callboard_cache_progress",
              { PLAYERHOOK_ON_LOGIN, PLAYERHOOK_ON_SEND_INITIAL_PACKETS_BEFORE_ADD_TO_MAP,
                PLAYERHOOK_ON_PLAYER_COMPLETE_QUEST }) { }

    void OnPlayerLogin(Player* player) override
    {
        SendProgressState(player);
    }

    void OnPlayerSendInitialPacketsBeforeAddToMap(Player* player, WorldPacket&) override
    {
        if (player->IsInWorld())
            SendProgressState(player);
    }

    void OnPlayerCompleteQuest(Player* player, Quest const* quest) override
    {
        AwardQuestPoints(player, quest->GetQuestId());
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
    new ascension_callboard_cache_progress();
    new ascension_callboard_cache_pools();
    AscensionCompatOpcodes::Claim(CMSG_QUERY_CALLBOARD_QUEST_POINTS, &HandleQuestPointsQuery);
}
