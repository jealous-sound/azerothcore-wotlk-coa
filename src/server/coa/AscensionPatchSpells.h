/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#ifndef ASCENSION_PATCH_SPELLS_H
#define ASCENSION_PATCH_SPELLS_H

#include "WorldPacket.h"
#include <optional>
#include <string>
#include <vector>

class Player;

namespace AscensionPatchSpells
{
enum class Table : uint16
{
    Charges = 0x0570,
    ChargesCategory = 0x0571,
    Affect = 0x0574,
    CustomAttr = 0x05F4,
    Addon = 0x0686,
    TagTypes = 0x06C0,
    Tags = 0x06C1,
    SpellSuggestions = 0x06CE,
    StatSuggestions = 0x06D2,
    ActivationOverlays = 0x06F6,
    Spell = 0x092A,
    Visual = 0x092C,
    VisualKit = 0x092D,
    VisualKitModelAttach = 0x092E,
    VisualEffectName = 0x092F,
    Missile = 0x0930,
    MissileMotion = 0x0931,
    ItemEnchantment = 0x0935,
    ShapeshiftForm = 0x0949,
    Icon = 0x094A,
    Category = 0x094F,
    Duration = 0x0950,
    CastTimes = 0x0951,
    Radius = 0x0952,
    Range = 0x0953,
    Difficulty = 0x0954,
    DescriptionVariables = 0x0955,
    EffectCameraShakes = 0x0956,
    ChainEffects = 0x0957,
    ItemEnchantmentCondition = 0x0958,
    VisualKitAreaModel = 0x0959,
    VisualPrecastTransitions = 0x095A,
    RuneCost = 0x095F,
    FocusObject = 0x0961
};

std::optional<WorldPacket> Encode(Table table, std::vector<uint32> const& fields,
    std::vector<std::string> const& strings = {});
bool Send(Player* player, Table table, std::vector<uint32> const& fields,
    std::vector<std::string> const& strings = {});
bool TooltipPatchesEnabled();
bool IsTooltipPatchEnabled(uint32 spellId);
void SendSpellPatches(Player* player);
}

#endif
