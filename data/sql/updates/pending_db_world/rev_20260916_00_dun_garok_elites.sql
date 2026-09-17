-- Dun Garok's garrison is elite on Ascension. The live creature cache reports rank 1 and a health
-- modifier of 3.77 for the Mountaineer (2344), Rifleman (2345) and Priest (2346), and rank 1 with
-- 3.78 for Captain Ironhill (2304), while this world still had them as rank 0 with no health bonus.
-- Dun Garok Soldier (7360) stays a normal mob; the cache reports rank 0 and modifier 1.0 for it.
UPDATE `creature_template` SET `rank` = 1, `HealthModifier` = 3.77 WHERE `entry` IN (2344, 2345, 2346);
UPDATE `creature_template` SET `rank` = 1, `HealthModifier` = 3.78 WHERE `entry` = 2304;
