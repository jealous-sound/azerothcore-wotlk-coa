-- Unclaimed Manastorm caches are real item instances, persisted with their floor reward.
-- Delivery moves the existing item into inventory or merges its stack in one transaction.
CREATE TABLE IF NOT EXISTS `ascension_manastorm_cache` (
  `item` int unsigned NOT NULL,
  `guid` int unsigned NOT NULL,
  PRIMARY KEY (`item`),
  KEY `idx_guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
