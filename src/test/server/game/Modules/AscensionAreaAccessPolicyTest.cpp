/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionAreaAccessPolicy.h"
#include "gtest/gtest.h"
#include <chrono>
#include <map>
#include <string>

using namespace AreaAccess;

namespace
{
constexpr uint32 Nagrand = 3518;
constexpr uint32 Hellfire = 3483;
constexpr uint32 Dalaran = 4395;
constexpr uint32 Icecrown = 210;
constexpr uint32 Eversong = 3430;
constexpr uint32 Ghostlands = 3433;
constexpr uint32 Silvermoon = 3487;
constexpr uint32 Azuremyst = 3524;
constexpr uint32 Bloodmyst = 3525;
constexpr uint32 Exodar = 3557;
constexpr uint32 SunstriderIsle = 10141;
constexpr uint32 AmmenVale = 10142;
constexpr uint32 StillpineHold = 10117;
constexpr uint32 AmaniCatacombs = 10127;
constexpr uint32 BlastedLands = 4;
constexpr uint32 Karazhan = 532;
constexpr uint32 Ulduar = 603;

Policy Make(std::map<std::string, bool> values = {})
{
    return Policy([values](std::string const& key) -> std::optional<bool> {
        auto it = values.find(key);
        if (it == values.end())
            return std::nullopt;
        return it->second;
    });
}
}

TEST(AscensionAreaAccessPolicyTest, DefaultsLockEveryOutlandAndNorthrendLocation)
{
    Policy const policy = Make();
    EXPECT_FALSE(policy.IsAllowed(false, 530, Nagrand));
    EXPECT_FALSE(policy.IsAllowed(false, 530, Hellfire));
    EXPECT_FALSE(policy.IsAllowed(false, 571, Dalaran));
    EXPECT_FALSE(policy.IsAllowed(false, 571, Icecrown));
    EXPECT_FALSE(policy.IsAllowed(false, Karazhan, 0));
    EXPECT_FALSE(policy.IsAllowed(false, Ulduar, 0));
}

TEST(AscensionAreaAccessPolicyTest, BloodElfAndDraeneiAreasAreLockedByDefault)
{
    Policy const policy = Make();
    for (uint32 zone : {Eversong, Ghostlands, Silvermoon, SunstriderIsle, AmaniCatacombs, Azuremyst, Bloodmyst,
        Exodar, AmmenVale, StillpineHold})
        EXPECT_FALSE(policy.IsAllowed(false, 530, zone)) << zone;
}

TEST(AscensionAreaAccessPolicyTest, BloodElfRegionKeyOpensOnlyBloodElfZones)
{
    Policy const policy = Make({{"Coa.Access.BloodElf", true}});
    for (uint32 zone : {Eversong, Ghostlands, Silvermoon, SunstriderIsle, AmaniCatacombs})
        EXPECT_TRUE(policy.IsAllowed(false, 530, zone)) << zone;
    for (uint32 zone : {Azuremyst, Bloodmyst, Exodar, AmmenVale, StillpineHold, Hellfire})
        EXPECT_FALSE(policy.IsAllowed(false, 530, zone)) << zone;
}

TEST(AscensionAreaAccessPolicyTest, DraeneiRegionKeyOpensOnlyDraeneiZones)
{
    Policy const policy = Make({{"Coa.Access.Draenei", true}});
    for (uint32 zone : {Azuremyst, Bloodmyst, Exodar, AmmenVale, StillpineHold})
        EXPECT_TRUE(policy.IsAllowed(false, 530, zone)) << zone;
    for (uint32 zone : {Eversong, Ghostlands, Silvermoon, SunstriderIsle, AmaniCatacombs, Hellfire})
        EXPECT_FALSE(policy.IsAllowed(false, 530, zone)) << zone;
}

TEST(AscensionAreaAccessPolicyTest, LeafOpensOneRacialZoneUnderLockedRegion)
{
    Policy const policy = Make({{"Coa.Access.Draenei.TheExodar", true}});
    EXPECT_TRUE(policy.IsAllowed(false, 530, Exodar));
    EXPECT_FALSE(policy.IsAllowed(false, 530, Azuremyst));
}

