/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#ifndef ASCENSION_PATCH_SPELL_STORE_H
#define ASCENSION_PATCH_SPELL_STORE_H

#include "AscensionPatchSpells.h"
#include <array>

namespace AscensionPatchSpells
{
using TextOverrides = std::array<std::optional<std::string>, 4>;

std::optional<WorldPacket> BuildSpellPatch(uint32 spellId, TextOverrides const& overrides);
}

#endif
