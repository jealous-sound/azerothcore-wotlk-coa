-- Westfall restore from Ascension captures: 17 missing quests (WDB 2026-09-07..10, Exiles DB 2026-09-13),
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
(776787, 0, 0, 0, 0, 0, 'Protector Brenolt', 'The People''s Militia', NULL, 0, 14, 14, 0, 12, 0, 1, 1.14286, 1, 1, 18, 0, 0, 1, 2000, 2000, 1, 1, 1, 512, 2048, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 1, 0.98, 1, 1, 1, 0, 0, 1, 0, 2, '', 0),
(776790, 0, 0, 0, 0, 0, 'Archivist Selnor', NULL, NULL, 0, 11, 11, 0, 12, 2, 1, 1.14286, 1, 1, 18, 0, 0, 1, 2000, 2000, 1, 1, 1, 512, 2048, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 0.98, 1, 1, 1, 0, 0, 1, 0, 2, '', 0),
(255150, 0, 0, 0, 0, 0, 'Idona Wyther', 'Alchemy Trainer', NULL, 4110, 26, 26, 0, 12, 83, 1, 1.14286, 1, 1, 18, 0, 0, 1, 1500, 2000, 1, 1, 1, 512, 2048, 0, 0, 7, 134217728, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1.064, 1, 1, 1, 0, 0, 1, 0, 2, '', 0),
(255151, 0, 0, 0, 0, 0, 'Tavin Wyther', 'Alchemy Supplies', NULL, 0, 23, 23, 0, 12, 128, 1, 1.14286, 1, 1, 18, 0, 0, 1, 1500, 2000, 1, 1, 1, 512, 2048, 0, 0, 7, 134217728, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1.064, 1, 1, 1, 0, 0, 1, 0, 2, '', 0),
(991515, 0, 0, 0, 0, 0, 'Lenore the Hoarder', '', NULL, 0, 15, 15, 0, 17, 0, 1, 1.14286, 1, 1, 18, 1, 0, 1, 2000, 2000, 1, 1, 1, 32768, 2048, 0, 0, 7, 0, 991515, 95, 0, 0, 0, 3, 24, 'SmartAI', 0, 1, 3, 1, 1, 1, 0, 0, 1, 0, 0, '', 0),
(255339, 0, 0, 0, 0, 0, 'Militia Recruit', 'The People''s Militia', NULL, 58274, 14, 16, 0, 12, 1, 1, 1.14286, 1, 1, 18, 0, 0, 1, 2000, 2000, 1, 1, 1, 0, 2048, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 1, 1.448, 1, 1, 1, 0, 0, 1, 0, 0, '', 0),
(157002, 0, 0, 0, 0, 0, 'Farmer Demont', '', NULL, 0, 15, 15, 0, 35, 2, 1, 1.14286, 1, 1, 18, 0, 0, 1, 1500, 2000, 1, 1, 1, 512, 2048, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1.25, 1, 1, 1, 0, 0, 1, 0, 2, '', 0),
(455339, 0, 0, 0, 0, 0, 'Slain Protector', 'The People''s Militia', NULL, 0, 15, 15, 0, 35, 2, 1, 1.14286, 1, 1, 18, 0, 0, 1, 2000, 2000, 1, 1, 1, 768, 2048, 32, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 2.048, 1, 1, 1, 0, 0, 1, 0, 2, '', 0),
(455343, 0, 0, 0, 0, 0, 'Half-Devoured Protector', 'The People''s Militia', NULL, 0, 15, 15, 0, 35, 2, 1, 1.14286, 1, 1, 18, 0, 0, 1, 2000, 2000, 1, 1, 1, 768, 2048, 32, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 2.048, 1, 1, 1, 0, 0, 1, 0, 2, '', 0),
(100467, 0, 0, 0, 0, 0, 'Path to Ascension Flightmaster Credit', '', NULL, 0, 1, 1, 0, 35, 0, 1, 1.14286, 1, 1, 20, 0, 0, 1, 0, 0, 1, 1, 1, 2, 0, 0, 0, 7, 2147483648, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, '', 0)
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

-- Stand-in displays: Olens 177231, Brenolt 177232, Selnor 177229, Idona 255150, Tavin 255151 and
-- Lenore 119854, Slain Protector 177256 and Half-Devoured Protector 177252 are not in the client.
DELETE FROM `creature_template_model` WHERE `CreatureID` IN (776786,776787,776790,255150,255151,991515,255339,157002,455339,455343,100467);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`,
  `VerifiedBuild`) VALUES
(776786, 0, 7310, 1, 1, 0),
(776787, 0, 7309, 1, 1, 0),
(776790, 0, 7311, 1, 1, 0),
(255150, 0, 1692, 1, 1, 0),
(255151, 0, 3649, 1, 1, 0),
(991515, 0, 4419, 1, 1, 0),
(255339, 0, 7308, 1, 1, 0),
(255339, 1, 7309, 1, 1, 0),
(255339, 2, 7310, 1, 1, 0),
(255339, 3, 7311, 1, 1, 0),
(157002, 0, 19354, 1, 1, 0),
(455339, 0, 2368, 1, 1, 0),
(455343, 0, 7308, 1, 1, 0),
(100467, 0, 11686, 1, 1, 0);

DELETE FROM `creature_template_addon` WHERE `entry` IN (455339,455343);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`,
  `visibilityDistanceType`, `auras`) VALUES