TEST(AscensionAreaAccessPolicyTest, OldWorldAndUnknownZonesAreOpen)
{
    Policy const policy = Make();
    EXPECT_TRUE(policy.IsAllowed(false, 0, 12));
    EXPECT_TRUE(policy.IsAllowed(false, 1, 14));
    EXPECT_TRUE(policy.IsAllowed(false, 0, 0));
    EXPECT_TRUE(policy.IsAllowed(false, 1, 999999));
}

TEST(AscensionAreaAccessPolicyTest, GameMastersBypassEverything)
{
    Policy const policy = Make();
    EXPECT_TRUE(policy.IsAllowed(true, 530, Nagrand));
    EXPECT_TRUE(policy.IsAllowed(true, 571, Icecrown));
    EXPECT_TRUE(policy.IsAllowed(true, Ulduar, 0));
    EXPECT_TRUE(policy.IsTeleportAllowed({true, false, 530, Hellfire}));
}

TEST(AscensionAreaAccessPolicyTest, LeafOpensOneLocationUnderLockedRegion)
{
    Policy const policy = Make({{"Coa.Access.Outland.Nagrand", true}});
    EXPECT_TRUE(policy.IsAllowed(false, 530, Nagrand));
    EXPECT_FALSE(policy.IsAllowed(false, 530, Hellfire));
}

TEST(AscensionAreaAccessPolicyTest, LeafLocksOneLocationUnderOpenRegion)
{
    Policy const policy = Make({{"Coa.Access.Northrend", true}, {"Coa.Access.Northrend.Icecrown", false}});
    EXPECT_TRUE(policy.IsAllowed(false, 571, Dalaran));
    EXPECT_FALSE(policy.IsAllowed(false, 571, Icecrown));
}

TEST(AscensionAreaAccessPolicyTest, RegionKeyOpensAllItsLocationsAndDungeons)
{
    Policy const policy = Make({{"Coa.Access.Outland", true}});
    EXPECT_TRUE(policy.IsAllowed(false, 530, Nagrand));
    EXPECT_TRUE(policy.IsAllowed(false, Karazhan, 0));
    EXPECT_FALSE(policy.IsAllowed(false, 571, Dalaran));
    EXPECT_FALSE(policy.IsAllowed(false, Ulduar, 0));
}

TEST(AscensionAreaAccessPolicyTest, DungeonsHaveTheirOwnKeys)
{
    Policy const policy = Make({{"Coa.Access.Northrend.Dungeons", true}});
    EXPECT_TRUE(policy.IsAllowed(false, Ulduar, 0));
    EXPECT_FALSE(policy.IsAllowed(false, 571, Icecrown));
}

TEST(AscensionAreaAccessPolicyTest, UnresolvedZoneOnNorthrendMapFollowsTheRegionKey)
{
    EXPECT_FALSE(Make().IsAllowed(false, 571, 0));
    EXPECT_TRUE(Make({{"Coa.Access.Northrend", true}}).IsAllowed(false, 571, 0));
}

TEST(AscensionAreaAccessPolicyTest, DarkPortalDestinationFollowsHellfirePeninsula)
{
    EXPECT_FALSE(Make().IsTeleportAllowed({false, false, 530, Hellfire}));
    Policy const open = Make({{"Coa.Access.Outland.HellfirePeninsula", true}});
    EXPECT_TRUE(open.IsTeleportAllowed({false, false, 530, Hellfire}));
}

TEST(AscensionAreaAccessPolicyTest, TeleportToSilvermoonAndTheExodarIsRefusedByDefault)
{
    Policy const policy = Make();
    EXPECT_FALSE(policy.IsTeleportAllowed({false, false, 530, Silvermoon}));
    EXPECT_FALSE(policy.IsTeleportAllowed({false, false, 530, Exodar}));
}

TEST(AscensionAreaAccessPolicyTest, TeleportToSilvermoonIsAllowedOnceBloodElfOpens)
{
    Policy const policy = Make({{"Coa.Access.BloodElf", true}});
    EXPECT_TRUE(policy.IsTeleportAllowed({false, false, 530, Silvermoon}));
    EXPECT_FALSE(policy.IsTeleportAllowed({false, false, 530, Exodar}));
}

