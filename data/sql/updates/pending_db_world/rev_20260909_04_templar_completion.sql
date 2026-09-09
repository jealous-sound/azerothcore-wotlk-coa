-- Templar completion: apply only with the matching source package.
-- SQL 00-03 and earlier applied migrations remain unchanged.
-- Review missing creature definitions as absent-or-exact before installation.
-- Complete the earlier external SQL41 proposal here; all main Librams need positive magic events.
DELETE FROM `spell_proc` WHERE `SpellId` IN (705274, 705290);
INSERT INTO `spell_proc` (`SpellId`, `SpellFamilyName`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `DisableEffectsMask`, `Chance`) VALUES
(705274, 25, 0, 2048, 16400, 7, 4, 6, 100),
(705290, 25, 536870912, 2048, 16400, 7, 4, 6, 100);
DELETE FROM `spell_proc` WHERE `SpellId` IN (300502, 300504, 301065, 301254, 301339, 500011, 504107, 504561, 504807, 504808, 520017, 520534, 520812, 520883, 523712, 524617, 524765, 572548, 572554, 680399, 680891, 681483, 705255, 705256, 705259, 705286, 705295, 705300, 706468, 707364, 707606, 712437, 712677, 803159, 803844, 804571, 804897, 804927, 805420, 806273, 806354, 806522, 807648);
DELETE FROM `spell_proc` WHERE `SpellId` IN (92108, 92111, 300513, 520007, 524620, 560648, 680397, 704116, 705298, 705306, 706325, 706385, 707391, 712346, 801441, 801457, 801463, 801466, 801482, 801483, 803158, 803890, 803891, 803892, 803893, 804912, 804932, 805415, 806353, 806516);
INSERT INTO `spell_proc` (`SpellId`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `Chance`) VALUES
(92108, 1048575, 7, 2, 35, 2, 100),
(92111, 1048575, 7, 2, 35, 2, 100),
(300513, 1048575, 7, 2, 35, 2, 100),
(520007, 1048575, 7, 2, 35, 2, 100),
(524620, 1048575, 7, 2, 35, 2, 100),
(560648, 1048575, 7, 2, 35, 2, 100),
(680397, 1048575, 7, 2, 35, 2, 100),
(704116, 1048575, 7, 2, 35, 2, 100),
(705298, 1048575, 7, 2, 35, 2, 100),
(705306, 1048575, 7, 2, 35, 2, 100),
(706325, 1048575, 7, 2, 35, 2, 100),
(706385, 1048575, 7, 2, 35, 2, 100),
(707391, 1048575, 7, 2, 35, 2, 100),
(712346, 1048575, 7, 2, 35, 2, 100),
(801441, 1048575, 7, 2, 35, 2, 100),
(801457, 1048575, 7, 2, 35, 2, 100),
(801463, 1048575, 7, 2, 35, 2, 100),
(801466, 1048575, 7, 2, 35, 2, 100),
(801482, 1048575, 7, 2, 35, 2, 100),
(801483, 1048575, 7, 2, 35, 2, 100),
(803158, 1048575, 7, 2, 35, 2, 100),
(803890, 1048575, 7, 2, 35, 2, 100),
(803891, 1048575, 7, 2, 35, 2, 100),
(803892, 1048575, 7, 2, 35, 2, 100),
(803893, 1048575, 7, 2, 35, 2, 100),
(804912, 1048575, 7, 2, 35, 2, 100),
(804932, 1048575, 7, 2, 35, 2, 100),
(805415, 1048575, 7, 2, 35, 2, 100),
(806353, 1048575, 7, 2, 35, 2, 100),
(806516, 1048575, 7, 2, 35, 2, 100);
DELETE FROM `spell_script_names` WHERE `spell_id` = 92108 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (92108, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 92109 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (92109, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 92109 AND `ScriptName` = 'aura_ascension_templar_stagger';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (92109, 'aura_ascension_templar_stagger');
DELETE FROM `spell_script_names` WHERE `spell_id` = 92111 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (92111, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300513 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300513, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300513 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300513, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 301172 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (301172, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 301283 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (301283, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520007 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520007, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 524617 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (524617, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 524620 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (524620, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 524766 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (524766, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 527023 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (527023, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 527270 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (527270, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 527272 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (527272, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560648 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560648, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561156 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561156, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 563269 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (563269, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 563270 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (563270, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 578276 AND `ScriptName` = 'spell_ascension_templar_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (578276, 'spell_ascension_templar_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680397 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680397, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681136 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681136, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704116 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704116, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704576 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704576, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705274 AND `ScriptName` = 'spell_ascension_templar_temporary_libram';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705274, 'spell_ascension_templar_temporary_libram');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705290 AND `ScriptName` = 'spell_ascension_templar_temporary_libram';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705290, 'spell_ascension_templar_temporary_libram');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705298 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705298, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705299 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705299, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705306 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705306, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706325 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706325, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706385 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706385, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706426 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706426, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706583 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706583, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707391 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707391, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 712346 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (712346, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 712378 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (712378, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801202 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801202, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801205 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801205, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801409 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801409, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801441 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801441, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801448 AND `ScriptName` = 'spell_ascension_templar_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801448, 'spell_ascension_templar_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801457 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801457, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801463 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801463, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801466 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801466, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801482 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801482, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801482 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801482, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801483 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801483, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801483 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801483, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803149 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803149, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803149 AND `ScriptName` = 'aura_ascension_templar_stagger';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803149, 'aura_ascension_templar_stagger');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803158 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803158, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803237 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803237, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803373 AND `ScriptName` = 'spell_ascension_templar_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803373, 'spell_ascension_templar_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803374 AND `ScriptName` = 'spell_ascension_templar_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803374, 'spell_ascension_templar_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803375 AND `ScriptName` = 'spell_ascension_templar_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803375, 'spell_ascension_templar_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803376 AND `ScriptName` = 'spell_ascension_templar_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803376, 'spell_ascension_templar_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803377 AND `ScriptName` = 'spell_ascension_templar_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803377, 'spell_ascension_templar_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803378 AND `ScriptName` = 'spell_ascension_templar_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803378, 'spell_ascension_templar_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803890 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803890, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803891 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803891, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803892 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803892, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803893 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803893, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804903 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804903, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804904 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804904, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804912 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804912, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804922 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804922, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804924 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804924, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804932 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804932, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805332 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805332, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805390 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805390, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805409 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805409, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805415 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805415, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805422 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805422, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806353 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806353, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806353 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806353, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806354 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806354, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806516 AND `ScriptName` = 'aura_ascension_templar_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806516, 'aura_ascension_templar_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806523 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806523, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807004 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807004, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807269 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807269, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807270 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807270, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807271 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807271, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807272 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807272, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807273 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807273, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807274 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807274, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807648 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807648, 'aura_ascension_templar_lifecycle');
DELETE FROM `spell_bonus_data` WHERE `entry` IN (500689, 501562, 501563, 501564, 501565, 501566, 501589, 501590, 501591, 501592, 501593, 501601, 501602, 501603, 501604, 501605, 501606, 503135, 503136, 503137, 503138, 503139, 503140, 503141, 504809, 520544, 524619, 525028, 527269, 567538, 567539, 567540, 567541, 570200, 570201, 570202, 570203, 570204, 570205, 572021, 572027, 572739, 572740, 572741, 573020, 573324, 578276, 680398, 706466, 706583, 707111, 707302, 707720, 748505, 748506, 748507, 801165, 801409, 801446, 801448, 801456, 801478, 801832, 803153, 803157, 803160, 803331, 803373, 803374, 803375, 803376, 803377, 803378, 803792, 803793, 803794, 803795, 803796, 803872, 803873, 803874, 803875, 803876, 803877, 803878, 803879, 803880, 804148, 804150, 804906, 804929, 805410, 805421, 806153, 806352, 806521, 806834, 806835, 806864, 806865, 806866, 806867, 806868, 807263, 807264, 807265, 807266, 807267, 807268, 807414, 807800, 807801, 807802, 807803, 807804, 807805);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(500689, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(501562, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(501563, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(501564, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(501565, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(501566, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(501589, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(501590, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(501591, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(501592, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(501593, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(501601, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(501602, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(501603, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(501604, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(501605, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(501606, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(503135, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(503136, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(503137, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(503138, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(503139, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(503140, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(503141, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(504809, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(520544, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(524619, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(525028, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(527269, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(567538, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(567539, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(567540, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(567541, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(570200, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(570201, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(570202, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(570203, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(570204, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(570205, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(572021, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(572027, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(572739, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(572740, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(572741, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(573020, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(573324, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(578276, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(680398, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(706466, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(706583, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(707111, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(707302, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(707720, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(748505, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(748506, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(748507, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(801165, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(801409, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(801446, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(801448, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(801456, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(801478, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(801832, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803153, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803157, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803160, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803331, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803373, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803374, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803375, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803376, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803377, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803378, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803792, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803793, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803794, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803795, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803796, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803872, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803873, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803874, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803875, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803876, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803877, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803878, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803879, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(803880, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(804148, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(804150, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(804906, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(804929, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(805410, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(805421, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(806153, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(806352, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(806521, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(806834, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(806835, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(806864, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(806865, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(806866, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(806867, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(806868, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(807263, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(807264, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(807265, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(807266, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(807267, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(807268, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(807414, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(807800, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(807801, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(807802, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(807803, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(807804, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result'),
(807805, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result');
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50250, 'Testament of Hope', 1, 1, 35, 1, 7, 2000, 2000, 64, 'npc_ascension_templar_hope'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50250);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50250, 0, 775, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50250);
