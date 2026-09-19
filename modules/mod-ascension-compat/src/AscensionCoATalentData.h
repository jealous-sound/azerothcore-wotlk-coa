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

// CharacterAdvancementEssence.dbc row of a custom class: the points its class tree (AE) and its
// specialization tree (TE) hold at a level, cumulative.
struct CoATalentBudget
{
    std::uint8_t ClassId;
    std::uint8_t Level;
    std::uint8_t AE;
    std::uint8_t TE;
};

// Sorted by EntryId.
extern std::vector<CoATalentEntry> CoATalentEntries;
extern std::vector<CoASelectableFreeEntry> CoASelectableFreeEntries;
// Sorted by EntryId.
extern std::vector<CoAAutomaticDependency> CoAAutomaticDependencies;
// Sorted by ClassId, then Level.
extern std::vector<CoATalentBudget> CoATalentBudgets;

// ChrClassesRoles.dbc row: the per-class registry the client's character-advancement window filters that
// class's tree entries with. Sent to the client verbatim, one packet per row. ClassId is the row's first
// dword, Values the ten that follow.
struct CoAChrClassRole
{
    std::uint32_t ClassId;
    std::array<std::uint32_t, 10> Values;
};

extern std::vector<CoAChrClassRole> CoAChrClassRoles;

// The class-tree (AE) and specialization-tree (TE) points a custom class holds at a level: the essence row of
// that level, or of the highest lower level the table has. False when the table has no row for the class.
bool GetCoATalentBudget(std::uint8_t classId, std::uint8_t level, std::uint32_t& ae, std::uint32_t& te);

// Reads CharacterAdvancement.dbc with its class type, tab type, ChrClasses, ChrSpecs and essence tables.
// Returns false and leaves the catalog empty when a table cannot be read.
bool LoadCoATalentData();
}

#endif
