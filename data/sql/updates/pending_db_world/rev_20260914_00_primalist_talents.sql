-- Throat Clamp commands the owner's pet to use the existing dash/interrupt helper.
DELETE FROM `spell_script_names` WHERE `spell_id` = 500764 AND `ScriptName` = 'spell_ascension_throat_clamp';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(500764, 'spell_ascension_throat_clamp');
