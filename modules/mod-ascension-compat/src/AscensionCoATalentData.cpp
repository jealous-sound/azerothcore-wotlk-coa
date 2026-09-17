/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionCoATalentData.h"
#include "ClientDBC.h"
#include "DBCStores.h"
#include "Log.h"
#include <algorithm>
#include <cctype>
#include <map>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace AscensionCompatData
{
std::vector<CoATalentEntry> CoATalentEntries;
std::vector<CoASelectableFreeEntry> CoASelectableFreeEntries;
std::vector<CoAAutomaticDependency> CoAAutomaticDependencies;

namespace
{
// CharacterAdvancementTabTypes row of the tree every specialization of a class shares.
constexpr uint32 CLASS_TAB = 87;

// ChrSpecs names each specialization's identity passive, a Level 10 Passive that must not wait for a purchased
// class ability. Infernus and Corrupting Whispers do not state Level 10 Passive and keep their authored gates.
constexpr std::array<uint32, 2> IDENTITY_PASSIVE_EXCLUSIONS = { 4037, 4041 };

// Barbarian's zero-cost grouped alternatives are choices the player makes, not automatic level grants.
constexpr std::array<uint32, 8> BARBARIAN_FREE_CHOICES = { 9172, 9861, 11172, 11257, 12112, 13111, 30764, 34257 };

// CharacterAdvancement.dbc DWORDs; the record's byte fields start after these.
enum AdvancementField : uint32
{
    ADVANCEMENT_ID           = 0,
    ADVANCEMENT_REQUIRED     = 2,  // 3 entry IDs
    ADVANCEMENT_SPELLS       = 5,  // 5 rank spells
    ADVANCEMENT_AE_COST      = 14,
    ADVANCEMENT_TE_COST      = 15,
    ADVANCEMENT_LEVEL        = 26,
    ADVANCEMENT_GROUP        = 29,
    ADVANCEMENT_CLASS_TYPE   = 32,
    ADVANCEMENT_TAB          = 33,
};

bool Contains(auto const& values, uint32 value)
{
    return std::find(values.begin(), values.end(), value) != values.end();
}

std::string Upper(std::string_view text)
{
    std::string result(text);
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return char(std::toupper(c)); });
    return result;
}

struct Node
{
    CoATalentEntry Entry;
    std::vector<uint32> Required;
    uint32 Group;
    bool ClassTab;
};
}