TEST(AscensionAreaAccessPolicyTest, TeleportIntoLockedZoneIsRefusedAndOutOfItIsAllowed)
{
    Policy const policy = Make();
    EXPECT_FALSE(policy.IsTeleportAllowed({false, false, 571, Dalaran}));
    EXPECT_TRUE(policy.IsTeleportAllowed({false, false, 0, 12}));
}

TEST(AscensionAreaAccessPolicyTest, ForcedMovesAreNeverRefused)
{
    Policy const policy = Make();
    EXPECT_TRUE(policy.IsTeleportAllowed({false, true, 530, Nagrand}));
    EXPECT_TRUE(policy.IsTeleportAllowed({false, true, 530, Hellfire}));
    EXPECT_TRUE(policy.IsTeleportAllowed({false, true, 571, Dalaran}));
}

TEST(AscensionAreaAccessPolicyTest, UnlistedZonesOnNorthrendMapFollowTheRegionKey)
{
    for (uint32 zone : {4742u, 3979u, 4258u, 4630u, 4201u})
    {
        EXPECT_FALSE(Make().IsAllowed(false, 571, zone)) << zone;
        EXPECT_TRUE(Make({{"Coa.Access.Northrend", true}}).IsAllowed(false, 571, zone)) << zone;
    }
}

TEST(AscensionAreaAccessPolicyTest, UnlistedZonesOnOutlandMapFollowTheRegionKey)
{
    for (uint32 zone : {3540u, 3569u, 3917u, 999999u, 0u})
    {
        EXPECT_FALSE(Make().IsAllowed(false, 530, zone)) << zone;
        EXPECT_TRUE(Make({{"Coa.Access.Outland", true}}).IsAllowed(false, 530, zone)) << zone;
    }
}

TEST(AscensionAreaAccessPolicyTest, SeaZonesOnMapFiveThirtyFollowOutland)
{
    for (uint32 zone : {3455u, 3479u})
    {
        EXPECT_FALSE(Make().IsAllowed(false, 530, zone)) << zone;
        EXPECT_TRUE(Make({{"Coa.Access.Outland", true}}).IsAllowed(false, 530, zone)) << zone;
    }
}

TEST(AscensionAreaAccessPolicyTest, CavernsOfTimeAndOtherReachableDungeonMapsAreLocked)
{
    Policy const policy = Make();
    for (uint32 map : {269u, 560u, 578u, 595u})
        EXPECT_FALSE(policy.IsAllowed(false, map, 0)) << map;
}

TEST(AscensionAreaAccessPolicyTest, DalaranSewersArenaIsNotLocked)
{
    EXPECT_TRUE(Make().IsAllowed(false, 617, 0));
}

TEST(AscensionAreaAccessPolicyTest, MalformedConfigTextLocksInsteadOfInheriting)
{
    EXPECT_EQ(FlagFromConfigText("no"), std::optional<bool>(false));
    EXPECT_EQ(FlagFromConfigText("2"), std::optional<bool>(false));
    EXPECT_EQ(FlagFromConfigText("1"), std::optional<bool>(true));
    EXPECT_EQ(FlagFromConfigText("0"), std::optional<bool>(false));
    EXPECT_FALSE(FlagFromConfigText("").has_value());
}

TEST(AscensionAreaAccessPolicyTest, MalformedLeafUnderOpenRegionStaysLocked)
{
    Policy const policy([](std::string const& key) -> std::optional<bool> {
        if (key == "Coa.Access.Northrend")
            return true;
        if (key == "Coa.Access.Northrend.Icecrown")
            return FlagFromConfigText("no");
        return std::nullopt;
    });
    EXPECT_FALSE(policy.IsAllowed(false, 571, Icecrown));
    EXPECT_TRUE(policy.IsAllowed(false, 571, Dalaran));
}

TEST(AscensionAreaAccessPolicyTest, ParseFlagAcceptsZeroOneTrueFalseCaseInsensitively)
{
    EXPECT_EQ(ParseFlag("1"), std::optional<bool>(true));
    EXPECT_EQ(ParseFlag("0"), std::optional<bool>(false));
    EXPECT_EQ(ParseFlag("true"), std::optional<bool>(true));
    EXPECT_EQ(ParseFlag("FALSE"), std::optional<bool>(false));
    EXPECT_EQ(ParseFlag(" 1 "), std::optional<bool>(true));
}

