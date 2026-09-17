-- CoA Fire Engraving (653211): direct damage has a 30% chance (Spell.dbc ProcChance) to apply Firebrand (653210).
-- ProcFlags: done melee/ranged auto attacks and melee, ranged and magic damage spells, on hit.
DELETE FROM `spell_proc` WHERE `SpellId` = 653211;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(653211, 0, 0, 0, 0, 0, 0x00010154, 0x1, 0x2, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `spell_script_names` WHERE `spell_id` = 653211 AND `ScriptName` = 'aura_ascension_runemaster_fire_engraving';
DELETE FROM `spell_script_names` WHERE `spell_id` = 653210 AND `ScriptName` = 'aura_ascension_runemaster_firebrand';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(653211, 'aura_ascension_runemaster_fire_engraving'),
(653210, 'aura_ascension_runemaster_firebrand');

-- Firebrand explosion (653212): ${$m1+0+$AP*.065+$spfi*.1} Fire damage per stack.
DELETE FROM `spell_bonus_data` WHERE `entry` = 653212;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(653212, 0.1, 0, 0.065, 0, 'CoA Fire Engraving - Firebrand explosion');
