-- #88: captured CoA Air Elemental, using the existing white elemental display and native PetAI.
-- Family 0 and class 2 use native summoned-pet stat defaults; no guessed Freepick pet-family spell list.
DELETE FROM `creature_template_model` WHERE `CreatureID` = 500941;
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `family`, `spell1`, `spell2`, `spell3`)
SELECT 500941, 'Air Elemental', 1, 1, 35, 2, 4, 0, 806016, 300836, 804022
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 500941);
UPDATE `creature_template` SET `spell1` = 806016, `spell2` = 300836, `spell3` = 804022 WHERE `entry` = 500941;
DELETE FROM `creature_template_model` WHERE `CreatureID` = 500941;
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) VALUES
(500941, 0, 8714, 1, 1);
DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('aura_ascension_air_invigoration', 'spell_ascension_air_invigoration_duration');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(806020, 'aura_ascension_air_invigoration'),
(680918, 'spell_ascension_air_invigoration_duration');
DELETE FROM `spell_proc` WHERE `SpellId` = 806020;
INSERT INTO `spell_proc` (`SpellId`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `Chance`)
VALUES (806020, 332116, 1, 2, 3, 2, 100);