TEST(AscensionAreaAccessPolicyTest, ParseFlagRejectsEverythingElse)
{
    EXPECT_FALSE(ParseFlag("").has_value());
    EXPECT_FALSE(ParseFlag("yes").has_value());
    EXPECT_FALSE(ParseFlag("2").has_value());
    EXPECT_FALSE(ParseFlag("-1").has_value());
    EXPECT_FALSE(ParseFlag("10").has_value());
}

TEST(AscensionAreaAccessPolicyTest, TableKeysAreUniqueAndEveryZoneBelongsToOneLocation)
{
    std::map<std::string, int> keys;
    std::map<uint32, int> zones;
    for (Location const& location : Locations())
    {
        ++keys[std::string(location.region) + "." + std::string(location.name)];
        for (uint32 zone : location.zones)
            ++zones[zone];
    }
    for (auto const& [key, count] : keys)
        EXPECT_EQ(count, 1) << key;
    for (auto const& [zone, count] : zones)
        EXPECT_EQ(count, 1) << zone;
}

TEST(AscensionAreaAccessPolicyTest, ThrottleAllowsTheFirstMessageAndBlocksRepeatsInsideTheInterval)
{
    using namespace std::chrono;
    Throttle throttle(30s);
    auto const start = steady_clock::time_point{} + 1h;
    EXPECT_TRUE(throttle.Allow(7, start));
    EXPECT_FALSE(throttle.Allow(7, start + 1s));
    EXPECT_FALSE(throttle.Allow(7, start + 29s));
    EXPECT_TRUE(throttle.Allow(7, start + 30s));
    EXPECT_FALSE(throttle.Allow(7, start + 31s));
}

TEST(AscensionAreaAccessPolicyTest, ThrottleTracksEachPlayerSeparately)
{
    using namespace std::chrono;
    Throttle throttle(30s);
    auto const start = steady_clock::time_point{} + 1h;
    EXPECT_TRUE(throttle.Allow(1, start));
    EXPECT_TRUE(throttle.Allow(2, start + 1s));
    EXPECT_FALSE(throttle.Allow(1, start + 2s));
}

TEST(AscensionAreaAccessPolicyTest, ThrottleForgetLetsAReturningPlayerBeNotifiedAtOnce)
{
    using namespace std::chrono;
    Throttle throttle(30s);
    auto const start = steady_clock::time_point{} + 1h;
    EXPECT_TRUE(throttle.Allow(1, start));
    throttle.Forget(1);
    EXPECT_TRUE(throttle.Allow(1, start + 1s));
}

TEST(AscensionAreaAccessPolicyTest, AreaTriggerTeleportWithTheTransportFlagWhileOnFootIsNotForced)
{
    EXPECT_FALSE(IsForcedMove(true, false, false));
}

TEST(AscensionAreaAccessPolicyTest, TransportMapChangeIsForced)
{
    EXPECT_TRUE(IsForcedMove(true, true, false));
}

TEST(AscensionAreaAccessPolicyTest, HearthstoneUsedOnABoatIsNotForced)
{
    EXPECT_FALSE(IsForcedMove(false, true, false));
}

TEST(AscensionAreaAccessPolicyTest, LeavingAnInstanceOrBattlegroundIsForced)
{
    EXPECT_TRUE(IsForcedMove(false, false, true));
}

TEST(AscensionAreaAccessPolicyTest, DarkPortalAreaTriggerIsRefusedWhileHellfireIsLocked)
{
    Policy const policy = Make();
    bool const forced = IsForcedMove(true, false, false);
    EXPECT_FALSE(policy.IsTeleportAllowed({false, forced, 530, Hellfire}));
}

