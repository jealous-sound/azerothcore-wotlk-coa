-- #88: captured tornado entry/model; its native aura provides root and school immunity.
-- Model DBC preparation is deferred to apps/coa-spells/raging_zephyr.py when packaging is requested.
DELETE FROM `creature_template_model` WHERE `CreatureID` = 4078281;
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `ScriptName`)
SELECT 4078281, 'Raging Zephyr', 1, 1, 35, 1, 11, 'npc_ascension_stormbringer_zephyr'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 4078281);
UPDATE `creature_template` SET `ScriptName` = 'npc_ascension_stormbringer_zephyr' WHERE `entry` = 4078281;
DELETE FROM `creature_template_model` WHERE `CreatureID` = 4078281;
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) VALUES
(4078281, 0, 407828, 1, 1);
DELETE FROM `creature_model_info` WHERE `DisplayID` = 407828;
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) VALUES
(407828, 0.5, 0, 2);
DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_ascension_raging_zephyr', 'spell_ascension_zephyr_pull');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(704201, 'spell_ascension_raging_zephyr'),
(704210, 'spell_ascension_zephyr_pull');
