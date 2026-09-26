/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionPatchSpells.h"
#include "Log.h"
#include "Player.h"

namespace AscensionPatchSpells
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
        case Table::Charges: return Layout{2, 0, 0, false};
        case Table::ChargesCategory: return Layout{3, 0, 0, false};
        case Table::Affect: return Layout{3, 0, 0, false};
        case Table::CustomAttr: return Layout{11, 0, 0, false};
        case Table::Addon: return Layout{23, 0, 0, false};
        case Table::TagTypes: return Layout{29, 6, 0, true};
        case Table::Tags: return Layout{3, 0, 0, false};
        case Table::SpellSuggestions: return Layout{4, 0, 0, false};
        case Table::StatSuggestions: return Layout{4, 0, 0, false};
        case Table::ActivationOverlays: return Layout{14, 2, 0, true};
        case Table::Spell: return Layout{170, 4, 0, false};
        case Table::Visual: return Layout{32, 0, 0, false};
        case Table::VisualKit: return Layout{38, 0, 0, false};
        case Table::VisualKitModelAttach: return Layout{10, 0, 0, false};
        case Table::VisualEffectName: return Layout{1, 2, 4, false};
        case Table::Missile: return Layout{15, 0, 0, false};
        case Table::MissileMotion: return Layout{1, 2, 2, false};
        case Table::ItemEnchantment: return Layout{14, 16, 8, false};
        case Table::ShapeshiftForm: return Layout{18, 1, 0, false};
        case Table::Icon: return Layout{1, 1, 0, false};
        case Table::Category: return Layout{2, 0, 0, false};
        case Table::Duration: return Layout{4, 0, 0, false};
        case Table::CastTimes: return Layout{4, 0, 0, false};
        case Table::Radius: return Layout{4, 0, 0, false};
        case Table::Range: return Layout{8, 2, 0, false};
        case Table::Difficulty: return Layout{5, 0, 0, false};
        case Table::DescriptionVariables: return Layout{2, 1, 0, false};
        case Table::EffectCameraShakes: return Layout{4, 0, 0, false};
        case Table::ChainEffects: return Layout{45, 2, 0, false};
        case Table::ItemEnchantmentCondition: return Layout{16, 0, 0, false};
        case Table::VisualKitAreaModel: return Layout{3, 1, 0, false};
        case Table::VisualPrecastTransitions: return Layout{3, 2, 0, false};
        case Table::RuneCost: return Layout{5, 0, 0, false};
        case Table::FocusObject: return Layout{2, 1, 0, false};
    }
    return std::nullopt;
}

bool IsPointer(Table table, std::size_t index)
{
    switch (table)
    {
        case Table::TagTypes:
            return index == 23 || index == 24 || index == 25 || index == 26 || index == 27 || index == 28;
        case Table::ActivationOverlays:
            return index == 2 || index == 9;
        case Table::Spell:
            return index == 136 || index == 137 || index == 138 || index == 139;
        case Table::Range:
            return index == 6 || index == 7;
        case Table::DescriptionVariables:
            return index == 1;
        case Table::ChainEffects:
            return index == 7 || index == 41;
        case Table::VisualKitAreaModel:
            return index == 1;
        case Table::VisualPrecastTransitions:
            return index == 1 || index == 2;
        case Table::FocusObject:
            return index == 1;
        default: return false;
    }
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

bool Send(Player* player, Table table, std::vector<uint32> const& fields,
    std::vector<std::string> const& strings)
{
    if (!player || !player->GetSession())
        return false;
    auto packet = Encode(table, fields, strings);
    if (!packet)
    {
        LOG_ERROR("module.ascension.patch", "Rejected malformed spell patch for opcode {}", static_cast<uint16>(table));
        return false;
    }
    player->SendDirectMessage(&*packet);
    return true;
}
}
