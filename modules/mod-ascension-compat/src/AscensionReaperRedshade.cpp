/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "Player.h"
#include "Opcodes.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellInfo.h"
#include "SpellScript.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include <algorithm>
#include <array>

namespace
{
constexpr uint32 SPELL_THRESH_DUMMY = 525058;
constexpr uint32 SPELL_THRESH = 505170;
constexpr uint32 SPELL_BLOODSHATTER_DUMMY = 525299;
constexpr uint32 SPELL_BLOODSHATTER = 505326;

constexpr std::array<uint32, 9> ReapRanks = { 354319, 500357, 504056, 504057, 504058, 504557,
    505151, 573302, 573303 };

bool IsReap(uint32 spellId)
{
    return std::find(ReapRanks.begin(), ReapRanks.end(), spellId) != ReapRanks.end();
}

void SendTransformedBar(Player* player, uint32 replacement)
{
    if (!player->GetSession())
        return;

    WorldPacket data(SMSG_ACTION_BUTTONS, 1 + (MAX_ACTION_BUTTONS * 4));
    data << uint8(1);
    for (uint8 button = 0; button < MAX_ACTION_BUTTONS; ++button)
    {
        ActionButton const* action = player->GetActionButton(button);
        if (!action)
        {
            data << uint32(0);
            continue;
        }

        uint32 packed = action->packedData;
        if (action->GetType() == ACTION_BUTTON_SPELL && IsReap(action->GetAction()))
            packed = replacement | (uint32(ACTION_BUTTON_SPELL) << 24);

        data << uint32(packed);
    }

    player->GetSession()->SendPacket(&data);
}

class aura_ascension_reaper_redshade_spells : public AuraScript
{
    PrepareAuraScript(aura_ascension_reaper_redshade_spells);

    bool Validate(SpellInfo const*) override
    {
        return ValidateSpellInfo({ SPELL_THRESH, SPELL_BLOODSHATTER });
    }

    void Apply(AuraEffect const*, AuraEffectHandleModes)
    {
        Player* player = GetTarget()->ToPlayer();
        if (!player)
            return;

        for (uint32 spellId : { SPELL_THRESH, SPELL_BLOODSHATTER })
            if (!player->HasActiveSpell(spellId))
                player->addSpell(spellId, player->GetActiveSpecMask(), true, true, true);
    }

    void Remove(AuraEffect const*, AuraEffectHandleModes)
    {
        Player* player = GetTarget()->ToPlayer();
        if (!player)
            return;

        player->RemoveAurasDueToSpell(SPELL_THRESH_DUMMY);
        player->RemoveAurasDueToSpell(SPELL_BLOODSHATTER_DUMMY);
        for (uint32 spellId : { SPELL_THRESH, SPELL_BLOODSHATTER })
            player->removeSpell(spellId, SPEC_MASK_ALL, true);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(aura_ascension_reaper_redshade_spells::Apply,
            EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(aura_ascension_reaper_redshade_spells::Remove,
            EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

class aura_ascension_reaper_redshade_transform : public AuraScript
{
    PrepareAuraScript(aura_ascension_reaper_redshade_transform);

    uint32 Replacement() const
    {
        return GetId() == SPELL_BLOODSHATTER_DUMMY ? SPELL_BLOODSHATTER : SPELL_THRESH;
    }

    bool Validate(SpellInfo const*) override
    {
        return ValidateSpellInfo({ SPELL_THRESH, SPELL_BLOODSHATTER });
    }

    void Apply(AuraEffect const*, AuraEffectHandleModes)
    {
        if (Player* player = GetTarget()->ToPlayer())
            SendTransformedBar(player, player->HasAura(SPELL_BLOODSHATTER_DUMMY) ? SPELL_BLOODSHATTER : Replacement());
    }

    void Remove(AuraEffect const*, AuraEffectHandleModes)
    {
        Player* player = GetTarget()->ToPlayer();
        if (!player)
            return;

        uint32 const other = GetId() == SPELL_BLOODSHATTER_DUMMY ? SPELL_THRESH_DUMMY
            : SPELL_BLOODSHATTER_DUMMY;
        if (player->HasAura(other))
            SendTransformedBar(player, other == SPELL_BLOODSHATTER_DUMMY ? SPELL_BLOODSHATTER : SPELL_THRESH);
        else
            player->SendActionButtons(1);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(aura_ascension_reaper_redshade_transform::Apply,
            EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(aura_ascension_reaper_redshade_transform::Remove,
            EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_ascension_reaper_redshade_reap : public SpellScript
{
    PrepareSpellScript(spell_ascension_reaper_redshade_reap);

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();
        if (!caster || !caster->IsPlayer())
            return SPELL_CAST_OK;

        uint32 const replacement = caster->HasAura(SPELL_BLOODSHATTER_DUMMY) ? SPELL_BLOODSHATTER
            : caster->HasAura(SPELL_THRESH_DUMMY) ? SPELL_THRESH : 0;
        if (!replacement)
            return SPELL_CAST_OK;

        Unit* target = GetExplTargetUnit();
        caster->CastSpell(target ? target : caster, replacement, false);

        return SPELL_FAILED_DONT_REPORT;
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_ascension_reaper_redshade_reap::CheckCast);
    }
};
}

void AddSC_AscensionReaperRedshade()
{
    RegisterSpellScript(aura_ascension_reaper_redshade_spells);
    RegisterSpellScript(aura_ascension_reaper_redshade_transform);
    RegisterSpellScript(spell_ascension_reaper_redshade_reap);
}
