UPDATE `gameobject_template` SET `size` = 1
WHERE `entry` = 9500202 AND `type` = 5 AND `displayId` = 7194 AND `name` = 'Camp Tent' AND `size` = 0.5;

UPDATE `gameobject_template` SET `size` = 0.35
WHERE `type` = 5 AND `size` = 1
AND ((`entry` = 9500204 AND `displayId` = 5771 AND `name` = 'Alliance Camp Banner')
OR (`entry` = 9500205 AND `displayId` = 5773 AND `name` = 'Horde Camp Banner'));
