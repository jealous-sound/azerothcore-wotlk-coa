-- All current Precision Shot ranks and the active Knockout command.
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_ascension_ranger_light_arrows'
AND `spell_id` IN (500075, 572108, 572109, 572110, 572111, 572112, 572113);
DELETE FROM `spell_script_names` WHERE `spell_id` = 801435 AND `ScriptName` = 'spell_ascension_ranger_knockout';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(500075, 'spell_ascension_ranger_light_arrows'),
(572108, 'spell_ascension_ranger_light_arrows'),
(572109, 'spell_ascension_ranger_light_arrows'),
(572110, 'spell_ascension_ranger_light_arrows'),
(572111, 'spell_ascension_ranger_light_arrows'),
(572112, 'spell_ascension_ranger_light_arrows'),
(572113, 'spell_ascension_ranger_light_arrows'),
(801435, 'spell_ascension_ranger_knockout');
