-- Chasing Death, which never fired, and Tormentor, whose cooldown was never cut.
--
-- 707455 promises that Deathchaser used against a target below 20% health reapplies itself.
-- Its record is a SPELL_AURA_PROC_TRIGGER_SPELL with ProcTypeMask 0, so LoadSpellProcs makes
-- no entry and the aura never sees an event.
--
-- It needs two answers at once and already has both. spell_ascension_reaper_talent_proc asks
-- whether the spell was one the talent names, and the list for 707455 is every rank of
-- Deathchaser. aura_ascension_jailers_call asks whether the victim is below 20% health, which
-- is the other half of this tooltip word for word. Every DoCheckProc on a spell has to pass,
-- so binding both is the whole condition.
--
-- The flags are the ones every "damage dealt" talent in rev_20260920_17 uses.

DELETE FROM `spell_proc` WHERE `SpellId` = 707455;
INSERT INTO `spell_proc`
  (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`,
   `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`,
   `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`)
VALUES
  (707455, 0, 0, 0, 0, 0, 69652, 1, 2, 0, 0, 0, 0, 100, 0, 0);

DELETE FROM `spell_script_names` WHERE `spell_id` = 707455;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(707455, 'spell_ascension_reaper_talent_proc'),
(707455, 'aura_ascension_jailers_call');
