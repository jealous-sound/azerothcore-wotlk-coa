/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license:
 * https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "AscensionCompatOpcodes.h"
#include "Chat.h"
#include "Log.h"
#include "Opcodes.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include <atomic>
#include <mutex>
#include <unordered_map>

namespace
{
    constexpr uint16 CMSG_BATTLEMASTER_JOIN_ARENA_WITH_ROLE = 0x05C1;

    struct ArenaRequest
    {
        uint32 Bracket = 0;
        uint32 Role = 0;
    };

    std::mutex PendingLock;
    std::unordered_map<uint32, ArenaRequest> PendingRequests;
    std::atomic<bool> AnyPending{false};

    bool QueueRequest(WorldSession* session, WorldPacket const& received)
    {
        if (!session)
            return true;

        WorldPacket packet(received);
        packet.rpos(0);
        ArenaRequest request;
        try
        {
            request.Bracket = packet.read<uint32>();
            request.Role = packet.read<uint32>();
        }
        catch (ByteBufferException const&)
        {
            return true;
        }

        std::lock_guard<std::mutex> lock(PendingLock);
        PendingRequests[session->GetAccountId()] = request;
        AnyPending = true;
        return true;
    }

    class AscensionArenaQueuePlayer final : public PlayerScript
    {
    public:
        AscensionArenaQueuePlayer() : PlayerScript("AscensionArenaQueuePlayer", { PLAYERHOOK_ON_UPDATE, PLAYERHOOK_ON_LOGOUT }) { }

        void OnPlayerLogout(Player* player) override
        {
            std::lock_guard<std::mutex> lock(PendingLock);
            PendingRequests.erase(player->GetSession()->GetAccountId());
            AnyPending = !PendingRequests.empty();
        }

        void OnPlayerUpdate(Player* player, uint32) override
        {
            if (!AnyPending)
                return;

            ArenaRequest request;
            {
                std::lock_guard<std::mutex> lock(PendingLock);
                auto itr = PendingRequests.find(player->GetSession()->GetAccountId());
                if (itr == PendingRequests.end())
                    return;
                request = itr->second;
                PendingRequests.erase(itr);
                AnyPending = !PendingRequests.empty();
            }

            LOG_DEBUG("bg.battleground", "Arena join with role from {}: bracket {}, role {}", player->GetName(), request.Bracket, request.Role);

            uint8 slot;
            switch (request.Bracket)
            {
                case 2:
                    slot = 0;
                    break;
                case 3:
                    slot = 1;
                    break;
                default:
                    ChatHandler(player->GetSession()).SendSysMessage("This arena bracket is not available yet.");
                    return;
            }

            WorldPacket join(CMSG_BATTLEMASTER_JOIN_ARENA, 8 + 3);
            join << ObjectGuid::Empty;
            join << slot;
            join << uint8(0);
            join << uint8(0);
            player->GetSession()->HandleBattlemasterJoinArena(join);
        }
    };
}

void AddAscensionArenaQueueScripts()
{
    AscensionCompatOpcodes::Claim(CMSG_BATTLEMASTER_JOIN_ARENA_WITH_ROLE, &QueueRequest);
    new AscensionArenaQueuePlayer();
}
