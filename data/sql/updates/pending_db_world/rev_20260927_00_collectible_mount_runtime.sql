INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`) VALUES
(97608, 'Steadfast Dunetrekker', 1, 1, 35, 1, 12, 2000, 2000), (10157259, 'Armored Spectral Tiger', 1, 1, 35, 1, 12, 2000, 2000) ON DUPLICATE KEY UPDATE `name` = VALUES(`name`), `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `unit_class` = VALUES(`unit_class`), `type` = VALUES(`type`), `BaseAttackTime` = VALUES(`BaseAttackTime`), `RangeAttackTime` = VALUES(`RangeAttackTime`);
DELETE FROM `creature_template_model` WHERE `CreatureID` IN (97608, 10157259);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) VALUES
(97608, 0, 8051, 1, 1), (10157259, 0, 470000, 1, 1);
