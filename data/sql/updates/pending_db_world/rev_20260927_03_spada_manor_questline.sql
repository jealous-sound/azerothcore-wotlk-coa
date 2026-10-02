-- Spada Manor / Goldshire questline reconstruction (2026-09-27).
-- Source: Exiles DB export (names, entries, models) + player's own
-- in-game walkthrough for order, givers/enders and exact positions.
-- Confirmed positions (player-supplied, map 0):
--   Aliscar Lend  : -9399.678, -13.539191, 62.227985, 5.723677
--   Dulcinea      : -9421.794,  50.940975, 57.145344, 3.6137972
--   Aldia Crayon  : -9275.592, 469.2705,   89.87461,  0.5612088
-- Lady Agria Spada is in the same room as Aldia (video), placed with a
-- small offset -- adjust once seen in game.
-- Eldor Hammer, Clara the Mad, Harvend Thorm and the 4 market vendors
-- (Rowena/Darron/Ainora/Joaquin) have no known position yet -- template
-- only, no spawn row. Add with `.npc add` and capture the guid once found.

-- ============ Curse Shard drop-chance correction ============
-- Player: only ~60% of Mirror Shard uses should spawn a Curse Shard,
-- not every time. SmartAI event_chance handles this directly.
UPDATE `smart_scripts` SET `event_chance` = 60
WHERE `entryorguid` = 162920 AND `source_type` = 1 AND `event_type` = 70;

-- ============ Creature templates ============
DELETE FROM `creature_template` WHERE `entry` IN
  (162802, 162803, 162800, 162801, 162805, 162806, 162807, 162809, 162811, 162814, 162826, 162943);

