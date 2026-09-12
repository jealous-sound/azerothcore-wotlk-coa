-- Correct two `item_template` rows that were left as auto-generated bootstrap
-- placeholders (name "Ascension Appearance <entry>", Quality 4/Epic, all
-- stat/damage/armor fields zeroed) instead of the real captured item data.
--
-- Source: recovered client itemcache (SMSG_ITEM_QUERY_SINGLE_RESPONSE),
-- cross-mode union, 122-127 corroborating sources per entry, captured 2026-09-10.
--
-- Scoped to only the two entries actually observed in-game so far; the wider
-- placeholder corpus (143k+ other entries with real cache data available) is
-- intentionally left untouched pending a separate decision.

UPDATE `item_template` SET
    `class` = 2, `subclass` = 15, `SoundOverrideSubclass` = -1,
    `name` = 'Decayed Dagger', `displayid` = 167100, `Quality` = 1,
    `Flags` = 0, `FlagsExtra` = 0, `BuyPrice` = 2, `SellPrice` = 1,
    `InventoryType` = 13, `AllowableClass` = -1, `AllowableRace` = -1,
    `ItemLevel` = 3, `RequiredLevel` = 1, `stackable` = 1,
    `dmg_min1` = 1, `dmg_max1` = 4, `dmg_type1` = 0, `delay` = 1600,
    `bonding` = 0, `Material` = 1, `sheath` = 3, `MaxDurability` = 18
WHERE `entry` = 484319;

UPDATE `item_template` SET
    `class` = 4, `subclass` = 2, `SoundOverrideSubclass` = -1,
    `name` = 'Noxious Kilt', `displayid` = 143973, `Quality` = 0,
    `Flags` = 0, `FlagsExtra` = 0, `BuyPrice` = 5, `SellPrice` = 1,
    `InventoryType` = 7, `AllowableClass` = -1, `AllowableRace` = -1,
    `ItemLevel` = 1, `RequiredLevel` = 1, `stackable` = 1,
    `armor` = 6, `bonding` = 0, `Material` = 8, `MaxDurability` = 35
WHERE `entry` = 967757;
