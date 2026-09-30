-- Hero (class 10), Ascension's classless shell. The client's ChrClasses, CharBaseInfo, CharStartOutfit,
-- SkillRaceClassInfo and gt* rows for class 10 mirror Druid, so the server baseline starts from Druid too.
START TRANSACTION;
DELETE FROM `playercreateinfo` WHERE `class` = 10;
INSERT INTO `playercreateinfo` (`race`, `class`, `map`, `zone`, `position_x`, `position_y`, `position_z`, `orientation`)
SELECT `race`, 10, `map`, `zone`, `position_x`, `position_y`, `position_z`, `orientation`
FROM `playercreateinfo`
WHERE (`class` = 1 AND `race` IN (1, 2, 3, 4, 5, 6, 7, 8, 11)) OR (`class` = 2 AND `race` = 10);

DELETE FROM `player_class_stats` WHERE `Class` = 10;
INSERT INTO `player_class_stats` (`Class`, `Level`, `BaseHP`, `BaseMana`, `Strength`, `Agility`, `Stamina`, `Intellect`, `Spirit`)
SELECT 10, `Level`, `BaseHP`, `BaseMana`, `Strength`, `Agility`, `Stamina`, `Intellect`, `Spirit`
FROM `player_class_stats` WHERE `Class` = 11;

DELETE FROM `playercreateinfo_action` WHERE `class` = 10;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`)
SELECT `race`, 10, 0, 6603, 0 FROM `playercreateinfo` WHERE `class` = 10;

-- A level-1 Hero on "Darkmoon - Season 10 Wildcard" knew these spells before its first roll.
-- Heroes wear every armor type and wield every weapon type.
DELETE FROM `playercreateinfo_spell_custom` WHERE `racemask` = 0 AND `classmask` = 512;
INSERT INTO `playercreateinfo_spell_custom` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(0, 512, 6603, 'Hero: Auto Attack'),
(0, 512, 81, 'Hero: Dodge'),
(0, 512, 3127, 'Hero: Parry'),
(0, 512, 107, 'Hero: Block'),
(0, 512, 674, 'Hero: Dual Wield'),
(0, 512, 3018, 'Hero: Shoot'),
(0, 512, 2764, 'Hero: Throw'),
(0, 512, 5019, 'Hero: Wand'),
(0, 512, 129243, 'Hero: Path of Duality'),
(0, 512, 129246, 'Hero: Twin Flurry'),
(0, 512, 979700, 'Hero: Resilient Constitution'),
(0, 512, 777000, 'Hero: Stone of Retreat: Orgrimmar'),
(0, 512, 777003, 'Hero: Stone of Retreat: Stormwind'),
(0, 512, 196, 'Hero: One-Handed Axes'),
(0, 512, 197, 'Hero: Two-Handed Axes'),
(0, 512, 198, 'Hero: One-Handed Maces'),
(0, 512, 199, 'Hero: Two-Handed Maces'),
(0, 512, 200, 'Hero: Polearms'),
(0, 512, 201, 'Hero: One-Handed Swords'),
(0, 512, 202, 'Hero: Two-Handed Swords'),
(0, 512, 227, 'Hero: Staves'),
(0, 512, 264, 'Hero: Bows'),
(0, 512, 266, 'Hero: Guns'),
(0, 512, 1180, 'Hero: Daggers'),
(0, 512, 2567, 'Hero: Thrown'),
(0, 512, 5009, 'Hero: Wands'),
(0, 512, 5011, 'Hero: Crossbows'),
(0, 512, 15590, 'Hero: Fist Weapons'),
(0, 512, 9078, 'Hero: Cloth'),
(0, 512, 9077, 'Hero: Leather'),
(0, 512, 8737, 'Hero: Mail'),
(0, 512, 750, 'Hero: Plate Mail'),
(0, 512, 9116, 'Hero: Shield');

DELETE FROM `playercreateinfo_item` WHERE `race` = 0 AND `class` = 10;
INSERT INTO `playercreateinfo_item` (`race`, `class`, `itemid`, `amount`, `Note`) VALUES
(0, 10, 38, 1, 'Hero starter kit'),
(0, 10, 39, 1, 'Hero starter kit'),
(0, 10, 40, 1, 'Hero starter kit'),
(0, 10, 25, 1, 'Hero starter kit'),
(0, 10, 2092, 1, 'Hero starter kit'),
(0, 10, 2362, 1, 'Hero starter kit'),
(0, 10, 2504, 1, 'Hero starter kit'),
(0, 10, 23346, 1, 'Hero starter kit'),
(0, 10, 35, 1, 'Hero starter kit'),
(0, 10, 6948, 1, 'Hero starter kit');
COMMIT;
