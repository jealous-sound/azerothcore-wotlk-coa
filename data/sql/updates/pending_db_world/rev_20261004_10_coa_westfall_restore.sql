-- Westfall restore from Ascension captures: 15 missing quests (WDB 2026-09-08, Exiles DB 2026-09-13),
-- their givers, targets and objects, plus field differences in existing Westfall quests.
-- Positions are estimated; displays marked stand-in replace Ascension models the client lacks.

INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`,
  `KillCredit1`, `KillCredit2`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`,
  `faction`, `npcflag`, `speed_walk`, `speed_run`, `speed_swim`, `speed_flight`, `detection_range`, `rank`,
  `dmgschool`, `DamageModifier`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`,
  `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `type`, `type_flags`, `lootid`, `pickpocketloot`,
  `skinloot`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`,
  `HealthModifier`, `ManaModifier`, `ArmorModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`,
  `RegenHealth`, `CreatureImmunitiesId`, `flags_extra`, `ScriptName`, `VerifiedBuild`)
VALUES
(776786, 0, 0, 0, 0, 0, 'Captain Olens', 'The People''s Militia', NULL, 9950100, 14, 14, 0, 12, 3, 1, 1.14286, 1, 1, 18, 0, 0, 1, 2000, 2000, 1, 1, 1, 512, 2048, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 0.98, 1, 1, 1, 0, 0, 1, 0, 2, '', 0),
(776790, 0, 0, 0, 0, 0, 'Archivist Selnor', NULL, NULL, 0, 11, 11, 0, 12, 2, 1, 1.14286, 1, 1, 18, 0, 0, 1, 2000, 2000, 1, 1, 1, 512, 2048, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 0.98, 1, 1, 1, 0, 0, 1, 0, 2, '', 0),
(255150, 0, 0, 0, 0, 0, 'Idona Wyther', 'Alchemy Trainer', NULL, 4110, 26, 26, 0, 12, 83, 1, 1.14286, 1, 1, 18, 0, 0, 1, 1500, 2000, 1, 1, 1, 512, 2048, 0, 0, 7, 134217728, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1.064, 1, 1, 1, 0, 0, 1, 0, 2, '', 0),
(991515, 0, 0, 0, 0, 0, 'Lenore the Hoarder', '', NULL, 0, 15, 15, 0, 17, 0, 1, 1.14286, 1, 1, 18, 1, 0, 1, 2000, 2000, 1, 1, 1, 32768, 2048, 0, 0, 7, 0, 95, 95, 0, 0, 0, 3, 24, 'SmartAI', 0, 1, 3, 1, 1, 1, 0, 0, 1, 0, 0, '', 0),
(255339, 0, 0, 0, 0, 0, 'Militia Recruit', 'The People''s Militia', NULL, 0, 14, 16, 0, 7, 0, 1, 1.14286, 1, 1, 18, 0, 0, 1, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 1, 1.448, 1, 1, 1, 0, 0, 1, 0, 0, '', 0),
(157002, 0, 0, 0, 0, 0, 'Farmer Demont', '', NULL, 0, 15, 15, 0, 35, 2, 1, 1.14286, 1, 1, 18, 0, 0, 1, 1500, 2000, 1, 1, 1, 512, 2048, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1.25, 1, 1, 1, 0, 0, 1, 0, 2, '', 0),
(999900, 0, 0, 0, 0, 0, 'Slain Protector', NULL, NULL, 0, 15, 15, 0, 35, 2, 1, 1.14286, 1, 1, 18, 0, 0, 1, 2000, 2000, 1, 1, 1, 768, 2048, 32, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 2, '', 0),
(999901, 0, 0, 0, 0, 0, 'Half-Devoured Protector', 'The People''s Militia', NULL, 0, 15, 15, 0, 35, 2, 1, 1.14286, 1, 1, 18, 0, 0, 1, 2000, 2000, 1, 1, 1, 768, 2048, 32, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 2, '', 0)
ON DUPLICATE KEY UPDATE `difficulty_entry_1` = VALUES(`difficulty_entry_1`),
  `difficulty_entry_2` = VALUES(`difficulty_entry_2`), `difficulty_entry_3` = VALUES(`difficulty_entry_3`),
  `KillCredit1` = VALUES(`KillCredit1`), `KillCredit2` = VALUES(`KillCredit2`), `name` = VALUES(`name`),
  `subname` = VALUES(`subname`), `IconName` = VALUES(`IconName`), `gossip_menu_id` = VALUES(`gossip_menu_id`),
  `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `exp` = VALUES(`exp`),
  `faction` = VALUES(`faction`), `npcflag` = VALUES(`npcflag`), `speed_walk` = VALUES(`speed_walk`),
  `speed_run` = VALUES(`speed_run`), `speed_swim` = VALUES(`speed_swim`), `speed_flight` = VALUES(`speed_flight`),
  `detection_range` = VALUES(`detection_range`), `rank` = VALUES(`rank`), `dmgschool` = VALUES(`dmgschool`),
  `DamageModifier` = VALUES(`DamageModifier`), `BaseAttackTime` = VALUES(`BaseAttackTime`),
  `RangeAttackTime` = VALUES(`RangeAttackTime`), `BaseVariance` = VALUES(`BaseVariance`),
  `RangeVariance` = VALUES(`RangeVariance`), `unit_class` = VALUES(`unit_class`),
  `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`),
  `dynamicflags` = VALUES(`dynamicflags`), `family` = VALUES(`family`), `type` = VALUES(`type`),
  `type_flags` = VALUES(`type_flags`), `lootid` = VALUES(`lootid`), `pickpocketloot` = VALUES(`pickpocketloot`),
  `skinloot` = VALUES(`skinloot`), `PetSpellDataId` = VALUES(`PetSpellDataId`), `VehicleId` = VALUES(`VehicleId`),
  `mingold` = VALUES(`mingold`), `maxgold` = VALUES(`maxgold`), `AIName` = VALUES(`AIName`),
  `MovementType` = VALUES(`MovementType`), `HoverHeight` = VALUES(`HoverHeight`),
  `HealthModifier` = VALUES(`HealthModifier`), `ManaModifier` = VALUES(`ManaModifier`),
  `ArmorModifier` = VALUES(`ArmorModifier`), `ExperienceModifier` = VALUES(`ExperienceModifier`),
  `RacialLeader` = VALUES(`RacialLeader`), `movementId` = VALUES(`movementId`),
  `RegenHealth` = VALUES(`RegenHealth`), `CreatureImmunitiesId` = VALUES(`CreatureImmunitiesId`),
  `flags_extra` = VALUES(`flags_extra`), `ScriptName` = VALUES(`ScriptName`),
  `VerifiedBuild` = VALUES(`VerifiedBuild`);

