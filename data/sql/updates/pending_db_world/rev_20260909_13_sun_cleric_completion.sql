-- Sun Cleric completion: new unapplied SQL13, requires matching source.
DELETE FROM `spell_proc` WHERE `SpellId` IN (92135, 92136, 92137, 300318, 300334, 300336, 300339, 300344, 300349, 300354, 300355, 300360, 300364, 300367, 300369, 300371, 300943, 301005, 301239, 301242, 301256, 301265, 301266, 301293, 301301, 503690, 503691, 505342, 520639, 534267, 547214, 560095, 560111, 560124, 560492, 560861, 561327, 572885, 582832, 680622, 680624, 680641, 680642, 680643, 680646, 680648, 680656, 680663, 680888, 680911, 681252, 681253, 681385, 681436, 681471, 704562, 704563, 704586, 704903, 704908, 704920, 704928, 704929, 704934, 704935, 704936, 704941, 707077, 707078, 707081, 707414, 707521, 707629, 800601, 800612, 800722, 802935, 803238, 803489, 803491, 803492, 803500, 803719, 804625, 804628, 804629, 804630, 804631, 804632, 805481, 805491, 805581, 805647, 806021, 806024, 806058, 806116, 806123, 806699, 807059, 807061, 807077, 807080, 807299, 807343, 807435, 807440, 807444, 807446, 807449, 807451, 807523, 807547, 807749, 807750, 807751, 807752);
INSERT INTO `spell_proc` (`SpellId`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `Chance`, `Charges`) VALUES
(92137, 1048575, 7, 2, 32767, 2, 100, 0),
(300336, 1048575, 7, 2, 32767, 2, 100, 0),
(300339, 1048575, 7, 2, 32767, 2, 100, 0),
(300349, 1048575, 7, 2, 32767, 2, 100, 0),
(300355, 1048575, 7, 2, 32767, 2, 100, 0),
(300369, 1048575, 7, 2, 32767, 2, 100, 0),
(300371, 1048575, 7, 2, 32767, 2, 100, 0),
(300943, 1048575, 7, 2, 32767, 2, 100, 0),
(301005, 1048575, 7, 2, 32767, 2, 100, 0),
(301239, 1048575, 7, 2, 32767, 2, 100, 0),
(301266, 1048575, 7, 2, 32767, 2, 100, 0),
(505342, 1048575, 7, 2, 32767, 2, 100, 0),
(534267, 1048575, 7, 2, 32767, 2, 100, 0),
(547214, 1048575, 7, 2, 32767, 2, 100, 0),
(560095, 1048575, 7, 2, 32767, 2, 100, 5),
(560492, 1048575, 7, 2, 32767, 2, 100, 0),
(560861, 1048575, 7, 2, 32767, 2, 100, 0),
(561327, 1048575, 7, 2, 32767, 2, 100, 0),
(572885, 1048575, 7, 2, 32767, 2, 100, 0),
(582832, 1048575, 7, 2, 32767, 2, 100, 0),
(680624, 1048575, 7, 2, 32767, 2, 100, 0),
(680641, 1048575, 7, 2, 32767, 2, 100, 0),
(680643, 1048575, 7, 2, 32767, 2, 100, 0),
(680646, 1048575, 7, 2, 32767, 2, 100, 0),
(680663, 1048575, 7, 2, 32767, 2, 100, 0),
(680888, 1048575, 7, 2, 32767, 2, 100, 1),
(680911, 1048575, 7, 2, 32767, 2, 100, 0),
(681252, 1048575, 7, 2, 32767, 2, 100, 0),
(704563, 1048575, 7, 2, 32767, 2, 100, 0),
(704903, 1048575, 7, 2, 32767, 2, 100, 0),
(704920, 1048575, 7, 2, 32767, 2, 100, 0),
(704928, 1048575, 7, 2, 32767, 2, 100, 0),
(704934, 1048575, 7, 2, 32767, 2, 100, 0),
(704936, 1048575, 7, 2, 32767, 2, 100, 0),
(704941, 1048575, 7, 2, 32767, 2, 100, 0),
(707077, 1048575, 7, 2, 32767, 2, 100, 0),
(707629, 1048575, 7, 2, 32767, 2, 100, 0),
(800612, 1048575, 7, 2, 32767, 2, 100, 0),
(802935, 1048575, 7, 2, 32767, 2, 100, 0),
(803489, 1048575, 7, 2, 32767, 2, 100, 0),
(803491, 1048575, 7, 2, 32767, 2, 100, 0),
(803719, 1048575, 7, 2, 32767, 2, 100, 0),
(804629, 1048575, 7, 2, 32767, 2, 100, 0),
(804632, 1048575, 7, 2, 32767, 2, 100, 0),
(805647, 1048575, 7, 2, 32767, 2, 100, 0),
(806024, 1048575, 7, 2, 32767, 2, 100, 0),
(806058, 1048575, 7, 2, 32767, 2, 100, 0),
(806116, 1048575, 7, 2, 32767, 2, 100, 0),
(806123, 1048575, 7, 2, 32767, 2, 100, 0),
(806699, 1048575, 7, 2, 32767, 2, 100, 0),
(807343, 1048575, 7, 2, 32767, 2, 100, 0),
(807435, 1048575, 7, 2, 32767, 2, 100, 0),
(807547, 1048575, 7, 2, 32767, 2, 100, 0),
(807749, 1048575, 7, 2, 32767, 2, 100, 0);
DELETE FROM `spell_script_names` WHERE `spell_id` = 92137 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (92137, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300314 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300314, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300334 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300334, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300336 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300336, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300339 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300339, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300349 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300349, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300351 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300351, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300354 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300354, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300355 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300355, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300361 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300361, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300363 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300363, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300369 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300369, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300371 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300371, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300621 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300621, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300626 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300626, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300858 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300858, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300859 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300859, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300862 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300862, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300863 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300863, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300864 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300864, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300865 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300865, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300866 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300866, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300943 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300943, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 301005 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (301005, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 301005 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (301005, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 301239 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (301239, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 301242 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (301242, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 301265 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (301265, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 301266 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (301266, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 301266 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (301266, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 301293 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (301293, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 301345 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (301345, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 301817 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (301817, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500477 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500477, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500511 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500511, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503498 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503498, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503499 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503499, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503500 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503500, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503501 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503501, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503502 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503502, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503503 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503503, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503504 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503504, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503505 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503505, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503506 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503506, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503507 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503507, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503508 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503508, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503691 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503691, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 505340 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (505340, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 505342 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (505342, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520024 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520024, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520026 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520026, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520647 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520647, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520647 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520647, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 534267 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (534267, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 547214 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (547214, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560095 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560095, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560095 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560095, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560123 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560123, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560347 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560347, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560492 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560492, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560861 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560861, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561023 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561023, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561327 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561327, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561328 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561328, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570125 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570125, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570187 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570187, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572384 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572384, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572385 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572385, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572386 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572386, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572387 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572387, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572388 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572388, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572389 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572389, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572390 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572390, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572552 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572552, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572553 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572553, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572752 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572752, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572754 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572754, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572885 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572885, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 575040 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (575040, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 575041 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (575041, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 575042 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (575042, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 575043 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (575043, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 575044 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (575044, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 575045 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (575045, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 582832 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (582832, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680624 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680624, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680624 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680624, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680630 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680630, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680639 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680639, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680641 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680641, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680642 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680642, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680643 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680643, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680646 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680646, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680663 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680663, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680888 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680888, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680911 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680911, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680911 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680911, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681160 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681160, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681252 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681252, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681254 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681254, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681506 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681506, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704563 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704563, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704585 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704585, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704903 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704903, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704920 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704920, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704928 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704928, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704930 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704930, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704934 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704934, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704935 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704935, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704936 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704936, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704941 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704941, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707077 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707077, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707078 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707078, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707629 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707629, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800601 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800601, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800612 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800612, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800722 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800722, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800764 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800764, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800764 AND `ScriptName` = 'spell_ascension_sun_cleric_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800764, 'spell_ascension_sun_cleric_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800852 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800852, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802161 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802161, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802598 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802598, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802935 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802935, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803489 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803489, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803489 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803489, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803491 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803491, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803491 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803491, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803492 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803492, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803500 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803500, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803719 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803719, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803719 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803719, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804097 AND `ScriptName` = 'spell_ascension_sun_cleric_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804097, 'spell_ascension_sun_cleric_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804098 AND `ScriptName` = 'spell_ascension_sun_cleric_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804098, 'spell_ascension_sun_cleric_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804247 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804247, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804249 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804249, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804250 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804250, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804252 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804252, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804253 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804253, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804254 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804254, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804584 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804584, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804629 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804629, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804631 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804631, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804632 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804632, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805267 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805267, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805301 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805301, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805647 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805647, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806024 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806024, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806058 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806058, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806116 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806116, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806121 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806121, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806123 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806123, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806477 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806477, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806699 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806699, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807059 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807059, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807061 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807061, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807064 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807064, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807077 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807077, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807080 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807080, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807299 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807299, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807343 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807343, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807367 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807367, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807368 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807368, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807369 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807369, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807370 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807370, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807371 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807371, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807372 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807372, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807435 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807435, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807435 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807435, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807436 AND `ScriptName` = 'spell_ascension_sun_cleric_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807436, 'spell_ascension_sun_cleric_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807440 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807440, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807441 AND `ScriptName` = 'spell_ascension_sun_cleric_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807441, 'spell_ascension_sun_cleric_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807446 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807446, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807523 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807523, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807547 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807547, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807547 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807547, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807749 AND `ScriptName` = 'aura_ascension_sun_cleric_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807749, 'aura_ascension_sun_cleric_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807749 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807749, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807750 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807750, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807751 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807751, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807752 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807752, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_bonus_data` WHERE `entry` IN (500141, 500143, 500146, 500147, 500152, 500154, 502366, 502367, 502368, 502369, 502370, 502371, 502380, 502381, 502382, 502383, 502384, 502385, 502386, 502387, 502388, 502389, 502390, 502391, 502392, 502393, 502394, 502395, 502396, 502397, 502407, 502408, 502409, 502410, 502411, 502421, 502422, 502423, 502424, 502425, 502426, 502427, 502428, 502429, 502430, 502431, 502432, 502433, 502440, 502441, 502449, 502450, 502451, 502452, 502453, 502454, 502455, 502456, 502496, 502497, 502498, 502499, 503623, 503624, 503625, 504579, 504580, 504764, 505197, 505340, 505341, 505343, 520641, 520648, 524857, 525005, 525006, 525007, 554406, 560493, 560862, 560863, 560864, 560865, 560866, 560867, 570034, 570147, 570187, 572035, 572040, 572041, 572042, 572043, 572044, 572045, 572157, 572753, 572796, 572801, 572849, 572850, 657125, 680621, 681386, 681432, 681433, 704911, 704926, 704931, 707522, 707774, 800038, 800231, 800357, 800654, 801215, 802598, 803490, 803493, 803816, 804252, 804752, 805489, 805583, 805639, 806060, 806477, 806479, 806980, 807058, 807064, 807078, 807175, 807215, 807367, 807368, 807369, 807370, 807371, 807372, 807853, 807867, 807868, 807869, 807870, 807994);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(500141, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(500143, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(500146, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(500147, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(500152, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(500154, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502366, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502367, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502368, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502369, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502370, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502371, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502380, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502381, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502382, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502383, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502384, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502385, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502386, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502387, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502388, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502389, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502390, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502391, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502392, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502393, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502394, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502395, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502396, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502397, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502407, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502408, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502409, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502410, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502411, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502421, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502422, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502423, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502424, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502425, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502426, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502427, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502428, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502429, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502430, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502431, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502432, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502433, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502440, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502441, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502449, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502450, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502451, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502452, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502453, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502454, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502455, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502456, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502496, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502497, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502498, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(502499, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(503623, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(503624, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(503625, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(504579, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(504580, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(504764, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(505197, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(505340, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(505341, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(505343, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(520641, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(520648, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(524857, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(525005, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(525006, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(525007, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(554406, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(560493, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(560862, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(560863, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(560864, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(560865, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(560866, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(560867, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(570034, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(570147, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(570187, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(572035, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(572040, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(572041, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(572042, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(572043, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(572044, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(572045, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(572157, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(572753, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(572796, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(572801, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(572849, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(572850, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(657125, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(680621, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(681386, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(681432, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(681433, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(704911, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(704926, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(704931, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(707522, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(707774, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(800038, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(800231, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(800357, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(800654, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(801215, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(802598, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(803490, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(803493, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(803816, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(804252, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(804752, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(805489, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(805583, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(805639, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(806060, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(806477, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(806479, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(806980, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(807058, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(807064, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(807078, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(807175, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(807215, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(807367, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(807368, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(807369, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(807370, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(807371, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(807372, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(807853, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(807867, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(807868, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(807869, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(807870, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount'),
(807994, 0, 0, 0, 0, 'Sun Cleric: explicit coefficient or resolved amount');
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `unit_flags`, `type`, `ScriptName`) SELECT 50331, 'Sun Gate', 1, 80, 35, 1, 0, 10, 'npc_ascension_sun_gate' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50331);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 50331, 0, 23719, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50331);
