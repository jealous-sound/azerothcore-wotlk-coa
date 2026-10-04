INSERT INTO `item_template`
(`entry`, `class`, `subclass`, `SoundOverrideSubclass`, `name`, `displayid`, `Quality`, `Flags`, `ItemLevel`, `RequiredLevel`, `stackable`, `description`, `ScriptName`, `spellid_1`, `spelltrigger_1`, `spellcharges_1`, `spellcategory_1`, `spellcategorycooldown_1`)
VALUES
(996200, 0, 8, -1, 'Empty Survival Flask', 14148, 1, 0, 1, 1, 20, 'Use while standing in water to fill. Drinking returns the empty flask. Crafted through Survivalist.', 'item_coa_survival_tool', 996128, 0, 0, 0, 0),
(996201, 0, 5, -1, 'Filled Survival Flask', 14154, 1, 0, 1, 1, 20, 'Restores hydration and returns the empty flask. Keep a spare bag slot.', '', 996132, 0, -1, 59, 1000),
(996202, 0, 8, -1, 'Rain Bucket', 18522, 1, 0, 1, 1, 20, 'Use outdoors in rain to collect drinkable water. Drinking returns the bucket. Crafted through Survivalist.', 'item_coa_survival_tool', 996129, 0, 0, 0, 0),
(996203, 0, 5, -1, 'Bucket of Rainwater', 18522, 1, 0, 1, 1, 20, 'Restores hydration and returns the empty bucket. Keep a spare bag slot.', '', 996133, 0, -1, 59, 1000),
(996204, 0, 8, -1, 'Field Repair Kit', 7842, 1, 0, 10, 10, 20, 'Repairs the equipped item missing the most durability. Consumed only on success. Use outside combat.', 'item_coa_survival_tool', 996130, 0, 0, 0, 0),
(996205, 0, 8, -1, 'Woodcarved Fishing Float', 12410, 1, 0, 1, 1, 20, 'Apply to a fishing pole: +25 Fishing for 10 minutes. Crafted through Survivalist.', '', 8087, 0, -1, 0, 0),
(996206, 0, 8, -1, 'Survival War Drum', 41050, 2, 8388672, 30, 30, 1, 'Rally nearby party members. 50 uses; shares a 2-minute cooldown with other drums.', '', 996134, 0, -50, 24, 120000),
(996207, 0, 8, -1, 'Survival Travel Drum', 41060, 2, 8388672, 40, 40, 1, 'Quicken nearby party members. 50 uses; shares a 2-minute cooldown with other drums.', '', 996135, 0, -50, 24, 120000),
(996208, 0, 8, -1, 'Personal Raft', 12410, 1, 0, 15, 15, 1, 'Face open water and deploy a stationary raft for 30 minutes. Consumed only on success. Crafted through Survivalist.', 'item_coa_survival_tool', 996131, 0, 0, 0, 0),
(996209, 2, 20, -1, 'Survival Fishing Pole', 16195, 1, 0, 1, 1, 1, 'A basic fishing pole. Train Fishing before using it. Crafted through Survivalist.', '', 0, 0, 0, 0, 0)
ON DUPLICATE KEY UPDATE `class` = VALUES(`class`), `subclass` = VALUES(`subclass`),
`SoundOverrideSubclass` = VALUES(`SoundOverrideSubclass`), `name` = VALUES(`name`),
`displayid` = VALUES(`displayid`), `Quality` = VALUES(`Quality`), `Flags` = VALUES(`Flags`),
`ItemLevel` = VALUES(`ItemLevel`), `RequiredLevel` = VALUES(`RequiredLevel`), `stackable` = VALUES(`stackable`),
`description` = VALUES(`description`), `ScriptName` = VALUES(`ScriptName`), `spellid_1` = VALUES(`spellid_1`),
`spelltrigger_1` = VALUES(`spelltrigger_1`), `spellcharges_1` = VALUES(`spellcharges_1`),
`spellcategory_1` = VALUES(`spellcategory_1`), `spellcategorycooldown_1` = VALUES(`spellcategorycooldown_1`);

UPDATE `item_template` SET `Material` = -1 WHERE `entry` BETWEEN 996200 AND 996209;

UPDATE `item_template` SET `InventoryType` = 17, `RequiredSkill` = 356, `RequiredSkillRank` = 1,
`Material` = -1, `delay` = 3000, `dmg_min1` = 2, `dmg_max1` = 4 WHERE `entry` = 996209;

INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`)
VALUES (996200, 5, 124, 'Personal Survival Raft', 1)
ON DUPLICATE KEY UPDATE `type` = VALUES(`type`), `displayId` = VALUES(`displayId`),
`name` = VALUES(`name`), `size` = VALUES(`size`);
