#ifndef COA_CAMPING_RULES_H
#define COA_CAMPING_RULES_H

#include <cstdint>

namespace CoACamping
{
enum class ContributionFailure
{
    None,
    Expired,
    Inaccessible,
    TooFar,
    Busy,
    Skill,
    Materials,
    Cooldown,
    AlreadyContributed,
    Full,
    FeaturePresent
};

struct ContributionCheck
{
    bool Expired = false;
    bool Accessible = true;
    bool InRange = true;
    bool Available = true;
    bool HasSkill = true;
    bool HasMaterials = true;
    bool OnCooldown = false;
    bool AlreadyContributed = false;
    bool AllowSoloContributions = false;
    bool FeaturePresent = false;
    std::uint32_t UsedSlots = 0;
    std::uint32_t Capacity = 3;
};

inline ContributionFailure CheckContribution(ContributionCheck const& check)
{
    if (check.Expired)
        return ContributionFailure::Expired;
    if (!check.Accessible)
        return ContributionFailure::Inaccessible;
    if (!check.InRange)
        return ContributionFailure::TooFar;
    if (!check.Available)
        return ContributionFailure::Busy;
    if (!check.HasSkill)
        return ContributionFailure::Skill;
    if (!check.HasMaterials)
        return ContributionFailure::Materials;
    if (!check.AllowSoloContributions && check.OnCooldown)
        return ContributionFailure::Cooldown;
    if (!check.AllowSoloContributions && check.AlreadyContributed)
        return ContributionFailure::AlreadyContributed;
    if (check.UsedSlots >= check.Capacity)
        return ContributionFailure::Full;
    if (check.FeaturePresent)
        return ContributionFailure::FeaturePresent;
    return ContributionFailure::None;
}

struct RestProgress
{
    std::uint64_t CampId = 0;
    std::uint64_t StartedAt = 0;
    bool Complete = false;

    void Reset()
    {
        CampId = 0;
        StartedAt = 0;
        Complete = false;
    }

    bool Observe(std::uint64_t campId, std::uint64_t now, std::uint64_t requiredMs, bool eligible)
    {
        if (!eligible || !campId)
        {
            Reset();
            return false;
        }
        if (CampId != campId || now < StartedAt)
        {
            CampId = campId;
            StartedAt = now;
            Complete = false;
        }
        if (Complete || now - StartedAt < requiredMs)
            return false;
        Complete = true;
        return true;
    }
};

inline bool CooldownActive(std::uint64_t expiresAt, std::uint64_t now)
{
    return expiresAt > now;
}

inline std::int32_t BannerSpirit(std::uint32_t level)
{
    return level < 40 ? 14 : level < 50 ? 19 : level < 60 ? 27 : 32;
}
}

#endif
