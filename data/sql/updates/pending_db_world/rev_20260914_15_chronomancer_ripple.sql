DELETE FROM `spell_script_names` WHERE `ScriptName` = 'aura_ascension_ripple_aeon';
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'aura_ascension_ripple_pulses';
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'aura_ascension_ripple_protection';
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'aura_ascension_ripple_resilience';
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'aura_ascension_ripple_debt';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(806296, 'aura_ascension_ripple_aeon'),
(560384, 'aura_ascension_ripple_pulses'),
(560388, 'aura_ascension_ripple_pulses'),
(560394, 'aura_ascension_ripple_protection'),
(560396, 'aura_ascension_ripple_resilience'),
(806733, 'aura_ascension_ripple_debt');

DELETE FROM `spell_bonus_data` WHERE `entry` IN (560385, 560387, 503826);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(560385, 0.125, 0, 0, 0, 'Eternity Warper - Rippling Renewal pulse'),
(560387, 0.385, 0, 0, 0, 'Eternity Warper - Rippling Oblivion pulse'),
(503826, 0.25, 0, 0, 0, 'End of Time - Anomaly Spike');