-- Stand-in displays: Olens 177231, Selnor 177229, Idona 255150 and Lenore 119854 are not in the client.
DELETE FROM `creature_template_model` WHERE `CreatureID` IN (776786,776790,255150,991515,255339,157002,999900,999901);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`,
  `VerifiedBuild`) VALUES
(776786, 0, 7310, 1, 1, 0),
(776790, 0, 7311, 1, 1, 0),
(255150, 0, 1692, 1, 1, 0),
(991515, 0, 4419, 1, 1, 0),
(255339, 0, 7308, 1, 1, 0),
(255339, 1, 7309, 1, 1, 0),
(255339, 2, 7310, 1, 1, 0),
(255339, 3, 7311, 1, 1, 0),
(157002, 0, 19354, 1, 1, 0),
(999900, 0, 2368, 1, 1, 0),
(999901, 0, 7308, 1, 1, 0);

DELETE FROM `creature_template_addon` WHERE `entry` IN (999900,999901);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`,
  `visibilityDistanceType`, `auras`) VALUES
(999900, 0, 0, 7, 1, 0, 0, NULL),
(999901, 0, 0, 7, 1, 0, 0, NULL);

-- Estimated: Idona Wyther teaches the same alchemy list as Alchemist Mallory.
DELETE FROM `creature_default_trainer` WHERE `CreatureId`=255150;
INSERT INTO `creature_default_trainer` (`CreatureId`, `TrainerId`) VALUES
(255150, 67);

DELETE FROM `smart_scripts` WHERE `entryorguid`=255339 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`,
  `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`,
  `event_param6`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`,
  `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`,
  `target_y`, `target_z`, `target_o`, `comment`) VALUES
