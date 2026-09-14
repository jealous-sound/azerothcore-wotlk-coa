-- Soul Capture requires and consumes one nearby, previously unharvested corpse.
DELETE FROM `spell_script_names` WHERE `spell_id` = 561289 AND `ScriptName` = 'spell_ascension_soul_capture';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(561289, 'spell_ascension_soul_capture');
