-- MAJOR FINDING: the 2300xxx entry ID range itself blocks the client-side
-- cast bar/lock-interaction UI for GAMEOBJECT_TYPE_GOOBER objects on this
-- server, independent of every field in gameobject_template (confirmed by
-- spawning a byte-for-byte clone of Lost Page I under entry 9900731 right
-- next to the original -- the clone showed a working cast bar, the
-- original didn't). Likely tied to how this Ascension client's DBC/WXL
-- extension recognizes entry ranges for interactive objects. The archive/
-- 2300xxx entry family (used throughout this session for Lost Pages,
-- Accursed Sisterhood items, Ritual Portals, Kobold Warren investigation
-- items, etc. -- see goldshire_northshire_restoration memory) should be
-- treated as suspect for this specific problem going forward; prefer a
-- high custom range like 9900xxx (already used for Stolen Supply Crate)
-- for anything that needs a working cast-bar interaction.
--
-- Renumbered all 8 currently-affected objects out of the 2300xxx range:
--   Lost Page I    2300500 -> 9900740
--   Lost Page II   2300503 -> 9900741
--   Lost Page III  2300504 -> 9900742
--   Lost Page IV   2300505 -> 9900743
--   Abbess' Journal        2300520 -> 9900744
--   Abbess's Staff         2300521 -> 9900745
--   Heretical Idol Purified 2300522 -> 9900746
--   Jewel                  2300523 -> 9900747
-- Updated: gameobject_template (new rows, old deleted), gameobject.id
-- (spawns repointed in place, same guids), smart_scripts.entryorguid (Lost
-- Pages' item-grant scripts), and quest_template.RequiredNpcOrGo1-4 for
-- Accursed Sisterhood (1660003), now referencing the new negative IDs.

INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild`)
SELECT 9900740, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild` FROM `gameobject_template` WHERE `entry`=2300500 AND NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry`=9900740);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild`)
SELECT 9900741, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild` FROM `gameobject_template` WHERE `entry`=2300503 AND NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry`=9900741);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild`)
SELECT 9900742, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild` FROM `gameobject_template` WHERE `entry`=2300504 AND NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry`=9900742);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild`)
SELECT 9900743, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild` FROM `gameobject_template` WHERE `entry`=2300505 AND NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry`=9900743);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild`)
SELECT 9900744, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild` FROM `gameobject_template` WHERE `entry`=2300520 AND NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry`=9900744);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild`)
SELECT 9900745, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild` FROM `gameobject_template` WHERE `entry`=2300521 AND NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry`=9900745);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild`)
SELECT 9900746, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild` FROM `gameobject_template` WHERE `entry`=2300522 AND NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry`=9900746);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild`)
SELECT 9900747, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild` FROM `gameobject_template` WHERE `entry`=2300523 AND NOT EXISTS (SELECT 1 FROM `gameobject_template` WHERE `entry`=9900747);

UPDATE `gameobject` SET `id`=9900740 WHERE `id`=2300500;
UPDATE `gameobject` SET `id`=9900741 WHERE `id`=2300503;
UPDATE `gameobject` SET `id`=9900742 WHERE `id`=2300504;
UPDATE `gameobject` SET `id`=9900743 WHERE `id`=2300505;
UPDATE `gameobject` SET `id`=9900744 WHERE `id`=2300520;
UPDATE `gameobject` SET `id`=9900745 WHERE `id`=2300521;
UPDATE `gameobject` SET `id`=9900746 WHERE `id`=2300522;
UPDATE `gameobject` SET `id`=9900747 WHERE `id`=2300523;

UPDATE `smart_scripts` SET `entryorguid`=9900740 WHERE `entryorguid`=2300500 AND `source_type`=1;
UPDATE `smart_scripts` SET `entryorguid`=9900741 WHERE `entryorguid`=2300503 AND `source_type`=1;
UPDATE `smart_scripts` SET `entryorguid`=9900742 WHERE `entryorguid`=2300504 AND `source_type`=1;
UPDATE `smart_scripts` SET `entryorguid`=9900743 WHERE `entryorguid`=2300505 AND `source_type`=1;

UPDATE `quest_template` SET `RequiredNpcOrGo1`=-9900744, `RequiredNpcOrGo2`=-9900745, `RequiredNpcOrGo3`=-9900746, `RequiredNpcOrGo4`=-9900747 WHERE `id`=1660003;

DELETE FROM `gameobject_template` WHERE `entry` IN (2300500,2300503,2300504,2300505,2300520,2300521,2300522,2300523);

-- Cleanup: remove the temporary diagnostic clone used to prove the entry-range theory.
DELETE FROM `gameobject` WHERE `guid`=9780560;
DELETE FROM `gameobject_template` WHERE `entry`=9900731;