(255339, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 0, 42, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Militia Recruit - On Reset - Set Invincibility Hp Level 1'),
(255339, 0, 1, 2, 2, 0, 100, 0, 0, 1, 1000, 1000, 0, 0, 33, 255339, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Militia Recruit - Between 0-1% Health - Quest Credit ''Militia Training'''),
(255339, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Militia Recruit - On Link - Evade');

DELETE FROM `creature_text` WHERE `CreatureID`=991515;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`,
  `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(991515, 0, 0, 'HEY! Get away from my stuff!!!', 12, 0, 100, 0, 0, 0, 0, 0, 'Lenore the Hoarder - Aggro');

DELETE FROM `smart_scripts` WHERE `entryorguid`=991515 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`,
  `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`,
  `event_param6`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`,
  `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`,
  `target_y`, `target_z`, `target_o`, `comment`) VALUES
(991515, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Lenore the Hoarder - On Aggro - Say Line 0');

DELETE FROM `npc_text` WHERE `ID`=9950100;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `BroadcastTextID0`, `lang0`, `Probability0`, `em0_0`, `em0_1`,
  `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `BroadcastTextID1`, `lang1`, `Probability1`, `em1_0`,
  `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `BroadcastTextID2`, `lang2`, `Probability2`,
  `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `BroadcastTextID3`, `lang3`,
  `Probability3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `BroadcastTextID4`,
  `lang4`, `Probability4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`,
  `BroadcastTextID5`, `lang5`, `Probability5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`,
  `text6_1`, `BroadcastTextID6`, `lang6`, `Probability6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`,
  `text7_0`, `text7_1`, `BroadcastTextID7`, `lang7`, `Probability7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`,
  `em7_5`, `VerifiedBuild`) VALUES
(9950100, 'We must do what we can to press back the Defias. Most of us aren''t fighters, but I''ve spent enough time on both the farms and the front to teach a fool how to replace a pitchfork with a sword. If you want to help out the recruits, or get some practice of your own, feel free. Otherwise report to Captain Danuven, we need all the hands we can get - assuming they''re capable, of course.', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
DELETE FROM `gossip_menu` WHERE `MenuID`=9950100;
INSERT INTO `gossip_menu` (`MenuID`, `TextID`) VALUES
(9950100, 9950100);

DELETE FROM `creature` WHERE `guid` BETWEEN 9950100 AND 9950110;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `equipment_id`,
  `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`,
  `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`,
  `CreateObject`, `Comment`) VALUES
(9950100, 776786, 0, 40, 108, 1, 1, 0, -10516.5, 1062.5, 55.57, 1.72, 300, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Captain Olens (estimated position)'),
(9950101, 255339, 0, 40, 108, 1, 1, 0, -10521, 1068, 54.27, 5, 120, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Militia Recruit (estimated position)'),
(9950102, 255339, 0, 40, 108, 1, 1, 0, -10517, 1070.5, 54.43, 4.75, 120, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Militia Recruit (estimated position)'),
(9950103, 255339, 0, 40, 108, 1, 1, 0, -10513, 1069, 55.01, 4.4, 120, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Militia Recruit (estimated position)'),
(9950104, 255339, 0, 40, 108, 1, 1, 0, -10521, 1072.5, 53.29, 5.1, 120, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Militia Recruit (estimated position)'),
(9950105, 255150, 0, 40, 108, 1, 1, 0, -10489, 1060, 54.58, 3.14, 300, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Idona Wyther (estimated position)'),
(9950106, 776790, 0, 40, 108, 1, 1, 0, -10501.5, 1029.5, 60.6, 3.84, 300, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Archivist Selnor (estimated position, tower floor)'),
(9950107, 991515, 0, 40, 40, 1, 1, 0, -10259.3, 1780.44, 73.85, 5.82, 600, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Lenore the Hoarder (estimated position, Defias tower upper floor)'),
(9950108, 157002, 0, 40, 921, 1, 1, 0, -11141, 1826, 39.15, 5.05, 300, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Farmer Demont (estimated position, Demont''s Place ruins by the chimney)'),
(9950109, 999900, 0, 40, 40, 1, 1, 0, -10100, 980, 40.37, 2.2, 300, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Slain Protector (estimated position, broken harvester)'),
(9950110, 999901, 0, 40, 922, 1, 1, 0, -11030, 790, 37.55, 1, 300, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Half-Devoured Protector (estimated position, Riverpaw camp)');

-- Westfall Hoard (480104): Ascension's object id for it is unknown.
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`,
  `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`,
  `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`,
  `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild`)
VALUES
(480101, 3, 6448, 'Pilfered Lesser Healing Potions', '', '', '', 1, 43, 480101, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0),
(480102, 3, 6448, 'Pilfered Minor Healing Potions', '', '', '', 1, 43, 480102, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0),
(480103, 3, 6448, 'Pilfered Elixirs', '', '', '', 1, 43, 480103, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0),
(480104, 3, 36, 'Westfall Hoard', '', '', '', 1, 43, 480104, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0)
ON DUPLICATE KEY UPDATE `type` = VALUES(`type`), `displayId` = VALUES(`displayId`), `name` = VALUES(`name`),
  `IconName` = VALUES(`IconName`), `castBarCaption` = VALUES(`castBarCaption`), `unk1` = VALUES(`unk1`),
  `size` = VALUES(`size`), `Data0` = VALUES(`Data0`), `Data1` = VALUES(`Data1`), `Data2` = VALUES(`Data2`),
  `Data3` = VALUES(`Data3`), `Data4` = VALUES(`Data4`), `Data5` = VALUES(`Data5`), `Data6` = VALUES(`Data6`),
  `Data7` = VALUES(`Data7`), `Data8` = VALUES(`Data8`), `Data9` = VALUES(`Data9`), `Data10` = VALUES(`Data10`),
  `Data11` = VALUES(`Data11`), `Data12` = VALUES(`Data12`), `Data13` = VALUES(`Data13`), `Data14` = VALUES(`Data14`),
  `Data15` = VALUES(`Data15`), `Data16` = VALUES(`Data16`), `Data17` = VALUES(`Data17`), `Data18` = VALUES(`Data18`),
  `Data19` = VALUES(`Data19`), `Data20` = VALUES(`Data20`), `Data21` = VALUES(`Data21`), `Data22` = VALUES(`Data22`),
  `Data23` = VALUES(`Data23`), `AIName` = VALUES(`AIName`), `ScriptName` = VALUES(`ScriptName`),
  `VerifiedBuild` = VALUES(`VerifiedBuild`);

DELETE FROM `gameobject_template_addon` WHERE `entry` BETWEEN 480101 AND 480104;
INSERT INTO `gameobject_template_addon` (`entry`, `faction`, `flags`, `mingold`, `maxgold`, `artkit0`, `artkit1`,
  `artkit2`, `artkit3`) VALUES
(480101, 0, 4, 0, 0, 0, 0, 0, 0),
(480102, 0, 4, 0, 0, 0, 0, 0, 0),
(480103, 0, 4, 0, 0, 0, 0, 0, 0),
(480104, 0, 4, 0, 0, 0, 0, 0, 0);

DELETE FROM `gameobject_loot_template` WHERE `Entry` BETWEEN 480101 AND 480104;
INSERT INTO `gameobject_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`,
  `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
(480101, 480201, 0, 100, 1, 1, 0, 1, 1, 'Pilfered Lesser Healing Potions'),
(480102, 480202, 0, 100, 1, 1, 0, 1, 1, 'Pilfered Minor Healing Potions'),
(480103, 480203, 0, 100, 1, 1, 0, 1, 1, 'Pilfered Elixirs'),
(480104, 1252801, 0, 100, 1, 1, 0, 1, 1, 'Westfall Hoard');

DELETE FROM `gameobject` WHERE `guid` BETWEEN 9001100 AND 9001103;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`,
  `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`,
  `animprogress`, `state`, `ScriptName`, `VerifiedBuild`, `Comment`) VALUES
(9001100, 480101, 0, 40, 919, 1, 1, -10725, 1400, 36.38, 1.2, 0, 0, 0.564642, 0.825336, 900, 100, 1, '', 0, 'Pilfered Lesser Healing Potions (estimated position, Stendel''s Pond)'),
(9001101, 480102, 0, 40, 111, 1, 1, -9995.5, 1471, 41.4, 2.5, 0, 0, 0.948985, 0.315322, 900, 100, 1, '', 0, 'Pilfered Minor Healing Potions (estimated position, Jangolode shed)'),
(9001102, 480103, 0, 40, 40, 1, 1, -10140, 1723, 33.01, 4, 0, 0, 0.909297, -0.416147, 900, 100, 1, '', 0, 'Pilfered Elixirs (estimated position, by Sergeant Brashclaw)'),
(9001103, 480104, 0, 40, 40, 1, 1, -10259.7, 1775.1, 73.85, 5.82, 0, 0, 0.229528, -0.973302, 900, 100, 1, '', 0, 'Westfall Hoard (estimated position, Defias tower upper floor)');

-- 999914 and 999933 are autocomplete (Method 0) in the WDB capture; QuestType 2 keeps them in the log
-- until the report reaches Archivist Selnor.
INSERT INTO `quest_template` (`ID`, `QuestType`, `QuestLevel`, `MinLevel`, `QuestSortID`, `QuestInfoID`,
  `SuggestedGroupNum`, `RequiredFactionId1`, `RequiredFactionId2`, `RequiredFactionValue1`, `RequiredFactionValue2`,
  `RewardNextQuest`, `RewardXPDifficulty`, `RewardMoney`, `RewardMoneyDifficulty`, `RewardDisplaySpell`,
  `RewardSpell`, `RewardHonor`, `RewardKillHonor`, `StartItem`, `Flags`, `RequiredPlayerKills`, `RewardItem1`,
  `RewardAmount1`, `RewardItem2`, `RewardAmount2`, `RewardItem3`, `RewardAmount3`, `RewardItem4`, `RewardAmount4`,
  `ItemDrop1`, `ItemDropQuantity1`, `ItemDrop2`, `ItemDropQuantity2`, `ItemDrop3`, `ItemDropQuantity3`, `ItemDrop4`,
  `ItemDropQuantity4`, `RewardChoiceItemID1`, `RewardChoiceItemQuantity1`, `RewardChoiceItemID2`,
  `RewardChoiceItemQuantity2`, `RewardChoiceItemID3`, `RewardChoiceItemQuantity3`, `RewardChoiceItemID4`,
  `RewardChoiceItemQuantity4`, `RewardChoiceItemID5`, `RewardChoiceItemQuantity5`, `RewardChoiceItemID6`,
  `RewardChoiceItemQuantity6`, `POIContinent`, `POIx`, `POIy`, `POIPriority`, `RewardTitle`, `RewardTalents`,
  `RewardArenaPoints`, `RewardFactionID1`, `RewardFactionValue1`, `RewardFactionOverride1`, `RewardFactionID2`,
  `RewardFactionValue2`, `RewardFactionOverride2`, `RewardFactionID3`, `RewardFactionValue3`,
  `RewardFactionOverride3`, `RewardFactionID4`, `RewardFactionValue4`, `RewardFactionOverride4`, `RewardFactionID5`,
  `RewardFactionValue5`, `RewardFactionOverride5`, `TimeAllowed`, `AllowableRaces`, `LogTitle`, `LogDescription`,
  `QuestDescription`, `AreaDescription`, `QuestCompletionLog`, `RequiredNpcOrGo1`, `RequiredNpcOrGo2`,
  `RequiredNpcOrGo3`, `RequiredNpcOrGo4`, `RequiredNpcOrGoCount1`, `RequiredNpcOrGoCount2`, `RequiredNpcOrGoCount3`,
  `RequiredNpcOrGoCount4`, `RequiredItemId1`, `RequiredItemId2`, `RequiredItemId3`, `RequiredItemId4`,
  `RequiredItemId5`, `RequiredItemId6`, `RequiredItemCount1`, `RequiredItemCount2`, `RequiredItemCount3`,
  `RequiredItemCount4`, `RequiredItemCount5`, `RequiredItemCount6`, `Unknown0`, `ObjectiveText1`, `ObjectiveText2`,
  `ObjectiveText3`, `ObjectiveText4`, `VerifiedBuild`)
VALUES
(1313, 2, 12, 10, 40, 0, 0, 0, 0, 0, 0, 0, 5, 0, 270, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2457, 2, 2454, 2, 3383, 2, 2458, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Potions for Sentinel Hill', 'Recover the stolen potions from the Defias camps in Westfall.', 'I know the militia relies on my potions, but when those damned thieves steal half of my store - how can I ever hope to keep up?$B$BEarlier this week, the Defias stole three crates ready to distribute to the People''s Militia. Our defenders need those potions! Retrieve them, and I''ll give you a few to keep you safe as well.', '', 'Return the potions to Idona Wyther', 0, 0, 0, 0, 0, 0, 0, 0, 480201, 480202, 480203, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 'Pilfered Lesser Healing Potions', 'Pilfered Minor Healing Potions', 'Pilfered Elixirs', '', 0),
(17011, 2, 11, 10, 40, 0, 0, 0, 0, 0, 0, 0, 6, 500, 375, 0, 0, 0, 0, 0, 8, 0, 1397886, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Riverpaw Genocide', 'Slay 8 Riverpaw Brutes, 12 Riverpaw Mongrels, and 10 Riverpaw Herbalists.', 'Confoundit, $N, look at my farm! Not that there''s much to look at nowadays. It wasn''t much to begin with, but I had ambition! Now... now I have nothing, and it''s all because of those damn gnolls!$B$BListen $N, I don''t have much, but give a poor farmer what''s owed to him—revenge! I want you to go out there and slay every last gnoll you find. Every. Last. One.$B$BOnce you''ve dealt with at least 8 Riverpaw Brutes, 12 Riverpaw Mongrels, and 10 Riverpaw Herbalists then return here to me for your reward. You should be able to find these gnolls to the West and then more to the North, parallel to the coast. And $N, I wouldn''t mind if you lost count after a while, if you know what I mean.', '', 'Slay 8 Riverpaw Brutes, 12 Riverpaw Mongrels, and 10 Riverpaw Herbalists.', 124, 123, 501, 0, 10, 8, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 0),
(17012, 2, 13, 10, 40, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 375250, 125, 1397886, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 157018, 1, 157019, 1, 157020, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Riverpaw Genocide', 'Slay 12 Riverpaw Bandits, 10 Riverpaw Taskmasters, and 8 Riverpaw Mystics.', 'You''ve already done a poor farmer a great service, $N, but my revenge is not yet sated. Not while there are still gnolls alive a drawing breath.$B$BI know there''s another encampment of gnolls to the East. I want them dead! Slay at least 12 Riverpaw Bandits, 10 Riverpaw Taskmasters and 8 Riverpaw Mystics, and maybe then I can finally be at peace. At least as much peace as one can be with his hope and dreams laid bare before him.', '', 'Slay 12 Riverpaw Bandits, 10 Riverpaw Taskmasters, and 8 Riverpaw Mystics.', 452, 98, 453, 0, 12, 10, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 0),
(17014, 2, 17, 10, 40, 0, 0, 0, 0, 0, 0, 0, 5, 1000, 660, 0, 0, 0, 0, 0, 0, 0, 375250, 100, 1397885, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'The Killing Fields', 'Farmer Saldean wants you to kill 15 Harvest Reapers.', '$N, after seeing what you''re capable of I think I have another offer to make. You see, the Harvest Watchers aren''t the only things that have claimed these once fertile farmlands.$B$BTo the Southeast of here is what they now call the Dead Acre. There you will find Harvest Reapers. They''re just like the Harvest Watchers only meaner, but I don''t think that will be a problem for you.$B$BCome back to me once you''ve slain 15 of ''em and I''ll make sure it''s worth your time.', '', 'Return to Farmer Saldean at Saldean''s Farm in Westfall.', 115, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 0),
(26993, 2, -1, 10, 40, 0, 0, 0, 0, 0, 0, 26994, 5, 300, 223, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 0, 10000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'The Killing Fields', 'Slay 10 Rusty Harvest Golems and return to Farmer Furlbrow at The Jansen Stead.', 'Look at the state of these lands! Curse the Defias Brotherhood, with their goblins making mechanical terrors and sending them out on these fields! We farmers didn''t want any of this. How am I to feed my family and poor Old Blanchy if these golems are here?$B$BCan you help get rid of the golems nearby? You can find them just around here, and over west, by my pumpkin farm. Please help us! Stormwind has given up on us.', '', 'Return to Farmer Furlbrow in Westfall.', 480, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 0),
(26994, 2, -1, 10, 40, 0, 0, 0, 0, 0, 0, 26995, 2, 0, 52, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 0, 2500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'The Killing Fields', 'Speak to Farmer Saldean at Saldean''s Farm.', 'You''ve made good work of the golems around here, but I''m not the only one dealing with this. My good friend, Saldean, also told me the golems are destroying his farm. Do you think you could assist him? You''ll find his farm just south of here.', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 0),
(26995, 2, -1, 12, 40, 0, 0, 0, 0, 0, 0, 26996, 5, 400, 236, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 0, 10000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'The Killing Fields', 'Slay 10 Harvest Golems and return to Farmer Saldean at Saldean''s Farm.', 'I''m glad to see someone helping out around here. These golems are frightening off our citizens, and if we don''t get this sorted out soon, I fear Westfall will be overrun and taken by The Defias Brotherhood. Anyways, get out there and kill some golems! You can find some right on this farm, or at the Molsen Farm over to the west.', '', 'Return to Farmer Saldean in Westfall.', 36, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 0),
(26996, 2, 17, 12, 40, 0, 0, 0, 0, 0, 0, 26997, 5, 600, 450, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 0, 10000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'The Killing Fields', 'Slay 10 Harvest Watchers and return to Farmer Saldean at Saldean''s Farm.', 'Those golems you killed earlier aren''t the only ones damaging our lands! One of my farmers claims he saw some more out by The Alexston Farmstead, near the Gold Coast Quarry. If you wouldn''t mind killing some more golems, your assistance would be most appreciated.', '', 'Return to Farmer Saldean in Westfall.', 114, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 0),
(26997, 2, 17, 12, 40, 0, 0, 0, 0, 0, 0, 0, 6, 0, 585, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1561, 1, 3578, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 0, 10000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'The Killing Fields', 'Slay 10 Harvest Reapers and return to Farmer Saldean at Saldean''s Farm.', 'Most of our farms have been reclaimed, but there is one farm left to fight for. Far south of here, past Sentinel Hill, you will find The Dead Acre, where the most powerful golems roam and scorch the land. I tried to bring a few workers there, but the golems chased us away! I hope you''re up to the task $n, these golems are no joke!', '', 'Return to Farmer Saldean in Westfall.', 115, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 0),
(254042, 2, 12, 9, 40, 1, 3, 0, 0, 0, 0, 0, 5, 375, 202, 0, 0, 0, 0, 0, 0, 0, 375250, 100, 1397886, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Wanted: Lenore the Hoarder', 'Kill Lenore the Hoarder and return to Protector Gariel at Sentinel Hill.', 'Wanted: Lenore the Hoarder$B$BBy order of the People''s Militia, Lenore the Hoarder is hereby declared a threat to the stability of Westfall.$B$BThis individual is responsible for orchestrating repeated raids on supply caravans bound for militia outposts. Reports confirm she is stockpiling stolen goods and distributing them to Defias agents operating in the region.$B$BShe is clever, fast, and rarely seen twice in the same place. Do not underestimate her.', '', 'Report to Protector Gariel at Sentinel Hill in Westfall.', 991515, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 0),
(254043, 2, 12, 9, 40, 0, 0, 0, 0, 0, 0, 0, 5, 375, 202, 0, 0, 0, 0, 0, 0, 0, 375250, 100, 1397884, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Take It Back', 'Protector Gariel wants you to gather Westfall Supplies, then return to her at Sentinel Hill.', 'We are stretched thin out here. Rations are low, morale is worse, and every night another crate goes missing. The Defias are relentless, picking us apart while we try to hold what little ground we have left.$B$BWord is they are stashing stolen goods somewhere nearby. Not just scraps either. Real supplies, food, bandages, tools, maybe even weapons. We have reason to believe they are hiding them in that old tower to the west.$B$BIf you can make your way in and recover anything useful, it could turn the tide out here. The militia is doing what it can, but we need every edge we can get.', '', 'Report to Protector Gariel at Sentinel Hill in Westfall', 0, 0, 0, 0, 0, 0, 0, 0, 1252801, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, '', '', '', '', 0),
(255057, 2, -1, 10, 40, 0, 0, 0, 0, 0, 0, 0, 5, 500, 236, 0, 0, 0, 0, 0, 8, 0, 375250, 100, 1397885, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Militia Training', 'Practice your skills against the Militia Recruits.', 'You look like you can hold your own, against a cutthroat or two. $B$BSince you''re here, why don''t you test your skills against our recruits? It isn''t safe outside of Sentinel Hill and you should make sure your skills are sharp before heading off into Defias territory... not to mention this batch can certainly use the practice.$B$BI don''t like the thought of sending more fools out to their deaths, show me what you can do.', '', 'Return to Captain Olens at Sentinel Hill.', 255339, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Militia Recruits Defeated', '', '', '', 0),
(999913, 2, 11, 9, 40, 0, 0, 0, 0, 0, 0, 0, 4, 0, 270, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'The Missing Report', 'Find the report from the missing patroller.', 'Do you know how difficult it is to sort papers when I''m missing so many reports? $B$BI understand the tedious task of filing paperwork is unappreciated by some, but that doesn''t mean it isn''t important.$B$BI''m missing the field report from one of our patrollers, he walks the route from Sentinel Hill to the border of Elwynn. Find him for me, and bring me back his report since he can''t be bothered to do it himself.', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 0),
(999914, 2, 11, 9, 40, 0, 0, 0, 0, 0, 0, 0, 4, 0, 270, 0, 0, 0, 0, 999920, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'The Half-Detailed Report', 'Return the Half-Detailed Report to Archivist Selnor.', 'Upon inspecting the body, you find the patroller''s half-completed report. $B$BThis should be delivered back to Sentinel Hill - the Archivist will want to know what happened.', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 999920, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 'Deliver the Half-Detailed Report to Archivist Selnor.', '', '', '', 0),
(999933, 2, 16, 12, 40, 0, 0, 0, 0, 0, 0, 0, 4, 0, 270, 0, 0, 0, 0, 999923, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'The Spit-Soaked Report', 'Deliver the Spit-Soaked Report to Archivist Selnor.', 'The parchment is disgusting - covered in spit and gore. The author of these papers lay in pieces all around you, unrecognizable. $B$BArchivist Selnor will want to see this report, she will know who this is.', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 999923, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 'Deliver the Spit-Soaked Report to Archivist Selnor.', '', '', '', 0)
ON DUPLICATE KEY UPDATE `QuestType` = VALUES(`QuestType`), `QuestLevel` = VALUES(`QuestLevel`),
  `MinLevel` = VALUES(`MinLevel`), `QuestSortID` = VALUES(`QuestSortID`), `QuestInfoID` = VALUES(`QuestInfoID`),
  `SuggestedGroupNum` = VALUES(`SuggestedGroupNum`), `RequiredFactionId1` = VALUES(`RequiredFactionId1`),
  `RequiredFactionId2` = VALUES(`RequiredFactionId2`), `RequiredFactionValue1` = VALUES(`RequiredFactionValue1`),
  `RequiredFactionValue2` = VALUES(`RequiredFactionValue2`), `RewardNextQuest` = VALUES(`RewardNextQuest`),
  `RewardXPDifficulty` = VALUES(`RewardXPDifficulty`), `RewardMoney` = VALUES(`RewardMoney`),
  `RewardMoneyDifficulty` = VALUES(`RewardMoneyDifficulty`), `RewardDisplaySpell` = VALUES(`RewardDisplaySpell`),
  `RewardSpell` = VALUES(`RewardSpell`), `RewardHonor` = VALUES(`RewardHonor`),
  `RewardKillHonor` = VALUES(`RewardKillHonor`), `StartItem` = VALUES(`StartItem`), `Flags` = VALUES(`Flags`),
  `RequiredPlayerKills` = VALUES(`RequiredPlayerKills`), `RewardItem1` = VALUES(`RewardItem1`),
  `RewardAmount1` = VALUES(`RewardAmount1`), `RewardItem2` = VALUES(`RewardItem2`),
  `RewardAmount2` = VALUES(`RewardAmount2`), `RewardItem3` = VALUES(`RewardItem3`),
  `RewardAmount3` = VALUES(`RewardAmount3`), `RewardItem4` = VALUES(`RewardItem4`),
  `RewardAmount4` = VALUES(`RewardAmount4`), `ItemDrop1` = VALUES(`ItemDrop1`),
  `ItemDropQuantity1` = VALUES(`ItemDropQuantity1`), `ItemDrop2` = VALUES(`ItemDrop2`),
  `ItemDropQuantity2` = VALUES(`ItemDropQuantity2`), `ItemDrop3` = VALUES(`ItemDrop3`),
  `ItemDropQuantity3` = VALUES(`ItemDropQuantity3`), `ItemDrop4` = VALUES(`ItemDrop4`),
  `ItemDropQuantity4` = VALUES(`ItemDropQuantity4`), `RewardChoiceItemID1` = VALUES(`RewardChoiceItemID1`),
  `RewardChoiceItemQuantity1` = VALUES(`RewardChoiceItemQuantity1`),
  `RewardChoiceItemID2` = VALUES(`RewardChoiceItemID2`),
  `RewardChoiceItemQuantity2` = VALUES(`RewardChoiceItemQuantity2`),
  `RewardChoiceItemID3` = VALUES(`RewardChoiceItemID3`),
  `RewardChoiceItemQuantity3` = VALUES(`RewardChoiceItemQuantity3`),
  `RewardChoiceItemID4` = VALUES(`RewardChoiceItemID4`),
  `RewardChoiceItemQuantity4` = VALUES(`RewardChoiceItemQuantity4`),
  `RewardChoiceItemID5` = VALUES(`RewardChoiceItemID5`),
  `RewardChoiceItemQuantity5` = VALUES(`RewardChoiceItemQuantity5`),
  `RewardChoiceItemID6` = VALUES(`RewardChoiceItemID6`),
  `RewardChoiceItemQuantity6` = VALUES(`RewardChoiceItemQuantity6`), `POIContinent` = VALUES(`POIContinent`),
  `POIx` = VALUES(`POIx`), `POIy` = VALUES(`POIy`), `POIPriority` = VALUES(`POIPriority`),
  `RewardTitle` = VALUES(`RewardTitle`), `RewardTalents` = VALUES(`RewardTalents`),
  `RewardArenaPoints` = VALUES(`RewardArenaPoints`), `RewardFactionID1` = VALUES(`RewardFactionID1`),
  `RewardFactionValue1` = VALUES(`RewardFactionValue1`), `RewardFactionOverride1` = VALUES(`RewardFactionOverride1`),
  `RewardFactionID2` = VALUES(`RewardFactionID2`), `RewardFactionValue2` = VALUES(`RewardFactionValue2`),
  `RewardFactionOverride2` = VALUES(`RewardFactionOverride2`), `RewardFactionID3` = VALUES(`RewardFactionID3`),
  `RewardFactionValue3` = VALUES(`RewardFactionValue3`), `RewardFactionOverride3` = VALUES(`RewardFactionOverride3`),
  `RewardFactionID4` = VALUES(`RewardFactionID4`), `RewardFactionValue4` = VALUES(`RewardFactionValue4`),
  `RewardFactionOverride4` = VALUES(`RewardFactionOverride4`), `RewardFactionID5` = VALUES(`RewardFactionID5`),
  `RewardFactionValue5` = VALUES(`RewardFactionValue5`), `RewardFactionOverride5` = VALUES(`RewardFactionOverride5`),
  `TimeAllowed` = VALUES(`TimeAllowed`), `AllowableRaces` = VALUES(`AllowableRaces`),
  `LogTitle` = VALUES(`LogTitle`), `LogDescription` = VALUES(`LogDescription`),
  `QuestDescription` = VALUES(`QuestDescription`), `AreaDescription` = VALUES(`AreaDescription`),
  `QuestCompletionLog` = VALUES(`QuestCompletionLog`), `RequiredNpcOrGo1` = VALUES(`RequiredNpcOrGo1`),
  `RequiredNpcOrGo2` = VALUES(`RequiredNpcOrGo2`), `RequiredNpcOrGo3` = VALUES(`RequiredNpcOrGo3`),
  `RequiredNpcOrGo4` = VALUES(`RequiredNpcOrGo4`), `RequiredNpcOrGoCount1` = VALUES(`RequiredNpcOrGoCount1`),
  `RequiredNpcOrGoCount2` = VALUES(`RequiredNpcOrGoCount2`),
  `RequiredNpcOrGoCount3` = VALUES(`RequiredNpcOrGoCount3`),
  `RequiredNpcOrGoCount4` = VALUES(`RequiredNpcOrGoCount4`), `RequiredItemId1` = VALUES(`RequiredItemId1`),
  `RequiredItemId2` = VALUES(`RequiredItemId2`), `RequiredItemId3` = VALUES(`RequiredItemId3`),
  `RequiredItemId4` = VALUES(`RequiredItemId4`), `RequiredItemId5` = VALUES(`RequiredItemId5`),
  `RequiredItemId6` = VALUES(`RequiredItemId6`), `RequiredItemCount1` = VALUES(`RequiredItemCount1`),
  `RequiredItemCount2` = VALUES(`RequiredItemCount2`), `RequiredItemCount3` = VALUES(`RequiredItemCount3`),
  `RequiredItemCount4` = VALUES(`RequiredItemCount4`), `RequiredItemCount5` = VALUES(`RequiredItemCount5`),
  `RequiredItemCount6` = VALUES(`RequiredItemCount6`), `Unknown0` = VALUES(`Unknown0`),
  `ObjectiveText1` = VALUES(`ObjectiveText1`), `ObjectiveText2` = VALUES(`ObjectiveText2`),
  `ObjectiveText3` = VALUES(`ObjectiveText3`), `ObjectiveText4` = VALUES(`ObjectiveText4`),
  `VerifiedBuild` = VALUES(`VerifiedBuild`);

DELETE FROM `quest_template_addon` WHERE `ID` IN (1313,17011,17012,17014,26993,26994,26995,26996,26997,254042,254043,255057,999913,999914,999933);
INSERT INTO `quest_template_addon` (`ID`, `MaxLevel`, `AllowableClasses`, `SourceSpellID`, `PrevQuestID`,
  `NextQuestID`, `ExclusiveGroup`, `BreadcrumbForQuestId`, `RewardMailTemplateID`, `RewardMailDelay`,
  `RequiredSkillID`, `RequiredSkillPoints`, `RequiredMinRepFaction`, `RequiredMaxRepFaction`, `RequiredMinRepValue`,
  `RequiredMaxRepValue`, `ProvidedItemCount`, `SpecialFlags`) VALUES
(1313, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(17011, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(17012, 0, 0, 0, 17011, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(17014, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(26993, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(26994, 0, 0, 0, 26993, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(26995, 0, 0, 0, 26994, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(26996, 0, 0, 0, 26995, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(26997, 0, 0, 0, 26996, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(254042, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(254043, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(255057, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(999913, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(999914, 0, 0, 0, 999913, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(999933, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `creature_queststarter` WHERE `quest` IN (1313,17011,17012,17014,26993,26994,26995,26996,26997,254042,254043,255057,999913,999914,999933);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(237, 26993),
(237, 26994),
(233, 26995),
(233, 26996),
(233, 26997),
(233, 17014),
(157002, 17011),
(157002, 17012),
(878, 254042),
(490, 254043),
(776786, 255057),
(776790, 999913),
(999900, 999914),
(999901, 999933),
(255150, 1313);

DELETE FROM `creature_questender` WHERE `quest` IN (1313,17011,17012,17014,26993,26994,26995,26996,26997,254042,254043,255057,999913,999914,999933);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(237, 26993),
(233, 26994),
(233, 26995),
(233, 26996),
(233, 26997),
(233, 17014),
(157002, 17011),
(157002, 17012),
(490, 254042),
(490, 254043),
(776786, 255057),
(999900, 999913),
(776790, 999914),
(776790, 999933),
(255150, 1313);

UPDATE `creature_template` SET `npcflag` = `npcflag` | 2 WHERE `entry` = 490;

UPDATE `quest_template` SET `QuestLevel` = -1 WHERE `ID` IN (22,36,38,64,65,151,153,184,208,214);
UPDATE `quest_template` SET `QuestLevel` = 15, `MinLevel` = 10 WHERE `ID` = 104;
UPDATE `quest_template` SET `MinLevel` = 35 WHERE `ID` = 50;
UPDATE `quest_template` SET `MinLevel` = 10 WHERE `ID` IN (166,214);
UPDATE `quest_template` SET `RequiredItemId1` = 1358, `RequiredItemCount1` = 1 WHERE `ID` = 138;
UPDATE `quest_template` SET `RequiredItemId1` = 1361, `RequiredItemCount1` = 1 WHERE `ID` = 139;
UPDATE `quest_template` SET `RequiredItemId1` = 1362, `RequiredItemCount1` = 1 WHERE `ID` = 140;
