-- Starcaller completion: new unapplied migration; matching source is required.
DELETE FROM `spell_proc` WHERE `SpellId` IN (300252, 300992, 500205, 503779, 504001, 504004, 504006, 504628, 504786, 520481, 524642, 524781, 536216, 536217, 560440, 560501, 560539, 560951, 560956, 561022, 570231, 572319, 680212, 680215, 680708, 680711, 680725, 680742, 680784, 680812, 680821, 680847, 704171, 704725, 704739, 704740, 704741, 704764, 704765, 704777, 704784, 706230, 706574, 706575, 707637, 707639, 800394, 800504, 800510, 801123, 801128, 801143, 801145, 801155, 801231, 801975, 802680, 803887, 803888, 804733, 804735, 805356, 805437, 805439, 805505, 806155, 806738, 807160, 807659);
INSERT INTO `spell_proc` (`SpellId`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `Chance`) VALUES
(300992, 1048575, 7, 2, 9331, 2, 100),
(504001, 1048575, 7, 2, 9331, 2, 100),
(504628, 1048575, 7, 2, 9331, 2, 100),
(504786, 1048575, 7, 2, 9331, 2, 100),
(524642, 1048575, 7, 2, 9331, 2, 100),
(536216, 1048575, 7, 2, 9331, 2, 100),
(560440, 1048575, 7, 2, 9331, 2, 100),
(560501, 1048575, 7, 2, 9331, 2, 100),
(560539, 1048575, 7, 2, 9331, 2, 100),
(560956, 1048575, 7, 2, 9331, 2, 100),
(561022, 1048575, 7, 2, 9331, 2, 100),
(680784, 1048575, 7, 2, 9331, 2, 100),
(801145, 1048575, 7, 2, 9331, 2, 100),
(801231, 1048575, 7, 2, 9331, 2, 100),
(805439, 1048575, 7, 2, 9331, 2, 100),
(805505, 1048575, 7, 2, 9331, 2, 100),
(807659, 1048575, 7, 2, 9331, 2, 100),
(806155, 1048575, 7, 2, 9331, 2, 100),
(704777, 1048575, 7, 2, 9331, 2, 100),
(801155, 1048575, 7, 2, 9331, 2, 100),
(801128, 1048575, 7, 2, 9331, 2, 100),
(805356, 1048575, 7, 2, 9331, 2, 100),
(801123, 1048575, 7, 2, 9331, 2, 100),
(800510, 1048575, 7, 2, 9331, 2, 100),
(803887, 1048575, 7, 2, 9331, 2, 100),
(803888, 1048575, 7, 2, 9331, 2, 100),
(800394, 1048575, 7, 2, 9331, 2, 100),
(804735, 1048575, 7, 2, 9331, 2, 100),
(804733, 1048575, 7, 2, 9331, 2, 100),
(706230, 1048575, 7, 2, 9331, 2, 100),
(801143, 1048575, 7, 2, 9331, 2, 100),
(706575, 1048575, 7, 2, 9331, 2, 100),
(704741, 1048575, 7, 2, 9331, 2, 100),
(680215, 1048575, 7, 2, 9331, 2, 100),
(536217, 1048575, 7, 2, 9331, 2, 100);
DELETE FROM `spell_script_names` WHERE `spell_id` = 300256 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300256, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300773 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300773, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300992 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300992, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500206 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500206, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502295 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502295, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502296 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502296, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502297 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502297, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502333 AND `ScriptName` = 'spell_ascension_starcaller_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502333, 'spell_ascension_starcaller_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502334 AND `ScriptName` = 'spell_ascension_starcaller_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502334, 'spell_ascension_starcaller_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502335 AND `ScriptName` = 'spell_ascension_starcaller_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502335, 'spell_ascension_starcaller_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502336 AND `ScriptName` = 'spell_ascension_starcaller_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502336, 'spell_ascension_starcaller_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502337 AND `ScriptName` = 'spell_ascension_starcaller_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502337, 'spell_ascension_starcaller_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502338 AND `ScriptName` = 'spell_ascension_starcaller_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502338, 'spell_ascension_starcaller_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502339 AND `ScriptName` = 'spell_ascension_starcaller_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502339, 'spell_ascension_starcaller_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502340 AND `ScriptName` = 'spell_ascension_starcaller_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502340, 'spell_ascension_starcaller_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503780 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503780, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504001 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504001, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504628 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504628, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504630 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504630, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504631 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504631, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504786 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504786, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 524642 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (524642, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 524781 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (524781, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 536216 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (536216, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 536217 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (536217, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 536217 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (536217, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560440 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560440, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560501 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560501, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560539 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560539, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560634 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560634, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560956 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560956, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561022 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561022, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561046 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561046, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570184 AND `ScriptName` = 'spell_ascension_starcaller_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570184, 'spell_ascension_starcaller_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572315 AND `ScriptName` = 'spell_ascension_starcaller_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572315, 'spell_ascension_starcaller_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572319 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572319, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 574349 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (574349, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 574351 AND `ScriptName` = 'aura_ascension_starcaller_absorb';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (574351, 'aura_ascension_starcaller_absorb');
DELETE FROM `spell_script_names` WHERE `spell_id` = 574351 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (574351, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680213 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680213, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680215 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680215, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680705 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680705, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680713 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680713, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680784 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680784, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680786 AND `ScriptName` = 'aura_ascension_starcaller_absorb';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680786, 'aura_ascension_starcaller_absorb');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680786 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680786, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680803 AND `ScriptName` = 'spell_ascension_starcaller_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680803, 'spell_ascension_starcaller_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680821 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680821, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680822 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680822, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680847 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680847, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704171 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704171, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704741 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704741, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704772 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704772, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704777 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704777, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704785 AND `ScriptName` = 'aura_ascension_starcaller_absorb';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704785, 'aura_ascension_starcaller_absorb');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704785 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704785, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706230 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706230, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706301 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706301, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706575 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706575, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707425 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707425, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800386 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800386, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800393 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800393, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800394 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800394, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800394 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800394, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800505 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800505, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800510 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800510, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801123 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801123, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801123 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801123, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801125 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801125, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801128 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801128, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801132 AND `ScriptName` = 'spell_ascension_starcaller_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801132, 'spell_ascension_starcaller_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801143 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801143, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801145 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801145, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801148 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801148, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801155 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801155, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801231 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801231, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801243 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801243, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802681 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802681, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802985 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802985, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803573 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803573, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803887 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803887, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803888 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803888, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803915 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803915, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803916 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803916, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803917 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803917, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804716 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804716, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804733 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804733, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804735 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804735, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804735 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804735, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805356 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805356, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805433 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805433, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805439 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805439, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805439 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805439, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805505 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805505, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805546 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805546, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806155 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806155, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807659 AND `ScriptName` = 'aura_ascension_starcaller_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807659, 'aura_ascension_starcaller_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807992 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807992, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 954791 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (954791, 'aura_ascension_starcaller_lifecycle');
DELETE FROM `spell_bonus_data` WHERE `entry` IN (100250, 302568, 302569, 502316, 502317, 502333, 502334, 502335, 502336, 502337, 502338, 502339, 502340, 502348, 502349, 502350, 502351, 503019, 503020, 503021, 503022, 503023, 503024, 503025, 503026, 503027, 503028, 503029, 504002, 520422, 524703, 531757, 562319, 562320, 562321, 562322, 563725, 574165, 574327, 574328, 575030, 575031, 575032, 575033, 575034, 575035, 575036, 575037, 575038, 575039, 575048, 575049, 575050, 575051, 575052, 575334, 680214, 680220, 680586, 680587, 680588, 680589, 680703, 680707, 681541, 681542, 681543, 681544, 681545, 704232, 707759, 800370, 800497, 800506, 800507, 801125, 801127, 801129, 801132, 801148, 801401, 801972, 801978, 801987, 801990, 801996, 802470, 802471, 802472, 802473, 802682, 803264, 803918, 803919, 803920, 803921, 803922, 803923, 803924, 803925, 804463, 804464, 804465, 804466, 804467, 804468, 804469, 804736, 804995, 805006, 805357, 805433, 805548, 805550, 805563, 806156, 806233, 806806, 806807, 806808, 806809, 806810, 806811, 807672, 807816, 807992);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(100250, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(302568, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(302569, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(502316, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(502317, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(502333, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(502334, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(502335, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(502336, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(502337, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(502338, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(502339, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(502340, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(502348, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(502349, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(502350, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(502351, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(503019, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(503020, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(503021, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(503022, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(503023, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(503024, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(503025, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(503026, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(503027, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(503028, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(503029, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(504002, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(520422, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(524703, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(531757, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(562319, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(562320, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(562321, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(562322, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(563725, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(574165, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(574327, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(574328, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(575030, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(575031, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(575032, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(575033, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(575034, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(575035, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(575036, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(575037, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(575038, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(575039, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(575048, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(575049, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(575050, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(575051, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(575052, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(575334, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(680214, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(680220, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(680586, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(680587, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(680588, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(680589, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(680703, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(680707, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(681541, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(681542, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(681543, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(681544, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(681545, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(704232, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(707759, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(800370, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(800497, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(800506, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(800507, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(801125, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(801127, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(801129, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(801132, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(801148, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(801401, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(801972, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(801978, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(801987, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(801990, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(801996, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(802470, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(802471, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(802472, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(802473, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(802682, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(803264, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(803918, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(803919, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(803920, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(803921, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(803922, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(803923, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(803924, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(803925, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(804463, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(804464, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(804465, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(804466, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(804467, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(804468, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(804469, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(804736, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(804995, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(805006, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(805357, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(805433, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(805548, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(805550, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(805563, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(806156, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(806233, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(806806, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(806807, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(806808, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(806809, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(806810, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(806811, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(807672, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(807816, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result'),
(807992, 0, 0, 0, 0, 'Starcaller: explicit coefficient or result');
DELETE FROM `spell_group` WHERE `id` = 2000184 AND `spell_id` = 704785;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES (2000184, 704785);
