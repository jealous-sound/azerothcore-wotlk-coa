/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "Group.h"
#include "LFGMgr.h"
#include "Map.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"

namespace
{
constexpr uint16 SMSG_LFG_UPDATE_CUSTOM = 0x066D;

class lfg_objective_dungeon : public PlayerScript
{
public:
    lfg_objective_dungeon() : PlayerScript("lfg_objective_dungeon", {PLAYERHOOK_ON_MAP_CHANGED}) { }

    void OnPlayerMapChanged(Player* player) override
    {
        WorldSession* session = player ? player->GetSession() : nullptr;
        if (!session)
            return;

        uint32 dungeonId = 0;
        Map const* map = player->GetMap();
        Group const* group = player->GetGroup();
        if (map && group && group->isLFGGroup()
            && sLFGMgr->inLfgDungeonMap(group->GetGUID(), map->GetId(), map->GetDifficulty()))
            dungeonId = sLFGMgr->GetDungeon(group->GetGUID());

        WorldPacket packet(SMSG_LFG_UPDATE_CUSTOM, 4);
        packet << uint32(dungeonId);
        session->SendPacket(&packet);
    }
};
}

void AddSC_AscensionLfgObjective()
{
    new lfg_objective_dungeon();
}
