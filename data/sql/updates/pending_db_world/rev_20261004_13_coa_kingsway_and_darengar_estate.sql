-- The Kingsway north of Stormwind and the Darengar Estate approach, from in-game captures (2026-10-04).
-- NPC ids and models come from the Exiles DB export where it has them. Guard Dornhelm, Merchant Tharwin and
-- the quests Roadside Violence, A Sword for the North and Graysky Justice are not in the export and use local ids.
INSERT INTO `creature_template` (`entry`, `name`, `subname`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `rank`, `unit_class`, `unit_flags`, `type`, `family`, `lootid`, `AIName`, `MovementType`, `HealthModifier`, `flags_extra`, `VerifiedBuild`)
VALUES
(164002, 'Captain Hedgar', '', 21, 21, 35, 2, 1, 1.14286, 1, 1, 770, 7, 0, 0, 'SmartAI', 0, 1, 2, 0),
(164020, 'Road Bandit', '', 17, 18, 14, 0, 1, 1.14286, 0, 1, 0, 7, 0, 164020, '', 0, 1, 0, 0),
(164021, 'Road Marauder', '', 17, 18, 14, 0, 1, 1.14286, 0, 1, 0, 7, 0, 164021, '', 0, 1, 0, 0),
(164027, 'Graysky Soldier', '', 19, 20, 35, 0, 1, 1.14286, 0, 1, 770, 7, 0, 0, '', 0, 1, 2, 0),
(164058, 'Graysky Soldier', '', 19, 20, 35, 0, 1, 1.14286, 0, 1, 770, 7, 0, 0, '', 0, 1, 2, 0),
(164083, 'Scadeald Fox', '', 21, 21, 7, 0, 1, 1.14286, 0, 1, 0, 1, 0, 0, '', 0, 1, 0, 0),
(164084, 'Scadeald Wolf', '', 17, 18, 32, 0, 1, 1.14286, 0, 1, 0, 1, 1, 0, '', 0, 1, 0, 0),
(164090, 'Graysky Donkey', '', 1, 1, 12, 0, 1, 1.14286, 0, 1, 768, 7, 0, 0, '', 0, 1, 2, 0),
(164210, 'Cadmilla', '', 25, 25, 12, 2, 1, 1.14286, 0, 1, 768, 7, 0, 0, 'SmartAI', 0, 1, 2, 0),
(164371, 'Raven', '', 1, 1, 188, 0, 1, 1.14286, 0, 1, 0, 8, 0, 0, '', 0, 1, 0, 0),
(164427, 'Stormwind Guard', '', 30, 30, 11, 0, 1, 1.14286, 0, 1, 768, 7, 0, 0, '', 0, 1, 2, 0),
(164428, 'Stormwind Guard', '', 30, 30, 11, 0, 1, 1.14286, 0, 1, 768, 7, 0, 0, '', 0, 1, 2, 0),
(164571, 'Scadeald Rabbit', '', 1, 1, 188, 0, 1, 1.14286, 0, 1, 0, 8, 0, 0, '', 0, 1, 0, 0),
(164573, 'Scadeald Moth', '', 1, 1, 188, 0, 1, 1.14286, 0, 1, 0, 8, 0, 0, '', 0, 1, 0, 0),
(164580, 'Squirrel', '', 1, 1, 188, 0, 1, 1.14286, 0, 1, 0, 8, 0, 0, '', 0, 1, 0, 0),
(9920040, 'Guard Dornhelm', '', 25, 25, 11, 2, 1, 1.14286, 0, 1, 768, 7, 0, 0, 'SmartAI', 0, 1, 2, 0),
(9920041, 'Merchant Tharwin', '', 18, 18, 12, 2, 1, 1.14286, 0, 1, 768, 7, 0, 0, '', 0, 1, 2, 0),
(164944, 'Possessed Crow', '', 1, 1, 188, 0, 1, 1.14286, 0, 1, 0, 8, 0, 0, '', 0, 1, 0, 0)
ON DUPLICATE KEY UPDATE `name` = VALUES(`name`), `subname` = VALUES(`subname`), `minlevel` = VALUES(`minlevel`),
  `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `npcflag` = VALUES(`npcflag`),
  `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `rank` = VALUES(`rank`),
  `unit_class` = VALUES(`unit_class`), `unit_flags` = VALUES(`unit_flags`), `type` = VALUES(`type`),
  `family` = VALUES(`family`), `lootid` = VALUES(`lootid`), `AIName` = VALUES(`AIName`),
  `MovementType` = VALUES(`MovementType`), `HealthModifier` = VALUES(`HealthModifier`), `flags_extra` = VALUES(`flags_extra`),
  `VerifiedBuild` = VALUES(`VerifiedBuild`);
