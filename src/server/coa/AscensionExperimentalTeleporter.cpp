/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "Chat.h"
#include "GameObject.h"
#include "GameObjectScript.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include <array>

namespace
{
constexpr uint32 TELEPORTER_TEXT = 61004;
constexpr uint32 TELEPORT_FEE = 8 * GOLD;
constexpr uint32 SPELL_TELEPORTER_LOCKOUT = 985513;
constexpr uint32 SPELL_PARACHUTE = 45472;
constexpr float ZONE_DROP_HEIGHT = 120.0f;
constexpr char const* PENDING_ARRIVAL = "coa_experimental_teleporter_arrival";

constexpr uint32 DESTINATION_ACTION_BASE = 100;

struct PendingArrival : DataMap::Base
{
    bool parachute = false;
};

enum class DestinationGroup : uint8
{
    Alliance,
    Horde,
    Neutral,
    Zone
};

struct Destination
{
    char const* name;
    DestinationGroup group;
    uint32 map;
    float x;
    float y;
    float z;
    float orientation;
};

std::array<Destination, 12> const Destinations =
{ {
    { "Stormwind", DestinationGroup::Alliance, 0, -8833.38f, 628.63f, 94.01f, 1.07f },
    { "Ironforge", DestinationGroup::Alliance, 0, -4918.88f, -940.41f, 501.56f, 5.42f },
    { "Darnassus", DestinationGroup::Alliance, 1, 9949.56f, 2284.21f, 1341.4f, 1.6f },
    { "Orgrimmar", DestinationGroup::Horde, 1, 1629.85f, -4373.64f, 31.56f, 3.7f },
    { "Undercity", DestinationGroup::Horde, 0, 1584.14f, 240.31f, -52.15f, 0.04f },
    { "Thunder Bluff", DestinationGroup::Horde, 1, -1277.37f, 124.8f, 131.29f, 5.22f },
    { "Booty Bay", DestinationGroup::Neutral, 0, -14297.2f, 530.99f, 8.78f, 3.99f },
    { "Burning Steppes", DestinationGroup::Zone, 0, -8118.54f, -1633.83f, 133.0f, 0.0f },
    { "Western Plaguelands", DestinationGroup::Zone, 0, 1728.65f, -1602.25f, 63.43f, 0.0f },
    { "Silithus", DestinationGroup::Zone, 1, -7426.87f, 1005.31f, 1.13f, 0.0f },
    { "Un'Goro Crater", DestinationGroup::Zone, 1, -7943.22f, -2119.09f, -218.34f, 0.0f },
    { "Winterspring", DestinationGroup::Zone, 1, 6759.18f, -4419.63f, 763.21f, 0.0f }
} };

bool IsZoneDrop(Destination const& destination)
{
    return destination.group == DestinationGroup::Zone;
}

DestinationGroup FactionCapitals(Player const* player)
{
    return player->GetTeamId() == TEAM_ALLIANCE ? DestinationGroup::Alliance : DestinationGroup::Horde;
}

void AddDestinations(Player* player, DestinationGroup group)
{
    for (uint32 index = 0; index < Destinations.size(); ++index)
    {
        Destination const& destination = Destinations[index];
        if (destination.group != group)
            continue;

        AddGossipItemFor(player, GOSSIP_ICON_TAXI, destination.name, GOSSIP_SENDER_MAIN,
            DESTINATION_ACTION_BASE + index, std::string("Teleport to ") + destination.name + "?", TELEPORT_FEE, false);
    }
}

void ShowMenu(Player* player, GameObject* go)
{
    ClearGossipMenuFor(player);
    AddDestinations(player, FactionCapitals(player));
    AddDestinations(player, DestinationGroup::Neutral);
    AddDestinations(player, DestinationGroup::Zone);
    SendGossipMenuFor(player, TELEPORTER_TEXT, go->GetGUID());
}

bool IsOffered(Player const* player, Destination const& destination)
{
    return destination.group == FactionCapitals(player) || destination.group == DestinationGroup::Neutral
        || IsZoneDrop(destination);
}

void Arrive(Player* player, bool parachute)
{
    if (parachute)
        player->CastSpell(player, SPELL_PARACHUTE, true);
    sScriptMgr->OnPlayerCoAProgress(player, CoAProgressEvent::ExperimentalTeleporter, 0);
}

void Teleport(Player* player, Destination const& destination)
{
    if (player->HasAura(SPELL_TELEPORTER_LOCKOUT))
    {
        ChatHandler(player->GetSession()).SendNotification("The teleporter is still recharging.");
        return;
    }

    if (!player->HasEnoughMoney(TELEPORT_FEE))
    {
        player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, nullptr, 0, 0);
        return;
    }

    bool const zoneDrop = IsZoneDrop(destination);
    float const z = zoneDrop ? destination.z + ZONE_DROP_HEIGHT : destination.z;
    bool const sameMap = player->GetMapId() == destination.map;
    if (!player->TeleportTo(destination.map, destination.x, destination.y, z, destination.orientation))
        return;

    player->ModifyMoney(-int32(TELEPORT_FEE));
    player->CastSpell(player, SPELL_TELEPORTER_LOCKOUT, true);
    if (sameMap)
        Arrive(player, zoneDrop);
    else
        player->CustomData.GetDefault<PendingArrival>(PENDING_ARRIVAL)->parachute = zoneDrop;
}
}

class go_coa_experimental_teleporter : public GameObjectScript
{
public:
    go_coa_experimental_teleporter() : GameObjectScript("go_coa_experimental_teleporter") { }

    bool OnGossipHello(Player* player, GameObject* go) override
    {
        ShowMenu(player, go);
        return true;
    }

    bool OnGossipSelect(Player* player, GameObject*, uint32, uint32 action) override
    {
        CloseGossipMenuFor(player);
        if (action < DESTINATION_ACTION_BASE || action - DESTINATION_ACTION_BASE >= Destinations.size())
            return true;

        Destination const& destination = Destinations[action - DESTINATION_ACTION_BASE];
        if (IsOffered(player, destination))
            Teleport(player, destination);
        return true;
    }
};

class coa_experimental_teleporter_arrival : public PlayerScript
{
public:
    coa_experimental_teleporter_arrival()
        : PlayerScript("coa_experimental_teleporter_arrival", {PLAYERHOOK_ON_MAP_CHANGED}) { }

    void OnPlayerMapChanged(Player* player) override
    {
        PendingArrival const* pending = player->CustomData.Get<PendingArrival>(PENDING_ARRIVAL);
        if (!pending)
            return;

        bool const parachute = pending->parachute;
        player->CustomData.Erase(PENDING_ARRIVAL);
        Arrive(player, parachute);
    }
};

void AddSC_AscensionExperimentalTeleporter()
{
    new go_coa_experimental_teleporter();
    new coa_experimental_teleporter_arrival();
}
