/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#ifndef ASCENSION_REALM_LEVEL_CAP_H
#define ASCENSION_REALM_LEVEL_CAP_H

#include "Define.h"
#include <optional>

class WorldSession;

std::optional<uint16> AscensionRealmLevelCapEvent(uint32 maxPlayerLevel);
void SendAscensionRealmLevelCap(WorldSession* session);

#endif
