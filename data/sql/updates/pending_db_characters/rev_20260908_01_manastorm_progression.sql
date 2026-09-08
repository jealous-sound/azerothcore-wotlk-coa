-- Durable bonus-cache probability and the four native loadout slots.
CREATE TABLE IF NOT EXISTS `ascension_manastorm_bonus` (
  `guid` int unsigned NOT NULL,
  `mode` tinyint unsigned NOT NULL,
  `pity` int unsigned NOT NULL DEFAULT 0,
  `caches` int unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`, `mode`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- A committed clear can survive a disconnect before the native XP/level save.
CREATE TABLE IF NOT EXISTS `ascension_manastorm_xp` (
  `guid` int unsigned NOT NULL,
  `amount` bigint unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE IF NOT EXISTS `ascension_manastorm_loadout` (
  `guid` int unsigned NOT NULL,
  `slot` tinyint unsigned NOT NULL,
  `spell` int unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`, `slot`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
