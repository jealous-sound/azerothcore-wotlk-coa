-- Curse Shard (Spada Family Manor, "Seven Years of Bad Luck" quest, 1660057).
-- Reconstructed from: Exiles DB export (name, entry 162919, model 33054),
-- and player video observation (level shown as 9 while scaled to a level-9
-- player via Destiny Weaver level scaling; true base level estimated 5-6).
-- No spawn position is known from any source (client cache, Exiles DB, or
-- community datamine all lack a placement for this entry) -- spawn it
-- yourself in game with `.npc add 162919` at a remembered object location.

DELETE FROM `creature_template` WHERE `entry` = 162919;
INSERT INTO `creature_template`
  (`entry`, `name`, `subname`, `minlevel`, `maxlevel`, `faction`, `npcflag`,
   `speed_walk`, `speed_run`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`,
   `unit_class`, `unit_flags`, `dynamicflags`, `family`, `type`, `type_flags`,
   `HealthModifier`, `ManaModifier`, `ArmorModifier`, `ExperienceModifier`,
   `RegenHealth`, `flags_extra`, `AIName`, `MovementType`, `VerifiedBuild`)
VALUES
  (162919, 'Curse Shard', '', 5, 6, 54, 0,
   1, 1.14286, 0, 0, 2000, 2000,
   1, 0, 0, 0, 9, 0,
   1, 1, 1, 1,
   1, 0, '', 0, 12340);

DELETE FROM `creature_model_info` WHERE `DisplayID` = 33054;
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`, `DisplayID_Other_Gender`, `VerifiedBuild`)
VALUES (33054, 0.5, 1.0, 2, 0, 12340);

DELETE FROM `creature_template_model` WHERE `CreatureID` = 162919;
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`)
VALUES (162919, 0, 33054, 1, 1, 12340);
