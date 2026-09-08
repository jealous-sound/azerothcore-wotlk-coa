-- Witch Doctor completion. Generated from the pinned class audit and recovered client data.
-- Install with matching source and server DBC candidates; check every missing-template guard first.
DELETE FROM `spell_proc` WHERE `SpellId` IN (92084, 804620, 807904, 705871, 503707, 503714, 704495, 681242, 704503, 705844, 705912, 705917, 706557, 802092, 802756, 705925, 801716, 706551, 807450, 560748, 802975, 803271, 500962);
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(92084, 0, 0, 0, 0, 0, 332116, 1, 2, 3, 2, 0, 0, 100, 0, 0),
(804620, 0, 0, 0, 0, 0, 349524, 3, 2, 3, 2, 0, 0, 8, 0, 0),
(807904, 0, 0, 0, 0, 0, 349524, 3, 2, 3, 2, 0, 0, 15, 0, 0),
(705871, 0, 0, 0, 0, 0, 349524, 3, 2, 3, 2, 0, 0, 5, 0, 0),
(503707, 0, 0, 0, 0, 0, 262144, 1, 0, 3, 2, 0, 0, 20, 0, 0),
(503714, 0, 0, 0, 0, 0, 332116, 1, 2, 2, 2, 0, 0, 100, 0, 0),
(704495, 0, 0, 0, 0, 0, 332116, 1, 2, 2, 2, 0, 0, 100, 0, 0),
(681242, 0, 0, 0, 0, 0, 320, 1, 2, 3, 2, 0, 0, 100, 0, 0),
(704503, 0, 0, 0, 0, 0, 332116, 1, 2, 3, 2, 0, 0, 100, 0, 0),
(705844, 0, 0, 0, 0, 0, 332116, 1, 2, 3, 2, 0, 0, 15, 0, 0),
(705912, 0, 0, 0, 0, 0, 332116, 1, 2, 3, 2, 0, 0, 100, 0, 0),
(705917, 0, 0, 0, 0, 0, 332116, 1, 2, 3, 2, 0, 0, 100, 0, 0),
(706557, 0, 0, 0, 0, 0, 332116, 1, 2, 3, 2, 0, 0, 100, 0, 0),
(802092, 0, 0, 0, 0, 0, 332116, 1, 2, 3, 2, 0, 0, 100, 0, 0),
(802756, 0, 0, 0, 0, 0, 332116, 1, 2, 3, 2, 0, 0, 100, 0, 0),
(705925, 0, 0, 0, 0, 0, 262144, 1, 0, 3, 2, 0, 0, 25, 0, 0),
(801716, 0, 0, 0, 0, 0, 262144, 1, 0, 3, 2, 0, 0, 100, 0, 0),
(706551, 0, 0, 0, 0, 0, 262144, 1, 0, 3, 2, 0, 0, 100, 0, 0),
(807450, 0, 0, 0, 0, 0, 262144, 1, 0, 3, 2, 0, 0, 30, 3000, 0),
(560748, 0, 0, 0, 0, 0, 332116, 1, 2, 3, 2, 0, 0, 100, 0, 0),
(802975, 0, 0, 0, 0, 0, 332116, 1, 2, 3, 2, 0, 0, 100, 500, 0),
(803271, 0, 0, 0, 0, 0, 332116, 1, 2, 3, 2, 0, 0, 100, 500, 0),
(500962, 0, 0, 0, 0, 0, 559104, 2, 2, 3, 2, 0, 0, 100, 0, 0);
DELETE FROM `spell_script_names` WHERE `spell_id` = 92084 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (92084, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 92086 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (92086, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500015 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500015, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500947 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500947, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500950 AND `ScriptName` = 'aura_ascension_witch_doctor_beam';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500950, 'aura_ascension_witch_doctor_beam');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500957 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500957, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500960 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500960, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500961 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500961, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500962 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500962, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 500962 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500962, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501080 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501080, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501081 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501081, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501082 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501082, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501083 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501083, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501084 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501084, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501085 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501085, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501086 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501086, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501087 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501087, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501088 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501088, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501114 AND `ScriptName` = 'aura_ascension_witch_doctor_beam';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501114, 'aura_ascension_witch_doctor_beam');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501115 AND `ScriptName` = 'aura_ascension_witch_doctor_beam';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501115, 'aura_ascension_witch_doctor_beam');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501116 AND `ScriptName` = 'aura_ascension_witch_doctor_beam';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501116, 'aura_ascension_witch_doctor_beam');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501117 AND `ScriptName` = 'aura_ascension_witch_doctor_beam';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501117, 'aura_ascension_witch_doctor_beam');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501118 AND `ScriptName` = 'aura_ascension_witch_doctor_beam';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501118, 'aura_ascension_witch_doctor_beam');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501136 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501136, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501136 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501136, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501153 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501153, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501154 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501154, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501155 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501155, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501156 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501156, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501157 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501157, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 501158 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (501158, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503707 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503707, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 503714 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (503714, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504465 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504465, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504608 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504608, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504759 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504759, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504777 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504777, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 504840 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (504840, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 505339 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (505339, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 506634 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (506634, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 506635 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (506635, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 507082 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (507082, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 507084 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (507084, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 547573 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (547573, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560294 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560294, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560748 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560748, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 560748 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (560748, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561017 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561017, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561067 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561067, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561068 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561068, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561136 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561136, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561137 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561137, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561140 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561140, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561141 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561141, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561142 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561142, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561143 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561143, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561299 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561299, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 561361 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (561361, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 562715 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (562715, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 562716 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (562716, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 562717 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (562717, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 562718 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (562718, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 562719 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (562719, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 570064 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (570064, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572577 AND `ScriptName` = 'aura_ascension_witch_doctor_beam';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572577, 'aura_ascension_witch_doctor_beam');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572836 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572836, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 572899 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (572899, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 578344 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (578344, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 674303 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (674303, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680872 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680872, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 680906 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (680906, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681007 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681007, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 681242 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (681242, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704495 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704495, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704497 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704497, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 704503 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (704503, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705844 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705844, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705870 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705870, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705871 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705871, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705912 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705912, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705917 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705917, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705925 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705925, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 705943 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (705943, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706369 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706369, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706542 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706542, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706551 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706551, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706557 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (706557, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707162 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707162, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707650 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707650, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707671 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707671, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707672 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707672, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707673 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707673, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707674 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707674, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707675 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707675, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707676 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707676, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 707677 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (707677, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 712373 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (712373, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 712458 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (712458, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800195 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800195, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800330 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800330, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 800330 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800330, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801607 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801607, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801660 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801660, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801662 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801662, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801663 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801663, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801664 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801664, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801678 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801678, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801693 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801693, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801716 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801716, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801716 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801716, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 801925 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (801925, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802065 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802065, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802092 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802092, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802100 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802100, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802711 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802711, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802719 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802719, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802720 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802720, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802756 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802756, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 802975 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (802975, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803271 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803271, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803325 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803325, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803387 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803387, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803678 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803678, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 803732 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803732, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804226 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804226, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804620 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804620, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 804684 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804684, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806285 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806285, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806289 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806289, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 806346 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (806346, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807040 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807040, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807057 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807057, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807214 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807214, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807450 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807450, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807829 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807829, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807830 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807830, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807831 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807831, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807832 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807832, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807833 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807833, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807904 AND `ScriptName` = 'aura_ascension_witch_doctor_event';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807904, 'aura_ascension_witch_doctor_event');
DELETE FROM `spell_script_names` WHERE `spell_id` = 808004 AND `ScriptName` = 'spell_ascension_witch_doctor_summon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (808004, 'spell_ascension_witch_doctor_summon');
DELETE FROM `spell_script_names` WHERE `spell_id` = 811111 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (811111, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 811112 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (811112, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 811113 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (811113, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 811114 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (811114, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 811115 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (811115, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 811117 AND `ScriptName` = 'aura_ascension_witch_doctor_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (811117, 'aura_ascension_witch_doctor_lifecycle');
DELETE FROM `spell_bonus_data` WHERE `entry` IN (500015, 500473, 500478, 500747, 500950, 501080, 501081, 501082, 501083, 501084, 501085, 501086, 501087, 501088, 501114, 501115, 501116, 501117, 501118, 501134, 501139, 501140, 501141, 501142, 501143, 501144, 501145, 501146, 501147, 501148, 501149, 501150, 501153, 501154, 501155, 501156, 501157, 501158, 501174, 501175, 501176, 501177, 501178, 501179, 501180, 501181, 501198, 501199, 501200, 501201, 501202, 501203, 501204, 501205, 501206, 501207, 501208, 501209, 501210, 501211, 501212, 501213, 504452, 504582, 504606, 504608, 547574, 560747, 562715, 562716, 562717, 562718, 562719, 567731, 567732, 567733, 567734, 567735, 567736, 570156, 570185, 570240, 570241, 572336, 572337, 572364, 572365, 572577, 572837, 573021, 573022, 573023, 573024, 573025, 573026, 573027, 573265, 573430, 573431, 573432, 573433, 573434, 573435, 680906, 680908, 680909, 680910, 705929, 706553, 706898, 712415, 712453, 801607, 801661, 801669, 801670, 801674, 801677, 801693, 801696, 801797, 802087, 802489, 802703, 802704, 802710, 802712, 802717, 802971, 803287, 803699, 804684, 805403, 806289, 806346, 806473, 806616, 807037, 807042, 807211, 807477, 807480, 808002, 808057, 808648, 808876, 809365, 811111, 811112, 811113, 811114, 811115, 811117, 899906);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(500015, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(500473, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(500478, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(500747, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(500950, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501080, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501081, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501082, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501083, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501084, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501085, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501086, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501087, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501088, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501114, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501115, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501116, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501117, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501118, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501134, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501139, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501140, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501141, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501142, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501143, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501144, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501145, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501146, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501147, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501148, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501149, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501150, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501153, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501154, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501155, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501156, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501157, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501158, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501174, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501175, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501176, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501177, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501178, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501179, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501180, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501181, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501198, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501199, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501200, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501201, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501202, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501203, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501204, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501205, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501206, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501207, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501208, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501209, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501210, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501211, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501212, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(501213, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(504452, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(504582, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(504606, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(504608, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(547574, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(560747, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(562715, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(562716, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(562717, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(562718, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(562719, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(567731, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(567732, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(567733, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(567734, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(567735, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(567736, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(570156, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(570185, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(570240, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(570241, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(572336, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(572337, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(572364, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(572365, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(572577, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(572837, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(573021, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(573022, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(573023, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(573024, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(573025, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(573026, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(573027, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(573265, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(573430, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(573431, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(573432, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(573433, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(573434, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(573435, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(680906, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(680908, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(680909, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(680910, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(705929, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(706553, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(706898, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(712415, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(712453, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(801607, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(801661, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(801669, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(801670, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(801674, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(801677, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(801693, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(801696, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(801797, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(802087, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(802489, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(802703, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(802704, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(802710, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(802712, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(802717, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(802971, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(803287, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(803699, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(804684, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(805403, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(806289, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(806346, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(806473, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(806616, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(807037, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(807042, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(807211, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(807477, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(807480, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(808002, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(808057, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(808648, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(808876, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(809365, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(811111, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(811112, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(811113, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(811114, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(811115, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(811117, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient'),
(899906, 0, 0, 0, 0, 'Witch Doctor: explicit base or forwarded coefficient');
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 50104, 'Healing Ward', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50104);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50104, 0, 15292, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50104);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 50105, 'Serpent Ward', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50105);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50105, 0, 16986, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50105);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 50106, 'Spirit Idol', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50106);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50106, 0, 410147, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50106);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 50108, 'Stasis Ward', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50108);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50108, 0, 1421, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50108);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 50116, 'Serene Idol', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50116);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50116, 0, 410144, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50116);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 50117, 'Dark Idol', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50117);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50117, 0, 401712, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50117);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 50118, 'Swift Idol', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50118);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50118, 0, 410145, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50118);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 50119, 'Shadow Effigy', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50119);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50119, 0, 411083, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50119);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 50120, 'Hexing Effigy', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50120);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50120, 0, 412985, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50120);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 50121, 'Cursed Effigy', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50121);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50121, 0, 427391, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50121);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 50122, 'Graven Effigy', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50122);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50122, 0, 25017, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50122);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 50129, 'Drakkari War Golem', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50129);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50129, 0, 26576, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50129);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 50217, 'Cleansing Idol', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50217);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50217, 0, 401712, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50217);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 50587, 'Serpent Ward', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50587);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 50587, 0, 16986, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 50587);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 51104, 'Sentry Ward', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 51104);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 51104, 0, 23229, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 51104);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 51105, 'Viper Ward', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 51105);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 51105, 0, 411030, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 51105);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 55117, 'Jungle Idol', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 55117);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 55117, 0, 401712, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 55117);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 216377, 'Polymorphed Frogduck', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 216377);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 216377, 0, 106677, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 216377);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 300659, 'Mimic Ward', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 300659);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 300659, 0, 411428, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 300659);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 300660, 'Fool''s Play', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 300660);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 300660, 0, 11686, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 300660);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 300661, 'Puppeteer''s Grasp', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 300661);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 300661, 0, 11686, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 300661);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 310659, 'Spirit of Bwonsamdi', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 310659);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 310659, 0, 75961, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 310659);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 506011, 'Cauldron Hidden Periodic', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 506011);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 506011, 0, 401941, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 506011);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 522106, 'Spirit Link Idol', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 522106);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 522106, 0, 410144, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 522106);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 554239, 'Spirit', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 554239);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 554239, 0, 100003, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 554239);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 759611, 'Spirit of Bwonsamdi', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 759611);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 759611, 0, 759611, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 759611);
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`, `ScriptName`)
SELECT 840000, 'Uncanny Likeness', 1, 1, 35, 1, 11, 2000, 2000, 'npc_ascension_witch_doctor'
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 840000);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT 840000, 0, 11686, 1, 1 WHERE NOT EXISTS (SELECT 1 FROM `creature_template_model` WHERE `CreatureID` = 840000);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 75961, 2.03128, 1.00000, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 75961);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 100003, 1.00000, 2.00000, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 100003);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 106677, 0.61111, 2.03128, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 106677);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 401712, 0.30000, 1.00000, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 401712);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 410144, 0.30000, 1.00000, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 410144);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 410145, 0.30000, 1.00000, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 410145);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 410147, 0.30000, 1.00000, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 410147);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 411030, 0.30000, 1.00000, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 411030);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 411083, 0.30000, 1.00000, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 411083);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 411428, 0.30000, 1.00000, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 411428);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 412985, 0.30000, 1.00000, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 412985);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 427391, 0.30000, 1.00000, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 427391);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`)
SELECT 759611, 2.03128, 1.00000, 2 WHERE NOT EXISTS (SELECT 1 FROM `creature_model_info` WHERE `DisplayID` = 759611);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `Data0`, `Data1`, `Data2`)
SELECT 9000003, 5, 1013678, 'Voodoo Cauldron', 1, 0, 0, 0
WHERE NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry` = 9000003);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `Data0`, `Data1`, `Data2`)
SELECT 129364, 22, 1013678, 'Mojo Cauldron', 1, 572413, 25, 1
WHERE NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry` = 129364);
DELETE FROM `spell_ranks` WHERE `spell_id` = 801674;
INSERT INTO `spell_ranks` (`first_spell_id`, `spell_id`, `rank`) VALUES (801674, 801674, 1);
DELETE FROM `spell_ranks` WHERE `spell_id` = 680908;
INSERT INTO `spell_ranks` (`first_spell_id`, `spell_id`, `rank`) VALUES (801674, 680908, 2);
DELETE FROM `spell_ranks` WHERE `spell_id` = 680909;
INSERT INTO `spell_ranks` (`first_spell_id`, `spell_id`, `rank`) VALUES (801674, 680909, 3);
DELETE FROM `spell_ranks` WHERE `spell_id` = 680910;
INSERT INTO `spell_ranks` (`first_spell_id`, `spell_id`, `rank`) VALUES (801674, 680910, 4);
DELETE FROM `spell_ranks` WHERE `spell_id` = 573265;
INSERT INTO `spell_ranks` (`first_spell_id`, `spell_id`, `rank`) VALUES (801674, 573265, 5);
DELETE FROM `spell_ranks` WHERE `spell_id` = 572364;
INSERT INTO `spell_ranks` (`first_spell_id`, `spell_id`, `rank`) VALUES (801674, 572364, 6);
DELETE FROM `spell_ranks` WHERE `spell_id` = 572365;
INSERT INTO `spell_ranks` (`first_spell_id`, `spell_id`, `rank`) VALUES (801674, 572365, 7);
DELETE FROM `spell_ranks` WHERE `spell_id` = 570240;
INSERT INTO `spell_ranks` (`first_spell_id`, `spell_id`, `rank`) VALUES (801674, 570240, 8);
DELETE FROM `spell_ranks` WHERE `spell_id` = 570241;
INSERT INTO `spell_ranks` (`first_spell_id`, `spell_id`, `rank`) VALUES (801674, 570241, 9);
DELETE FROM `spell_group` WHERE `id` = 2000184;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES (2000184, 63531), (2000184, 75593), (2000184, 731579), (2000184, 731869), (2000184, 1131579), (2000184, 1131869), (2000184, 1163531), (2000184, 1175593), (2000184, 1231579), (2000184, 560545);
DELETE FROM `spell_group_stack_rules` WHERE `group_id` = 2000184;
INSERT INTO `spell_group_stack_rules` (`group_id`, `stack_rule`) VALUES (2000184, 3);
