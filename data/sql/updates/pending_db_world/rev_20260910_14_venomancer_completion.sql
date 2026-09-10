-- Venomancer completion: new unapplied SQL14, requires matching source.
DELETE FROM `spell_proc` WHERE `SpellId` IN (92142, 92143, 92144, 300680, 300855, 500219, 500230, 503812, 503851, 503854, 503856, 503907, 503918, 503919, 503960, 503976, 504211, 504339, 504341, 504352, 504355, 504356, 504361, 504375, 504402, 504406, 504736, 504737, 504792, 504798, 504855, 560200, 560201, 560264, 560265, 560281, 573307, 574354, 630932, 630934, 631226, 680763, 680768, 680800, 681320, 681321, 704264, 705959, 705961, 705969, 705970, 705980, 705982, 705993, 705998, 706001, 706007, 706014, 706016, 706018, 706021, 706026, 706027, 706030, 706032, 706035, 706270, 706271, 706272, 706370, 706455, 706938, 706940, 706945, 706956, 707097, 707224, 707233, 707382, 707620, 707668, 800878, 800892, 800906, 803210, 804963, 804981, 804987, 805098, 805102, 805103, 805104, 805140, 805238, 805884, 805933, 806449, 806602, 806603, 806604, 807600, 808082, 808083);
INSERT INTO `spell_proc` (`SpellId`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `Chance`, `Charges`) VALUES
(92142, 1048575, 7, 2, 32767, 2, 100, 0),
(300855, 1048575, 7, 2, 32767, 2, 100, 0),
(503812, 1048575, 7, 2, 32767, 2, 100, 0),
(503851, 1048575, 7, 2, 32767, 2, 100, 0),
(503854, 1048575, 7, 2, 32767, 2, 100, 0),
(503856, 1048575, 7, 2, 32767, 2, 100, 0),
(503919, 1048575, 7, 2, 32767, 2, 100, 0),
(503960, 1048575, 7, 2, 32767, 2, 100, 0),
(503976, 1048575, 7, 2, 32767, 2, 100, 0),
(504352, 1048575, 7, 2, 32767, 2, 100, 0),
(504355, 1048575, 7, 2, 32767, 2, 100, 0),
(504361, 1048575, 7, 2, 32767, 2, 100, 0),
(504375, 1048575, 7, 2, 32767, 2, 100, 0),
(504402, 1048575, 7, 2, 32767, 2, 100, 0),
(504406, 1048575, 7, 2, 32767, 2, 100, 0),
(504792, 1048575, 7, 2, 32767, 2, 100, 0),
(560200, 1048575, 7, 2, 32767, 2, 100, 0),
(560264, 1048575, 7, 2, 32767, 2, 100, 0),
(560281, 1048575, 7, 2, 32767, 2, 100, 0),
(573307, 1048575, 7, 2, 32767, 2, 100, 0),
(574354, 1048575, 7, 2, 32767, 2, 100, 0),
(680768, 1048575, 7, 2, 32767, 2, 100, 0),
(680800, 1048575, 7, 2, 32767, 2, 100, 0),
(704264, 1048575, 7, 2, 32767, 2, 100, 0),
(705959, 1048575, 7, 2, 32767, 2, 100, 0),
(705961, 1048575, 7, 2, 32767, 2, 100, 0),
(705969, 1048575, 7, 2, 32767, 2, 100, 0),
(705980, 1048575, 7, 2, 32767, 2, 100, 0),
(705982, 1048575, 7, 2, 32767, 2, 100, 0),
(705993, 1048575, 7, 2, 32767, 2, 100, 0),
(705998, 1048575, 7, 2, 32767, 2, 100, 0),
(706001, 1048575, 7, 2, 32767, 2, 100, 0),
(706016, 1048575, 7, 2, 32767, 2, 100, 0),
(706018, 1048575, 7, 2, 32767, 2, 100, 0),
(706021, 1048575, 7, 2, 32767, 2, 100, 0),
(706030, 1048575, 7, 2, 32767, 2, 100, 0),
(706035, 1048575, 7, 2, 32767, 2, 100, 0),
(706271, 1048575, 7, 2, 32767, 2, 100, 0),
(706370, 1048575, 7, 2, 32767, 2, 100, 0),
(706455, 1048575, 7, 2, 32767, 2, 100, 0),
(706938, 1048575, 7, 2, 32767, 2, 100, 0),
(707224, 1048575, 7, 2, 32767, 2, 100, 0),
(707233, 1048575, 7, 2, 32767, 2, 100, 0),
(707382, 1048575, 7, 2, 32767, 2, 100, 0),
(707620, 1048575, 7, 2, 32767, 2, 100, 0),
(707668, 1048575, 7, 2, 32767, 2, 100, 0),
(800892, 1048575, 7, 2, 32767, 2, 100, 0),
(803210, 1048575, 7, 2, 32767, 2, 100, 0),
(804963, 1048575, 7, 2, 32767, 2, 100, 0),
(804981, 1048575, 7, 2, 32767, 2, 100, 0),
(804987, 1048575, 7, 2, 32767, 2, 100, 0),
(805098, 1048575, 7, 2, 32767, 2, 100, 0),
(805884, 1048575, 7, 2, 32767, 2, 100, 0),
(805933, 1048575, 7, 2, 32767, 2, 100, 0),
(806449, 1048575, 7, 2, 32767, 2, 100, 0),
(806603, 1048575, 7, 2, 32767, 2, 100, 0),
(806604, 1048575, 7, 2, 32767, 2, 100, 0),
(808082, 1048575, 7, 2, 32767, 2, 100, 0),
(808083, 1048575, 7, 2, 32767, 2, 100, 12);
DELETE FROM `spell_script_names` WHERE `spell_id` = 92142 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (92142, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300855 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300855, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502877 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502877, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502877 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502877, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502878 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502878, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502878 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502878, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502879 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502879, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502879 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502879, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502880 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502880, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502880 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502880, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502896 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502896, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502897 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502897, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502898 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502898, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502899 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502899, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502900 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502900, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502901 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502901, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502902 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502902, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502903 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502903, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502904 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502904, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502939 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502939, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502940 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502940, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502941 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502941, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502942 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502942, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502943 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502943, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502944 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502944, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502945 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502945, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502946 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502946, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502947 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502947, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502968 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502968, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502969 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502969, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502970 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502970, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502971 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502971, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502972 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502972, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502973 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502973, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502974 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502974, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502974 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502974, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502975 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502975, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502975 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502975, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502976 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502976, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502976 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502976, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502977 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502977, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502977 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502977, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502978 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502978, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502978 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502978, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502979 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502979, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502979 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502979, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502980 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502980, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502980 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502980, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502981 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502981, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 502981 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (502981, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503812 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503812, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503851 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503851, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503854 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503854, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503856 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503856, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503919 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503919, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503931 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503931, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503932 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503932, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503933 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503933, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503934 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503934, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503935 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503935, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503960 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503960, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503976 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503976, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503983 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503983, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503984 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503984, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503985 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503985, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503986 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503986, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504211 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504211, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504341 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504341, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504342 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504342, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504344 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504344, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504352 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504352, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504355 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504355, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504361 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504361, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504375 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504375, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504402 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504402, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504406 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504406, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504705 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504705, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504714 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504714, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504715 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504715, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504716 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504716, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504717 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504717, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504718 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504718, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504735 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504735, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504737 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504737, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504792 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504792, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504798 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504798, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504867 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504867, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 505203 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (505203, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 552781 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (552781, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 552783 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (552783, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560200 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560200, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560201 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560201, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560264 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560264, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560265 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560265, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560281 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560281, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560989 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560989, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570206 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570206, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572150 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572150, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572151 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572151, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572153 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572153, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572154 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572154, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572155 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572155, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572156 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572156, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572868 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572868, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572869 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572869, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572900 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572900, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572900 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572900, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572901 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572901, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572901 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572901, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572902 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572902, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572902 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572902, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572903 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572903, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572903 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572903, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 573307 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (573307, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 573341 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (573341, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 573342 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (573342, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 573355 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (573355, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 574354 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (574354, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 578310 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (578310, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 578342 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (578342, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 578342 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (578342, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 578343 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (578343, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 578343 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (578343, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 630932 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (630932, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 631226 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (631226, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680764 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680764, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680767 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680767, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680768 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680768, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680800 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680800, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681056 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681056, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681291 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681291, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681291 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681291, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681321 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681321, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681417 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681417, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704235 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704235, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704235 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704235, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704264 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704264, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704264 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704264, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705959 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705959, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705961 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705961, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705966 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705966, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705969 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705969, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705970 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705970, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705980 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705980, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705982 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705982, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705993 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705993, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705998 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705998, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706001 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706001, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706016 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706016, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706017 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706017, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706018 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706018, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706021 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706021, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706021 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706021, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706021 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706021, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706026 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706026, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706030 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706030, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706032 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706032, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706035 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706035, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706271 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706271, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706272 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706272, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706370 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706370, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706453 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706453, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706455 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706455, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706456 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706456, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706938 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706938, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706940 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706940, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706962 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706962, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706962 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706962, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707084 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707084, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707084 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707084, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707085 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707085, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707085 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707085, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707086 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707086, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707086 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707086, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707087 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707087, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707087 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707087, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707088 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707088, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707088 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707088, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707089 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707089, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707089 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707089, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707090 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707090, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707090 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707090, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707097 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707097, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707191 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707191, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707224 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707224, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707233 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707233, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707234 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707234, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707382 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707382, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707620 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707620, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707658 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707658, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707668 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707668, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 712357 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (712357, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800293 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800293, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800389 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800389, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800841 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800841, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800843 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800843, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800848 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800848, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800848 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800848, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800870 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800870, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800871 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800871, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800871 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800871, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800878 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800878, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800880 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800880, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800892 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800892, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800892 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800892, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800892 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800892, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800901 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800901, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800902 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800902, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800902 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800902, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800906 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800906, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800910 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800910, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800912 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800912, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800921 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800921, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800921 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800921, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800926 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800926, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800939 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800939, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800946 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800946, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800959 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800959, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800960 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800960, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801110 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801110, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801115 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801115, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801152 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801152, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803183 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803183, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803196 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803196, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803197 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803197, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803206 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803206, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803207 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803207, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803208 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803208, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803210 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803210, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803210 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803210, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803211 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803211, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803216 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803216, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803525 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803525, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803537 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803537, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804961 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804961, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804962 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804962, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804963 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804963, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804971 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804971, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804977 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804977, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804977 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804977, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804980 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804980, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804981 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804981, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804983 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804983, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804986 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804986, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804987 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804987, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805094 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805094, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805095 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805095, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805097 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805097, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805098 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805098, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805102 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805102, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805104 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805104, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805139 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805139, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805140 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805140, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805238 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805238, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805513 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805513, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805514 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805514, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805515 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805515, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805516 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805516, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805517 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805517, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805518 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805518, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805774 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805774, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805884 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805884, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805931 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805931, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805933 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805933, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806154 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806154, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806154 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806154, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806217 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806217, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806449 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806449, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806454 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806454, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806602 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806602, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806603 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806603, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806604 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806604, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807152 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807152, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807153 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807153, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807244 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807244, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807244 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807244, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807342 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807342, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807591 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807591, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807592 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807592, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807593 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807593, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807594 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807594, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807595 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807595, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807596 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807596, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807597 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807597, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807598 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807598, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807599 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807599, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807602 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807602, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807602 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807602, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807603 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807603, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807603 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807603, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807604 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807604, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807604 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807604, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807605 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807605, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807605 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807605, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807606 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807606, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807606 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807606, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807607 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807607, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807607 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807607, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807611 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807611, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807702 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807702, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807759 AND `ScriptName` = 'spell_ascension_venomancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807759, 'spell_ascension_venomancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 808082 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (808082, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 808082 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (808082, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 808083 AND `ScriptName` = 'aura_ascension_venomancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (808083, 'aura_ascension_venomancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 808083 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (808083, 'aura_ascension_venomancer_lifecycle');
DELETE FROM `spell_bonus_data` WHERE `entry` IN (502870, 502871, 502872, 502873, 502874, 502875, 502876, 502877, 502878, 502879, 502880, 502896, 502897, 502898, 502899, 502900, 502901, 502902, 502903, 502904, 502905, 502906, 502907, 502908, 502909, 502910, 502911, 502939, 502940, 502941, 502942, 502943, 502944, 502945, 502946, 502947, 502951, 502952, 502953, 502954, 502955, 502956, 502957, 502968, 502969, 502970, 502971, 502972, 502973, 502974, 502975, 502976, 502977, 502978, 502979, 502980, 502981, 503149, 503150, 503151, 503152, 503153, 503154, 503155, 503156, 503157, 503158, 503159, 503160, 503161, 503162, 503163, 503164, 503793, 503857, 503925, 503929, 503931, 503932, 503933, 503934, 503935, 503983, 503984, 503985, 503986, 504342, 504376, 504543, 504705, 504706, 504714, 504715, 504716, 504717, 504718, 504735, 504794, 504796, 504803, 504869, 552781, 552783, 560202, 560248, 570206, 570208, 572037, 572038, 572039, 572150, 572151, 572153, 572154, 572155, 572156, 572868, 572869, 572900, 572901, 572902, 572903, 573061, 573062, 573063, 573333, 573334, 573335, 573341, 573342, 573355, 578310, 578342, 578343, 681301, 704271, 705985, 706392, 706456, 706962, 707084, 707085, 707086, 707087, 707088, 707089, 707090, 707234, 707594, 800869, 800870, 800871, 800880, 800882, 800899, 800901, 800902, 800926, 800939, 800946, 800959, 801110, 801115, 801152, 803178, 803193, 803195, 803199, 803231, 803232, 803233, 803234, 803235, 803529, 803538, 803539, 803540, 803541, 803542, 803543, 803544, 803545, 803570, 803641, 803642, 803643, 803644, 804961, 804977, 804983, 804986, 805094, 805348, 805513, 805514, 805515, 805516, 805517, 805518, 807152, 807342, 807591, 807592, 807593, 807594, 807595, 807596, 807597, 807598, 807599, 807602, 807603, 807604, 807605, 807606, 807607, 807611, 808084);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(502870, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502871, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502872, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502873, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502874, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502875, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502876, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502877, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502878, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502879, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502880, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502896, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502897, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502898, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502899, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502900, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502901, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502902, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502903, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502904, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502905, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502906, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502907, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502908, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502909, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502910, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502911, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502939, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502940, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502941, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502942, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502943, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502944, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502945, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502946, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502947, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502951, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502952, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502953, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502954, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502955, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502956, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502957, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502968, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502969, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502970, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502971, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502972, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502973, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502974, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502975, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502976, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502977, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502978, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502979, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502980, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(502981, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503149, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503150, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503151, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503152, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503153, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503154, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503155, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503156, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503157, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503158, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503159, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503160, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503161, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503162, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503163, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503164, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503793, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503857, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503925, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503929, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503931, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503932, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503933, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503934, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503935, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503983, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503984, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503985, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(503986, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(504342, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(504376, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(504543, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(504705, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(504706, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(504714, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(504715, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(504716, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(504717, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(504718, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(504735, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(504794, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(504796, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(504803, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(504869, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(552781, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(552783, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(560202, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(560248, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(570206, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(570208, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(572037, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(572038, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(572039, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(572150, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(572151, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(572153, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(572154, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(572155, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(572156, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(572868, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(572869, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(572900, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(572901, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(572902, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(572903, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(573061, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(573062, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(573063, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(573333, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(573334, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(573335, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(573341, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(573342, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(573355, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(578310, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(578342, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(578343, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(681301, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(704271, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(705985, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(706392, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(706456, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(706962, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(707084, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(707085, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(707086, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(707087, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(707088, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(707089, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(707090, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(707234, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(707594, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(800869, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(800870, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(800871, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(800880, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(800882, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(800899, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(800901, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(800902, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(800926, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(800939, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(800946, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(800959, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(801110, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(801115, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(801152, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803178, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803193, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803195, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803199, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803231, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803232, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803233, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803234, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803235, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803529, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803538, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803539, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803540, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803541, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803542, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803543, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803544, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803545, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803570, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803641, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803642, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803643, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(803644, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(804961, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(804977, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(804983, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(804986, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(805094, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(805348, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(805513, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(805514, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(805515, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(805516, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(805517, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(805518, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(807152, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(807342, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(807591, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(807592, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(807593, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(807594, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(807595, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(807596, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(807597, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(807598, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(807599, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(807602, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(807603, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(807604, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(807605, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(807606, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(807607, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(807611, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount'),
(808084, 0, 0, 0, 0, 'Venomancer: explicit coefficient or forwarded amount');
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `ScriptName`) SELECT 45896, 'Fungarian', 1, 80, 35, 0, 1, 'npc_ascension_venomancer_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 45896);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 45896, 0, 18227, 0.5, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 45896);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `ScriptName`) SELECT 506018, 'Mushroom', 1, 80, 35, 0, 1, 'npc_ascension_venomancer_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 506018);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 506018, 0, 26981, 0.55, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 506018);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `ScriptName`) SELECT 52121, 'Brood Trap', 1, 80, 35, 0, 1, 'npc_ascension_venomancer_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 52121);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 52121, 0, 23058, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 52121);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `ScriptName`) SELECT 999298, 'Spiderling', 1, 80, 35, 0, 1, 'npc_ascension_venomancer_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 999298);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 999298, 0, 955, 0.35, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 999298);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_flags`, `type`, `ScriptName`) SELECT 999299, 'Scarab', 1, 80, 35, 0, 1, 'npc_ascension_venomancer_summon' WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 999299);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) SELECT 999299, 0, 10005, 0.6, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 999299);
DELETE FROM `spell_group` WHERE `id` = 2000185 AND `spell_id` = 300974;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES (2000185, 300974);
DELETE FROM `spell_group` WHERE `id` = 1094 AND `spell_id` = 504348;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES (1094, 504348);
