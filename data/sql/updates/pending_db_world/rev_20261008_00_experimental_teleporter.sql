-- First-Class Experimental Teleporter (gameobject 323232): an 8 gold goblin teleporter beside the capitals'
-- Call Boards, in Darnassus and in Booty Bay, Gadgetzan and Everlook.
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `Data14`, `Data15`, `Data19`,
`Data20`, `ScriptName`) VALUES
(323232, 10, 2047, 'First-Class Experimental Teleporter', 1, 65535, 65535, 61004, 1, 'go_coa_experimental_teleporter')
ON DUPLICATE KEY UPDATE `type` = VALUES(`type`), `displayId` = VALUES(`displayId`), `name` = VALUES(`name`),
`size` = VALUES(`size`), `Data14` = VALUES(`Data14`), `Data15` = VALUES(`Data15`), `Data19` = VALUES(`Data19`),
`Data20` = VALUES(`Data20`), `ScriptName` = VALUES(`ScriptName`);

DELETE FROM `npc_text` WHERE `ID` = 61004;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `Probability0`) VALUES
(61004, 'Welcome to the First-Class Experimental Teleporter! $B$BFor a modest fee, this marvel of goblin engineering will hurl you across Azeroth in the blink of an eye. $B$BSimply select your destination and hold on tight. Management is not responsible for unexpected detours, rough landings, or spontaneous combustion.', 'Welcome to the First-Class Experimental Teleporter! $B$BFor a modest fee, this marvel of goblin engineering will hurl you across Azeroth in the blink of an eye. $B$BSimply select your destination and hold on tight. Management is not responsible for unexpected detours, rough landings, or spontaneous combustion.', 1);

DELETE FROM `gossip_menu` WHERE `MenuID` = 61004;
INSERT INTO `gossip_menu` (`MenuID`, `TextID`) VALUES
(61004, 61004);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 7905410 AND 7905416;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`,
`position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`,
`animprogress`, `state`, `ScriptName`, `VerifiedBuild`, `Comment`) VALUES
(7905410, 323232, 0, 0, 0, 1, 1, -8821.4, 634.0, 94.2, 3.92699, 0, 0, 0.9238797, -0.3826831, 0, 100, 1, '', 0,
'First-Class Experimental Teleporter (Stormwind)'),
(7905411, 323232, 1, 0, 0, 1, 1, 1579.26, -4421.08, 8.04226, 3.14159, 0, 0, 1, 0, 0, 100, 1, '', 0,
'First-Class Experimental Teleporter (Orgrimmar)'),
(7905412, 323232, 1, 0, 0, 1, 1, 9968.133, 2342.439, 1330.776, 0, 0, 0, 0, 1, 0, 100, 1, '', 0,
'First-Class Experimental Teleporter (Darnassus)'),
(7905413, 323232, 0, 0, 0, 1, 1, -4915.63, -944.21, 501.56, 2.2784, 0, 0, 0.9082991, 0.4183213, 0, 100, 1, '', 0,
'First-Class Experimental Teleporter (Ironforge)'),
(7905414, 323232, 0, 0, 0, 1, 1, -14300.5, 527.8, 8.8, 0.8484, 0, 0, 0.4115918, 0.9113683, 0, 100, 1, '', 0,
'First-Class Experimental Teleporter (Booty Bay)'),
(7905415, 323232, 1, 0, 0, 1, 1, -7173.9, -3787.6, 8.4, 2.9584, 0, 0, 0.995808, 0.0914683, 0, 100, 1, '', 0,
'First-Class Experimental Teleporter (Gadgetzan)'),
(7905416, 323232, 1, 0, 0, 1, 1, 6729.3, -4617.0, 720.9, 1.5284, 0, 0, 0.6919597, 0.7219362, 0, 100, 1, '', 0,
'First-Class Experimental Teleporter (Everlook)');
