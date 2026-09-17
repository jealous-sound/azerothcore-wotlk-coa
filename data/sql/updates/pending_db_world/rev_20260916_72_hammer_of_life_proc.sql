-- Hammer of Life (803973): heal nearby allies and damage nearby enemies for 20% of melee damage dealt. Spell.dbc
-- gives it no proc flags; bind its script to melee auto attacks and melee abilities that deal damage.
START TRANSACTION;
DELETE FROM `spell_proc` WHERE `SpellId` = 803973;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(803973, 0, 0, 0, 0, 0, 20, 1, 2, 0, 0, 0, 0, 0, 0, 0);
DELETE FROM `spell_script_names` WHERE `spell_id` = 803973 AND `ScriptName` = 'aura_ascension_hammer_of_life';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(803973, 'aura_ascension_hammer_of_life');
COMMIT;