INSERT INTO `creature_template`
  (`entry`, `name`, `subname`, `minlevel`, `maxlevel`, `faction`, `npcflag`,
   `speed_walk`, `speed_run`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`,
   `unit_class`, `unit_flags`, `dynamicflags`, `family`, `type`, `type_flags`,
   `HealthModifier`, `ManaModifier`, `ArmorModifier`, `ExperienceModifier`,
   `RegenHealth`, `flags_extra`, `AIName`, `MovementType`, `VerifiedBuild`)
VALUES
  (162802, 'Aldia Crayon', 'Majordomo', 30, 30, 12, 3,
   1, 1.14286, 0, 0, 2000, 2000, 1, 0, 0, 0, 7, 0, 1, 1, 1, 1, 1, 0, '', 0, 12340),
  (162803, 'Lady Agria Spada', '', 45, 45, 12, 1,
   1, 1.14286, 0, 0, 2000, 2000, 1, 0x2, 0, 0, 7, 0, 1, 1, 1, 1, 1, 0, '', 0, 12340),
  (162800, 'Dulcinea', 'Maid of House Spada', 20, 20, 12, 3,
   1, 1.14286, 0, 0, 2000, 2000, 1, 0, 0, 0, 7, 0, 1, 1, 1, 1, 1, 0, '', 0, 12340),
  (162801, 'Eldor Hammer', 'Westfall Refugee', 20, 20, 12, 3,
   1, 1.14286, 0, 0, 2000, 2000, 1, 0, 0, 0, 7, 0, 1, 1, 1, 1, 1, 0, '', 0, 12340),
  (162805, 'Clara the Mad', '', 20, 20, 12, 3,
   1, 1.14286, 0, 0, 2000, 2000, 1, 0, 0, 0, 7, 0, 1, 1, 1, 1, 1, 0, '', 0, 12340),
  (162806, 'Aliscar Lend', '', 20, 20, 12, 3,
   1, 1.14286, 0, 0, 2000, 2000, 1, 0, 0, 0, 7, 0, 1, 1, 1, 1, 1, 0, '', 0, 12340),
  (162807, 'Harvend Thorm', 'Mayor of Goldshire', 20, 20, 12, 3,
   1, 1.14286, 0, 0, 2000, 2000, 1, 0, 0, 0, 7, 0, 1, 1, 1, 1, 1, 0, '', 0, 12340),
  (162809, 'Rowena', '', 20, 20, 12, 128,
   1, 1.14286, 0, 0, 2000, 2000, 1, 0, 0, 0, 7, 0, 1, 1, 1, 1, 1, 0, '', 0, 12340),
  (162811, 'Darron', '', 20, 20, 12, 128,
   1, 1.14286, 0, 0, 2000, 2000, 1, 0, 0, 0, 7, 0, 1, 1, 1, 1, 1, 0, '', 0, 12340),
  (162814, 'Ainora', 'Florist', 20, 20, 12, 128,
   1, 1.14286, 0, 0, 2000, 2000, 1, 0, 0, 0, 7, 0, 1, 1, 1, 1, 1, 0, '', 0, 12340),
  (162826, 'Joaquin', '', 20, 20, 12, 128,
   1, 1.14286, 0, 0, 2000, 2000, 1, 0, 0, 0, 7, 0, 1, 1, 1, 1, 1, 0, '', 0, 12340),
  (162943, 'Arcane Projection of Aliscar', '', 20, 20, 12, 0,
   1, 1.14286, 0, 0, 2000, 2000, 1, 0, 0, 0, 7, 0, 1, 1, 1, 1, 1, 0, '', 0, 12340);

-- ============ Models ============
DELETE FROM `creature_template_model` WHERE `CreatureID` IN
  (162802, 162803, 162800, 162801, 162805, 162806, 162807, 162809, 162811, 162814, 162826, 162943);
DELETE FROM `creature_model_info` WHERE `DisplayID` IN
  (652414, 652415, 652354, 652413, 652441, 652448, 652350, 652352, 652355, 652358, 652370);

INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`, `DisplayID_Other_Gender`, `VerifiedBuild`) VALUES
  (652414, 0.383, 1.5, 0, 0, 12340),
  (652415, 0.383, 1.5, 1, 0, 12340),
  (652354, 0.383, 1.5, 1, 0, 12340),
  (652413, 0.383, 1.5, 0, 0, 12340),
  (652441, 0.383, 1.5, 1, 0, 12340),
  (652448, 0.383, 1.5, 0, 0, 12340),
  (652350, 0.383, 1.5, 0, 0, 12340),
  (652352, 0.383, 1.5, 1, 0, 12340),
  (652355, 0.383, 1.5, 0, 0, 12340),
  (652358, 0.383, 1.5, 1, 0, 12340),
  (652370, 0.383, 1.5, 0, 0, 12340);

INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES
  (162802, 0, 652414, 1, 1, 12340),
  (162803, 0, 652415, 1, 1, 12340),
  (162800, 0, 652354, 1, 1, 12340),
  (162801, 0, 652413, 1, 1, 12340),
  (162805, 0, 652441, 1, 1, 12340),
  (162806, 0, 652448, 1, 1, 12340),
  (162807, 0, 652350, 1, 1, 12340),
  (162809, 0, 652352, 1, 1, 12340),
  (162811, 0, 652355, 1, 1, 12340),
  (162814, 0, 652358, 1, 1, 12340),
  (162826, 0, 652370, 1, 1, 12340),
  (162943, 0, 652448, 1, 1, 12340);

-- ============ Spawns (only where position is known) ============
DELETE FROM `creature` WHERE `id` IN (162802, 162803, 162800, 162806);
INSERT INTO `creature`
  (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `MovementType`)
VALUES
  (9780113, 162802, 0, 1, 1, -9275.592, 469.2705, 89.87461, 0.5612088, 300, 0),
  (9780114, 162803, 0, 1, 1, -9273.482, 470.02893, 90.9133, 4.763736, 300, 0),
  (9780115, 162800, 0, 1, 1, -9421.794, 50.940975, 57.145344, 3.6137972, 300, 0),
  (9780116, 162806, 0, 1, 1, -9399.678, -13.539191, 62.227985, 5.723677, 300, 0);

