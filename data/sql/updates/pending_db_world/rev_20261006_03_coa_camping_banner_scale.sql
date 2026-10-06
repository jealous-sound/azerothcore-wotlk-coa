UPDATE `gameobject_template` SET `size` = 1
WHERE `type` = 5 AND `size` BETWEEN 0.3499 AND 0.3501
AND ((`entry` = 9500204 AND `displayId` = 5771 AND `name` = 'Alliance Camp Banner')
OR (`entry` = 9500205 AND `displayId` = 5773 AND `name` = 'Horde Camp Banner'));
