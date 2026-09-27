/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#ifndef ASCENSION_PATCH_ITEMS_H
#define ASCENSION_PATCH_ITEMS_H

#include "WorldPacket.h"
#include <optional>
#include <string>
#include <vector>

class Player;

namespace AscensionPatchItems
{
enum class Table : uint16
{
    Addon = 0x0569,
    DisplayInfoCollections = 0x056A,
    Stat = 0x059F,
    Appearances = 0x0693,
    SetAppearances = 0x06EC,
    Item = 0x0932,
    ExtendedCost = 0x0944,
    Visuals = 0x0964,
    VisualEffects = 0x0965,
    Set = 0x0966,
    RandomSuffix = 0x0967,
    RandomProperties = 0x0968,
    PurchaseGroup = 0x0969,
    LimitCategory = 0x096A,
    DisplayInfo = 0x096B,
    CondExtCosts = 0x096C
};

std::optional<WorldPacket> Encode(Table table, std::vector<uint32> const& fields,
    std::vector<std::string> const& strings = {});
bool Send(Player* player, Table table, std::vector<uint32> const& fields,
    std::vector<std::string> const& strings = {});
}

#endif
