/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information.
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU GPL.
 */

#ifndef AC_LOCAL_LEVEL_SCALING_H
#define AC_LOCAL_LEVEL_SCALING_H

#include <algorithm>
#include <atomic>
#include <cstdint>

namespace LocalLevelScaling
{
inline std::atomic<bool> CreatureEnabled{false};
inline std::atomic<bool> QuestEnabled{false};
inline std::atomic<std::uint8_t> CreatureOffset{3};

inline std::uint8_t ScaleCreatureLevel(std::uint8_t originalLevel, std::uint8_t playerLevel,
    std::uint8_t offset = 3)
{
    std::uint8_t floor = playerLevel > offset ? playerLevel - offset : 1;
    return std::max(originalLevel, floor);
}

inline std::uint8_t ScaleQuestLevel(std::int32_t originalLevel, std::uint8_t playerLevel)
{
    if (originalLevel <= 0)
        return playerLevel;
    std::uint8_t questLevel = static_cast<std::uint8_t>(std::min<std::int32_t>(originalLevel, UINT8_MAX));
    return std::max(questLevel, playerLevel);
}
}

#endif
