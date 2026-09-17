-- Devotion of Khaz'goroth (560096): damage dealt with auto attacks reduces the cooldown of Libram spells by 0.5 sec.
-- Its proc 560097 names three exact spell ids (only rank 1 of Libram of Consecration, and 2 sec for Fervor and
-- Grace); the Templar ability script applies the first effect's 0.5 sec to every Libram and rank instead.
DELETE FROM `spell_script_names` WHERE `spell_id` = 560097 AND `ScriptName` = 'spell_ascension_templar_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(560097, 'spell_ascension_templar_ability');
