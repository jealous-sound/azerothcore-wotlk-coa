-- Bind Wildcard Mount (91944) to its new spell_ascension_wildcard_mount SpellScript.
START TRANSACTION;
DELETE FROM `spell_script_names` WHERE `spell_id` = 91944 AND `ScriptName` = 'spell_ascension_wildcard_mount';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (91944, 'spell_ascension_wildcard_mount');
COMMIT;
