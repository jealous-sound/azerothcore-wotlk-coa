-- Redridge Mountains restore from Ascension captures: six missing Lakeshire quests (WDB questcache 2026-09-07..08),
-- their givers and objects, plus field differences in existing Redridge data.
-- Positions follow the client QuestSuperTrack/SuperTrack points; displays marked stand-in replace Ascension models
-- the client lacks, and NPC levels are estimated (the captures carry none).

INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`,
  `KillCredit1`, `KillCredit2`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`,
  `faction`, `npcflag`, `speed_walk`, `speed_run`, `speed_swim`, `speed_flight`, `detection_range`, `rank`,
  `dmgschool`, `DamageModifier`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`,
  `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `type`, `type_flags`, `lootid`, `pickpocketloot`,
  `skinloot`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`,
  `HealthModifier`, `ManaModifier`, `ArmorModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`,
  `RegenHealth`, `CreatureImmunitiesId`, `flags_extra`, `ScriptName`, `VerifiedBuild`)
VALUES
(255204, 0, 0, 0, 0, 0, 'Luca Tyndall', NULL, NULL, 0, 20, 20, 0, 12, 2, 1, 1.14286, 1, 1, 18, 0, 0, 1, 2000, 2000, 1, 1, 1, 512, 2048, 0, 0, 7, 134217728, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1.312, 1, 1, 1, 0, 0, 1, 0, 2, '', 0),
(255206, 0, 0, 0, 0, 0, 'Lady Idelia Solomon', NULL, NULL, 0, 30, 30, 0, 12, 2, 1, 1.14286, 1, 1, 18, 0, 0, 1, 2000, 2000, 1, 1, 1, 512, 2048, 0, 0, 7, 134217728, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1.312, 1, 1, 1, 0, 0, 1, 0, 2, '', 0),
(255207, 0, 0, 0, 0, 0, 'Father Norice', NULL, NULL, 0, 25, 25, 0, 12, 2, 1, 1.14286, 1, 1, 18, 0, 0, 1, 2000, 2000, 1, 1, 1, 512, 2048, 0, 0, 7, 134217728, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1.312, 1, 1, 1, 0, 0, 1, 0, 2, '', 0),
(991477, 0, 0, 0, 0, 0, 'Ley-Tracker Hestlor', 'Master Mage', NULL, 0, 30, 30, 0, 12, 2, 1, 1.14286, 1, 1, 18, 0, 0, 1, 2000, 2000, 1, 1, 1, 512, 2048, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 2, '', 0)
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

-- Stand-in displays: Ascension displays 255204, 255206, 255207 and 811477 are not in the client's
-- CreatureDisplayInfo.dbc, so Luca Tyndall wears 1541, Lady Idelia Solomon 15947, Father Norice 3283 and
-- Ley-Tracker Hestlor 1484.
DELETE FROM `creature_template_model` WHERE `CreatureID` IN (255204,255206,255207,991477);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`,
  `VerifiedBuild`) VALUES
(255204, 0, 1541, 1, 1, 0),
(255206, 0, 15947, 1, 1, 0),
(255207, 0, 3283, 1, 1, 0),
(991477, 0, 1484, 1, 1, 0);

-- Heights are the client SuperTrack points, which stand on the floors of the Lakeshire buildings.
DELETE FROM `creature` WHERE `guid` BETWEEN 9950400 AND 9950403;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `equipment_id`,
  `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`,
  `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`,
  `CreateObject`, `Comment`) VALUES
