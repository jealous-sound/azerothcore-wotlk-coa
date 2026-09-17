-- Ascension's adventurer caches hand their reward straight to the bags. The live item cache reports
-- flags 67584 for the Satchel (1397884) and both Caches (1397885, 1397886); this world carried 67588,
-- the extra bit being ITEM_FLAG_HAS_LOOT (0x4), which makes the client treat them as lootable containers.
UPDATE `item_template` SET `Flags` = `Flags` & ~4 WHERE `entry` IN (1397884, 1397885, 1397886);
