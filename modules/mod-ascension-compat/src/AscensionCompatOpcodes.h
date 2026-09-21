/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license:
 * https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

// The compat module's protocol-discovery consumer swallows every Ascension extension opcode it does
// not recognise, and it is registered first, so a script that runs after it never sees one. When a
// module does recognise an opcode - it owns the setting the packet carries - it claims the opcode
// here and the consumer hands that packet over instead of absorbing it.
//
// There is nothing Ascension-specific in this seam: it exists so the custom opcode space has one
// consumer that modules can take single opcodes out of, rather than several scripts racing for the
// same packet in registration order.
#ifndef ASCENSION_COMPAT_OPCODES_H
#define ASCENSION_COMPAT_OPCODES_H

#include "Define.h"

class WorldPacket;
class WorldSession;

namespace AscensionCompatOpcodes
{
    /// Handles one claimed opcode. Returns true when the packet is fully handled. A false return
    /// leaves the packet to the generic consumer, which is what a handler does when it cannot act
    /// on it yet (no player attached, an unexpected payload, and so on).
    using Handler = bool (*)(WorldSession* session, WorldPacket const& packet);

    /// Claim one opcode for a module. Called once per opcode at startup, from that module's loader.
    void Claim(uint16 opcode, Handler handler);

    /// Dispatch a claimed opcode to its handler, if it has one. False means the opcode is unclaimed.
    bool Dispatch(WorldSession* session, WorldPacket const& packet);
}

#endif
