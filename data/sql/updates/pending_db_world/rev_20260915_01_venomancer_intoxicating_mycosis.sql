START TRANSACTION;
DELETE FROM `spell_script_names` WHERE `spell_id` = 572150 AND `ScriptName` = 'spell_ascension_venomancer_intoxicating_mycosis';
DELETE FROM `spell_script_names` WHERE `spell_id` = 572151 AND `ScriptName` = 'spell_ascension_venomancer_intoxicating_mycosis';
DELETE FROM `spell_script_names` WHERE `spell_id` = 572153 AND `ScriptName` = 'spell_ascension_venomancer_intoxicating_mycosis';
DELETE FROM `spell_script_names` WHERE `spell_id` = 572154 AND `ScriptName` = 'spell_ascension_venomancer_intoxicating_mycosis';
DELETE FROM `spell_script_names` WHERE `spell_id` = 572155 AND `ScriptName` = 'spell_ascension_venomancer_intoxicating_mycosis';
DELETE FROM `spell_script_names` WHERE `spell_id` = 572156 AND `ScriptName` = 'spell_ascension_venomancer_intoxicating_mycosis';
DELETE FROM `spell_script_names` WHERE `spell_id` = 573355 AND `ScriptName` = 'spell_ascension_venomancer_intoxicating_mycosis';
DELETE FROM `spell_script_names` WHERE `spell_id` = 804986 AND `ScriptName` = 'spell_ascension_venomancer_intoxicating_mycosis';
DELETE FROM `spell_script_names` WHERE `spell_id` = 706454 AND `ScriptName` = 'spell_ascension_venomancer_intoxicating_extension';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(572150, 'spell_ascension_venomancer_intoxicating_mycosis'),
(572151, 'spell_ascension_venomancer_intoxicating_mycosis'),
(572153, 'spell_ascension_venomancer_intoxicating_mycosis'),
(572154, 'spell_ascension_venomancer_intoxicating_mycosis'),
(572155, 'spell_ascension_venomancer_intoxicating_mycosis'),
(572156, 'spell_ascension_venomancer_intoxicating_mycosis'),
(573355, 'spell_ascension_venomancer_intoxicating_mycosis'),
(804986, 'spell_ascension_venomancer_intoxicating_mycosis'),
(706454, 'spell_ascension_venomancer_intoxicating_extension');
COMMIT;
