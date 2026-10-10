-- #6240: Portal: Blasted Lands 966370 (item 100576, Legion Pocket Portal) is two SPELL_EFFECT_TRANS_DOOR effects
-- placing gameobjects 100576 and 100575, and neither has a gameobject_template row, so
-- Spell::EffectTransmitted returns without placing anything.
--
-- Neither id is in the Exiles export (release exiles-db-export-2026-09-13, table `gameobject`), so the rows are
-- reconstructed from the stock pair that does the same job, 195141 and 195142 "Portal to Blasted Lands":
-- GAMEOBJECT_TYPE_SPELLCASTER, display 8948. Their use spells 65728 and 65729 carry a level 58 condition and teleport
-- to -11708.4, -3168, -5.07 on map 0; the client spell 966369 "Portal Effect: Blasted Lands" has the same
-- TELEPORT_UNITS to TARGET_DEST_DB shape without that condition, but no spell_target_position row, so it gets the
-- stock destination. 966370's description says it teleports group members who use it to the Dark Portal in Blasted
-- Lands, so both objects are party-only (Data2 = 1) and use 966369.
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`,
`name`, `size`, `Data0`, `Data1`, `Data2`) VALUES
(100575, 22, 8948, 'Portal to Blasted Lands', 1, 966369, 0, 1),
(100576, 22, 8948, 'Portal to Blasted Lands', 1, 966369, 0, 1)
ON DUPLICATE KEY UPDATE `type` = VALUES(`type`), `displayId` = VALUES(`displayId`), `name` = VALUES(`name`),
`size` = VALUES(`size`), `Data0` = VALUES(`Data0`), `Data1` = VALUES(`Data1`), `Data2` = VALUES(`Data2`);

DELETE FROM `spell_target_position` WHERE `ID` = 966369 AND `EffectIndex` = 0;
INSERT INTO `spell_target_position` (`ID`, `EffectIndex`, `MapID`, `PositionX`, `PositionY`, `PositionZ`, `Orientation`) VALUES
(966369, 0, 0, -11708.4, -3168, -5.07, 3.35103);
