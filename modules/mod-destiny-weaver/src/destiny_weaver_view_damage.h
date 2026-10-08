/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license:
 * https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#ifndef DESTINY_WEAVER_VIEW_DAMAGE_H
#define DESTINY_WEAVER_VIEW_DAMAGE_H

#include "Define.h"
#include <algorithm>

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

    /// Unit::CalculateDamage rolls a creature's blow as `urand(uint32(min), uint32(max))`, so a blow from a
    /// 1.5 - 2.3 range is 1 or 2 and nothing between, and a view factor of 20 turns those into two flat values.
    /// This puts the blow back at `spot` (0 <= spot < 1) inside its whole-number bucket and maps the buckets onto
    /// the creature's real range, whose average is what the view factor was measured against. A number the roll
    /// cannot have produced (an aura already changed it) is kept as it is.
    inline double BlowInRange(uint32 rolled, float minDamage, float maxDamage, double spot)
    {
        if (minDamage < 0.0f || !(maxDamage > minDamage))
            return double(rolled);

        uint32 const low = uint32(minDamage);
        uint32 const high = uint32(maxDamage);
        if (rolled < low || rolled > high)
            return double(rolled);

        double const position = (double(rolled - low) + spot) / double(high - low + 1);
        return double(minDamage) + position * double(maxDamage - minDamage);
    }

    /// A scaled blow as whole damage: its fraction is added with that probability (`chance`, 0 <= chance < 1), so
    /// many blows add up to the scaled amount, and a blow that landed never does less than 1.
    inline uint32 WholeDamage(double damage, double chance)
    {
        uint32 const whole = uint32(std::max(0.0, damage));
        uint32 const roundedUp = chance < damage - double(whole) ? 1 : 0;
        return std::max<uint32>(1, whole + roundedUp);
    }
}

#endif
