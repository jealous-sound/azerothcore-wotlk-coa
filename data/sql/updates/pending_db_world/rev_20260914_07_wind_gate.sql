-- #88: captured Wind Gate entry/display; neutral marker durability is local compatibility policy.
-- The matching display/model DBC rows are prepared by apps/coa-spells/wind_gate.py when packaging is requested.
DELETE FROM `creature_template_model` WHERE `CreatureID` = 617478;
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `ScriptName`)
SELECT 617478, 'Wind Gate', 1, 1, 35, 1, 11, 'npc_ascension_stormbringer_gate'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 617478);
UPDATE `creature_template` SET `ScriptName` = 'npc_ascension_stormbringer_gate' WHERE `entry` = 617478;
DELETE FROM `creature_template_model` WHERE `CreatureID` = 617478;
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) VALUES
(617478, 0, 135809, 1, 1);
DELETE FROM `creature_model_info` WHERE `DisplayID` = 135809;
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) VALUES
(135809, 0.5, 0, 2);
DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_ascension_wind_gate', 'spell_ascension_wind_gate_evacuate');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(504401, 'spell_ascension_wind_gate'),
(504643, 'spell_ascension_wind_gate_evacuate');
