DELETE FROM `spell_script_names` WHERE `ScriptName` = 'aura_ascension_resource_talent';
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'aura_ascension_resource_talent_refresh';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(301287, 'aura_ascension_resource_talent'),
(504252, 'aura_ascension_resource_talent'),
(800058, 'aura_ascension_resource_talent_refresh'),
(706613, 'aura_ascension_resource_talent_refresh');
