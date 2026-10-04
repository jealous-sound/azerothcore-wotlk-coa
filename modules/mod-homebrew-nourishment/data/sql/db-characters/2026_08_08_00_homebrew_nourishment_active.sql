-- Homebrew Nourishment 1.0.0 persistent character state.
CREATE TABLE IF NOT EXISTS `mod_homebrew_nourishment_active` (
  `guid` INT UNSIGNED NOT NULL,
  `item_id` INT UNSIGNED NOT NULL,
  `expires_at` BIGINT UNSIGNED NOT NULL,
  PRIMARY KEY (`guid`),
  KEY `idx_expires_at` (`expires_at`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
