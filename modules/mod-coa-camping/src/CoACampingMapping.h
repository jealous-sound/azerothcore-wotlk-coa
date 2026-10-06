#ifndef COA_CAMPING_MAPPING_H
#define COA_CAMPING_MAPPING_H

#include <array>
#include <cstdint>

namespace CoACamping
{
inline constexpr std::uint32_t FireSpell = 818;
inline constexpr std::uint32_t FireEntry = 29784;
inline constexpr std::uint32_t FireDisplay = 192;
inline constexpr std::uint32_t ControllerEntry = 9500200;
inline constexpr std::uint32_t ControllerDisplay = 345;
inline constexpr std::uint32_t CandleEntry = 9500201;
inline constexpr std::uint32_t CandleDisplay = 100;
inline constexpr std::uint32_t TentEntry = 9500202;
inline constexpr std::uint32_t TentDisplay = 7194;
inline constexpr std::uint32_t ChairEntry = 9500203;
inline constexpr std::uint32_t ChairDisplay = 39;
inline constexpr std::uint32_t AllianceBannerEntry = 9500204;
inline constexpr std::uint32_t HordeBannerEntry = 9500205;
inline constexpr std::uint32_t AllianceBannerDisplay = 5771;
inline constexpr std::uint32_t HordeBannerDisplay = 5773;
inline constexpr std::uint32_t ReagentBotEntry = 9500220;
inline constexpr std::uint32_t RepairBotEntry = 9500221;
inline constexpr std::uint32_t BotDisplay = 14379;
inline constexpr std::uint32_t RewardSpell = 1459;
inline constexpr std::uint8_t RewardEffectMask = 1;
inline constexpr std::int32_t RewardIntellect = 2;
inline constexpr std::uint32_t SpiritSpell = 14752;
inline constexpr std::uint32_t CritSpell = 34833;
inline constexpr std::int32_t RewardCrit = 2;
inline constexpr std::uint32_t Herbalism = 182;
inline constexpr std::uint32_t RequiredHerbalism = 20;
inline constexpr char CooldownSettings[] = "core.coa.camping";
inline constexpr char MapStateKey[] = "mod.coa.camping.map";
inline constexpr char PlayerStateKey[] = "mod.coa.camping.player";
inline constexpr std::array<std::uint32_t, 4> IntellectFamilies = {1459, 23028, 61024, 61316};
inline constexpr std::array<std::uint32_t, 2> SpiritFamilies = {14752, 27681};
inline constexpr std::array<std::uint32_t, 3> CritFamilies = {24907, 60433, CritSpell};

struct Material
{
    std::uint32_t Item;
    std::uint32_t Count;
};

inline constexpr std::array<Material, 2> CandleMaterials = {{{2447, 1}, {765, 1}}};

enum class Feature : std::uint32_t
{
    Candle = 1,
    Tent,
    Chair,
    Banner,
    ReagentBot,
    RepairBot
};

struct FeatureDefinition
{
    Feature Kind;
    std::uint32_t Entry;
    std::uint32_t Skill;
    std::uint32_t Rank;
    std::array<Material, 3> Materials;
    float Offset;
    float Angle;
    char const* Name;
    char const* Menu;
};

inline constexpr std::array<FeatureDefinition, 6> Features = {{
    {Feature::Candle, CandleEntry, Herbalism, RequiredHerbalism, {{{2447, 1}, {765, 1}, {0, 0}}}, 1.6f, 2.6f,
        "Incense Candle", "Incense Candle: Herbalism 20; 1 Peacebloom + 1 Silverleaf; +2 Intellect"},
    {Feature::Tent, TentEntry, 165, 20, {{{2318, 5}, {0, 0}, {0, 0}}}, 6.0f, 0.0f,
        "Camp Tent", "Camp Tent: Leatherworking 20; 5 Light Leather; rested XP after 30 seconds"},
    {Feature::Chair, ChairEntry, 393, 20, {{{2318, 3}, {4470, 2}, {0, 0}}}, 2.4f, 3.8f,
        "Camp Chair", "Camp Chair: Skinning 20; 3 Light Leather + 2 Simple Wood; +2% crit"},
    {Feature::Banner, AllianceBannerEntry, 197, 20, {{{2996, 1}, {2320, 1}, {0, 0}}}, 2.8f, 5.0f,
        "Faction Banner", "Faction Banner: Tailoring 20; 1 Bolt of Linen Cloth + 1 Coarse Thread; Spirit"},
    {Feature::ReagentBot, ReagentBotEntry, 202, 20, {{{4359, 1}, {4361, 1}, {0, 0}}}, 2.6f, 1.2f,
        "Reagent Bot", "Reagent Bot: Engineering 20; 1 Handful of Copper Bolts + 1 Copper Tube"},
    {Feature::RepairBot, RepairBotEntry, 202, 140, {{{4375, 2}, {4371, 1}, {4234, 1}}}, 2.6f, 1.2f,
        "Repair Bot", "Repair Bot: Engineering 140; 2 Whirring Bronze Gizmos + 1 Bronze Tube + 1 Heavy Leather"}
}};

inline constexpr std::array<std::uint32_t, 9> VendorItems = {
    17020, 17021, 17026, 17028, 17029, 17030, 17031, 17032, 17033
};

inline bool IsService(Feature kind)
{
    return kind == Feature::ReagentBot || kind == Feature::RepairBot;
}

inline Feature Family(Feature kind)
{
    return IsService(kind) ? Feature::ReagentBot : kind;
}

inline FeatureDefinition const* Definition(std::uint32_t action)
{
    return action >= 1 && action <= Features.size() ? &Features[action - 1] : nullptr;
}
}

#endif