namespace
{
constexpr Endpoint Orgrimmar{1, 1637};
constexpr Endpoint ThunderBluff{1, 1638};
constexpr Endpoint WarsongHold{571, 3537};
constexpr Endpoint Menethil{0, 11};
constexpr Endpoint Valgarde{571, 495};
constexpr Endpoint Auberdine{1, 148};
constexpr Endpoint AzuremystDock{530, 3524};
constexpr Endpoint GromGol{0, 33};
constexpr Endpoint IcecrownGunship{571, 210};
constexpr Endpoint MoakiHarbor{571, 65};
}

TEST(AscensionAreaAccessPolicyTest, ZeppelinToWarsongHoldDoesNotSpawnUntilBoreanTundraOpens)
{
    EXPECT_FALSE(Make().IsRouteAllowed({Orgrimmar, WarsongHold}));
    EXPECT_TRUE(Make({{"Coa.Access.Northrend.BoreanTundra", true}}).IsRouteAllowed({Orgrimmar, WarsongHold}));
}

TEST(AscensionAreaAccessPolicyTest, BoatToValgardeFollowsHowlingFjord)
{
    EXPECT_FALSE(Make().IsRouteAllowed({Menethil, Valgarde}));
    EXPECT_TRUE(Make({{"Coa.Access.Northrend.HowlingFjord", true}}).IsRouteAllowed({Menethil, Valgarde}));
}

TEST(AscensionAreaAccessPolicyTest, BoatToAzuremystFollowsDraenei)
{
    EXPECT_FALSE(Make().IsRouteAllowed({Auberdine, AzuremystDock}));
    EXPECT_TRUE(Make({{"Coa.Access.Draenei", true}}).IsRouteAllowed({Auberdine, AzuremystDock}));
}

TEST(AscensionAreaAccessPolicyTest, OldWorldTransportsStillSpawn)
{
    Policy const policy = Make();
    EXPECT_TRUE(policy.IsRouteAllowed({GromGol, Orgrimmar}));
    EXPECT_TRUE(policy.IsRouteAllowed({Orgrimmar, ThunderBluff}));
}

TEST(AscensionAreaAccessPolicyTest, TransportsInsideNorthrendDoNotSpawn)
{
    EXPECT_FALSE(Make().IsRouteAllowed({WarsongHold, MoakiHarbor}));
    EXPECT_FALSE(Make().IsRouteAllowed({IcecrownGunship, IcecrownGunship}));
}

TEST(AscensionAreaAccessPolicyTest, TransportInsideNorthrendSpawnsWhenBothEndsAreOpen)
{
    Policy const policy = Make({{"Coa.Access.Northrend", true}});
    EXPECT_TRUE(policy.IsRouteAllowed({WarsongHold, MoakiHarbor}));
}

TEST(AscensionAreaAccessPolicyTest, OneLockedEndBlocksTheTransportWhateverTheOtherEndIs)
{
    Policy const policy = Make({{"Coa.Access.Northrend.Dragonblight", true}});
    EXPECT_FALSE(policy.IsRouteAllowed({WarsongHold, MoakiHarbor}));
    EXPECT_FALSE(policy.IsRouteAllowed({MoakiHarbor, WarsongHold}));
}

TEST(AscensionAreaAccessPolicyTest, UnresolvedZoneOnMapFiveThirtyFollowsOutlandForTransports)
{
    EXPECT_FALSE(Make().IsRouteAllowed({Auberdine, {530, 0}}));
    EXPECT_TRUE(Make({{"Coa.Access.Outland", true}}).IsRouteAllowed({Auberdine, {530, 0}}));
}

TEST(AscensionAreaAccessPolicyTest, UnresolvedZoneOnNorthrendMapFollowsTheRegionKeyForTransports)
{
    EXPECT_FALSE(Make().IsRouteAllowed({Orgrimmar, {571, 0}}));
    EXPECT_TRUE(Make({{"Coa.Access.Northrend", true}}).IsRouteAllowed({Orgrimmar, {571, 0}}));
}

TEST(AscensionAreaAccessPolicyTest, LockedOutlandZoneBlocksATransportEnd)
{
    EXPECT_FALSE(Make().IsRouteAllowed({Orgrimmar, {530, 3518}}));
}

