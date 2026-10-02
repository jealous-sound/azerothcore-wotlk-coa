-- Adds 4 more PR #5423 Goldshire flavor NPCs: four "Goldshire Farmer"
-- ambient spawns (162821, 162822, 162823, 162824). No quest links on the
-- source side. Spawn guids picked from our own 9780xxx sequence to avoid
-- colliding with PR #5423's 9001xxx numbering.
INSERT IGNORE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `speed_swim`, `speed_flight`, `detection_range`, `rank`, `dmgschool`, `DamageModifier`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `CreatureImmunitiesId`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
(162821,0,0,0,0,0,'Goldshire Farmer',NULL,NULL,0,5,5,0,12,0,1,1.14286,1,1,18,0,0,1,1500,2000,1,1,1,512,2048,0,0,7,0,0,0,0,0,0,0,0,'',0,1,0.93,1,1,1,0,0,1,0,2,'',0),
(162822,0,0,0,0,0,'Goldshire Farmer',NULL,NULL,0,5,5,0,12,0,1,1.14286,1,1,18,0,0,1,1500,2000,1,1,1,512,2048,0,0,7,0,0,0,0,0,0,0,0,'',0,1,0.96,1,1,1,0,0,1,0,2,'',0),
(162823,0,0,0,0,0,'Goldshire Farmer',NULL,NULL,0,5,5,0,12,0,1,1.14286,1,1,18,0,0,1,1500,2000,1,1,1,512,2048,0,0,7,0,0,0,0,0,0,0,0,'',0,1,0.96,1,1,1,0,0,1,0,2,'',0),
(162824,0,0,0,0,0,'Goldshire Farmer',NULL,NULL,0,5,5,0,12,0,1,1.14286,1,1,18,0,0,1,1500,2000,1,1,1,512,2048,0,0,7,0,0,0,0,0,0,0,0,'',0,1,0.96,1,1,1,0,0,1,0,2,'',0);

INSERT IGNORE INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES
(162821,0,3614,1,1,0),(162822,0,16920,1,1,0),(162823,0,19178,1,1,0),(162824,0,25563,1,1,0);

INSERT IGNORE INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`, `CreateObject`, `Comment`) VALUES
(9780508,162821,0,0,0,1,1,0,-9500,95,56.973,5.1448,300,0,0,0,0,0,0,0,0,'',0,0,'Goldshire WB1 content'),
(9780509,162823,0,0,0,1,1,0,-9445,-40,60.225,1.91382,300,0,0,0,0,0,0,0,0,'',0,0,'Goldshire WB1 content'),
(9780510,162824,0,0,0,1,1,0,-9508,80,56.959,5.36226,300,0,0,0,0,0,0,0,0,'',0,0,'Goldshire WB1 content'),
(9780511,162822,0,0,0,1,1,0,-9406.93,-49.6264,64.5041,6.12616,300,0,0,0,0,0,0,0,0,'',0,0,'Goldshire WB1 content');
