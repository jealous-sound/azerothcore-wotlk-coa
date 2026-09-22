/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#ifndef ASCENSION_COA_CONFIG_H
#define ASCENSION_COA_CONFIG_H

class WorldPacket;
class WorldSession;

WorldPacket BuildAscensionCoAXpConfig();
void SendAscensionCoAXpConfig(WorldSession* session);

#endif
