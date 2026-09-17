-- CoA Achievement.dbc and Achievement_Criteria.dbc use IDs above 65535 (up to 322523 and 313488).
ALTER TABLE `character_achievement` MODIFY COLUMN `achievement` INT UNSIGNED NOT NULL;
ALTER TABLE `character_achievement_progress` MODIFY COLUMN `criteria` INT UNSIGNED NOT NULL;