DELETE FROM `creature_template_model` WHERE `CreatureID` IN (164002, 164020, 164021, 164027, 164058, 164083, 164084, 164090, 164210, 164371, 164427, 164428, 164571, 164573, 164580, 9920040, 9920041, 164944);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES
(164002, 0, 49, 1, 1, 0),
(164020, 0, 49, 1, 1, 0),
(164021, 0, 49, 1, 1, 0),
(164027, 0, 49, 1, 1, 0),
(164058, 0, 50, 1, 1, 0),
(164083, 0, 142806, 1, 1, 0),
(164084, 0, 137698, 1, 1, 0),
(164090, 0, 138808, 1, 1, 0),
(164210, 0, 50, 1, 1, 0),
(164371, 0, 32546, 1, 1, 0),
(164427, 0, 3258, 1, 1, 0),
(164428, 0, 3258, 1, 1, 0),
(164571, 0, 6302, 1, 1, 0),
(164573, 0, 36944, 1, 1, 0),
(164580, 0, 134, 1, 1, 0),
(9920040, 0, 3258, 1, 1, 0),
(9920041, 0, 5032, 1, 1, 0),
(164944, 0, 32546, 1.5, 1, 0),
(164020, 1, 50, 1, 1, 0),
(164021, 1, 50, 1, 1, 0);
DELETE FROM `creature_model_info` WHERE `DisplayID` IN (137698, 138808, 36944, 32546, 142806);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`, `DisplayID_Other_Gender`, `VerifiedBuild`) VALUES
(137698, 0.975, 0.8125, 2, 0, 0),
(138808, 0.6, 1.5, 2, 0, 0),
(36944, 0.1, 0.3, 2, 0, 0),
(32546, 0.3, 0.5, 2, 0, 0),
(142806, 0.4, 0.6, 2, 0, 0);
DELETE FROM `creature_display_preset` WHERE `entry` IN (164020, 164021);
INSERT INTO `creature_display_preset` (`entry`, `display_id`, `race`, `gender`, `class`, `skin`, `face`, `hair`, `haircolor`, `facialhair`, `guild_id`, `item_head`, `item_shoulders`, `item_body`, `item_chest`, `item_waist`, `item_legs`, `item_feet`, `item_wrists`, `item_hands`, `item_back`, `item_tabard`) VALUES
(164020, 49, 1, 0, 4, 3, 2, 4, 1, 2, 0, 34850, 22379, 0, 22360, 22377, 22361, 22362, 18909, 22366, 17880, 0),
(164020, 50, 1, 1, 4, 2, 3, 5, 1, 0, 0, 34850, 22379, 0, 22360, 22377, 22361, 22362, 18909, 22366, 17880, 0),
(164021, 49, 1, 0, 4, 5, 5, 6, 0, 4, 0, 34850, 12878, 0, 12863, 22377, 13455, 12885, 18909, 22366, 17880, 0),
(164021, 50, 1, 1, 4, 6, 4, 2, 0, 0, 0, 34850, 12878, 0, 12863, 22377, 13455, 12885, 18909, 22366, 17880, 0);
DELETE FROM `creature_equip_template` WHERE `CreatureID` IN (164020, 164021, 164027);
INSERT INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
(164020, 1, 2209, 2209, 0, 0),
(164021, 1, 2027, 2207, 0, 0),
(164027, 1, 2488, 3651, 0, 0);
DELETE FROM `creature_template_addon` WHERE `entry` IN (164020, 164021);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `visibilityDistanceType`, `auras`) VALUES
(164020, 0, 0, 0, 1, 0, 0, '28126'),
(164021, 0, 0, 0, 1, 0, 0, '28126');
DELETE FROM `creature_loot_template` WHERE `Entry` IN (164020, 164021);
INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
(164020, 559855, 0, 35, 1, 1, 0, 1, 1, 'Road Bandit - Stolen Goods'),
(164021, 559855, 0, 50, 1, 1, 0, 1, 2, 'Road Marauder - Stolen Goods');
DELETE FROM `creature_text` WHERE `CreatureID` IN (164002, 164210, 9920040);
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(164002, 1, 0, 'You! Yes, you. Get over here.', 12, 0, 100, 0, 0, 0, 0, 0, 'captured in game'),
(164002, 0, 0, 'Scour the manor from top to bottom. Rip up the floorboards if you must. Lady Serenya has placed great expectations upon you.', 12, 0, 100, 0, 0, 0, 0, 0, 'captured in game'),
(164210, 0, 0, 'You''re nothing but thugs, do you know that? And what''s worse: thieves!', 12, 0, 100, 0, 0, 0, 0, 0, 'captured in game'),
(164210, 1, 0, 'Though the entries are ambiguous, the journal mentions something about the well and a cave beneath the manor. It might be worth taking a look.', 12, 0, 100, 0, 0, 0, 0, 0, 'captured in game'),
(9920040, 0, 0, 'Proceed with caution in your journey.', 12, 0, 100, 0, 0, 0, 0, 0, 'captured in game');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 164002 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 164210 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 9920040 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `event_param6`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(164002, 0, 0, 0, 10, 0, 100, 0, 1, 15, 60000, 90000, 1, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'Captain Hedgar - Player in range - Say orders'),
(164002, 0, 1, 0, 10, 0, 100, 0, 1, 20, 30000, 30000, 1, 0, 1, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'Captain Hedgar - Player with the Ealdir plot uncovered in range - Call them over'),
(164210, 0, 0, 0, 10, 0, 100, 0, 1, 15, 60000, 90000, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Cadmilla - Player in range - Berate the soldiers'),
(164210, 0, 1, 0, 19, 0, 100, 0, 9920015, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'Cadmilla - On Notes of a Conspiracy accepted - Point to the well'),
(9920040, 0, 0, 0, 20, 0, 100, 0, 9920013, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'Guard Dornhelm - On A Sword for the North rewarded - Say farewell');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceEntry` = 164002;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 2, 164002, 0, 0, 28, 0, 9920016, 0, 0, 0, 0, 0, '', 'Hedgar calls the player over once To Believe In An Ideal is complete');
DELETE FROM `gameobject_loot_template` WHERE `Entry` IN (2300635, 2300632);
INSERT INTO `gameobject_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
(2300635, 559855, 0, 100, 1, 1, 0, 1, 2, 'Stolen Goods'),
(2300632, 559851, 0, 100, 1, 1, 0, 1, 1, 'Arathon''s Journal');
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data8`, `VerifiedBuild`)
VALUES
(2300635, 3, 31, 'Stolen Goods', 1, 1689, 2300635, 0, 1, 9920012, 0),
(2300632, 3, 1048022, 'Arathon''s Journal', 1, 1689, 2300632, 0, 1, 9920014, 0)
ON DUPLICATE KEY UPDATE `type` = VALUES(`type`), `displayId` = VALUES(`displayId`), `name` = VALUES(`name`),
  `size` = VALUES(`size`), `Data0` = VALUES(`Data0`), `Data1` = VALUES(`Data1`),
  `Data2` = VALUES(`Data2`), `Data3` = VALUES(`Data3`), `Data8` = VALUES(`Data8`),
  `VerifiedBuild` = VALUES(`VerifiedBuild`);
