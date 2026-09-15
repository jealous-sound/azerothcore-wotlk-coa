-- Allow friendly players to buff the wards, including Berserking's owner-cast haste helper.
UPDATE `creature_template` SET `type_flags` = `type_flags` | 4096 WHERE `entry` IN (50105, 50587, 51105);
