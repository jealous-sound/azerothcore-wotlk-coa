-- Northshire Abbey chain (1660000-1660005), from Questie-X-AscensionDB.
-- Positions converted from zone-12 (Elwynn Forest) percent coords with the
-- same validated formula used for the Goldshire corrections. Z is not
-- sampled (approximate); GO/creature models are best-effort placeholders
-- where the addon carries no display id.

UPDATE `creature_template` SET `minlevel`=55, `maxlevel`=55 WHERE `entry`=161702;

DELETE FROM `creature_template` WHERE `entry` IN (161705, 161716, 161736);
INSERT INTO `creature_template`
  (`entry`, `name`, `subname`, `minlevel`, `maxlevel`, `faction`, `npcflag`,
   `speed_walk`, `speed_run`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`,
   `unit_class`, `unit_flags`, `dynamicflags`, `family`, `type`, `type_flags`,
   `HealthModifier`, `ManaModifier`, `ArmorModifier`, `ExperienceModifier`,
   `RegenHealth`, `flags_extra`, `AIName`, `MovementType`, `VerifiedBuild`)
VALUES
  (161705, 'Injured Northshire Guard', '', 17, 17, 72, 3, 1, 1.14286, 0, 0, 2000, 2000, 1, 0, 0, 0, 7, 0, 1, 1, 1, 1, 1, 0, '', 0, 12340),
  (161716, 'Shadewell Murloc', '', 3, 3, 25, 0, 1, 1.14286, 0, 0, 2000, 2000, 1, 0, 0, 6, 8, 0, 1, 1, 1, 1, 1, 0, '', 1, 12340),
  (161736, 'Defias Plunderer', '', 3, 3, 25, 0, 1, 1.14286, 0, 0, 2000, 2000, 1, 0, 0, 0, 7, 0, 1, 1, 1, 1, 1, 0, '', 1, 12340);

DELETE FROM `creature_template_model` WHERE `CreatureID` IN (161705, 161716, 161736);
DELETE FROM `creature_model_info` WHERE `DisplayID` IN (510, 100, 134);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`, `DisplayID_Other_Gender`, `VerifiedBuild`) VALUES
  (510, 0.383, 1.5, 0, 0, 12340), (100, 0.383, 1.5, 0, 0, 12340), (134, 0.383, 1.5, 0, 0, 12340);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES
  (161705, 0, 510, 1, 1, 12340),
  (161716, 0, 100, 1, 1, 12340),
  (161736, 0, 100, 1, 1, 12340);

DELETE FROM `creature` WHERE `id` IN (161705, 161716, 161736);
INSERT INTO `creature`
  (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `MovementType`)
VALUES
  (9780200, 161705, 0, 1, 1, -8853.42, -370.03, 60, 0, 300, 0),
  (9780201, 161716, 0, 1, 1, -8676.79, -481.42, 60, 0, 300, 1),
  (9780202, 161716, 0, 1, 1, -8681.65, -474.82, 60, 0, 300, 1),
  (9780203, 161716, 0, 1, 1, -8701.33, -452.96, 60, 0, 300, 1),
  (9780204, 161716, 0, 1, 1, -8683.97, -451.23, 60, 0, 300, 1),
  (9780205, 161716, 0, 1, 1, -8669.15, -468.58, 60, 0, 300, 1),
  (9780206, 161716, 0, 1, 1, -8658.97, -497.38, 60, 0, 300, 1),
  (9780207, 161716, 0, 1, 1, -8636.05, -489.05, 60, 0, 300, 1),
  (9780208, 161716, 0, 1, 1, -8615.91, -483.85, 60, 0, 300, 1),
  (9780209, 161716, 0, 1, 1, -8597.39, -490.09, 60, 0, 300, 1),
  (9780210, 161716, 0, 1, 1, -8603.87, -500.15, 60, 0, 300, 1),
  (9780211, 161716, 0, 1, 1, -8610.81, -505.01, 60, 0, 300, 1),
  (9780212, 161716, 0, 1, 1, -8587.89, -502.93, 60, 0, 300, 1),
  (9780213, 161716, 0, 1, 1, -8562.2, -520.63, 60, 0, 300, 1),
  (9780214, 161716, 0, 1, 1, -8545.99, -524.79, 60, 0, 300, 1),
  (9780215, 161716, 0, 1, 1, -8539.05, -511.61, 60, 0, 300, 1),
  (9780216, 161716, 0, 1, 1, -8536.96, -488.36, 60, 0, 300, 1),
  (9780217, 161716, 0, 1, 1, -8521.22, -488.36, 60, 0, 300, 1),
  (9780218, 161716, 0, 1, 1, -8531.87, -466.5, 60, 0, 300, 1),
  (9780219, 161716, 0, 1, 1, -8545.53, -471.35, 60, 0, 300, 1),
  (9780220, 161736, 0, 1, 1, -8835.83, -395.01, 60, 0, 300, 1),
  (9780221, 161736, 0, 1, 1, -8832.82, -383.22, 60, 0, 300, 1),
  (9780222, 161736, 0, 1, 1, -8824.02, -367.6, 60, 0, 300, 1),
  (9780223, 161736, 0, 1, 1, -8790.92, -388.42, 60, 0, 300, 1),
  (9780224, 161736, 0, 1, 1, -8806.89, -411.67, 60, 0, 300, 1),
  (9780225, 161736, 0, 1, 1, -8811.99, -381.13, 60, 0, 300, 1),
  (9780226, 161736, 0, 1, 1, -8810.13, -391.54, 60, 0, 300, 1),
  (9780227, 161736, 0, 1, 1, -8823.33, -396.4, 60, 0, 300, 1),
  (9780228, 161736, 0, 1, 1, -8807.36, -396.75, 60, 0, 300, 1),
  (9780229, 161736, 0, 1, 1, -8808.05, -393.97, 60, 0, 300, 1),
  (9780230, 161736, 0, 1, 1, -8819.86, -396.4, 60, 0, 300, 1),
  (9780231, 161736, 0, 1, 1, -8785.83, -409.24, 60, 0, 300, 1);

