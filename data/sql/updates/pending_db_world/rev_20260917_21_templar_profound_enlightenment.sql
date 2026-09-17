-- Profound Enlightenment (680953) reduces the remaining cooldown of Testaments by 50%. Its three effects are the
-- client's "reduce remaining cooldown by X%" type 192, which has no core handler; the Templar ability script applies
-- them to every learned rank of the named Testament.
DELETE FROM `spell_script_names` WHERE `spell_id` = 680953 AND `ScriptName` = 'spell_ascension_templar_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(680953, 'spell_ascension_templar_ability');
