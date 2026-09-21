/*
 * Why a purchase can be quiet, and what this does about it.
 *
 * The client announces a learned spell from its own handler for SMSG_LEARNED_SPELL, and that
 * handler asks one question first: does the row its SpellCustomAttr table holds for this spell
 * carry bit 0x400 of the third attribute dword? If it does, the client fires
 * NOTABLE_SPELL_LEARNED, which is the event ToasTNotificationSystem draws the "New Spell
 * Learned" toast and plays the sound from. If it does not, the client checks whether the spell
 * is marked hidden and otherwise fires nothing at all - which is how a purchase from a book can
 * produce the trainer's chat line and no announcement.
 *
 * That table is patchable over the wire: the client's Extensions.dll registers a handler for its
 * "SMSG_PATCH_SPELL_CUSTOM_ATTR" opcode that reads one row of 0x2C bytes - the row id, the spell
 * id, then nine attribute dwords, exactly as the table holds them - and either updates the row
 * with that id or adds a new one, then rebuilds the spell id index the announce check reads.
 * The row's other fields are copied from the table itself (see SpellbookNotifyData.h), so the
 * only thing this ever changes about a spell is that one bit.
 *
 * The table has no row at all for some of the spells the books sell; those get a row of their
 * own on an id past the table's last, followed by one unchanged copy of the table's first row,
 * because the client only rebuilds that index when a push updates a row it already holds.
 */

#include "Configuration/Config.h"
#include "Log.h"
#include "Player.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "SpellbookNotifyData.h"
#include "spellbook_api.h"
#include "spellbook_notify.h"

#include <algorithm>

namespace
{
    /// The wire number lives with the module's public surface (spellbook_api.h); this file is
    /// the one that sends it, and the test driver is the one that watches for it.
    using Spellbook::SMSG_PATCH_SPELL_CUSTOM_ATTR;

    constexpr char const *ENABLE_KEY = "Spellbook.Notify.Enable";

    /// A row as the client's reader consumes it: eleven dwords, no length prefix, no count.
    constexpr std::size_t ROW_FIELDS = 11;
    constexpr std::size_t ROW_BYTES = ROW_FIELDS * sizeof(uint32);

    void SendRow(Player *player, SpellbookNotifyData::Row const &row)
    {
        uint32 const fields[ROW_FIELDS] = {row.RowId, row.SpellId, row.Field2, row.Field3,
                                           row.Field4, row.Field5, row.Field6, row.Field7,
                                           row.Field8, row.Field9, row.Field10};
        WorldPacket packet(SMSG_PATCH_SPELL_CUSTOM_ATTR, ROW_BYTES);
        packet.append(reinterpret_cast<uint8 const *>(fields), ROW_BYTES);
        player->SendDirectMessage(&packet);
    }

    /// A row whose id is one of ours rather than the table's: the client adds it instead of
    /// updating an existing one, and the index rebuild has to be asked for separately.
    bool AddsRow(SpellbookNotifyData::Row const &row)
    {
        return row.RowId >= SpellbookNotifyData::FIRST_FRESH_ROW_ID;
    }

    SpellbookNotifyData::Row const *FindRow(uint32 spellId)
    {
        auto const &rows = SpellbookNotifyData::Rows;
        auto const found = std::lower_bound(rows.begin(), rows.end(), spellId,
            [](SpellbookNotifyData::Row const &row, uint32 id) { return row.SpellId < id; });

        if (found == rows.end() || found->SpellId != spellId)
            return nullptr;

        return &*found;
    }
}

namespace SpellbookNotify
{
    bool Enabled()
    {
        return sConfigMgr->GetOption<bool>(ENABLE_KEY, true);
    }

    void Push(Player *player, uint32 spellId)
    {
        if (!player || !player->GetSession() || !Enabled())
            return;

        SpellbookNotifyData::Row const *row = FindRow(spellId);
        if (!row)
            return;

        SendRow(player, *row);

        if (AddsRow(*row))
            SendRow(player, SpellbookNotifyData::RefreshRow);

        LOG_DEBUG("module.spellbook", "Pushed the SpellCustomAttr row for {} (row {}, {}) to {}",
                  spellId, row->RowId, AddsRow(*row) ? "added" : "updated", player->GetName());
    }

    void Push(Player *player, std::vector<uint32> const &spellIds)
    {
        for (uint32 spellId : spellIds)
            Push(player, spellId);
    }
}
