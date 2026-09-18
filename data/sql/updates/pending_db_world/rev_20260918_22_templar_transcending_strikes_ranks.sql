-- Transcending Strikes (#3810): the helper 804918 is a SPELL_EFFECT_ASCENSION_MODIFY_COOLDOWN whose misc value
-- names Divine Force rank 2 (807800). ModifyAscensionCooldown acts on that exact id, so a Templar who owns another
-- rank gets nothing. Bind the ability script, which applies the reduction to every owned rank of the named ability.
START TRANSACTION;
DELETE FROM `spell_script_names` WHERE `spell_id` = 804918 AND `ScriptName` = 'spell_ascension_templar_ability';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (804918, 'spell_ascension_templar_ability');
COMMIT;
