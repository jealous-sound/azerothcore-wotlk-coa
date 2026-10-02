-- 1. Cast bar fix, cracked via Supply Run's working crate: Data0=43 (lock)
-- DOES work, but only with castBarCaption EMPTY -- a custom text combined
-- with a lock apparently breaks it. Reapplied Data0=43 + castBarCaption=''
-- (was previously reverted to Data0=0 after the earlier failed attempts
-- with non-empty captions -- see rev_20260929_19/20/22/23).
UPDATE `gameobject_template` SET `Data0`=43, `castBarCaption`='' WHERE `entry` IN (2300579,162920,999013,999014,999015,2300520,2300521,2300522,2300523);

-- 2. Suspicious Guard (991516) respawn was the 300s default; set to 60s.
UPDATE `creature` SET `spawntimesecs`=60 WHERE `id`=991516;

-- 3. Added the missing Defias Bandit (116, stock) and Defias Rogue Wizard
-- (474, stock) spawns for Defias Disruption (1660071) at Bandit Bastion --
-- positions pulled from PR #5423's test server (same area as the Stolen
-- Supply Crate/Suspicious Guard spawns). 30s respawn each.
INSERT IGNORE INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`, `CreateObject`, `Comment`) VALUES
(9780541,116,0,12,87,1,1,0,-9794.6,-450.4,29.598,0,30,0,0,0,0,0,0,0,0,'',0,0,'Bandit Bastion - Defias Bandit'),
(9780542,116,0,12,87,1,1,0,-9802.6,-452.4,29.467,0.24498,30,0,0,0,0,0,0,0,0,'',0,0,'Bandit Bastion - Defias Bandit'),
(9780543,116,0,12,87,1,1,0,-9796.6,-458.4,29.495,1.32582,30,0,0,0,0,0,0,0,0,'',0,0,'Bandit Bastion - Defias Bandit'),
(9780544,116,0,12,87,1,1,0,-9782.6,-442.4,30.522,3.7296,30,0,0,0,0,0,0,0,0,'',0,0,'Bandit Bastion - Defias Bandit'),
(9780545,116,0,12,87,1,1,0,-9794.6,-466.4,29.086,1.5708,30,0,0,0,0,0,0,0,0,'',0,0,'Bandit Bastion - Defias Bandit'),
(9780546,116,0,12,87,1,1,0,-9782.6,-434.4,31.169,4.06889,30,0,0,0,0,0,0,0,0,'',0,0,'Bandit Bastion - Defias Bandit'),
(9780547,116,0,12,87,1,1,0,-9812.6,-438.4,29.758,5.69518,30,0,0,0,0,0,0,0,0,'',0,0,'Bandit Bastion - Defias Bandit'),
(9780548,474,0,12,87,1,1,0,-9786.6,-450.4,30.337,3.14159,30,0,0,0,0,0,0,0,0,'',0,0,'Bandit Bastion - Defias Rogue Wizard'),
(9780549,474,0,12,87,1,1,0,-9800.6,-444.4,29.862,5.49779,30,0,0,0,0,0,0,0,0,'',0,0,'Bandit Bastion - Defias Rogue Wizard'),
(9780550,474,0,12,87,1,1,0,-9778.6,-450.4,31.214,3.14159,30,0,0,0,0,0,0,0,0,'',0,0,'Bandit Bastion - Defias Rogue Wizard'),
(9780551,474,0,12,87,1,1,0,-9794.6,-432.4,29.819,4.71239,30,0,0,0,0,0,0,0,0,'',0,0,'Bandit Bastion - Defias Rogue Wizard'),
(9780552,474,0,12,87,1,1,0,-9786.6,-470.4,29.225,1.9513,30,0,0,0,0,0,0,0,0,'',0,0,'Bandit Bastion - Defias Rogue Wizard');
