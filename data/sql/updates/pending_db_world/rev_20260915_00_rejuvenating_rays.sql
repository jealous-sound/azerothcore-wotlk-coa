-- Rejuvenating Rays: the field's inert dummy must grant its authored stacking regeneration helper.
DELETE FROM `spell_script_names` WHERE `spell_id` = 807240 AND `ScriptName` = 'aura_ascension_sun_cleric_lifecycle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807240, 'aura_ascension_sun_cleric_lifecycle');
DELETE FROM `spell_script_names` WHERE `spell_id` = 807239 AND `ScriptName` = 'aura_ascension_rejuvenating_rays';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (807239, 'aura_ascension_rejuvenating_rays');
