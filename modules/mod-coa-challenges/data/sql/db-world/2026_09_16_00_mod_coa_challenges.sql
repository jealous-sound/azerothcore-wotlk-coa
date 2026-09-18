-- mod-coa-challenges: world-side (realm) challenge definitions.
--
-- Realm/map data (definitions, auras per level and rewards) belongs to the
-- world database, not the characters database. The module reads these tables
-- into an in-memory cache at startup and on `.coa challenges reload`; the world
-- DB is the single source of truth (there is no generated-.conf fallback).
--
-- Applied by the AzerothCore DB updater (module SQL). Filename is the update
-- key recorded in `updates` (state = MODULE); do not rename once applied.

CREATE TABLE IF NOT EXISTS `coa_challenge_definition` (
    `id` INT UNSIGNED NOT NULL,
    `name` VARCHAR(190) NOT NULL DEFAULT '',
    `icon` VARCHAR(190) NOT NULL DEFAULT '',
    `levelCount` INT UNSIGNED NOT NULL DEFAULT 1,
    `isTrial` TINYINT UNSIGNED NOT NULL DEFAULT 0,
    `isPrestige` TINYINT UNSIGNED NOT NULL DEFAULT 0,
    `exclusiveGroup` INT UNSIGNED NOT NULL DEFAULT 0,
    `requiredGameMode` INT UNSIGNED NOT NULL DEFAULT 0,
    `requiredGameEvent` INT UNSIGNED NOT NULL DEFAULT 0,
    `noRewards` TINYINT UNSIGNED NOT NULL DEFAULT 0,
    `noResurrect` TINYINT UNSIGNED NOT NULL DEFAULT 0,
    `lives` INT UNSIGNED NOT NULL DEFAULT 0,
    `sharedFate` TINYINT UNSIGNED NOT NULL DEFAULT 0,
    `survivalist` TINYINT UNSIGNED NOT NULL DEFAULT 0,
    `rules` TEXT,
    `conditions` TEXT,
    `objectives` TEXT,
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS `coa_challenge_spell` (
    `challengeId` INT UNSIGNED NOT NULL,
    `level` INT UNSIGNED NOT NULL DEFAULT 0,
    `pve` TEXT,
    `pvp` TEXT,
    PRIMARY KEY (`challengeId`, `level`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS `coa_challenge_reward` (
    `challengeId` INT UNSIGNED NOT NULL,
    `level` INT UNSIGNED NOT NULL DEFAULT 1,
    `itemId` INT UNSIGNED NOT NULL DEFAULT 0,
    `amount` INT UNSIGNED NOT NULL DEFAULT 1,
    `achievement` INT UNSIGNED NOT NULL DEFAULT 0,
    `isSpecial` TINYINT UNSIGNED NOT NULL DEFAULT 0,
    `isFirst` TINYINT UNSIGNED NOT NULL DEFAULT 1,
    PRIMARY KEY (`challengeId`, `level`, `itemId`, `achievement`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
