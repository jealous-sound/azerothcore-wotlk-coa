/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "Map.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptMgr.h"

namespace
{
constexpr char const* PENDING_DUNGEON_RELEASE = "ascension_dungeon_release";
constexpr float RELEASE_RESTORE_PERCENT = 0.5f;

struct PendingDungeonRelease : DataMap::Base { };

AreaTriggerTeleport const* NearestInstanceStart(Player* player)
{
    AreaTriggerTeleport const* nearest = nullptr;
    float nearestDistance = 0.0f;
    for (auto const& [triggerId, teleport] : sObjectMgr->GetAllAreaTriggerTeleports())
    {
        if (teleport.target_mapId != player->GetMapId())
            continue;

        float const distance = player->GetExactDistSq(teleport.target_X, teleport.target_Y, teleport.target_Z);
        if (!nearest || distance < nearestDistance)
        {
            nearest = &teleport;
            nearestDistance = distance;
        }
    }
    return nearest;
}

class ascension_dungeon_release_player : public PlayerScript
{
public:
    ascension_dungeon_release_player()
        : PlayerScript("ascension_dungeon_release_player",
            {PLAYERHOOK_ON_PLAYER_RELEASED_GHOST, PLAYERHOOK_CAN_REPOP_AT_GRAVEYARD}) { }

    void OnPlayerReleasedGhost(Player* player) override
    {
        if (player->GetMap()->IsDungeon())
            player->CustomData.GetDefault<PendingDungeonRelease>(PENDING_DUNGEON_RELEASE);
    }

    bool OnPlayerCanRepopAtGraveyard(Player* player) override
    {
        if (!player->CustomData.Erase(PENDING_DUNGEON_RELEASE))
            return true;

        if (player->IsAlive() || !player->GetMap()->IsDungeon() || !player->m_InstanceValid)
            return true;

        AreaTriggerTeleport const* start = NearestInstanceStart(player);
        if (!start)
            return true;

        player->ResurrectPlayer(RELEASE_RESTORE_PERCENT);
        if (!player->IsAlive())
            return true;

        player->SpawnCorpseBones();
        player->NearTeleportTo(start->target_X, start->target_Y, start->target_Z, start->target_Orientation);
        return false;
    }
};
}

void AddSC_AscensionDungeonRelease()
{
    new ascension_dungeon_release_player();
}
