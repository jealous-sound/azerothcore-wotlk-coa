-- CoA Spiritual Recall: destroy the Witch Doctor's Wards, Idols and Effigies (module summons, not totems).
DELETE FROM `spell_script_names` WHERE `spell_id` = 583092 AND `ScriptName` = 'spell_ascension_witch_doctor_spiritual_recall';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(583092, 'spell_ascension_witch_doctor_spiritual_recall');
