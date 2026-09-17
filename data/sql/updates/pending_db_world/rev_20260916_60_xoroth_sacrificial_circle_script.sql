-- Sacrificial Circle (805677) checks the living Hellfire Imps and sacrifices only the caster's own imps.
DELETE FROM `spell_script_names` WHERE `spell_id` = 805677;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(805677, 'spell_ascension_xoroth_sacrificial_circle');
