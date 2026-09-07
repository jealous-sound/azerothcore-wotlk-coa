/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionClassMechanics19To25.h"
#include "Log.h"
#include "Player.h"
#include "Spell.h"
#include "SpellInfo.h"

namespace
{
constexpr std::uint32_t SPELL_TEMPLAR_RECKONING = 805421;
constexpr std::uint32_t SPELL_TEMPLAR_RECKONING_ENERGY = 521241;
constexpr std::uint32_t SPELL_CHRONOMANCER_INFINITE_SHIELD = 520457;
constexpr std::uint32_t SPELL_CHRONOMANCER_INFINITE_SHIELD_HEAL = 520458;
constexpr std::uint32_t SPELL_CHRONOMANCER_PARADOX_CANNON = 806203;
constexpr std::uint32_t SPELL_CHRONOMANCER_ECHO_FRAGMENT = 804455;
constexpr std::uint32_t SPELL_PYROMANCER_UNFATHOMABLY_HOT = 807404;
constexpr std::uint32_t SPELL_PYROMANCER_CLEANSING_FLAMES_BONUS = 807405;
constexpr std::uint32_t SPELL_PYROMANCER_ADD_FIVE_HEAT = 807392;
constexpr std::uint32_t CHRONOMANCER_FAMILY = 28;
constexpr std::uint32_t PYROMANCER_FAMILY = 30;
constexpr std::uint32_t INFINITE_SHIELD_CHARGES = 10;
constexpr std::uint32_t PARADOX_CANNON_PERIOD_MS = 3000;
}

void ApplyAscensionClassMechanics19To25(SpellInfo* spellInfo)
{
    if (!spellInfo)
        return;

    if (spellInfo->Id == SPELL_CHRONOMANCER_INFINITE_SHIELD)
    {
        SpellEffectInfo const& effect = spellInfo->Effects[EFFECT_0];
        if (spellInfo->SpellFamilyName == CHRONOMANCER_FAMILY &&
            (spellInfo->ProcCharges == 6 ||
                spellInfo->ProcCharges == INFINITE_SHIELD_CHARGES) &&
            effect.Effect == SPELL_EFFECT_APPLY_AURA &&
            effect.ApplyAuraName == SPELL_AURA_PROC_TRIGGER_SPELL &&
            effect.TriggerSpell == SPELL_CHRONOMANCER_INFINITE_SHIELD_HEAL)
        {
            // The current live node explicitly says ten charges. This does
            // not invent the still-private damage/proc/one-target policy.
            spellInfo->ProcCharges = INFINITE_SHIELD_CHARGES;
        }
        else
        {
            LOG_ERROR("module.ascension_compat",
                "Skipped unexpected Infinite Shield record {}",
                spellInfo->Id);
        }
        return;
    }

    if (spellInfo->Id == SPELL_CHRONOMANCER_PARADOX_CANNON)
    {
        SpellEffectInfo& effect = spellInfo->Effects[EFFECT_0];
        if (spellInfo->SpellFamilyName == CHRONOMANCER_FAMILY &&
            effect.Effect == SPELL_EFFECT_APPLY_AURA &&
            effect.ApplyAuraName == SPELL_AURA_PERIODIC_TRIGGER_SPELL &&
            effect.TriggerSpell == SPELL_CHRONOMANCER_ECHO_FRAGMENT &&
            (effect.Amplitude == 2500 ||
                effect.Amplitude == PARADOX_CANNON_PERIOD_MS))
        {
            // The native trigger already grants one fragment. Only its
            // copied 2.5-second interval disagrees with the live 3-second text.
            effect.Amplitude = PARADOX_CANNON_PERIOD_MS;
        }
        else
        {
            LOG_ERROR("module.ascension_compat",
                "Skipped unexpected Paradox Cannon record {}",
                spellInfo->Id);
        }
    }
}

bool CanPrepareAscensionClassMechanics19To25(Spell* spell)
{
    if (!spell || !spell->GetCaster())
        return true;

    Player* player = spell->GetCaster()->ToPlayer();
    SpellInfo const* info = spell->GetSpellInfo();
    if (!player || player->getClass() != CLASS_PYROMANCER ||
        info->Id != SPELL_PYROMANCER_CLEANSING_FLAMES_BONUS ||
        info->SpellFamilyName != PYROMANCER_FAMILY)
        return true;

    SpellEffectInfo const& effect = info->Effects[EFFECT_0];
    if (effect.Effect != SPELL_EFFECT_TRIGGER_SPELL ||
        effect.TriggerSpell != SPELL_PYROMANCER_ADD_FIVE_HEAT)
        return true;

    // Cleansing Flames' native tick already triggers +10 Heat. Its second
    // authored helper is the +5 talent bonus, not an unconditional extra gain.
    // This check intentionally includes triggered spells: native triggered
    // casts ignore CasterAuraSpell, while CanPrepare still runs for them.
    return player->HasAura(SPELL_PYROMANCER_UNFATHOMABLY_HOT);
}

void HandleAscensionClassMechanics19To25Hit(Spell* spell, Player* player,
    Unit* target, std::uint8_t missInfo, std::uint32_t damage)
{
    if (!spell || !player || !target || spell->IsTriggered() ||
        player->getClass() != CLASS_MONK ||
        spell->GetSpellInfo()->Id != SPELL_TEMPLAR_RECKONING ||
        spell->GetSpellInfo()->SpellFamilyName != std::uint32_t(CLASS_MONK) + 6 ||
        target == player || missInfo != SPELL_MISS_NONE || !damage ||
        player->IsFriendlyTo(target))
        return;

    // One call per successfully damaged enemy, including a killing blow.
    // IsValidAttackTarget would incorrectly reject a target after it died;
    // IsHostileTo would exclude otherwise attackable neutral creatures.
    player->CastSpell(player, SPELL_TEMPLAR_RECKONING_ENERGY, true);
}
