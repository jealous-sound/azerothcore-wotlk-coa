-- The well cave beneath the Darengar Estate, Dunshire and the surrounding wilds, from in-game captures (2026-10-04).
-- Wild Ealdir, Dunshire Guard and Ulira Greyvale use the CoA mirror-image presets already in creature_display_preset.
-- To Believe In An Ideal Is To Be Willing To Betray It and The Black Rook of Graysky are not in the Exiles export.
INSERT INTO `creature_template` (`entry`, `name`, `subname`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `gossip_menu_id`, `speed_walk`, `speed_run`, `rank`, `unit_class`, `unit_flags`, `type`, `lootid`, `AIName`, `MovementType`, `HealthModifier`, `flags_extra`, `VerifiedBuild`)
VALUES
(164004, 'Wild Ealdir', '', 18, 19, 14, 0, 0, 1, 1.14286, 0, 1, 0, 7, 164004, '', 0, 1, 0, 0),
(164926, 'Ealdir Ambusher', '', 18, 19, 14, 0, 0, 1, 1.14286, 0, 1, 0, 7, 0, '', 0, 1, 0, 0),
(164039, 'Scadeald Bear', '', 20, 21, 32, 0, 0, 1, 1.14286, 0, 1, 0, 1, 0, '', 0, 1, 0, 0),
(164122, 'Rylan', 'Stable Master', 25, 25, 12, 4194305, 9821, 1, 1.14286, 0, 1, 768, 7, 0, '', 0, 1, 2, 0),
(164124, 'Dunshire Guard', '', 30, 30, 12, 1, 9920060, 1, 1.14286, 0, 1, 768, 7, 0, '', 0, 1, 2, 0),
(164116, 'Ulira Greyvale', '', 20, 20, 12, 2, 0, 1, 1.14286, 0, 1, 768, 7, 0, '', 0, 1, 2, 0),
(164005, 'Lady Serenya Coldmere', 'Regent of Graysky', 30, 30, 12, 2, 0, 1, 1.14286, 0, 1, 768, 7, 0, '', 0, 1, 2, 0),
(164034, 'Hestwin Honeycrust', 'Baker of Dunshire', 20, 20, 12, 131, 9920061, 1, 1.14286, 0, 1, 768, 7, 0, '', 0, 1, 2, 0),
(164035, 'Miller Bartren', '', 20, 20, 12, 2, 0, 1, 1.14286, 0, 1, 768, 7, 0, 'SmartAI', 0, 1, 2, 0),
(164036, 'Innkeeper Miralda', '', 30, 30, 12, 128, 0, 1, 1.14286, 0, 1, 768, 7, 0, '', 0, 1, 2, 0),
(164119, 'Yorin Hollowbrook', '', 20, 20, 12, 2, 0, 1, 1.14286, 0, 1, 768, 7, 0, '', 0, 1, 2, 0),
(164037, 'Darond', '', 20, 20, 12, 2, 0, 1, 1.14286, 0, 1, 768, 7, 0, '', 0, 1, 2, 0),
(164117, 'Aldwin Greymoor', '', 20, 20, 12, 1, 9920062, 1, 1.14286, 0, 1, 768, 7, 0, '', 0, 1, 2, 0),
(164172, 'Geralda', 'Assistant Baker', 20, 20, 12, 0, 0, 1, 1.14286, 0, 1, 768, 7, 0, '', 0, 1, 2, 0),
(164120, 'Helara Nublar', 'Weaponsmith', 25, 25, 12, 4224, 0, 1, 1.14286, 0, 1, 768, 7, 0, '', 0, 1, 2, 0)
ON DUPLICATE KEY UPDATE `name` = VALUES(`name`), `subname` = VALUES(`subname`), `minlevel` = VALUES(`minlevel`),
  `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `npcflag` = VALUES(`npcflag`),
  `gossip_menu_id` = VALUES(`gossip_menu_id`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`),
  `rank` = VALUES(`rank`), `unit_class` = VALUES(`unit_class`), `unit_flags` = VALUES(`unit_flags`),
  `type` = VALUES(`type`), `lootid` = VALUES(`lootid`), `AIName` = VALUES(`AIName`),
  `MovementType` = VALUES(`MovementType`), `HealthModifier` = VALUES(`HealthModifier`), `flags_extra` = VALUES(`flags_extra`),
  `VerifiedBuild` = VALUES(`VerifiedBuild`);
