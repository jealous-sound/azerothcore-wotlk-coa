-- Potion of Experience accumulates duration instead of refreshing to one hour.
DELETE FROM `spell_script_names` WHERE `spell_id` = 818046 AND `ScriptName` = 'spell_ascension_experience_potion';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(818046, 'spell_ascension_experience_potion');
