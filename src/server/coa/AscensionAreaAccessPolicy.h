#ifndef COA_ASCENSION_AREA_ACCESS_POLICY_H
#define COA_ASCENSION_AREA_ACCESS_POLICY_H

#include "Define.h"
#include <algorithm>
#include <chrono>
#include <cctype>
#include <functional>
#include <iterator>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace AreaAccess
{
using ConfigLookup = std::function<std::optional<bool>(std::string const& key)>;

struct Location
{
    std::string_view region;
    std::string_view name;
    std::vector<uint32> zones;
    std::vector<uint32> maps;
};

inline std::optional<bool> ParseFlag(std::string const& text)
{
    auto const begin = std::find_if_not(text.begin(), text.end(), [](unsigned char c) { return std::isspace(c); });
    auto const end = std::find_if_not(text.rbegin(), std::string::const_reverse_iterator(begin),
        [](unsigned char c) { return std::isspace(c); }).base();
    std::string value(begin, end);
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return std::tolower(c); });
    if (value == "1" || value == "true")
        return true;
    if (value == "0" || value == "false")
        return false;
    return std::nullopt;
}

inline std::optional<bool> FlagFromConfigText(std::string const& text)
{
    if (text.empty())
        return std::nullopt;
    return ParseFlag(text).value_or(false);
}

struct Endpoint
{
    uint32 mapId;
    uint32 zoneId;
};

constexpr bool operator==(Endpoint const& left, Endpoint const& right)
{
    return left.mapId == right.mapId && left.zoneId == right.zoneId;
}

struct RouteFrame
{
    Endpoint where;
    bool stop;
};

inline std::unordered_map<uint32, std::vector<Endpoint>> const& TransportDestinations()
{
    static std::unordered_map<uint32, std::vector<Endpoint>> const destinations = {
        {181646, {{1, 148}, {530, 3524}}},
        {181688, {{0, 11}, {571, 495}}},
        {181689, {{571, 495}, {0, 85}}},
        {186238, {{571, 3537}, {1, 14}}},
        {186371, {{571, 495}}},
        {187038, {{571, 495}}},
        {187568, {{571, 65}, {571, 3537}}},
        {188511, {{571, 65}, {571, 495}}},
        {190536, {{0, 1519}, {571, 3537}}},
        {192241, {{571, 210}}},
        {192242, {{571, 210}}},
    };
    return destinations;
}

inline std::vector<Endpoint> TransportDocks(uint32 entry, std::vector<RouteFrame> const& frames)
{
    if (auto listed = TransportDestinations().find(entry); listed != TransportDestinations().end())
        return listed->second;

    std::vector<Endpoint> docks;
    for (RouteFrame const& frame : frames)
        if (frame.stop)
            docks.push_back(frame.where);
    if (docks.empty() && !frames.empty())
        docks = {frames.front().where, frames.back().where};
    return docks;
}

struct Teleport
{
    bool isGm;
    bool forced;
    uint32 toMap;
    uint32 toZone;
};

constexpr bool IsForcedMove(bool transportFlag, bool onTransport, bool fromInstanceOrBattleground)
{
    return (transportFlag && onTransport) || fromInstanceOrBattleground;
}

