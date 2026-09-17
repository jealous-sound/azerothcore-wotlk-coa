-- ----------------------------------------------------------------------------
-- Worldforged pickup ledger (companion to worldforged-pickups.sql)
-- ----------------------------------------------------------------------------
-- One row per (character, pickup spawn) the character has already looted. The pickup
-- itself stays in the world for everyone else; this is what makes it inert for the
-- character who took its item, permanently, across restarts.
--
-- Keyed on the spawn id (`gameobject`.`guid`), never on the runtime object GUID: this
-- core hands out map-local generated GUIDs, so a runtime GUID is not the database row
-- and is not stable across grid reloads. The spawn ids used by this layer are the
-- fixed block 6900001+ written by worldforged-pickups.sql.
--
-- Apply to acore_characters. The module `mod-worldforged-pickups` reads and writes it;
-- `Updates.EnableDatabases = 0` in this repack, so apply it by hand.
-- ----------------------------------------------------------------------------

CREATE TABLE IF NOT EXISTS `character_worldforged_loot` (
  `guid`      int unsigned NOT NULL COMMENT 'character guid',
  `spawn_id`  int unsigned NOT NULL COMMENT 'gameobject.guid of the pickup',
  `entry`     int unsigned NOT NULL DEFAULT 0 COMMENT 'pickup template entry',
  `looted_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`guid`,`spawn_id`),
  KEY `idx_spawn` (`spawn_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='Worldforged pickups already looted, per character';

-- The first pass used a differently shaped table for the same idea.
DROP TABLE IF EXISTS `character_worldforged_cache`;
