/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information.
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU GPL.
 */

#ifndef AC_LOCAL_LEVEL_SCALING_H
#define AC_LOCAL_LEVEL_SCALING_H

#include <algorithm>
#include <atomic>
#include <cstdint>

namespace LocalLevelScaling
{
inline std::atomic<bool> CreatureEnabled{false};
inline std::atomic<bool> QuestEnabled{false};
inline std::atomic<std::uint8_t> CreatureOffset{3};

// De combien de niveaux, au maximum, une creature peut etre remontee au-dessus
// du sien. Sans ce plafond, un joueur de niveau 30 traversant une zone de
// depart hissait chaque creature a 27 : les bots de bas niveau qui les
// engageaient se faisaient tuer par des creatures qui n'etaient pas les leurs.
//
// Une creature n'a qu'un seul niveau, diffuse a tous les clients : elle ne peut
// pas valoir 27 pour un joueur et 2 pour un bot. Borner la remontee est donc le
// seul compromis possible entre « le contenu reste pertinent » et « le monde
// reste jouable pour ceux qui y sont deja ».
//
// 0 desactive le plafond et rend le comportement d'origine.
inline std::atomic<std::uint8_t> CreatureMaxLift{5};

inline std::uint8_t ScaleCreatureLevel(std::uint8_t originalLevel, std::uint8_t playerLevel,
    std::uint8_t offset = 3)
{
    std::uint8_t floor = playerLevel > offset ? playerLevel - offset : 1;

    // Le plafond s'applique AVANT le max : une creature deja plus haute que
    // originalLevel + lift garde son niveau, on ne la rabaisse jamais.
    std::uint8_t lift = CreatureMaxLift.load(std::memory_order_relaxed);
    if (lift)
    {
        std::uint32_t capped = static_cast<std::uint32_t>(originalLevel) + lift;
        if (floor > capped)
            floor = static_cast<std::uint8_t>(std::min<std::uint32_t>(capped, 255));
    }

    return std::max(originalLevel, floor);
}

inline std::uint8_t ScaleQuestLevel(std::int32_t originalLevel, std::uint8_t playerLevel)
{
    if (originalLevel <= 0)
        return playerLevel;
    std::uint8_t questLevel = static_cast<std::uint8_t>(std::min<std::int32_t>(originalLevel, UINT8_MAX));
    return std::max(questLevel, playerLevel);
}
}

#endif
