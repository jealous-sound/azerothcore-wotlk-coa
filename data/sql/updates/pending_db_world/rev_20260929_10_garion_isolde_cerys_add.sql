-- Adds 3 flavor NPCs from PR #5423's Goldshire content that didn't exist on
-- our side yet: Garion Hunter (97921, Traveling Riding Trainer), Isolde
-- (162810, Maid of House Bruck), Cerys (162812, Maid of House Mortel).
-- No quest links, vendor lists, or equipment on the source side -- pure
-- ambient NPCs. Spawn guids picked from our own 9780xxx sequence to avoid
-- colliding with PR #5423's 9001xxx numbering.
INSERT IGNORE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `speed_swim`, `speed_flight`, `detection_range`, `rank`, `dmgschool`, `DamageModifier`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `CreatureImmunitiesId`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
(97921,0,0,0,0,0,'Garion Hunter','Traveling Riding Trainer',NULL,4018,10,10,0,12,83,1,1.14286,1,1,18,0,0,1,2000,2000,1,1,1,512,2048,0,0,7,134217728,0,0,0,0,0,0,0,'',0,1,1.225,1,1,1,0,0,1,0,2,'',0),
(162810,0,0,0,0,0,'Isolde','Maid of House Bruck',NULL,0,5,5,0,12,0,1,1.14286,1,1,18,0,0,1,1500,2000,1,1,1,512,2048,0,0,7,0,0,0,0,0,0,0,0,'',0,1,0.98,1,1,1,0,0,1,0,2,'',0),
(162812,0,0,0,0,0,'Cerys','Maid of House Mortel',NULL,0,5,5,0,12,0,1,1.14286,1,1,18,0,0,1,1500,2000,1,1,1,512,2048,0,0,7,0,0,0,0,0,0,0,0,'',0,1,0.96,1,1,1,0,0,1,0,2,'',0);

INSERT IGNORE INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES
(97921,0,3274,1,1,0),(162810,0,25555,1,1,0),(162812,0,25556,1,1,0);

INSERT IGNORE INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`, `CreateObject`, `Comment`) VALUES
(9780503,97921,0,0,0,1,1,0,-9424.3,9.24,57.924,2.43039,300,0,0,0,0,0,0,0,0,'',0,0,'Goldshire WB1 content'),
(9780504,162810,0,0,0,1,1,0,-9468.5,39.5,56.538,0.69866,300,0,0,0,0,0,0,0,0,'',0,0,'Goldshire WB1 content'),
(9780505,162812,0,0,0,1,1,0,-9464.5,44.5,56.527,4.23504,300,0,0,0,0,0,0,0,0,'',0,0,'Goldshire WB1 content');
