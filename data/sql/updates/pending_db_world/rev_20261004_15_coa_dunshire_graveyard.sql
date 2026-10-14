-- Dunshire graveyard for Pale Reach (zone 10302), from an in-game capture (2026-10-04).
DELETE FROM `game_graveyard` WHERE `ID` = 9920001;
INSERT INTO `game_graveyard` (`ID`, `Map`, `x`, `y`, `z`, `Comment`) VALUES
(9920001, 0, -7728.941, 684.0378, 136.4791, 'Pale Reach, Dunshire');

DELETE FROM `graveyard_zone` WHERE `ID` = 9920001;
INSERT INTO `graveyard_zone` (`ID`, `GhostZone`, `Faction`, `Comment`) VALUES
(9920001, 10302, 0, 'Pale Reach - Dunshire');

DELETE FROM `creature` WHERE `guid` = 9920500;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `MovementType`, `Comment`) VALUES
(9920500, 6491, 0, 0, 0, 1, 1, -7728.941, 684.0378, 136.4791, 0.95169264, 60, 0, 0, 'Spirit Healer, Dunshire graveyard');
