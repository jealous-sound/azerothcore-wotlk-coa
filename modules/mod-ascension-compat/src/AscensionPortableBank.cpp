/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "Player.h"
#include "SpellScript.h"
#include "SpellScriptLoader.h"
#include "WorldSession.h"

namespace
{
// Ascension's bank items - Personal Bank (110000 and 134985), Celestial Personal Bank (509892) and
// Realm Bank (1180097) - cast "Summon Personal Bank" (100702), "Summon Celestial Personal Bank"
// (93416) and "Summon Realm Bank" (92078). Each of those is a single SPELL_EFFECT_DUMMY with
// TARGET_UNIT_TARGET_ANY and no trigger spell, implemented inside Ascension's own core.
// Spell::EffectDummy has no case for them, so the cast succeeded, the item's ten minute cooldown
// was applied, and no bank window was ever sent.
class spell_ascension_summon_bank : public SpellScript
{
    PrepareSpellScript(spell_ascension_summon_bank);

    bool Load() override
    {
        return GetCaster()->IsPlayer();
    }

    void OpenBank(SpellEffIndex /*effIndex*/)
    {
        // The spells target TARGET_UNIT_TARGET_ANY, so the explicit target is whatever the user
        // happened to have selected; the bank always belongs to whoever used the item.
        Player* player = GetCaster()->ToPlayer();

        // A banker GUID equal to the player's own is the supported window without a banker NPC:
        // SendShowBank records it as the current banker and WorldSession::CanUseBank accepts it.
        player->GetSession()->SendShowBank(player->GetGUID());
    }

    void Register() override
    {
        // Opening the bank only needs the caster, so hook the caster-side effect phase.
        // Spell::_handle_immediate_phase runs SPELL_EFFECT_HANDLE_HIT for every effect
        // unconditionally, while SPELL_EFFECT_HANDLE_HIT_TARGET is skipped whenever the
        // explicit unit target misses - immune, evading or dead by the time the cast lands.
        // The item cooldown is already committed by then (Spell::SendSpellCooldown runs during
        // Spell::_cast), so hooking the target phase would keep burning ten minutes for nothing.
        OnEffectHit += SpellEffectFn(spell_ascension_summon_bank::OpenBank, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};
}

void AddSC_AscensionPortableBank()
{
    RegisterSpellScript(spell_ascension_summon_bank);
}