(455339, 0, 0, 7, 1, 0, 0, NULL),
(455343, 0, 0, 7, 1, 0, 0, NULL);

-- Estimated: Idona Wyther teaches the same alchemy list as Alchemist Mallory.
DELETE FROM `creature_default_trainer` WHERE `CreatureId`=255150;
INSERT INTO `creature_default_trainer` (`CreatureId`, `TrainerId`) VALUES
(255150, 67);

-- Tavin Wyther's first vendor page as seen in footage; the rest of his list is unknown.
DELETE FROM `npc_vendor` WHERE `entry`=255151;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `VerifiedBuild`) VALUES
(255151, 0, 3371, 0, 0, 0, 0),
(255151, 1, 3372, 0, 0, 0, 0),
(255151, 2, 8925, 0, 0, 0, 0),
(255151, 3, 18256, 0, 0, 0, 0),
(255151, 4, 40411, 0, 0, 0, 0),
(255151, 5, 858, 3, 3600, 0, 0),
(255151, 6, 929, 3, 3600, 0, 0),
(255151, 7, 3385, 3, 3600, 0, 0),
(255151, 8, 3827, 3, 3600, 0, 0),
(255151, 9, 3388, 2, 3600, 0, 0);

DELETE FROM `smart_scripts` WHERE `entryorguid`=255339 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`,
  `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`,
  `event_param6`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`,
  `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`,
  `target_y`, `target_z`, `target_o`, `comment`) VALUES
(255339, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 0, 42, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Militia Recruit - On Reset - Set Invincibility Hp Level 1'),
(255339, 0, 1, 2, 2, 0, 100, 0, 0, 1, 1000, 1000, 0, 0, 33, 255339, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Militia Recruit - Between 0-1% Health - Quest Credit ''Militia Training'''),
(255339, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Militia Recruit - On Link - Evade'),
(255339, 0, 3, 4, 62, 0, 100, 0, 58274, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'Militia Recruit - On Gossip Option 0 Selected - Close Gossip'),
(255339, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 0, 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Militia Recruit - On Link - Remove Npc Flag Gossip'),
(255339, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 0, 2, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Militia Recruit - On Link - Set Faction 7'),
(255339, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'Militia Recruit - On Link - Start Attacking'),
(255339, 0, 7, 8, 7, 0, 100, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Militia Recruit - On Evade - Restore Faction'),
(255339, 0, 8, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 0, 82, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Militia Recruit - On Link - Add Npc Flag Gossip');

-- Path to Ascension: Westfall (100466): no capture names its ender; Dungar Longdrink grants the credit
-- on greeting and takes the quest, as an estimate.
DELETE FROM `smart_scripts` WHERE `entryorguid`=352 AND `source_type`=0 AND `id`=3;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`,
  `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`,
  `event_param6`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`,
  `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`,
  `target_y`, `target_z`, `target_o`, `comment`) VALUES
(352, 0, 3, 0, 64, 0, 100, 0, 0, 0, 0, 0, 0, 0, 33, 100467, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'Dungar Longdrink - On Gossip Hello - Quest Credit ''Path to Ascension: Westfall''');

DELETE FROM `creature_text` WHERE `CreatureID`=991515;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`,
  `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(991515, 0, 0, 'HEY! Get away from my stuff!!!', 12, 0, 100, 0, 0, 0, 0, 0, 'Lenore the Hoarder - Aggro');

-- Lenore the Hoarder spells: MobSpells capture (cachedata MobSpells.lua); timers are estimated.
DELETE FROM `smart_scripts` WHERE `entryorguid`=991515 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`,
  `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`,
  `event_param6`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`,
  `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`,
  `target_y`, `target_z`, `target_o`, `comment`) VALUES
(991515, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Lenore the Hoarder - On Aggro - Say Line 0'),
(991515, 0, 1, 0, 0, 0, 100, 0, 5000, 8000, 15000, 20000, 0, 0, 11, 12024, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 'Lenore the Hoarder - In Combat - Cast ''Net'''),
(991515, 0, 2, 0, 0, 0, 100, 0, 3000, 6000, 7000, 10000, 0, 0, 11, 992957, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 'Lenore the Hoarder - In Combat - Cast ''Sinister Strike''');

-- Protector Brenolt and Captain Olens talk as seen in footage; the repeat interval is estimated.
DELETE FROM `creature_text` WHERE `CreatureID` IN (776787,776786);
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`,
  `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(776787, 0, 0, 'Recruitment rates are high, the promise of revenge against the Defias and a hot meal are appealing to many.', 12, 0, 100, 1, 0, 0, 0, 0, 'Protector Brenolt - Militia talk 1'),
(776786, 0, 0, 'An army of farmers against an army of thieves. They may have the heart, but not the skill. We need more time.', 12, 0, 100, 1, 0, 0, 0, 0, 'Captain Olens - Militia talk 2'),
(776787, 1, 0, 'Any news about the reinforcements from Stormwind, sir?', 12, 0, 100, 1, 0, 0, 0, 0, 'Protector Brenolt - Militia talk 3'),
(776786, 1, 0, 'You know they''re not coming, Brenolt. We''re on our own out here.', 12, 0, 100, 1, 0, 0, 0, 0, 'Captain Olens - Militia talk 4');
DELETE FROM `smart_scripts` WHERE (`entryorguid`=776787 AND `source_type`=0) OR (`entryorguid`=77678700 AND `source_type`=9);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`,
  `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`,
  `event_param6`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`,
  `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`,
  `target_y`, `target_z`, `target_o`, `comment`) VALUES