TEST(AscensionAreaAccessPolicyTest, UnreleasedCustomZonesAreLockedByDefault)
{
    Policy const policy = Make();
    EXPECT_FALSE(policy.IsAllowed(false, 0, 10292));
    EXPECT_FALSE(policy.IsAllowed(false, 903, 10011));
    EXPECT_FALSE(policy.IsAllowed(false, 903, 0));
    EXPECT_FALSE(policy.IsAllowed(false, 904, 10012));
    EXPECT_FALSE(policy.IsAllowed(false, 972, 10162));
    EXPECT_FALSE(policy.IsAllowed(false, 907, 10163));
    EXPECT_FALSE(policy.IsAllowed(false, 1781, 11430));
    EXPECT_FALSE(policy.IsAllowed(false, 1807, 11440));
    EXPECT_FALSE(policy.IsAllowed(false, 940, 10328));
}

TEST(AscensionAreaAccessPolicyTest, ReleasedCustomAndOldWorldZonesStayOpen)
{
    Policy const policy = Make();
    EXPECT_TRUE(policy.IsAllowed(false, 0, 10302));
    EXPECT_TRUE(policy.IsAllowed(false, 909, 10081));
    EXPECT_TRUE(policy.IsAllowed(false, 960, 10069));
    EXPECT_TRUE(policy.IsAllowed(false, 806, 4999));
    EXPECT_TRUE(policy.IsAllowed(false, 936, 0));
    EXPECT_TRUE(policy.IsAllowed(false, 937, 0));
}

TEST(AscensionAreaAccessPolicyTest, UnreleasedRegionKeyOpensEveryUnreleasedZone)
{
    Policy const policy = Make({{"Coa.Access.Unreleased", true}});
    EXPECT_TRUE(policy.IsAllowed(false, 0, 10292));
    EXPECT_TRUE(policy.IsAllowed(false, 903, 10011));
    EXPECT_TRUE(policy.IsAllowed(false, 904, 10012));
    EXPECT_TRUE(policy.IsAllowed(false, 907, 10163));
    EXPECT_TRUE(policy.IsAllowed(false, 1781, 11430));
    EXPECT_TRUE(policy.IsAllowed(false, 1807, 11440));
    EXPECT_TRUE(policy.IsAllowed(false, 940, 10328));
    EXPECT_FALSE(policy.IsAllowed(false, 530, Hellfire));
}

TEST(AscensionAreaAccessPolicyTest, LeafOpensOneUnreleasedZone)
{
    Policy const policy = Make({{"Coa.Access.Unreleased.DrukThar", true}});
    EXPECT_TRUE(policy.IsAllowed(false, 0, 10292));
    EXPECT_FALSE(policy.IsAllowed(false, 903, 10011));
    EXPECT_FALSE(policy.IsAllowed(false, 907, 10163));
    EXPECT_FALSE(policy.IsAllowed(false, 1807, 0));
    EXPECT_FALSE(policy.IsAllowed(false, 940, 0));
}

TEST(AscensionAreaAccessPolicyTest, ClassicNaxxramasStaysOpenAndTheWrathCloneIsLocked)
{
    Policy const policy = Make();
    EXPECT_TRUE(policy.IsAllowed(false, 533, 3456));
    EXPECT_TRUE(policy.IsAllowed(false, 533, 0));
    EXPECT_FALSE(policy.IsAllowed(false, 535, 3458));
    EXPECT_FALSE(policy.IsAllowed(false, 535, 0));
}

TEST(AscensionAreaAccessPolicyTest, NorthrendDungeonsKeyOpensTheWrathNaxxramasClone)
{
    Policy const policy = Make({{"Coa.Access.Northrend.Dungeons", true}});
    EXPECT_TRUE(policy.IsAllowed(false, 535, 3458));
}

namespace
{
RouteFrame Frame(uint32 map, uint32 zone, bool stop = false)
{
    return {{map, zone}, stop};
}

std::vector<RouteFrame> const AzuremystBoatPath = {Frame(530, 3479), Frame(530, 3479, true), Frame(530, 3479),
    Frame(1, 148), Frame(1, 148, true), Frame(1, 148)};
constexpr uint32 AzuremystBoat = 181646;
constexpr uint32 Northspear = 181688;
constexpr uint32 UnlistedTransport = 1;
std::vector<RouteFrame> const NorthspearPath = {Frame(0, 11), Frame(0, 11, true), Frame(571, 3979),
    Frame(571, 495, true), Frame(571, 3979)};
}

