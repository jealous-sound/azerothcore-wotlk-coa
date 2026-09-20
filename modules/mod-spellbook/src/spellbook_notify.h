// The announcement half of the book.
//
// A purchase learns the spell server side either way; whether the client *shows* anything is
// its own decision, and it decides from the row its SpellCustomAttr table holds for the spell.
// This pushes that row with the bit its learn handler tests, so a purchase always gets the
// "New Spell Learned" toast and its sound, whatever the table said before.
#ifndef SPELLBOOK_NOTIFY_H
#define SPELLBOOK_NOTIFY_H

#include <cstdint>
#include <vector>

class Player;

namespace SpellbookNotify
{
    /// Whether the push is switched on (Spellbook.Notify.Enable).
    bool Enabled();

    /// Marks one spell worth announcing, for as long as the client is running. Idempotent, and
    /// silent about spells a book does not offer (there is no row to push for those).
    void Push(Player *player, std::uint32_t spellId);

    /// The same for a list, in order. The bulk "learn everything" action uses it per spell, so
    /// each learn arrives announced exactly as a single purchase is.
    void Push(Player *player, std::vector<std::uint32_t> const &spellIds);
}

#endif