INSERT INTO `quest_template` (`ID`, `QuestType`, `QuestLevel`, `MinLevel`, `QuestSortID`, `RewardXPDifficulty`, `RewardMoney`, `Flags`, `LogTitle`, `LogDescription`, `QuestDescription`, `QuestCompletionLog`, `RequiredNpcOrGo1`, `RequiredNpcOrGoCount1`, `RequiredItemId1`, `RequiredItemCount1`, `VerifiedBuild`)
VALUES
(9920012, 2, -1, 17, 10302, 7, 700, 8, 'Roadside Violence', 'Recover the goods stolen from Merchant Tharwin by the bandits on the Kingsway.', '<Sweat beads on the merchant''s brow as he struggles to find the words. His wrists bear marks from being bound tightly; he has clearly been held captive.>$B$BMy wagon... my wares... my poor donkey, Tito... All ruined. What a disaster...$B$BThank the Light the guard arrived in time and managed to scare off those scoundrels before they left me in the same state as Tito.$B$B<The merchant gazes at his looted wagon, his shoulders slumped.>$B$BI won''t be able to recover from this... I invested everything I had into these goods. Please, help me! Find the hideout of those thugs and recover the items they stole from me.', 'Return to Merchant Tharwin on the Kingsway.', 0, 0, 559855, 16, 0),
(9920013, 2, -1, 17, 10302, 7, 800, 8, 'A Sword for the North', 'Defeat the bandits in Northern Elwynn hiding within the Deepstone Cave.', 'The Kingsway has never been truly safe, but with the southern provinces hogging our attention, damn those Defias, it has become more dangerous than ever.$B$BThere are far too few of us soldiers on this side of the city, and the local bandits know it well.$B$BWould you be willing to lend us a hand? Someone must hunt down the brigands who assaulted that merchant. For the good of the kingdom.', 'Return to Guard Dornhelm on the Kingsway.', 164020, 8, 0, 0, 0),
(9920014, 2, -1, 18, 10302, 7, 350, 8, 'Graysky Justice', 'Find Arathon''s Journal inside the Darengar Estate.', '<Cadmilla dabs at her eyes. A sudden determination burns within them.>$B$BYou carry Lord Servin''s seal. That means you have his trust. So, listen closely:$B$BI need you to bring me Arathon''s journal before those Graysky thugs find it. If those pages fall into Lady Serenya''s hands, she will have his death warrant signed before dawn.$B$BGo! I shall try to distract the captain at the entrance.', 'Return to Cadmilla at the Darengar Estate.', 0, 0, 559851, 1, 0),
(9920015, 2, -1, 18, 10302, 7, 350, 8, 'Notes of a Conspiracy', 'Descend into the well at the Darengar Estate and investigate what lies hidden within.', '<Cadmilla snaps the journal shut.>$B$BIt is true, then. My lord Arathon was conspiring with the Ealdir.$B$BHave you heard of them? They claim to be descendants of this land''s original inhabitants. Pagans and heretics who crave independence. Savages, in short.$B$BMy brother died years ago in an Ealdir ambush.$B$BThe journal mentions a cave beneath the manor and something about the well. I suspect we might find answers down there. Would you mind looking into it?', 'Inspect the weapon crates in the cave beneath the well.', 0, 0, 0, 0, 0)
ON DUPLICATE KEY UPDATE `QuestType` = VALUES(`QuestType`), `QuestLevel` = VALUES(`QuestLevel`), `MinLevel` = VALUES(`MinLevel`),
  `QuestSortID` = VALUES(`QuestSortID`), `RewardXPDifficulty` = VALUES(`RewardXPDifficulty`), `RewardMoney` = VALUES(`RewardMoney`),
  `Flags` = VALUES(`Flags`), `LogTitle` = VALUES(`LogTitle`), `LogDescription` = VALUES(`LogDescription`),
  `QuestDescription` = VALUES(`QuestDescription`), `QuestCompletionLog` = VALUES(`QuestCompletionLog`), `RequiredNpcOrGo1` = VALUES(`RequiredNpcOrGo1`),
  `RequiredNpcOrGoCount1` = VALUES(`RequiredNpcOrGoCount1`), `RequiredItemId1` = VALUES(`RequiredItemId1`), `RequiredItemCount1` = VALUES(`RequiredItemCount1`),
  `VerifiedBuild` = VALUES(`VerifiedBuild`);
