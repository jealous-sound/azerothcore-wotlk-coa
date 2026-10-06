/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#ifndef ASCENSION_FREEPICK_H
#define ASCENSION_FREEPICK_H

#include "AscensionCoATalentState.h"
#include <cstdint>
#include <vector>

class Player;
class SpellInfo;

namespace AscensionFreepick
{
struct UploadResult
{
    char const* Result = "CA_UPDATE_ENTRIES_OK";
    char const* Learn = "";
    std::uint32_t EntryId = 0;
    std::uint32_t Rank = 0;
};

bool RealmIsClassless();
bool IsFreepickHero(Player const* player);
std::vector<AscensionCoATalentState::KnownEntry> KnownEntries(Player const* player);
UploadResult ApplyUpload(Player* player, std::vector<AscensionCoATalentState::KnownEntry> const& upload);
void Synchronize(Player* player);
}

void ApplyAscensionPathPassiveContract(SpellInfo* spellInfo);

#endif
