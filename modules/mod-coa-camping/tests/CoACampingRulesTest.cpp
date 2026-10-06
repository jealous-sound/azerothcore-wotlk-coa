#include "CoACampingRules.h"
#include "CoACampingMapping.h"
#include "gtest/gtest.h"

using namespace CoACamping;

TEST(CoACamping, RestRequiresUninterruptedElapsedTime)
{
    RestProgress rest;
    EXPECT_FALSE(rest.Observe(7, 1000, 60000, true));
    EXPECT_FALSE(rest.Observe(7, 60999, 60000, true));
    EXPECT_TRUE(rest.Observe(7, 61000, 60000, true));
    EXPECT_FALSE(rest.Observe(7, 121000, 60000, true));
    EXPECT_FALSE(rest.Observe(7, 122000, 60000, false));
    EXPECT_FALSE(rest.Observe(7, 123000, 60000, true));
    EXPECT_FALSE(rest.Observe(7, 182999, 60000, true));
    EXPECT_TRUE(rest.Observe(7, 183000, 60000, true));
}

TEST(CoACamping, SwitchingCampOrLosingCampResetsRest)
{
    RestProgress rest;
    EXPECT_FALSE(rest.Observe(1, 1000, 60000, true));
    EXPECT_FALSE(rest.Observe(2, 61000, 60000, true));
    EXPECT_FALSE(rest.Observe(0, 121000, 60000, true));
    EXPECT_FALSE(rest.Observe(2, 122000, 60000, true));
    EXPECT_TRUE(rest.Observe(2, 182000, 60000, true));
}

TEST(CoACamping, IrregularUpdatesCountElapsedTimeAndBackwardClockRestarts)
{
    RestProgress rest;
    EXPECT_FALSE(rest.Observe(1, 70000, 60000, true));
    EXPECT_FALSE(rest.Observe(1, 71000, 60000, true));
    EXPECT_FALSE(rest.Observe(1, 5000, 60000, true));
    EXPECT_TRUE(rest.Observe(1, 65001, 60000, true));
}

TEST(CoACamping, ContributionRevalidatesSharedState)
{
    ContributionCheck check;
    EXPECT_EQ(CheckContribution(check), ContributionFailure::None);
    check.FeaturePresent = true;
    EXPECT_EQ(CheckContribution(check), ContributionFailure::FeaturePresent);
    check.UsedSlots = 3;
    EXPECT_EQ(CheckContribution(check), ContributionFailure::Full);
    check.OnCooldown = true;
    EXPECT_EQ(CheckContribution(check), ContributionFailure::Cooldown);
    check.Expired = true;
    EXPECT_EQ(CheckContribution(check), ContributionFailure::Expired);
}

TEST(CoACamping, RejectedContributionsHaveDistinctFailures)
{
    ContributionCheck check;
    check.Accessible = false;
    EXPECT_EQ(CheckContribution(check), ContributionFailure::Inaccessible);
    check.Accessible = true;
    check.InRange = false;
    EXPECT_EQ(CheckContribution(check), ContributionFailure::TooFar);
    check.InRange = true;
    check.Available = false;
    EXPECT_EQ(CheckContribution(check), ContributionFailure::Busy);
    check.Available = true;
    check.HasSkill = false;
    EXPECT_EQ(CheckContribution(check), ContributionFailure::Skill);
    check.HasSkill = true;
    check.HasMaterials = false;
    EXPECT_EQ(CheckContribution(check), ContributionFailure::Materials);
    check.HasMaterials = true;
    check.AlreadyContributed = true;
    EXPECT_EQ(CheckContribution(check), ContributionFailure::AlreadyContributed);
}

TEST(CoACamping, ContributionCooldownEndsAtItsPersistedDeadline)
{
    EXPECT_TRUE(CooldownActive(4600, 4599));
    EXPECT_FALSE(CooldownActive(4600, 4600));
    EXPECT_FALSE(CooldownActive(4600, 4601));
    EXPECT_FALSE(CooldownActive(0, 1));
    EXPECT_TRUE(CooldownActive(0x100000001ULL, 0xFFFFFFFFULL));
}

TEST(CoACamping, SoloContributionsBypassSharedLimitsAndKeepPlacementChecks)
{
    ContributionCheck check;
    check.OnCooldown = CooldownActive(4600, 1000);
    check.AlreadyContributed = true;
    EXPECT_EQ(CheckContribution(check), ContributionFailure::Cooldown);
    check.AllowSoloContributions = true;
    EXPECT_EQ(CheckContribution(check), ContributionFailure::None);
    check.FeaturePresent = true;
    EXPECT_EQ(CheckContribution(check), ContributionFailure::FeaturePresent);
    check.UsedSlots = 5;
    check.Capacity = 5;
    EXPECT_EQ(CheckContribution(check), ContributionFailure::Full);
    check.HasMaterials = false;
    EXPECT_EQ(CheckContribution(check), ContributionFailure::Materials);
    check.HasSkill = false;
    EXPECT_EQ(CheckContribution(check), ContributionFailure::Skill);
    check.AllowSoloContributions = false;
    check.HasMaterials = true;
    check.HasSkill = true;
    check.UsedSlots = 0;
    check.FeaturePresent = false;
    check.OnCooldown = false;
    EXPECT_EQ(CheckContribution(check), ContributionFailure::AlreadyContributed);
}

TEST(CoACamping, BannerUsesCapturedLevelBands)
{
    EXPECT_EQ(BannerSpirit(1), 14);
    EXPECT_EQ(BannerSpirit(39), 14);
    EXPECT_EQ(BannerSpirit(40), 19);
    EXPECT_EQ(BannerSpirit(49), 19);
    EXPECT_EQ(BannerSpirit(50), 27);
    EXPECT_EQ(BannerSpirit(59), 27);
    EXPECT_EQ(BannerSpirit(60), 32);
    EXPECT_EQ(BannerSpirit(80), 32);
}

TEST(CoACamping, ServiceBotsShareOneFamilyWithoutUpgrades)
{
    EXPECT_EQ(Family(Feature::RepairBot), Family(Feature::ReagentBot));
    EXPECT_NE(Family(Feature::Tent), Family(Feature::Chair));
    EXPECT_EQ(Definition(0), nullptr);
    EXPECT_EQ(Definition(7), nullptr);
    ASSERT_NE(Definition(6), nullptr);
    EXPECT_EQ(Definition(6)->Rank, 140);
}
