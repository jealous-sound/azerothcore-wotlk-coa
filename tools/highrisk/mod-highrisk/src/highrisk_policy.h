#ifndef HIGHRISK_POLICY_H
#define HIGHRISK_POLICY_H

#include <cstdint>

namespace HighRisk
{
// Realm tuning, not recovered Ascension probabilities.
constexpr unsigned MinimumLevel = 15;
constexpr unsigned MaximumLevel = 60;
constexpr unsigned ChestEntry = 994300;

constexpr bool Eligible(unsigned level, unsigned mode, bool instance, bool bot)
{
    return level >= MinimumLevel && mode == 3 && !instance && !bot;
}

constexpr unsigned GearLimit(unsigned level)
{
    return level >= MaximumLevel ? 2 : 1;
}

constexpr std::uint32_t GoldLoss(std::uint32_t carried, unsigned percent)
{
    return static_cast<std::uint32_t>(static_cast<std::uint64_t>(carried) * percent / 100);
}
}

#endif
