#ifndef HOMEBREW_NOURISHMENT_SLOT_LOGIC_H
#define HOMEBREW_NOURISHMENT_SLOT_LOGIC_H

#include <array>
#include <cstddef>
#include <cstdint>

constexpr std::size_t kNourishmentSlotCount = 3;

struct NourishmentSlotEntry
{
    uint32_t itemId = 0;
    uint64_t expiresAt = 0;
};

struct NourishmentSlotDecision
{
    std::size_t index = kNourishmentSlotCount;
    bool refreshExistingItem = false;
    bool replaceExisting = false;
};

inline NourishmentSlotDecision SelectNourishmentSlot(
    std::array<NourishmentSlotEntry, kNourishmentSlotCount> const& slots,
    uint32_t itemId, uint64_t now)
{
    if (!itemId)
        return {};

    for (std::size_t i = 0; i < slots.size(); ++i)
        if (slots[i].itemId == itemId && slots[i].expiresAt > now)
            return {};

    for (std::size_t i = 0; i < slots.size(); ++i)
        if (!slots[i].itemId || slots[i].expiresAt <= now)
            return { i, false, false };

    return {};
}

#endif
