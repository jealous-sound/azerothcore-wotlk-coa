--
-- Extend the existing local quest-reward container implementation to the Satchel and rare Cache.
UPDATE `item_template` SET `Flags` = `Flags` | 4, `ScriptName` = 'item_ascension_adventurer_cache'
WHERE `entry` IN (1397884, 1397886);

-- Preserve the established fallback pool and weights; the original private loot tables are unavailable.
DELETE FROM `item_loot_template` WHERE `Entry` IN (1397884, 1397886);
INSERT INTO `item_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`,
`MinCount`, `MaxCount`, `Comment`)
SELECT `rewards`.`Entry`, `loot`.`Item`, `loot`.`Reference`, `loot`.`Chance`, `loot`.`QuestRequired`,
`loot`.`LootMode`, `loot`.`GroupId`, `loot`.`MinCount`, `loot`.`MaxCount`, `loot`.`Comment`
FROM `item_loot_template` AS `loot`
CROSS JOIN (SELECT 1397884 AS `Entry` UNION ALL SELECT 1397886) AS `rewards`
WHERE `loot`.`Entry` = 1397885;
