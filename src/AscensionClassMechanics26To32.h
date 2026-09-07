/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#ifndef ASCENSION_CLASS_MECHANICS_26_TO_32_H
#define ASCENSION_CLASS_MECHANICS_26_TO_32_H

#include <cstdint>

class Player;
class Spell;
class Unit;

// These exact event contracts supplement the shared resource table.  In
// particular, triggered child hits must not be discarded with normal casts.
void HandleAscensionClassMechanics26To32Hit(Spell* spell, Player* player,
    Unit* target, std::uint8_t missInfo, std::uint32_t damage, bool critical);

// Call once per target only after at least one cast was actually interrupted.
// A successful hit on an idle target is not a successful interrupt.
void HandleAscensionClassMechanics26To32SuccessfulInterrupt(Spell* spell,
    Player* player);

#endif
