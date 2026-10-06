#ifndef ASCENSION_INCARNATION_H
#define ASCENSION_INCARNATION_H

#include "Define.h"

class Player;

/// Model of the Wardrobe incarnation the player wears for this shapeshift form or form spell,
/// or 0 when none is selected (the form keeps its own model).
uint32 GetAscensionIncarnationDisplay(Player const* player, uint32 form, uint32 spellId);

/// Re-applies the incarnation model when the player changes it while shapeshifted.
void RefreshAscensionIncarnationDisplay(Player* player);

/// NPC look a player of a custom race wears in game (custom_race_display, chosen by skin colour), or 0.
uint32 GetAscensionCustomRaceDisplay(Player const* player);

/// Whether a race / gender wears custom_race_display looks in game.
bool HasAscensionCustomRaceDisplay(uint8 race, uint8 gender);

/// Extra races whose client model exists only as a male (the creation screen hides their Female button).
bool IsAscensionMaleOnlyRace(uint8 race);

#endif
