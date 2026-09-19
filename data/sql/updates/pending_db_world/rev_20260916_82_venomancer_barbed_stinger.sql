-- Barbed Stinger (803196): the embedded stinger 803206 runs its once-per-second 680854 growth tick in
-- aura_ascension_venomancer_lifecycle, which was never bound to it.
DELETE FROM `spell_script_names` WHERE `spell_id` = 803206 AND `ScriptName` = 'aura_ascension_venomancer_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (803206, 'aura_ascension_venomancer_lifecycle');