DELETE FROM `quest_template_addon` WHERE `ID` IN (9920012, 9920013, 9920014, 9920015);
INSERT INTO `quest_template_addon` (`ID`, `PrevQuestID`, `SpecialFlags`) VALUES
(9920012, 0, 0),
(9920013, 0, 0),
(9920014, 175002, 0),
(9920015, 9920014, 0);
DELETE FROM `quest_offer_reward` WHERE `ID` IN (9920012, 9920014, 9920015);
INSERT INTO `quest_offer_reward` (`ID`, `RewardText`, `VerifiedBuild`) VALUES
(9920012, 'Oh, thank you, thank you so much! You''ve recovered more than I dared hope for; enough to save me from ruin, at least.$B$BI''ll try to repair the wagon, and as soon as Tito recovers, I''ll resume my journey toward Graysky.$B$BI don''t know how I can ever pay you back, but I am forever in your debt.', 0),
(9920014, 'This is...$B$B<As she flips through the pages of the journal, Cadmilla''s expression shifts from sorrow to confusion.>$B$BI don''t understand any of this.$B$B<Cadmilla shakes her head.>$B$BIt is Lord Arathon''s handwriting, there is no doubt of that. Oh, oh dear! How could he...?', 0),
(9920015, '<Splintered crates bearing the seal of the Alliance. It appears they were tossed from the top of the well with criminal haste. The fall shattered several, scattering Stormwind steel across the damp floor of the cave.>$B$B<This isn''t refuse. It''s an arsenal.>', 0);
DELETE FROM `quest_request_items` WHERE `ID` = 9920014;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `CompletionText`, `VerifiedBuild`) VALUES
(9920014, 0, 0, 'Arathon''s room is upstairs.', 0);
DELETE FROM `creature_queststarter` WHERE `quest` IN (9920012, 9920013, 9920014, 9920015);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(9920041, 9920012),
(9920040, 9920013),
(164210, 9920014),
(164210, 9920015);
DELETE FROM `creature_questender` WHERE `quest` IN (9920012, 9920013, 9920014, 9920015);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(9920041, 9920012),
(9920040, 9920013),
(164210, 9920014);
DELETE FROM `gameobject_questender` WHERE `quest` = 9920015;
INSERT INTO `gameobject_questender` (`id`, `quest`) VALUES
(2300631, 9920015);
DELETE FROM `creature` WHERE `guid` BETWEEN 9920300 AND 9920399;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `MovementType`, `Comment`) VALUES
(9920300, 164084, 0, 0, 0, 1, 1, -8136.1313, 749.7535, 68.12572, 4.7687836, 300, 8, 1, 'Scadeald Wolf (captured in game)'),
(9920301, 164084, 0, 0, 0, 1, 1, -8155.9443, 861.5914, 73.755806, 1.4905264, 300, 8, 1, 'Scadeald Wolf (captured in game)'),
(9920302, 164084, 0, 0, 0, 1, 1, -8151.052, 736.7633, 69.123344, 3.241223, 300, 8, 1, 'Scadeald Wolf (captured in game)'),
(9920303, 164084, 0, 0, 0, 1, 1, -7992.108, 711.46796, 84.80895, 1.4229629, 300, 8, 1, 'Scadeald Wolf (captured in game)'),
(9920304, 164084, 0, 0, 0, 1, 1, -7913.051, 721.0459, 85.78433, 3.6283572, 300, 8, 1, 'Scadeald Wolf (captured in game)'),
(9920305, 164084, 0, 0, 0, 1, 1, -7785.737, 722.6081, 97.33572, 4.171852, 300, 8, 1, 'Scadeald Wolf (captured in game)'),
(9920306, 164580, 0, 0, 0, 1, 1, -8131.3145, 731.5445, 69.68152, 5.1418476, 120, 5, 1, 'Squirrel (captured in game)'),
(9920307, 164083, 0, 0, 0, 1, 1, -7807.975, 676.1117, 98.2839, 2.015923, 120, 5, 1, 'Scadeald Fox (captured in game)'),
(9920308, 164083, 0, 0, 0, 1, 1, -7652.3804, 583.0219, 108.17255, 2.0450056, 120, 5, 1, 'Scadeald Fox (captured in game)'),
(9920309, 164083, 0, 0, 0, 1, 1, -7657.356, 752.28406, 137.88445, 5.6256313, 120, 0, 0, 'Scadeald Fox, sitting (captured in game)'),
(9920310, 164944, 0, 0, 0, 1, 1, -7760.204, 665.6242, 95.9402, 2.720434, 300, 0, 0, 'Possessed Crow, perched (captured in game)'),
(9920311, 161847, 0, 0, 0, 1, 1, -7706.41, 635.7281, 102.72801, 5.7756443, 120, 5, 1, 'Butterfly (captured in game)'),
(9920312, 164573, 0, 0, 0, 1, 1, -7625.741, 675.2636, 136.29047, 0.60301334, 120, 5, 1, 'Scadeald Moth (captured in game)'),
(9920313, 164571, 0, 0, 0, 1, 1, -7674.1343, 698.94214, 134.86157, 4.9808235, 120, 5, 1, 'Scadeald Rabbit (captured in game)'),
(9920314, 164020, 0, 0, 0, 1, 1, -7866.17, 816.4472, 72.81804, 5.94218, 300, 0, 0, 'Road Bandit (captured in game)'),
(9920315, 164020, 0, 0, 0, 1, 1, -7853.7793, 811.8974, 72.57498, 3.4446132, 300, 0, 0, 'Road Bandit (captured in game)'),
(9920316, 164020, 0, 0, 0, 1, 1, -8035.335, 830.1459, 68.53115, 3.299304, 300, 0, 0, 'Road Bandit, attack stance (captured in game)'),
(9920317, 164020, 0, 0, 0, 1, 1, -8035.5273, 828.3209, 68.54486, 4.671437, 300, 0, 0, 'Road Bandit (captured in game)'),
(9920318, 164020, 0, 0, 0, 1, 1, -8021.159, 807.0317, 68.43181, 3.7485807, 300, 0, 0, 'Road Bandit (captured in game)'),
(9920319, 164020, 0, 0, 0, 1, 1, -7982.648, 794.6008, 70.29563, 2.6113145, 300, 0, 0, 'Road Bandit (captured in game)'),
(9920320, 164020, 0, 0, 0, 1, 1, -7994.147, 818.907, 64.14584, 4.3376184, 300, 0, 0, 'Road Bandit (captured in game)'),
(9920321, 164020, 0, 0, 0, 1, 1, -7985.32, 842.0341, 73.618965, 4.1334176, 300, 0, 0, 'Road Bandit (captured in game)'),
(9920322, 164020, 0, 0, 0, 1, 1, -7868.058, 854.91754, 65.79009, 2.070157, 300, 0, 0, 'Road Bandit (captured in game)'),
(9920323, 164020, 0, 0, 0, 1, 1, -7846.085, 866.1569, 60.5606, 2.285356, 300, 4, 1, 'Road Bandit, small circuit (captured in game)'),
(9920324, 164020, 0, 0, 0, 1, 1, -7861.736, 777.5702, 71.32616, 2.4644282, 300, 0, 0, 'Road Bandit, sleeping (captured in game)'),
(9920325, 164021, 0, 0, 0, 1, 1, -7840.138, 868.6254, 59.241955, 2.1911077, 300, 0, 0, 'Road Marauder (captured in game)'),
(9920326, 164020, 0, 0, 0, 1, 1, -7900.0444, 788.9813, 70.04167, 1.7259935, 300, 0, 0, 'Road Bandit (captured in game)'),
(9920327, 164021, 0, 0, 0, 1, 1, -7852.28, 811.897, 72.574, 3.44461, 300, 0, 0, 'Road Marauder, camp (captured in game)'),
(9920328, 164020, 0, 0, 0, 1, 1, -7853.78, 813.4, 72.574, 3.44461, 300, 0, 0, 'Road Bandit, sleeping, camp (captured in game)'),
(9920329, 164021, 0, 0, 0, 1, 1, -7855.28, 811.897, 72.574, 3.44461, 300, 0, 0, 'Road Marauder, sitting, camp (captured in game)'),
(9920330, 164428, 0, 0, 0, 1, 1, -8067.1074, 739.967, 67.1801, 5.7049665, 300, 0, 0, 'Stormwind Guard, attack stance (captured in game)'),
(9920331, 164427, 0, 0, 0, 1, 1, -8068.022, 754.5123, 67.2222, 0.6800326, 300, 0, 0, 'Stormwind Guard, attack stance (captured in game)'),
(9920332, 164427, 0, 0, 0, 1, 1, -8002.701, 869.2036, 63.98377, 4.186825, 300, 0, 0, 'Stormwind Guard, attack stance (captured in game)'),
(9920333, 164090, 0, 0, 0, 1, 1, -8079.012, 744.3209, 67.57425, 2.7691936, 300, 0, 0, 'Graysky Donkey (Tito) (captured in game)'),
(9920334, 9920040, 0, 0, 0, 1, 1, -8071.8003, 750.63654, 67.60621, 5.534577, 300, 0, 0, 'Guard Dornhelm (captured in game)'),
(9920335, 9920041, 0, 0, 0, 1, 1, -8071.4688, 748.13135, 67.64727, 0.78056353, 300, 0, 0, 'Merchant Tharwin (captured in game)'),
(9920336, 164210, 0, 0, 0, 1, 1, -7773.255, 785.41675, 149.99352, 2.6214836, 300, 0, 0, 'Cadmilla (captured in game)'),
(9920337, 164002, 0, 0, 0, 1, 1, -7783.8423, 790.5608, 150.65681, 3.1319928, 300, 0, 0, 'Captain Hedgar (captured in game)'),
(9920338, 164058, 0, 0, 0, 1, 1, -7767.231, 806.4722, 146.79147, 0.66898257, 300, 0, 0, 'Graysky Soldier (captured in game)'),
(9920339, 164027, 0, 0, 0, 1, 1, -7788.43, 790.594, 150.644, 6.28144, 300, 0, 0, 'Graysky Soldier with shield, picking the door lock (captured in game)'),
(9920340, 164944, 0, 0, 0, 1, 1, -7789.5737, 811.95435, 163.28224, 3.5694466, 300, 0, 0, 'Possessed Crow, perched on the manor (captured in game)'),
(9920341, 164490, 0, 0, 0, 1, 1, -8206.36, 795.04443, 71.078514, 1.9060051, 300, 0, 0, 'Stormwind Lumberjack (captured in game)'),
(9920342, 164490, 0, 0, 0, 1, 1, -8196.331, 818.28467, 70.04737, 0.7106286, 300, 0, 0, 'Stormwind Lumberjack (captured in game)'),
(9920343, 164020, 0, 0, 0, 1, 1, -7862.5493, 820.7635, 71.239334, 0, 300, 0, 2, 'Road Bandit, patrol (captured in game)'),
(9920344, 164020, 0, 0, 0, 1, 1, -7918.9854, 806.1785, 71.08814, 0, 300, 0, 2, 'Road Bandit, patrol (captured in game)'),
(9920345, 164020, 0, 0, 0, 1, 1, -7787.4375, 861.30786, 55.996147, 0, 300, 0, 2, 'Road Bandit, cave patrol (captured in game)'),
(9920346, 164027, 0, 0, 0, 1, 1, -7812.7173, 792.94586, 150.64519, 0, 300, 0, 2, 'Graysky Soldier, patrol to the door (captured in game)'),
(9920347, 164027, 0, 0, 0, 1, 1, -7766.5713, 772.18085, 150.66223, 0, 300, 0, 2, 'Graysky Soldier, garden patrol (captured in game)');
UPDATE `creature` SET `equipment_id` = 1 WHERE `guid` BETWEEN 9920300 AND 9920399 AND `id` IN (164020, 164021, 164027);
DELETE FROM `creature_addon` WHERE `guid` BETWEEN 9920300 AND 9920399;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `visibilityDistanceType`, `auras`) VALUES
(9920309, 0, 0, 1, 1, 0, 0, ''),
(9920310, 0, 0, 1, 1, 0, 0, ''),
(9920316, 0, 0, 0, 1, 333, 0, '28126'),
(9920324, 0, 0, 3, 1, 0, 0, '28126'),
(9920328, 0, 0, 3, 1, 0, 0, '28126'),
(9920329, 0, 0, 1, 1, 0, 0, '28126'),
(9920330, 0, 0, 0, 1, 333, 0, ''),
(9920331, 0, 0, 0, 1, 333, 0, ''),
(9920332, 0, 0, 0, 1, 333, 0, ''),
(9920339, 0, 0, 0, 1, 69, 0, ''),
(9920340, 0, 0, 1, 1, 0, 0, ''),
(9920341, 0, 0, 0, 1, 234, 0, ''),
(9920342, 0, 0, 0, 1, 234, 0, ''),
(9920343, 99203430, 0, 0, 1, 0, 0, '28126'),
(9920344, 99203440, 0, 0, 1, 0, 0, '28126'),
(9920345, 99203450, 0, 0, 1, 0, 0, '28126'),
(9920346, 99203460, 0, 0, 1, 0, 0, ''),
(9920347, 99203470, 0, 0, 1, 0, 0, '');
DELETE FROM `waypoint_data` WHERE `id` BETWEEN 99203000 AND 99203990;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(99203430, 1, -7862.5493, 820.7635, 71.239334, 0, 3000, 0, 0, 100, 0),
(99203430, 2, -7810.351, 864.64276, 56.87374, 0, 3000, 0, 0, 100, 0),
(99203440, 1, -7918.9854, 806.1785, 71.08814, 0, 3000, 0, 0, 100, 0),
(99203440, 2, -7874.7476, 792.6419, 71.32849, 0, 3000, 0, 0, 100, 0),
(99203450, 1, -7787.4375, 861.30786, 55.996147, 0, 3000, 0, 0, 100, 0),
(99203450, 2, -7811.4175, 823.4547, 53.64115, 0, 3000, 0, 0, 100, 0),
(99203460, 1, -7812.7173, 792.94586, 150.64519, 0, 3000, 0, 0, 100, 0),
(99203460, 2, -7790.4, 790.8, 150.64, 0, 3000, 0, 0, 100, 0),
(99203470, 1, -7766.5713, 772.18085, 150.66223, 0, 3000, 0, 0, 100, 0),
(99203470, 2, -7746.8096, 754.3615, 151.35115, 0, 3000, 0, 0, 100, 0);
DELETE FROM `gameobject` WHERE `guid` BETWEEN 9920300 AND 9920399;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `Comment`) VALUES
(9920300, 2300632, 0, 0, 0, 1, 1, -7787.5195, 814.0022, 157.92937, 3.9676428, 0, 0, 0, 1, 60, 100, 1, 'Arathon''s Journal, Arathon''s room upstairs (captured in game)'),
(9920301, 2300635, 0, 0, 0, 1, 1, -7897.5005, 818.03217, 69.72067, 3.981818, 0, 0, 0, 1, 120, 100, 1, 'Stolen Goods (captured in game)'),
(9920302, 2300635, 0, 0, 0, 1, 1, -7901.7197, 787.89105, 70.30573, 4.5488796, 0, 0, 0, 1, 120, 100, 1, 'Stolen Goods (captured in game)'),
(9920303, 2300635, 0, 0, 0, 1, 1, -7875.3696, 788.90784, 71.41653, 6.017575, 0, 0, 0, 1, 120, 100, 1, 'Stolen Goods (captured in game)'),
(9920304, 2300635, 0, 0, 0, 1, 1, -7859.448, 792.90045, 69.33908, 2.198187, 0, 0, 0, 1, 120, 100, 1, 'Stolen Goods (captured in game)'),
(9920305, 2300635, 0, 0, 0, 1, 1, -7860.1304, 771.6815, 70.351906, 2.2932222, 0, 0, 0, 1, 120, 100, 1, 'Stolen Goods (captured in game)'),
(9920306, 2300635, 0, 0, 0, 1, 1, -7852.5317, 815.7784, 71.55686, 3.9417815, 0, 0, 0, 1, 120, 100, 1, 'Stolen Goods (captured in game)'),
(9920307, 2300635, 0, 0, 0, 1, 1, -7867.7593, 811.85846, 73.553276, 6.232785, 0, 0, 0, 1, 120, 100, 1, 'Stolen Goods (captured in game)'),
(9920308, 2300635, 0, 0, 0, 1, 1, -7861.0645, 831.1301, 68.610725, 0.7781903, 0, 0, 0, 1, 120, 100, 1, 'Stolen Goods (captured in game)'),
(9920309, 2300635, 0, 0, 0, 1, 1, -7835.7764, 871.1807, 58.818165, 1.0946931, 0, 0, 0, 1, 120, 100, 1, 'Stolen Goods (captured in game)'),
(9920310, 2300635, 0, 0, 0, 1, 1, -7846.085, 866.1569, 60.5606, 2.285356, 0, 0, 0, 1, 120, 100, 1, 'Stolen Goods (captured in game)'),
(9920311, 2300635, 0, 0, 0, 1, 1, -7819.632, 825.8347, 53.398613, 6.122667, 0, 0, 0, 1, 120, 100, 1, 'Stolen Goods (captured in game)'),
(9920312, 2300635, 0, 0, 0, 1, 1, -7812.1753, 819.56476, 53.308956, 1.514741, 0, 0, 0, 1, 120, 100, 1, 'Stolen Goods (captured in game)'),
(9920313, 2300635, 0, 0, 0, 1, 1, -7798.7354, 826.1075, 55.04171, 0.1497192, 0, 0, 0, 1, 120, 100, 1, 'Stolen Goods (captured in game)'),
(9920314, 1731, 0, 0, 0, 1, 1, -7903.8823, 790.18304, 70.69241, 0.41297057, 0, 0, 0, 1, 900, 100, 1, 'Copper Vein (captured in game)'),
(9920315, 1731, 0, 0, 0, 1, 1, -7848.8013, 828.714, 64.55052, 3.736002, 0, 0, 0, 1, 900, 100, 1, 'Copper Vein (captured in game)'),
(9920316, 1731, 0, 0, 0, 1, 1, -7968.2026, 774.9403, 81.22918, 0.46478534, 0, 0, 0, 1, 900, 100, 1, 'Copper Vein (captured in game)');
