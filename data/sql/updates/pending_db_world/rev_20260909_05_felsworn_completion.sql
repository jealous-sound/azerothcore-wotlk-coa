-- Felsworn completion; requires matching source. No earlier migration is rewritten.
-- Guarded creature/gameobject definitions require an absent-or-exact installation check.
DELETE FROM `spell_proc` WHERE `SpellId` IN (92087, 92088, 92089, 300468, 300474, 300476, 300478, 300480, 300483, 300484, 300489, 300490, 520240, 520244, 520246, 520257, 520805, 520817, 520833, 520853, 524947, 560503, 560542, 560637, 560639, 560641, 560645, 560838, 560840, 570077, 570158, 572889, 574143, 574145, 574146, 681376, 685321, 704360, 704370, 704374, 704606, 704610, 705131, 705135, 705137, 705145, 706267, 706425, 707513, 707902, 800214, 801235, 801892, 801899, 802058, 803478, 803904, 804822, 805236, 805239, 805245, 805249, 805998, 806111, 806235, 807426, 807431, 807438, 807461);
INSERT INTO `spell_proc` (`SpellId`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `Chance`) VALUES
(92087, 1048575, 7, 2, 63, 2, 100),
(300474, 1048575, 7, 2, 63, 2, 100),
(300476, 1048575, 7, 2, 63, 2, 100),
(300478, 1048575, 7, 2, 63, 2, 100),
(300480, 1048575, 7, 2, 63, 2, 100),
(300483, 1048575, 7, 2, 63, 2, 100),
(300484, 1048575, 7, 2, 63, 2, 100),
(300490, 1048575, 7, 2, 63, 2, 100),
(520244, 1048575, 7, 2, 63, 2, 100),
(520246, 1048575, 7, 2, 63, 2, 100),
(520833, 1048575, 7, 2, 63, 2, 100),
(560503, 1048575, 7, 2, 63, 2, 100),
(560542, 1048575, 7, 2, 63, 2, 100),
(560639, 1048575, 7, 2, 63, 2, 100),
(570077, 1048575, 7, 2, 63, 2, 100),
(570158, 1048575, 7, 2, 63, 2, 100),
(572889, 1048575, 7, 2, 63, 2, 100),
(574143, 1048575, 7, 2, 63, 2, 100),
(574145, 1048575, 7, 2, 63, 2, 100),
(681376, 1048575, 7, 2, 63, 2, 100),
(685321, 1048575, 7, 2, 63, 2, 100),
(704370, 1048575, 7, 2, 63, 2, 100),
(704606, 1048575, 7, 2, 63, 2, 100),
(704610, 1048575, 7, 2, 63, 2, 100),
(705131, 1048575, 7, 2, 63, 2, 100),
(707902, 1048575, 7, 2, 63, 2, 100),
(800214, 1048575, 7, 2, 63, 2, 100),
(801899, 1048575, 7, 2, 63, 2, 100),
(803904, 1048575, 7, 2, 63, 2, 100),
(804822, 1048575, 7, 2, 63, 2, 100),
(805239, 1048575, 7, 2, 63, 2, 100),
(805245, 1048575, 7, 2, 63, 2, 100),
(805249, 1048575, 7, 2, 63, 2, 100),
(805998, 1048575, 7, 2, 63, 2, 100),
(806111, 1048575, 7, 2, 63, 2, 100),
(806235, 1048575, 7, 2, 63, 2, 100),
(807426, 1048575, 7, 2, 63, 2, 100),
(807461, 1048575, 7, 2, 63, 2, 100);
DELETE FROM `spell_script_names` WHERE `spell_id` = 92087 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (92087, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 92089 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (92089, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300470 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300470, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300474 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300474, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300476 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300476, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300478 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300478, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300480 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300480, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300483 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300483, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300484 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300484, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300486 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300486, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 300490 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (300490, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500610 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500610, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520244 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520244, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520246 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520246, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520252 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520252, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520253 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520253, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520833 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520833, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 520853 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (520853, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 525001 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (525001, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 525027 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (525027, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 552210 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (552210, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 552211 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (552211, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 552212 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (552212, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 552213 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (552213, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 555276 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (555276, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 555277 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (555277, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 555738 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (555738, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560503 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560503, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560542 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560542, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560639 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560639, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561216 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561216, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570077 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570077, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570158 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570158, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572163 AND `ScriptName` = 'spell_ascension_felsworn_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572163, 'spell_ascension_felsworn_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572183 AND `ScriptName` = 'spell_ascension_felsworn_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572183, 'spell_ascension_felsworn_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572889 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572889, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 574140 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (574140, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 574143 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (574143, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 574145 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (574145, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 574145 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (574145, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 574146 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (574146, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681376 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681376, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 685321 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (685321, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704368 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704368, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704370 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704370, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704606 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704606, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704610 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704610, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704611 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704611, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705121 AND `ScriptName` = 'spell_ascension_felsworn_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705121, 'spell_ascension_felsworn_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705122 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705122, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705129 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705129, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705131 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705131, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705146 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705146, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706269 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706269, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706415 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706415, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706416 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706416, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706417 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706417, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706418 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706418, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706419 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706419, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706420 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706420, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706818 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706818, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707518 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707518, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707523 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707523, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707524 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707524, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707525 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707525, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707526 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707526, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707527 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707527, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707901 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707901, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707902 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707902, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707902 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707902, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707903 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707903, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 712483 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (712483, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800029 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800029, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800031 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800031, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800206 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800206, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800214 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800214, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800355 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800355, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801899 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801899, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801902 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801902, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802058 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802058, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802108 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802108, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803343 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803343, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803465 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803465, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803478 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803478, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803716 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803716, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803881 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803881, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803882 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803882, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803883 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803883, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803884 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803884, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803885 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803885, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803904 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803904, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803904 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803904, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804216 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804216, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804610 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804610, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804764 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804764, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804822 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804822, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804823 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804823, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804823 AND `ScriptName` = 'aura_ascension_felsworn_resolve';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804823, 'aura_ascension_felsworn_resolve');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805236 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805236, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805239 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805239, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805245 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805245, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805249 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805249, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805998 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805998, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 805998 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (805998, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806109 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806109, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806111 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806111, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806128 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806128, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806235 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806235, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807163 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807163, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807421 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807421, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807424 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807424, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807426 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807426, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807426 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807426, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807428 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807428, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807438 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807438, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807461 AND `ScriptName` = 'aura_ascension_felsworn_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807461, 'aura_ascension_felsworn_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807727 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807727, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807727 AND `ScriptName` = 'aura_ascension_felsworn_stagger';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807727, 'aura_ascension_felsworn_stagger');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807942 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807942, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807942 AND `ScriptName` = 'spell_ascension_felsworn_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807942, 'spell_ascension_felsworn_ability');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807962 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807962, 'aura_ascension_felsworn_lifecycle');
DELETE FROM `spell_bonus_data` WHERE `entry` IN (300469, 501270, 501271, 501272, 501281, 501282, 501283, 501284, 501285, 501286, 501287, 501288, 501289, 501290, 501291, 501292, 501293, 501294, 501295, 501296, 501297, 501298, 501299, 501314, 501315, 501316, 501317, 501318, 501319, 501320, 501321, 520236, 520262, 520688, 520689, 520690, 520691, 520692, 520693, 520806, 520832, 555742, 556503, 560284, 560627, 560839, 563271, 563730, 563731, 563732, 563733, 563734, 563735, 570159, 572585, 572615, 572616, 572617, 574166, 705124, 705129, 707523, 707524, 707525, 707526, 707527, 712399, 712483, 800204, 800207, 800208, 800355, 800598, 801312, 801895, 802060, 802676, 802677, 802678, 803467, 803484, 803485, 803486, 803487, 803715, 805240, 805241, 805748, 806062, 806096, 806112, 807347, 807554);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(300469, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501270, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501271, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501272, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501281, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501282, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501283, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501284, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501285, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501286, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501287, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501288, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501289, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501290, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501291, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501292, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501293, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501294, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501295, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501296, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501297, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501298, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501299, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501314, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501315, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501316, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501317, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501318, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501319, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501320, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(501321, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(520236, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(520262, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(520688, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(520689, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(520690, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(520691, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(520692, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(520693, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(520806, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(520832, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(555742, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(556503, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(560284, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(560627, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(560839, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(563271, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(563730, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(563731, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(563732, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(563733, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(563734, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(563735, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(570159, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(572585, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(572615, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(572616, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(572617, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(574166, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(705124, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(705129, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(707523, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(707524, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(707525, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(707526, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(707527, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(712399, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(712483, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(800204, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(800207, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(800208, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(800355, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(800598, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(801312, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(801895, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(802060, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(802676, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(802677, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(802678, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(803467, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(803484, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(803485, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(803486, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(803487, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(803715, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(805240, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(805241, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(805748, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(806062, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(806096, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(806112, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(807347, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result'),
(807554, 0, 0, 0, 0, 'Felsworn: explicit coefficient or actual result');
-- Match the native Mark/Gift of the Wild group without rewriting its existing members.
DELETE FROM `spell_group` WHERE `id` = 1078 AND `spell_id` IN (523478, 523495);
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES (1078, 523478), (1078, 523495);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `flags_extra`, `ScriptName`)
SELECT 51320, 'Fel Infernal', 1, 1, 35, 1, 3, 2000, 2000, 64, 'npc_ascension_felsworn_infernal'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 51320);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 51320, 0, 169, 0.65, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 51320);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `ScriptName`)
SELECT 9000140, 10, 4396, 'Fel Rift: Stormwind', 1, 'go_ascension_felsworn_rift'
WHERE NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry` = 9000140);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `ScriptName`)
SELECT 9000141, 10, 4394, 'Fel Rift: Ironforge', 1, 'go_ascension_felsworn_rift'
WHERE NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry` = 9000141);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `ScriptName`)
SELECT 9000142, 10, 4393, 'Fel Rift: Darnassus', 1, 'go_ascension_felsworn_rift'
WHERE NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry` = 9000142);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `ScriptName`)
SELECT 9000143, 10, 4395, 'Fel Rift: Orgrimmar', 1, 'go_ascension_felsworn_rift'
WHERE NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry` = 9000143);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `ScriptName`)
SELECT 9000144, 10, 4397, 'Fel Rift: Thunder Bluff', 1, 'go_ascension_felsworn_rift'
WHERE NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry` = 9000144);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `ScriptName`)
SELECT 9000145, 10, 4398, 'Fel Rift: Undercity', 1, 'go_ascension_felsworn_rift'
WHERE NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry` = 9000145);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `ScriptName`)
SELECT 9000146, 10, 7848, 'Fel Rift: Theramore', 1, 'go_ascension_felsworn_rift'
WHERE NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry` = 9000146);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `ScriptName`)
SELECT 9000147, 10, 4398, 'Fel Rift: Altar of Storms', 1, 'go_ascension_felsworn_rift'
WHERE NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry` = 9000147);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `ScriptName`)
SELECT 9000148, 10, 4398, 'Fel Rift: Mannoroc Coven', 1, 'go_ascension_felsworn_rift'
WHERE NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry` = 9000148);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `ScriptName`)
SELECT 9000149, 10, 4398, 'Fel Rift: Dark Whisper Gorge', 1, 'go_ascension_felsworn_rift'
WHERE NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry` = 9000149);
