/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */
#ifndef ASCENSION_CHARACTER_SELECTION_H
#define ASCENSION_CHARACTER_SELECTION_H

#include "Define.h"

class WorldSession;
class WorldPacket;

// Ascension character-selection protocol (Extensions.dll, 2026-07 build).
// The custom client layers its character screen on top of the standard enum
// (names taken from the client's opcode table):
//   CMSG 0x072E CharacterSelection ActivateCharacter(guid)
//   CMSG 0x072F CharacterSelection DeactivateCharacter(guid)
//   CMSG 0x0772 CHARACTER_SELECTION_SET_SORT_ORDER(payload)
//   SMSG 0x075E CHARACTER_LIST_INFO (four counts, then one entry per character)
//   SMSG 0x075F/0x0760 activate/deactivate results
//   SMSG 0x076F CHARACTER_SELECTION_SORT_ORDER
//   SMSG 0x0770 CHARACTER_SELECTION_MAIL      (per character: list index, hasMail, hasStoreMail)
//   SMSG 0x0771 CHARACTER_SELECTION_GAME_MODE (per character: list index, active/enabled modes,
//                                              team, mercenary, ruleset -> banner icon)
//   SMSG 0x09D0 (SMSG_CHARACTER_CUSTOMIZATION_UNLOCKS) / 0x09BC / 0x06E5
//                              realm-info cluster sent with the character list:
//                              0x09BC carries the realm parameters and the
//                              eight feature-gate bytes that Extensions.dll
//                              copies into DLL+0xBDB178 (required for the
//                              Character Advancement UI to render at all).
// The four counts of SMSG 0x075E are max, total, active and inactive, in that
// order: the client derives its create gate from them (it offers a new
// character while active < max).
// The per-character packets are keyed by the character's 1-based position in the
// character list (the value the client passes to GetCharacterSelectionGameModeData),
// not by guid; inactive characters are not part of that list.
// These requests arrive while the session is STATUS_AUTHED (there is no Player),
// so they are handled from the early extension sink and stay account-scoped.
bool IsAscensionCharacterSelectionOpcode(uint16 opcode);

// Returns true when the packet was consumed by this feature.
bool HandleAscensionCharacterSelectionPacket(WorldSession* session, WorldPacket const& packet);

// Called when the client asks for its character list; sends the Ascension list
// info, the stored sort order and the per-character data after the core's own
// SMSG_CHAR_ENUM handling. Does nothing when the feature is disabled.
void SendAscensionCharacterListInfo(WorldSession* session);

#endif // ASCENSION_CHARACTER_SELECTION_H
