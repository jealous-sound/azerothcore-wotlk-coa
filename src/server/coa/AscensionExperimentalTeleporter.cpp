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

constexpr uint32 ACTION_SHOW_EASTERN_KINGDOMS = 1000;
constexpr uint32 ACTION_SHOW_KALIMDOR = 1001;
constexpr uint32 ACTION_SHOW_MAIN = 1002;
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
    EasternKingdoms,
    Kalimdor
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

std::array<Destination, 49> const Destinations =
{ {
    { "Stormwind", DestinationGroup::Alliance, 0, -8833.38f, 628.63f, 94.01f, 1.07f },
    { "Ironforge", DestinationGroup::Alliance, 0, -4918.88f, -940.41f, 501.56f, 5.42f },
    { "Darnassus", DestinationGroup::Alliance, 1, 9949.56f, 2284.21f, 1341.4f, 1.6f },
    { "Orgrimmar", DestinationGroup::Horde, 1, 1629.85f, -4373.64f, 31.56f, 3.7f },
    { "Undercity", DestinationGroup::Horde, 0, 1584.14f, 240.31f, -52.15f, 0.04f },
    { "Thunder Bluff", DestinationGroup::Horde, 1, -1277.37f, 124.8f, 131.29f, 5.22f },
    { "Booty Bay", DestinationGroup::Neutral, 0, -14297.2f, 530.99f, 8.78f, 3.99f },
    { "Gadgetzan", DestinationGroup::Neutral, 1, -7177.15f, -3785.34f, 8.37f, 6.1f },
    { "Everlook", DestinationGroup::Neutral, 1, 6725.69f, -4619.44f, 720.91f, 4.67f },
    { "Alterac Mountains", DestinationGroup::EasternKingdoms, 0, 370.76f, -491.36f, 175.36f, 0.0f },
    { "Arathi Highlands", DestinationGroup::EasternKingdoms, 0, -1508.51f, -2732.06f, 32.5f, 0.0f },
    { "Badlands", DestinationGroup::EasternKingdoms, 0, -6779.2f, -3423.64f, 241.67f, 0.0f },
    { "Blasted Lands", DestinationGroup::EasternKingdoms, 0, -11182.5f, -3016.67f, 7.42f, 0.0f },
    { "Burning Steppes", DestinationGroup::EasternKingdoms, 0, -8118.54f, -1633.83f, 133.0f, 0.0f },
    { "Deadwind Pass", DestinationGroup::EasternKingdoms, 0, -10438.8f, -1932.75f, 104.62f, 0.0f },
    { "Dun Morogh", DestinationGroup::EasternKingdoms, 0, -5451.55f, -656.99f, 392.67f, 0.0f },
    { "Duskwood", DestinationGroup::EasternKingdoms, 0, -10898.3f, -364.78f, 39.27f, 0.0f },
    { "Eastern Plaguelands", DestinationGroup::EasternKingdoms, 0, 2300.97f, -4613.36f, 73.62f, 0.0f },
    { "Elwynn Forest", DestinationGroup::EasternKingdoms, 0, -9617.06f, -288.95f, 57.31f, 4.73f },
    { "Hillsbrad Foothills", DestinationGroup::EasternKingdoms, 0, -436.66f, -581.25f, 53.59f, 0.0f },
    { "Loch Modan", DestinationGroup::EasternKingdoms, 0, -5202.94f, -2855.18f, 336.82f, 0.0f },
    { "Redridge Mountains", DestinationGroup::EasternKingdoms, 0, -9551.81f, -2204.73f, 93.47f, 5.47f },
    { "Searing Gorge", DestinationGroup::EasternKingdoms, 0, -7012.47f, -1065.13f, 241.79f, 0.0f },
    { "Silverpine Forest", DestinationGroup::EasternKingdoms, 0, 878.74f, 1359.33f, 50.35f, 5.9f },
    { "Stranglethorn Vale", DestinationGroup::EasternKingdoms, 0, -12644.3f, -377.41f, 10.1f, 6.1f },
    { "Swamp of Sorrows", DestinationGroup::EasternKingdoms, 0, -10345.4f, -2773.42f, 21.99f, 0.0f },
    { "The Hinterlands", DestinationGroup::EasternKingdoms, 0, 119.39f, -3190.37f, 117.33f, 2.34f },
    { "Tirisfal Glades", DestinationGroup::EasternKingdoms, 0, 2036.02f, 161.33f, 33.87f, 0.14f },
    { "Western Plaguelands", DestinationGroup::EasternKingdoms, 0, 1728.65f, -1602.25f, 63.43f, 0.0f },
    { "Westfall", DestinationGroup::EasternKingdoms, 0, -10235.2f, 1222.47f, 43.63f, 0.0f },
    { "Wetlands", DestinationGroup::EasternKingdoms, 0, -3242.81f, -2469.04f, 15.92f, 0.0f },
    { "Ashenvale", DestinationGroup::Kalimdor, 1, 1928.34f, -2165.95f, 93.79f, 0.0f },
    { "Azshara", DestinationGroup::Kalimdor, 1, 3341.36f, -4603.79f, 92.5f, 0.0f },
    { "Darkshore", DestinationGroup::Kalimdor, 1, 5756.25f, 298.51f, 20.6f, 0.0f },
    { "Desolace", DestinationGroup::Kalimdor, 1, -606.4f, 2211.75f, 92.98f, 0.0f },
    { "Durotar", DestinationGroup::Kalimdor, 1, 1007.78f, -4446.22f, 11.2f, 0.0f },
    { "Dustwallow Marsh", DestinationGroup::Kalimdor, 1, -4043.65f, -2991.32f, 36.4f, 0.0f },
    { "Felwood", DestinationGroup::Kalimdor, 1, 4102.25f, -1006.79f, 272.72f, 0.0f },
    { "Feralas", DestinationGroup::Kalimdor, 1, -4841.19f, 1309.44f, 81.39f, 0.0f },
    { "Moonglade", DestinationGroup::Kalimdor, 1, 7654.3f, -2232.87f, 462.11f, 0.0f },
    { "Mulgore", DestinationGroup::Kalimdor, 1, -2192.62f, -736.32f, -13.33f, 0.0f },
    { "Silithus", DestinationGroup::Kalimdor, 1, -7426.87f, 1005.31f, 1.13f, 0.0f },
    { "Stonetalon Mountains", DestinationGroup::Kalimdor, 1, 1570.92f, 1031.52f, 137.96f, 0.0f },
    { "Tanaris", DestinationGroup::Kalimdor, 1, -7931.2f, -3414.28f, 80.74f, 0.0f },
    { "Teldrassil", DestinationGroup::Kalimdor, 1, 10111.3f, 1557.73f, 1324.33f, 0.0f },
    { "The Barrens", DestinationGroup::Kalimdor, 1, 884.54f, -3548.45f, 91.85f, 0.0f },
    { "Thousand Needles", DestinationGroup::Kalimdor, 1, -4969.02f, -1726.89f, -62.13f, 0.0f },
    { "Un'Goro Crater", DestinationGroup::Kalimdor, 1, -7943.22f, -2119.09f, -218.34f, 0.0f },
    { "Winterspring", DestinationGroup::Kalimdor, 1, 6759.18f, -4419.63f, 763.21f, 0.0f }
} };

