/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionRealmLevelCap.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

namespace
{
constexpr uint16 SMSG_GAME_EVENT_INFO = 0x09BD;
constexpr std::size_t GAME_EVENT_INFO_SIZE = 52;
constexpr std::size_t GAME_EVENT_INFO_RESERVED_SIZE = 28;
constexpr uint8 GAME_EVENT_STATE_NORMAL = 0;

struct RealmLevelCapEvent
{
    uint32 maxLevel;
    uint16 eventId;
};

constexpr RealmLevelCapEvent RealmLevelCapEvents[] = {
    { 25, 486 },
    { 35, 487 },
    { 45, 488 },
    { 55, 489 },
    { 60, 352 },
    { 70, 173 },
};
}

std::optional<uint16> AscensionRealmLevelCapEvent(uint32 maxPlayerLevel)
{
    for (RealmLevelCapEvent const& capEvent : RealmLevelCapEvents)
        if (capEvent.maxLevel == maxPlayerLevel)
            return capEvent.eventId;

    return std::nullopt;
}

void SendAscensionRealmLevelCap(WorldSession* session)
{
    if (!session)
        return;

    std::optional<uint16> eventId = AscensionRealmLevelCapEvent(sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL));
    if (!eventId)
        return;

    WorldPacket packet(SMSG_GAME_EVENT_INFO, GAME_EVENT_INFO_SIZE);
    packet << *eventId;
    packet << GAME_EVENT_STATE_NORMAL;
    packet << uint64(0) << uint64(0);
    packet << uint8(1);
    packet << uint32(0);
    for (std::size_t i = 0; i < GAME_EVENT_INFO_RESERVED_SIZE; ++i)
        packet << uint8(0);

    session->SendPacket(&packet);
}
