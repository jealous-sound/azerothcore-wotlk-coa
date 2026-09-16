-- #122 follow-up to the `currencytypes_dbc` row for item 375250 "Rune of Ascension".
-- Restoring that row only helps characters that hold no runes yet. Player::CanStoreItem runs its
-- "search stack for merge to" pass (PlayerStorage.cpp) before the currency-slot pass, and that merge pass
-- covers INVENTORY_SLOT_ITEM_START..INVENTORY_SLOT_ITEM_END and the equipped bags. A character that already
-- has a non-full rune stack in a bag therefore keeps receiving new runes into that bag stack forever:
-- Player::_StoreItem takes its pItem2 branch, never reaches the currency slots, never calls
-- Player::AddKnownCurrency, and PLAYER_FIELD_KNOWN_CURRENCIES / `characters`.`knownCurrencies` stay 0.
-- The Character -> Currency page then stays blank for exactly the players who reported #122 and #199.
-- This migration puts those pre-existing stacks where the core would have put them all along.
-- Scope and safety:
--   * Only `character_inventory`.`bag` / `.`slot`` are rewritten. No row is inserted or deleted, no stack is
--     merged or split, and `item_instance` is untouched, so no item can be lost or duplicated here.
--   * Backpack slots 23..38 and the four equipped bag containers (19..22) are relocated. Bank slots 39..66
--     and bank bags 67..73 are deliberately left alone: those load fine either way, and silently emptying a
--     player's bank is worse than a currency tab that only counts what is carried.
--   * Each stack is moved into a currency slot (118..149) that is free for that character, so the
--     `guid`/`bag`/`slot` unique key cannot collide with a stock currency token already parked there.
--     A character with more relocatable stacks than free currency slots keeps the surplus in its bags.
--   * Apply this together with the world migration that adds the `currencytypes_dbc` row. Without that row
--     ItemTemplate::IsCurrencyToken() is false, Player::CanStoreItem_InSpecificSlot rejects slots 118..149 at
--     login, and _LoadInventory mails the runes back to the player instead of loading them. Both migrations
--     run through the updater before any character can log in, so applying the pair is safe; reverting only
--     the world row later is what triggers the mail-back path.
--   * Apply through the normal server updater while the realm is down. Rewriting the rows under an online
--     character only gets overwritten by that character's next save.
DROP TEMPORARY TABLE IF EXISTS `_rune_of_ascension_move_v1`;
CREATE TEMPORARY TABLE `_rune_of_ascension_move_v1` (
    `item` INT UNSIGNED NOT NULL PRIMARY KEY,
    `guid` INT UNSIGNED NOT NULL,
    `seq` INT UNSIGNED NOT NULL
) ENGINE=InnoDB;
DELETE FROM `_rune_of_ascension_move_v1`;
INSERT INTO `_rune_of_ascension_move_v1` (`item`, `guid`, `seq`)
SELECT
    `ci`.`item`,
    `ci`.`guid`,
    ROW_NUMBER() OVER (PARTITION BY `ci`.`guid` ORDER BY `ii`.`count` DESC, `ci`.`bag`, `ci`.`slot`)
FROM `character_inventory` `ci`
INNER JOIN `item_instance` `ii`
    ON `ii`.`guid` = `ci`.`item`
LEFT JOIN `character_inventory` `holder`
    ON `holder`.`item` = `ci`.`bag`
WHERE `ii`.`itemEntry` = 375250
    AND (
        (`ci`.`bag` = 0 AND `ci`.`slot` BETWEEN 23 AND 38)
        OR (`ci`.`bag` <> 0 AND `holder`.`bag` = 0 AND `holder`.`slot` BETWEEN 19 AND 22)
    );
-- CURRENCYTOKEN_SLOT_START..CURRENCYTOKEN_SLOT_END, spelled out rather than generated.
DROP TEMPORARY TABLE IF EXISTS `_rune_of_ascension_free_v1`;
CREATE TEMPORARY TABLE `_rune_of_ascension_free_v1` (
    `slot` TINYINT UNSIGNED NOT NULL PRIMARY KEY
) ENGINE=InnoDB;
DELETE FROM `_rune_of_ascension_free_v1`;
INSERT INTO `_rune_of_ascension_free_v1` (`slot`) VALUES
(118),(119),(120),(121),(122),(123),(124),(125),(126),(127),(128),(129),(130),(131),(132),(133),
(134),(135),(136),(137),(138),(139),(140),(141),(142),(143),(144),(145),(146),(147),(148),(149);
-- Number the currency slots that are still free for each affected character, lowest slot first.
DROP TEMPORARY TABLE IF EXISTS `_rune_of_ascension_slot_v1`;
CREATE TEMPORARY TABLE `_rune_of_ascension_slot_v1` (
    `guid` INT UNSIGNED NOT NULL,
    `slot` TINYINT UNSIGNED NOT NULL,
    `seq` INT UNSIGNED NOT NULL,
    PRIMARY KEY (`guid`, `seq`)
) ENGINE=InnoDB;
DELETE FROM `_rune_of_ascension_slot_v1`;
INSERT INTO `_rune_of_ascension_slot_v1` (`guid`, `slot`, `seq`)
SELECT
    `owner`.`guid`,
    `free`.`slot`,
    ROW_NUMBER() OVER (PARTITION BY `owner`.`guid` ORDER BY `free`.`slot`)
FROM (
    SELECT DISTINCT `guid`
    FROM `_rune_of_ascension_move_v1`
) `owner`
CROSS JOIN `_rune_of_ascension_free_v1` `free`
LEFT JOIN `character_inventory` `taken`
    ON `taken`.`guid` = `owner`.`guid`
    AND `taken`.`bag` = 0
    AND `taken`.`slot` = `free`.`slot`
WHERE `taken`.`item` IS NULL;
-- Move each stack into the matching free currency slot. Surplus stacks find no row here and stay put.
UPDATE `character_inventory` `ci`
INNER JOIN `_rune_of_ascension_move_v1` `m`
    ON `m`.`item` = `ci`.`item`
INNER JOIN `_rune_of_ascension_slot_v1` `s`
    ON `s`.`guid` = `m`.`guid`
    AND `s`.`seq` = `m`.`seq`
SET `ci`.`bag` = 0, `ci`.`slot` = `s`.`slot`;
-- Light the rune's bit (CurrencyTypes BitIndex 35, so 1 << 34) for every character that now carries one in a
-- currency slot. Player::AddKnownCurrency would also do this on the next login, but setting it here keeps the
-- stored state consistent with the inventory rows and makes the migration verifiable without logging in.
UPDATE `characters` `c`
INNER JOIN (
    SELECT DISTINCT `ci`.`guid` AS `guid`
    FROM `character_inventory` `ci`
    INNER JOIN `item_instance` `ii`
        ON `ii`.`guid` = `ci`.`item`
    WHERE `ii`.`itemEntry` = 375250
        AND `ci`.`bag` = 0
        AND `ci`.`slot` BETWEEN 118 AND 149
) `carrier`
    ON `carrier`.`guid` = `c`.`guid`
SET `c`.`knownCurrencies` = `c`.`knownCurrencies` | (1 << 34);
DROP TEMPORARY TABLE `_rune_of_ascension_slot_v1`;
DROP TEMPORARY TABLE `_rune_of_ascension_free_v1`;
DROP TEMPORARY TABLE `_rune_of_ascension_move_v1`;
