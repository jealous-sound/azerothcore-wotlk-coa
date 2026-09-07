/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU
 * AGPL v3 license:
 * https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#ifndef ASCENSION_FRESH_CHARACTER_CHECK_H
#define ASCENSION_FRESH_CHARACTER_CHECK_H

class ChatHandler;

// Console/RA only. Requires no authenticated game sessions and a guarded DB
// snapshot around invocation. Creates no persistent characters or accounts.
bool HandleAscensionFreshCharacterCheck(ChatHandler* handler);

#endif
