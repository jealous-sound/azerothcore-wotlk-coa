-- Living Heresy (161711) -- new ambush mob, 30% chance to spawn when
-- picking up any of the 4 Lost Pages (9900740-9900743, matching the same
-- pattern as Kobold Warren's Kobold Prospector ambush). Faction 25 (same
-- family as Accursed Judge/Censor, the existing Secret Inquisitorial
-- Dungeon content this ties into thematically). Fields the user gave
-- directly (KillCredit, modelid, HealthModifier/ManaModifier, RacialLeader,
-- movementId 999, type=0/family=0/type_flags=0) preserved as specified.
INSERT IGNORE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `speed_swim`, `speed_flight`, `detection_range`, `rank`, `dmgschool`, `DamageModifier`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `CreatureImmunitiesId`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
(161711,0,0,0,0,0,'Living Heresy','',NULL,0,6,7,0,17,0,1,1.14286,1,1,20,0,0,1,2000,2000,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,1,1.0,1.0,1,1,0,999,1,0,0,'',0);
INSERT IGNORE INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES (161711,0,19110,0.6,1,0);

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (9900740,9900741,9900742,9900743) AND `source_type`=1 AND `id`=1;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(9900740,1,1,0,70,0,30,0,2,0,0,0, 12,161711,6,15000,1,0,0, 1,0,0,0,0,0,'Lost Page I - on use, 30% chance to ambush-summon Living Heresy'),
(9900741,1,1,0,70,0,30,0,2,0,0,0, 12,161711,6,15000,1,0,0, 1,0,0,0,0,0,'Lost Page II - on use, 30% chance to ambush-summon Living Heresy'),
(9900742,1,1,0,70,0,30,0,2,0,0,0, 12,161711,6,15000,1,0,0, 1,0,0,0,0,0,'Lost Page III - on use, 30% chance to ambush-summon Living Heresy'),
(9900743,1,1,0,70,0,30,0,2,0,0,0, 12,161711,6,15000,1,0,0, 1,0,0,0,0,0,'Lost Page IV - on use, 30% chance to ambush-summon Living Heresy');
