-- Two placements at the Orgrimmar entrance: the pair of training dummies, and the level scaling
-- NPC moved out of the bank to stand beside them.
-- 1. Two training dummies at the Valley of Strength entrance, where a player leaving the city
--    passes anyway: (1481.73, -4395.51, 26.68), a ground height taken from a character standing
--    there rather than guessed. The second sits three yards along the same contour.
DELETE FROM `creature` WHERE `guid` IN (5400001, 5400002);
INSERT INTO `creature`
  (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `equipment_id`,
   `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`,
   `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `Comment`)
VALUES
  (5400001, 31144, 1, 0, 0, 1, 1, 0, 1481.73, -4395.51, 26.68, 6.229, 300, 0, 0, 1, 0, 0, 'Orgrimmar entrance training dummy (level 80)'),
  (5400002, 31146, 1, 0, 0, 1, 1, 0, 1484.70, -4396.50, 26.60, 6.229, 300, 0, 0, 1, 0, 0, 'Orgrimmar entrance training dummy (level 83 boss)');

-- 2. The level scaling NPC moves out of the bank to the same entrance, a few yards to the side so it
--    does not stand on the dummies. (1481.00, -4405.00, 25.60) sits between the character's own
--    position and the Orgrimmar Grunt already spawned at (1479.39, -4406.25, 25.56), so the ground
--    height comes from a confirmed spawn.
UPDATE `creature`
SET `position_x` = 1481.00, `position_y` = -4405.00, `position_z` = 25.60, `orientation` = 3.100
WHERE `guid` = 5400003;
