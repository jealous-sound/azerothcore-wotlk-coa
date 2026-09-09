-- Knight of Xoroth: new unapplied migration, requires matching source.
-- Creature and model inserts require absent-or-exact checks before installation.
DELETE FROM `spell_proc` WHERE `SpellId` IN (92104, 300375, 300376, 300388, 300391, 300395, 300398, 301302, 301358, 302548, 500551, 500578, 520008, 520296, 520372, 520662, 524920, 560546, 560630, 560633, 562029, 573034, 680199, 681449, 704452, 704952, 704957, 704958, 704961, 704971, 704972, 704975, 704979, 704987, 704991, 704997, 705005, 705008, 705015, 705018, 706295, 706331, 706502, 706563, 706565, 706590, 706755, 707631, 707840, 800443, 800702, 800997, 801053, 801054, 801065, 802344, 802345, 802602, 802603, 802604, 802605, 802610, 802615, 804345, 804786, 805693, 805703, 807144, 807336, 807587);
INSERT INTO `spell_proc` (`SpellId`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `Chance`) VALUES
(92104, 1048575, 7, 2, 63, 2, 100),
(300376, 1048575, 7, 2, 63, 2, 100),
(302548, 1048575, 7, 2, 63, 2, 100),
(520372, 1048575, 7, 2, 63, 2, 100),
(524920, 1048575, 7, 2, 63, 2, 100),
(560546, 1048575, 7, 2, 63, 2, 100),
(573034, 1048575, 7, 2, 63, 2, 100),
(681449, 1048575, 7, 2, 63, 2, 100),
(704971, 1048575, 7, 2, 63, 2, 100),
(704972, 1048575, 7, 2, 63, 2, 100),
(704979, 1048575, 7, 2, 63, 2, 100),
(704991, 1048575, 7, 2, 63, 2, 100),
(706502, 1048575, 7, 2, 63, 2, 100),
(706590, 1048575, 7, 2, 63, 2, 100),
(800997, 1048575, 7, 2, 63, 2, 100),
(801065, 1048575, 7, 2, 63, 2, 100),
(804345, 1048575, 7, 2, 63, 2, 100),
(800702, 1048575, 7, 2, 63, 2, 100),
(560630, 1048575, 7, 2, 63, 2, 100),
(802602, 1048575, 7, 2, 63, 2, 100),
(802603, 1048575, 7, 2, 63, 2, 100),
(802604, 1048575, 7, 2, 63, 2, 100),
(802605, 1048575, 7, 2, 63, 2, 100),
(800443, 1048575, 7, 2, 63, 2, 100),
(562029, 1048575, 7, 2, 63, 2, 100),
(520662, 1048575, 7, 2, 63, 2, 100);
DELETE FROM `spell_script_names` WHERE `spell_id` = 92101 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (92101, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 92104 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (92104, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 92104 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (92104, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300376 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300376, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 301302 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (301302, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 302548 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (302548, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 302555 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (302555, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500904 AND `ScriptName` = 'spell_ascension_xoroth_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500904, 'spell_ascension_xoroth_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500906 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500906, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501499 AND `ScriptName` = 'spell_ascension_xoroth_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501499, 'spell_ascension_xoroth_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501500 AND `ScriptName` = 'spell_ascension_xoroth_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501500, 'spell_ascension_xoroth_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501501 AND `ScriptName` = 'spell_ascension_xoroth_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501501, 'spell_ascension_xoroth_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501502 AND `ScriptName` = 'spell_ascension_xoroth_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501502, 'spell_ascension_xoroth_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501503 AND `ScriptName` = 'spell_ascension_xoroth_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501503, 'spell_ascension_xoroth_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501504 AND `ScriptName` = 'spell_ascension_xoroth_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501504, 'spell_ascension_xoroth_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501505 AND `ScriptName` = 'spell_ascension_xoroth_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501505, 'spell_ascension_xoroth_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501506 AND `ScriptName` = 'spell_ascension_xoroth_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501506, 'spell_ascension_xoroth_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501507 AND `ScriptName` = 'spell_ascension_xoroth_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501507, 'spell_ascension_xoroth_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520021 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520021, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520294 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520294, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520295 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520295, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520372 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520372, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520662 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520662, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520752 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520752, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 524897 AND `ScriptName` = 'spell_ascension_xoroth_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (524897, 'spell_ascension_xoroth_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 524913 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (524913, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 524920 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (524920, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 524920 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (524920, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560546 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560546, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560630 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560630, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560630 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560630, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 562029 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (562029, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570727 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570727, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 573034 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (573034, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 573035 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (573035, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 573075 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (573075, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 573336 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (573336, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680197 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680197, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680203 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680203, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680300 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680300, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680729 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680729, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681184 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681184, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681449 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681449, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704247 AND `ScriptName` = 'spell_ascension_xoroth_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704247, 'spell_ascension_xoroth_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704971 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704971, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704972 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704972, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704979 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704979, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704991 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704991, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704993 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704993, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706502 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706502, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706564 AND `ScriptName` = 'aura_ascension_xoroth_block';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706564, 'aura_ascension_xoroth_block');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706589 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706589, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706590 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706590, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706755 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706755, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706756 AND `ScriptName` = 'spell_ascension_xoroth_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706756, 'spell_ascension_xoroth_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707131 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707131, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707341 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707341, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707342 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707342, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707343 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707343, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707344 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707344, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707345 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707345, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707693 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707693, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707694 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707694, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707695 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707695, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707696 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707696, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 712294 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (712294, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 712460 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (712460, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800443 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800443, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800702 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800702, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800710 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800710, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800997 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800997, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800999 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800999, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801017 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801017, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801042 AND `ScriptName` = 'spell_ascension_xoroth_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801042, 'spell_ascension_xoroth_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801053 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801053, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801054 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801054, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801064 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801064, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801065 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801065, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802344 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802344, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802345 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802345, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802602 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802602, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802602 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802602, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802603 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802603, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802604 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802604, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802605 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802605, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802617 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802617, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802618 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802618, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803667 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803667, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803668 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803668, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803669 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803669, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803670 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803670, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803671 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803671, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803730 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803730, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803731 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803731, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803889 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803889, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804345 AND `ScriptName` = 'aura_ascension_xoroth_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804345, 'aura_ascension_xoroth_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804703 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804703, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804774 AND `ScriptName` = 'spell_ascension_xoroth_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804774, 'spell_ascension_xoroth_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804785 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804785, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804786 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804786, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805679 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805679, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805680 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805680, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805746 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805746, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805966 AND `ScriptName` = 'spell_ascension_xoroth_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805966, 'spell_ascension_xoroth_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806965 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806965, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806966 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806966, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806967 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806967, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806968 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806968, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806969 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806969, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806970 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806970, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807248 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807248, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807587 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807587, 'aura_ascension_xoroth_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807699 AND `ScriptName` = 'spell_ascension_xoroth_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807699, 'spell_ascension_xoroth_ability');
DELETE FROM `spell_bonus_data` WHERE `entry` IN (500020, 501488, 501489, 501490, 503361, 503362, 503363, 503364, 503365, 503366, 503367, 520292, 520294, 520857, 524897, 524919, 560664, 560665, 560666, 560667, 560668, 560817, 573336, 573448, 630931, 680204, 681206, 704974, 705002, 800081, 800341, 800444, 801004, 801018, 801037, 801055, 801059, 801064, 802411, 802412, 802413, 802414, 802415, 802608, 802620, 802855, 803254, 804169, 804353, 804702, 804704, 804886, 805074, 805671, 805680, 806219, 806965, 806966, 806967, 806968, 806969, 806970);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(500020, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(501488, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(501489, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(501490, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(503361, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(503362, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(503363, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(503364, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(503365, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(503366, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(503367, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(520292, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(520294, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(520857, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(524897, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(524919, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(560664, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(560665, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(560666, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(560667, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(560668, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(560817, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(573336, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(573448, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(630931, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(680204, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(681206, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(704974, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(705002, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(800081, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(800341, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(800444, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(801004, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(801018, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(801037, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(801055, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(801059, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(801064, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(802411, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(802412, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(802413, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(802414, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(802415, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(802608, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(802620, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(802855, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(803254, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(804169, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(804353, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(804702, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(804704, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(804886, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(805074, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(805671, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(805680, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(806219, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(806965, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(806966, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(806967, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(806968, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(806969, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result'),
(806970, 0, 0, 0, 0, 'Xoroth: explicit coefficient or copied result');
DELETE FROM `spell_group` WHERE `id` = 1029 AND `spell_id` = 704994;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES (1029, 704994);
DELETE FROM `spell_group` WHERE `id` = 2000181 AND `spell_id` = 520372;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES (2000181, 520372);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 510100, 'Greater Imp', 1, 1, 35, 8, 3, 2000, 2000, 64, '' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 510100);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 510100, 0, 4449, 0.85, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 510100);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50301, 'Hellfire Imp', 1, 1, 35, 8, 3, 2000, 2000, 64, 'npc_ascension_xoroth_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50301);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50301, 0, 4449, 0.55, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50301);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50375, 'Hellfire Abyssal', 1, 1, 35, 1, 3, 2000, 2000, 64, 'npc_ascension_xoroth_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50375);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50375, 0, 169, 0.7, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50375);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 51323, 'Chains of Xoroth', 1, 1, 35, 1, 3, 2000, 2000, 64, 'npc_ascension_xoroth_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 51323);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 51323, 0, 11686, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 51323);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50268, 'Dreadsteed Portal', 1, 1, 35, 1, 3, 2000, 2000, 64, 'npc_ascension_xoroth_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50268);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50268, 0, 9510, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50268);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 914505, 'Xorothian Warsteed', 1, 1, 35, 1, 3, 2000, 2000, 64, '' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 914505);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 914505, 0, 14554, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 914505);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 177480, 'Doomguard', 1, 1, 35, 1, 3, 2000, 2000, 64, '' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 177480);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 177480, 0, 68, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 177480);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 1, 34, 10, 22, 20, 20, 20, 25, 23, 0, 1 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 1);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 2, 43, 48, 21, 21, 20, 20, 26, 24, 1, 2 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 2);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 3, 56, 57, 32, 22, 20, 21, 27, 25, 2, 4 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 3);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 4, 68, 66, 57, 23, 21, 21, 29, 26, 3, 6 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 4);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 5, 80, 76, 87, 23, 21, 22, 30, 27, 4, 8 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 5);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 6, 92, 85, 122, 24, 21, 22, 31, 28, 6, 10 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 6);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 7, 105, 95, 166, 25, 21, 23, 32, 29, 7, 12 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 7);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 8, 119, 105, 216, 26, 21, 23, 33, 30, 8, 13 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 8);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 9, 134, 115, 275, 27, 21, 23, 35, 32, 9, 14 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 9);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 10, 149, 126, 342, 27, 22, 24, 36, 33, 9, 15 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 10);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 11, 167, 151, 362, 28, 22, 24, 39, 35, 10, 16 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 11);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 12, 184, 177, 384, 29, 22, 26, 44, 38, 11, 17 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 12);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 13, 202, 198, 405, 30, 22, 26, 47, 40, 11, 18 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 13);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 14, 220, 234, 429, 31, 22, 28, 56, 45, 12, 19 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 14);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 15, 239, 260, 451, 32, 23, 29, 61, 48, 12, 20 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 15);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 16, 257, 282, 475, 34, 23, 30, 64, 50, 13, 21 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 16);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 17, 277, 309, 498, 36, 23, 31, 68, 53, 14, 23 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 17);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 18, 297, 336, 519, 37, 23, 32, 72, 55, 15, 24 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 18);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 19, 318, 368, 543, 38, 23, 33, 76, 58, 17, 26 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 19);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 20, 340, 396, 565, 40, 24, 35, 81, 61, 17, 27 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 20);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 21, 362, 419, 588, 42, 24, 35, 84, 64, 18, 28 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 21);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 22, 389, 447, 612, 44, 24, 37, 89, 67, 19, 30 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 22);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 23, 415, 476, 633, 45, 24, 37, 92, 69, 20, 31 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 23);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 24, 444, 509, 657, 46, 25, 39, 97, 73, 20, 31 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 24);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 25, 471, 538, 679, 47, 25, 40, 101, 75, 21, 33 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 25);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 26, 502, 563, 701, 49, 25, 41, 105, 78, 21, 34 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 26);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 27, 532, 597, 725, 50, 25, 43, 110, 81, 23, 35 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 27);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 28, 562, 652, 747, 52, 25, 43, 121, 86, 23, 36 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 28);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 29, 591, 682, 770, 54, 26, 45, 126, 89, 23, 37 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 29);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 30, 618, 717, 792, 54, 26, 46, 131, 92, 25, 39 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 30);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 31, 646, 743, 814, 56, 26, 47, 134, 95, 26, 40 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 31);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 32, 674, 779, 838, 57, 26, 49, 139, 98, 27, 41 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 32);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 33, 701, 810, 860, 59, 27, 49, 143, 101, 27, 42 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 33);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 34, 728, 842, 883, 60, 27, 51, 147, 104, 27, 43 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 34);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 35, 757, 884, 904, 62, 27, 52, 152, 107, 28, 44 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 35);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 36, 785, 911, 972, 63, 28, 53, 156, 110, 29, 45 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 36);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 37, 814, 943, 1045, 64, 28, 55, 161, 114, 30, 47 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 37);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 38, 843, 982, 1118, 66, 28, 55, 165, 116, 31, 48 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 38);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 39, 873, 1015, 1196, 68, 28, 57, 170, 120, 32, 49 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 39);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 40, 904, 1053, 1277, 70, 29, 59, 174, 123, 33, 51 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 40);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 41, 932, 1087, 1363, 72, 29, 59, 178, 126, 35, 54 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 41);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 42, 970, 1145, 1454, 77, 29, 61, 190, 131, 37, 57 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 42);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 43, 1008, 1180, 1546, 81, 29, 62, 194, 134, 39, 60 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 43);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 44, 1047, 1214, 1646, 88, 30, 63, 199, 138, 42, 65 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 44);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 45, 1087, 1254, 1747, 91, 30, 65, 204, 141, 45, 75 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 45);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 46, 1127, 1294, 1782, 93, 30, 66, 208, 144, 46, 70 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 46);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 47, 1170, 1330, 1818, 95, 31, 68, 213, 148, 47, 72 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 47);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 48, 1212, 1366, 1851, 97, 31, 68, 218, 151, 48, 73 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 48);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 49, 1257, 1412, 1887, 99, 31, 70, 223, 155, 49, 75 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 49);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 50, 1302, 1449, 1920, 101, 32, 72, 228, 158, 51, 77 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 50);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 51, 1347, 1487, 1956, 103, 32, 73, 232, 161, 51, 78 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 51);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 52, 1394, 1534, 1991, 105, 32, 74, 237, 165, 52, 80 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 52);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 53, 1441, 1572, 2025, 107, 33, 75, 241, 168, 54, 82 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 53);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 54, 1489, 1615, 2060, 109, 33, 77, 247, 172, 54, 83 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 54);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 55, 1537, 1654, 2093, 111, 33, 79, 252, 176, 56, 85 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 55);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 56, 1587, 1717, 2131, 114, 34, 80, 265, 181, 57, 87 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 56);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 57, 1637, 1762, 2165, 116, 34, 81, 271, 185, 58, 88 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 57);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 58, 1688, 1802, 2198, 117, 34, 82, 275, 188, 69, 105 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 58);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 59, 1741, 1847, 2234, 120, 35, 84, 281, 193, 75, 114 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 59);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 60, 1793, 1897, 2427, 122, 35, 86, 286, 196, 109, 165 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 60);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 61, 1848, 1938, 2617, 125, 35, 87, 286, 206, 116, 175 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 61);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 62, 1903, 1984, 2807, 126, 35, 89, 286, 212, 124, 187 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 62);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 63, 1958, 2031, 2998, 129, 36, 90, 286, 220, 132, 199 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 63);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 64, 2014, 2078, 3188, 131, 36, 92, 287, 226, 139, 211 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 64);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 65, 2071, 2125, 3378, 134, 36, 94, 288, 233, 148, 223 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 65);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 66, 2131, 2173, 3569, 135, 37, 94, 297, 239, 156, 236 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 66);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 67, 2192, 2226, 3759, 138, 37, 96, 306, 246, 164, 248 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 67);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 68, 2255, 2270, 3949, 140, 37, 97, 312, 251, 192, 290 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 68);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 69, 2317, 2319, 4141, 143, 38, 99, 319, 257, 211, 317 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 69);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 70, 2381, 2374, 4330, 145, 38, 101, 327, 263, 228, 343 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 70);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 71, 2526, 2419, 4524, 147, 41, 102, 331, 269, 235, 354 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 71);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 72, 2666, 2475, 4717, 150, 44, 104, 335, 275, 242, 364 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 72);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 73, 2809, 2526, 4910, 153, 47, 105, 339, 281, 249, 374 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 73);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 74, 2955, 2577, 5104, 168, 51, 107, 343, 333, 256, 385 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 74);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 75, 3102, 2634, 5299, 185, 55, 109, 347, 339, 264, 397 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 75);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 76, 3250, 2686, 5492, 203, 59, 110, 351, 344, 271, 408 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 76);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 77, 3401, 2743, 5688, 224, 64, 113, 355, 349, 279, 420 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 77);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 78, 3553, 2791, 5881, 246, 68, 114, 360, 355, 287, 432 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 78);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 79, 3709, 2850, 6078, 270, 74, 116, 364, 360, 295, 444 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 79);
INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT 510100, 80, 3867, 2908, 6273, 297, 79, 118, 369, 367, 305, 458 WHERE NOT EXISTS (SELECT 1 FROM `pet_levelstats` WHERE `creature_entry` = 510100 AND `level` = 80);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `Data0`, `Data1`)
SELECT 132847, 8, 209, 'Hell''s Forge', 1, 3, 10 WHERE NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry` = 132847);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `Data0`, `Data1`)
SELECT 391505, 8, 273, 'Hell''s Anvil', 1, 1, 10 WHERE NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry` = 391505);
