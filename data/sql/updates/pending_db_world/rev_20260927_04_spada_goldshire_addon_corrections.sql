-- Corrections from Questie-X-AscensionDB (addon-captured, authoritative):
-- real display models, real positions (zone-% converted to world coords via
-- the validated formula: world_y = loc_left-(px/100)*(loc_left-loc_right),
-- world_x = loc_top-(py/100)*(loc_top-loc_bottom); Elwynn Forest bounding
-- box loc_left=1535, loc_right=-1935, loc_top=-7939, loc_bottom=-10254 --
-- validated against Aldia Crayon's own player-confirmed in-game position,
-- which matched the converted value to within half a yard), and the real
-- quest giver/ender for "Agria's Medicine": a third NPC "Aldia" (162815),
-- not Dulcinea or Aldia Crayon (same first name, different person).

-- ============ New NPC: Aldia (162815), Agria's Medicine giver/ender ============
DELETE FROM `creature_template` WHERE `entry` = 162815;
INSERT INTO `creature_template`
  (`entry`, `name`, `subname`, `minlevel`, `maxlevel`, `faction`, `npcflag`,
   `speed_walk`, `speed_run`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`,
   `unit_class`, `unit_flags`, `dynamicflags`, `family`, `type`, `type_flags`,
   `HealthModifier`, `ManaModifier`, `ArmorModifier`, `ExperienceModifier`,
   `RegenHealth`, `flags_extra`, `AIName`, `MovementType`, `VerifiedBuild`)
VALUES
  (162815, 'Aldia', '', 20, 20, 12, 3,
   1, 1.14286, 0, 0, 2000, 2000, 1, 0, 0, 0, 7, 0, 1, 1, 1, 1, 1, 0, '', 0, 12340);

DELETE FROM `creature_model_info` WHERE `DisplayID` IN (293, 510, 688, 120);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`, `DisplayID_Other_Gender`, `VerifiedBuild`) VALUES
  (293, 0.383, 1.5, 0, 0, 12340),
  (510, 0.383, 1.5, 1, 0, 12340),
  (688, 0.383, 1.5, 0, 0, 12340),
  (120, 0.383, 1.5, 0, 0, 12340);

-- ============ Real display models (addon-confirmed, replacing earlier guesses) ============
DELETE FROM `creature_template_model` WHERE `CreatureID` IN
  (162800, 162801, 162802, 162805, 162806, 162807, 162809, 162811, 162814, 162815, 162826, 162943);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES
  (162800, 0, 510,  1, 1, 12340),  -- Dulcinea
  (162801, 0, 688,  1, 1, 12340),  -- Eldor Hammer
  (162802, 0, 1394, 1, 1, 12340),  -- Aldia Crayon
  (162805, 0, 120,  1, 1, 12340),  -- Clara the Mad
  (162806, 0, 1593, 1, 1, 12340),  -- Aliscar Lend
  (162807, 0, 1923, 1, 1, 12340),  -- Harvend Thorm
  (162809, 0, 120,  1, 1, 12340),  -- Rowena
  (162811, 0, 120,  1, 1, 12340),  -- Darron
  (162814, 0, 120,  1, 1, 12340),  -- Ainora
  (162815, 0, 120,  1, 1, 12340),  -- Aldia
  (162826, 0, 120,  1, 1, 12340),  -- Joaquin
  (162943, 0, 1593, 1, 1, 12340);  -- Arcane Projection of Aliscar

-- ============ Real levels (addon-confirmed) ============
UPDATE `creature_template` SET `minlevel`=17, `maxlevel`=17 WHERE `entry`=162800; -- Dulcinea
UPDATE `creature_template` SET `minlevel`=21, `maxlevel`=21 WHERE `entry`=162801; -- Eldor Hammer
UPDATE `creature_template` SET `minlevel`=8,  `maxlevel`=8  WHERE `entry`=162805; -- Clara the Mad
UPDATE `creature_template` SET `minlevel`=32, `maxlevel`=32 WHERE `entry`=162806; -- Aliscar Lend
UPDATE `creature_template` SET `minlevel`=35, `maxlevel`=35 WHERE `entry`=162807; -- Harvend Thorm
UPDATE `creature_template` SET `minlevel`=32, `maxlevel`=32 WHERE `entry`=162943; -- Arcane Projection

