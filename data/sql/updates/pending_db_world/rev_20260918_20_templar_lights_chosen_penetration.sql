-- Light's Chosen (#1816): the talent's third effect is a 5-second periodic trigger of 301369, whose own effect
-- carries a flat point of spell penetration. The talent's "10% of your Intellect" never reaches the helper, so bind
-- the helper to the Templar lifecycle script, which calculates the amount from the talent's own effect value.
START TRANSACTION;
DELETE FROM `spell_script_names` WHERE `spell_id` = 301369 AND `ScriptName` = 'aura_ascension_templar_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (301369, 'aura_ascension_templar_lifecycle');
COMMIT;
