/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */
#ifndef ASCENSION_MANASTORM_H
#define ASCENSION_MANASTORM_H

class WorldSession;
class WorldPacket;

// Called only by the existing early extension sink. Copies bounded requests with a login token.
bool QueueAscensionManastormPacket(WorldSession* session, WorldPacket const& packet);
void AddAscensionManastormScripts();

#endif
