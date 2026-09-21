/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license:
 * https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

// The claim registry behind AscensionCompatOpcodes.h. Its own file so the registry lives at
// global scope: AscensionCompat.cpp keeps most of its helpers inside one anonymous namespace, and
// a namespace defined there would be a different namespace from the one modules include.
#include "AscensionCompatOpcodes.h"

#include "WorldPacket.h"
#include "WorldSession.h"

#include <utility>
#include <vector>

namespace AscensionCompatOpcodes
{
    namespace
    {
        /// One entry per opcode a module owns. Filled during startup, before the world accepts
        /// connections, and read-only afterwards, so lookups need no lock.
        std::vector<std::pair<uint16, Handler>>& ClaimedOpcodes()
        {
            static std::vector<std::pair<uint16, Handler>> claimed;
            return claimed;
        }
    }

    void Claim(uint16 opcode, Handler handler)
    {
        if (!handler)
            return;

        ClaimedOpcodes().emplace_back(opcode, handler);
    }

    bool Dispatch(WorldSession* session, WorldPacket const& packet)
    {
        uint16 const opcode = packet.GetOpcode();
        for (auto const& [claimed, handler] : ClaimedOpcodes())
            if (claimed == opcode)
                return handler(session, packet);
        return false;
    }
}
