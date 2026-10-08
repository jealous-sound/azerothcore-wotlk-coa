/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#ifndef COA_ASCENSION_ITEM_LADDER_H
#define COA_ASCENSION_ITEM_LADDER_H

#include "ItemTemplate.h"
#include "ObjectGuid.h"
#include "WorldPacket.h"
#include <array>
#include <cstddef>
#include <vector>

namespace ItemLadder
{
constexpr uint16 ItemStatQueryOpcode = 0x06FF;
constexpr uint16 ItemStatResponseOpcode = 0x0700;
constexpr uint16 UpdateObjectAddonOpcode = 0x0578;
constexpr uint32 LevelAddonField = 0;
constexpr uint32 RecordFields = 39;
constexpr uint32 RecordSize = RecordFields * sizeof(uint32);
constexpr std::size_t ItemStatResponseSize = 168;
constexpr uint8 MaxLevel = 65;
constexpr std::size_t ResistanceCount = 6;

struct Row
{
    uint32 ItemId = 0;
    uint32 Level = 0;
    std::array<_ItemStat, MAX_ITEM_PROTO_STATS> Stats{};
    std::array<float, MAX_ITEM_PROTO_DAMAGES> DamageMin{};
    std::array<float, MAX_ITEM_PROTO_DAMAGES> DamageMax{};
    uint32 Armor = 0;
    uint32 ArmorReborn = 0;
    std::array<int32, ResistanceCount> Resistances{};
    uint32 Block = 0;
    uint32 RandomProperty = 0;
    uint32 RequiredLevel = 0;
    uint32 SellPrice = 0;
};

[[nodiscard]] Row ParseRecord(uint8 const* record);

class Ladder
{
public:
    Ladder() = default;
    explicit Ladder(std::vector<Row> rows, ItemTemplate const* authored = nullptr);

    [[nodiscard]] bool Empty() const { return _rows.empty(); }
    [[nodiscard]] Row const* At(uint32 level) const;

private:
    std::vector<Row> _rows;
};

[[nodiscard]] bool ScalableItem(ItemTemplate const& proto);
[[nodiscard]] uint8 DropLevel(uint8 looterLevel, int32 offset);
void ApplyRow(ItemTemplate& proto, Row const& row);
[[nodiscard]] WorldPacket BuildItemStatResponse(uint32 itemId, uint32 level, Row const& row,
    ItemTemplate const* proto);
[[nodiscard]] WorldPacket BuildLevelAddon(ObjectGuid guid, uint32 level);
}

#endif
