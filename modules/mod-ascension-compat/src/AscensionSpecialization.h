#ifndef ASCENSION_SPECIALIZATION_H
#define ASCENSION_SPECIALIZATION_H

#include "Define.h"

#include <vector>

class Player;

// Read and set the CoA specialization and Character Advancement state of a character, and list what a CoA class
// can learn, for other modules (mod-playerbots) that must not depend on this module's internal data.

// Active CoA specialization of a custom-class character, 0 when none. Falls back to the
// saved setting, so a caller running before this module's login hook still sees the
// choice of a returning character.
uint32 GetAscensionActiveSpecialization(Player const* player);

// Activates a specialization the same way the client's choice does (".local spec").
bool SwitchAscensionSpecialization(Player* player, uint32 specializationId);

// Rank currently held in a paid Character Advancement entry (ability or talent essence), 0 if none.
uint32 GetAscensionTalentRank(Player const* player, uint32 entryId);

// Sets a paid Character Advancement entry to `rank` (0 removes it) with the same rules as
// ".local talent", without chat replies. Essence budgets are the caller's business: like the
// command, this does not count points.
bool SetAscensionTalentRank(Player* player, uint32 entryId, uint32 rank);

// Whether a class id is one of the CoA custom classes.
bool IsAscensionCustomClassId(uint8 classId);

// A spell a CoA class can learn.
struct AscensionClassAbility
{
    uint32 SpellId;
    uint32 FirstSpellId; // first rank of the spell, SpellId itself when it has no lower rank
    uint16 SpecId;       // specialization that grants it, 0 for the whole class
    uint8 RequiredLevel;
};

// Every spell a CoA class can learn: class grants, each rank of its Character Advancement entries and the higher
// ranks taught with level. Passives are included (SpellInfo::IsPassive tells them apart) and a spell can appear
// more than once. Empty for a class that is not a CoA class.
std::vector<AscensionClassAbility> GetAscensionClassAbilities(uint8 classId);

#endif
