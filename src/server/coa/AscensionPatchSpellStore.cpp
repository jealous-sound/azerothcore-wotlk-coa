/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionPatchSpellStore.h"
#include "AscensionPatchSpellData.h"

namespace AscensionPatchSpells
{
std::optional<WorldPacket> BuildSpellPatch(uint32 spellId, TextOverrides const& overrides)
{
    for (auto const& record : AscensionPatchSpellData::Records)
    {
        if (record.Fields[0] != spellId)
            continue;
        std::vector<uint32> fields(record.Fields.begin(), record.Fields.end());
        std::vector<std::string> strings;
        strings.reserve(record.Strings.size());
        for (std::size_t index = 0; index < record.Strings.size(); ++index)
            strings.emplace_back(overrides[index].has_value() ? *overrides[index] : std::string(record.Strings[index]));
        return Encode(Table::Spell, fields, strings);
    }
    return std::nullopt;
}
}
