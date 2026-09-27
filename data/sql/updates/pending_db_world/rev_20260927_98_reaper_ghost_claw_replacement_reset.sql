-- Talent 504269 replaces Ghost Claw 803985 with 807234 (AscensionTalentReplacementData.h), so the kill reset
-- from rev_20260927_97 also has to run on the replacement spell.
DELETE FROM `spell_script_names` WHERE `spell_id` = 807234 AND `ScriptName` = 'aura_ascension_reaper_ghost_claw';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(807234, 'aura_ascension_reaper_ghost_claw');
