-- Absolution (#3181): 800424 only taunts. The movement speed its description promises lives in 520659, which
-- nothing casts, so bind the ability script and let it cast that buff on the Templar after the taunt.
START TRANSACTION;
DELETE FROM `spell_script_names` WHERE `spell_id` = 800424 AND `ScriptName` = 'spell_ascension_templar_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (800424, 'spell_ascension_templar_ability');
COMMIT;
