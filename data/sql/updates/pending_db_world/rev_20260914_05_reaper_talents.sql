DELETE FROM `spell_script_names` WHERE `spell_id` = 92145 AND `ScriptName` = 'aura_ascension_harvester';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (92145, 'aura_ascension_harvester');
DELETE FROM `spell_proc` WHERE `SpellId` = 92145;
INSERT INTO `spell_proc` (`SpellId`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `Chance`)
VALUES (92145, 332116, 1, 2, 3, 2, 100);
DELETE FROM `spell_bonus_data` WHERE `entry` = 805720;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(805720, 0, 0, 0, 0.05, 'Reaper - Soul Splinter: 5% AP per tick; 3.5% Stamina in the base-value hook');
