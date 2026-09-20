-- Resurrect in Closest Town / Capital City: the two spells the client's death dialog casts.
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_ascension_closest_resurrection';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(84423, 'spell_ascension_closest_resurrection'),
(84433, 'spell_ascension_closest_resurrection');
