/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */
#ifndef ASCENSION_MANASTORM_RULES_H
#define ASCENSION_MANASTORM_RULES_H

#include <algorithm>
#include <cstdint>

namespace Ascension::Manastorm
{
    inline constexpr std::uint32_t MinPlayerLevel = 10;
    // Difficulty-0 pilot; endgame/config parity is a separate release.
    inline constexpr std::uint32_t MaxPilotPlayerLevel = 59;
    inline constexpr std::uint32_t MaxDepth = 15;
    inline constexpr std::uint32_t RewardCopper = 1000; // Explicit local pilot reward, first clear only.
    inline constexpr std::uint32_t MaxQueuedRequests = 8;
    inline constexpr std::uint32_t ReconnectSeconds = 120;

    enum class Phase : std::uint8_t { Idle, Transferring, Preparing, Running, Committing, Completed, Leaving, Failed };

    inline bool CanStart(std::uint32_t depth, std::uint32_t completed)
    {
        return depth >= 1 && depth <= MaxDepth && (depth - 1) % 5 == 0 && depth - 1 <= completed;
    }

    inline bool CanAdvance(Phase phase, std::uint32_t depth)
    {
        return phase == Phase::Completed && depth < MaxDepth;
    }

    inline std::uint32_t LinkedHealth(std::uint32_t baseHealth, std::uint32_t stacks)
    {
        return baseHealth + baseHealth * std::min(stacks, 4u) / 4;
    }

    inline float LinkedDamage(std::uint32_t stacks)
    {
        return 1.0f + float(std::min(stacks, 4u)) * 0.10f;
    }
}

#endif