DELETE FROM `gameobject_template` WHERE `entry` IN (900001,900002,900003,900004,999005,999006,999007,999008,999010,999011,999012,999013,999014,999015);
INSERT INTO `gameobject_template`
  (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`,
   `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`,
   `AIName`, `ScriptName`, `VerifiedBuild`)
VALUES
  (900001, 3, 107, 'Lost Page I', '', '', '', 1, 0, 900001, 0, 1, 0, 0, 0, 0, '', '', 12340),
  (900002, 3, 107, 'Lost Page II', '', '', '', 1, 0, 900002, 0, 1, 0, 0, 0, 0, '', '', 12340),
  (900003, 3, 107, 'Lost Page III', '', '', '', 1, 0, 900003, 0, 1, 0, 0, 0, 0, '', '', 12340),
  (900004, 3, 107, 'Lost Page IV', '', '', '', 1, 0, 900004, 0, 1, 0, 0, 0, 0, '', '', 12340),
  (999005, 10, 52, 'Abbess'' Journal Purified', '', '', '', 1, 0, 1660003, 0, 0, 0, 1, 0, 0, '', '', 12340),
  (999006, 10, 52, 'Abbess''s Staff Purified', '', '', '', 1, 0, 1660003, 0, 0, 0, 1, 0, 0, '', '', 12340),
  (999007, 10, 52, 'Heretical Idol Purified', '', '', '', 1, 0, 1660003, 0, 0, 0, 1, 0, 0, '', '', 12340),
  (999008, 10, 52, 'Jeweled Relic Purified', '', '', '', 1, 0, 1660003, 0, 0, 0, 1, 0, 0, '', '', 12340),
  (999010, 10, 52, 'Hidden Path', '', '', '', 1, 0, 1660004, 0, 0, 0, 1, 0, 0, '', '', 12340),
  (999011, 10, 52, 'Ruined Estate', '', '', '', 1, 0, 1660004, 0, 0, 0, 1, 0, 0, '', '', 12340),
  (999012, 10, 52, 'Wayward Theologian', '', '', '', 1, 0, 1660004, 0, 0, 0, 1, 0, 0, '', '', 12340),
  (999013, 3, 1655, 'Melon', '', '', '', 1, 0, 999013, 0, 1, 0, 0, 0, 0, '', '', 12340),
  (999014, 3, 1616, 'Pumpkin', '', '', '', 1, 0, 999014, 0, 1, 0, 0, 0, 0, '', '', 12340),
  (999015, 3, 2698, 'Apple', '', '', '', 1, 0, 999015, 0, 1, 0, 0, 0, 0, '', '', 12340);

DELETE FROM `gameobject_loot_template` WHERE `Entry` IN (900001,900002,900003,900004,999013,999014,999015);
INSERT INTO `gameobject_loot_template` (`Entry`,`Item`,`Reference`,`Chance`,`QuestRequired`,`GroupId`,`MinCount`,`MaxCount`) VALUES
  (900001, 559130, 0, 100, 1, 0, 1, 1),
  (900002, 559131, 0, 100, 1, 0, 1, 1),
  (900003, 559132, 0, 100, 1, 0, 1, 1),
  (900004, 559133, 0, 100, 1, 0, 1, 1),
  (999013, 558962, 0, 100, 0, 0, 1, 1),
  (999014, 558961, 0, 100, 0, 0, 1, 1),
  (999015, 558963, 0, 100, 0, 0, 1, 1);

DELETE FROM `gameobject` WHERE `id` IN (900001,900002,900003,900004,999005,999006,999007,999008,999010,999011,999012,999013,999014,999015);
INSERT INTO `gameobject`
  (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`,`rotation1`,`rotation2`,`rotation3`, `spawntimesecs`, `animprogress`, `state`)
