/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#ifndef ASCENSION_PATCH_ACHIEVEMENTS_H
#define ASCENSION_PATCH_ACHIEVEMENTS_H

#include "WorldPacket.h"
#include <optional>
#include <string>
#include <vector>

class Player;

namespace AscensionPatchAchievements
{
enum class Table : uint16
{
    Achievement = 0x0936,
    Criteria = 0x0937,
    Category = 0x0938
};

std::optional<WorldPacket> Encode(Table table, std::vector<uint32> const& fields,
    std::vector<std::string> const& strings = {});
using TextOverrides = std::vector<std::optional<std::string>>;
std::optional<WorldPacket> BuildPatch(Table table, uint32 id, TextOverrides const& overrides);
bool Send(Player* player, Table table, std::vector<uint32> const& fields,
    std::vector<std::string> const& strings = {});
}

#endif
