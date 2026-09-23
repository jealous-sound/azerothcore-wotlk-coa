/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionCoAConfig.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include <iterator>
#include <string_view>
#include <utility>

namespace
{
struct ClientRate
{
    std::string_view key;
    ServerConfigs setting;
};

constexpr ClientRate XpRates[] = {
    { "RATE_XP_GLOBAL", RATE_XP_GLOBAL },
    { "RATE_XP_KILL", RATE_XP_KILL },
    { "RATE_XP_KILL_TBC", RATE_XP_KILL_TBC },
    { "RATE_XP_KILL_WOTLK", RATE_XP_KILL_WOTLK },
    { "RATE_XP_QUEST", RATE_XP_QUEST },
    { "RATE_XP_QUEST_TBC", RATE_XP_QUEST_TBC },
    { "RATE_XP_QUEST_WOTLK", RATE_XP_QUEST_WOTLK },
    { "RATE_XP_EXPLORE", RATE_XP_EXPLORE },
    { "RATE_XP_PROFESSION", RATE_XP_PROFESSION },
    { "RATE_XP_ELITE", RATE_XP_ELITE },
    { "RATE_XP_DUNGEON_ELITE", RATE_XP_DUNGEON_ELITE },
    { "RATE_XP_PROFESSION_GRAY_MODIFIER", RATE_XP_PROFESSION_GRAY },
    { "RATE_XP_PROFESSION_GREEN_MODIFIER", RATE_XP_PROFESSION_GREEN },
    { "RATE_XP_PROFESSION_YELLOW_MODIFIER", RATE_XP_PROFESSION_YELLOW },
    { "RATE_XP_PROFESSION_ORANGE_MODIFIER", RATE_XP_PROFESSION_ORANGE },
    { "RATE_XP_PROFESSION_MINING_MODIFIER", RATE_XP_PROFESSION_MINING },
    { "RATE_XP_PROFESSION_HERBALISM_MODIFIER", RATE_XP_PROFESSION_HERBALISM },
    { "RATE_XP_PROFESSION_DISENCHANTING_MODIFIER", RATE_XP_PROFESSION_DISENCHANTING },
    { "RATE_XP_PROFESSION_SKINNING_MODIFIER", RATE_XP_PROFESSION_SKINNING },
    { "RATE_XP_PROFESSION_FISHING_MODIFIER", RATE_XP_PROFESSION_FISHING },
    { "RATE_XP_PROFESSION_BLACKSMITHING_MODIFIER", RATE_XP_PROFESSION_BLACKSMITHING },
    { "RATE_XP_PROFESSION_JEWELCRAFTING_MODIFIER", RATE_XP_PROFESSION_JEWELCRAFTING },
    { "RATE_XP_PROFESSION_ALCHEMY_MODIFIER", RATE_XP_PROFESSION_ALCHEMY },
    { "RATE_XP_PROFESSION_ENCHANTING_MODIFIER", RATE_XP_PROFESSION_ENCHANTING },
    { "RATE_XP_PROFESSION_LEATHERWORKING_MODIFIER", RATE_XP_PROFESSION_LEATHERWORKING },
    { "RATE_XP_PROFESSION_FIRST_AID_MODIFIER", RATE_XP_PROFESSION_FIRST_AID },
    { "RATE_XP_PROFESSION_COOKING_MODIFIER", RATE_XP_PROFESSION_COOKING },
    { "RATE_XP_PROFESSION_ENGINEERING_MODIFIER", RATE_XP_PROFESSION_ENGINEERING },
    { "RATE_XP_PROFESSION_TAILORING_MODIFIER", RATE_XP_PROFESSION_TAILORING },
    { "RATE_XP_PROFESSION_LOCKPICKING_MODIFIER", RATE_XP_PROFESSION_LOCKPICKING },
    { "RATE_XP_PROFESSION_INSCRIPTION_MODIFIER", RATE_XP_PROFESSION_INSCRIPTION },
};

std::vector<CoAConfigBoolSource>& BoolSources()
{
    static std::vector<CoAConfigBoolSource> sources;
    return sources;
}

void AppendKey(WorldPacket& packet, std::string_view key)
{
    packet << uint32(key.size());
    packet.append(reinterpret_cast<uint8 const*>(key.data()), key.size());
}
}

void AddAscensionCoAConfigBoolSource(CoAConfigBoolSource source)
{
    BoolSources().push_back(std::move(source));
}

WorldPacket BuildAscensionCoAConfig()
{
    CoAConfigBools bools;
    for (CoAConfigBoolSource const& source : BoolSources())
        source(bools);

    WorldPacket packet(SMSG_COA_CONFIG);
    uint32 constexpr integerConfigCount = 0;
    uint32 constexpr floatConfigCount = 0;
    uint32 constexpr integerVectorConfigCount = 0;
    uint32 constexpr floatVectorConfigCount = 0;
    packet << integerConfigCount;
    packet << uint32(bools.size());
    for (auto const& [key, value] : bools)
    {
        AppendKey(packet, key);
        packet << value;
    }
    packet << floatConfigCount;
    packet << uint32(std::size(XpRates));
    for (ClientRate const& rate : XpRates)
    {
        AppendKey(packet, rate.key);
        packet << sWorld->getRate(rate.setting);
    }
    packet << integerVectorConfigCount << floatVectorConfigCount;
    return packet;
}

void SendAscensionCoAConfig(WorldSession* session)
{
    if (!session)
        return;

    WorldPacket packet = BuildAscensionCoAConfig();
    session->SendPacket(&packet);
}
