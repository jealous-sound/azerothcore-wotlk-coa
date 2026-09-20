-- Hemoglobe (524907, #1978): "Coalesce a powerful sanguine orb above you, healing up to 12 allies within
-- 20 yds for <ticks> * ($524906m1 + $524906ppl1 + $bh*0.1125 + $SPI*0.45 + $STA*0.35) over 15 sec."
-- Spell.dbc gives 524907 DurationIndex 8 (15000 ms) and three effects: 0 and 1 are aura 23
-- (SPELL_AURA_PERIODIC_TRIGGER_SPELL) at Amplitude 1500 triggering Hemopulse 524906 and Pooled Vitality +2
-- 524905, and 2 is aura 24 (SPELL_AURA_PERIODIC_ENERGIZE) at Amplitude 1500 with MiscValue 1 (POWER_RAGE)
-- and base 49 -> 50 internal, i.e. 5 Rage a tick. The Rage energize and the Pooled Vitality stacks are
-- native and correct; only the heal's scaling was missing.
-- 524906 Hemopulse: effect 0 = SPELL_EFFECT_HEAL, TargetA 22 / TargetB 30 (TARGET_SRC_CASTER /
-- TARGET_UNIT_SRC_AREA_ALLY), RadiusIndex 9 (20 yds), MaxAffectedTargets 12, base 539 -> 540 with
-- RealPointsPerLevel 1.05 from SpellLevel 35. Its EffectBonusMultiplier is 0.0 and no `spell_bonus_data`
-- row existed, so Unit::SpellHealingBonusDone used coeff 0 and the caster's bonus healing contributed
-- nothing at all to the tick; the tooltip promises 11.25% of it. (Unit::SpellHealingBonusTaken's default
-- CalculateDefaultCoefficient fallback only scales the *target's* SPELL_AURA_MOD_HEALING benefit, which is
-- normally zero, so it was not supplying the missing spell-power term either.)
-- 524906 has no rank chain, so a single exact-id row is enough for SpellMgr::GetSpellBonusData.
-- The Spirit and Stamina terms are not expressible in this table and are added to the effect's base value
-- by the module's Bloodmage vitality scaling hook, beside the existing Sanguine Mend Spirit term.
START TRANSACTION;
DELETE FROM `spell_bonus_data` WHERE `entry` = 524906;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(524906, 0.1125, 0, 0, 0, 'Bloodmage - Hemopulse (Hemoglobe tick): tooltip $bh*0.1125 per tick');
COMMIT;