TEST(AscensionAreaAccessPolicyTest, UnlistedTransportDocksAreItsStopFramesNotItsRouteEnds)
{
    std::vector<Endpoint> const docks = TransportDocks(UnlistedTransport, NorthspearPath);
    ASSERT_EQ(docks.size(), 2u);
    EXPECT_EQ(docks[0], (Endpoint{0, 11}));
    EXPECT_EQ(docks[1], (Endpoint{571, 495}));
}

TEST(AscensionAreaAccessPolicyTest, KnownTransportsUseTheirListedDestinationsOverTheirResolvedDocks)
{
    std::vector<Endpoint> const docks = TransportDocks(AzuremystBoat, AzuremystBoatPath);
    ASSERT_EQ(docks.size(), 2u);
    EXPECT_EQ(docks[0], (Endpoint{1, 148}));
    EXPECT_EQ(docks[1], (Endpoint{530, 3524}));
}

TEST(AscensionAreaAccessPolicyTest, TransportDocksFallBackToTheRouteEndsWithoutStopFrames)
{
    std::vector<Endpoint> const docks = TransportDocks(UnlistedTransport, {Frame(0, 11), Frame(0, 12), Frame(1, 14)});
    ASSERT_EQ(docks.size(), 2u);
    EXPECT_EQ(docks[0], (Endpoint{0, 11}));
    EXPECT_EQ(docks[1], (Endpoint{1, 14}));
}

TEST(AscensionAreaAccessPolicyTest, AzuremystBoatFollowsDraeneiNotTheVeiledSea)
{
    std::vector<Endpoint> const docks = TransportDocks(AzuremystBoat, AzuremystBoatPath);
    EXPECT_TRUE(Make({{"Coa.Access.Draenei", true}}).IsRouteAllowed(docks));
    EXPECT_FALSE(Make({{"Coa.Access.Outland", true}}).IsRouteAllowed(docks));
}

TEST(AscensionAreaAccessPolicyTest, NorthspearFollowsHowlingFjordNotTheFrozenSea)
{
    std::vector<Endpoint> const docks = TransportDocks(Northspear, NorthspearPath);
    EXPECT_FALSE(Make().IsRouteAllowed(docks));
    EXPECT_TRUE(Make({{"Coa.Access.Northrend.HowlingFjord", true}}).IsRouteAllowed(docks));
}

TEST(AscensionAreaAccessPolicyTest, FlightBetweenOpenStopsThroughALockedZoneIsRefused)
{
    std::vector<Endpoint> const telaarToShattrath = {{530, Nagrand}, {530, 3519}, {530, 3703}};
    Policy const stopsOpen = Make({{"Coa.Access.Outland.Nagrand", true}, {"Coa.Access.Outland.ShattrathCity", true}});
    EXPECT_FALSE(stopsOpen.IsRouteAllowed(telaarToShattrath));
    EXPECT_TRUE(stopsOpen.IsRouteAllowed({{530, Nagrand}, {530, 3703}}));
    Policy const allOpen = Make({{"Coa.Access.Outland.Nagrand", true}, {"Coa.Access.Outland.ShattrathCity", true},
        {"Coa.Access.Outland.TerokkarForest", true}});
    EXPECT_TRUE(allOpen.IsRouteAllowed(telaarToShattrath));
}

TEST(AscensionAreaAccessPolicyTest, EveryListedTransportDestinationIsInAKeyedOrOldWorldZone)
{
    Policy const allOpen = Make({{"Coa.Access.Outland", true}, {"Coa.Access.BloodElf", true},
        {"Coa.Access.Draenei", true}, {"Coa.Access.Northrend", true}, {"Coa.Access.Unreleased", true}});
    Policy const allLocked = Make();
    for (auto const& [entry, destinations] : TransportDestinations())
    {
        EXPECT_TRUE(allOpen.IsRouteAllowed(destinations)) << entry;
        EXPECT_FALSE(allLocked.IsRouteAllowed(destinations)) << entry;
    }
}