-- Lady Agria Spada lies asleep in bed (player-confirmed, not a mechanic --
-- just her stand state).
DELETE FROM `creature_addon` WHERE `guid` = 9780114;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `visibilityDistanceType`)
VALUES (9780114, 0, 0, 3, 0, 0, 0);

-- ============ Vendor items ============
DELETE FROM `npc_vendor` WHERE `entry` IN (162809, 162811, 162814, 162826);
INSERT INTO `npc_vendor` (`entry`, `item`, `maxcount`, `incrtime`, `ExtendedCost`) VALUES
  (162809, 558958, 0, 0, 0), -- Rowena: Pumpkin Juice
  (162811, 558957, 0, 0, 0), -- Darron: Dun Kazad Liquor Concentrate
  (162814, 558956, 0, 0, 0), -- Ainora: Elgris Blossom Petals
  (162826, 558959, 0, 0, 0); -- Joaquin: Murloc Eyeball

-- ============ Quest chain ============
DELETE FROM `quest_template` WHERE `ID` IN (1660055, 1660056, 1660057, 1660058, 1660059, 1660060);

INSERT INTO `quest_template`
  (`ID`, `QuestType`, `QuestLevel`, `MinLevel`, `QuestSortID`, `RewardXPDifficulty`,
   `RewardMoney`, `RewardMoneyDifficulty`, `RequiredNpcOrGo1`, `RequiredNpcOrGoCount1`,
   `RequiredItemId1`, `RequiredItemCount1`, `RequiredItemId2`, `RequiredItemCount2`,
   `RequiredItemId3`, `RequiredItemCount3`, `RequiredItemId4`, `RequiredItemCount4`,
   `RewardChoiceItemID1`, `RewardChoiceItemQuantity1`, `RewardChoiceItemID2`, `RewardChoiceItemQuantity2`,
   `RewardChoiceItemID3`, `RewardChoiceItemQuantity3`,
   `LogTitle`, `LogDescription`, `QuestDescription`, `AreaDescription`, `QuestCompletionLog`, `ObjectiveText1`)
