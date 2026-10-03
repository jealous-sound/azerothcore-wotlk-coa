/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionAreaAccessPolicy.h"
#include "Chat.h"
#include "Config.h"
#include "DBCStores.h"
#include "Log.h"
#include "MapMgr.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "TransportMgr.h"
#include "WorldSession.h"
#include <sstream>
#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace
{
using PolicyPointer = std::shared_ptr<AreaAccess::Policy const>;

constexpr char const* LockedMessage = "This area is currently inaccessible.";
constexpr char const* BindResetMessage =
    "Your hearthstone was bound to an area that is no longer accessible. It is now bound to your starting area.";

PolicyPointer policy;

thread_local bool evicting = false;

AreaAccess::Throttle messageThrottle(std::chrono::seconds(30));

std::mutex taxiPathsLock;
std::unordered_map<uint32, std::vector<AreaAccess::Endpoint>> taxiPaths;

bool IsGameMaster(Player const* player)
{
    WorldSession const* session = player->GetSession();
    return session && session->GetSecurity() > SEC_PLAYER;
}

void Deny(Player* player)
{
    if (messageThrottle.Allow(player->GetGUID().GetRawValue(), std::chrono::steady_clock::now()))
        ChatHandler(player->GetSession()).SendSysMessage(LockedMessage);
}

uint32 ZoneAt(uint32 mapId, float x, float y, float z)
{
    return sMapMgr->GetZoneId(PHASEMASK_NORMAL, mapId, x, y, z);
}

std::vector<AreaAccess::Endpoint> const& TaxiPathPoints(uint32 pathId)
{
    std::lock_guard<std::mutex> guard(taxiPathsLock);
    auto it = taxiPaths.find(pathId);
    if (it == taxiPaths.end())
    {
        std::vector<AreaAccess::Endpoint> points;
        if (pathId < sTaxiPathNodesByPath.size())
            for (TaxiPathNodeEntry const* node : sTaxiPathNodesByPath[pathId])
                points.push_back({node->mapid, ZoneAt(node->mapid, node->x, node->y, node->z)});
        it = taxiPaths.emplace(pathId, std::move(points)).first;
    }
    return it->second;
}

void Reload()
{
    auto lookup = [](std::string const& key) -> std::optional<bool>
    {
        std::string const value = sConfigMgr->GetOption<std::string>(key, "");
        if (!value.empty() && !AreaAccess::ParseFlag(value))
            LOG_ERROR("server.loading", "Area access: invalid value '{}' for {}, treated as locked", value, key);
        return AreaAccess::FlagFromConfigText(value);
    };
    std::atomic_store(&policy, std::make_shared<AreaAccess::Policy const>(lookup));
}

PolicyPointer CurrentPolicy()
{
    PolicyPointer current = std::atomic_load(&policy);
    if (!current)
    {
        Reload();
        current = std::atomic_load(&policy);
    }
    return current;
}

bool IsTaxiRouteAllowed(Player const* player, std::vector<uint32> const& nodes)
{
    if (IsGameMaster(player))
        return true;

    std::vector<AreaAccess::Endpoint> points;
    for (uint32 nodeId : nodes)
        if (TaxiNodesEntry const* node = sTaxiNodesStore.LookupEntry(nodeId))
            points.push_back({node->map_id, ZoneAt(node->map_id, node->x, node->y, node->z)});
    for (std::size_t i = 1; i < nodes.size(); ++i)
    {
        uint32 pathId = 0;
        uint32 cost = 0;
        sObjectMgr->GetTaxiPath(nodes[i - 1], nodes[i], pathId, cost);
        std::vector<AreaAccess::Endpoint> const& path = TaxiPathPoints(pathId);
        points.insert(points.end(), path.begin(), path.end());
    }
    return CurrentPolicy()->IsRouteAllowed(points);
}

std::vector<AreaAccess::Endpoint> TransportDocks(TransportTemplate const& transport)
{
    std::vector<AreaAccess::RouteFrame> frames;
    for (KeyFrame const& frame : transport.keyFrames)
    {
        TaxiPathNodeEntry const* node = frame.Node;
        frames.push_back({{node->mapid, ZoneAt(node->mapid, node->x, node->y, node->z)}, frame.IsStopFrame()});
    }
    return AreaAccess::TransportDocks(transport.entry, frames);
}

std::string Describe(std::vector<AreaAccess::Endpoint> const& points)
{
    std::ostringstream text;
    for (std::size_t i = 0; i < points.size(); ++i)
        text << (i ? ", " : "") << "map " << points[i].mapId << " zone " << points[i].zoneId;
    return text.str();
}

void ResetLockedBind(Player* player)
{
    if (IsGameMaster(player))
        return;

    AreaTableEntry const* bindArea = sAreaTableStore.LookupEntry(player->m_homebindAreaId);
    uint32 const bindZone = !bindArea ? ZoneAt(player->m_homebindMapId, player->m_homebindX, player->m_homebindY,
        player->m_homebindZ) : (bindArea->zone ? bindArea->zone : bindArea->ID);
    PolicyPointer const current = CurrentPolicy();
    if (current->IsAllowed(false, player->m_homebindMapId, bindZone))
        return;

    WorldLocation const start = player->GetStartPosition();
    if (!current->IsAllowed(false, start.GetMapId(), ZoneAt(start.GetMapId(), start.GetPositionX(),
        start.GetPositionY(), start.GetPositionZ())))
        return;

    player->SetHomebind(start, sMapMgr->GetAreaId(PHASEMASK_NORMAL, start));
    ChatHandler(player->GetSession()).SendSysMessage(BindResetMessage);
}

void Evict(ObjectGuid guid)
{
    Player* player = ObjectAccessor::FindPlayer(guid);
    if (!player || !player->IsInWorld() || IsGameMaster(player))
        return;

    PolicyPointer const current = CurrentPolicy();
    if (current->IsAllowed(false, player->GetMapId(), player->GetZoneId()))
        return;

    WorldLocation const bind(player->m_homebindMapId, player->m_homebindX, player->m_homebindY, player->m_homebindZ, 0);
    for (WorldLocation const& destination : {bind, player->GetStartPosition()})
    {
        uint32 const zone = ZoneAt(destination.GetMapId(), destination.GetPositionX(), destination.GetPositionY(),
            destination.GetPositionZ());
        if (!current->IsAllowed(false, destination.GetMapId(), zone))
            continue;
        evicting = true;
        player->TeleportTo(destination);
        evicting = false;
        break;
    }
    Deny(player);
}

class Configuration : public WorldScript
{
public:
    Configuration() : WorldScript("AscensionAreaAccessConfiguration", { WORLDHOOK_ON_STARTUP,
        WORLDHOOK_ON_AFTER_CONFIG_LOAD, WORLDHOOK_ON_CAN_SPAWN_CONTINENT_TRANSPORT }) { }

    void OnStartup() override { Reload(); }

    void OnAfterConfigLoad(bool reload) override
    {
        if (reload)
            Reload();
    }

    bool OnCanSpawnContinentTransport(TransportTemplate const& transport) override
    {
        std::vector<AreaAccess::Endpoint> const docks = TransportDocks(transport);
        if (CurrentPolicy()->IsRouteAllowed(docks))
            return true;

        LOG_INFO("server.loading", "Area access: transport {} not spawned, a dock is in a locked area ({})",
            transport.entry, Describe(docks));
        return false;
    }
};

class Enforcement : public PlayerScript
{
public:
    Enforcement() : PlayerScript("AscensionAreaAccessEnforcement", { PLAYERHOOK_ON_CAN_TELEPORT_TO,
        PLAYERHOOK_ON_UPDATE_ZONE, PLAYERHOOK_ON_BEFORE_ACTIVATE_TAXI_PATH,
        PLAYERHOOK_ON_LOGIN, PLAYERHOOK_ON_LOGOUT }) { }

    bool OnPlayerCanTeleportTo(Player* player, uint32 mapId, float x, float y, float z, uint32 options) override
    {
        if (evicting)
            return true;

        Map const* source = player->FindMap();
        AreaAccess::Teleport const teleport{IsGameMaster(player),
            AreaAccess::IsForcedMove((options & TELE_TO_NOT_LEAVE_TRANSPORT) != 0, player->GetTransport() != nullptr,
                source && (source->Instanceable() || source->IsBattlegroundOrArena())),
            mapId, ZoneAt(mapId, x, y, z)};
        if (CurrentPolicy()->IsTeleportAllowed(teleport))
            return true;

        Deny(player);
        return false;
    }

    void OnPlayerLogin(Player* player) override
    {
        ResetLockedBind(player);
    }

    void OnPlayerLogout(Player* player) override
    {
        messageThrottle.Forget(player->GetGUID().GetRawValue());
    }

    bool OnPlayerBeforeActivateTaxiPath(Player* player, std::vector<uint32> const& nodes) override
    {
        if (IsTaxiRouteAllowed(player, nodes))
            return true;

        Deny(player);
        return false;
    }

    void OnPlayerUpdateZone(Player* player, uint32 newZone, uint32) override
    {
        ResetLockedBind(player);
        if (IsGameMaster(player) || CurrentPolicy()->IsAllowed(false, player->GetMapId(), newZone))
            return;

        ObjectGuid const guid = player->GetGUID();
        player->m_Events.AddEventAtOffset([guid]() { Evict(guid); }, Milliseconds(1));
    }
};
}

void AddSC_AscensionAreaAccess()
{
    new Configuration();
    new Enforcement();
}