(776787, 0, 0, 0, 1, 0, 100, 0, 30000, 60000, 240000, 300000, 0, 0, 80, 77678700, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Protector Brenolt - Out of Combat - Run Militia talk'),
(77678700, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Protector Brenolt - Militia talk - Protector Brenolt Say Line 0'),
(77678700, 9, 1, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 776786, 15, 0, 0, 0, 0, 0, 0, 'Protector Brenolt - Militia talk - Captain Olens Say Line 0'),
(77678700, 9, 2, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Protector Brenolt - Militia talk - Protector Brenolt Say Line 1'),
(77678700, 9, 3, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 19, 776786, 15, 0, 0, 0, 0, 0, 0, 'Protector Brenolt - Militia talk - Captain Olens Say Line 1');

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

-- Militia Recruit greetings: npc_text 58274, Ascension npccache.wdb capture of 2026-09-05. The sparring
-- option text and the recruits' wander distance are estimated.
DELETE FROM `npc_text` WHERE `ID`=58274;
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
(58274, 'Why am I wasting this time ''training''? I could be out there taking back my farm and putting all those Defias scum to the sword!', 'Why am I wasting this time ''training''? I could be out there taking back my farm and putting all those Defias scum to the sword!', 0, 0, 0.25, 0, 0, 0, 0, 0, 0, 'I don''t know about this… I''ve been practicing for so long but I still feel like its my first day. This is hopeless…', 'I don''t know about this… I''ve been practicing for so long but I still feel like its my first day. This is hopeless…', 0, 0, 0.25, 0, 0, 0, 0, 0, 0, 'Eugh, what do you want? Can''t you see I''m busy?', 'Eugh, what do you want? Can''t you see I''m busy?', 0, 0, 0.25, 0, 0, 0, 0, 0, 0, 'Hi there! Need some practice?', 'Hi there! Need some practice?', 0, 0, 0.25, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
DELETE FROM `gossip_menu` WHERE `MenuID`=58274;
INSERT INTO `gossip_menu` (`MenuID`, `TextID`) VALUES
(58274, 58274);
DELETE FROM `gossip_menu_option` WHERE `MenuID`=58274;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`,
  `OptionType`, `OptionNpcFlag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`,
  `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(58274, 0, 0, 'Let''s practice.', 0, 1, 1, 0, 0, 0, 0, '', 0, 0);

DELETE FROM `creature` WHERE `guid` BETWEEN 9950100 AND 9950116;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `equipment_id`,
  `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`,
  `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`,
  `CreateObject`, `Comment`) VALUES
(9950100, 776786, 0, 40, 108, 1, 1, 0, -10718.9, 980.03, 36.77, 3.1, 300, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Captain Olens (client SuperTrack 21342, training yard; facing estimated)'),
(9950101, 255339, 0, 40, 108, 1, 1, 0, -10731.5, 978.0, 36.84, 2.7, 120, 3, 0, 0, 0, 1, 0, 0, 0, '', 0, 0, 'Militia Recruit (estimated position and wander distance, training yard)'),
(9950102, 255339, 0, 40, 108, 1, 1, 0, -10740.5, 971.5, 36.92, 2.71, 120, 3, 0, 0, 0, 1, 0, 0, 0, '', 0, 0, 'Militia Recruit (estimated position and wander distance, training yard)'),
(9950103, 255339, 0, 40, 108, 1, 1, 0, -10736.5, 975.5, 36.91, 3.43, 120, 3, 0, 0, 0, 1, 0, 0, 0, '', 0, 0, 'Militia Recruit (estimated position and wander distance, training yard)'),
(9950104, 255339, 0, 40, 108, 1, 1, 0, -10737.5, 989.5, 36.73, 2.46, 120, 3, 0, 0, 0, 1, 0, 0, 0, '', 0, 0, 'Militia Recruit (estimated position and wander distance, training yard)'),
(9950105, 255150, 0, 40, 108, 1, 1, 0, -10511.37, 1147.09, 40.0, 1.02, 300, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Idona Wyther (estimated position, alchemist''s farmhouse)'),
(9950106, 776790, 0, 40, 108, 1, 1, 0, -10673.5, 959.97, 38.47, 2.88, 300, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Archivist Selnor (client SuperTrack 21338, Sentinel Hill library)'),
(9950107, 991515, 0, 40, 40, 1, 1, 0, -10265.7, 1782.1, 74.83, 5.82, 600, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Lenore the Hoarder (client SuperTrack 8525, Defias tower upper floor; facing estimated)'),
(9950108, 157002, 0, 40, 921, 1, 1, 0, -11137.8, 1817.78, 38.96, 5.05, 300, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Farmer Demont (client SuperTrack 3308, Demont''s Place; facing estimated)'),
(9950109, 455339, 0, 40, 40, 1, 1, 0, -9976.29, 955.25, 31.96, 2.2, 300, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Slain Protector (client SuperTrack 21334)'),
(9950110, 455343, 0, 40, 922, 1, 1, 0, -11030.0, 790.0, 37.55, 1, 300, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Half-Devoured Protector (estimated position, Riverpaw camp)'),
(9950111, 255339, 0, 40, 108, 1, 1, 0, -10734.0, 985.0, 36.75, 3.5, 120, 3, 0, 0, 0, 1, 0, 0, 0, '', 0, 0, 'Militia Recruit (estimated position and wander distance, training yard)'),
(9950112, 255339, 0, 40, 108, 1, 1, 0, -10733.0, 994.5, 36.22, 2.74, 120, 3, 0, 0, 0, 1, 0, 0, 0, '', 0, 0, 'Militia Recruit (estimated position and wander distance, training yard)'),
(9950113, 255339, 0, 40, 108, 1, 1, 0, -10729.0, 990.0, 36.25, 2.46, 120, 3, 0, 0, 0, 1, 0, 0, 0, '', 0, 0, 'Militia Recruit (estimated position and wander distance, training yard)'),
(9950114, 255339, 0, 40, 108, 1, 1, 0, -10738.5, 981.5, 36.88, 2.78, 120, 3, 0, 0, 0, 1, 0, 0, 0, '', 0, 0, 'Militia Recruit (estimated position and wander distance, training yard)'),
(9950115, 255151, 0, 40, 108, 1, 1, 0, -10509.95, 1144.83, 40.0, 3.48, 300, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Tavin Wyther (estimated position, alchemist''s farmhouse)'),
(9950116, 776787, 0, 40, 108, 1, 1, 0, -10724.26, 988.23, 36.25, 0.77, 300, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Protector Brenolt (estimated position, training yard by Olens)');

-- Lenore the Hoarder (991515) loot: Exiles DB export 2026-09-13, creature_loot entry 991515.
DELETE FROM `creature_loot_template` WHERE `Entry`=991515;
INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`,
  `MinCount`, `MaxCount`, `Comment`) VALUES
(991515, 9771, 0, 0.012, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 14127, 0, 0.012, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 14133, 0, 0.012, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 14165, 0, 0.012, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 14179, 0, 0.012, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 15117, 0, 0.012, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 15122, 0, 0.012, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 15124, 0, 0.012, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 15329, 0, 0.012, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 15330, 0, 0.012, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 15500, 0, 0.012, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 15511, 0, 0.012, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 15512, 0, 0.012, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 15513, 0, 0.012, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 15517, 0, 0.012, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 15526, 0, 0.012, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 15972, 0, 0.012, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 554126, 0, 0.417, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 1013664, 0, 0.417, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 1013673, 0, 0.417, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 1013677, 0, 0.417, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 1013679, 0, 0.417, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 1013977, 0, 0.417, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 2089832, 0, 3.125, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 2089833, 0, 3.125, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 2089834, 0, 3.125, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 2089835, 0, 3.125, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 2089836, 0, 3.125, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 2089837, 0, 3.125, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 2089838, 0, 3.125, 0, 1, 0, 1, 1, 'Lenore the Hoarder'),
(991515, 2089839, 0, 3.125, 0, 1, 0, 1, 1, 'Lenore the Hoarder');

-- Reef Shark (12123) spawns: Exiles DB export 2026-09-13, creature_spawn guids 60841-60844.
DELETE FROM `creature` WHERE `guid` BETWEEN 9950120 AND 9950123;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `equipment_id`,
  `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`,
  `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`,
  `CreateObject`, `Comment`) VALUES
(9950120, 12123, 0, 40, 40, 1, 1, 0, -11327.4, 2292.78, -48.1677, 0, 300, 5, 0, 0, 0, 1, 0, 0, 0, '', 0, 0, 'Reef Shark (Exiles DB export 2026-09-13)'),
(9950121, 12123, 0, 40, 40, 1, 1, 0, -10638.2, 2354.37, -49.2443, 0, 300, 5, 0, 0, 0, 1, 0, 0, 0, '', 0, 0, 'Reef Shark (Exiles DB export 2026-09-13)'),
(9950122, 12123, 0, 40, 40, 1, 1, 0, -9985.74, 2374.5, -49.2355, 0, 300, 5, 0, 0, 0, 1, 0, 0, 0, '', 0, 0, 'Reef Shark (Exiles DB export 2026-09-13)'),
(9950123, 12123, 0, 40, 40, 1, 1, 0, -9783.67, 2353.35, -49.3094, 0, 300, 5, 0, 0, 0, 1, 0, 0, 0, '', 0, 0, 'Reef Shark (Exiles DB export 2026-09-13)');

-- Westfall Hoard (480104): Ascension's object id for it is unknown. Loosely Packed Dirt, Waterlogged Trunk
-- and Wanted: Lenore the Hoarder: WDB gameobject cache 2026-09-09..10.
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`,
  `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`,
  `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`,
  `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild`)
VALUES
(480101, 3, 6448, 'Pilfered Lesser Healing Potions', '', '', '', 1, 43, 480101, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0),
(480102, 3, 6448, 'Pilfered Minor Healing Potions', '', '', '', 1, 43, 480102, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0),
(480103, 3, 6448, 'Pilfered Elixirs', '', '', '', 1, 43, 480103, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0),
(480104, 3, 36, 'Westfall Hoard', '', '', '', 1, 43, 480104, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0),
(96004, 3, 20, 'Loosely Packed Dirt', '', '', '', 0.5, 43, 96004, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0),
(96005, 2, 1, 'Waterlogged Trunk', '', '', '', 1.33, 43, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0),
(518551, 2, 2491, 'Wanted: Lenore the Hoarder', '', '', '', 1, 0, 93, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0)
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

-- Estimated: the map text says the treasure map lies buried under the dirt, so it always drops.
DELETE FROM `gameobject_loot_template` WHERE `Entry`=96004;
INSERT INTO `gameobject_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`,
  `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
(96004, 157016, 0, 100, 0, 1, 0, 1, 1, 'Loosely Packed Dirt - Faded Treasure Map');

DELETE FROM `gameobject` WHERE `guid` BETWEEN 9001100 AND 9001106;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`,
  `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`,
  `animprogress`, `state`, `ScriptName`, `VerifiedBuild`, `Comment`) VALUES
(9001100, 480101, 0, 40, 919, 1, 1, -10723.1, 1391.13, 35.37, 1.2, 0, 0, 0.564642, 0.825336, 900, 100, 1, '', 0, 'Pilfered Lesser Healing Potions (client SuperTrack 21332; facing estimated)'),
(9001101, 480102, 0, 40, 111, 1, 1, -9999.35, 1469.87, 40.89, 2.5, 0, 0, 0.948985, 0.315322, 900, 100, 1, '', 0, 'Pilfered Minor Healing Potions (client SuperTrack 21330; facing estimated)'),
(9001102, 480103, 0, 40, 40, 1, 1, -9845.18, 1036.06, 33.41, 4, 0, 0, 0.909297, -0.416147, 900, 100, 1, '', 0, 'Pilfered Elixirs (client SuperTrack 21331; facing estimated)'),
(9001103, 480104, 0, 40, 40, 1, 1, -10258.8, 1767.37, 50.18, 5.82, 0, 0, 0.229528, -0.973302, 900, 100, 1, '', 0, 'Westfall Hoard (client SuperTrack 8526, Defias tower middle floor; facing estimated)'),
(9001104, 96004, 0, 40, 40, 1, 1, -10377.0, 2196.17, 20.431, 0, 0, 0, 0, 1, 900, 100, 1, '', 0, 'Loosely Packed Dirt (catalogue position, orientation unknown)'),
(9001105, 96005, 0, 40, 40, 1, 1, -9783.71, 1819.44, -8.025, 0, 0, 0, 0, 1, 900, 100, 1, '', 0, 'Waterlogged Trunk (catalogue position, orientation unknown)'),
(9001106, 518551, 0, 40, 108, 1, 1, -10646.1, 1157.59, 32.996, 0, 0, 0, 0, 1, 900, 100, 1, '', 0, 'Wanted: Lenore the Hoarder (catalogue position, orientation unknown)');

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
(999933, 2, 16, 12, 40, 0, 0, 0, 0, 0, 0, 0, 4, 0, 270, 0, 0, 0, 0, 999923, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'The Spit-Soaked Report', 'Deliver the Spit-Soaked Report to Archivist Selnor.', 'The parchment is disgusting - covered in spit and gore. The author of these papers lay in pieces all around you, unrecognizable. $B$BArchivist Selnor will want to see this report, she will know who this is.', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 999923, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 'Deliver the Spit-Soaked Report to Archivist Selnor.', '', '', '', 0),
(100466, 2, 12, 10, 40, 0, 0, 0, 0, 0, 0, 0, 4, 0, 225, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Path to Ascension: Westfall', 'Explore Stormwind to your heart''s content, then visit Flightmaster Dungar Longdrink to fly to Westfall to continue your adventure.', 'Hail, Hero.$B$BWelcome to Stormwind, capital of the Alliance. I assume you''ve arrived to rest and restock. You''ll find many things to aid you on your adventure, from personal banks and the Auction House to blacksmiths, enchanters, and of course other heroes. Explore as you like, but when you''re ready to return to your adventure, speak to Flightmaster Dungar Longdrink. There is trouble brewing in the western province that requires your aid.', '', '', 100467, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Explore Stormwind to your heart''s content, then visit Flightmaster Dungar Longdrink to fly to Westfall to continue your adventure', '', '', '', 0),
(17009, 2, 15, 13, 40, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 544, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Sunken Treasure', 'Find the Waterlogged Trunk off the coast of Westfall.', 'The map is i smudged with seaweed and dirt. Fortunately there seems to be some text scribbled towards the bottom which you can barely make out.$B$B"I can''t believe we managed to escape those horrendous fish ''things'' with our lives, but we somehow made it out with their treasure too! I''ve marked the map so as not to forget where I''ve hidden the booty. Would be a shame to come this far only to forget which ship I''ve thrown the chest in. I''ll be sure leave this map someplace safe where prying eye won''t think to look." There seems to be more scribbled down, but you can''t make it out.$B$BThere''s not much to go off of, but it would seem the treasure this map speaks of is hidden beneath the waves in one of the shipwrecks.', '', 'Find the Waterlogged Trunk off the coast of Westfall.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 0)
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

DELETE FROM `quest_template_addon` WHERE `ID` IN (1313,17011,17012,17014,26993,26994,26995,26996,26997,254042,254043,255057,999913,999914,999933,100466,17009);
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
(999933, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(100466, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(17009, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `creature_queststarter` WHERE `quest` IN (1313,17011,17012,17014,26993,26994,26995,26996,26997,254042,254043,255057,999913,999914,999933,100466,17009);
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
(455339, 999914),
(455343, 999933),
(255150, 1313),
(466, 100466);

DELETE FROM `creature_questender` WHERE `quest` IN (1313,17011,17012,17014,26993,26994,26995,26996,26997,254042,254043,255057,999913,999914,999933,100466,17009);
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
(455339, 999913),
(776790, 999914),
(776790, 999933),
(255150, 1313),
(234, 100466);

DELETE FROM `gameobject_queststarter` WHERE `quest` IN (1313,17011,17012,17014,26993,26994,26995,26996,26997,254042,254043,255057,999913,999914,999933,100466,17009);
INSERT INTO `gameobject_queststarter` (`id`, `quest`) VALUES
(518551, 254042);

DELETE FROM `gameobject_questender` WHERE `quest` IN (1313,17011,17012,17014,26993,26994,26995,26996,26997,254042,254043,255057,999913,999914,999933,100466,17009);
INSERT INTO `gameobject_questender` (`id`, `quest`) VALUES
(96005, 17009);

UPDATE `creature_template` SET `npcflag` = `npcflag` | 2 WHERE `entry` = 490;

-- Turn-in positions follow the client QuestSuperTrack/SuperTrack points: Path to Ascension: Westfall ends at
-- SuperTrack 310 (Gryan Stoutmantle), and 254042/254043 end at SuperTrack 8587/8588, where Protector Gariel
-- now stands (facing unchanged).
UPDATE `creature` SET `position_x` = -10633.9, `position_y` = 1181.08, `position_z` = 34.76 WHERE `guid` = 89531 AND `id` = 490;

UPDATE `quest_template` SET `QuestLevel` = -1 WHERE `ID` IN (22,36,38,64,65,151,153,184,208,214);
UPDATE `quest_template` SET `QuestLevel` = 15, `MinLevel` = 10 WHERE `ID` = 104;
UPDATE `quest_template` SET `MinLevel` = 35 WHERE `ID` = 50;
UPDATE `quest_template` SET `MinLevel` = 10 WHERE `ID` IN (166,214);
UPDATE `quest_template` SET `RequiredItemId1` = 1358, `RequiredItemCount1` = 1 WHERE `ID` = 138;
UPDATE `quest_template` SET `RequiredItemId1` = 1361, `RequiredItemCount1` = 1 WHERE `ID` = 139;
UPDATE `quest_template` SET `RequiredItemId1` = 1362, `RequiredItemCount1` = 1 WHERE `ID` = 140;

-- Westfall creature drops missing here: Exiles DB export 2026-09-13, creature_loot.
DELETE FROM `creature_loot_template` WHERE (`Entry`=36 AND `Item`=3595661) OR (`Entry`=121 AND `Item`=1013734) OR
  (`Entry`=122 AND `Item`=201450) OR (`Entry`=122 AND `Item`=1013661) OR (`Entry`=122 AND `Item`=1013841) OR
  (`Entry`=122 AND `Item`=1178874) OR (`Entry`=124 AND `Item`=1013642) OR (`Entry`=124 AND `Item`=1013649) OR
  (`Entry`=154 AND `Item`=79348) OR (`Entry`=157 AND `Item`=79349) OR (`Entry`=199 AND `Item`=79350) OR
  (`Entry`=391 AND `Item`=1180) OR (`Entry`=449 AND `Item`=1013734) OR (`Entry`=454 AND `Item`=79359) OR
  (`Entry`=456 AND `Item`=955) OR (`Entry`=462 AND `Item`=79360) OR (`Entry`=462 AND `Item`=103729) OR
  (`Entry`=501 AND `Item`=765) OR (`Entry`=501 AND `Item`=785) OR (`Entry`=501 AND `Item`=2447) OR
  (`Entry`=501 AND `Item`=2449) OR (`Entry`=501 AND `Item`=2450) OR (`Entry`=501 AND `Item`=2452) OR
  (`Entry`=506 AND `Item`=56925) OR (`Entry`=513 AND `Item`=835) OR (`Entry`=519 AND `Item`=56927) OR
  (`Entry`=547 AND `Item`=79367) OR (`Entry`=572 AND `Item`=56931) OR (`Entry`=573 AND `Item`=56932) OR
  (`Entry`=573 AND `Item`=450560) OR (`Entry`=589 AND `Item`=1175834) OR (`Entry`=589 AND `Item`=1178864) OR
  (`Entry`=589 AND `Item`=2005012) OR (`Entry`=830 AND `Item`=79393) OR (`Entry`=831 AND `Item`=79394) OR
  (`Entry`=833 AND `Item`=79395) OR (`Entry`=834 AND `Item`=79396) OR (`Entry`=1109 AND `Item`=79424) OR
  (`Entry`=1150 AND `Item`=79439) OR (`Entry`=1151 AND `Item`=79440) OR (`Entry`=1216 AND `Item`=79455);
INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`,
  `MinCount`, `MaxCount`, `Comment`) VALUES
(36, 3595661, 0, 2, 0, 1, 0, 1, 1, 'Harvest Golem - Handcrafted Crossbow'),
(121, 1013734, 0, 1.25, 0, 1, 0, 1, 1, 'Defias Pathstalker - Mystic Scroll: Impale'),
(122, 201450, 0, 3.75, 0, 1, 0, 1, 1, 'Defias Highwayman - Mystic Scroll: Bleeding Edge'),
(122, 1013661, 0, 0.625, 0, 1, 0, 1, 1, 'Defias Highwayman - Mystic Scroll: Dual Wield Specialization'),
(122, 1013841, 0, 0.625, 0, 1, 0, 1, 1, 'Defias Highwayman - Mystic Scroll: Dual Wield Specialization'),
(122, 1178874, 0, 5.5, 0, 1, 0, 1, 1, 'Defias Highwayman - Mystic Scroll: Retaliatory Justice'),
(124, 1013642, 0, 1.25, 0, 1, 0, 1, 1, 'Riverpaw Brute - Mystic Scroll: Booming Voice'),
(124, 1013649, 0, 1.25, 0, 1, 0, 1, 1, 'Riverpaw Brute - Mystic Scroll: Improved Demoralizing Shout'),
(154, 79348, 0, 1, 0, 1, 0, 1, 1, 'Greater Fleshripper - Beastmaster''s Whistle: Greater Fleshripper'),
(157, 79349, 0, 1, 0, 1, 0, 1, 1, 'Goretusk - Beastmaster''s Whistle: Goretusk'),
(199, 79350, 0, 1, 0, 1, 0, 1, 1, 'Young Fleshripper - Beastmaster''s Whistle: Young Fleshripper'),
(391, 1180, 0, 0.56, 0, 1, 0, 1, 1, 'Old Murk-Eye - Scroll of Stamina'),
(449, 1013734, 0, 2.5, 0, 1, 0, 1, 1, 'Defias Knuckleduster - Mystic Scroll: Impale'),
(454, 79359, 0, 1, 0, 1, 0, 1, 1, 'Young Goretusk - Beastmaster''s Whistle: Young Goretusk'),
(456, 955, 0, 0.5, 0, 1, 0, 1, 1, 'Murloc Minor Oracle - Scroll of Intellect'),
(462, 79360, 0, 1, 0, 1, 0, 1, 1, 'Vultros - Beastmaster''s Whistle: Vultros'),
(462, 103729, 0, 1, 0, 1, 0, 1, 1, 'Vultros - Sigil of Vultros'),
(501, 765, 0, 3.22, 0, 1, 0, 1, 1, 'Riverpaw Herbalist - Silverleaf'),
(501, 785, 0, 3.54, 0, 1, 0, 1, 1, 'Riverpaw Herbalist - Mageroyal'),
(501, 2447, 0, 3.38, 0, 1, 0, 1, 1, 'Riverpaw Herbalist - Peacebloom'),
(501, 2449, 0, 3.34, 0, 1, 0, 1, 1, 'Riverpaw Herbalist - Earthroot'),
(501, 2450, 0, 3.44, 0, 1, 0, 1, 1, 'Riverpaw Herbalist - Briarthorn'),
(501, 2452, 0, 1.6121, 0, 1, 0, 1, 1, 'Riverpaw Herbalist - Swiftthistle'),
(506, 56925, 0, 1, 0, 1, 0, 1, 1, 'Sergeant Brashclaw - Sigil of Sergeant Brashclaw'),
(513, 835, 0, 4.503, 0, 1, 0, 1, 1, 'Murloc Netter - Large Rope Net'),
(519, 56927, 0, 1, 0, 1, 0, 1, 1, 'Slark - Sigil of Slark'),
(547, 79367, 0, 1, 0, 1, 0, 1, 1, 'Great Goretusk - Beastmaster''s Whistle: Great Goretusk'),
(572, 56931, 0, 1, 0, 1, 0, 1, 1, 'Leprithus - Sigil of Leprithus'),
(573, 56932, 0, 1, 0, 1, 0, 1, 1, 'Foe Reaper 4000 - Sigil of Foe Reaper 4000'),
(573, 450560, 0, 100, 0, 1, 0, 1, 1, 'Foe Reaper 4000 - Harvest Golem Scythe'),
(589, 1175834, 0, 5.5, 0, 1, 0, 1, 1, 'Defias Pillager - Mystic Scroll: Blazing Speed'),
(589, 1178864, 0, 5.5, 0, 1, 0, 1, 1, 'Defias Pillager - Mystic Scroll: Flagellation'),
(589, 2005012, 0, 4, 0, 1, 0, 1, 1, 'Defias Pillager - Magic Stick'),
(830, 79393, 0, 1, 0, 1, 0, 1, 1, 'Sand Crawler - Beastmaster''s Whistle: Sand Crawler'),
(831, 79394, 0, 1, 0, 1, 0, 1, 1, 'Sea Crawler - Beastmaster''s Whistle: Sea Crawler'),
(833, 79395, 0, 1, 0, 1, 0, 1, 1, 'Coyote Packleader - Beastmaster''s Whistle: Coyote Packleader'),
(834, 79396, 0, 1, 0, 1, 0, 1, 1, 'Coyote - Beastmaster''s Whistle: Coyote'),
(1109, 79424, 0, 1, 0, 1, 0, 1, 1, 'Fleshripper - Beastmaster''s Whistle: Fleshripper'),
(1150, 79439, 0, 1, 0, 1, 0, 1, 1, 'River Crocolisk - Beastmaster''s Whistle: River Crocolisk'),
(1151, 79440, 0, 1, 0, 1, 0, 1, 1, 'Saltwater Crocolisk - Beastmaster''s Whistle: Saltwater Crocolisk'),
(1216, 79455, 0, 1, 0, 1, 0, 1, 1, 'Shore Crawler - Beastmaster''s Whistle: Shore Crawler');

UPDATE `creature_loot_template` SET `Chance` = 28.0247 WHERE `Entry` = 480 AND `Item` = 732;
UPDATE `creature_loot_template` SET `Chance` = 30 WHERE `Entry` = 573 AND `Item` = 732;

-- Westfall vendor goods missing here: Exiles DB export 2026-09-13, npc_vendor.
DELETE FROM `npc_vendor` WHERE (`entry`=491 AND `item` IN (417, 466, 469, 471, 473, 474, 475, 476, 477)) OR
  (`entry`=491 AND `item` IN (480, 352621, 421276, 3595665, 3595667)) OR
  (`entry`=843 AND `item` IN (2692, 3713, 6954)) OR (`entry`=8934 AND `item` IN (2692, 3713, 6954)) OR
  (`entry`=1668 AND `item` IN (3595665)) OR (`entry`=1670 AND `item` IN (772065, 772074)) OR
  (`entry`=8931 AND `item` IN (6948, 772061, 772062, 772063, 772064, 772065, 772066, 772067, 772068)) OR
  (`entry`=8931 AND `item` IN (772069, 772070, 772071, 772072, 772073, 772074, 772075, 772076));
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `VerifiedBuild`) VALUES
(491, 0, 417, 0, 0, 0, 0),
(491, 0, 466, 0, 0, 0, 0),
(491, 0, 469, 0, 0, 0, 0),
(491, 0, 471, 0, 0, 0, 0),
(491, 0, 473, 0, 0, 0, 0),
(491, 0, 474, 0, 0, 0, 0),
(491, 0, 475, 0, 0, 0, 0),
(491, 0, 476, 0, 0, 0, 0),
(491, 0, 477, 0, 0, 0, 0),
(491, 0, 480, 0, 0, 0, 0),
(491, 0, 352621, 0, 0, 0, 0),
(491, 0, 421276, 0, 0, 0, 0),
(491, 0, 3595665, 0, 0, 0, 0),
(491, 0, 3595667, 0, 0, 0, 0),
(843, 0, 2692, 0, 0, 0, 0),
(843, 0, 3713, 0, 0, 0, 0),
(843, 0, 6954, 0, 0, 0, 0),
(8934, 0, 2692, 0, 0, 0, 0),
(8934, 0, 3713, 0, 0, 0, 0),
(8934, 0, 6954, 0, 0, 0, 0),
(1668, 0, 3595665, 0, 0, 0, 0),
(1670, 0, 772065, 0, 0, 0, 0),
(1670, 0, 772074, 0, 0, 0, 0),
(8931, 0, 6948, 0, 0, 0, 0),
(8931, 0, 772061, 0, 0, 0, 0),
(8931, 0, 772062, 0, 0, 0, 0),
(8931, 0, 772063, 0, 0, 0, 0),
(8931, 0, 772064, 0, 0, 0, 0),
(8931, 0, 772065, 0, 0, 0, 0),
(8931, 0, 772066, 0, 0, 0, 0),
(8931, 0, 772067, 0, 0, 0, 0),
(8931, 0, 772068, 0, 0, 0, 0),
(8931, 0, 772069, 0, 0, 0, 0),
(8931, 0, 772070, 0, 0, 0, 0),
(8931, 0, 772071, 0, 0, 0, 0),
(8931, 0, 772072, 0, 0, 0, 0),
(8931, 0, 772073, 0, 0, 0, 0),
(8931, 0, 772074, 0, 0, 0, 0),
(8931, 0, 772075, 0, 0, 0, 0),
(8931, 0, 772076, 0, 0, 0, 0);

-- Westfall reputation and template values: Exiles DB export 2026-09-13.
UPDATE `creature_onkill_reputation` SET `RewOnKillRepValue1` = 25 WHERE `creature_id` IN (1094,1096,1097);
UPDATE `creature_template` SET `minlevel` = 15 WHERE `entry` = 121;
UPDATE `creature_template` SET `minlevel` = 13 WHERE `entry` IN (123,456);
UPDATE `creature_template` SET `maxlevel` = 60 WHERE `entry` = 25962;
UPDATE `creature_template` SET `mingold` = 0, `maxgold` = 0 WHERE `entry` = 7050;
