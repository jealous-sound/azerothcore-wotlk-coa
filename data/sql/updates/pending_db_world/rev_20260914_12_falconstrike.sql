-- #88: Falconstrike is usable only during its talent's temporary replacement window.
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_ascension_ranger_falconstrike';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(806345, 'spell_ascension_ranger_falconstrike'),
(806437, 'spell_ascension_ranger_falconstrike'),
(806438, 'spell_ascension_ranger_falconstrike'),
(806439, 'spell_ascension_ranger_falconstrike'),
(806440, 'spell_ascension_ranger_falconstrike'),
(806441, 'spell_ascension_ranger_falconstrike'),
(806442, 'spell_ascension_ranger_falconstrike'),
(806443, 'spell_ascension_ranger_falconstrike');