VALUES
  (9780300, 900001, 0, 1, 1, -8912.69, -210.06, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780301, 900002, 0, 1, 1, -8855.74, -186.12, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780302, 900003, 0, 1, 1, -8883.52, -182.65, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780303, 900004, 0, 1, 1, -8858.06, -186.12, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780304, 999005, 0, 1, 1, -8575.62, -252.05, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780305, 999006, 0, 1, 1, -8638.13, -404.73, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780306, 999007, 0, 1, 1, -8619.61, -279.81, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780307, 999008, 0, 1, 1, -8658.97, -317.98, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780308, 999010, 0, 1, 1, -8816.39, -383.91, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780309, 999011, 0, 1, 1, -8709.9, -501.89, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780310, 999012, 0, 1, 1, -8603.41, -564.35, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780311, 999013, 0, 1, 1, -9505.79, 93.56, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780312, 999013, 0, 1, 1, -9497.23, 92.17, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780313, 999013, 0, 1, 1, -9502.78, 91.83, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780314, 999013, 0, 1, 1, -9498.38, 95.64, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780315, 999013, 0, 1, 1, -9501.39, 99.81, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780316, 999013, 0, 1, 1, -9503.25, 102.24, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780317, 999013, 0, 1, 1, -9499.77, 102.24, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780318, 999013, 0, 1, 1, -9496.53, 98.42, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780319, 999013, 0, 1, 1, -9495.84, 101.54, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780320, 999013, 0, 1, 1, -9502.09, 108.48, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780321, 999014, 0, 1, 1, -9504.87, 75.87, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780322, 999014, 0, 1, 1, -9502.32, 69.97, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780323, 999014, 0, 1, 1, -9505.33, 68.92, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780324, 999015, 0, 1, 1, -9439.81, -28.93, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780325, 999015, 0, 1, 1, -9442.82, -29.62, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780326, 999015, 0, 1, 1, -9443.29, -27.54, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780327, 999015, 0, 1, 1, -9448.84, -27.89, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780328, 999015, 0, 1, 1, -9447.69, -30.32, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780329, 999015, 0, 1, 1, -9444.21, -33.79, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780330, 999015, 0, 1, 1, -9442.13, -33.09, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780331, 999015, 0, 1, 1, -9441.67, -35.52, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780332, 999015, 0, 1, 1, -9441.9, -42.46, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780333, 999015, 0, 1, 1, -9444.21, -44.2, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780334, 999015, 0, 1, 1, -9441.9, -45.58, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780335, 999015, 0, 1, 1, -9445.6, -46.28, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780336, 999015, 0, 1, 1, -9446.99, -44.2, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780337, 999015, 0, 1, 1, -9448.15, -45.58, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780338, 999015, 0, 1, 1, -9448.38, -40.03, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780339, 999015, 0, 1, 1, -9447.92, -38.99, 60, 0, 0, 0, 0, 1, 300, 0, 1),
  (9780340, 999015, 0, 1, 1, -9446.76, -37.6, 60, 0, 0, 0, 0, 1, 300, 0, 1);

