-- The NPC behind npc_ascension_level_scaling_toggle: a gossip switch for the two process-wide
-- LocalLevelScaling flags, which until now could only be changed by editing
-- mod_ascension_compat.conf and restarting the world.
--
-- Placed in the Orgrimmar bank, inside the triangle formed by the three bankers already spawned
-- there - Karus (1627.32, -4376.07, 12.06), Koma (1632.39, -4381.97, 12.04) and Soran
-- (1622.90, -4369.06, 12.05) - so the ground height is taken from confirmed spawns.

DELETE FROM `npc_text` WHERE `ID` = 990010;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `Probability0`) VALUES
(990010,
 'Level scaling decides whether the world rises to meet you or stays where it was written. Say the word and I will hold it either way.',
 'Level scaling decides whether the world rises to meet you or stays where it was written. Say the word and I will hold it either way.',
 0, 1);

DELETE FROM `creature_template_model` WHERE `CreatureID` = 990010;
DELETE FROM `creature_template` WHERE `entry` = 990010;
INSERT INTO `creature_template`
  (`entry`, `name`, `subname`, `gossip_menu_id`, `minlevel`, `maxlevel`, `faction`, `npcflag`,
   `speed_walk`, `speed_run`, `unit_class`, `unit_flags`, `type`, `AIName`,
   `MovementType`, `flags_extra`, `ScriptName`)
VALUES
  (990010, 'Keeper of Proportion', 'Level Scaling', 0, 80, 80, 35, 1,
   1, 1.14286, 1, 2, 7, '', 0, 2, 'npc_ascension_level_scaling_toggle');

-- Display 1310 is the orc male the Orgrimmar banker Karus (3309) already uses, so the NPC reads as
-- one more attendant standing in the bank.
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) VALUES
(990010, 0, 1310, 1, 1);

DELETE FROM `creature` WHERE `guid` = 5400003;
INSERT INTO `creature`
  (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `equipment_id`,
   `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`,
   `currentwaypoint`, `curhealth`, `curmana`, `MovementType`)
VALUES
  (5400003, 990010, 1, 0, 0, 1, 1, 0, 1630.00, -4372.50, 12.05, 3.500, 300, 0, 0, 1, 0, 0);
