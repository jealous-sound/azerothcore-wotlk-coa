-- The CoA client's CharBaseInfo.dbc offers these stock-class pairs, but the server had no start for them, so
-- creating one failed. Each pair uses its race's existing start.
DELETE FROM `playercreateinfo` WHERE `race` = 1 AND `class` = 3;
DELETE FROM `playercreateinfo` WHERE `race` = 5 AND `class` = 3;
DELETE FROM `playercreateinfo` WHERE `race` = 2 AND `class` = 8;
DELETE FROM `playercreateinfo` WHERE `race` = 3 AND `class` = 8;
DELETE FROM `playercreateinfo` WHERE `race` = 4 AND `class` = 8;
DELETE FROM `playercreateinfo` WHERE `race` = 3 AND `class` = 7;
DELETE FROM `playercreateinfo` WHERE `race` = 5 AND `class` = 2;
DELETE FROM `playercreateinfo` WHERE `race` = 6 AND `class` = 2;
DELETE FROM `playercreateinfo` WHERE `race` = 7 AND `class` = 5;
DELETE FROM `playercreateinfo` WHERE `race` = 8 AND `class` = 9;
DELETE FROM `playercreateinfo` WHERE `race` = 8 AND `class` = 11;
DELETE FROM `playercreateinfo` WHERE `race` = 10 AND `class` = 1;
INSERT INTO `playercreateinfo` (`race`, `class`, `map`, `zone`, `position_x`, `position_y`, `position_z`, `orientation`)
VALUES
(1, 3, 0, 12, -8949.95, -132.493, 83.5312, 0),
(5, 3, 0, 85, 1676.71, 1678.31, 121.67, 2.70526),
(2, 8, 1, 14, -618.518, -4251.67, 38.718, 0),
(3, 8, 0, 1, -6240.32, 331.033, 382.758, 6.17716),
(4, 8, 1, 141, 10311.3, 832.463, 1326.41, 5.69632),
(3, 7, 0, 1, -6240.32, 331.033, 382.758, 6.17716),
(5, 2, 0, 85, 1676.71, 1678.31, 121.67, 2.70526),
(6, 2, 1, 215, -2917.58, -257.98, 52.9968, 0),
(7, 5, 0, 1, -6240.32, 331.033, 382.758, 0),
(8, 9, 1, 14, -618.518, -4251.67, 38.718, 0),
(8, 11, 1, 14, -618.518, -4251.67, 38.718, 0),
(10, 1, 530, 3431, 10349.6, -6357.29, 33.4026, 5.31605);
