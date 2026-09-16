/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionReaperScytheRush.h"
#include "SpellInfo.h"

namespace
{
constexpr uint32 REAPER_SPELL_FAMILY = 36;
constexpr uint32 SPELL_REAPER_SCYTHE_RUSH = 500359;

// Scythe Rush's public record is a bare charge with Speed 0, so Spell::EffectCharge falls back
// to SPEED_CHARGE, the same 42 yards/second warrior Charge uses. The generic correction in
// SpellInfoCorrections only fills that field for family 0, so this Reaper record is skipped.
// The movement belonged to server-side helper 805689 "Scythe Rush (Charge/Root/Trigger)", whose
// own Spell.dbc Speed is 200; take that value rather than a tuned estimate.
constexpr float REAPER_SCYTHE_RUSH_SPEED = 200.0f;
}

void ApplyAscensionReaperScytheRushContracts(SpellInfo* spellInfo)
{
    if (!spellInfo || spellInfo->Id != SPELL_REAPER_SCYTHE_RUSH ||
        spellInfo->SpellFamilyName != REAPER_SPELL_FAMILY ||
        spellInfo->SpellFamilyFlags != flag96(0, 512, 0))
        return;

    SpellEffectInfo const& charge = spellInfo->Effects[EFFECT_0];
    if (charge.Effect != SPELL_EFFECT_CHARGE || charge.TargetA.GetTarget() != TARGET_UNIT_TARGET_ENEMY ||
        spellInfo->Effects[EFFECT_1].Effect || spellInfo->Effects[EFFECT_2].Effect)
        return;

    // Keep the correction idempotent: a Spell.dbc that already carries a speed is left alone.
    if (!spellInfo->Speed)
        spellInfo->Speed = REAPER_SCYTHE_RUSH_SPEED;
}
