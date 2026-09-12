-- Continue the calibrated level-one stats from rev_20260903_01_live_class_baseline.sql.
-- Keep the existing legacy per-level gains, offset to each custom class's own starting values.
-- This remains a compatibility curve, not a reconstruction of proprietary CoA progression.
DROP TEMPORARY TABLE IF EXISTS `_coa_class_stat_progression`;
CREATE TEMPORARY TABLE `_coa_class_stat_progression` ENGINE=InnoDB AS
SELECT `custom`.`class` AS `Class`, `source`.`Level`,
    CAST(`start`.`BaseHP` AS SIGNED) + CAST(`source`.`BaseHP` AS SIGNED)
        - CAST(`base`.`BaseHP` AS SIGNED) AS `BaseHP`,
    IF(`start`.`BaseMana` = 0, 0, CAST(`start`.`BaseMana` AS SIGNED) + CAST(`source`.`BaseMana` AS SIGNED)
        - CAST(`base`.`BaseMana` AS SIGNED)) AS `BaseMana`,
    CAST(`start`.`Strength` AS SIGNED) + CAST(`source`.`Strength` AS SIGNED)
        - CAST(`base`.`Strength` AS SIGNED) AS `Strength`,
    CAST(`start`.`Agility` AS SIGNED) + CAST(`source`.`Agility` AS SIGNED)
        - CAST(`base`.`Agility` AS SIGNED) AS `Agility`,
    CAST(`start`.`Stamina` AS SIGNED) + CAST(`source`.`Stamina` AS SIGNED)
        - CAST(`base`.`Stamina` AS SIGNED) AS `Stamina`,
    CAST(`start`.`Intellect` AS SIGNED) + CAST(`source`.`Intellect` AS SIGNED)
        - CAST(`base`.`Intellect` AS SIGNED) AS `Intellect`,
    CAST(`start`.`Spirit` AS SIGNED) + CAST(`source`.`Spirit` AS SIGNED)
        - CAST(`base`.`Spirit` AS SIGNED) AS `Spirit`
FROM `ascension_custom_class` AS `custom`
INNER JOIN `player_class_stats` AS `start` ON `start`.`Class` = `custom`.`class` AND `start`.`Level` = 1
INNER JOIN `player_class_stats` AS `base` ON `base`.`Class` = `custom`.`fallback_class` AND `base`.`Level` = 1
INNER JOIN `player_class_stats` AS `source` ON `source`.`Class` = `custom`.`fallback_class`
WHERE `custom`.`class` BETWEEN 12 AND 32 AND `custom`.`fallback_class` BETWEEN 1 AND 11
    AND `source`.`Level` BETWEEN 2 AND 80;

-- Derive every replacement from the unchanged level-one anchors and legacy rows, so replay cannot add offsets twice.
-- Zero-mana classes stay at zero instead of acquiring their donor class's mana pool at level two.
UPDATE `player_class_stats` AS `target`, `_coa_class_stat_progression` AS `source` SET
    `target`.`BaseHP` = `source`.`BaseHP`,
    `target`.`BaseMana` = `source`.`BaseMana`,
    `target`.`Strength` = `source`.`Strength`,
    `target`.`Agility` = `source`.`Agility`,
    `target`.`Stamina` = `source`.`Stamina`,
    `target`.`Intellect` = `source`.`Intellect`,
    `target`.`Spirit` = `source`.`Spirit`
WHERE `target`.`Class` = `source`.`Class` AND `target`.`Level` = `source`.`Level`;

DROP TEMPORARY TABLE `_coa_class_stat_progression`;