DELETE FROM `quest_template` WHERE `ID` IN (1660000,1660001,1660002,1660003,1660004,1660005);
INSERT INTO `quest_template`
  (`ID`, `QuestType`, `QuestLevel`, `MinLevel`, `QuestSortID`, `RewardXPDifficulty`,
   `RewardMoney`, `RewardMoneyDifficulty`, `RequiredNpcOrGo1`, `RequiredNpcOrGoCount1`,
   `RequiredNpcOrGo2`, `RequiredNpcOrGoCount2`, `RequiredNpcOrGo3`, `RequiredNpcOrGoCount3`, `RequiredNpcOrGo4`, `RequiredNpcOrGoCount4`,
   `RequiredItemId1`, `RequiredItemCount1`, `RequiredItemId2`, `RequiredItemCount2`,
   `RequiredItemId3`, `RequiredItemCount3`, `RequiredItemId4`, `RequiredItemCount4`,
   `LogTitle`, `LogDescription`, `QuestCompletionLog`, `ObjectiveText1`)
VALUES
  (1660000, 2, 6, 3, 9, 3,
   0, 0, 0,0, 0,0, 0,0, 0,0,
   0,0,0,0,0,0,0,0,
   'Bookworm', 'Speak with Seminarian Moroi, brother of Bianca Spada.', 'Speak with Seminarian Moroi.', 'Speak with Seminarian Moroi, brother of Bianca Spada.'),

  (1660001, 2, 6, 3, 9, 3,
   0, 0, 0,0, 0,0, 0,0, 0,0,
   559130,1, 559131,1, 559132,1, 559133,1,
   'Knowledge Corrupts', 'Recover the missing pages from Moroi''s unclassified manuscript, scattered throughout the abbey.', 'Return the pages to Moroi.', 'Recover the missing manuscript pages.'),

  (1660002, 2, 6, 3, 9, 3,
   0, 0, 0,0, 0,0, 0,0, 0,0,
   0,0,0,0,0,0,0,0,
   'The Ruins of Northshire', 'Locate the cellar entrance to the Secret Inquisitorial Dungeon.', 'Speak with Sister Alma.', 'Locate the cellar entrance to the Secret Inquisitorial Dungeon.'),

  (1660003, 2, 6, 3, 9, 3,
   0, 0, -999005,1, -999006,1, -999007,1, -999008,1,
   0,0,0,0,0,0,0,0,
   'Accursed Sisterhood', 'Purify the belongings of the former abbess, scattered throughout the Secret Inquisitorial Dungeon.', 'Return to Sister Alma.', 'Purify the abbess'' belongings.'),

  (1660004, 2, 6, 3, 9, 3,
   0, 0, -999010,1, -999011,1, -999012,1, 0,0,
   0,0,0,0,0,0,0,0,
   'Words That Shepherd Madness', 'Find the hidden path leading to the top of the waterfall and confront the sins of the former abbess of Northshire.', 'Return to Moroi.', 'Confront the sins of the former abbess.'),

  (1660005, 2, 6, 3, 9, 3,
   0, 0, 161736,8, 161716,8, 0,0, 0,0,
   0,0,0,0,0,0,0,0,
   'The Threat Swept Downstream', 'Defeat the Defias lurking in the ruined tower and thin the ranks of the Shadewell murlocs atop the waterfall, across the rope bridge.', 'Return to the Injured Northshire Guard.', 'Slay Defias Plunderers and Shadewell Murlocs.');

DELETE FROM `quest_template_addon` WHERE `ID` IN (1660000,1660001,1660002,1660003,1660004,1660005);
INSERT INTO `quest_template_addon` (`ID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`) VALUES
  (1660000, 0, 0, 0),
  (1660001, 1660000, 0, 0),
  (1660002, 1660001, 0, 0),
  (1660003, 1660002, 0, 0),
  (1660004, 1660003, 0, 0),
  (1660005, 1660003, 0, 0);

DELETE FROM `creature_queststarter` WHERE `quest` IN (1660000,1660001,1660002,1660003,1660004,1660005);
DELETE FROM `creature_questender` WHERE `quest` IN (1660000,1660001,1660002,1660003,1660004,1660005);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
  (161700, 1660000),
  (161701, 1660001),
  (161701, 1660002),
  (161702, 1660003),
  (161702, 1660004),
  (161705, 1660005);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
  (161701, 1660000),
  (161701, 1660001),
  (161702, 1660002),
  (161702, 1660003),
  (161701, 1660004),
  (161705, 1660005);