-- ============ Real positions (converted from addon zone-% capture) ============
-- Aliscar Lend, Dulcinea and Aldia Crayon keep the player's own exact
-- in-game coordinates (more precise than the 2-decimal-% addon capture).
DELETE FROM `creature` WHERE `id` IN (162801, 162805, 162807, 162809, 162811, 162814, 162815, 162826);
INSERT INTO `creature`
  (`id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `MovementType`)
VALUES
  (162801, 0, 1, 1, -9565.29, 9.59,   57, 0, 300, 0),  -- Eldor Hammer
  (162805, 0, 1, 1, -9579.64, 35.27,  59, 0, 300, 0),  -- Clara the Mad
  (162807, 0, 1, 1, -9562.28, 63.03,  59, 0, 300, 0),  -- Harvend Thorm
  (162809, 0, 1, 1, -9487.74, 35.96,  59, 0, 300, 0),  -- Rowena
  (162811, 0, 1, 1, -9464.58, 60.25,  57, 0, 300, 0),  -- Darron
  (162814, 0, 1, 1, -9388.19, 22.08,  60, 0, 300, 0),  -- Ainora
  (162815, 0, 1, 1, -9858.14, 341.32, 60, 0, 300, 0),  -- Aldia
  (162826, 0, 1, 1, -9450.69, -82.02, 57, 0, 300, 0);  -- Joaquin
-- Z values above are approximate (ground height not sampled) -- expect
-- some to need a `.npc move`/`.gobject move`-style Z correction in game.

-- ============ Fix Agria's Medicine (1660056) giver/ender: Aldia (162815), not Dulcinea/Aldia Crayon ============
DELETE FROM `creature_queststarter` WHERE `quest` = 1660056;
DELETE FROM `creature_questender` WHERE `quest` = 1660056;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES (162815, 1660056);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES (162815, 1660056);

-- ============ Fix Agria's Medicine required items (addon confirms RequiredSourceItems, not lootable) ============
UPDATE `quest_template` SET
  `RequiredItemId1`=558956, `RequiredItemCount1`=1,
  `RequiredItemId2`=558957, `RequiredItemCount2`=1,
  `RequiredItemId3`=558958, `RequiredItemCount3`=1,
  `RequiredItemId4`=558959, `RequiredItemCount4`=1
WHERE `ID`=1660056;

-- ============ Fix Goldshire's Generosity (1660059) required items (addon-confirmed) ============
UPDATE `quest_template` SET
  `RequiredItemId1`=558962, `RequiredItemCount1`=3, -- Melon x3
  `RequiredItemId2`=558961, `RequiredItemCount2`=3, -- Pumpkin x3
  `RequiredItemId3`=558963, `RequiredItemCount3`=10 -- Apple x10
WHERE `ID`=1660059;

-- ============ Quest chain sequencing (quest_template_addon.PrevQuestID) ============
-- Addon-confirmed: 1660059 follows 1660055 directly, not 1660058 as guessed
-- earlier. 1660055's own prerequisite (1660004) is the Northshire Abbey
-- finale quest, not yet built in this DB -- harmless to set now, it just
-- won't gate anything until that quest exists.
DELETE FROM `quest_template_addon` WHERE `ID` IN (1660055,1660056,1660057,1660058,1660059,1660060);
INSERT INTO `quest_template_addon` (`ID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`) VALUES
  (1660055, 1660004, 0, 0),
  (1660056, 1660055, 0, 0),
  (1660057, 1660056, 0, 0),
  (1660058, 0, 0, 0),
  (1660059, 1660055, 0, 0),
  (1660060, 0, 0, 0);

-- Correction: ascension-db.ascension-archive.workers.dev record capture
-- (13 independent sources, "free-pick" method) confirms Bianca/Moroi's
-- ORIGINAL display models (652000/652001, set at the very start of this
-- session) were correct -- the Questie-X-AscensionDB addon's values
-- (158/293) were wrong for these two. Also sets their real HealthModifier.
DELETE FROM `creature_template_model` WHERE `CreatureID` IN (161700, 161701);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES
  (161700, 0, 652000, 1, 1, 12340),
  (161701, 0, 652001, 1, 1, 12340);
DELETE FROM `creature_model_info` WHERE `DisplayID` IN (652000, 652001);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`, `DisplayID_Other_Gender`, `VerifiedBuild`) VALUES
  (652000, 0.383, 1.5, 1, 0, 12340),
  (652001, 0.383, 1.5, 0, 0, 12340);
UPDATE `creature_template` SET `HealthModifier`=0.96 WHERE `entry`=161700;
UPDATE `creature_template` SET `HealthModifier`=0.98 WHERE `entry`=161701;

-- Server log on restart: "Creature (Entry: X) lists non-existing
-- CreatureDisplayID id (Y), this can crash the client" for 161700 (652000),
-- 161701 (652001), 162801 (688), 162803 (652415) -- the actual loaded
-- client DBC (checked in memory at runtime, not the `creaturedisplayinfo_dbc`
-- table, which is also empty for these ids) doesn't have these display ids,
-- so the creatures failed to load at all after restart. The archive-
-- confirmed 652000/652001 for Bianca/Moroi are real Ascension displays,
-- just not present in this server's client DBC data -- would need the
-- actual CreatureDisplayInfo.dbc row copied in (same procedure as
-- docs/coa/npc-restoration.md) to use them correctly.
-- Temporary fix: swap to confirmed-working stock human displays already
-- used by existing Goldshire NPCs, so the quest chain is usable now.
DELETE FROM `creature_template_model` WHERE `CreatureID` IN (161700, 161701, 162801, 162803);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES
  (161700, 0, 1287, 1, 1, 12340),  -- Bianca Spada (female stand-in)
  (161701, 0, 1291, 1, 1, 12340),  -- Moroi Spada (male stand-in)
  (162801, 0, 1292, 1, 1, 12340),  -- Eldor Hammer (male stand-in)
  (162803, 0, 1295, 1, 1, 12340);  -- Lady Agria Spada (female stand-in)
