-- Accursed Judge (161708): make hostile (attacks nearby players) + random walk 5 yards
UPDATE `creature_template` SET `faction` = 16 WHERE `entry` = 161708;

UPDATE `creature` SET `MovementType` = 1, `wander_distance` = 5
WHERE `id` = 161708 AND `guid` NOT IN (9780555, 9780495, 9780492, 9780484, 9780489);

UPDATE `creature` SET `MovementType` = 1, `wander_distance` = 3
WHERE `guid` IN (9780492, 9780484, 9780489);

-- Accursed Censor (161712) guid 9780418: switch to waypoint movement
UPDATE `creature` SET `MovementType` = 2, `wander_distance` = 0
WHERE `guid` = 9780418;

-- Spawn Z (60) didn't match the waypoint path's Z (~53.7), leaving it
-- floating above its own route and unable to move. Fixed to match.
UPDATE `creature` SET `position_z` = 53.7255 WHERE `guid` = 9780418;

-- WAYPOINT_MOTION_TYPE alone does not auto-use waypoint_data.id == guid;
-- AzerothCore requires an explicit creature_addon.path_id (Creature.cpp
-- only sets m_path_id from cainfo->path_id, default is 0). Without this row
-- the log showed "doesn't have waypoint path id: 0" every time it tried to move.
INSERT IGNORE INTO `creature_addon` (`guid`, `path_id`) VALUES (9780418, 9780418);

-- The 9-point patrol route itself (added in-game via .wp add, exported here
-- so a clean DB actually has the path, not just the pointer to it).
INSERT IGNORE INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`action_chance`) VALUES (9780418,1,-8583,-259.239,53.7255,100);
INSERT IGNORE INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`action_chance`) VALUES (9780418,2,-8583.13,-254.201,53.7255,100);
INSERT IGNORE INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`action_chance`) VALUES (9780418,3,-8580.28,-256.013,53.7255,100);
INSERT IGNORE INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`action_chance`) VALUES (9780418,4,-8577.26,-258.716,53.7239,100);
INSERT IGNORE INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`action_chance`) VALUES (9780418,5,-8574.09,-258.738,53.7239,100);
INSERT IGNORE INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`action_chance`) VALUES (9780418,6,-8573.29,-254.322,53.7239,100);
INSERT IGNORE INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`action_chance`) VALUES (9780418,7,-8575,-252.881,54.1455,100);
INSERT IGNORE INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`action_chance`) VALUES (9780418,8,-8579.72,-252.937,53.7232,100);
INSERT IGNORE INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`action_chance`) VALUES (9780418,9,-8581.23,-255.773,53.7232,100);
