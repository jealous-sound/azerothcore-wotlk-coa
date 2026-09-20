/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellMgr.h"
#include "SpellScript.h"

#include <vector>

namespace
{
class spell_ascension_thanes_guidance : public SpellScript
{
    PrepareSpellScript(spell_ascension_thanes_guidance);

    bool Load() override
    {
        return GetCaster()->IsPlayer() && GetCaster()->getClass() == CLASS_WILDWALKER;
    }

    void Reduce(SpellEffIndex index)
    {
        PreventHitDefaultEffect(index);
        // The unused middle slot contains zero, which native effect 165 treats
        // as a full Earthen Avatar reset. The tooltip names only Hammer and Rush.
        if (index != EFFECT_0 && index != EFFECT_2)
            return;
        Player* player = GetHitPlayer();
        int32 delta = GetEffectValue();
        if (!player || delta >= 0)
            return;
        uint32 firstRank = GetSpellInfo()->Effects[index].MiscValue;
        std::vector<uint32> cooldowns;
        // Shared cooldown categories store one entry per rank. Adjust each
        // matching entry, including the active rank, without touching other abilities.
        for (auto const& entry : player->GetSpellCooldownMap())
            if (sSpellMgr->GetFirstSpellInChain(entry.first) == firstRank)
                cooldowns.push_back(entry.first);
        for (uint32 spell : cooldowns)
        {
            uint32 remaining = player->GetSpellCooldownDelay(spell);
            if (uint64(-int64(delta)) >= remaining)
                player->RemoveSpellCooldown(spell, true);
            else
                player->ModifySpellCooldown(spell, delta);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_ascension_thanes_guidance::Reduce,
            EFFECT_ALL, SPELL_EFFECT_ASCENSION_MODIFY_COOLDOWN);
    }
};
}

void AddSC_AscensionPrimalistGuidance()
{
    RegisterSpellScript(spell_ascension_thanes_guidance);
}