DELETE FROM `creature_template_model` WHERE `CreatureID` IN (164004, 164124, 164116, 164005, 164034, 164035, 164036, 164119, 164037, 164117, 164172, 164120, 164122, 164039);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES
(164004, 0, 49, 1, 1, 0),
(164004, 1, 50, 1, 1, 0),
(164124, 0, 49, 1, 1, 0),
(164124, 1, 50, 1, 1, 0),
(164116, 0, 50, 1, 1, 0),
(164005, 0, 50, 1, 1, 0),
(164034, 0, 49, 1, 1, 0),
(164035, 0, 49, 1, 1, 0),
(164036, 0, 50, 1, 1, 0),
(164119, 0, 49, 1, 1, 0),
(164037, 0, 49, 1, 1, 0),
(164117, 0, 49, 1, 1, 0),
(164172, 0, 141456, 1, 1, 0),
(164120, 0, 50, 1, 1, 0),
(164122, 0, 49, 1, 1, 0),
(164039, 0, 820, 1, 1, 0);
UPDATE `creature_template` SET `family` = 4 WHERE `entry` = 164039;
DELETE FROM `creature_model_info` WHERE `DisplayID` = 141456;
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`, `DisplayID_Other_Gender`, `VerifiedBuild`) VALUES
(141456, 0.208, 1, 1, 0, 0);
DELETE FROM `npc_vendor` WHERE `entry` IN (164034, 164036, 164120);
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `VerifiedBuild`) VALUES
(164034, 0, 4540, 0, 0, 0, 0),
(164034, 1, 4541, 0, 0, 0, 0),
(164034, 2, 4542, 0, 0, 0, 0),
(164034, 3, 4544, 0, 0, 0, 0),
(164034, 4, 4601, 0, 0, 0, 0),
(164034, 5, 8950, 0, 0, 0, 0),
(164034, 6, 772065, 10, 3600, 0, 0),
(164034, 7, 772074, 10, 3600, 0, 0),
(164036, 0, 559863, 0, 0, 0, 0),
(164036, 1, 135060, 0, 0, 0, 0),
(164036, 2, 4496, 0, 0, 0, 0),
(164036, 3, 4498, 0, 0, 0, 0),
(164036, 4, 5576, 0, 0, 0, 0),
(164036, 5, 4499, 0, 0, 0, 0),
(164036, 6, 1708, 0, 0, 0, 0),
(164036, 7, 8766, 0, 0, 0, 0),
(164036, 8, 6948, 0, 0, 0, 0),
(164036, 9, 3927, 0, 0, 0, 0),
(164120, 0, 2901, 0, 0, 0, 0),
(164120, 1, 5956, 0, 0, 0, 0),
(164120, 2, 7005, 0, 0, 0, 0),
(164120, 3, 2880, 0, 0, 0, 0),
(164120, 4, 3466, 0, 0, 0, 0),
(164120, 5, 3857, 0, 0, 0, 0),
(164120, 6, 922, 0, 0, 0, 0),
(164120, 7, 923, 0, 0, 0, 0),
(164120, 8, 924, 0, 0, 0, 0),
(164120, 9, 925, 0, 0, 0, 0),
(164120, 10, 926, 0, 0, 0, 0),
(164120, 11, 927, 0, 0, 0, 0),
(164120, 12, 928, 0, 0, 0, 0),
(164120, 13, 2209, 0, 0, 0, 0),
(164120, 14, 4817, 0, 0, 0, 0),
(164120, 15, 4818, 1, 3600, 0, 0),
(164120, 16, 2587501, 1, 3600, 0, 0);
DELETE FROM `creature_text` WHERE `CreatureID` = 164035;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(164035, 0, 0, 'Turn the crank with all your might! Don''t stop!', 12, 0, 100, 0, 0, 0, 0, 0, 'Miller Bartren - From Grain to Flour accepted'),
(164035, 1, 0, 'You''ve kept that mill spinning like a dozen oxen! Now, enjoy the pie!', 12, 0, 100, 0, 0, 0, 0, 0, 'Miller Bartren - Flour ground');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 164035 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `event_param6`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(164035, 0, 0, 0, 19, 0, 100, 0, 9920021, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'Miller Bartren - On From Grain to Flour accepted - Urge the player on');
DELETE FROM `npc_text` WHERE `ID` IN (9920060, 9920061, 9920062);
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `Probability0`, `VerifiedBuild`) VALUES
(9920060, 'Just passing through? Tell me, how can I help you?', 'Just passing through? Tell me, how can I help you?', 1, 0),
(9920061, 'I sell bread from all over, but none compares to the bread of Dunshire. Crispy, aromatic, and freshly baked...', 'I sell bread from all over, but none compares to the bread of Dunshire. Crispy, aromatic, and freshly baked...', 1, 0),
(9920062, 'Are you new to Dunshire? <Despite his cordial tone, Aldwyn''s gaze feels impossibly distant.>$B$BYes, I thought so. My family has lived in these parts for generations, and I know a thing or two about life here. It has always been said that when you meet someone from Dunshire on a staircase, it is impossible to tell whether they are going up or coming down. Some take us for indecisive, when in truth our character is born of caution.$B$BYou never know what your neighbor thinks, nor whom he prays to in the privacy of his own home. Caught between Stormwind and Scadeald, we folk of Dunshire have learned that the wisest course is to keep our opinions to ourselves and pass unnoticed.', 'Are you new to Dunshire? <Despite his cordial tone, Aldwyn''s gaze feels impossibly distant.>$B$BYes, I thought so. My family has lived in these parts for generations, and I know a thing or two about life here. It has always been said that when you meet someone from Dunshire on a staircase, it is impossible to tell whether they are going up or coming down. Some take us for indecisive, when in truth our character is born of caution.$B$BYou never know what your neighbor thinks, nor whom he prays to in the privacy of his own home. Caught between Stormwind and Scadeald, we folk of Dunshire have learned that the wisest course is to keep our opinions to ourselves and pass unnoticed.', 1, 0);
DELETE FROM `gossip_menu` WHERE `MenuID` IN (9920060, 9920061, 9920062);
INSERT INTO `gossip_menu` (`MenuID`, `TextID`) VALUES
(9920060, 9920060),
(9920061, 9920061),
(9920062, 9920062);
DELETE FROM `gossip_menu_option` WHERE `MenuID` = 9920061;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcFlag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(9920061, 0, 1, 'I want to browse your goods.', 0, 3, 128, 0, 0, 0, 0, '', 0, 0);
UPDATE `creature_template` SET `lootid` = 164083 WHERE `entry` = 164083;
DELETE FROM `creature_loot_template` WHERE `Entry` IN (164004, 164083);
INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
(164004, 559852, 0, 20, 1, 1, 0, 1, 1, 'Wild Ealdir - Notes: "Agreement with Arathon Darengar"'),
(164083, 560340, 0, 50, 1, 1, 0, 1, 1, 'Scadeald Fox - Hen Remains');
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `Data0`, `Data1`, `Data3`, `Data8`, `VerifiedBuild`)
VALUES
(2300631, 2, 6708, 'Weapon Crate', 1, 0, 0, 0, 0, 0),
(2300640, 3, 300499, 'Dunshire Wheat', 2, 1689, 2300640, 1, 9920019, 0),
(2300644, 3, 1066612, 'Elven Medallion of the Sanctuary', 1, 1689, 2300644, 1, 9920020, 0),
(2300641, 10, 1054456, 'Dunshire Mill', 1, 0, 9920021, 0, 0, 0),
(2300642, 3, 1051861, 'Dunshire Eggs', 1, 1689, 2300642, 1, 9920022, 0),
(2300643, 3, 1040324, 'Kisswind Cow Milk', 1, 1689, 2300643, 1, 0, 0)
ON DUPLICATE KEY UPDATE `type` = VALUES(`type`), `displayId` = VALUES(`displayId`), `name` = VALUES(`name`),
  `size` = VALUES(`size`), `Data0` = VALUES(`Data0`), `Data1` = VALUES(`Data1`),
  `Data3` = VALUES(`Data3`), `Data8` = VALUES(`Data8`), `VerifiedBuild` = VALUES(`VerifiedBuild`);
UPDATE `gameobject_template` SET `ScriptName` = 'go_dunshire_mill' WHERE `entry` = 2300641;
DELETE FROM `gameobject_loot_template` WHERE `Entry` IN (2300640, 2300644, 2300642, 2300643);
INSERT INTO `gameobject_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
(2300640, 559861, 0, 100, 1, 1, 0, 1, 2, 'Dunshire Wheat'),
(2300644, 559870, 0, 100, 1, 1, 0, 1, 1, 'Elven Medallion of the Sanctuary'),
(2300642, 559865, 0, 100, 1, 1, 0, 1, 2, 'Dunshire Egg'),
(2300643, 559864, 0, 100, 0, 1, 0, 1, 1, 'Windswept Cow Milk');
DELETE FROM `spell_script_names` WHERE `spell_id` = 365012;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(365012, 'spell_coa_grinding_the_wheat');
INSERT INTO `quest_template` (`ID`, `QuestType`, `QuestLevel`, `MinLevel`, `QuestSortID`, `RewardXPDifficulty`, `RewardMoney`, `Flags`, `RewardItem1`, `RewardAmount1`, `RewardItem2`, `RewardAmount2`, `RewardChoiceItemID1`, `RewardChoiceItemQuantity1`, `RewardChoiceItemID2`, `RewardChoiceItemQuantity2`, `RewardChoiceItemID3`, `RewardChoiceItemQuantity3`, `RewardChoiceItemID4`, `RewardChoiceItemQuantity4`, `LogTitle`, `LogDescription`, `QuestDescription`, `QuestCompletionLog`, `RequiredNpcOrGo1`, `RequiredNpcOrGoCount1`, `RequiredItemId1`, `RequiredItemCount1`, `RequiredItemId2`, `RequiredItemCount2`, `VerifiedBuild`)
VALUES
(9920016, 2, -1, 18, 10302, 7, 700, 8, 375250, 100, 1397884, 1, 559969, 1, 560147, 1, 560148, 1, 560149, 1, 'To Believe In An Ideal Is To Be Willing To Betray It', 'Eliminate the Ealdir threat and uncover the plot they were hatching within the well of the Darengar Estate.', '<As you inspect the crates, a chill runs down your spine: nearly all the weapons have been plundered. The trail of footprints surrounding them reveals an unsettling mix of heavy boots and bare feet.>$B$B<It seems it is true, then: Arathon Darengar was in league with the Ealdir.>$B$B<You should seek more information to bring their plans to light.>', 'Return to Captain Hedgar at the Darengar Estate.', 164004, 8, 559852, 1, 0, 0, 0),
(9920017, 2, -1, 18, 10302, 7, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'The Black Rook of Graysky', 'Report to Lady Serenya Coldmere in Graysky.', 'I suspect there are yet many secrets to be unearthed in this manor. The Darengars have been conspiring behind House Coldmere''s back for years; we have only scratched the surface of their intrigues.$B$BAs for you, you have shown diligence and sound judgment. If you are bound for Graysky, I have a task for you; the highest of honors:$B$BPresent yourself to my lady, Serenya Coldmere, and apprise her of what we have uncovered here today: the sheer scope of Lord Arathon''s treason and the Ealdir''s grim plot.', 'Report to Lady Serenya Coldmere in Graysky.', 0, 0, 0, 0, 0, 0, 0),
(9920018, 2, -1, 18, 10302, 7, 1200, 8, 375250, 100, 1397884, 1, 559977, 1, 559980, 1, 559994, 1, 0, 0, 'Wolves Nearby', 'Slay the wolves prowling around Dunshire, at the foot of the cliff.', 'Ah, it is rare to see new faces in Dunshire these days.$B$BI suppose you are something of a... mercenary?$B$BGood, good. I have money to spare, and work that needs doing.$B$BWith trade disrupted, the kingdom has neglected the roads, and the wolves have grown bold. It is only a matter of time before they reach Dunshire.$B$BKill a few south of here. Remind them to fear steel and flame.', 'Return to Yorin Hollowbrook in Dunshire.', 164084, 8, 0, 0, 0, 0, 0),
(9920019, 2, -1, 18, 10302, 7, 250, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Dunshire''s Signature Pie', 'Gather the wheat needed to prepare the famous Dunshire Pie.', 'I''m sorry, I''m up to my neck today and I''m not taking any more orders. Come back some other tim--$B$B<The baker looks up for a moment.>$B$BWell, well, well... what do we have here? A true sweet tooth!$B$BYou''re here because you want to taste my specialty, surely. The world-famous Dunshire Pie! People from Graysky, Cresthairn, Rockrend, Shadewell, and even Dun Kazad come just for it... and they always come back!$B$BYes, your mouth is watering. I''ll make one for you, don''t worry, but in exchange, I need you to lend me a hand with today''s work. Start by harvesting the wheat from the crop fields. Once you have it all, take it to the miller.', 'Take the wheat to Miller Bartren in Dunshire.', 0, 0, 559861, 12, 0, 0, 0),
(9920020, 2, -1, 18, 10302, 7, 350, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'The Lost Medallion', 'Find the magic amulet belonging to Darond, the Dunshire porter.', 'You! Yes, you. You haven''t seen a medallion around here, have you?$B$B<Darond''s shoulders slump as he sees you shake your head.>$B$BI am the porter of this village, and a ''friend'' to the elves. The medallion I''ve lost is my safeguard to pass through the enchanted barrier in the cave. Without it, I cannot do my job; every now and then, the elves require my services.$B$BThis is one of those times. And I have already searched for the amulet everywhere.$B$BWould you mind lending me a hand?', 'Return to Darond in Dunshire.', 0, 0, 559870, 1, 0, 0, 0),
(9920021, 2, -1, 18, 10302, 7, 250, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'From Grain to Flour', 'Activate the windmill to produce the flour needed for the famous Dunshire Pie.', 'Flour production cannot stop, and the wind isn''t blowing nearly hard enough today to get through all these orders...$B$B<You explain to the miller that you only need to grind a little wheat for the baker''s errand.>$B$BA Dunshire Pie!? You should have said so sooner! Give me the wheat, I''ll help you myself!$B$BI''ll set the grain in the hopper; you give that crank a good turn. The wind isn''t helping us today, and we need a fine flour if we want a proper pie.', 'Bring the flour to Hestwin Honeycrust in Dunshire.', 0, 0, 559862, 1, 0, 0, 0),
(9920022, 2, -1, 18, 10302, 7, 350, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'From Plot to Plate', 'Obtain Dunshire eggs and Exquisite Windswept Milk from the Dunshire tavern.', 'The recipe for Dunshire Pie requires several ingredients. I have most of what''s needed right here, but you still have to provide a couple more, you sweet tooth.$B$BGet out there and bring me the best eggs from the Dunshire hens.$B$BAnd... hmmm... I could make you climb the mountain, but it''ll be easier if you go to the inn and buy a couple of liters of that Exquisite Windswept Milk they bring down from the summit.', 'Bring the eggs and milk to Hestwin Honeycrust in Dunshire.', 0, 0, 559865, 7, 559863, 2, 0),
(175327, 2, -1, 18, 10302, 3, 1310, 8, 375250, 50, 1397884, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Poor Petunia', 'Find the remains of Petunia the hen in the stomach of one of the foxes prowling around Dunshire.', 'You are... armed.$B$BOf course. You made it all the way here. And with the roads as they are...$B$B<Ulira keeps scratching her hands compulsively.>$B$BMight you do me a favor?$B$BLast night... Petunia disappeared. One of my hens. She was a beautiful soul. A laying hen, like the others.$B$BI saw blood. And feathers. I think one of the foxes prowling around the village ate her.$B$BCould you... look for her? She will be in some fox''s belly, I suppose. And the truth is, I would like to bury her.$B$B<Ulira''s watery eyes give you an imploring look.>', 'Return to Ulira Greyvale in Dunshire.', 0, 0, 560340, 1, 0, 0, 0)
ON DUPLICATE KEY UPDATE `QuestType` = VALUES(`QuestType`), `QuestLevel` = VALUES(`QuestLevel`), `MinLevel` = VALUES(`MinLevel`),
  `QuestSortID` = VALUES(`QuestSortID`), `RewardXPDifficulty` = VALUES(`RewardXPDifficulty`), `RewardMoney` = VALUES(`RewardMoney`),
  `Flags` = VALUES(`Flags`), `RewardItem1` = VALUES(`RewardItem1`), `RewardAmount1` = VALUES(`RewardAmount1`),
  `RewardItem2` = VALUES(`RewardItem2`), `RewardAmount2` = VALUES(`RewardAmount2`), `RewardChoiceItemID1` = VALUES(`RewardChoiceItemID1`),
  `RewardChoiceItemQuantity1` = VALUES(`RewardChoiceItemQuantity1`), `RewardChoiceItemID2` = VALUES(`RewardChoiceItemID2`), `RewardChoiceItemQuantity2` = VALUES(`RewardChoiceItemQuantity2`),
  `RewardChoiceItemID3` = VALUES(`RewardChoiceItemID3`), `RewardChoiceItemQuantity3` = VALUES(`RewardChoiceItemQuantity3`), `RewardChoiceItemID4` = VALUES(`RewardChoiceItemID4`),
  `RewardChoiceItemQuantity4` = VALUES(`RewardChoiceItemQuantity4`), `LogTitle` = VALUES(`LogTitle`), `LogDescription` = VALUES(`LogDescription`),
  `QuestDescription` = VALUES(`QuestDescription`), `QuestCompletionLog` = VALUES(`QuestCompletionLog`), `RequiredNpcOrGo1` = VALUES(`RequiredNpcOrGo1`),
  `RequiredNpcOrGoCount1` = VALUES(`RequiredNpcOrGoCount1`), `RequiredItemId1` = VALUES(`RequiredItemId1`), `RequiredItemCount1` = VALUES(`RequiredItemCount1`),
  `RequiredItemId2` = VALUES(`RequiredItemId2`), `RequiredItemCount2` = VALUES(`RequiredItemCount2`), `VerifiedBuild` = VALUES(`VerifiedBuild`);
DELETE FROM `quest_template_addon` WHERE `ID` IN (9920016, 9920017, 9920021, 9920022);
INSERT INTO `quest_template_addon` (`ID`, `PrevQuestID`) VALUES
(9920016, 9920015),
(9920017, 9920016),
(9920021, 9920019),
(9920022, 9920021);
DELETE FROM `quest_request_items` WHERE `ID` IN (9920016, 9920019, 9920021);
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `CompletionText`, `VerifiedBuild`) VALUES
(9920016, 0, 0, '<The Captain has been watching you closely since you arrived at the estate.>', 0),
(9920019, 0, 0, 'As long as the sails keep turning, everything will be fine.$B$BOh... do you need something?', 0),
(9920021, 0, 0, 'The baker''s main ingredient is flour. Do you have it yet, you sweet tooth?', 0);
DELETE FROM `quest_offer_reward` WHERE `ID` IN (9920016, 9920019);
INSERT INTO `quest_offer_reward` (`ID`, `RewardText`, `VerifiedBuild`) VALUES
(9920016, '<The captain beckons you from a distance with a grim gesture.>$B$BThe handmaid has told me everything.$B$BWe had grave suspicions that the Darengars were conspiring against the government of Graysky, but to think they would dare go this far...$B$BThey were arming the Ealdir! I imagine their intent was to deepen the crisis facing Scadeald, further undermining Lady Serenya''s rule.', 0),
(9920019, 'The wind isn''t blowing today... You''ll have to wait until we finish grinding what we already have.$B$BThen you can grind your wheat.', 0);
DELETE FROM `gameobject_queststarter` WHERE `quest` = 9920016;
INSERT INTO `gameobject_queststarter` (`id`, `quest`) VALUES
(2300631, 9920016);
DELETE FROM `creature_queststarter` WHERE `quest` IN (9920017, 9920018, 9920019, 9920020, 9920021, 9920022, 175327);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(164002, 9920017),
(164119, 9920018),
(164034, 9920019),
(164037, 9920020),
(164035, 9920021),
(164034, 9920022),
(164116, 175327);
DELETE FROM `creature_questender` WHERE `quest` IN (9920016, 9920017, 9920018, 9920019, 9920020, 9920021, 9920022, 175327);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(164002, 9920016),
(164005, 9920017),
(164119, 9920018),
(164035, 9920019),
(164037, 9920020),
(164034, 9920021),
(164034, 9920022),
(164116, 175327);
DELETE FROM `creature` WHERE `guid` BETWEEN 9920400 AND 9920499;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `MovementType`, `Comment`) VALUES
(9920400, 17467, 0, 0, 0, 1, 1, -7814.8066, 841.43896, 47.87271, 4.6045384, 120, 5, 1, 'Skunk (captured in game)'),
(9920401, 883, 0, 0, 0, 1, 1, -7886.989, 609.69183, 90.834724, 2.1431053, 120, 6, 1, 'Deer (captured in game)'),
(9920402, 883, 0, 0, 0, 1, 1, -7888.5, 611.2, 90.8, 2.1431053, 120, 6, 1, 'Deer (captured in game)'),
(9920403, 883, 0, 0, 0, 1, 1, -7891.144, 605.56354, 90.627594, 2.3818595, 120, 6, 1, 'Deer (captured in game)'),
(9920404, 164083, 0, 0, 0, 1, 1, -7649.1807, 672.4626, 133.51622, 2.961479, 120, 5, 1, 'Scadeald Fox (captured in game)'),
(9920405, 164083, 0, 0, 0, 1, 1, -7646.273, 877.5343, 132.43797, 5.7739816, 120, 5, 1, 'Scadeald Fox (captured in game)'),
(9920406, 164083, 0, 0, 0, 1, 1, -7785.7007, 491.97812, 144.82101, 2.5067172, 120, 5, 1, 'Scadeald Fox, south of Dunshire (captured in game)'),
(9920407, 164083, 0, 0, 0, 1, 1, -7812.0264, 503.54224, 143.39964, 0.2926149, 120, 5, 1, 'Scadeald Fox, south-west of Dunshire (captured in game)'),
(9920408, 164084, 0, 0, 0, 1, 1, -7745.9263, 714.53406, 139.96092, 3.6729724, 300, 0, 0, 'Scadeald Wolf, sitting (captured in game)'),
(9920409, 164571, 0, 0, 0, 1, 1, -7669.7783, 762.7092, 137.17673, 1.2091789, 120, 5, 1, 'Scadeald Rabbit, Oldmist Forest (captured in game)'),
(9920410, 164083, 0, 0, 0, 1, 1, -7667.9653, 783.79626, 135.90915, 2.9315572, 120, 0, 0, 'Scadeald Fox, sitting, Oldmist Forest (captured in game)'),
(9920411, 164571, 0, 0, 0, 1, 1, -7651.1714, 797.44763, 137.84245, 0.7245883, 120, 5, 1, 'Scadeald Rabbit, Oldmist Forest (captured in game)'),
(9920412, 164039, 0, 0, 0, 1, 1, -7618.316, 802.2309, 138.96922, 0.88166803, 300, 5, 1, 'Scadeald Bear, Oldmist Forest (captured in game)'),
(9920413, 161847, 0, 0, 0, 1, 1, -7708.3296, 632.40393, 99.75835, 4.188772, 120, 6, 1, 'Butterfly (captured in game)'),
(9920414, 620, 0, 0, 0, 1, 1, -7772.501, 610.9505, 136.94449, 4.431749, 120, 5, 1, 'Chicken (captured in game)'),
(9920415, 164571, 0, 0, 0, 1, 1, -7674.422, 828.612, 133.6041, 4.309215, 120, 5, 1, 'Scadeald Rabbit (captured in game)'),
(9920416, 164944, 0, 0, 0, 1, 1, -7746.3003, 817.2704, 143.07234, 5.8737264, 120, 0, 0, 'Possessed Crow, perched (captured in game)'),
(9920417, 164944, 0, 0, 0, 1, 1, -7785.817, 569.17334, 140.21169, 2.577353, 120, 0, 0, 'Possessed Crow, perched in Dunshire (captured in game)'),
(9920418, 164124, 0, 0, 0, 1, 1, -7754.385, 657.2021, 138.98824, 5.5124664, 300, 0, 0, 'Dunshire Guard (captured in game)'),
(9920419, 164124, 0, 0, 0, 1, 1, -7746.1235, 651.2093, 138.98824, 2.6913116, 300, 0, 0, 'Dunshire Guard (captured in game)'),
(9920420, 164124, 0, 0, 0, 1, 1, -7785.1055, 614.60034, 136.71065, 0.30919698, 300, 0, 0, 'Dunshire Guard (captured in game)'),
(9920421, 164124, 0, 0, 0, 1, 1, -7782.752, 492.0177, 144.83365, 1.0840343, 300, 0, 0, 'Dunshire Guard, south road (captured in game)'),
(9920422, 164580, 0, 0, 0, 1, 1, -7789.7827, 491.978, 144.54425, 2.9204738, 120, 5, 1, 'Squirrel, south road (captured in game)'),
(9920423, 164571, 0, 0, 0, 1, 1, -7731.874, 569.5521, 141.78296, 5.394579, 120, 5, 1, 'Scadeald Rabbit, east of Dunshire (captured in game)'),
(9920424, 620, 0, 0, 0, 1, 1, -7775.0034, 541.4566, 137.53401, 5.147166, 120, 3, 1, 'Chicken, Dunshire (captured in game)'),
(9920425, 164580, 0, 0, 0, 1, 1, -7758.982, 528.0936, 143.33397, 5.740141, 120, 5, 1, 'Squirrel, south-east of Dunshire (captured in game)'),
(9920426, 164926, 0, 0, 0, 1, 1, -7678.9707, 555.05414, 137.54062, 1.9639443, 300, 0, 0, 'Ealdir Ambusher, female, east of Dunshire (captured in game)'),
(9920427, 164926, 0, 0, 0, 1, 1, -7673.1006, 554.57654, 137.8944, 1.6804214, 300, 0, 0, 'Ealdir Ambusher, female, east of Dunshire (captured in game)'),
(9920428, 164116, 0, 0, 0, 1, 1, -7796.0576, 597.5543, 136.57681, 2.081839, 300, 0, 0, 'Ulira Greyvale (captured in game)'),
(9920429, 164036, 0, 0, 0, 1, 1, -7772.1997, 601.4853, 138.2803, 3.9620538, 300, 0, 0, 'Innkeeper Miralda (captured in game)'),
(9920430, 164119, 0, 0, 0, 1, 1, -7777.361, 599.1155, 138.96074, 4.506335, 300, 0, 0, 'Yorin Hollowbrook, sitting (captured in game)'),
(9920431, 164034, 0, 0, 0, 1, 1, -7795.688, 535.22626, 137.6808, 2.4415216, 300, 0, 0, 'Hestwin Honeycrust (captured in game)'),
(9920432, 164037, 0, 0, 0, 1, 1, -7745.6196, 498.62296, 144.51157, 1.8399191, 300, 0, 0, 'Darond, the Dunshire porter (captured in game)'),
(9920433, 164117, 0, 0, 0, 1, 1, -7798.242, 617.6581, 136.97688, 2.6811025, 300, 0, 0, 'Aldwin Greymoor, by the chicken coops (captured in game)'),
(9920434, 164172, 0, 0, 0, 1, 1, -7801.7725, 537.171, 137.68141, 0.32646108, 300, 0, 0, 'Geralda, assistant baker (captured in game)'),
(9920435, 164120, 0, 0, 0, 1, 1, -7811.114, 581.91473, 136.04013, 5.128385, 300, 0, 0, 'Helara Nublar, weaponsmith (captured in game)'),
(9920436, 164122, 0, 0, 0, 1, 1, -7777.651, 587.8279, 136.87245, 3.4719121, 300, 0, 0, 'Rylan, stable master (captured in game)'),
(9920437, 620, 0, 0, 0, 1, 1, -7815.6263, 545.4382, 136.3579, 1.2, 120, 0, 0, 'Chicken, in the chicken box (captured in game)'),
(9920438, 620, 0, 0, 0, 1, 1, -7815.6389, 546.4396, 136.3574, 4.1, 120, 0, 0, 'Chicken, in the chicken box (captured in game)'),
(9920439, 164035, 0, 0, 0, 1, 1, -7832.8516, 592.3018, 136.0996, 0.32331982, 300, 0, 0, 'Miller Bartren, Dunshire mill (captured in game)'),
(9920440, 164004, 0, 0, 0, 1, 1, -7795.2705, 773.5789, 96.51171, 5.4456787, 300, 0, 0, 'Wild Ealdir (captured in game)'),
(9920441, 164004, 0, 0, 0, 1, 1, -7816.775, 810.82684, 92.60332, 4.58802, 300, 0, 0, 'Wild Ealdir (captured in game)'),
(9920442, 164004, 0, 0, 0, 1, 1, -7832.3735, 820.15784, 92.77728, 5.6624427, 300, 0, 0, 'Wild Ealdir (captured in game)'),
(9920443, 164004, 0, 0, 0, 1, 1, -7804.316, 834.2209, 93.29082, 4.2856493, 300, 0, 0, 'Wild Ealdir (captured in game)'),
(9920444, 164004, 0, 0, 0, 1, 1, -7855.801, 803.1483, 92.667305, 1.8092895, 300, 0, 0, 'Wild Ealdir (captured in game)'),
(9920445, 164004, 0, 0, 0, 1, 1, -7749.349, 889.80164, 111.470695, 5.4833803, 300, 0, 0, 'Wild Ealdir (captured in game)'),
(9920446, 164004, 0, 0, 0, 1, 1, -7731.663, 898.6018, 111.62887, 2.376347, 300, 0, 0, 'Wild Ealdir (captured in game)'),
(9920447, 164004, 0, 0, 0, 1, 1, -7746.5864, 878.2058, 113.69305, 0.33902198, 300, 0, 0, 'Wild Ealdir (captured in game)'),
(9920448, 164004, 0, 0, 0, 1, 1, -7744.243, 870.74066, 114.04001, 1.1848978, 300, 0, 0, 'Wild Ealdir (captured in game)'),
(9920449, 164004, 0, 0, 0, 1, 1, -7737.63, 871.74146, 114.52851, 1.9679414, 300, 0, 0, 'Wild Ealdir (captured in game)'),
(9920450, 164004, 0, 0, 0, 1, 1, -7727.9336, 924.15375, 112.551346, 4.7686815, 300, 0, 0, 'Wild Ealdir (captured in game)'),
(9920451, 164004, 0, 0, 0, 1, 1, -7732.5146, 927.4217, 112.76372, 4.2495313, 300, 0, 0, 'Wild Ealdir (captured in game)'),
(9920452, 164004, 0, 0, 0, 1, 1, -7754.7295, 916.7695, 111.63754, 5.7197967, 300, 0, 0, 'Wild Ealdir (captured in game)'),
(9920453, 164004, 0, 0, 0, 1, 1, -7757.3286, 912.724, 111.92561, 6.044947, 300, 0, 0, 'Wild Ealdir, sleeping (captured in game)'),
(9920454, 164004, 0, 0, 0, 1, 1, -7867.0063, 834.2622, 93.071495, 0, 300, 0, 2, 'Wild Ealdir, patrol (captured in game)'),
(9920455, 164004, 0, 0, 0, 1, 1, -7799.2812, 860.1844, 94.073235, 0, 300, 0, 2, 'Wild Ealdir, patrol (captured in game)'),
(9920456, 164004, 0, 0, 0, 1, 1, -7719.6104, 885.1427, 114.28041, 0, 300, 0, 2, 'Wild Ealdir, patrol (captured in game)'),
(9920457, 164124, 0, 0, 0, 1, 1, -7718.4854, 698.7057, 136.7058, 0, 300, 0, 2, 'Dunshire Guard, patrol (captured in game)');
DELETE FROM `creature_addon` WHERE `guid` BETWEEN 9920400 AND 9920499;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `visibilityDistanceType`, `auras`) VALUES
(9920408, 0, 0, 1, 1, 0, 0, ''),
(9920410, 0, 0, 1, 1, 0, 0, ''),
(9920416, 0, 0, 1, 1, 0, 0, ''),
(9920417, 0, 0, 1, 1, 0, 0, ''),
(9920430, 0, 0, 1, 1, 0, 0, ''),
(9920453, 0, 0, 3, 1, 0, 0, ''),
(9920454, 99204540, 0, 0, 1, 0, 0, ''),
(9920455, 99204550, 0, 0, 1, 0, 0, ''),
(9920456, 99204560, 0, 0, 1, 0, 0, ''),
(9920457, 99204570, 0, 0, 1, 0, 0, '');
DELETE FROM `waypoint_data` WHERE `id` BETWEEN 99204000 AND 99204990;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(99204540, 1, -7867.0063, 834.2622, 93.071495, 0, 3000, 0, 0, 100, 0),
(99204540, 2, -7807.638, 832.73846, 93.31855, 0, 3000, 0, 0, 100, 0),
(99204550, 1, -7799.2812, 860.1844, 94.073235, 0, 3000, 0, 0, 100, 0),
(99204550, 2, -7763.073, 886.0415, 109.91479, 0, 3000, 0, 0, 100, 0),
(99204560, 1, -7719.6104, 885.1427, 114.28041, 0, 3000, 0, 0, 100, 0),
(99204560, 2, -7679.1836, 870.33044, 139.54771, 0, 3000, 0, 0, 100, 0),
(99204570, 1, -7718.4854, 698.7057, 136.7058, 0, 3000, 0, 0, 100, 0),
(99204570, 2, -7773.2607, 621.82947, 136.79907, 0, 3000, 0, 0, 100, 0);
DELETE FROM `gameobject` WHERE `guid` BETWEEN 9920400 AND 9920499;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `Comment`) VALUES
(9920400, 2300631, 0, 0, 0, 1, 1, -7798.016, 775.98883, 96.96318, 5.374205, 0, 0, 0.439004, -0.898485, 60, 100, 1, 'Weapon Crate, well cave (captured in game)'),
(9920401, 1731, 0, 0, 0, 1, 1, -7814.8066, 841.43896, 47.87271, 4.6045384, 0, 0, 0.744191, -0.667966, 900, 100, 1, 'Copper Vein (captured in game)'),
(9920402, 1731, 0, 0, 0, 1, 1, -7841.613, 829.74164, 92.7602, 5.1590056, 0, 0, 0.532956, -0.846143, 900, 100, 1, 'Copper Vein (captured in game)'),
(9920403, 1731, 0, 0, 0, 1, 1, -7783.8335, 734.16235, 97.083534, 4.094799, 0, 0, 0.888558, -0.458764, 900, 100, 1, 'Copper Vein (captured in game)'),
(9920404, 1732, 0, 0, 0, 1, 1, -7775.232, 780.06976, 96.21515, 1.0278132, 0, 0, 0.491583, 0.870831, 900, 100, 1, 'Tin Vein, well cave (captured in game)'),
(9920405, 1619, 0, 0, 0, 1, 1, -7776.553, 756.5266, 152.00365, 4.5848913, 0, 0, 0.750717, -0.660624, 600, 100, 1, 'Earthroot (captured in game)'),
(9920406, 1619, 0, 0, 0, 1, 1, -7738.6865, 580.2143, 139.37424, 3.588167, 0, 0, 0.975175, -0.221436, 600, 100, 1, 'Earthroot, east of Dunshire (captured in game)'),
(9920407, 1732, 0, 0, 0, 1, 1, -7672.591, 552.54706, 138.31294, 3.6509843, 0, 0, 0.96774, -0.251951, 600, 100, 1, 'Tin Vein, east of Dunshire (captured in game)'),
(9920408, 1731, 0, 0, 0, 1, 1, -7737.549, 720.0509, 141.14828, 1.2955716, 0, 0, 0.603422, 0.797422, 600, 100, 1, 'Copper Vein, north-east of Dunshire (captured in game)'),
(9920409, 1731, 0, 0, 0, 1, 1, -7728.0547, 485.46466, 149.37198, 4.752964, 0, 0, 0.692617, -0.721306, 900, 100, 1, 'Copper Vein, near Darond (captured in game)'),
(9920410, 2300641, 0, 0, 0, 1, 1, -7835.2134, 596.66223, 136.0996, 2.3402207, 0, 0, 0.920794, 0.39005, 60, 100, 1, 'Dunshire Mill, millstone inside the mill (captured in game)'),
(9920411, 2300644, 0, 0, 0, 1, 1, -7801.985, 497.81995, 143.57635, 0.3931511, 0, 0, 0.195312, 0.980741, 30, 100, 1, 'Elven Medallion of the Sanctuary (captured in game)'),
(9920412, 2300643, 0, 0, 0, 1, 1, -7771.264, 599.018, 139.49002, 4.317084, 0, 0, 0.832193, -0.554486, 30, 100, 1, 'Kisswind Cow Milk, Dunshire tavern (captured in game)'),
(9920413, 2300643, 0, 0, 0, 1, 1, -7770.826, 599.2558, 139.493, 5.580011, 0, 0, 0.344388, -0.938827, 30, 100, 1, 'Kisswind Cow Milk, Dunshire tavern (captured in game)'),
(9920414, 2300643, 0, 0, 0, 1, 1, -7771.1543, 599.5339, 139.48853, 5.580011, 0, 0, 0.344388, -0.938827, 30, 100, 1, 'Kisswind Cow Milk, Dunshire tavern (captured in game)'),
(9920415, 2300642, 0, 0, 0, 1, 1, -7813.1131, 545.7727, 136.3595, 0.0, 0, 0, 0.0, 1.0, 30, 100, 1, 'Dunshire Eggs, chicken box south-west of the mill (captured in game)'),
(9920416, 2300642, 0, 0, 0, 1, 1, -7814.4474, 544.2118, 136.3594, 0.79, 0, 0, 0.384808, 0.922997, 30, 100, 1, 'Dunshire Eggs, chicken box south-west of the mill (captured in game)'),
(9920417, 2300642, 0, 0, 0, 1, 1, -7815.7818, 542.6509, 136.3592, 1.58, 0, 0, 0.710353, 0.703845, 30, 100, 1, 'Dunshire Eggs, chicken box south-west of the mill (captured in game)'),
(9920418, 2300642, 0, 0, 0, 1, 1, -7814.4294, 546.9141, 136.358, 2.37, 0, 0, 0.926499, 0.376297, 30, 100, 1, 'Dunshire Eggs, chicken box south-west of the mill (captured in game)'),
(9920419, 2300642, 0, 0, 0, 1, 1, -7816.8232, 543.9623, 136.3577, 3.16, 0, 0, 0.999958, -0.009204, 30, 100, 1, 'Dunshire Eggs, chicken box south-west of the mill (captured in game)'),
(9920420, 2300642, 0, 0, 0, 1, 1, -7815.7457, 548.0554, 136.3565, 3.95, 0, 0, 0.919416, -0.393287, 30, 100, 1, 'Dunshire Eggs, chicken box south-west of the mill (captured in game)'),
(9920421, 2300642, 0, 0, 0, 1, 1, -7816.8052, 546.6646, 136.3563, 4.74, 0, 0, 0.697278, -0.716801, 30, 100, 1, 'Dunshire Eggs, chicken box south-west of the mill (captured in game)'),
(9920422, 2300642, 0, 0, 0, 1, 1, -7817.8647, 545.2737, 136.3562, 5.53, 0, 0, 0.367754, -0.929923, 30, 100, 1, 'Dunshire Eggs, chicken box south-west of the mill (captured in game)'),
(9920423, 2300640, 0, 0, 0, 1, 1, -7798.349, 560.182, 136.429, 2.7817, 0, 0, 0.983853, 0.178977, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920424, 2300640, 0, 0, 0, 1, 1, -7796.867, 562.225, 136.433, 1.9892, 0, 0, 0.838541, 0.544838, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920425, 2300640, 0, 0, 0, 1, 1, -7795.386, 564.268, 136.437, 2.6419, 0, 0, 0.96895, 0.247255, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920426, 2300640, 0, 0, 0, 1, 1, -7793.905, 566.311, 136.441, 1.5128, 0, 0, 0.686308, 0.727311, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920427, 2300640, 0, 0, 0, 1, 1, -7792.424, 568.354, 136.445, 1.9846, 0, 0, 0.837286, 0.546766, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920428, 2300640, 0, 0, 0, 1, 1, -7790.942, 570.396, 136.449, 5.7554, 0, 0, 0.26084, -0.965382, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920429, 2300640, 0, 0, 0, 1, 1, -7789.461, 572.439, 136.453, 5.5635, 0, 0, 0.352127, -0.935952, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920430, 2300640, 0, 0, 0, 1, 1, -7787.98, 574.482, 136.456, 3.9132, 0, 0, 0.926496, -0.376304, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920431, 2300640, 0, 0, 0, 1, 1, -7796.407, 558.718, 136.444, 1.6242, 0, 0, 0.725734, 0.687976, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920432, 2300640, 0, 0, 0, 1, 1, -7794.92, 560.761, 136.449, 0.0313, 0, 0, 0.015649, 0.999878, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920433, 2300640, 0, 0, 0, 1, 1, -7793.433, 562.805, 136.454, 1.1154, 0, 0, 0.529236, 0.848475, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920434, 2300640, 0, 0, 0, 1, 1, -7791.946, 564.848, 136.459, 2.8773, 0, 0, 0.991281, 0.131762, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920435, 2300640, 0, 0, 0, 1, 1, -7790.458, 566.892, 136.464, 6.0326, 0, 0, 0.124965, -0.992161, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920436, 2300640, 0, 0, 0, 1, 1, -7788.971, 568.935, 136.469, 1.0954, 0, 0, 0.520725, 0.853724, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920437, 2300640, 0, 0, 0, 1, 1, -7787.484, 570.978, 136.474, 3.5463, 0, 0, 0.979596, -0.200976, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920438, 2300640, 0, 0, 0, 1, 1, -7785.997, 573.022, 136.479, 1.6938, 0, 0, 0.749231, 0.662309, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920439, 2300640, 0, 0, 0, 1, 1, -7794.466, 557.254, 136.458, 1.6714, 0, 0, 0.741766, 0.670659, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920440, 2300640, 0, 0, 0, 1, 1, -7792.973, 559.298, 136.464, 5.4996, 0, 0, 0.381846, -0.924226, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920441, 2300640, 0, 0, 0, 1, 1, -7791.48, 561.342, 136.47, 2.478, 0, 0, 0.945459, 0.325742, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920442, 2300640, 0, 0, 0, 1, 1, -7789.986, 563.386, 136.477, 0.4196, 0, 0, 0.208264, 0.978073, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920443, 2300640, 0, 0, 0, 1, 1, -7788.493, 565.43, 136.483, 0.3232, 0, 0, 0.160898, 0.986971, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920444, 2300640, 0, 0, 0, 1, 1, -7787.0, 567.473, 136.489, 2.9986, 0, 0, 0.997445, 0.071435, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920445, 2300640, 0, 0, 0, 1, 1, -7785.507, 569.517, 136.495, 0.5885, 0, 0, 0.290022, 0.95702, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920446, 2300640, 0, 0, 0, 1, 1, -7784.014, 571.561, 136.501, 0.4669, 0, 0, 0.231335, 0.972874, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920447, 2300640, 0, 0, 0, 1, 1, -7792.525, 555.79, 136.473, 0.4168, 0, 0, 0.206895, 0.978363, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920448, 2300640, 0, 0, 0, 1, 1, -7791.026, 557.835, 136.48, 2.0433, 0, 0, 0.85297, 0.521959, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920449, 2300640, 0, 0, 0, 1, 1, -7789.526, 559.879, 136.487, 4.1697, 0, 0, 0.870758, -0.491711, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920450, 2300640, 0, 0, 0, 1, 1, -7788.027, 561.923, 136.495, 2.2874, 0, 0, 0.910172, 0.41423, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920451, 2300640, 0, 0, 0, 1, 1, -7786.528, 563.968, 136.502, 0.9721, 0, 0, 0.467137, 0.884185, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920452, 2300640, 0, 0, 0, 1, 1, -7785.029, 566.012, 136.509, 4.187, 0, 0, 0.866473, -0.499225, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920453, 2300640, 0, 0, 0, 1, 1, -7783.53, 568.056, 136.516, 3.1344, 0, 0, 0.999994, 0.003596, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920454, 2300640, 0, 0, 0, 1, 1, -7782.031, 570.1, 136.523, 0.0475, 0, 0, 0.023748, 0.999718, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920455, 2300640, 0, 0, 0, 1, 1, -7790.583, 554.326, 136.488, 3.3498, 0, 0, 0.994586, -0.103916, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920456, 2300640, 0, 0, 0, 1, 1, -7789.078, 556.371, 136.496, 6.2556, 0, 0, 0.013792, -0.999905, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920457, 2300640, 0, 0, 0, 1, 1, -7787.573, 558.416, 136.504, 5.2948, 0, 0, 0.474321, -0.880352, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920458, 2300640, 0, 0, 0, 1, 1, -7786.068, 560.461, 136.512, 5.9358, 0, 0, 0.172821, -0.984953, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920459, 2300640, 0, 0, 0, 1, 1, -7784.563, 562.506, 136.521, 0.1317, 0, 0, 0.065802, 0.997833, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920460, 2300640, 0, 0, 0, 1, 1, -7783.058, 564.55, 136.529, 5.1637, 0, 0, 0.530968, -0.847392, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920461, 2300640, 0, 0, 0, 1, 1, -7781.553, 566.595, 136.537, 1.4244, 0, 0, 0.653501, 0.756926, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920462, 2300640, 0, 0, 0, 1, 1, -7780.048, 568.64, 136.546, 3.6482, 0, 0, 0.96809, -0.250604, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920463, 2300640, 0, 0, 0, 1, 1, -7788.642, 552.863, 136.502, 2.3753, 0, 0, 0.927493, 0.373841, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920464, 2300640, 0, 0, 0, 1, 1, -7787.131, 554.908, 136.512, 4.1856, 0, 0, 0.866822, -0.498618, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920465, 2300640, 0, 0, 0, 1, 1, -7785.62, 556.953, 136.521, 2.6258, 0, 0, 0.966929, 0.255047, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920466, 2300640, 0, 0, 0, 1, 1, -7784.109, 558.998, 136.53, 2.6457, 0, 0, 0.969418, 0.245414, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920467, 2300640, 0, 0, 0, 1, 1, -7782.598, 561.044, 136.54, 1.7929, 0, 0, 0.781115, 0.624387, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920468, 2300640, 0, 0, 0, 1, 1, -7781.087, 563.089, 136.549, 3.7786, 0, 0, 0.949705, -0.313146, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920469, 2300640, 0, 0, 0, 1, 1, -7779.576, 565.134, 136.559, 4.5067, 0, 0, 0.775964, -0.630777, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920470, 2300640, 0, 0, 0, 1, 1, -7778.064, 567.179, 136.568, 3.8252, 0, 0, 0.942152, -0.335187, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920471, 2300640, 0, 0, 0, 1, 1, -7786.701, 551.399, 136.517, 1.2727, 0, 0, 0.594264, 0.80427, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920472, 2300640, 0, 0, 0, 1, 1, -7785.184, 553.444, 136.527, 2.0385, 0, 0, 0.851715, 0.524005, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920473, 2300640, 0, 0, 0, 1, 1, -7783.667, 555.49, 136.538, 0.5711, 0, 0, 0.281685, 0.959507, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920474, 2300640, 0, 0, 0, 1, 1, -7782.15, 557.536, 136.548, 3.8296, 0, 0, 0.941412, -0.337259, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920475, 2300640, 0, 0, 0, 1, 1, -7780.633, 559.582, 136.559, 1.1715, 0, 0, 0.552825, 0.833298, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920476, 2300640, 0, 0, 0, 1, 1, -7779.115, 561.627, 136.569, 0.0256, 0, 0, 0.0128, 0.999918, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920477, 2300640, 0, 0, 0, 1, 1, -7777.598, 563.673, 136.58, 1.0795, 0, 0, 0.513922, 0.857837, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920478, 2300640, 0, 0, 0, 1, 1, -7776.081, 565.719, 136.59, 4.1088, 0, 0, 0.885325, -0.464973, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920479, 2300640, 0, 0, 0, 1, 1, -7784.759, 549.935, 136.532, 2.9229, 0, 0, 0.994028, 0.109129, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920480, 2300640, 0, 0, 0, 1, 1, -7783.236, 551.981, 136.543, 5.2932, 0, 0, 0.475025, -0.879972, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920481, 2300640, 0, 0, 0, 1, 1, -7781.713, 554.027, 136.555, 0.882, 0, 0, 0.426844, 0.904325, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920482, 2300640, 0, 0, 0, 1, 1, -7780.19, 556.073, 136.566, 0.4123, 0, 0, 0.204693, 0.978826, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920483, 2300640, 0, 0, 0, 1, 1, -7778.667, 558.12, 136.578, 3.7416, 0, 0, 0.955335, -0.295524, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920484, 2300640, 0, 0, 0, 1, 1, -7777.144, 560.166, 136.589, 2.566, 0, 0, 0.958872, 0.28384, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920485, 2300640, 0, 0, 0, 1, 1, -7775.621, 562.212, 136.601, 5.28, 0, 0, 0.480823, -0.876818, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)'),
(9920486, 2300640, 0, 0, 0, 1, 1, -7774.098, 564.258, 136.612, 3.7462, 0, 0, 0.954653, -0.29772, 60, 100, 1, 'Dunshire Wheat, crop field (captured in game)');
