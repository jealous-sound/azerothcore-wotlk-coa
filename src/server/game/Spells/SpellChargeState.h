/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license.
 */

#ifndef AC_SPELL_CHARGE_STATE_H
#define AC_SPELL_CHARGE_STATE_H

#include <algorithm>
#include <cstdint>

// Sequential recharge, using absolute milliseconds so normal character saves
// preserve partial pools across logout/server restart. No borrowed Unit pointers.
struct SpellChargeState
{
    std::uint32_t Available = 0;
    std::uint64_t NextRecovery = 0;
    std::uint32_t RecoveryTime = 0;

    void Update(std::uint32_t maximum, std::uint64_t now)
    {
        Available = std::min(Available, maximum);
        if (Available == maximum)
        {
            NextRecovery = 0;
            return;
        }

        // A partial pool without a recovery deadline cannot make progress and
        // used to leave the ability permanently unusable. Such a state can
        // only come from incomplete/legacy persisted data; fail open to a full
        // pool instead of manufacturing an arbitrary historical deadline.
        if (!RecoveryTime || !NextRecovery)
        {
            Available = maximum;
            NextRecovery = 0;
            return;
        }

        if (NextRecovery > now)
            return;

        std::uint64_t recovered = std::min<std::uint64_t>(maximum - Available,
            1 + (now - NextRecovery) / RecoveryTime);
        Available += static_cast<std::uint32_t>(recovered);
        NextRecovery = Available == maximum ? 0 : NextRecovery + recovered * RecoveryTime;
    }

    bool Consume(std::uint32_t maximum, std::uint32_t recovery, std::uint64_t now)
    {
        Update(maximum, now);
        if (!Available || !recovery)
            return false;
        if (Available == maximum)
        {
            RecoveryTime = recovery;
            NextRecovery = now + recovery;
        }
        --Available;
        return true;
    }

    void Restore(std::uint32_t maximum, std::uint32_t count, std::uint64_t now)
    {
        Update(maximum, now);
        Available += std::min(count, maximum - Available);
        if (Available == maximum)
            NextRecovery = 0;
    }
};

#endif
