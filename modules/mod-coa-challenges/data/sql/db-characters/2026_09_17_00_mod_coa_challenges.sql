-- mod-coa-challenges: NO_NON_LOOTED_ITEMS ("Scavenger") tracking.
--
-- One row per item instance the character looted itself (item GUID), used to
-- gate equip/use when the challenge rule
-- CHALLENGE_RULES_TYPE_NO_NON_LOOTED_ITEMS is active. The in-memory set is
-- rebuilt at login while the rule is active; loot is always recorded so items
-- taken before activation still count.
--
-- Filename is the update key recorded in `updates` (state = MODULE); do not
-- rename once applied.

CREATE TABLE IF NOT EXISTS `coa_character_looted_item` (
    `guid` INT UNSIGNED NOT NULL,
    `itemGuid` INT UNSIGNED NOT NULL,
    PRIMARY KEY (`guid`, `itemGuid`),
    KEY `ix_guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