-- Fix: Bianca Spada (161700) and Moroi Spada (161701) pre-existed as
-- templates from earlier in the session but were never actually spawned
-- (no `creature` row at all) -- player found the quest chain unreachable
-- because of this. Positions from addon zone-% (Bianca: zone 12 Elwynn
-- 48.1,42.4; Moroi: "northshire" subzone 39.89,51.29, tighter bounding box
-- loc_left=187.5, loc_right=-781.25, loc_top=-8570.83, loc_bottom=-9216.67).
-- Z=81 is a rough guess (stock Northshire Abbey elevation), not sampled.
DELETE FROM `creature` WHERE `id` IN (161700, 161701);
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`MovementType`) VALUES
  (9780400, 161700, 0, 1, 1, -8920.56, -134.07, 81, 0, 300, 0),
  (9780401, 161701, 0, 1, 1, -8900.869, -198.85036, 81.94019, 4.040796, 300, 0);

-- Bianca Spada's periodic yell at Moroi, confirmed by player screenshot.
DELETE FROM `creature_text` WHERE `CreatureID` = 161700;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
  (161700, 0, 0, 'Moroi, I know you''re in there! We don''t have all day. Mother doesn''t either! Get your head out here, you scoundrel!', 1, 0, 100, 0, 0, 0, 0, 0, 'Bianca Spada periodic yell at Moroi');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=161700;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 161700 AND `source_type` = 0;
INSERT INTO `smart_scripts`
  (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`,
   `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `event_param6`,
   `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`,
   `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`,
   `target_x`, `target_y`, `target_z`, `target_o`, `comment`)
VALUES
  (161700, 0, 0, 0, 1, 0, 100, 0,
   30000, 60000, 30000, 60000, 0, 0,
   1, 0, 0, 0, 0, 0, 0,
   1, 0, 0, 0, 0,
   0,0,0,0, 'Bianca Spada - periodic yell at Moroi, every 30-60s');

-- Fix: Melon/Pumpkin/Apple (999013-999015) used made-up displayIds
-- (1655/1616/2698) that don't exist in the loaded client DBC -- server log:
-- "Gameobject ... has an invalid displayId, not loaded." Swapped to
-- confirmed-working stock harvest displays already used by real content.
UPDATE `gameobject_template` SET `displayId`=334 WHERE `entry`=999013; -- Melon (Barrel of Melon Juice display)
UPDATE `gameobject_template` SET `displayId`=60  WHERE `entry`=999014; -- Pumpkin (Ripe Pumpkin display)
UPDATE `gameobject_template` SET `displayId`=433 WHERE `entry`=999015; -- Apple (exact match, stock Apple object)

-- Player-supplied respawn timers (15-60s spread, not the default 300s) for
-- the Mirror Shard (162920) and Lost Page (900001-900004) gameobjects.
UPDATE `gameobject` SET `spawntimesecs`=35 WHERE `guid`=6960085;
UPDATE `gameobject` SET `spawntimesecs`=24 WHERE `guid`=6960086;
UPDATE `gameobject` SET `spawntimesecs`=40 WHERE `guid`=6960087;
UPDATE `gameobject` SET `spawntimesecs`=56 WHERE `guid`=6960088;
UPDATE `gameobject` SET `spawntimesecs`=18 WHERE `guid`=6960089;
UPDATE `gameobject` SET `spawntimesecs`=19 WHERE `guid`=6960090;
UPDATE `gameobject` SET `spawntimesecs`=49 WHERE `guid`=6960091;
UPDATE `gameobject` SET `spawntimesecs`=21 WHERE `guid`=6960092;
UPDATE `gameobject` SET `spawntimesecs`=38 WHERE `guid`=6960093;
UPDATE `gameobject` SET `spawntimesecs`=52 WHERE `guid`=6960094;
UPDATE `gameobject` SET `spawntimesecs`=18 WHERE `guid`=6960095;
UPDATE `gameobject` SET `spawntimesecs`=47 WHERE `guid`=6960096;
UPDATE `gameobject` SET `spawntimesecs`=28 WHERE `guid`=6960097;
UPDATE `gameobject` SET `spawntimesecs`=17 WHERE `guid`=6960098;
UPDATE `gameobject` SET `spawntimesecs`=20 WHERE `guid`=9780300;
UPDATE `gameobject` SET `spawntimesecs`=42 WHERE `guid`=9780301;
UPDATE `gameobject` SET `spawntimesecs`=41 WHERE `guid`=9780302;
UPDATE `gameobject` SET `spawntimesecs`=19 WHERE `guid`=9780303;

