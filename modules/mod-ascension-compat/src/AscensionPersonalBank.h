/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#ifndef MOD_ASCENSION_PERSONAL_BANK_H
#define MOD_ASCENSION_PERSONAL_BANK_H

#include "Define.h"
#include "ObjectGuid.h"

class Player;
class WorldPacket;

/// The storage behind the vault window the Personal Bank, Celestial Personal Bank and
/// Realm Bank items open. The client speaks the guild-bank protocol there, so this
/// module answers those opcodes itself and keeps the contents per character (or, for the
/// Realm Bank, per account, which its characters share).
namespace AscensionPersonalBank
{
    enum Kind : uint8
    {
        PERSONAL = 0,
        REALM = 1
    };

    /// One of our own bank objects was clicked: load the bank and send the frame its
    /// rights and contents. `vault` is the object the window is anchored to - once it is
    /// gone, or the character walks away from it, the bank closes again.
    void Opened(Player* player, uint8 kind, ObjectGuid vault);

    /// True while this character has one of our bank windows open. While that is the case
    /// the guild bank opcodes must come here instead of the core's guild handling.
    [[nodiscard]] bool IsOpen(Player* player);

    /// Answers one of the bank opcodes. Returns true when the packet belonged to the
    /// personal bank and has been answered here.
    bool HandlePacket(Player* player, WorldPacket const& packet);

    /// The character is gone: persist nothing (changes are written as they happen) and
    /// release the loaded items.
    void Closed(Player* player);

    /// Adds a tab to the bank the voucher names - `kind` picks the personal bank or the
    /// realm-wide one, which is the only difference between the two vouchers. Returns false
    /// when that bank already owns every tab.
    bool AddTab(Player* player, uint8 kind);

    /// Tells the frame which bank it is looking at, which is what puts it in its personal
    /// presentation. The frame's BANK_PERMISSIONS_PAYLOAD hook also re-selects its first tab
    /// while it is showing, so sending this again re-draws the tab strip: that is how a tab
    /// unlocked (or bought) while the window is open stops being drawn as the buy tab.
    void SendKindHint(Player* player, uint8 kind);
}

#endif // MOD_ASCENSION_PERSONAL_BANK_H
