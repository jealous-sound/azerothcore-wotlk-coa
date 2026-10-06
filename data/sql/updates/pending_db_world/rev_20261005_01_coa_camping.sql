INSERT INTO `gameobject_template`
(`entry`, `type`, `displayId`, `name`, `size`, `Data3`, `VerifiedBuild`) VALUES
(9500200, 10, 345, 'Campsite Supplies', 1, 0, 12340),
(9500201, 5, 100, 'Incense Candle', 1, 0, 12340)
ON DUPLICATE KEY UPDATE `entry` = `gameobject_template`.`entry`;
