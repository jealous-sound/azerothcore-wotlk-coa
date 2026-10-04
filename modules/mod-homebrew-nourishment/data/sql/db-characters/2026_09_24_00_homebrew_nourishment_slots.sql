-- Expand persistent Homebrew Nourishment from one effect to three independent meal slots.
ALTER TABLE `mod_homebrew_nourishment_active`
  DROP PRIMARY KEY,
  ADD COLUMN `slot` TINYINT UNSIGNED NOT NULL DEFAULT 1 AFTER `guid`,
  ADD PRIMARY KEY (`guid`, `slot`),
  DROP INDEX `idx_expires_at`,
  ADD KEY `idx_expires_at` (`expires_at`);
