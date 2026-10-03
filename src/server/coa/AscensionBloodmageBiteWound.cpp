/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellInfo.h"
#include "SpellMgr.h"

#include <algorithm>
#include <array>
#include <limits>

namespace
{
enum BiteWoundSpells : uint32
{
    SPELL_BITE_WOUND_PASSIVE = 532612,
    SPELL_BITE_WOUND = 706654
};

constexpr std::array<uint32, 10> BLOODFANG_BITE_RANKS = {
    800156, 501695, 501696, 501697, 503613, 503614, 503615, 572549, 572550, 572551 };

bool IsBloodfangBite(uint32 id)
{
    return std::find(BLOODFANG_BITE_RANKS.begin(), BLOODFANG_BITE_RANKS.end(), id) != BLOODFANG_BITE_RANKS.end();
}

Player* Bloodmage(Unit* unit)
{
    Player* player = unit ? unit->ToPlayer() : nullptr;
    return player && player->getClass() == CLASS_SON_OF_ARUGAL && player->IsAlive() && player->IsInWorld()
        ? player : nullptr;
}

class bloodmage_bite_wound : public AllSpellScript
{
public:
    bloodmage_bite_wound() : AllSpellScript("bloodmage_bite_wound", {ALLSPELLHOOK_ON_HIT_RESULT}) { }

    void OnSpellHitResult(Spell* spell, Unit* target, uint8 miss, uint32, uint32, bool) override
    {
        Player* player = Bloodmage(spell->GetCaster());
        if (!player || !target || target == player || !target->IsAlive() || miss != SPELL_MISS_NONE ||
            !IsBloodfangBite(spell->GetSpellInfo()->Id) || !sSpellMgr->GetSpellInfo(SPELL_BITE_WOUND))
            return;
        player->AddAura(SPELL_BITE_WOUND, target);
    }
};

class bloodmage_bite_wound_leech : public UnitScript
{
public:
    bloodmage_bite_wound_leech() : UnitScript("bloodmage_bite_wound_leech", true,
        {UNITHOOK_MODIFY_MELEE_DAMAGE}) { }

    void ModifyMeleeDamage(Unit* target, Unit* attacker, uint32& damage) override
    {
        Player* player = Bloodmage(attacker);
        SpellInfo const* passive = sSpellMgr->GetSpellInfo(SPELL_BITE_WOUND_PASSIVE);
        if (!player || !target || !damage || !passive || !target->HasAura(SPELL_BITE_WOUND, player->GetGUID()))
            return;
        uint32 const percent = std::clamp(passive->Effects[EFFECT_0].CalcValue(player), 0, 100);
        uint32 const amount = uint32(uint64(damage) * percent / 100);
        if (amount)
            Unit::DealHeal(player, player, amount);
    }
};
}

void AddSC_AscensionBloodmageBiteWound()
{
    new bloodmage_bite_wound();
    new bloodmage_bite_wound_leech();
}
