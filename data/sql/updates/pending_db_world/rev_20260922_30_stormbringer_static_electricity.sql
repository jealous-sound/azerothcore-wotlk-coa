-- Static Electricity (524954) promises an Electrified Water Elemental and never spawned one.
--
-- Spell.dbc 573438 summons creature 310603, and that creature has no creature_template row
-- anywhere in this repository, so even a working summon would have had no template to build.
--
-- The template follows the ones the other custom classes already use for a short-lived companion
-- the player leaves behind: faction 35 so nothing attacks it by faction alone, type 4 (elemental),
-- and a level band that keeps it out of level scaling. The script sets the owner's faction and
-- level when it is summoned. Display 525 is the shipped Water Elemental display, the one
-- creature_template_model already gives creature 510.
INSERT INTO `creature_template`
  (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `AIName`,
   `MovementType`, `ScriptName`)
VALUES
  (310603, 'Electrified Water Elemental', 80, 80, 35, 1, 4, '', 0,
   'npc_ascension_stormbringer_electrified_water')
ON DUPLICATE KEY UPDATE
  `name` = VALUES(`name`), `faction` = VALUES(`faction`), `unit_class` = VALUES(`unit_class`),
  `type` = VALUES(`type`), `AIName` = VALUES(`AIName`), `MovementType` = VALUES(`MovementType`),
  `ScriptName` = VALUES(`ScriptName`);

DELETE FROM `creature_template_model` WHERE `CreatureID` = 310603;
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) VALUES
(310603, 0, 525, 1, 1);