VALUES
  (1660055, 2, 8, 5, 12, 4,
   120, 135, 0, 0,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,
   'The Maid I Left Behind', 'Find Dulcinea, Bianca''s maid, in Goldshire.',
   'You''ve been in and out of the abbey all day, yet I still haven''t seen my brother anywhere.$B$BI take it you couldn''t snap him out of it. Well... worth a try.$B$B<Her long, weary sigh says enough.>$B$BHeading out again? Maybe you can do me one more favor. On the way to the abbey I left one of my maids in Goldshire with a long list of ingredients to buy; medicine for my mother.$B$BHer name is Dulcinea. Could you find her and let her know I''ll be late?',
   '', 'Speak with Dulcinea.', 'Find Dulcinea, Bianca''s maid, in Goldshire.'),

  (1660056, 2, 8, 5, 12, 5,
   110, 120, 0, 0,
   558956, 1, 558957, 1, 558958, 1, 558959, 1,
   2302015, 1, 2302020, 1, 0, 0,
   'Agria''s Medicine', 'Buy the potion''s ingredients at the Goldshire market, then report to the manor.',
   'Lady Agria Spada is a woman besieged by age. Of late she''s borne a litany of ailments that keep her to her bed.$B$BOnly one thing eases her: the draught brewed by her master alchemist.$B$BI''ve gathered a few ingredients, but the list is long and fussy. We still need Elgris Blossom Petals, Dun Kazad Liquor Concentrate, Pumpkin Juice, and a Murloc Eyeball.$B$BTake a turn through the market. Once you''ve got the lot, report to my lady''s manor. I don''t dare the roads alone, but you... you''re made of sterner stuff, aren''t you?',
   '', 'Deliver the ingredients to Aldia Crayon.', 'Gather Elgris Blossom Petals, Dun Kazad Liquor Concentrate, Pumpkin Juice and a Murloc Eyeball from the Goldshire market.'),

  (1660057, 2, 8, 5, 12, 5,
   350, 382, -162920, 6,
   0,0,0,0,0,0,0,0,
   2302030, 1, 2302035, 1, 2302040, 1,
   'Seven Years of Bad Luck', 'Inspect the broken mirror shards that Aldia Crayon blames for the curse afflicting Lady Agria Spada.',
   'Hm...$B$B<The majordomo gives you a long, measuring look.>$B$BBefore you go... I need your help with one more matter.$B$BI''ve had the sense for some time that my lady''s ailments aren''t physical, but magical. A curse.$B$BIt started with a broken mirror. And you know what they say. However thorough I''ve been, shards keep turning up; bits of glass tucked around the manor and the grounds.$B$BWould you kindly deal with them?',
   '', 'Return to the butler.', 'Deal with 6 broken mirror shards around the manor and grounds.'),

  (1660060, 2, 8, 5, 12, 3,
   0, 0, 0, 0,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,
   'Stay a While', 'Take a moment from the noise and haste and stay a while to listen to Aliscar Lend.',
   'As it turns out, I''m the leading authority around here when it comes to the village''s history. Care for a short lesson?$B$B<The aging sorcerer seems eager, almost desperate, to talk.>$B$B<Perhaps he has something worth sharing.>',
   '', 'Say goodbye to Aliscar Lend.', 'Listen to Aliscar Lend''s history lesson.'),

  (1660058, 2, 8, 5, 12, 5,
   260, 337, 0, 0,
   0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,
   'Worm-Eaten Apple', 'Find and destroy the kobold warrens around Goldshire.',
   'Whispers. They think no one''s noticed, but I sleep with my ear to the floor! Ha!$B$BThey dig and dig and dig. They''ll strike when you least expect it... unless you do something first.$B$bKobolds and kobolds and more kobolds. Under our feet! Watch where you step. Mind your footing!$B$BFind their warrens and set them alight. Crush them. With a big, heavy hammer!$B$B<She laughs to herself, then stares into the middle distance.>',
   '', 'Return to Clara the Mad.', 'Destroy the kobold warrens around Goldshire.'),

  (1660059, 2, 8, 5, 12, 5,
   156, 213, 0, 0,
   0,0,0,0,0,0,0,0,
   2302050, 1, 2302055, 1, 2302060, 1,
   'Goldshire''s Generosity', 'Gather Melons, Pumpkins, and Apples from the farms around Goldshire, then deliver them to Eldor Hammer.',
   'Welcome.$B$BI am Thorm -- Harvend Thorm -- mayor of Goldshire, serving on behalf of Lord Bruk and his house.$B$BYou may have noticed the makeshift camp in the shadow of this grand hall. Of late, Goldshire has taken in countless refugees out of Westfall. At times I fear it''s beyond our means...$B$BEven so, I won''t turn away while they go hungry. Walk the village fields and take a little from each harvest to bring to the camp. You have my leave -- and, by extension, Lord Bruk''s.',
   '', 'Deliver the basket of food to Eldor Hammer.', 'Gather Melons, Pumpkins, and Apples from the farms around Goldshire.');

-- ============ Quest giver / ender relations ============
DELETE FROM `creature_queststarter` WHERE `quest` IN (1660055,1660056,1660057,1660058,1660059,1660060);
DELETE FROM `creature_questender` WHERE `quest` IN (1660055,1660056,1660057,1660058,1660059,1660060);

INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
  (161700, 1660055),  -- Bianca Spada
  (162800, 1660056),  -- Dulcinea
  (162802, 1660057),  -- Aldia Crayon
  (162806, 1660060),  -- Aliscar Lend
  (162805, 1660058),  -- Clara the Mad
  (162807, 1660059);  -- Harvend Thorm

INSERT INTO `creature_questender` (`id`, `quest`) VALUES
  (162800, 1660055),  -- Dulcinea
  (162802, 1660056),  -- Aldia Crayon
  (162802, 1660057),  -- Aldia Crayon ("Return to the butler")
  (162806, 1660060),  -- Aliscar Lend
  (162805, 1660058),  -- Clara the Mad
  (162801, 1660059);  -- Eldor Hammer
