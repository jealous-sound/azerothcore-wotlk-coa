-- Callboard Cache progress: the points each Callboard quest adds to the character's progress bar.
-- Captured live values win (client questcacheaddon.wdb, Bronzebeard and Rexxar, 2026-08-15 and 2026-09-04):
-- 18017-18019 20, Dungeon Diving 81244/81245/81246 15, Lead a Dungeon 100078 15, world boss 170015 50.
-- Uncaptured quests follow the Bronzebeard video of 2025-11-05: quick (profession) 15, medium (world kill
-- and collect, PvP) 20, long (elite) 25.

DROP TABLE IF EXISTS `ascension_callboard_quest_points`;
CREATE TABLE `ascension_callboard_quest_points` (
  `QuestId` INT UNSIGNED NOT NULL,
  `Points` SMALLINT UNSIGNED NOT NULL,
  PRIMARY KEY (`QuestId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

DELETE FROM `ascension_callboard_quest_points`;
INSERT INTO `ascension_callboard_quest_points` (`QuestId`, `Points`) VALUES
(26035, 15),
(26053, 15),
(1005092, 15),
(1005124, 15),
(1005170, 15),
(1005259, 15),
(1005301, 15),
(1005468, 15),
(1005474, 15),
(1005647, 15),
(1005660, 15),
(1005664, 15),
(1007453, 15),
(1007496, 15),
(1038797, 15),
(1038884, 15),
(80651, 20),
(81260, 20),
(17838, 20),
(17926, 20),
(18017, 20),
(18018, 20),
(18019, 20),
(18030, 20),
(18044, 20),
(18045, 20),
(18046, 20),
(18054, 20),
(18055, 20),
(18056, 20),
(18089, 20),
(18090, 20),
(18091, 20),
(18105, 20),
(18106, 20),
(18110, 20),
(18111, 20),
(18034, 25),
(18036, 25),
(18104, 25),
(18109, 25),
(81244, 15),
(81245, 15),
(81246, 15),
(100078, 15),
(170015, 50);
