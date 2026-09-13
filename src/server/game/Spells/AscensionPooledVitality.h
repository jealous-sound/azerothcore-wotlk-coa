/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#ifndef ASCENSION_POOLED_VITALITY_H
#define ASCENSION_POOLED_VITALITY_H

#include "Define.h"

namespace AscensionBloodmage
{
enum Spells : uint32
{
    PooledVitalityTalent = 681078,
    PooledVitality = 680687,
    VitalityForLater = 681026,
    VitalityHeal = 681025,
    MendSelfHeal = 681032,
    HeartbreakBuff = 807563,
    VisceralPower = 807687,
    CursedFormCheck = 524861,
    CursedForm = 802877
};

enum Empowerment : uint32
{
    None, Mend, CrimsonTide, Heartbreak, Fleshcraft, Bloodbolt, AnimatedBlood, Transfusion, Apotheosis
};

inline Empowerment GetEmpowerment(uint32 id)
{
    if (id == 802310 || (id >= 504079 && id <= 504086))
        return Mend;
    if ((id >= 504129 && id <= 504136) || id == 504282)
        return CrimsonTide;
    if (id == 520314 || (id >= 520836 && id <= 520838) || (id >= 572895 && id <= 572898))
        return Heartbreak;
    if (id == 801952)
        return Fleshcraft;
    if (id == 804685 || id == 578304 || id == 578305 || (id >= 806928 && id <= 806932))
        return Bloodbolt;
    if (id == 573299 || id == 573356 || id == 573357)
        return AnimatedBlood;
    if (id == 705734)
        return Transfusion;
    if (id == 804195)
        return Apotheosis;
    return None;
}
}
#endif
