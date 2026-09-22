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

struct KnownEntry
{
    std::uint32_t EntryId;
    std::uint32_t Rank;
};

std::uint32_t KnownRank(AscensionCompatData::CoATalentEntry const& entry, HasSpell const& hasSpell);

std::vector<KnownEntry> KnownEntries(std::uint8_t classId, HasSpell const& hasSpell);

struct SpentPoints
{
    std::uint32_t AE = 0;
    std::uint32_t TE = 0;
};

SpentPoints Spent(std::vector<KnownEntry> const& known);

std::vector<std::uint8_t> KnownEntriesPayload(std::vector<KnownEntry> const& known);

bool ParseKnownEntriesUpload(std::uint8_t const* data, std::size_t size, std::vector<KnownEntry>& known);
}

#endif