-- Player-supplied exact Lost Page positions (in screenshot order I-IV),
-- replacing the earlier zone-% estimate and the first (mistaken, all-
-- identical) capture.
UPDATE `gameobject` SET `position_x`=-8912.868, `position_y`=-210.38501, `position_z`=82.9935, `orientation`=4.807898 WHERE `id`=900001;
UPDATE `gameobject` SET `position_x`=-8881.236, `position_y`=-182.74419, `position_z`=81.940735, `orientation`=3.1592171 WHERE `id`=900002;
UPDATE `gameobject` SET `position_x`=-8857.602, `position_y`=-185.54759, `position_z`=83.11996, `orientation`=2.7268105 WHERE `id`=900003;
UPDATE `gameobject` SET `position_x`=-8857.163, `position_y`=-187.26309, `position_z`=90.383385, `orientation`=5.4250703 WHERE `id`=900004;

-- Fix: Lost Pages opened no loot window at all despite QuestRequired loot
-- being wired correctly. GameObject.cpp's ActivateToQuest() (Chest case)
-- checks `chest.questId` (gameobject_template.Data8), which was left at 0.
-- Set it to the actual quest (1660001, Knowledge Corrupts) that needs these.
UPDATE `gameobject_template` SET `Data8`=1660001 WHERE `entry` IN (900001,900002,900003,900004);

