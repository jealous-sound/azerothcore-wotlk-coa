// CoA talent state derived from a character's spellbook, and its wire form.
#include "AscensionCoATalentState.h"
#include <algorithm>
#include <cstring>
#include <unordered_set>

namespace AscensionCoATalentState
{
namespace
{
constexpr std::size_t RECORD_SIZE = 21;

void AppendUInt32(std::vector<std::uint8_t>& out, std::uint32_t value)
{
    for (int shift = 0; shift < 32; shift += 8)
        out.push_back(std::uint8_t(value >> shift));
}

std::uint32_t ReadUInt32(std::uint8_t const* data)
{
    return std::uint32_t(data[0]) | (std::uint32_t(data[1]) << 8) | (std::uint32_t(data[2]) << 16) |
        (std::uint32_t(data[3]) << 24);
}

AscensionCompatData::CoATalentEntry const* FindEntry(std::uint32_t entryId)
{
    auto const& entries = AscensionCompatData::CoATalentEntries;
    auto itr = std::lower_bound(entries.begin(), entries.end(), entryId,
        [](AscensionCompatData::CoATalentEntry const& entry, std::uint32_t id) { return entry.EntryId < id; });
    return itr != entries.end() && itr->EntryId == entryId ? &*itr : nullptr;
}
}

std::uint32_t KnownRank(AscensionCompatData::CoATalentEntry const& entry, HasSpell const& hasSpell)
{
    std::uint32_t rank = 0;
    for (std::uint32_t index = 0; index < entry.SpellCount; ++index)
        if (entry.SpellIds[index] && hasSpell(entry.SpellIds[index]))
            rank = index + 1;
    return rank;
}

std::vector<KnownEntry> KnownEntries(std::uint8_t classId, HasSpell const& hasSpell)
{
    std::vector<KnownEntry> known;
    for (AscensionCompatData::CoATalentEntry const& entry : AscensionCompatData::CoATalentEntries)
    {
        if (entry.ClassId != classId)
            continue;
        if (std::uint32_t rank = KnownRank(entry, hasSpell))
            known.push_back({ entry.EntryId, rank });
    }
    return known;
}

SpentPoints Spent(std::vector<KnownEntry> const& known)
{
    SpentPoints spent;
    std::unordered_set<std::uint32_t> charged;
    for (KnownEntry const& item : known)
    {
        AscensionCompatData::CoATalentEntry const* entry = FindEntry(item.EntryId);
        if (!entry || (!entry->AECost && !entry->TECost))
            continue;

        std::uint32_t const ranks = std::min<std::uint32_t>(item.Rank, entry->SpellCount);
        for (std::uint32_t index = 0; index < ranks; ++index)
        {
            if (!charged.insert(entry->SpellIds[index]).second)
                continue;
            if (entry->SpecId)
                spent.TE += entry->TECost;
            else
                spent.AE += entry->AECost;
        }
    }
    return spent;
}

std::vector<std::uint8_t> KnownEntriesPayload(std::vector<KnownEntry> const& known)
{
    std::vector<std::uint8_t> out;
    out.reserve(sizeof(std::uint32_t) + known.size() * RECORD_SIZE);
    AppendUInt32(out, std::uint32_t(known.size()));
    for (KnownEntry const& item : known)
    {
        AppendUInt32(out, item.EntryId);
        AppendUInt32(out, item.Rank);
        AppendUInt32(out, 0);
        out.push_back(0);
        AppendUInt32(out, 0);
        AppendUInt32(out, 0);
    }
    return out;
}

bool ParseKnownEntriesUpload(std::uint8_t const* data, std::size_t size, std::vector<KnownEntry>& known)
{
    known.clear();
    if (!data || size < sizeof(std::uint32_t))
        return false;

    std::uint32_t const count = ReadUInt32(data);
    if (size != sizeof(std::uint32_t) + std::size_t(count) * RECORD_SIZE)
        return false;

    known.reserve(count);
    for (std::uint32_t index = 0; index < count; ++index)
    {
        std::uint8_t const* record = data + sizeof(std::uint32_t) + std::size_t(index) * RECORD_SIZE;
        known.push_back({ ReadUInt32(record), ReadUInt32(record + 4) });
    }
    return true;
}
}
