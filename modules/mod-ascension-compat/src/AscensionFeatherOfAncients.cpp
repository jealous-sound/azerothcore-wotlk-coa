/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "Chat.h"
#include "CreatureData.h"
#include "DBCStores.h"
#include "Item.h"
#include "ItemScript.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include <array>
#include <vector>

namespace
{
// CoA is Classic content: only the Azeroth feathers are handled. The TBC and Northrend ones (977026, 977027)
// teach regions this realm does not use.
enum FeatherOfAncients : uint32
{
    FeatherAzerothLegacy = 134989,
    FeatherAzeroth = 977025
};

// A flight master stands within a few yards of its node (Nesingwary Base Camp is the farthest, 11 yards).
constexpr float FlightMasterReach = 40.0f;

bool InMask(TaxiMask const& mask, uint32 node)
{
    return mask[(node - 1) / 32] & (1 << ((node - 1) % 32));
}

// The flight masters' own nodes, per team. Faction masks alone also hold quest and scripted flight nodes
// (their mount is set, and a node without outgoing paths passes the core's check), which no flight
// master serves: a node counts when it is a path origin and a flight master is spawned next to it.
std::array<std::vector<uint32>, 2> const& FlightMasterNodes()
{
    static std::array<std::vector<uint32>, 2> const nodes = []
    {
        std::vector<CreatureData const*> masters;
        for (auto const& [spawnId, data] : sObjectMgr->GetAllCreatureData())
        {
            CreatureTemplate const* creature = sObjectMgr->GetCreatureTemplate(data.id);
            if (creature && ((creature->npcflag | data.npcflag) & UNIT_NPC_FLAG_FLIGHTMASTER))
                masters.push_back(&data);
        }

        std::array<std::vector<uint32>, 2> result;
        for (uint32 node = 1; node < sTaxiNodesStore.GetNumRows(); ++node)
        {
            TaxiNodesEntry const* entry = sTaxiNodesStore.LookupEntry(node);
            if (!entry || !sTaxiPathSetBySource.count(node))
                continue;

            bool served = false;
            for (CreatureData const* master : masters)
            {
                float dx = master->posX - entry->x, dy = master->posY - entry->y, dz = master->posZ - entry->z;
                if (master->mapid == entry->map_id &&
                    dx * dx + dy * dy + dz * dz <= FlightMasterReach * FlightMasterReach)
                {
                    served = true;
                    break;
                }
            }
            if (!served)
                continue;

            if (InMask(sAllianceTaxiNodesMask, node))
                result[TEAM_ALLIANCE].push_back(node);
            if (InMask(sHordeTaxiNodesMask, node))
                result[TEAM_HORDE].push_back(node);
        }
        return result;
    }();
    return nodes;
}

class item_ascension_feather_of_ancients : public ItemScript
{
public:
    item_ascension_feather_of_ancients() : ItemScript("item_ascension_feather_of_ancients") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const&) override
    {
        if (item->GetEntry() != FeatherAzerothLegacy && item->GetEntry() != FeatherAzeroth)
            return false;

        // Finish the client's item request before consuming its inventory object.
        player->SendEquipError(EQUIP_ERR_NONE, item, nullptr);

        TeamId team = player->GetTeamId();
        if (team != TEAM_ALLIANCE && team != TEAM_HORDE)
            return true;

        uint32 learned = 0;
        for (uint32 node : FlightMasterNodes()[team])
        {
            // Kalimdor and the Eastern Kingdoms, plus the Silvermoon, Tranquillien, Exodar and Blood Watch
            // nodes that sit on map 530: the core's own old-continent mask.
            if (!InMask(sOldContinentsNodesMask, node) || !player->m_taxi.SetTaximaskNode(node))
                continue;
            sScriptMgr->OnPlayerLearnTaxiNode(player, node);
            ++learned;
        }

        if (!learned)
        {
            ChatHandler(player->GetSession()).SendNotification("You already know every flight path of Azeroth.");
            return true;
        }

        uint32 count = 1;
        player->DestroyItemCount(item, count, true);

        // The template's two charge-consuming casts would share the deleted item (#401), and the unlock
        // spell 979610 has no effect of its own: learn the paths here instead of casting either.
        WorldPacket discovered(SMSG_NEW_TAXI_PATH, 0);
        player->SendDirectMessage(&discovered);
        return true;
    }
};
}

void AddSC_AscensionFeatherOfAncients()
{
    new item_ascension_feather_of_ancients();
}
