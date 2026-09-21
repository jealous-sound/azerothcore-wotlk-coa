#ifndef ASCENSION_SPECIALIZATION_H
#define ASCENSION_SPECIALIZATION_H

#include "Define.h"

#include <vector>

class Player;

uint32 GetAscensionActiveSpecialization(Player const* player);

bool SwitchAscensionSpecialization(Player* player, uint32 specializationId);

uint32 GetAscensionTalentRank(Player const* player, uint32 entryId);

bool SetAscensionTalentRank(Player* player, uint32 entryId, uint32 rank);

bool IsAscensionCustomClassId(uint8 classId);

struct AscensionClassAbility
{
    uint32 SpellId;
    uint32 FirstSpellId;
    uint16 SpecId;
    uint8 RequiredLevel;
};

std::vector<AscensionClassAbility> GetAscensionClassAbilities(uint8 classId);

#endif
