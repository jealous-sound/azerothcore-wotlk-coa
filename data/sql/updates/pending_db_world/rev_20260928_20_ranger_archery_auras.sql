-- Hawkeye (800358) is a passive whose only effect is SPELL_EFFECT_APPLY_AREA_AURA_ENEMY carrying
-- SPELL_AURA_PERIODIC_TRIGGER_SPELL (801192, 75% ranged weapon damage, 3 s tick, 30 yd radius).
-- UnitAura::FillTargetMap (src/server/game/Spells/Auras/SpellAuras.cpp, SPELL_EFFECT_APPLY_AREA_AURA_ENEMY)
-- puts that aura on every hostile unit inside the radius, so the periodic trigger fired at anything the
-- Ranger happened to be standing next to, whether or not the target was ever marked by Hunting Shot.
-- The tooltip is "enemies within 30 yds affected by your Hunting Shot", and #4829 / #4928 both report the
-- unfiltered behaviour in play.
--
-- aura_ascension_ranger_hawkeye answers the area-target question instead of filtering the tick:
-- Aura::CanBeAppliedOn calls Aura::CheckAreaTarget, which runs the AuraScript DoCheckAreaTarget handlers,
-- for every unit in the area on each 500 ms UpdateTargetMap pass (SpellAuras.cpp, UPDATE_TARGET_MAP_INTERVAL).
-- An enemy without the mark is therefore never given the aura, so it never ticks.
-- The mark itself is the aura the Hunting Shot ranks apply to the struck target; the chain root is 801191
-- and the ranks are 547202-547208, which is the same list AscensionClassMechanics.cpp IsRangerHuntingShot uses.
--
-- Woodland Stalker (705034) is a passive whose only effect is SPELL_AURA_ADD_FLAT_MODIFIER (aura 107,
-- BasePoints 19 + DieSides 1 = 20) on SPELLMOD_CRITICAL_CHANCE. Spell mods cannot be conditional, so the
-- 20% was permanently on; the tooltip is "While in Elude". aura_ascension_ranger_woodland_stalker
-- recalculates the effect amount to 20 only while the Ranger holds Elude (801345) and 0 otherwise, and
-- refreshes on the same 500 ms period aura_ascension_ranger_wingman already uses for the War Falcon
-- presence auras, so entering and leaving Elude takes effect immediately.
DELETE FROM `spell_script_names` WHERE `ScriptName` IN
    ('aura_ascension_ranger_hawkeye', 'aura_ascension_ranger_woodland_stalker');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(800358, 'aura_ascension_ranger_hawkeye'),
(705034, 'aura_ascension_ranger_woodland_stalker');
