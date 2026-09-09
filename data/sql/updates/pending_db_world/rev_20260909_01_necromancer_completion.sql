-- Necromancer completion. Source and additive server model candidates are one future install package.
-- Applied migrations and the pending Witch Doctor migration are immutable dependencies.
-- Check guarded missing-template insertions against the exact candidate before installation.
DELETE FROM `spell_proc` WHERE `SpellId` IN (805011, 805015);
INSERT INTO `spell_proc` (`SpellId`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `Chance`) VALUES
(805011, 1048575, 3, 2, 3, 2, 100),
(805015, 1048575, 3, 2, 3, 2, 100);
DELETE FROM `spell_script_names` WHERE `spell_id` = 301207 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (301207, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 302586 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (302586, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 302913 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (302913, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500217 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500217, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500329 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500329, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500330 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500330, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500331 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500331, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500335 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500335, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500338 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500338, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500365 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500365, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500443 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500443, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500456 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500456, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500457 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500457, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500458 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500458, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500459 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500459, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500539 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500539, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500730 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500730, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500933 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500933, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500965 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500965, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500968 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500968, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500969 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500969, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500970 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500970, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500971 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500971, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500981 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500981, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500982 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500982, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500983 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500983, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500985 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500985, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500989 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500989, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500991 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500991, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501055 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501055, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501855 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501855, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501856 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501856, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501857 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501857, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501858 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501858, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501859 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501859, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501860 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501860, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501861 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501861, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501880 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501880, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501881 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501881, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501882 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501882, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501883 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501883, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501884 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501884, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501885 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501885, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501886 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501886, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501887 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501887, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501888 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501888, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501890 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501890, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501891 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501891, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501892 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501892, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501940 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501940, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501941 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501941, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501942 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501942, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503095 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503095, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503640 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503640, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504020 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504020, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504021 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504021, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504050 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504050, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504052 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504052, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504313 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504313, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504316 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504316, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504318 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504318, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504319 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504319, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504334 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504334, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504489 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504489, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504505 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504505, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504682 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504682, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504859 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504859, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504860 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504860, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504861 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504861, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504862 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504862, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504863 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504863, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504864 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504864, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504865 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504865, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504866 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504866, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504868 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504868, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504901 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504901, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504905 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504905, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504907 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504907, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504908 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504908, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520367 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520367, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 525379 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (525379, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 525380 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (525380, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 525388 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (525388, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 525388 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (525388, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 525389 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (525389, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 525600 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (525600, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 531126 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (531126, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 531130 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (531130, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 533236 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (533236, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 533237 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (533237, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 533238 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (533238, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 533239 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (533239, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560012 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560012, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561138 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561138, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561316 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561316, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 562210 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (562210, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 562211 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (562211, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 562212 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (562212, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 562213 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (562213, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570131 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570131, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570132 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570132, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572638 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572638, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572777 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572777, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572842 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572842, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572843 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572843, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572844 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572844, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572845 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572845, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 573233 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (573233, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 583255 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (583255, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 583256 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (583256, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 600992 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (600992, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680388 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680388, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681460 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681460, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681529 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681529, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704292 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704292, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704676 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704676, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704722 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704722, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705746 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705746, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705754 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705754, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706472 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706472, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707007 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707007, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707176 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707176, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707194 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707194, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800936 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800936, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800979 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800979, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801545 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801545, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801747 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801747, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801938 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801938, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801941 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801941, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801945 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801945, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802121 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802121, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802122 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802122, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803139 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803139, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803530 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803530, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803767 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803767, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803773 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803773, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803781 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803781, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803782 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803782, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804371 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804371, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804681 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804681, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805011 AND `ScriptName` = 'aura_ascension_necromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805011, 'aura_ascension_necromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805015 AND `ScriptName` = 'aura_ascension_necromancer_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805015, 'aura_ascension_necromancer_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805027 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805027, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805032 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805032, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805038 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805038, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805044 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805044, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805048 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805048, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805049 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805049, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805197 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805197, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805252 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805252, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805426 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805426, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805428 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805428, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805871 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805871, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805977 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805977, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806087 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806087, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807098 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807098, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807796 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807796, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807811 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807811, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807813 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807813, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807856 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807856, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807938 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807938, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807939 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807939, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807940 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807940, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807941 AND `ScriptName` = 'spell_ascension_necromancer_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807941, 'spell_ascension_necromancer_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 850012 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (850012, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 850013 AND `ScriptName` = 'aura_ascension_necromancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (850013, 'aura_ascension_necromancer_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 899900 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (899900, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 899901 AND `ScriptName` = 'spell_ascension_necromancer_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (899901, 'spell_ascension_necromancer_summon');
DELETE FROM `spell_bonus_data` WHERE `entry` IN (302509, 500237, 500267, 500338, 500365, 500585, 500968, 501855, 501856, 501857, 501858, 501859, 501860, 501861, 501890, 501891, 501892, 501940, 501941, 501942, 501969, 501970, 501971, 501972, 501973, 501974, 501975, 501976, 501977, 501978, 501979, 501980, 503095, 503640, 504022, 505224, 505225, 533240, 561095, 561318, 570042, 570050, 570131, 572842, 572843, 572844, 572845, 573242, 583255, 583256, 680928, 681463, 706450, 706662, 707002, 707010, 707194, 707284, 707575, 707592, 707598, 801241, 801411, 801513, 801514, 801516, 801518, 801545, 801722, 801728, 801945, 802122, 802130, 802132, 802353, 803779, 805031, 808016);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(302509, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(500237, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(500267, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(500338, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(500365, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(500585, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(500968, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501855, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501856, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501857, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501858, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501859, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501860, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501861, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501890, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501891, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501892, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501940, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501941, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501942, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501969, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501970, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501971, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501972, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501973, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501974, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501975, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501976, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501977, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501978, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501979, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(501980, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(503095, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(503640, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(504022, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(505224, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(505225, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(533240, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(561095, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(561318, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(570042, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(570050, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(570131, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(572842, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(572843, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(572844, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(572845, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(573242, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(583255, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(583256, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(680928, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(681463, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(706450, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(706662, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(707002, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(707010, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(707194, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(707284, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(707575, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(707592, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(707598, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(801241, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(801411, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(801513, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(801514, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(801516, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(801518, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(801545, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(801722, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(801728, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(801945, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(802122, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(802130, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(802132, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(802353, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(803779, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(805031, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result'),
(808016, 0, 0, 0, 0, 'Necromancer: explicit base or forwarded actual result');
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50065, 'Lesser Skeletal Warrior', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50065);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50065, 0, 9786, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50065);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50067, 'Raised Gargoyle', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50067);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50067, 0, 33924, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50067);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50068, 'Abomination', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50068);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50068, 0, 15958, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50068);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50073, 'Ghoul', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50073);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50073, 0, 10626, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50073);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50075, 'Skeletal Mage', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50075);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50075, 0, 11396, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50075);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50076, 'Skeletal Archer', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50076);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50076, 0, 7550, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50076);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50078, 'Skeletal Rogue', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50078);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50078, 0, 136859, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50078);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50115, 'Decaying Colossus', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50115);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50115, 0, 95426, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50115);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50132, 'Phylactery', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50132);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50132, 0, 24889, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50132);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50133, 'Muckworm', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50133);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50133, 0, 9905, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50133);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50173, 'Frenzied Ghoul', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50173);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50173, 0, 14708, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50173);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50177, 'Frost Wyrm', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50177);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50177, 0, 27064, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50177);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50261, 'Skeletal Smith', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50261);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50261, 0, 25498, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50261);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50303, 'Rotling', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50303);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50303, 0, 98192, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50303);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50309, 'Bone Wraith', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50309);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50309, 0, 30810, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50309);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50320, 'Tomb King', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50320);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50320, 0, 94932, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50320);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50323, 'Crypt Fiend', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50323);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50323, 0, 17308, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50323);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 50385, 'Plaguefather', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50385);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50385, 0, 94473, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50385);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 51065, 'Greater Skeletal Warrior', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 51065);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 51065, 0, 775, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 51065);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 51275, 'Rotting Frost Giant', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 51275);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 51275, 0, 402059, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 51275);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 444914, 'Foul Invocation', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 444914);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 444914, 0, 310061, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 444914);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 500482, 'Gravebound Champion', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 500482);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 500482, 0, 561889, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 500482);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 500483, 'Spellbound Champion', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 500483);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 500483, 0, 561890, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 500483);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 500484, 'Icebound Champion', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 500484);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 500484, 0, 561888, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 500484);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 500650, 'Banshee', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 500650);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 500650, 0, 10752, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 500650);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 503030, 'Lesser Zombie', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 503030);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 503030, 0, 10971, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 503030);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 503031, 'Zombie', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 503031);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 503031, 0, 10975, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 503031);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 503032, 'Greater Zombie', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 503032);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 503032, 0, 10973, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 503032);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 503200, 'Bone Construct', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 503200);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 503200, 0, 12073, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 503200);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 523032, 'Zombie', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 523032);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 523032, 0, 25495, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 523032);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 542064, 'Tombstone', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 542064);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 542064, 0, 977363, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 542064);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 542065, 'Risen Ghoul', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 542065);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 542065, 0, 26079, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 542065);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 575091, 'Scourge Transporter', 1, 1, 35, 1, 6, 2000, 2000, 64, 'npc_ascension_necromancer'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 575091);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 575091, 0, 405700, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 575091);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`, `DisplayID_Other_Gender`, `VerifiedBuild`)
SELECT 14708, 0.7995, 2.25, 2, 0, 0 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 14708);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 33924, 0.61110, 2.03100, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 33924);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 94473, 2.03128, 1.00000, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 94473);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 94932, 2.03128, 1.00000, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 94932);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 95426, 0.61110, 2.03100, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 95426);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 98192, 2.03128, 1.00000, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 98192);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 136859, 2.03128, 1.00000, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 136859);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 310061, 0.61110, 2.03100, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 310061);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 402059, 0.61110, 2.03100, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 402059);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 405700, 0.30000, 1.00000, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 405700);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 977363, 0.50833, 1.57374, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 977363);
DELETE FROM `spell_group` WHERE `id` = 2000184 AND `spell_id` = 503738;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES (2000184, 503738);
DELETE FROM `spell_group` WHERE `id` = 2000182 AND `spell_id` = 560537;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES (2000182, 560537);
DELETE FROM `spell_group` WHERE `id` = 2000181 AND `spell_id` = 300958;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES (2000181, 300958);
UPDATE `item_template` SET `spellcategorycooldown_1` = 60000 WHERE `entry` IN (11521, 555555) AND `spellcategorycooldown_1` = 120000;
