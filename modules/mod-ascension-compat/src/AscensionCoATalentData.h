// CoA talent catalog: the custom classes' CharacterAdvancement.dbc nodes, loaded at startup.
#ifndef ASCENSION_COA_TALENT_DATA_H
#define ASCENSION_COA_TALENT_DATA_H

#include <array>
#include <cstdint>
#include <vector>

namespace AscensionCompatData
{
struct CoATalentEntry
{
    std::uint32_t EntryId;
    std::uint8_t ClassId;
    std::uint16_t SpecId;
    std::uint8_t SpellCount;
    std::uint8_t AECost;
    std::uint8_t TECost;
    std::uint8_t RequiredLevel;
    std::array<std::uint32_t, 3> SpellIds;
};

struct CoASelectableFreeEntry
{
    std::uint32_t EntryId;
    std::uint32_t GroupId;
};

struct CoAAutomaticDependency
{
    std::uint32_t EntryId;
    std::array<std::uint32_t, 2> RequiredEntryIds;
};

// Sorted by EntryId.
extern std::vector<CoATalentEntry> CoATalentEntries;
extern std::vector<CoASelectableFreeEntry> CoASelectableFreeEntries;
// Sorted by EntryId.
extern std::vector<CoAAutomaticDependency> CoAAutomaticDependencies;

// Reads CharacterAdvancement.dbc with its class type, tab type, ChrClasses and ChrSpecs tables.
// Returns false and leaves the catalog empty when a table cannot be read.
bool LoadCoATalentData();
}

#endif
