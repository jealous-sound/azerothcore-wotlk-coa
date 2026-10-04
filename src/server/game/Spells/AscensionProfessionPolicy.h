#ifndef ASCENSION_PROFESSION_POLICY_H
#define ASCENSION_PROFESSION_POLICY_H

#include "Define.h"

class Player;

namespace HxcProfessions
{
bool AllowGathering(Player const* player, uint32 skill);
}

#endif