inline std::vector<Location> const& Locations()
{
    static std::vector<Location> const locations = {
        {"Outland", "HellfirePeninsula", {3483}, {}},
        {"Outland", "Zangarmarsh", {3521}, {}},
        {"Outland", "TerokkarForest", {3519}, {}},
        {"Outland", "Nagrand", {3518}, {}},
        {"Outland", "BladesEdgeMountains", {3522}, {}},
        {"Outland", "Netherstorm", {3523}, {}},
        {"Outland", "ShadowmoonValley", {3520}, {}},
        {"Outland", "ShattrathCity", {3703}, {}},
        {"Outland", "IsleOfQuelDanas", {4080}, {}},
        {"Outland", "Dungeons", {}, {540, 542, 543, 544, 545, 546, 547, 548, 550, 552, 553, 554, 555, 556, 557, 558,
            564, 565, 568, 580, 585, 532, 534, 269, 560}},
        {"BloodElf", "EversongWoods", {3430}, {}},
        {"BloodElf", "Ghostlands", {3433}, {}},
        {"BloodElf", "SilvermoonCity", {3487}, {}},
        {"BloodElf", "SunstriderIsle", {10141}, {}},
        {"BloodElf", "AmaniCatacombs", {10127}, {}},
        {"Draenei", "AzuremystIsle", {3524}, {}},
        {"Draenei", "BloodmystIsle", {3525}, {}},
        {"Draenei", "TheExodar", {3557}, {}},
        {"Draenei", "AmmenVale", {10142}, {}},
        {"Draenei", "StillpineHold", {10117}, {}},
        {"Unreleased", "DrukThar", {10292}, {}},
        {"Unreleased", "StormwindSewers", {}, {903}},
        {"Unreleased", "OrgrimmarDepths", {}, {904, 972}},
        {"Unreleased", "FadingIsland", {}, {907}},
        {"Unreleased", "ForgottenMine", {}, {1781}},
        {"Unreleased", "KarazhanCrypts", {}, {1807}},
        {"Unreleased", "ShadowboneDepths", {}, {940}},
        {"Northrend", "BoreanTundra", {3537}, {}},
        {"Northrend", "HowlingFjord", {495}, {}},
        {"Northrend", "Dragonblight", {65}, {}},
        {"Northrend", "GrizzlyHills", {394}, {}},
        {"Northrend", "ZulDrak", {66}, {}},
        {"Northrend", "SholazarBasin", {3711}, {}},
        {"Northrend", "StormPeaks", {67}, {}},
        {"Northrend", "Icecrown", {210}, {}},
        {"Northrend", "CrystalsongForest", {2817}, {}},
        {"Northrend", "Dalaran", {4395}, {}},
        {"Northrend", "Wintergrasp", {4197}, {}},
        {"Northrend", "Dungeons", {}, {535, 574, 575, 576, 578, 595, 599, 600, 601, 602, 603, 604, 608, 615, 616,
            619, 624, 631, 632, 649, 650, 658, 668, 724}},
    };
    return locations;
}

class Throttle
{
public:
    explicit Throttle(std::chrono::milliseconds interval) : _interval(interval) { }

    bool Allow(uint64 key, std::chrono::steady_clock::time_point now)
    {
        std::lock_guard<std::mutex> guard(_lock);
        auto [it, inserted] = _last.try_emplace(key, now);
        if (inserted)
            return true;
        if (now - it->second < _interval)
            return false;
        it->second = now;
        return true;
    }

    void Forget(uint64 key)
    {
        std::lock_guard<std::mutex> guard(_lock);
        _last.erase(key);
    }

private:
    std::chrono::milliseconds _interval;
    std::mutex _lock;
    std::unordered_map<uint64, std::chrono::steady_clock::time_point> _last;
};

class Policy
{
public:
    explicit Policy(ConfigLookup lookup)
    {
        std::string const prefix = "Coa.Access.";
        for (Location const& location : Locations())
        {
            std::string const region(location.region);
            std::optional<bool> value = lookup(prefix + region + "." + std::string(location.name));
            if (!value)
                value = lookup(prefix + region);
            bool const allowed = value.value_or(false);
            for (uint32 zone : location.zones)
                _zones[zone] = allowed;
            for (uint32 map : location.maps)
                _maps[map] = allowed;
        }
        _northrend = lookup(prefix + "Northrend").value_or(false);
        _outland = lookup(prefix + "Outland").value_or(false);
    }

    bool IsAllowed(bool isGm, uint32 mapId, uint32 zoneId) const
    {
        if (isGm)
            return true;
        if (auto it = _maps.find(mapId); it != _maps.end())
            return it->second;
        if (auto it = _zones.find(zoneId); it != _zones.end())
            return it->second;
        if (mapId == 571)
            return _northrend;
        if (mapId == 530)
            return _outland;
        return true;
    }

    bool IsTeleportAllowed(Teleport const& teleport) const
    {
        if (teleport.isGm || teleport.forced)
            return true;
        return IsAllowed(false, teleport.toMap, teleport.toZone);
    }

    bool IsRouteAllowed(std::vector<Endpoint> const& points) const
    {
        return std::all_of(points.begin(), points.end(),
            [this](Endpoint const& point) { return IsAllowed(false, point.mapId, point.zoneId); });
    }

private:
    std::unordered_map<uint32, bool> _zones;
    std::unordered_map<uint32, bool> _maps;
    bool _northrend = false;
    bool _outland = false;
};
}

#endif
