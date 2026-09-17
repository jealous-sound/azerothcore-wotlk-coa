/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#ifndef ASCENSION_CLASS_TUNING_H
#define ASCENSION_CLASS_TUNING_H

#include "Player.h"
#include "SpellAuras.h"
#include <array>

namespace AscensionClassTuning
{
struct Entry
{
    uint32 SpellId;
    uint8 ClassId;
    uint32 SpecId;
};

// Copied Spell.dbc normal tuning, joined to ChrSpecs IDs. PvP 887100-887190
// has separate targeting rules and is deliberately not granted here.
inline constexpr std::array<Entry, 91> Entries = {{
    { 887000, 12, 0 },
    { 887001, 13, 0 },
    { 887002, 14, 0 },
    { 887003, 15, 0 },
    { 887004, 16, 0 },
    { 887005, 17, 0 },
    { 887006, 18, 0 },
    { 887007, 19, 0 },
    { 887008, 20, 0 },
    { 887009, 21, 0 },
    { 887010, 22, 0 },
    { 887011, 23, 0 },
    { 887012, 24, 0 },
    { 887013, 25, 0 },
    { 887014, 26, 0 },
    { 887015, 27, 0 },
    { 887016, 28, 0 },
    { 887017, 29, 0 },
    { 887018, 30, 0 },
    { 887019, 31, 0 },
    { 887020, 32, 0 },
    { 887021, 12, 2 }, // Brutality
    { 887022, 12, 1 }, // Headhunting
    { 887023, 12, 3 }, // Ancestry
    { 887024, 13, 5 }, // Voodoo
    { 887025, 13, 6 }, // Brewing
    { 887026, 13, 4 }, // Shadowhunting
    { 887027, 14, 8 }, // Slayer
    { 887028, 14, 7 }, // Infernal
    { 887029, 14, 9 }, // Tyrant
    { 887030, 15, 10 }, // Boltslinger
    { 887031, 15, 11 }, // Houndmaster
    { 887032, 15, 97 }, // Black Knight
    { 887033, 15, 12 }, // Inquisition
    { 887034, 16, 15 }, // Lightning
    { 887035, 16, 13 }, // Wind
    { 887036, 16, 14 }, // Maelstrom
    { 887037, 17, 18 }, // War
    { 887038, 17, 16 }, // Hellfire
    { 887039, 17, 17 }, // Defiance
    { 887040, 18, 21 }, // Vanguard
    { 887041, 18, 20 }, // Inspiration
    { 887042, 18, 19 }, // Gladiator
    { 887043, 19, 23 }, // Zealot
    { 887044, 19, 22 }, // Oathkeeper
    { 887045, 19, 24 }, // Crusader
    { 887046, 20, 26 }, // Sanguine
    { 887047, 20, 27 }, // Accursed
    { 887048, 20, 25 }, // Fleshweaver
    { 887049, 20, 99 }, // Eternal
    { 887050, 21, 28 }, // Archery
    { 887051, 21, 30 }, // Brigand
    { 887052, 21, 29 }, // Farstrider
    { 887053, 22, 32 }, // Infinite
    { 887054, 22, 31 }, // Time
    { 887055, 22, 33 }, // Artificer
    { 887056, 23, 34 }, // Death
    { 887057, 23, 36 }, // Rime
    { 887058, 23, 35 }, // Animation
    { 887059, 24, 38 }, // Incineration
    { 887060, 24, 37 }, // Flameweaving
    { 887061, 24, 39 }, // Draconic
    { 887062, 25, 42 }, // Godblade
    { 887063, 25, 41 }, // Corruption
    { 887064, 25, 96 }, // Dreadnought
    { 887065, 25, 40 }, // Heretic
    { 887066, 26, 100 }, // Moon Guard
    { 887067, 26, 44 }, // Sentinel
    { 887068, 26, 43 }, // Moon Priest
    { 887069, 26, 45 }, // Warden
    { 887070, 27, 46 }, // Piety
    { 887071, 27, 47 }, // Valkyrie
    { 887072, 27, 98 }, // Blessings
    { 887073, 27, 48 }, // Seraphim
    { 887074, 28, 49 }, // Demolition
    { 887075, 28, 51 }, // Invention
    { 887076, 28, 50 }, // Mechanics
    { 887077, 29, 54 }, // Rot
    { 887078, 29, 53 }, // Stalking
    { 887079, 29, 52 }, // Fortitude
    { 887080, 29, 101 }, // Vizier
    { 887081, 30, 56 }, // Harvest
    { 887082, 30, 55 }, // Soul
    { 887083, 30, 57 }, // Domination
    { 887084, 31, 59 }, // Wildwalker
    { 887085, 31, 60 }, // Mountain King
    { 887086, 31, 95 }, // Geomancy
    { 887087, 31, 58 }, // Grovekeeper
    { 887088, 32, 62 }, // Glyphic
    { 887089, 32, 61 }, // Engravement
    { 887090, 32, 63 }, // Riftblade
}};

inline void Synchronize(Player* player, uint32 specId, bool refreshAmounts)
{
    if (!player || !player->IsAlive() || !player->FindMap())
        return;

    for (Entry const& entry : Entries)
    {
        if (entry.ClassId != player->getClass() || (entry.SpecId && entry.SpecId != specId))
        {
            player->RemoveAurasDueToSpell(entry.SpellId);
            continue;
        }

        if (Aura* aura = player->GetAura(entry.SpellId))
        {
            if (refreshAmounts)
                aura->RecalculateAmountOfEffects();
        }
        else
            player->AddAura(entry.SpellId, player);
    }
}
}

#endif