bool IsZoneDrop(Destination const& destination)
{
    return destination.group == DestinationGroup::EasternKingdoms || destination.group == DestinationGroup::Kalimdor;
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

void ShowMainMenu(Player* player, GameObject* go)
{
    ClearGossipMenuFor(player);
    AddDestinations(player, FactionCapitals(player));
    AddDestinations(player, DestinationGroup::Neutral);
    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Eastern Kingdoms...", GOSSIP_SENDER_MAIN,
        ACTION_SHOW_EASTERN_KINGDOMS);
    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Kalimdor...", GOSSIP_SENDER_MAIN, ACTION_SHOW_KALIMDOR);
    SendGossipMenuFor(player, TELEPORTER_TEXT, go->GetGUID());
}

void ShowZoneMenu(Player* player, GameObject* go, DestinationGroup group)
{
    ClearGossipMenuFor(player);
    AddDestinations(player, group);
    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Back", GOSSIP_SENDER_MAIN, ACTION_SHOW_MAIN);
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
        ShowMainMenu(player, go);
        return true;
    }

    bool OnGossipSelect(Player* player, GameObject* go, uint32, uint32 action) override
    {
        switch (action)
        {
            case ACTION_SHOW_MAIN:
                ShowMainMenu(player, go);
                return true;
            case ACTION_SHOW_EASTERN_KINGDOMS:
                ShowZoneMenu(player, go, DestinationGroup::EasternKingdoms);
                return true;
            case ACTION_SHOW_KALIMDOR:
                ShowZoneMenu(player, go, DestinationGroup::Kalimdor);
                return true;
            default:
                break;
        }

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
