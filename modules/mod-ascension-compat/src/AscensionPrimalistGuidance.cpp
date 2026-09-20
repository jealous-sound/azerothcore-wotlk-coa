/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellMgr.h"
#include "SpellScript.h"

#include <vector>

namespace
{
enum GuidanceSpells : uint32
{
    SeismicGrasp = 807432
};

void ReduceRankCooldowns(Player* player, uint32 firstRank, int32 delta)
{
    if (!player || delta >= 0)
        return;
    std::vector<uint32> cooldowns;
    // Shared categories store an entry per rank. Update the active rank as well.
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
        ReduceRankCooldowns(GetHitPlayer(), GetSpellInfo()->Effects[index].MiscValue, GetEffectValue());
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_ascension_thanes_guidance::Reduce,
            EFFECT_ALL, SPELL_EFFECT_ASCENSION_MODIFY_COOLDOWN);
    }
};

class spell_ascension_spiritbound_cooldowns : public SpellScript
{
    PrepareSpellScript(spell_ascension_spiritbound_cooldowns);

    bool Load() override
    {
        return GetCaster()->IsPlayer() && GetCaster()->getClass() == CLASS_WILDWALKER;
    }

    void Reduce(SpellEffIndex index)
    {
        PreventHitDefaultEffect(index);
        ReduceRankCooldowns(GetHitPlayer(), GetSpellInfo()->Effects[index].MiscValue, GetEffectValue());
        // Grasp shares the Seismic cooldown but was added after this three-slot helper.
        if (index == EFFECT_0)
            ReduceRankCooldowns(GetHitPlayer(), SeismicGrasp, GetEffectValue());
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_ascension_spiritbound_cooldowns::Reduce,
            EFFECT_ALL, SPELL_EFFECT_ASCENSION_MODIFY_COOLDOWN);
    }
};
}

void AddSC_AscensionPrimalistGuidance()
{
    RegisterSpellScript(spell_ascension_thanes_guidance);
    RegisterSpellScript(spell_ascension_spiritbound_cooldowns);
}
