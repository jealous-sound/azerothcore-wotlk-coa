/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionPatchItems.h"
#include "Log.h"
#include "Player.h"

namespace AscensionPatchItems
{
namespace
{
struct Layout
{
    std::size_t Words;
    std::size_t Strings;
};

std::optional<Layout> GetLayout(Table table)
{
    switch (table)
    {
        case Table::Addon: return Layout{15, 2};
        case Table::DisplayInfoCollections: return Layout{8, 2};
        case Table::Stat: return std::nullopt;
        case Table::Appearances: return Layout{3, 0};
        case Table::SetAppearances: return Layout{3, 0};
        case Table::Item: return Layout{8, 0};
        case Table::ExtendedCost: return Layout{16, 0};
        case Table::Visuals: return Layout{6, 0};
        case Table::VisualEffects: return Layout{2, 1};
        case Table::Set: return Layout{37, 1};
        case Table::RandomSuffix: return Layout{13, 2};
        case Table::RandomProperties: return Layout{8, 2};
        case Table::PurchaseGroup: return Layout{10, 1};
        case Table::LimitCategory: return Layout{4, 1};
        case Table::DisplayInfo: return Layout{25, 14};
        case Table::CondExtCosts: return Layout{4, 0};
    }
    return std::nullopt;
}

bool IsPointer(Table table, std::size_t index)
{
    switch (table)
    {
        case Table::Addon:
            return index == 1 || index == 2;
        case Table::DisplayInfoCollections:
            return index == 4 || index == 5;
        case Table::VisualEffects:
            return index == 1;
        case Table::Set:
            return index == 1;
        case Table::RandomSuffix:
            return index == 1 || index == 2;
        case Table::RandomProperties:
            return index == 1 || index == 7;
        case Table::PurchaseGroup:
            return index == 9;
        case Table::LimitCategory:
            return index == 1;
        case Table::DisplayInfo:
            return (index >= 1 && index <= 6) || (index >= 15 && index <= 22);
        default: return false;
    }
}
}

std::optional<WorldPacket> Encode(Table table, std::vector<uint32> const& fields,
    std::vector<std::string> const& strings)
{
    auto layout = GetLayout(table);
    if (!layout || fields.size() != layout->Words || strings.size() != layout->Strings)
        return std::nullopt;
    std::size_t size = fields.size() * sizeof(uint32);
    for (std::string const& text : strings)
    {
        if (text.size() > 4096 || text.find('\0') != std::string::npos)
            return std::nullopt;
        size += sizeof(uint32) + text.size();
    }
    if (size > 65533)
        return std::nullopt;
    WorldPacket packet(static_cast<uint16>(table), size);
    for (std::size_t index = 0; index < fields.size(); ++index)
        packet << uint32(IsPointer(table, index) ? 0 : fields[index]);
    for (std::string const& text : strings)
    {
        packet << uint32(text.size());
        if (!text.empty())
            packet.append(text.data(), text.size());
    }
    return packet;
}

bool Send(Player* player, Table table, std::vector<uint32> const& fields,
    std::vector<std::string> const& strings)
{
    if (!player || !player->GetSession())
        return false;
    auto packet = Encode(table, fields, strings);
    if (!packet)
    {
        LOG_ERROR("module.ascension.patch", "Rejected unsupported or malformed item patch: {}",
            static_cast<uint16>(table));
        return false;
    }
    player->SendDirectMessage(&*packet);
    return true;
}
}
