-- Cultist completion: new unapplied SQL12, requires matching source.
DELETE FROM `spell_proc` WHERE `SpellId` IN (255283, 300275, 300278, 300280, 300286, 300291, 300295, 300296, 300298, 300300, 300308, 300313, 301067, 301068, 301183, 301200, 301209, 301262, 302015, 500719, 500721, 500767, 504642, 520388, 520405, 524887, 525049, 525060, 525065, 525078, 525307, 560091, 560320, 560715, 560977, 561336, 570246, 570247, 570248, 570249, 570250, 570251, 570252, 570253, 570254, 570255, 570256, 570257, 570258, 570259, 570260, 570261, 570262, 570264, 570265, 570266, 570267, 570268, 570269, 572064, 574318, 600327, 680508, 680557, 680570, 680579, 680581, 681088, 681106, 681425, 681474, 704527, 704869, 704871, 704880, 704884, 704889, 704892, 706182, 706187, 706245, 706909, 706911, 706926, 707640, 707749, 707753, 707785, 800463, 802043, 803035, 803037, 803082, 803339, 805110, 805126, 805606, 806250, 806382, 807128, 807307, 807877);
INSERT INTO `spell_proc` (`SpellId`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `Chance`, `Charges`) VALUES
(255283, 1048575, 7, 2, 32767, 2, 100, 0),
(300278, 1048575, 7, 2, 32767, 2, 100, 0),
(300280, 1048575, 7, 2, 32767, 2, 100, 0),
(300286, 1048575, 7, 2, 32767, 2, 100, 0),
(300295, 1048575, 7, 2, 32767, 2, 100, 0),
(300300, 1048575, 7, 2, 32767, 2, 100, 0),
(300308, 1048575, 7, 2, 32767, 2, 100, 0),
(300313, 1048575, 7, 2, 32767, 2, 100, 0),
(301262, 1048575, 7, 2, 32767, 2, 100, 0),
(500719, 1048575, 7, 2, 32767, 2, 100, 0),
(500721, 1048575, 7, 2, 32767, 2, 100, 0),
(500767, 1048575, 7, 2, 32767, 2, 100, 0),
(504642, 1048575, 7, 2, 32767, 2, 100, 0),
(520388, 1048575, 7, 2, 32767, 2, 100, 5),
(520405, 1048575, 7, 2, 32767, 2, 100, 0),
(524887, 1048575, 7, 2, 32767, 2, 100, 0),
(525065, 1048575, 7, 2, 32767, 2, 100, 0),
(525078, 1048575, 7, 2, 32767, 2, 100, 0),
(525307, 1048575, 7, 2, 32767, 2, 100, 0),
(560091, 1048575, 7, 2, 32767, 2, 100, 0),
(560320, 1048575, 7, 2, 32767, 2, 100, 0),
(560715, 1048575, 7, 2, 32767, 2, 100, 0),
(561336, 1048575, 7, 2, 32767, 2, 100, 0),
(570246, 1048575, 7, 2, 32767, 2, 100, 0),
(570247, 1048575, 7, 2, 32767, 2, 100, 0),
(570248, 1048575, 7, 2, 32767, 2, 100, 0),
(570249, 1048575, 7, 2, 32767, 2, 100, 0),
(570250, 1048575, 7, 2, 32767, 2, 100, 0),
(570251, 1048575, 7, 2, 32767, 2, 100, 0),
(570252, 1048575, 7, 2, 32767, 2, 100, 0),
(570253, 1048575, 7, 2, 32767, 2, 100, 0),
(570254, 1048575, 7, 2, 32767, 2, 100, 0),
(570255, 1048575, 7, 2, 32767, 2, 100, 0),
(570256, 1048575, 7, 2, 32767, 2, 100, 0),
(570257, 1048575, 7, 2, 32767, 2, 100, 0),
(570258, 1048575, 7, 2, 32767, 2, 100, 0),
(570259, 1048575, 7, 2, 32767, 2, 100, 0),
(570260, 1048575, 7, 2, 32767, 2, 100, 0),
(570261, 1048575, 7, 2, 32767, 2, 100, 0),
(570262, 1048575, 7, 2, 32767, 2, 100, 0),
(570264, 1048575, 7, 2, 32767, 2, 100, 0),
(570265, 1048575, 7, 2, 32767, 2, 100, 0),
(570266, 1048575, 7, 2, 32767, 2, 100, 0),
(570267, 1048575, 7, 2, 32767, 2, 100, 0),
(570268, 1048575, 7, 2, 32767, 2, 100, 0),
(570269, 1048575, 7, 2, 32767, 2, 100, 0),
(572064, 1048575, 7, 2, 32767, 2, 100, 0),
(574318, 1048575, 7, 2, 32767, 2, 100, 0),
(600327, 1048575, 7, 2, 32767, 2, 100, 10),
(680508, 1048575, 7, 2, 32767, 2, 100, 0),
(680579, 1048575, 7, 2, 32767, 2, 100, 0),
(680581, 1048575, 7, 2, 32767, 2, 100, 0),
(681425, 1048575, 7, 2, 32767, 2, 100, 0),
(681474, 1048575, 7, 2, 32767, 2, 100, 0),
(704527, 1048575, 7, 2, 32767, 2, 100, 0),
(704871, 1048575, 7, 2, 32767, 2, 100, 0),
(706182, 1048575, 7, 2, 32767, 2, 100, 0),
(706245, 1048575, 7, 2, 32767, 2, 100, 0),
(706911, 1048575, 7, 2, 32767, 2, 100, 0),
(706926, 1048575, 7, 2, 32767, 2, 100, 0),
(707640, 1048575, 7, 2, 32767, 2, 100, 0),
(707749, 1048575, 7, 2, 32767, 2, 100, 0),
(707785, 1048575, 7, 2, 32767, 2, 100, 0),
(800463, 1048575, 7, 2, 32767, 2, 100, 0),
(802043, 1048575, 7, 2, 32767, 2, 100, 0),
(803035, 1048575, 7, 2, 32767, 2, 100, 0),
(803037, 1048575, 7, 2, 32767, 2, 100, 0),
(803082, 1048575, 7, 2, 32767, 2, 100, 0),
(803339, 1048575, 7, 2, 32767, 2, 100, 0),
(805110, 1048575, 7, 2, 32767, 2, 100, 0),
(805606, 1048575, 7, 2, 32767, 2, 100, 0),
(807307, 1048575, 7, 2, 32767, 2, 100, 0);
DELETE FROM `spell_script_names` WHERE `spell_id` = 92130 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (92130, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 255070 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (255070, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 255281 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (255281, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 255282 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (255282, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 255283 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (255283, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 255283 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (255283, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300177 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300177, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300277 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300277, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300278 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300278, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300280 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300280, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300286 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300286, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300287 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300287, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300290 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300290, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300295 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300295, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300300 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300300, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300308 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300308, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300313 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300313, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 301184 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (301184, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 301186 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (301186, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 301259 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (301259, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 301262 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (301262, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 301988 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (301988, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500704 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500704, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500706 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500706, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500707 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500707, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500715 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500715, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500715 AND `ScriptName` = 'spell_ascension_cultist_shield';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500715, 'spell_ascension_cultist_shield');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500719 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500719, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500721 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500721, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500727 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500727, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500728 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500728, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500751 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500751, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500767 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500767, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502133 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502133, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502244 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502244, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502244 AND `ScriptName` = 'spell_ascension_cultist_shield';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502244, 'spell_ascension_cultist_shield');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502245 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502245, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502245 AND `ScriptName` = 'spell_ascension_cultist_shield';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502245, 'spell_ascension_cultist_shield');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502246 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502246, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502246 AND `ScriptName` = 'spell_ascension_cultist_shield';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502246, 'spell_ascension_cultist_shield');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502247 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502247, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502247 AND `ScriptName` = 'spell_ascension_cultist_shield';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502247, 'spell_ascension_cultist_shield');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502267 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502267, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502268 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502268, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502269 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502269, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502270 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502270, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502271 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502271, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502272 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502272, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502273 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502273, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502274 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502274, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502275 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502275, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503652 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503652, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503695 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503695, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504642 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504642, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504852 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504852, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520326 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520326, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520345 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520345, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520346 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520346, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520348 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520348, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520388 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520388, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520388 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520388, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520405 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520405, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520439 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520439, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520450 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520450, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520497 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520497, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520775 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520775, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520790 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520790, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 524887 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (524887, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 525065 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (525065, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 525078 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (525078, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 525307 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (525307, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560015 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560015, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560091 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560091, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560301 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560301, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560320 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560320, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560715 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560715, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560716 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560716, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561288 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561288, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561336 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561336, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561386 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561386, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561387 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561387, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561389 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561389, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561390 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561390, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561391 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561391, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561392 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561392, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 567524 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (567524, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 567529 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (567529, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 567548 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (567548, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 567549 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (567549, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 567550 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (567550, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 567551 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (567551, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 567552 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (567552, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570188 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570188, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570246 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570246, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570247 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570247, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570248 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570248, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570249 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570249, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570250 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570250, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570251 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570251, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570252 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570252, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570253 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570253, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570254 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570254, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570255 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570255, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570256 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570256, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570257 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570257, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570258 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570258, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570259 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570259, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570260 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570260, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570261 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570261, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570262 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570262, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570264 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570264, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570265 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570265, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570266 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570266, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570267 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570267, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570268 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570268, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570269 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570269, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572064 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572064, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572103 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572103, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572637 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572637, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572791 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572791, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572819 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572819, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572857 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572857, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572905 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572905, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 573067 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (573067, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 574147 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (574147, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 574318 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (574318, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 578114 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (578114, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 578114 AND `ScriptName` = 'spell_ascension_cultist_shield';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (578114, 'spell_ascension_cultist_shield');
DELETE FROM `spell_script_names` WHERE `spell_id` = 578115 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (578115, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 578115 AND `ScriptName` = 'spell_ascension_cultist_shield';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (578115, 'spell_ascension_cultist_shield');
DELETE FROM `spell_script_names` WHERE `spell_id` = 582591 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (582591, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 600327 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (600327, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 600327 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (600327, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680508 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680508, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680573 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680573, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680574 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680574, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680576 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680576, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680579 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680579, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680581 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680581, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680607 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680607, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680609 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680609, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681103 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681103, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681104 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681104, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681425 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681425, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681474 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681474, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681476 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681476, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681794 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681794, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704476 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704476, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704527 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704527, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704871 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704871, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704872 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704872, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706182 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706182, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706245 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706245, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706264 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706264, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706910 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706910, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706911 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706911, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706923 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706923, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706925 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706925, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706926 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706926, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706932 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706932, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707098 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707098, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707640 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707640, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707749 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707749, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707785 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707785, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800105 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800105, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800430 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800430, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800432 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800432, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800463 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800463, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800965 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800965, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801153 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801153, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801157 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801157, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802042 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802042, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802043 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802043, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802044 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802044, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803035 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803035, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803035 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803035, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803037 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803037, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803037 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803037, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803061 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803061, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803082 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803082, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803082 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803082, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803339 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803339, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803339 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803339, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804153 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804153, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804209 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804209, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804211 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804211, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804214 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804214, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804215 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804215, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804275 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804275, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804314 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804314, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804670 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804670, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804711 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804711, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804777 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804777, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804779 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804779, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805110 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805110, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805114 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805114, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805586 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805586, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805606 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805606, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806039 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806039, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806250 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806250, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806745 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806745, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806745 AND `ScriptName` = 'spell_ascension_cultist_shield';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806745, 'spell_ascension_cultist_shield');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806769 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806769, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807124 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807124, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807216 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807216, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807307 AND `ScriptName` = 'aura_ascension_cultist_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807307, 'aura_ascension_cultist_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807308 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807308, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807847 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807847, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807878 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807878, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807883 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807883, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 808024 AND `ScriptName` = 'spell_ascension_cultist_resource';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (808024, 'spell_ascension_cultist_resource');
DELETE FROM `spell_script_names` WHERE `spell_id` = 808036 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (808036, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 808037 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (808037, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 808038 AND `ScriptName` = 'spell_ascension_cultist_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (808038, 'spell_ascension_cultist_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 850002 AND `ScriptName` = 'aura_ascension_cultist_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (850002, 'aura_ascension_cultist_lifecycle');
DELETE FROM `spell_bonus_data` WHERE `entry` IN (301211, 301983, 302897, 354191, 500711, 500712, 500714, 500715, 500720, 500748, 500773, 500862, 502114, 502115, 502116, 502117, 502118, 502119, 502120, 502121, 502122, 502123, 502124, 502166, 502167, 502168, 502169, 502170, 502171, 502172, 502173, 502174, 502175, 502176, 502177, 502178, 502179, 502180, 502181, 502182, 502183, 502184, 502226, 502227, 502228, 502229, 502230, 502231, 502232, 502233, 502234, 502235, 502236, 502237, 502238, 502239, 502240, 502241, 502242, 502243, 502244, 502245, 502246, 502247, 502267, 502268, 502269, 502270, 502271, 502272, 502273, 502274, 502275, 503487, 503488, 504719, 504904, 520152, 520332, 520333, 520346, 520450, 520497, 524876, 525062, 567524, 567548, 567549, 567550, 567551, 567552, 570263, 572140, 572141, 572715, 573311, 574325, 578114, 578115, 680576, 680769, 704473, 706910, 707185, 707750, 800105, 800402, 800413, 800416, 800432, 802046, 802727, 803083, 803395, 803396, 803397, 803398, 804153, 804208, 804999, 805573, 806039, 806596, 806745, 807083, 807219, 807632, 807815, 807854, 808043, 808044, 808045, 808050, 808051, 808052, 850002);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(301211, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(301983, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(302897, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(354191, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(500711, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(500712, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(500714, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(500715, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(500720, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(500748, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(500773, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(500862, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502114, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502115, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502116, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502117, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502118, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502119, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502120, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502121, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502122, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502123, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502124, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502166, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502167, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502168, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502169, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502170, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502171, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502172, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502173, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502174, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502175, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502176, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502177, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502178, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502179, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502180, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502181, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502182, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502183, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502184, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502226, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502227, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502228, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502229, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502230, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502231, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502232, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502233, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502234, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502235, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502236, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502237, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502238, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502239, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502240, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502241, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502242, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502243, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502244, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502245, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502246, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502247, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502267, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502268, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502269, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502270, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502271, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502272, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502273, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502274, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(502275, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(503487, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(503488, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(504719, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(504904, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(520152, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(520332, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(520333, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(520346, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(520450, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(520497, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(524876, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(525062, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(567524, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(567548, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(567549, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(567550, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(567551, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(567552, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(570263, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(572140, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(572141, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(572715, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(573311, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(574325, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(578114, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(578115, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(680576, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(680769, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(704473, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(706910, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(707185, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(707750, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(800105, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(800402, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(800413, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(800416, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(800432, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(802046, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(802727, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(803083, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(803395, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(803396, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(803397, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(803398, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(804153, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(804208, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(804999, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(805573, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(806039, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(806596, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(806745, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(807083, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(807219, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(807632, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(807815, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(807854, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(808043, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(808044, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(808045, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(808050, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(808051, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(808052, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result'),
(850002, 0, 0, 0, 0, 'Cultist: explicit coefficient or resolved result');
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `BaseAttackTime`, `RangeAttackTime`, `unit_class`, `unit_flags`, `type`, `ScriptName`) SELECT 533030, 'Apocalyptic Skies', 1, 80, 35, 0, 1, 1.14286, 2000, 2000, 1, 0, 4, 'npc_ascension_cultist_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 533030);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 533030, 0, 27735, 0.6, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 533030);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `BaseAttackTime`, `RangeAttackTime`, `unit_class`, `unit_flags`, `type`, `ScriptName`) SELECT 500464, 'Eldritch Tentacle', 1, 80, 35, 0, 1, 1.14286, 2000, 2000, 1, 0, 4, 'npc_ascension_cultist_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 500464);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 500464, 0, 28813, 0.25, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 500464);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `BaseAttackTime`, `RangeAttackTime`, `unit_class`, `unit_flags`, `type`, `ScriptName`) SELECT 501464, 'Tentacle of N''Zoth', 1, 80, 35, 0, 1, 1.14286, 2000, 2000, 1, 0, 4, 'npc_ascension_cultist_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 501464);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 501464, 0, 28813, 0.5, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 501464);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `BaseAttackTime`, `RangeAttackTime`, `unit_class`, `unit_flags`, `type`, `ScriptName`) SELECT 397771, 'Manifestation of Y''Shaarj', 1, 80, 35, 0, 1, 1.14286, 2000, 2000, 1, 0, 4, 'npc_ascension_cultist_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 397771);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 397771, 0, 28844, 0.5, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 397771);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `BaseAttackTime`, `RangeAttackTime`, `unit_class`, `unit_flags`, `type`, `ScriptName`) SELECT 840025, 'Hallucination', 1, 80, 35, 0, 1, 1.14286, 2000, 2000, 1, 0, 4, 'npc_ascension_cultist_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 840025);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 840025, 0, 49, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 840025);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `BaseAttackTime`, `RangeAttackTime`, `unit_class`, `unit_flags`, `type`, `ScriptName`) SELECT 50272, 'Tentacle of C''Thun', 1, 80, 35, 0, 1, 1.14286, 2000, 2000, 1, 0, 4, 'npc_ascension_cultist_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50272);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 50272, 0, 15788, 0.7, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50272);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `BaseAttackTime`, `RangeAttackTime`, `unit_class`, `unit_flags`, `type`, `ScriptName`) SELECT 500465, 'Tentacle of Yogg-Saron', 1, 80, 35, 0, 1, 1.14286, 2000, 2000, 1, 0, 4, 'npc_ascension_cultist_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 500465);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 500465, 0, 28815, 0.5, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 500465);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `BaseAttackTime`, `RangeAttackTime`, `unit_class`, `unit_flags`, `type`, `ScriptName`) SELECT 50096, 'Tentacle of Y''Shaarj', 1, 80, 35, 0, 1, 1.14286, 2000, 2000, 1, 0, 4, 'npc_ascension_cultist_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50096);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 50096, 0, 28814, 0.5, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50096);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `BaseAttackTime`, `RangeAttackTime`, `unit_class`, `unit_flags`, `type`, `ScriptName`) SELECT 50298, 'Eldritch Ritual', 1, 80, 35, 1, 1, 1.14286, 2000, 2000, 1, 0, 4, 'npc_ascension_cultist_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50298);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 50298, 0, 29074, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50298);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `BaseAttackTime`, `RangeAttackTime`, `unit_class`, `unit_flags`, `type`, `ScriptName`) SELECT 50263, 'Faceless Destroyer', 1, 80, 14, 0, 1, 1.14286, 2000, 2000, 1, 0, 4, 'npc_ascension_cultist_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50263);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 50263, 0, 28844, 1.5, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50263);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `BaseAttackTime`, `RangeAttackTime`, `unit_class`, `unit_flags`, `type`, `ScriptName`) SELECT 250042, 'Herald of the Depths', 1, 80, 35, 0, 1, 1.14286, 2000, 2000, 1, 0, 4, '' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 250042);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 250042, 0, 28844, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 250042);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `BaseAttackTime`, `RangeAttackTime`, `unit_class`, `unit_flags`, `type`, `ScriptName`) SELECT 250043, 'Strength of the Black Empire', 1, 80, 35, 0, 1, 1.14286, 2000, 2000, 1, 0, 4, '' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 250043);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 250043, 0, 28844, 1.1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 250043);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `BaseAttackTime`, `RangeAttackTime`, `unit_class`, `unit_flags`, `type`, `ScriptName`) SELECT 250044, 'Faceless Monstrosity', 1, 80, 35, 0, 1, 1.14286, 2000, 2000, 1, 0, 4, '' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 250044);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 250044, 0, 28844, 1.3, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 250044);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`) SELECT 194110, 18, 1327, 'Summoning Obelisk', 1, 3, 7720, 32783, 0, 0, 1, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry` = 194110);
DELETE FROM `spell_group` WHERE `id` = 2000185 AND `spell_id` = 560002;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES (2000185, 560002);
DELETE FROM `spell_group` WHERE `id` = 2000185 AND `spell_id` = 704434;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES (2000185, 704434);
