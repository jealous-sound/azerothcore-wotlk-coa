/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#ifndef ASCENSION_CLASS_MECHANICS_12_TO_17_H
#define ASCENSION_CLASS_MECHANICS_12_TO_17_H

#include <cstdint>

class Player;
class Spell;
class Unit;
struct TargetInfo;

// Reviewed tooltip contracts for Barbarian through Knight of Xoroth.  These
// entry points intentionally stay separate from AscensionClassMechanics.cpp so
// the group audit can be integrated without overlapping the Guardian/Ranger
// implementation.
void HandleAscensionClassMechanics12To17CalculatedTarget(Spell* spell,
    Player* player, Unit* target, TargetInfo& targetInfo);
void HandleAscensionClassMechanics12To17Hit(Spell* spell, Player* player,
    Unit* target, std::uint8_t missInfo, std::uint32_t damage,
    bool critical);

#endif
