-- mod-coa-challenges: make the failure history unique per (guid, challengeId).
--
-- The original table only had a non-unique KEY ix_guid, so a repeated fail path
-- (death + shared-fate/group-leave in the same tick, or a retried GM action)
-- could insert duplicate rows: SendFailureList would show the challenge twice
-- and the table would grow unbounded. The failure INSERT now uses INSERT IGNORE.
--
-- Dedupe is done through a rebuilt table (INSERT IGNORE keeps the first row per
-- key) so no data is lost when duplicates already exist. The ADD PRIMARY KEY is
-- guarded by a dynamic statement to stay safe if it was already applied.
--
-- Filename is the update key recorded in `updates` (state = MODULE); do not
-- rename once applied.

CREATE TABLE IF NOT EXISTS `coa_challenge_failure_new` LIKE `coa_challenge_failure`;

SET @coa_failure_pk := (SELECT COUNT(*) FROM INFORMATION_SCHEMA.STATISTICS
    WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'coa_challenge_failure_new'
    AND INDEX_NAME = 'PRIMARY');
SET @coa_failure_ddl := IF(@coa_failure_pk = 0,
    'ALTER TABLE `coa_challenge_failure_new` ADD PRIMARY KEY (`guid`, `challengeId`)',
    'DO 0');
PREPARE coa_failure_stmt FROM @coa_failure_ddl;
EXECUTE coa_failure_stmt;
DEALLOCATE PREPARE coa_failure_stmt;

INSERT IGNORE INTO `coa_challenge_failure_new` (`guid`, `challengeId`, `level`, `deaths`, `failTime`)
    SELECT `guid`, `challengeId`, `level`, `deaths`, `failTime` FROM `coa_challenge_failure`;

DROP TABLE `coa_challenge_failure`;
RENAME TABLE `coa_challenge_failure_new` TO `coa_challenge_failure`;
