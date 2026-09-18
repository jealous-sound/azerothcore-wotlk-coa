-- mod-coa-challenges: character-side schema (authoritative).
--
-- Applied by the AzerothCore DB updater (module SQL, derived from
-- AC_MODULES_LIST). The module also keeps an idempotent runtime fallback
-- (CoAChallenges.AutoCreateSchema) for development; this file is the source
-- of truth and the migration that fixes existing installs.
--
-- Filename is the update key recorded in `updates` (state = MODULE); do not
-- rename once applied.

CREATE TABLE IF NOT EXISTS `coa_character_challenge` (
    `guid` INT UNSIGNED NOT NULL,
    `challengeId` INT UNSIGNED NOT NULL,
    `level` INT UNSIGNED NOT NULL DEFAULT 1,
    `deaths` INT UNSIGNED NOT NULL DEFAULT 0,
    `hunger` INT NOT NULL DEFAULT 100,
    `thirst` INT NOT NULL DEFAULT 100,
    `startTime` INT UNSIGNED NOT NULL DEFAULT 0,
    PRIMARY KEY (`guid`, `challengeId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS `coa_challenge_failure` (
    `guid` INT UNSIGNED NOT NULL,
    `challengeId` INT UNSIGNED NOT NULL,
    `level` INT UNSIGNED NOT NULL DEFAULT 1,
    `deaths` INT UNSIGNED NOT NULL DEFAULT 0,
    `failTime` INT UNSIGNED NOT NULL DEFAULT 0,
    KEY `ix_guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS `coa_character_condition` (
    `guid` INT UNSIGNED NOT NULL,
    `flag` VARCHAR(64) NOT NULL,
    PRIMARY KEY (`guid`, `flag`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS `coa_character_objective` (
    `guid` INT UNSIGNED NOT NULL,
    `challengeId` INT UNSIGNED NOT NULL,
    `objective` VARCHAR(96) NOT NULL,
    PRIMARY KEY (`guid`, `challengeId`, `objective`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS `coa_challenge_completion` (
    `guid` INT UNSIGNED NOT NULL,
    `challengeId` INT UNSIGNED NOT NULL,
    `level` INT UNSIGNED NOT NULL DEFAULT 1,
    `completeTime` INT UNSIGNED NOT NULL DEFAULT 0,
    `startTime` INT UNSIGNED NOT NULL DEFAULT 0,
    PRIMARY KEY (`guid`, `challengeId`, `level`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS `coa_custom_trial` (
    `guid` INT UNSIGNED NOT NULL,
    `trialId` VARCHAR(64) NOT NULL,
    `title` VARCHAR(128) NOT NULL,
    `about` VARCHAR(1024) NOT NULL,
    `icon` VARCHAR(256) NOT NULL,
    `author` VARCHAR(64) NOT NULL DEFAULT '',
    PRIMARY KEY (`guid`, `trialId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS `coa_custom_trial_entry` (
    `guid` INT UNSIGNED NOT NULL,
    `trialId` VARCHAR(64) NOT NULL,
    `challengeId` INT UNSIGNED NOT NULL,
    `level` INT UNSIGNED NOT NULL DEFAULT 1,
    `description` VARCHAR(512) NOT NULL,
    PRIMARY KEY (`guid`, `trialId`, `challengeId`, `level`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS `coa_custom_trial_vote` (
    `guid` INT UNSIGNED NOT NULL,
    `trialId` VARCHAR(64) NOT NULL,
    `upvote` TINYINT UNSIGNED NOT NULL DEFAULT 0,
    `downvote` TINYINT UNSIGNED NOT NULL DEFAULT 0,
    PRIMARY KEY (`guid`, `trialId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS `coa_custom_trial_active` (
    `guid` INT UNSIGNED NOT NULL,
    `trialId` VARCHAR(64) NOT NULL,
    `startTime` INT UNSIGNED NOT NULL DEFAULT 0,
    PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS `coa_custom_trial_completion` (
    `guid` INT UNSIGNED NOT NULL,
    `trialId` VARCHAR(64) NOT NULL,
    `startTime` INT UNSIGNED NOT NULL DEFAULT 0,
    `completeTime` INT UNSIGNED NOT NULL DEFAULT 0,
    PRIMARY KEY (`trialId`, `guid`),
    KEY `ix_trial` (`trialId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS `coa_character_gamemode` (
    `guid` INT UNSIGNED NOT NULL,
    `gameMode` INT UNSIGNED NOT NULL DEFAULT 0,
    PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS `coa_character_survival` (
    `guid` INT UNSIGNED NOT NULL,
    `hunger` INT NOT NULL DEFAULT 0,
    `thirst` INT NOT NULL DEFAULT 0,
    PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS `coa_character_fatigue` (
    `guid` INT UNSIGNED NOT NULL,
    `challengeId` INT UNSIGNED NOT NULL,
    `fatigue` INT NOT NULL DEFAULT 0,
    PRIMARY KEY (`guid`, `challengeId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS `coa_character_gamemode_lives` (
    `guid` INT UNSIGNED NOT NULL,
    `gameMode` INT UNSIGNED NOT NULL,
    `deaths` INT NOT NULL DEFAULT 0,
    PRIMARY KEY (`guid`, `gameMode`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Custom achievement ids exceed the core's SMALLINT columns (max 65535);
-- granting one would otherwise fail on save (MySQL errno 1264). Widen the
-- core tracking columns (idempotent MODIFY).
ALTER TABLE `character_achievement` MODIFY `achievement` INT UNSIGNED NOT NULL;
ALTER TABLE `character_achievement_progress` MODIFY `criteria` INT UNSIGNED NOT NULL;
