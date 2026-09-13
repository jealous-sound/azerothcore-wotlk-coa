-- #88: Petalkeeper's captured red flower; model tables are prepared only when packaging is requested.
DELETE FROM `creature_template_model` WHERE `CreatureID` = 454240;
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `ScriptName`)
SELECT 454240, 'Red Dream Flower', 1, 1, 35, 1, 11, 'npc_ascension_ranger_red_flower'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 454240);
UPDATE `creature_template` SET `ScriptName` = 'npc_ascension_ranger_red_flower' WHERE `entry` = 454240;
DELETE FROM `creature_template_model` WHERE `CreatureID` = 454240;
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) VALUES
(454240, 0, 137539, 1, 1);
DELETE FROM `creature_model_info` WHERE `DisplayID` = 137539;
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) VALUES
(137539, 0.5, 0, 2);
DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_ascension_ranger_red_flower', 'aura_ascension_ranger_petalkeeper', 'aura_ascension_ranger_red_dream');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(521222, 'spell_ascension_ranger_red_flower'),
(520925, 'aura_ascension_ranger_petalkeeper'),
(521451, 'aura_ascension_ranger_red_dream');
DELETE FROM `spell_proc` WHERE `SpellId` IN (520925, 521451);
INSERT INTO `spell_proc` (`SpellId`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `Chance`) VALUES
(520925, 332116, 1, 2, 2, 2, 100),
(521451, 332116, 1, 2, 3, 2, 100);
