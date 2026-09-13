-- #88: captured green Dream Flower marker and spells; native pickup visual, radius and heal remain intact.
DELETE FROM `creature_template_model` WHERE `CreatureID` = 454239;
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `ScriptName`)
SELECT 454239, 'Dream Flower', 1, 1, 35, 1, 11, 'npc_ascension_ranger_flower'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 454239);
UPDATE `creature_template` SET `ScriptName` = 'npc_ascension_ranger_flower' WHERE `entry` = 454239;
DELETE FROM `creature_template_model` WHERE `CreatureID` = 454239;
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) VALUES
(454239, 0, 100003, 1, 1);
DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_ascension_ranger_green_flower', 'spell_ascension_ranger_flower_pickup', 'aura_ascension_ranger_dream_flowers', 'spell_ascension_ranger_highlander');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(561005, 'spell_ascension_ranger_green_flower'),
(800246, 'spell_ascension_ranger_green_flower'),
(803507, 'spell_ascension_ranger_green_flower'),
(803508, 'spell_ascension_ranger_green_flower'),
(803510, 'spell_ascension_ranger_green_flower'),
(803720, 'spell_ascension_ranger_green_flower'),
(561008, 'spell_ascension_ranger_flower_pickup'),
(92117, 'aura_ascension_ranger_dream_flowers'),
(806345, 'spell_ascension_ranger_highlander'),
(806437, 'spell_ascension_ranger_highlander'),
(806438, 'spell_ascension_ranger_highlander'),
(806439, 'spell_ascension_ranger_highlander'),
(806440, 'spell_ascension_ranger_highlander'),
(806441, 'spell_ascension_ranger_highlander'),
(806442, 'spell_ascension_ranger_highlander'),
(806443, 'spell_ascension_ranger_highlander');
DELETE FROM `spell_proc` WHERE `SpellId` = 92117;
INSERT INTO `spell_proc` (`SpellId`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `Chance`, `Cooldown`)
VALUES (92117, 332116, 1, 2, 2, 2, 100, 1000);
