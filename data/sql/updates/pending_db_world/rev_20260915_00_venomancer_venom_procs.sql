START TRANSACTION;
DELETE FROM `spell_script_names` WHERE `spell_id` = 630868 AND `ScriptName` = 'aura_ascension_venomancer_venom_proc';
DELETE FROM `spell_script_names` WHERE `spell_id` = 805731 AND `ScriptName` = 'aura_ascension_venomancer_venom_proc';
DELETE FROM `spell_script_names` WHERE `spell_id` = 805775 AND `ScriptName` = 'aura_ascension_venomancer_venom_proc';
DELETE FROM `spell_script_names` WHERE `spell_id` = 805776 AND `ScriptName` = 'aura_ascension_venomancer_venom_proc';
DELETE FROM `spell_script_names` WHERE `spell_id` = 805777 AND `ScriptName` = 'aura_ascension_venomancer_venom_proc';
DELETE FROM `spell_script_names` WHERE `spell_id` = 805778 AND `ScriptName` = 'aura_ascension_venomancer_venom_proc';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(630868, 'aura_ascension_venomancer_venom_proc'),
(805731, 'aura_ascension_venomancer_venom_proc'),
(805775, 'aura_ascension_venomancer_venom_proc'),
(805776, 'aura_ascension_venomancer_venom_proc'),
(805777, 'aura_ascension_venomancer_venom_proc'),
(805778, 'aura_ascension_venomancer_venom_proc');
COMMIT;
