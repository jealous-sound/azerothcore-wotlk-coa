--
-- Preserve legacy class-quest rewards for ordinary classes, but suppress their spell grants on custom classes.
SET @LegacyQuestScript := 'spell_ascension_legacy_quest_reward';
DELETE FROM `spell_script_names` WHERE `spell_id` IN (8073, 8121) AND `ScriptName` = @LegacyQuestScript;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(8073, @LegacyQuestScript),
(8121, @LegacyQuestScript);
