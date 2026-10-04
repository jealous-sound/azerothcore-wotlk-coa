INSERT INTO `item_template`
(`entry`, `class`, `subclass`, `SoundOverrideSubclass`, `name`, `displayid`, `Quality`, `Flags`,
 `ItemLevel`, `RequiredLevel`, `stackable`, `description`, `ScriptName`, `spellid_1`, `spelltrigger_1`,
 `spellcharges_1`, `spellcategory_1`, `spellcategorycooldown_1`)
VALUES
(996210, 0, 8, -1, 'Barber Scissors', 21102, 2, 0, 1, 1, 1,
 'Reusable scissors. Set up a temporary barber chair outdoors outside combat and instances.',
 'item_coa_survival_tool', 996137, 0, 0, 0, 0)
ON DUPLICATE KEY UPDATE `name` = VALUES(`name`), `displayid` = VALUES(`displayid`),
 `description` = VALUES(`description`), `ScriptName` = VALUES(`ScriptName`), `spellid_1` = VALUES(`spellid_1`);

UPDATE `item_template` SET `RequiredLevel` = 1 WHERE `entry` BETWEEN 996200 AND 996209;
