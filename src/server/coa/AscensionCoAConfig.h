/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#ifndef ASCENSION_COA_CONFIG_H
#define ASCENSION_COA_CONFIG_H

#include "Define.h"
#include <functional>
#include <string>
#include <utility>
#include <vector>

class WorldPacket;
class WorldSession;

using CoAConfigBools = std::vector<std::pair<std::string, uint8>>;
using CoAConfigBoolSource = std::function<void(CoAConfigBools&)>;

void AddAscensionCoAConfigBoolSource(CoAConfigBoolSource source);
WorldPacket BuildAscensionCoAConfig();
void SendAscensionCoAConfig(WorldSession* session);

#endif
