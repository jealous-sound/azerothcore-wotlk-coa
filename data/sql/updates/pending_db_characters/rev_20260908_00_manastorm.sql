-- Manastorm first-clear journal. Reward mail is inserted in the same transaction.
CREATE TABLE IF NOT EXISTS `ascension_manastorm_clear` (
  `guid` int unsigned NOT NULL,
  `mode` tinyint unsigned NOT NULL,
  `depth` int unsigned NOT NULL,
  `scene` int unsigned NOT NULL,
  `mail_id` int unsigned NOT NULL,
  `completed_at` int unsigned NOT NULL,
  PRIMARY KEY (`guid`, `mode`, `depth`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