bool LoadCoATalentData()
{
    CoATalentEntries.clear();
    CoASelectableFreeEntries.clear();
    CoAAutomaticDependencies.clear();

    ClientDBC classes, classTypes, tabTypes, specs, advancement;
    if (!classes.Load(GetClientDBCPath("ChrClasses.dbc"), 56) ||
        !classTypes.Load(GetClientDBCPath("CharacterAdvancementClassTypes.dbc"), 5) ||
        !tabTypes.Load(GetClientDBCPath("CharacterAdvancementTabTypes.dbc"), 2) ||
        !specs.Load(GetClientDBCPath("ChrSpecs.dbc"), 29) ||
        !advancement.Load(GetClientDBCPath("CharacterAdvancement.dbc"), ADVANCEMENT_TAB + 1))
        return false;

    std::unordered_map<uint32, std::string> classTokens;
    for (uint32 row = 0; row < classes.GetRecordCount(); ++row)
        classTokens[classes.GetRecord(row).GetUInt32(0)] = std::string(classes.GetRecord(row).GetString(55));

    std::unordered_map<uint32, std::pair<uint32, bool>> classTypeById; // class ID, custom class
    for (uint32 row = 0; row < classTypes.GetRecordCount(); ++row)
    {
        ClientDBC::Record record = classTypes.GetRecord(row);
        classTypeById[record.GetUInt32(0)] = { record.GetUInt32(2), record.GetUInt32(4) != 0 };
    }

    std::unordered_map<uint32, std::string> tabTokens;
    for (uint32 row = 0; row < tabTypes.GetRecordCount(); ++row)
        tabTokens[tabTypes.GetRecord(row).GetUInt32(0)] = Upper(tabTypes.GetRecord(row).GetString(1));

    std::map<std::pair<std::string, std::string>, uint32> specByClassAndTab;
    std::unordered_map<uint32, uint32> identitySpecByEntry;
    for (uint32 row = 0; row < specs.GetRecordCount(); ++row)
    {
        ClientDBC::Record record = specs.GetRecord(row);
        std::pair<std::string, std::string> key(record.GetString(1), record.GetString(2));
        specByClassAndTab[key] = record.GetUInt32(0);
        if (uint32 identity = record.GetUInt32(28))
            identitySpecByEntry[identity] = record.GetUInt32(0);
    }

    std::vector<Node> nodes;
    for (uint32 row = 0; row < advancement.GetRecordCount(); ++row)
    {
        ClientDBC::Record record = advancement.GetRecord(row);
        uint32 const entryId = record.GetUInt32(ADVANCEMENT_ID);
        auto classType = classTypeById.find(record.GetUInt32(ADVANCEMENT_CLASS_TYPE));
        if (classType == classTypeById.end() || !classType->second.second ||
            classType->second.first < 12 || classType->second.first > 32)
            continue;

        uint32 const classId = classType->second.first;
        uint32 const tab = record.GetUInt32(ADVANCEMENT_TAB);
        uint32 specId = 0;
        if (tab != CLASS_TAB)
        {
            auto spec = specByClassAndTab.find({ classTokens[classId], tabTokens[tab] });
            if (spec == specByClassAndTab.end())
                continue;
            specId = spec->second;
        }

        Node node{};
        node.Entry.EntryId = entryId;
        node.Entry.ClassId = uint8(classId);
        node.Entry.SpecId = uint16(specId);
        node.Entry.AECost = uint8(record.GetUInt32(ADVANCEMENT_AE_COST));
        node.Entry.TECost = uint8(record.GetUInt32(ADVANCEMENT_TE_COST));
        node.Entry.RequiredLevel = uint8(record.GetUInt32(ADVANCEMENT_LEVEL));
        node.Group = record.GetUInt32(ADVANCEMENT_GROUP);
        node.ClassTab = tab == CLASS_TAB;

        bool tooManyRanks = false;
        for (uint32 field = ADVANCEMENT_SPELLS; field < ADVANCEMENT_SPELLS + 5; ++field)
        {
            uint32 const spellId = record.GetUInt32(field);
            if (!spellId)
                continue;
            if (node.Entry.SpellCount == node.Entry.SpellIds.size())
            {
                tooManyRanks = true;
                break;
            }
            node.Entry.SpellIds[node.Entry.SpellCount++] = spellId;
        }

        if (tooManyRanks)
        {
            LOG_ERROR("module.ascension_compat", "Skipped CoA talent entry {} with more than 3 ranks", entryId);
            continue;
        }

        for (uint32 field = ADVANCEMENT_REQUIRED; field < ADVANCEMENT_REQUIRED + 3; ++field)
            if (uint32 requiredId = record.GetUInt32(field))
                node.Required.push_back(requiredId);

        auto identity = identitySpecByEntry.find(entryId);
        if (identity != identitySpecByEntry.end() && identity->second == specId &&
            !Contains(IDENTITY_PASSIVE_EXCLUSIONS, entryId))
        {
            node.Entry.RequiredLevel = 10;
            node.Required.clear();
        }

        nodes.push_back(std::move(node));
    }

    std::sort(nodes.begin(), nodes.end(), [](Node const& left, Node const& right)
    {
        return left.Entry.EntryId < right.Entry.EntryId;
    });

    std::unordered_map<uint32, Node const*> nodeById;
    for (Node const& node : nodes)
        nodeById[node.Entry.EntryId] = &node;

    for (Node const& node : nodes)
    {
        CoATalentEntries.push_back(node.Entry);
        if (Contains(BARBARIAN_FREE_CHOICES, node.Entry.EntryId))
            CoASelectableFreeEntries.push_back({ node.Entry.EntryId, node.Group });

        if (node.Entry.AECost || node.Entry.TECost || node.Required.empty())
            continue;

        // A free specialization node never waits for a purchased node of the shared class tree.
        std::vector<uint32> required;
        for (uint32 requiredId : node.Required)
        {
            auto requiredNode = nodeById.find(requiredId);
            bool const paidClassNode = requiredNode != nodeById.end() && requiredNode->second->ClassTab &&
                (requiredNode->second->Entry.AECost || requiredNode->second->Entry.TECost);
            if (!(node.Entry.SpecId && !Contains(BARBARIAN_FREE_CHOICES, node.Entry.EntryId) && paidClassNode))
                required.push_back(requiredId);
        }

        if (required.empty())
            continue;

        if (required.size() > 2)
        {
            LOG_ERROR("module.ascension_compat", "Skipped CoA talent entry {} dependencies: {} required entries",
                node.Entry.EntryId, required.size());
            continue;
        }

        CoAAutomaticDependency dependency{ node.Entry.EntryId, {} };
        std::copy(required.begin(), required.end(), dependency.RequiredEntryIds.begin());
        CoAAutomaticDependencies.push_back(dependency);
    }

    LOG_INFO("module.ascension_compat", "Loaded {} CoA talent entries ({} selectable free, {} automatic dependencies)",
        CoATalentEntries.size(), CoASelectableFreeEntries.size(), CoAAutomaticDependencies.size());
    return true;
}
}
