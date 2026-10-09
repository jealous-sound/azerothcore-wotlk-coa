/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#ifndef ASCENSION_RUNEMASTER_TALENTS_H
#define ASCENSION_RUNEMASTER_TALENTS_H

#include "Define.h"

class Player;
class SpellInfo;
void ApplyAscensionRunemasterTalentContracts(SpellInfo* info);
void ApplyAscensionManuscriptionContracts(SpellInfo* info);
void ApplyAscensionRunemasterTravelContracts(SpellInfo* info);
void AscensionRunemasterTravelDropButtonCopies(Player* player, uint8 button, uint32 action, uint32 previous);
void AddSC_AscensionRunemasterTalents();
void AddSC_AscensionRunemasterManuscription();

#endif