(9950400, 255206, 0, 0, 0, 1, 1, 0, -9453.16, -2314.81, 83.44, 1.0, 300, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Lady Idelia Solomon (client SuperTrack 21385/21397; facing estimated)'),
(9950401, 991477, 0, 0, 0, 1, 1, 0, -9656.0, -1970.4, 99.49, 5.5, 300, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Ley-Tracker Hestlor (client SuperTrack 21386, Red Mage Tower; facing estimated)'),
(9950402, 255204, 0, 0, 0, 1, 1, 0, -9466.77, -2186.57, 70.2, 0.3, 300, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Luca Tyndall (client SuperTrack 21392; facing estimated)'),
(9950403, 255207, 0, 0, 0, 1, 1, 0, -9428.72, -2075.62, 68.25, 4.7, 300, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 0, 'Father Norice (client SuperTrack 21396, chapel; facing estimated)');

-- Gnoll Food Barrel, Chalice of Tyrenel and Alther's Mill Plank: WDB gameobject cache 2026-09-04..08.
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`,
  `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`,
  `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`,
  `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild`)
VALUES
(255062, 3, 32, 'Gnoll Food Barrel', '', 'Collecting', '', 1, 43, 255062, 0, 1, 0, 0, 0, 0, 255113, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0),
(255063, 3, 185436, 'Chalice of Tyrenel', '', 'Collecting', '', 1, 43, 255063, 0, 1, 0, 0, 0, 0, 255114, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0),
(255064, 3, 4651, 'Alther''s Mill Plank', '', 'Collecting', '', 0.3, 43, 255064, 0, 1, 0, 0, 0, 0, 255117, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0)
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

DELETE FROM `gameobject_template_addon` WHERE `entry` BETWEEN 255062 AND 255064;
INSERT INTO `gameobject_template_addon` (`entry`, `faction`, `flags`, `mingold`, `maxgold`, `artkit0`, `artkit1`,
  `artkit2`, `artkit3`) VALUES
(255062, 0, 4, 0, 0, 0, 0, 0, 0),
(255063, 0, 4, 0, 0, 0, 0, 0, 0),
(255064, 0, 4, 0, 0, 0, 0, 0, 0);

DELETE FROM `gameobject_loot_template` WHERE `Entry` BETWEEN 255062 AND 255064;
INSERT INTO `gameobject_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`,
  `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
(255062, 355199, 0, 100, 1, 1, 0, 1, 1, 'Gnoll Food Barrel - Gnoll Food Supplies'),
(255063, 355200, 0, 100, 1, 1, 0, 1, 1, 'Chalice of Tyrenel - Chalice of Tyrenel'),
(255064, 355201, 0, 100, 1, 1, 0, 1, 1, 'Alther''s Mill Plank - Wood Planks');

-- Object spawns at the quests' client SuperTrack objective points; the respawn time is estimated.
DELETE FROM `gameobject` WHERE `guid` BETWEEN 9001300 AND 9001317;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`,
  `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`,
  `animprogress`, `state`, `ScriptName`, `VerifiedBuild`, `Comment`) VALUES
(9001300, 255062, 0, 0, 0, 1, 1, -9792.87, -2206.67, 58.61, 1.2, 0, 0, 0.564642, 0.825336, 120, 100, 1, '', 0, 'Gnoll Food Barrel (client SuperTrack 21387, gnoll camp; second barrel and facings estimated)'),
(9001301, 255062, 0, 0, 0, 1, 1, -9789.37, -2209.17, 58.61, 4.1, 0, 0, 0.887362, -0.461073, 120, 100, 1, '', 0, 'Gnoll Food Barrel (client SuperTrack 21387, gnoll camp; second barrel and facings estimated)'),
(9001302, 255062, 0, 0, 0, 1, 1, -9601.91, -2512.04, 59.25, 1.2, 0, 0, 0.564642, 0.825336, 120, 100, 1, '', 0, 'Gnoll Food Barrel (client SuperTrack 21388, gnoll camp; second barrel and facings estimated)'),
(9001303, 255062, 0, 0, 0, 1, 1, -9598.41, -2514.54, 59.65, 4.1, 0, 0, 0.887362, -0.461073, 120, 100, 1, '', 0, 'Gnoll Food Barrel (client SuperTrack 21388, gnoll camp; second barrel and facings estimated)'),
(9001304, 255062, 0, 0, 0, 1, 1, -9037.15, -2411.9, 129.14, 1.2, 0, 0, 0.564642, 0.825336, 120, 100, 1, '', 0, 'Gnoll Food Barrel (client SuperTrack 21389, gnoll camp; second barrel and facings estimated)'),
(9001305, 255062, 0, 0, 0, 1, 1, -9033.65, -2414.4, 129.43, 4.1, 0, 0, 0.887362, -0.461073, 120, 100, 1, '', 0, 'Gnoll Food Barrel (client SuperTrack 21389, gnoll camp; second barrel and facings estimated)'),
(9001306, 255062, 0, 0, 0, 1, 1, -8941.3, -2314.05, 132.45, 1.2, 0, 0, 0.564642, 0.825336, 120, 100, 1, '', 0, 'Gnoll Food Barrel (client SuperTrack 21390, gnoll camp; second barrel and facings estimated)'),
(9001307, 255062, 0, 0, 0, 1, 1, -8937.8, -2316.55, 132.47, 4.1, 0, 0, 0.887362, -0.461073, 120, 100, 1, '', 0, 'Gnoll Food Barrel (client SuperTrack 21390, gnoll camp; second barrel and facings estimated)'),
(9001308, 255062, 0, 0, 0, 1, 1, -8964.85, -2081.38, 132.44, 1.2, 0, 0, 0.564642, 0.825336, 120, 100, 1, '', 0, 'Gnoll Food Barrel (client SuperTrack 21391, gnoll camp; second barrel and facings estimated)'),
(9001309, 255062, 0, 0, 0, 1, 1, -8961.35, -2083.88, 132.44, 4.1, 0, 0, 0.887362, -0.461073, 120, 100, 1, '', 0, 'Gnoll Food Barrel (client SuperTrack 21391, gnoll camp; second barrel and facings estimated)'),
(9001310, 255063, 0, 0, 0, 1, 1, -9010.18, -3221.56, 109.31, 2.3, 0, 0, 0.912764, 0.408487, 120, 100, 1, '', 0, 'Chalice of Tyrenel (client SuperTrack 21395; facing estimated)'),
(9001311, 255064, 0, 0, 0, 1, 1, -9227.62, -2701.3, 88.8, 0.4, 0, 0, 0.198669, 0.980067, 120, 100, 1, '', 0, 'Alther''s Mill Plank (client SuperTrack 21393, Alther''s Mill; spread and facings estimated)'),
(9001312, 255064, 0, 0, 0, 1, 1, -9223.62, -2698.3, 88.8, 2.0, 0, 0, 0.841471, 0.540302, 120, 100, 1, '', 0, 'Alther''s Mill Plank (client SuperTrack 21393, Alther''s Mill; spread and facings estimated)'),
(9001313, 255064, 0, 0, 0, 1, 1, -9231.12, -2697.3, 88.8, 3.3, 0, 0, 0.996865, -0.079121, 120, 100, 1, '', 0, 'Alther''s Mill Plank (client SuperTrack 21393, Alther''s Mill; spread and facings estimated)'),
(9001314, 255064, 0, 0, 0, 1, 1, -9232.62, -2703.8, 88.8, 5.1, 0, 0, 0.557684, -0.830054, 120, 100, 1, '', 0, 'Alther''s Mill Plank (client SuperTrack 21393, Alther''s Mill; spread and facings estimated)'),
(9001315, 255064, 0, 0, 0, 1, 1, -9225.12, -2706.3, 88.8, 1.1, 0, 0, 0.522687, 0.852525, 120, 100, 1, '', 0, 'Alther''s Mill Plank (client SuperTrack 21393, Alther''s Mill; spread and facings estimated)'),
(9001316, 255064, 0, 0, 0, 1, 1, -9220.62, -2702.3, 88.8, 4.4, 0, 0, 0.808496, -0.588501, 120, 100, 1, '', 0, 'Alther''s Mill Plank (client SuperTrack 21393, Alther''s Mill; spread and facings estimated)'),
(9001317, 255064, 0, 0, 0, 1, 1, -9228.62, -2693.3, 88.8, 0.9, 0, 0, 0.434966, 0.900447, 120, 100, 1, '', 0, 'Alther''s Mill Plank (client SuperTrack 21393, Alther''s Mill; spread and facings estimated)');

-- 'Pefectly Good Planks' keeps the title as captured.
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
(255111, 2, 16, 11, 44, 0, 0, 0, 0, 0, 0, 255112, 1, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Alternative Allies', 'Meet with Ley-Tracker Hestlor at the Red Mage Tower on behalf of Lady Idelia Solomon.', 'If Stormwind won''t send more aid then fine, there''s other places we can turn.    The mages of the Red Mage Tower have stayed out of the conflict, but one of their archmages protected Lakeshire during the dragon attack a while back. Light knows we could use Minervia right now...    Well, she disappeared soon after that, but it''ll be their problem next if Lakeshire falls, so maybe some others will be sympathetic to us. I don''t suppose you''d be willing to travel up there and talk to them? I don''t think they''d be all that welcoming to me.', '', 'Speak with Ley-Tracker Hestlor in the Red Mage Tower.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 0),
(255112, 2, 16, 11, 44, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Old Mistakes', 'Return to Lady Idelia Solomon with news of Hestlor''s response.', 'Still, I suppose the request is for all of Lakeshire, not just herself...    Most of us aren''t specialized in combat. If the orcs did get up here, we''d escape through Dorian and Silas''s portal and close it behind us. Alex is the most likely to help, but she''s only a novice.    <Hestlor sighs.>    I think Minervia gave the town a skewed perception of us. If the soldiers bring their equipment up here, we''ll Veridan can enchant it all, and my own tracking will detect if the orcs are in the process of any particularly dangerous rituals. That''s the best we can offer.', '', 'Return to Lady Idelia Solomon in Lakeshire.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 0),
(255113, 2, 17, 11, 44, 0, 0, 0, 0, 0, 0, 0, 5, 700, 427, 0, 0, 0, 0, 0, 8, 0, 375250, 95, 1397884, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Food Fit for Gnolls', 'Collect Gnoll Food Supplies from gnoll camps around Redridge for Luca Tyndall.', 'It''s too dangerous to hunt these days, and the shipments of grain from Westfall stopped. We''ve always eaten a lot of fish in Lakeshire, but there aren''t enough fish in the lake to live on just that.    I can''t cook for my family without food to cook, but I do have an idea you might be able to help with. The local gnolls have only gotten bolder, I can''t tell if they''re allied with the orcs or just don''t care. Either way, they''ve been hunting plenty. I''ll bet they''ve even got some to spare, if you know what I mean.', '', 'Return to Luca Tyndall in Lakeshire.', 0, 0, 0, 0, 0, 0, 0, 0, 355199, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, '', '', '', '', 0),
(255114, 2, 25, 18, 44, 0, 2, 0, 0, 0, 0, 0, 5, 700, 427, 0, 0, 0, 0, 0, 8, 0, 375250, 95, 1397884, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Blessings of the Light', 'Recover the Chalice of Tyrenel for Father Norice.', 'May the Light bless you, good traveler!    If you''re venturing into the wilds, perhaps you could do a small favor in the Light''s name?    We once had an old relic here at Lakeshire, the Chalice of Tyrenel. A beautiful thing, my descriptions could hardly do it justice. It was taken by the Shadowhide gnoll clan a while back, and we have not the means of recovering it.    But if you, perhaps, stumble upon it in your journey, I implore you to bring it back to my chapel.', '', 'Return to Father Norice in Lakeshire.', 0, 0, 0, 0, 0, 0, 0, 0, 355200, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, '', '', '', '', 0),
(255116, 2, 19, 13, 44, 0, 0, 0, 0, 0, 0, 0, 5, 700, 427, 0, 0, 0, 0, 0, 8, 0, 375250, 95, 1397884, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Reclaiming Alther''s Mill', 'Slay Greater Tarantulas around Alther''s Mill to help the people of Lakeshire reclaim the area.', 'Without many shipments coming in, we''re low on just about every form of supplies we could use. There isn''t an immediate solution to many of them, but Lakeshire has always had a good lumber mill until Stonewatch fell. It''s dangerously close to the Blackrocks, but they aren''t actually holding it down either.    Reports say Alther''s Mill is infested with spiders. Think you could clear them out?', '', 'Return to Lady Idelia Solomon in Lakeshire.', 505, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', '', 0),
(255117, 2, 19, 13, 44, 0, 0, 0, 0, 0, 0, 0, 5, 700, 427, 0, 0, 0, 0, 0, 8, 0, 375250, 95, 1397884, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Pefectly Good Planks', 'Collect Wood Planks from Alther''s Mill for Foreman Oslow.', 'We''ve got some wood to spare for now, but between the dock repairs and the bridge, I don''t see our supply lasting long.    I hear there''s been talk about reclaiming Alther''s Mill, but that doesn''t help me very much right now. If you ask me, that idea''s insane anyways, it''s too close to the orcs'' stronghold.    Still, it was evacuated in a hurry. I''ll bet there''s plenty of good planks just lying around there. I''ve got good gold if you bring me some, more than you''ll get selling it off in Stormwind.', '', 'Return to Foreman Oslow in Lakeshire.', 0, 0, 0, 0, 0, 0, 0, 0, 355201, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, '', '', '', '', 0)
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

DELETE FROM `quest_template_addon` WHERE `ID` IN (255111,255112,255113,255114,255116,255117);
INSERT INTO `quest_template_addon` (`ID`, `MaxLevel`, `AllowableClasses`, `SourceSpellID`, `PrevQuestID`,
  `NextQuestID`, `ExclusiveGroup`, `BreadcrumbForQuestId`, `RewardMailTemplateID`, `RewardMailDelay`,
  `RequiredSkillID`, `RequiredSkillPoints`, `RequiredMinRepFaction`, `RequiredMaxRepFaction`, `RequiredMinRepValue`,
  `RequiredMaxRepValue`, `ProvidedItemCount`, `SpecialFlags`) VALUES
(255111, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(255112, 0, 0, 0, 255111, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(255113, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(255114, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(255116, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(255117, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `creature_queststarter` WHERE `quest` IN (255111,255112,255113,255114,255116,255117);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(255206, 255111),
(991477, 255112),
(255204, 255113),
(255207, 255114),
(255206, 255116),
(341, 255117);

DELETE FROM `creature_questender` WHERE `quest` IN (255111,255112,255113,255114,255116,255117);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(991477, 255111),
(255206, 255112),
(255204, 255113),
(255207, 255114),
(255206, 255116),
(341, 255117);

-- Redridge quest levels and requirements: Ascension questcache.wdb captures 2026-09-07..08.
UPDATE `quest_template` SET `QuestLevel` = -1 WHERE `ID` IN (19,20,34,89,91,92,94,115,116,118,119,120,121,122,124,126,
  127,128,129,130,131,143,150,178,180,244,246,3741);
UPDATE `quest_template` SET `MinLevel` = 10 WHERE `ID` IN (89,92,125,126,178);
UPDATE `quest_template` SET `QuestLevel` = 11 WHERE `ID` = 125;
UPDATE `quest_template` SET `SuggestedGroupNum` = 3 WHERE `ID` = 169;
UPDATE `quest_template` SET `QuestInfoID` = 1 WHERE `ID` IN (248,249);

-- Redridge creature cache values: Ascension creaturecache.wdb captures up to 2026-09-05.
UPDATE `creature_template` SET `rank` = 2 WHERE `entry` IN (61,947,1106,14269,14270,14271,14272,14273);
UPDATE `creature_template` SET `subname` = 'Mining and Smithing Supplies' WHERE `entry` = 790;
UPDATE `creature_template` SET `subname` = 'Bait and Tackle Supplier' WHERE `entry` = 1678;
UPDATE `creature_template` SET `subname` = 'Poison Supplier' WHERE `entry` = 3090;
UPDATE `creature_template` SET `subname` = 'Food and Drinks' WHERE `entry` = 5620;

-- Redridge creature levels: db.exil.es dump 2026-10-04, creature.
UPDATE `creature_template` SET `minlevel` = 23, `maxlevel` = 24 WHERE `entry` IN (579,615);
UPDATE `creature_template` SET `minlevel` = 22, `maxlevel` = 23 WHERE `entry` = 4463;

-- Lakeshire vendor goods missing here: db.exil.es dump 2026-10-04, npc_vendor.
DELETE FROM `npc_vendor` WHERE (`entry`=6727 AND `item` IN (6948, 21829, 21833, 772061, 772062, 772063, 772064, 772065, 772066, 772067, 772068, 772069, 772070, 772071, 772072, 772073, 772074, 772075, 772076)) OR
  (`entry`=5620 AND `item` IN (772061, 772062, 772063, 772064, 772065, 772066, 772067, 772068, 772069, 772070, 772071, 772072, 772073, 772074, 772075, 772076)) OR
  (`entry`=777 AND `item` IN (2692, 3713, 6954)) OR
  (`entry`=3085 AND `item` IN (2692, 3713)) OR
  (`entry`=789 AND `item` IN (421276, 3595665)) OR
  (`entry`=1678 AND `item` IN (1061923));
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `VerifiedBuild`) VALUES
(6727, 0, 6948, 0, 0, 0, 0),
(6727, 0, 21829, 0, 0, 0, 0),
(6727, 0, 21833, 0, 0, 0, 0),
(6727, 0, 772061, 0, 0, 0, 0),
(6727, 0, 772062, 0, 0, 0, 0),
(6727, 0, 772063, 0, 0, 0, 0),
(6727, 0, 772064, 0, 0, 0, 0),
(6727, 0, 772065, 0, 0, 0, 0),
(6727, 0, 772066, 0, 0, 0, 0),
(6727, 0, 772067, 0, 0, 0, 0),
(6727, 0, 772068, 0, 0, 0, 0),
(6727, 0, 772069, 0, 0, 0, 0),
(6727, 0, 772070, 0, 0, 0, 0),
(6727, 0, 772071, 0, 0, 0, 0),
(6727, 0, 772072, 0, 0, 0, 0),
(6727, 0, 772073, 0, 0, 0, 0),
(6727, 0, 772074, 0, 0, 0, 0),
(6727, 0, 772075, 0, 0, 0, 0),
(6727, 0, 772076, 0, 0, 0, 0),
(5620, 0, 772061, 0, 0, 0, 0),
(5620, 0, 772062, 0, 0, 0, 0),
(5620, 0, 772063, 0, 0, 0, 0),
(5620, 0, 772064, 0, 0, 0, 0),
(5620, 0, 772065, 0, 0, 0, 0),
(5620, 0, 772066, 0, 0, 0, 0),
(5620, 0, 772067, 0, 0, 0, 0),
(5620, 0, 772068, 0, 0, 0, 0),
(5620, 0, 772069, 0, 0, 0, 0),
(5620, 0, 772070, 0, 0, 0, 0),
(5620, 0, 772071, 0, 0, 0, 0),
(5620, 0, 772072, 0, 0, 0, 0),
(5620, 0, 772073, 0, 0, 0, 0),
(5620, 0, 772074, 0, 0, 0, 0),
(5620, 0, 772075, 0, 0, 0, 0),
(5620, 0, 772076, 0, 0, 0, 0),
(777, 0, 2692, 0, 0, 0, 0),
(777, 0, 3713, 0, 0, 0, 0),
(777, 0, 6954, 0, 0, 0, 0),
(3085, 0, 2692, 0, 0, 0, 0),
(3085, 0, 3713, 0, 0, 0, 0),
(789, 0, 421276, 0, 0, 0, 0),
(789, 0, 3595665, 0, 0, 0, 0),
(1678, 0, 1061923, 0, 0, 0, 0);
