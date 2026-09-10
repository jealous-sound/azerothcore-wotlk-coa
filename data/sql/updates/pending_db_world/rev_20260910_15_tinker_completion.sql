-- Tinker completion: new unapplied SQL15; requires matching source and absent-or-exact templates.
DELETE FROM `spell_proc` WHERE `SpellId` IN (92138, 92140, 92141, 300636, 300877, 503534, 503552, 503553, 503569, 504523, 504749, 520022, 524834, 524979, 537247, 560734, 560782, 560785, 560787, 572367, 572545, 573247, 653273, 680975, 680998, 681001, 681245, 704107, 704449, 705786, 705803, 705810, 705815, 705817, 705820, 705831, 705846, 706379, 706680, 706695, 707237, 707240, 707244, 707249, 707250, 707256, 707259, 707260, 707261, 707262, 707265, 707271, 707272, 707273, 707277, 707395, 707698, 803074, 805314, 806627, 806629, 806631, 806758, 807388, 807499, 807500);
INSERT INTO `spell_proc` (`SpellId`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `Chance`, `Charges`) VALUES
(92138, 1048575, 7, 2, 32767, 2, 100, 0),
(300877, 1048575, 7, 2, 32767, 2, 100, 0),
(503552, 1048575, 7, 2, 32767, 2, 100, 0),
(504523, 1048575, 7, 2, 32767, 2, 100, 0),
(504749, 1048575, 7, 2, 32767, 2, 100, 0),
(560785, 1048575, 7, 2, 32767, 2, 100, 0),
(572367, 1048575, 7, 2, 32767, 2, 100, 0),
(573247, 1048575, 7, 2, 32767, 2, 100, 0),
(680975, 1048575, 7, 2, 32767, 2, 100, 0),
(681001, 1048575, 7, 2, 32767, 2, 100, 0),
(704107, 1048575, 7, 2, 32767, 2, 100, 0),
(704449, 1048575, 7, 2, 32767, 2, 100, 0),
(705810, 1048575, 7, 2, 32767, 2, 100, 0),
(705815, 1048575, 7, 2, 32767, 2, 100, 0),
(705817, 1048575, 7, 2, 32767, 2, 100, 0),
(705820, 1048575, 7, 2, 32767, 2, 100, 0),
(705846, 1048575, 7, 2, 32767, 2, 100, 0),
(706379, 1048575, 7, 2, 32767, 2, 100, 0),
(706680, 1048575, 7, 2, 32767, 2, 100, 0),
(707698, 1048575, 7, 2, 32767, 2, 100, 0),
(806627, 1048575, 7, 2, 32767, 2, 100, 0),
(806629, 1048575, 7, 2, 32767, 2, 100, 0),
(806758, 1048575, 7, 2, 32767, 2, 100, 0);
DELETE FROM `spell_script_names` WHERE `spell_id` = 92138 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (92138, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300877 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300877, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500220 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500220, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500232 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500232, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500232 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500232, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500236 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500236, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500239 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500239, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500535 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500535, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500549 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500549, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500556 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500556, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500557 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500557, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500558 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500558, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500559 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500559, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500560 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500560, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500561 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500561, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500600 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500600, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500612 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500612, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500612 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500612, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502510 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502510, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502511 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502511, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502512 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502512, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502512 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502512, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502513 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502513, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502513 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502513, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502514 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502514, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502514 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502514, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502515 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502515, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502515 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502515, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502516 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502516, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502516 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502516, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502517 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502517, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502517 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502517, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502537 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502537, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502544 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502544, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502545 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502545, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502546 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502546, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502547 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502547, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502548 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502548, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502549 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502549, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502550 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502550, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502551 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502551, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502552 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502552, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502552 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502552, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502553 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502553, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502553 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502553, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502554 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502554, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502554 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502554, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502555 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502555, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502555 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502555, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502556 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502556, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502556 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502556, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502557 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502557, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502557 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502557, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502558 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502558, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502558 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502558, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502559 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502559, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502559 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502559, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502560 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502560, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502560 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502560, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502561 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502561, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502561 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502561, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502562 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502562, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502562 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502562, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502563 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502563, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502563 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502563, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502564 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502564, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502564 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502564, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503552 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503552, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503553 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503553, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504519 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504519, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504523 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504523, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504594 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504594, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504667 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504667, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504667 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504667, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504678 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504678, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504680 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504680, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504749 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504749, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 505160 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (505160, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520445 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520445, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 524835 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (524835, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 537247 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (537247, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 547209 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (547209, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560600 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560600, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560601 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560601, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560602 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560602, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560603 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560603, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560709 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560709, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560742 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560742, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560744 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560744, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560750 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560750, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560754 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560754, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560785 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560785, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572367 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572367, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 573247 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (573247, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 653273 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (653273, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680315 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680315, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680975 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680975, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680998 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680998, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681001 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681001, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681245 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681245, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704107 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704107, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704449 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704449, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705792 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705792, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705810 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705810, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705815 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705815, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705817 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705817, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705820 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705820, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705846 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705846, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706379 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706379, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706647 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706647, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706680 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706680, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706692 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706692, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706742 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706742, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706904 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706904, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707250 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707250, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707261 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707261, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707272 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707272, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707278 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707278, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707290 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707290, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707346 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707346, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707347 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707347, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707348 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707348, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707349 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707349, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707350 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707350, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707495 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707495, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707688 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707688, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707698 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707698, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707698 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707698, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 712289 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (712289, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800349 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800349, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801005 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801005, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801009 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801009, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801384 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801384, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801389 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801389, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801390 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801390, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801709 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801709, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801718 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801718, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801744 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801744, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801745 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801745, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801798 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801798, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801799 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801799, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801801 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801801, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801808 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801808, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801809 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801809, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801809 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801809, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801816 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801816, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801982 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801982, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802052 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802052, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802176 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802176, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802236 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802236, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802477 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802477, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803445 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803445, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803446 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803446, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803447 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803447, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803448 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803448, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803449 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803449, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803450 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803450, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803451 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803451, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803552 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803552, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803658 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803658, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803659 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803659, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803660 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803660, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803661 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803661, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803662 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803662, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803663 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803663, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803664 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803664, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803665 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803665, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803799 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803799, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803800 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803800, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803801 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803801, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803802 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803802, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803803 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803803, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804673 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804673, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804707 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804707, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805308 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805308, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805315 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805315, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805351 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805351, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805372 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805372, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805651 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805651, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805652 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805652, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805653 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805653, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805654 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805654, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805655 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805655, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805656 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805656, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805657 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805657, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806074 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806074, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806627 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806627, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806629 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806629, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806757 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806757, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806758 AND `ScriptName` = 'aura_ascension_tinker_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806758, 'aura_ascension_tinker_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806760 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806760, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807287 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807287, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807288 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807288, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807289 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807289, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807290 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807290, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807291 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807291, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807635 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807635, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807723 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807723, 'spell_ascension_tinker_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 808008 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (808008, 'aura_ascension_tinker_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 850020 AND `ScriptName` = 'spell_ascension_tinker_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (850020, 'spell_ascension_tinker_ability');
DELETE FROM `spell_bonus_data` WHERE `entry` IN (500220, 500232, 500249, 500579, 500601, 500612, 502510, 502511, 502512, 502513, 502514, 502515, 502516, 502517, 502533, 502534, 502535, 502536, 502564, 502573, 502574, 502575, 502576, 502577, 502578, 502579, 502580, 502581, 504667, 505160, 520375, 524903, 547209, 560600, 560601, 560602, 560603, 560709, 560710, 560746, 560786, 561267, 573054, 573268, 574152, 575027, 578335, 680196, 681296, 681297, 681298, 681513, 705847, 706255, 706648, 706689, 706694, 706700, 706829, 707238, 800347, 801005, 801009, 801639, 801707, 801982, 802176, 802477, 802684, 802685, 802686, 802687, 802688, 802689, 802690, 802691, 803639, 805657, 806074, 806628, 806763, 806781);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(500220, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(500232, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(500249, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(500579, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(500601, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(500612, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502510, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502511, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502512, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502513, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502514, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502515, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502516, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502517, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502533, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502534, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502535, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502536, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502564, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502573, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502574, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502575, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502576, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502577, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502578, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502579, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502580, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(502581, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(504667, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(505160, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(520375, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(524903, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(547209, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(560600, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(560601, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(560602, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(560603, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(560709, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(560710, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(560746, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(560786, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(561267, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(573054, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(573268, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(574152, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(575027, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(578335, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(680196, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(681296, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(681297, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(681298, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(681513, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(705847, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(706255, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(706648, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(706689, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(706694, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(706700, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(706829, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(707238, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(800347, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(801005, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(801009, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(801639, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(801707, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(801982, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(802176, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(802477, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(802684, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(802685, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(802686, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(802687, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(802688, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(802689, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(802690, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(802691, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(803639, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(805657, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(806074, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(806628, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(806763, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount'),
(806781, 0, 0, 0, 0, 'Tinker: explicit coefficient or forwarded amount');
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 50046, 'Sentry Turret', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50046);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 50046, 0, 28526, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50046);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 50048, 'Scrapmaw', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_pet' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50048);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 50048, 0, 110049, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50048);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 500481, 'ZIGGI-6K', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_pet' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 500481);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 500481, 0, 274803, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 500481);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 60671, 'Mechano-Bear', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_pet' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 60671);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 60671, 0, 94943, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 60671);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 60070, 'Clockwork Assistant', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_pet' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 60070);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 60070, 0, 40388, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 60070);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 60672, 'Rusthound', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_pet' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 60672);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 60672, 0, 337532, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 60672);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 226312, 'Deathball', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 226312);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 226312, 0, 10045, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 226312);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 500362, 'MY GREATEST INVENTION', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 500362);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 500362, 0, 28830, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 500362);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 500360, 'Replenishment Beacon', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 500360);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 500360, 0, 461411, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 500360);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 500361, 'Alarm Beacon', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 500361);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 500361, 0, 461414, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 500361);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 50600, 'Shrapnel Mine', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50600);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 50600, 0, 421993, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50600);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 500366, 'Repulsion Unit', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 500366);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 500366, 0, 404286, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 500366);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 51204, 'Oil Spill Pylon', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 51204);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 51204, 0, 416909, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 51204);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 50045, 'Blast Mine', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50045);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 50045, 0, 6271, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50045);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 50037, 'Restorative Beacon', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50037);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 50037, 0, 561413, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50037);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 467073, 'Spider Bomb Factory', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 467073);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 467073, 0, 407073, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 467073);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 50300, 'Destructo-Bot', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50300);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 50300, 0, 6977, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50300);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 289612, 'Anti-Block Dummy', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 289612);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 289612, 0, 200003, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 289612);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 506051, 'Battery Recharge Station', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 506051);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 506051, 0, 406700, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 506051);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 500711, 'Clockwork Guardian', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 500711);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 500711, 0, 29118, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 500711);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 52036, 'Noise Box', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 52036);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 52036, 0, 461417, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 52036);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 226012, 'Spider Bomb', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 226012);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 226012, 0, 408331, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 226012);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 226112, 'Firepot Drone', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 226112);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 226112, 0, 22878, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 226112);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 229921, 'Build:Mechsuit', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 229921);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 229921, 0, 916645, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 229921);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 50036, 'Shield Beacon', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50036);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 50036, 0, 461417, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50036);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 51036, 'Shield Beacon', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 51036);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 51036, 0, 461417, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 51036);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 53036, 'Shield Beacon', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 53036);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 53036, 0, 461417, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 53036);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 54036, 'Shield Beacon', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 54036);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 54036, 0, 461417, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 54036);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 55036, 'Shield Beacon', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 55036);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 55036, 0, 461417, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 55036);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 56036, 'Shield Beacon', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 56036);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 56036, 0, 461417, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 56036);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 347770, 'Battle Turret X-13', 1, 80, 35, 0, 9, 116, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 347770);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 347770, 0, 27101, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 347770);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 457771, 'Battle Turret X-13', 1, 80, 35, 0, 9, 116, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 457771);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 457771, 0, 27101, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 457771);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 457772, 'Battle Turret X-13', 1, 80, 35, 0, 9, 116, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 457772);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 457772, 0, 27101, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 457772);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `VehicleId`, `ScriptName`) SELECT 840028, 'Heavy Spider Bomb', 1, 80, 35, 0, 9, 0, 'npc_ascension_tinker_device' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 840028);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 840028, 0, 408331, 1.5, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 840028);
INSERT INTO `creature_template_spell` (`CreatureID`, `Index`, `Spell`) SELECT 50300, 0, 802176 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_spell` WHERE `CreatureID` = 50300 AND `Index` = 0);
INSERT INTO `creature_template_spell` (`CreatureID`, `Index`, `Spell`) SELECT 347770, 0, 706689 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_spell` WHERE `CreatureID` = 347770 AND `Index` = 0);
INSERT INTO `creature_template_spell` (`CreatureID`, `Index`, `Spell`) SELECT 347770, 1, 706694 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_spell` WHERE `CreatureID` = 347770 AND `Index` = 1);
INSERT INTO `creature_template_spell` (`CreatureID`, `Index`, `Spell`) SELECT 457771, 0, 706689 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_spell` WHERE `CreatureID` = 457771 AND `Index` = 0);
INSERT INTO `creature_template_spell` (`CreatureID`, `Index`, `Spell`) SELECT 457771, 1, 706694 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_spell` WHERE `CreatureID` = 457771 AND `Index` = 1);
INSERT INTO `creature_template_spell` (`CreatureID`, `Index`, `Spell`) SELECT 457772, 0, 706689 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_spell` WHERE `CreatureID` = 457772 AND `Index` = 0);
INSERT INTO `creature_template_spell` (`CreatureID`, `Index`, `Spell`) SELECT 457772, 1, 706694 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_spell` WHERE `CreatureID` = 457772 AND `Index` = 1);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `ScriptName`) SELECT 2201005, 8, 1248, 'Portable Sawmill', 1, 1653, 10, '' WHERE NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry` = 2201005);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `ScriptName`) SELECT 9000007, 10, 1010827, 'Battery Recharge Station', 1, 0, 0, 'go_ascension_tinker_battery' WHERE NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry` = 9000007);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) SELECT 40388, 0.61111, 2.03128, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 40388);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) SELECT 94943, 0.61111, 2.03128, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 94943);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) SELECT 110049, 2.03128, 1.0, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 110049);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) SELECT 200003, 1.0, 2.0, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 200003);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) SELECT 337532, 2.03128, 1.25, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 337532);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) SELECT 404286, 0.3, 1.0, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 404286);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) SELECT 406700, 0.3, 1.0, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 406700);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) SELECT 407073, 0.3, 1.0, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 407073);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) SELECT 408331, 0.3, 1.0, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 408331);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) SELECT 416909, 0.3, 1.0, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 416909);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) SELECT 421993, 0.3, 1.0, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 421993);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) SELECT 461411, 0.3, 1.0, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 461411);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) SELECT 461414, 0.3, 1.0, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 461414);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) SELECT 461417, 0.3, 1.0, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 461417);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) SELECT 561413, 0.3, 1.0, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 561413);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) SELECT 916645, 0.6111, 2.031, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 916645);
-- Native charged/enchantment scripts retain their logic; admit resolved blocks and absorbs too.
UPDATE `spell_proc` SET `HitMask` = 9283 WHERE `SpellId` IN (653232, 653235, 653237, 653243, 653246, 801827, 707403, 805305);
