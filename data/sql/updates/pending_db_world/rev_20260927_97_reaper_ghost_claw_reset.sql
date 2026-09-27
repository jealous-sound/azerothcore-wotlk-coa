-- Ghost Claw 803985 resets its cooldown when the enemy carrying it dies.
DELETE FROM `spell_script_names` WHERE `spell_id` = 803985 AND `ScriptName` = 'aura_ascension_reaper_ghost_claw';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(803985, 'aura_ascension_reaper_ghost_claw');
