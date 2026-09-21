// CoA talent state derived from a character's spellbook: which catalog entries it holds at which rank, what those
// ranks cost, which automatic grants a build carries, and the wire form the client's character-advancement service
// exchanges for that state.
#ifndef ASCENSION_COA_TALENT_STATE_H
#define ASCENSION_COA_TALENT_STATE_H

#include "AscensionCoATalentData.h"
#include <cstddef>
#include <cstdint>
#include <functional>
#include <vector>

namespace AscensionCoATalentState
{
using HasSpell = std::function<bool(std::uint32_t)>;
using RequirementHeld = std::function<bool(AscensionCompatData::CoATalentEntry const&)>;

struct KnownEntry
{
    std::uint32_t EntryId;
    std::uint32_t Rank;
};

// The rank of one entry: the highest rank whose spell the character owns, 0 when it owns none.
std::uint32_t KnownRank(AscensionCompatData::CoATalentEntry const& entry, HasSpell const& hasSpell);

// Every entry of the class the character holds a rank of, automatic ones included. Sorted by entry id.
std::vector<KnownEntry> KnownEntries(std::uint8_t classId, HasSpell const& hasSpell);

// The rule behind every automatic grant: a cost-free catalog entry of that class and specialization at
// that level, not a selectable free choice, whose CoAAutomaticDependencies row's entries the build holds
// (`requirementHeld` answers for one of them). The live grant pass answers its requirements from the
// player's spellbook, the per-slot derivation from the slot's own picks, so both read the same rule.
bool IsAutomaticEntryAvailable(AscensionCompatData::CoATalentEntry const& entry, std::uint8_t classId,
                               std::uint16_t specializationId, std::uint8_t level,
                               RequirementHeld const& requirementHeld);

// The entries a build holds without owning a rank spell of them, through the rule above over the build's
// own known set. A slot that is not the active one is described from its stored picks instead of the
// live spellbook, which holds the active slot's grants. Ranks follow what KnownRank reports after the
// module's top-rank grant. Sorted by entry id.
std::vector<KnownEntry> AutomaticEntries(std::uint8_t classId, std::uint16_t specializationId,
                                         std::uint8_t level, std::vector<KnownEntry> const& held);

struct SpentPoints
{
    std::uint32_t AE = 0; // class tree
    std::uint32_t TE = 0; // specialization trees
};

// What the known paid ranks cost. A rank spell that two entries share is charged once, to the lower entry id.
SpentPoints Spent(std::vector<KnownEntry> const& known);

// SMSG_CHARACTER_ADVANCEMENT_KNOWN_ENTRIES body: u32 count, then per entry u32 id, u32 rank and the four fields
// the client's record carries, as the live realm sends them in the capture: u32 1, u8 0, u32 build timestamp,
// u32 0. 21 bytes per record.
std::vector<std::uint8_t> KnownEntriesPayload(std::vector<KnownEntry> const& known);

// CMSG_CHARACTER_ADVANCEMENT_KNOWN_ENTRIES body, the same records as above: the client's complete known set, not
// a delta. False unless the body is exactly the announced number of records.
bool ParseKnownEntriesUpload(std::uint8_t const* data, std::size_t size, std::vector<KnownEntry>& known);
}

#endif
