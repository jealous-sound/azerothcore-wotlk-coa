INSERT INTO `gameobject_template`
(`entry`, `type`, `displayId`, `name`, `size`, `Data0`, `Data1`, `VerifiedBuild`) VALUES
(9500202, 5, 7194, 'Camp Tent', 0.5, 0, 0, 12340),
(9500203, 7, 39, 'Camp Chair', 1, 1, 1, 12340),
(9500204, 5, 5771, 'Alliance Camp Banner', 1, 0, 0, 12340),
(9500205, 5, 5773, 'Horde Camp Banner', 1, 0, 0, 12340)
ON DUPLICATE KEY UPDATE `entry` = `gameobject_template`.`entry`;

INSERT INTO `creature_template`
(`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `unit_class`, `unit_flags`, `type`,
`VerifiedBuild`) VALUES
(9500220, 'Camp Reagent Bot', 60, 60, 190, 128, 1, 768, 9, 12340),
(9500221, 'Camp Repair Bot', 60, 60, 190, 4224, 1, 768, 9, 12340)
ON DUPLICATE KEY UPDATE `entry` = `creature_template`.`entry`;

DELETE FROM `creature_template_model`
WHERE `Idx` = 0 AND `CreatureDisplayID` = 14379 AND `DisplayScale` = 1 AND `Probability` = 1 AND `VerifiedBuild` = 12340
AND `CreatureID` IN (SELECT `entry` FROM `creature_template`
WHERE (`entry` = 9500220 AND `name` = 'Camp Reagent Bot' AND `npcflag` = 128 AND `faction` = 190)
OR (`entry` = 9500221 AND `name` = 'Camp Repair Bot' AND `npcflag` = 4224 AND `faction` = 190));
INSERT INTO `creature_template_model`
(`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`)
SELECT `entry`, 0, 14379, 1, 1, 12340 FROM `creature_template`
WHERE (`entry` = 9500220 AND `name` = 'Camp Reagent Bot' AND `npcflag` = 128 AND `faction` = 190)
OR (`entry` = 9500221 AND `name` = 'Camp Repair Bot' AND `npcflag` = 4224 AND `faction` = 190)
ON DUPLICATE KEY UPDATE `CreatureID` = `creature_template_model`.`CreatureID`;

DELETE FROM `npc_vendor`
WHERE `maxcount` = 0 AND `incrtime` = 0 AND `ExtendedCost` = 0 AND `VerifiedBuild` = 12340
AND `slot` = FIELD(`item`, 17020, 17021, 17026, 17028, 17029, 17030, 17031, 17032, 17033)
AND `slot` BETWEEN 1 AND 9
AND `entry` IN (SELECT `entry` FROM `creature_template`
WHERE (`entry` = 9500220 AND `name` = 'Camp Reagent Bot' AND `npcflag` = 128 AND `faction` = 190)
OR (`entry` = 9500221 AND `name` = 'Camp Repair Bot' AND `npcflag` = 4224 AND `faction` = 190));
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `VerifiedBuild`)
SELECT `bot`.`entry`, `stock`.`slot`, `stock`.`item`, 0, 0, 0, 12340 FROM `creature_template` AS `bot`
CROSS JOIN (
  SELECT 1 AS `slot`, 17020 AS `item` UNION ALL
  SELECT 2, 17021 UNION ALL
  SELECT 3, 17026 UNION ALL
  SELECT 4, 17028 UNION ALL
  SELECT 5, 17029 UNION ALL
  SELECT 6, 17030 UNION ALL
  SELECT 7, 17031 UNION ALL
  SELECT 8, 17032 UNION ALL
  SELECT 9, 17033
) AS `stock`
WHERE (`bot`.`entry` = 9500220 AND `bot`.`name` = 'Camp Reagent Bot' AND `bot`.`npcflag` = 128 AND `bot`.`faction` = 190)
OR (`bot`.`entry` = 9500221 AND `bot`.`name` = 'Camp Repair Bot'
AND `bot`.`npcflag` = 4224 AND `bot`.`faction` = 190)
ON DUPLICATE KEY UPDATE `entry` = `npc_vendor`.`entry`;
