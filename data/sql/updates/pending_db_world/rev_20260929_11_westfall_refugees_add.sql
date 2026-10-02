-- Adds 2 more PR #5423 Goldshire flavor NPCs: two "Westfall Refugee"
-- ambient spawns (162817 display 3554, 162819 display 18619). No quest
-- links, vendor lists, or equipment on the source side. Spawn guids picked
-- from our own 9780xxx sequence to avoid colliding with PR #5423's 9001xxx
-- numbering.
INSERT IGNORE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `speed_swim`, `speed_flight`, `detection_range`, `rank`, `dmgschool`, `DamageModifier`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `CreatureImmunitiesId`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
(162817,0,0,0,0,0,'Westfall Refugee',NULL,NULL,0,5,5,0,12,0,1,1.14286,1,1,18,0,0,1,1500,2000,1,1,1,512,2048,0,0,7,0,0,0,0,0,0,0,0,'',0,1,0.96,1,1,1,0,0,1,0,2,'',0),
(162819,0,0,0,0,0,'Westfall Refugee',NULL,NULL,0,5,5,0,12,0,1,1.14286,1,1,18,0,0,1,1500,2000,1,1,1,512,2048,0,0,7,0,0,0,0,0,0,0,0,'',0,1,0.96,1,1,1,0,0,1,0,2,'',0);

INSERT IGNORE INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES
(162817,0,3554,1,1,0),(162819,0,18619,1,1,0);

INSERT IGNORE INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`, `CreateObject`, `Comment`) VALUES
(9780506,162817,0,0,0,1,1,0,-9566,12,59.09,2.30361,300,0,0,0,0,0,0,0,0,'',0,0,'Goldshire WB1 content'),
(9780507,162819,0,0,0,1,1,0,-9582,34,58.933,5.24046,300,0,0,0,0,0,0,0,0,'',0,0,'Goldshire WB1 content');
