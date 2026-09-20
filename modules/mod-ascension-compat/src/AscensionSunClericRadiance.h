/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#ifndef ASCENSION_SUN_CLERIC_RADIANCE_H
#define ASCENSION_SUN_CLERIC_RADIANCE_H
class SpellInfo;
// Load-time SpellInfo contract patches for this group (issue #2446 Harmonious Bells). Called from
// ApplyAscensionClassMechanics() in AscensionClassMechanics.cpp -- see the wiring note reported
// alongside this file, since that shared loader is outside this group's files.
void ApplyAscensionSunClericRadianceContracts(SpellInfo* info);
// Registers this group's SpellScript/AuraScript classes (issue #2442 Champion's Arrival). Called
// from MP_loader.cpp -- see the wiring note reported alongside this file, since that shared loader
// is outside this group's files.
void AddSC_AscensionSunClericRadiance();
#endif
