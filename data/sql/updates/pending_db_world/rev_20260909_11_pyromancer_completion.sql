-- Pyromancer completion: new unapplied SQL11, requires matching source.
DELETE FROM `spell_proc` WHERE `SpellId` IN (92124, 300755, 300757, 300932, 300975, 301212, 503803, 503806, 503915, 504380, 504394, 504720, 504733, 504750, 520381, 520770, 520823, 520927, 524623, 524624, 524707, 524818, 524874, 524994, 535650, 535651, 538441, 560525, 570001, 572381, 572582, 572806, 573283, 573284, 582762, 582763, 582764, 680366, 680367, 680378, 680387, 681196, 681334, 704275, 704800, 704801, 704807, 704813, 704823, 704855, 704856, 704863, 706239, 706464, 706894, 707432, 707478, 707483, 707493, 707651, 801687, 802068, 802117, 802164, 802168, 802780, 804300, 805448, 805468, 806736, 806747, 806783, 807126, 807146, 807147, 807319, 807400, 807686);
INSERT INTO `spell_proc` (`SpellId`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `Chance`) VALUES
(300755, 1048575, 7, 2, 9331, 2, 100),
(300757, 1048575, 7, 2, 9331, 2, 100),
(300932, 1048575, 7, 2, 9331, 2, 100),
(300975, 1048575, 7, 2, 9331, 2, 100),
(504380, 1048575, 7, 2, 9331, 2, 100),
(504720, 1048575, 7, 2, 9331, 2, 100),
(504733, 1048575, 7, 2, 9331, 2, 100),
(524624, 1048575, 7, 2, 9331, 2, 100),
(535650, 1048575, 7, 2, 9331, 2, 100),
(535651, 1048575, 7, 2, 9331, 2, 100),
(538441, 1048575, 7, 2, 9331, 2, 100),
(572582, 1048575, 7, 2, 9331, 2, 100),
(582762, 1048575, 7, 2, 9331, 2, 100),
(582763, 1048575, 7, 2, 9331, 2, 100),
(582764, 1048575, 7, 2, 9331, 2, 100),
(680378, 1048575, 7, 2, 9331, 2, 100),
(680387, 1048575, 7, 2, 9331, 2, 100),
(704275, 1048575, 7, 2, 9331, 2, 100),
(704800, 1048575, 7, 2, 9331, 2, 100),
(704801, 1048575, 7, 2, 9331, 2, 100),
(704855, 1048575, 7, 2, 9331, 2, 100),
(704856, 1048575, 7, 2, 9331, 2, 100),
(704863, 1048575, 7, 2, 9331, 2, 100),
(706239, 1048575, 7, 2, 9331, 2, 100),
(706464, 1048575, 7, 2, 9331, 2, 100),
(707432, 1048575, 7, 2, 9331, 2, 100),
(707483, 1048575, 7, 2, 9331, 2, 100),
(802068, 1048575, 7, 2, 9331, 2, 100),
(802117, 1048575, 7, 2, 9331, 2, 100),
(805468, 1048575, 7, 2, 9331, 2, 100),
(806736, 1048575, 7, 2, 9331, 2, 100),
(807146, 1048575, 7, 2, 9331, 2, 100),
(807400, 1048575, 7, 2, 9331, 2, 100);
DELETE FROM `spell_script_names` WHERE `spell_id` = 92126 AND `ScriptName` = 'aura_ascension_pyromancer_phoenix';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (92126, 'aura_ascension_pyromancer_phoenix');
DELETE FROM `spell_script_names` WHERE `spell_id` = 92128 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (92128, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300755 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300755, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300757 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300757, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300932 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300932, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300975 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300975, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502020 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502020, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502021 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502021, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502022 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502022, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502023 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502023, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502024 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502024, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502025 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502025, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502026 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502026, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502027 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502027, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502028 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502028, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502029 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502029, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502030 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502030, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502031 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502031, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504380 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504380, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504707 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504707, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504720 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504720, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504720 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504720, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504733 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504733, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520019 AND `ScriptName` = 'spell_ascension_pyromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520019, 'spell_ascension_pyromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520823 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520823, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520826 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520826, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520868 AND `ScriptName` = 'spell_ascension_pyromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520868, 'spell_ascension_pyromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520927 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520927, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520937 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520937, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 521642 AND `ScriptName` = 'spell_ascension_pyromancer_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (521642, 'spell_ascension_pyromancer_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 524624 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (524624, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 524707 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (524707, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 534600 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (534600, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 534601 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (534601, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 534602 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (534602, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 534603 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (534603, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 534604 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (534604, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 535650 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (535650, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 535651 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (535651, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 538441 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (538441, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572159 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572159, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572160 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572160, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572381 AND `ScriptName` = 'spell_ascension_pyromancer_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572381, 'spell_ascension_pyromancer_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572582 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572582, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572806 AND `ScriptName` = 'spell_ascension_pyromancer_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572806, 'spell_ascension_pyromancer_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 573220 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (573220, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 573284 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (573284, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 582762 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (582762, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 582763 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (582763, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 582764 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (582764, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680365 AND `ScriptName` = 'spell_ascension_pyromancer_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680365, 'spell_ascension_pyromancer_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680369 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680369, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680378 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680378, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680378 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680378, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680387 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680387, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680387 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680387, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680842 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680842, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680962 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680962, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681314 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681314, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704275 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704275, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704278 AND `ScriptName` = 'spell_ascension_pyromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704278, 'spell_ascension_pyromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704800 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704800, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704801 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704801, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704855 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704855, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704856 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704856, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704863 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704863, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706239 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706239, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706464 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706464, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706854 AND `ScriptName` = 'spell_ascension_pyromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706854, 'spell_ascension_pyromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706874 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706874, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707432 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707432, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707478 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707478, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707483 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707483, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800791 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800791, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800796 AND `ScriptName` = 'spell_ascension_pyromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800796, 'spell_ascension_pyromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800816 AND `ScriptName` = 'spell_ascension_pyromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800816, 'spell_ascension_pyromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801911 AND `ScriptName` = 'spell_ascension_pyromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801911, 'spell_ascension_pyromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802068 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802068, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802117 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802117, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802120 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802120, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802168 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802168, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802791 AND `ScriptName` = 'spell_ascension_pyromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802791, 'spell_ascension_pyromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804301 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804301, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804301 AND `ScriptName` = 'spell_ascension_pyromancer_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804301, 'spell_ascension_pyromancer_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805468 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805468, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805500 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805500, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806736 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806736, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806783 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806783, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807146 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807146, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807146 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807146, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807147 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807147, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807390 AND `ScriptName` = 'spell_ascension_pyromancer_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807390, 'spell_ascension_pyromancer_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807391 AND `ScriptName` = 'spell_ascension_pyromancer_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807391, 'spell_ascension_pyromancer_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807392 AND `ScriptName` = 'spell_ascension_pyromancer_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807392, 'spell_ascension_pyromancer_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807393 AND `ScriptName` = 'spell_ascension_pyromancer_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807393, 'spell_ascension_pyromancer_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807394 AND `ScriptName` = 'spell_ascension_pyromancer_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807394, 'spell_ascension_pyromancer_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807400 AND `ScriptName` = 'aura_ascension_pyromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807400, 'aura_ascension_pyromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807400 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807400, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807401 AND `ScriptName` = 'spell_ascension_pyromancer_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807401, 'spell_ascension_pyromancer_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807402 AND `ScriptName` = 'spell_ascension_pyromancer_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807402, 'spell_ascension_pyromancer_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807403 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807403, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807412 AND `ScriptName` = 'spell_ascension_pyromancer_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807412, 'spell_ascension_pyromancer_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807533 AND `ScriptName` = 'spell_ascension_pyromancer_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807533, 'spell_ascension_pyromancer_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807535 AND `ScriptName` = 'spell_ascension_pyromancer_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807535, 'spell_ascension_pyromancer_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807536 AND `ScriptName` = 'spell_ascension_pyromancer_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807536, 'spell_ascension_pyromancer_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807613 AND `ScriptName` = 'spell_ascension_pyromancer_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807613, 'spell_ascension_pyromancer_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807768 AND `ScriptName` = 'spell_ascension_pyromancer_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807768, 'spell_ascension_pyromancer_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807944 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807944, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 808064 AND `ScriptName` = 'spell_ascension_pyromancer_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (808064, 'spell_ascension_pyromancer_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 1119751 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (1119751, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 1119754 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (1119754, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 1119755 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (1119755, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 1119756 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (1119756, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 1119757 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (1119757, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 1119758 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (1119758, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 1119901 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (1119901, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 1119944 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (1119944, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 1119953 AND `ScriptName` = 'aura_ascension_pyromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (1119953, 'aura_ascension_pyromancer_lifecycle');
DELETE FROM `spell_bonus_data` WHERE `entry` IN (502011, 502012, 502013, 502014, 502015, 502016, 502017, 502018, 502019, 502044, 502045, 502046, 502047, 502048, 502049, 502050, 502051, 502052, 502057, 502058, 502059, 502060, 502061, 502062, 502063, 502085, 502086, 502087, 502107, 502108, 502109, 502110, 502111, 502112, 502113, 503231, 503232, 503233, 503234, 503235, 503236, 503237, 503238, 503239, 503864, 504380, 520320, 520403, 520751, 520826, 524623, 534600, 534601, 534602, 534603, 534604, 535509, 535510, 535511, 535512, 535650, 535651, 556833, 556834, 556835, 556836, 556837, 567589, 567590, 570750, 572159, 572160, 572161, 572618, 572619, 572620, 572621, 572622, 572623, 572890, 572891, 572892, 572893, 572894, 578307, 578308, 578309, 582762, 582763, 582764, 680366, 680370, 680371, 680842, 680962, 681141, 681142, 704274, 704277, 704279, 704817, 706855, 706856, 706874, 707110, 707595, 707892, 712482, 800790, 800792, 800806, 800818, 801687, 801905, 801915, 801929, 802107, 802173, 802174, 803407, 803408, 803409, 803410, 803455, 803459, 803547, 803704, 803819, 803820, 803821, 803822, 803823, 803824, 803825, 803950, 804076, 804103, 805477, 805496, 805500, 806611, 806742, 806743, 806749, 807403, 807406, 807407, 807408, 807409, 807410, 807540, 807615, 807616, 807617, 807618, 807619, 807620, 807621, 807622, 807623);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(502011, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502012, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502013, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502014, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502015, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502016, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502017, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502018, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502019, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502044, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502045, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502046, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502047, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502048, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502049, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502050, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502051, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502052, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502057, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502058, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502059, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502060, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502061, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502062, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502063, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502085, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502086, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502087, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502107, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502108, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502109, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502110, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502111, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502112, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(502113, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(503231, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(503232, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(503233, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(503234, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(503235, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(503236, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(503237, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(503238, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(503239, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(503864, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(504380, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(520320, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(520403, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(520751, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(520826, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(524623, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(534600, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(534601, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(534602, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(534603, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(534604, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(535509, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(535510, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(535511, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(535512, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(535650, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(535651, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(556833, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(556834, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(556835, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(556836, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(556837, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(567589, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(567590, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(570750, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(572159, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(572160, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(572161, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(572618, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(572619, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(572620, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(572621, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(572622, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(572623, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(572890, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(572891, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(572892, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(572893, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(572894, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(578307, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(578308, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(578309, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(582762, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(582763, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(582764, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(680366, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(680370, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(680371, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(680842, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(680962, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(681141, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(681142, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(704274, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(704277, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(704279, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(704817, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(706855, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(706856, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(706874, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(707110, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(707595, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(707892, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(712482, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(800790, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(800792, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(800806, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(800818, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(801687, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(801905, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(801915, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(801929, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(802107, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(802173, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(802174, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(803407, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(803408, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(803409, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(803410, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(803455, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(803459, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(803547, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(803704, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(803819, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(803820, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(803821, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(803822, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(803823, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(803824, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(803825, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(803950, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(804076, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(804103, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(805477, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(805496, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(805500, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(806611, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(806742, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(806743, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(806749, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(807403, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(807406, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(807407, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(807408, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(807409, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(807410, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(807540, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(807615, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(807616, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(807617, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(807618, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(807619, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(807620, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(807621, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(807622, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result'),
(807623, 0, 0, 0, 0, 'Pyromancer: explicit coefficient or result');
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `BaseAttackTime`, `RangeAttackTime`, `unit_class`, `unit_flags`, `type`, `ScriptName`) SELECT 50359, 'Roaring Pyre', 1, 80, 35, 0, 1, 1.14286, 2000, 2000, 1, 0, 4, 'npc_ascension_pyromancer_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50359);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 50359, 0, 1405, 0.35, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50359);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `BaseAttackTime`, `RangeAttackTime`, `unit_class`, `unit_flags`, `type`, `ScriptName`) SELECT 50258, 'Phoenix Egg', 1, 80, 35, 0, 1, 1.14286, 2000, 2000, 1, 0, 4, 'npc_ascension_pyromancer_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50258);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 50258, 0, 20245, 0.6, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50258);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `BaseAttackTime`, `RangeAttackTime`, `unit_class`, `unit_flags`, `type`, `ScriptName`) SELECT 52258, 'Firestorm', 1, 80, 35, 0, 1, 1.14286, 2000, 2000, 1, 0, 4, 'npc_ascension_pyromancer_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 52258);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 52258, 0, 1405, 0.8, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 52258);
