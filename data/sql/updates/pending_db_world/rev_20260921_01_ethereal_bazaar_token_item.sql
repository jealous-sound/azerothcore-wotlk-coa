-- Ethereal Bazaar: create item_template entry for Bazaar Token (975001)
--
-- The client DBCs (Item.dbc, ItemExtendedCost.dbc) already define item 975001
-- with displayId 30015 (Spell_Shadow_Teleport), class 0 (Consumable), subclass 8 (Other).
-- However, item_template was missing an entry for 975001. When a player receives
-- tokens (from creature kills or quests), CanStoreNewItem/Item::CreateItem failed
-- because sObjectMgr->GetItemTemplate(975001) returned nullptr, causing an ABORT()
-- assertion crash in Item::CreateItem.
--
-- Values match Item.dbc:
--   entry: 975001
--   class: 0 (ITEM_CLASS_CONSUMABLE)
--   subclass: 8 (ITEM_SUBCLASS_CONSUMABLE_OTHER)
--   SoundOverrideSubclass: -1
--   displayid: 30015
--   InventoryType: 0 (non-equippable)
--   Material: -1
--   Quality: 4 (Epic)
--   stackable: 5000 (allows holding tokens for high-cost bazaar vanity items)
--   bonding: 0 (tradeable / auctionable as noted in Tiraxis gossip text)

DELETE FROM `item_template` WHERE `entry` = 975001;
INSERT INTO `item_template`
 (`entry`, `class`, `subclass`, `SoundOverrideSubclass`, `name`, `displayid`, `Quality`, `Flags`, `BuyCount`,
  `BuyPrice`, `SellPrice`, `InventoryType`, `AllowableClass`, `AllowableRace`, `ItemLevel`, `RequiredLevel`,
  `maxcount`, `stackable`, `bonding`, `description`, `Material`, `BagFamily`)
VALUES
 (975001, 0, 8, -1, 'Bazaar Token', 30015, 4, 0, 1,
  0, 0, 0, -1, -1, 1, 0,
  0, 5000, 0, 'Used to purchase exotic goods and vanity items from Tiraxis at the Ethereal Bazaar.', -1, 0);
