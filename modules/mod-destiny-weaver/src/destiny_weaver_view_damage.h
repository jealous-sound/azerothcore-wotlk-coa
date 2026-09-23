/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license:
 * https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#ifndef DESTINY_WEAVER_VIEW_DAMAGE_H
#define DESTINY_WEAVER_VIEW_DAMAGE_H

#include "Define.h"

namespace DestinyWeaver
{
    /// Creature::SelectLevel gives a creature the weapon range `base .. base * 1.5`.
    constexpr double CREATURE_MAX_WEAPON_DAMAGE_FACTOR = 1.5;

    /// The average of the melee range Creature::CalculateMinMaxDamage builds from one row of
    /// `creature_classlevelstats`, before the template, rank and aura modifiers that do not depend on level.
    inline double AverageCreatureMeleeHit(float baseDamage, uint32 attackPower, float variance, uint32 attackTimeMs)
    {
        double const averageWeaponDamage = double(baseDamage) * (1.0 + CREATURE_MAX_WEAPON_DAMAGE_FACTOR) / 2.0;
        double const attackPowerDamage = double(attackPower) / 14.0 * double(variance);
        return (averageWeaponDamage + attackPowerDamage) * double(attackTimeMs) / 1000.0;
    }

    /// What one of the creature's blows is worth in the viewer's version of the fight.
    inline double ViewDamageTakenFactor(double viewAverageHit, double ownAverageHit)
    {
        return ownAverageHit > 0.0 ? viewAverageHit / ownAverageHit : 1.0;
    }
}

#endif
