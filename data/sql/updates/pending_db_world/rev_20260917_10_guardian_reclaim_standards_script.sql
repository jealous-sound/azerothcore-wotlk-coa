-- Reclaim Standards (574339) is the Guardian's own button; only its hidden twin 801504 had the reclaim script.
DELETE FROM `spell_script_names` WHERE `spell_id` = 574339 AND `ScriptName` = 'spell_ascension_guardian_reclaim';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(574339, 'spell_ascension_guardian_reclaim');
