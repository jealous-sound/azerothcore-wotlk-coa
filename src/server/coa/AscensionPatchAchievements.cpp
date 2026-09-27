/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionPatchAchievements.h"
#include "AscensionPatchAchievementData.h"
#include "Log.h"
#include "Player.h"

namespace AscensionPatchAchievements
{
namespace
{
struct Layout
{
    std::size_t Prefix;
    std::size_t Strings;
    std::size_t Suffix;
    bool CString;
};

std::optional<Layout> GetLayout(Table table)
{
    switch (table)
    {
        case Table::Achievement: return Layout{14, 3, 0, false};
        case Table::Criteria: return Layout{9, 16, 5, false};
        case Table::Category: return Layout{2, 16, 2, false};
    }
    return std::nullopt;
}

bool IsPointer(Table table, std::size_t index)
{
    return table == Table::Achievement && (index == 4 || index == 5 || index == 11);
}
}

std::optional<WorldPacket> Encode(Table table, std::vector<uint32> const& fields,
    std::vector<std::string> const& strings)
{
    auto layout = GetLayout(table);
    if (!layout || fields.size() != layout->Prefix + layout->Suffix || strings.size() != layout->Strings)
        return std::nullopt;
    std::size_t size = fields.size() * sizeof(uint32);
    for (std::string const& text : strings)
    {
        if (text.size() > 4096 || text.find('\0') != std::string::npos)
            return std::nullopt;
        size += (layout->CString ? 1 : sizeof(uint32)) + text.size();
    }
    if (size > 65533)
        return std::nullopt;
    WorldPacket packet(static_cast<uint16>(table), size);
    auto appendFields = [&](std::size_t begin, std::size_t end)
    {
        for (std::size_t index = begin; index < end; ++index)
            packet << uint32(IsPointer(table, index) ? 0 : fields[index]);
    };
    appendFields(0, layout->Prefix);
    for (std::string const& text : strings)
    {
        if (!layout->CString)
            packet << uint32(text.size());
        if (!text.empty())
            packet.append(text.data(), text.size());
        if (layout->CString)
            packet << uint8(0);
    }
    appendFields(layout->Prefix, fields.size());
    return packet;
}

std::optional<WorldPacket> BuildPatch(Table table, uint32 id, TextOverrides const& overrides)
{
    auto buildRecord = [&](auto const& records) -> std::optional<WorldPacket>
    {
        for (auto const& record : records)
        {
            if (record.Fields[0] != id)
                continue;
            if (overrides.size() != record.Strings.size())
                return std::nullopt;
            std::vector<uint32> fields(record.Fields.begin(), record.Fields.end());
            std::vector<std::string> strings;
            strings.reserve(record.Strings.size());
            for (std::size_t index = 0; index < record.Strings.size(); ++index)
                strings.emplace_back(overrides[index].has_value() ? *overrides[index] :
                    std::string(record.Strings[index]));
            return Encode(table, fields, strings);
        }
        return std::nullopt;
    };
    switch (table)
    {
        case Table::Achievement: return buildRecord(AscensionPatchAchievementData::Achievements);
        case Table::Criteria: return buildRecord(AscensionPatchAchievementData::Criteria);
        case Table::Category: return buildRecord(AscensionPatchAchievementData::Categories);
    }
    return std::nullopt;
}

bool Send(Player* player, Table table, std::vector<uint32> const& fields,
    std::vector<std::string> const& strings)
{
    if (!player || !player->GetSession())
        return false;
    auto packet = Encode(table, fields, strings);
    if (!packet)
    {
        LOG_ERROR("module.ascension.patch", "Rejected malformed achievement patch for opcode {}",
            static_cast<uint16>(table));
        return false;
    }
    player->SendDirectMessage(&*packet);
    return true;
}
}