-- Root cause found: GameObject::Use() in this codebase has NO case for
-- GAMEOBJECT_TYPE_CHEST at all (grep confirms: 8 references to that type
-- elsewhere in GameObject.cpp, none inside Use()'s switch) -- it silently
-- falls to `default:` and does nothing. CMSG_LOOT (the loot opcode) also
-- explicitly rejects non-creature/vehicle guids (LootHandler.cpp:297), so
-- gameobjects can never open a loot window through that path either.
-- Chest-type was simply the wrong choice here.
-- Fix: convert the 4 Lost Pages to GAMEOBJECT_TYPE_GOOBER (10, same type
-- as the already-working Mirror Shard), and change the quest objective
-- from "collect 4 items" to "use each of the 4 page objects" (GO kill
-- credit via RequiredNpcOrGo, negative = gameobject). This still matches
-- the quest text ("recover the missing pages... scattered throughout the
-- abbey") without needing a custom item-granting spell.
UPDATE `gameobject_template` SET `type`=10, `Data0`=0, `Data1`=1660001, `Data5`=1, `Data8`=0 WHERE `entry` IN (900001,900002,900003,900004);
DELETE FROM `gameobject_loot_template` WHERE `Entry` IN (900001,900002,900003,900004);
UPDATE `quest_template` SET
  `RequiredItemId1`=0,`RequiredItemCount1`=0,`RequiredItemId2`=0,`RequiredItemCount2`=0,
  `RequiredItemId3`=0,`RequiredItemCount3`=0,`RequiredItemId4`=0,`RequiredItemCount4`=0,
  `RequiredNpcOrGo1`=-900001,`RequiredNpcOrGoCount1`=1,
  `RequiredNpcOrGo2`=-900002,`RequiredNpcOrGoCount2`=1,
  `RequiredNpcOrGo3`=-900003,`RequiredNpcOrGoCount3`=1,
  `RequiredNpcOrGo4`=-900004,`RequiredNpcOrGoCount4`=1
WHERE `ID`=1660001;

-- Same Chest-type bug applies to Melon/Pumpkin/Apple (999013-999015) for
-- Goldshire's Generosity (1660059) -- converted to GOOBER + GO-use credit
-- the same way, replacing the "collect items" objective.
UPDATE `gameobject_template` SET `type`=10, `Data0`=0, `Data1`=1660059, `Data5`=1 WHERE `entry` IN (999013,999014,999015);
DELETE FROM `gameobject_loot_template` WHERE `Entry` IN (999013,999014,999015);
UPDATE `quest_template` SET
  `RequiredItemId1`=0,`RequiredItemCount1`=0,`RequiredItemId2`=0,`RequiredItemCount2`=0,`RequiredItemId3`=0,`RequiredItemCount3`=0,
  `RequiredNpcOrGo1`=-999013,`RequiredNpcOrGoCount1`=3,
  `RequiredNpcOrGo2`=-999014,`RequiredNpcOrGoCount2`=3,
  `RequiredNpcOrGo3`=-999015,`RequiredNpcOrGoCount3`=10
WHERE `ID`=1660059;

-- Fix quest tracker label: ObjectiveText1 was generic text instead of
-- "Lost Page I" (matching the II/III/IV pattern the other 3 objectives use).
UPDATE `quest_template` SET `ObjectiveText1`='Lost Page I' WHERE `ID`=1660001;

-- Superseding fix: 900001-900004 were invented placeholder entries. Player
-- pulled the REAL entries from the ascension-db archive debug tooltip:
-- Lost Page I=2300500, II=2300503, III=2300504, IV=2300505 (not
-- sequential!), all real type=3 Chest, displayId=300450, lockId(Data0)=1689,
-- chest.questId(Data8)=1660001 on the actual Ascension server. Kept as
-- GOOBER locally (Data0=0, no lock, since Chest still has no working
-- Use() case in this codebase) but with the correct entries and the real
-- displayId, replacing the placeholder 107.
DELETE FROM `gameobject` WHERE `id` IN (900001,900002,900003,900004,2300500,2300503,2300504,2300505);
DELETE FROM `gameobject_template` WHERE `entry` IN (900001,900002,900003,900004,2300500,2300503,2300504,2300505);

INSERT INTO `gameobject_template`
  (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`,
   `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`,
   `AIName`, `ScriptName`, `VerifiedBuild`)
VALUES
  (2300500, 10, 300450, 'Lost Page I', '', '', '', 1, 0, 1660001, 0, 0, 0, 1, 0, 0, '', '', 12340),
  (2300503, 10, 300450, 'Lost Page II', '', '', '', 1, 0, 1660001, 0, 0, 0, 1, 0, 0, '', '', 12340),
  (2300504, 10, 300450, 'Lost Page III', '', '', '', 1, 0, 1660001, 0, 0, 0, 1, 0, 0, '', '', 12340),
  (2300505, 10, 300450, 'Lost Page IV', '', '', '', 1, 0, 1660001, 0, 0, 0, 1, 0, 0, '', '', 12340);

INSERT INTO `gameobject`
  (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`,`rotation1`,`rotation2`,`rotation3`, `spawntimesecs`, `animprogress`, `state`)
VALUES
  (9780350, 2300500, 0, 1, 1, -8912.868, -210.38501, 82.9935, 4.807898, 0,0,0,1, 20, 0, 1),
  (9780351, 2300503, 0, 1, 1, -8881.236, -182.74419, 81.940735, 3.1592171, 0,0,0,1, 42, 0, 1),
  (9780352, 2300504, 0, 1, 1, -8857.602, -185.54759, 83.11996, 2.7268105, 0,0,0,1, 41, 0, 1),
  (9780353, 2300505, 0, 1, 1, -8857.163, -187.26309, 90.383385, 5.4250703, 0,0,0,1, 19, 0, 1);

UPDATE `quest_template` SET
  `RequiredNpcOrGo1`=-2300500, `RequiredNpcOrGoCount1`=1,
  `RequiredNpcOrGo2`=-2300503, `RequiredNpcOrGoCount2`=1,
  `RequiredNpcOrGo3`=-2300504, `RequiredNpcOrGoCount3`=1,
  `RequiredNpcOrGo4`=-2300505, `RequiredNpcOrGoCount4`=1
WHERE `ID`=1660001;

-- Fine-tune: Page II clipped into the ground, nudged up 0.4 in Z.
UPDATE `gameobject` SET `position_z`=82.0407 WHERE `guid`=9780351;

-- Fine-tune: Page III raised 0.2 in Z.
UPDATE `gameobject` SET `position_z`=83.17 WHERE `guid`=9780352;

-- Fix: Sister Alma (161702) had a template and questender/starter relations
-- for the dungeon chain but was never actually spawned (same bug as
-- Bianca/Moroi before). Position player-confirmed via .gps.
DELETE FROM `creature` WHERE `id` = 161702;
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`MovementType`)
VALUES (9780402, 161702, 0, 1, 1, -8748.924, -282.76862, 66.52375, 2.9222476, 300, 0);

-- Fix: Sister Alma's stand-in model looked like a generic skeleton, not a
-- ghost -- player confirmed she should read as a ghostly former priestess.
-- Swapped to the Eldreth Spirit display (10751, translucent robed female
-- ghost), a much closer fit than the earlier stand-in.
DELETE FROM `creature_template_model` WHERE `CreatureID`=161702;
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES (161702, 0, 10751, 1, 1, 12340);
