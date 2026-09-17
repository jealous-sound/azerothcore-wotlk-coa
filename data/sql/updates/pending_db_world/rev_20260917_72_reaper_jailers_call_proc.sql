-- The Jailer's Call (805195): "Attacks against enemies below 20% health deal an additional ... Shadow damage".
-- Its proc aura triggers 704299, but Spell.dbc gives it ProcFlags 0 and no row existed, so it never procced.
-- Proc on melee auto attacks and melee abilities that deal damage; the script limits it to targets below 20%.
DELETE FROM `spell_proc` WHERE `SpellId` = 805195;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(805195, 0, 0, 0, 0, 0, 20, 1, 2, 0, 0, 0, 0, 0, 0, 0);
DELETE FROM `spell_script_names` WHERE `spell_id` = 805195 AND `ScriptName` = 'aura_ascension_jailers_call';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(805195, 'aura_ascension_jailers_call');
