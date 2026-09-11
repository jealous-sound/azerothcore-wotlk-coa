-- Gatling consumes its selected finite modifiers after the owner-side channel ends.
DELETE FROM `spell_script_names` WHERE `spell_id` = 500213 AND `ScriptName` = 'aura_ascension_tinker_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (500213, 'aura_ascension_tinker_lifecycle');
