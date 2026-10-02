-- Mirror Shard (the world object; the creature it spawns is 162919, "Curse
-- Shard"). No entry ID, model or type is confirmed for this object in any
-- source (client cache, Exiles DB and the community datamine all lack it),
-- and the quest's own req_npc_or_go/req_items are empty everywhere too.
-- Entry 162920 is invented (free in both the local DB and Exiles DB as of
-- 2026-09-27) to sit next to creature 162919 in the same content block.
-- Display model 1061017, confirmed by the player from memory.
--
-- Mechanic, per the player: the shard is a static pickup that only works
-- while "Seven Years of Bad Luck" (quest 1660057) is active, and picking
-- one up spawns a Curse Shard to fight. Built as GAMEOBJECT_TYPE_GOOBER
-- (Data1 = questId 1660057, which the core enforces -- see
-- GameObject::Use()/MeetsInvisibilityValue) with Data5 = consumable so it
-- disappears once used, and a SmartGameObjectAI script
-- (SMART_EVENT_GO_STATE_CHANGED -> SMART_ACTION_SUMMON_CREATURE 162919) to
-- spawn the creature on use. The summon trigger is untested -- verify in
-- game and adjust the smart_scripts row below if it doesn't fire.
--
-- Placement: 13 spawns placed in game by the player from memory
-- (`.gobject add`, which writes straight to `gameobject`), covering the
-- manor building and its grounds on map 0 around x -9260..-9315 / y 431..503
-- -- a few hundred yards east of Goldshire. Captured into this file below
-- so a DB rebuild doesn't lose them.

DELETE FROM `gameobject_template` WHERE `entry` = 162920;
INSERT INTO `gameobject_template`
  (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`,
   `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`,
   `AIName`, `ScriptName`, `VerifiedBuild`)
VALUES
  (162920, 10, 1061017, 'Mirror Shard', '', '', '', 1,
   0, 1660057, 0, 0, 0, 1, 0, 0,
   'SmartGameObjectAI', '', 12340);

DELETE FROM `smart_scripts` WHERE `entryorguid` = 162920 AND `source_type` = 1;
INSERT INTO `smart_scripts`
  (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`,
   `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `event_param6`,
   `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`,
   `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`,
   `target_x`, `target_y`, `target_z`, `target_o`, `comment`)
VALUES
  (162920, 1, 0, 0, 70, 0, 100, 0,
   0, 0, 0, 0, 0, 0,
   12, 162919, 2, 1800000, 1, 0, 0,
   1, 0, 0, 0, 0,
   0, 0, 0, 0, 'Mirror Shard - on use, summon Curse Shard (untested, best-effort mechanic)');

DELETE FROM `gameobject` WHERE `guid` BETWEEN 6960085 AND 6960098;
INSERT INTO `gameobject`
  (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`,
   `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`,
   `animprogress`, `state`)
VALUES
  (6960085,162920,0,1,1,-9299.61,461.083,86.049,2.29063,0,0,0.910841,0.412758,300,0,1),
  (6960086,162920,0,1,1,-9283.84,466.426,89.8739,0.353982,0,0,0.176069,0.984378,300,0,1),
  (6960087,162920,0,1,1,-9276.19,456.491,82.2662,5.30285,0,0,0.470775,-0.882254,300,0,1),
  (6960088,162920,0,1,1,-9269.18,458.005,82.2662,6.18119,0,0,0.0509775,-0.9987,300,0,1),
  (6960089,162920,0,1,1,-9272.12,451.249,79.1292,1.08352,0,0,0.515643,0.856803,300,0,1),
  (6960090,162920,0,1,1,-9264.2,434.552,80.0831,5.62203,0,0,0.32459,-0.945855,300,0,1),
  (6960091,162920,0,1,1,-9308.83,431.041,77.6255,3.431,0,0,0.989549,-0.144198,300,0,1),
  (6960092,162920,0,1,1,-9312.72,467.131,78.4115,2.0583,0,0,0.856861,0.515549,300,0,1),
  (6960093,162920,0,1,1,-9314.87,503.475,77.6926,1.89511,0,0,0.81199,0.583671,300,0,1),
  (6960094,162920,0,1,1,-9285.17,478.54,77.7975,4.98172,0,0,0.60577,-0.79564,300,0,1),
  (6960095,162920,0,1,1,-9278.39,493.968,78.7332,0.428812,0,0,0.212767,0.977103,300,0,1),
  (6960096,162920,0,1,1,-9272.65,478.557,78.7338,3.8562,0,0,0.936843,-0.34975,300,0,1),
  (6960097,162920,0,1,1,-9259.26,468.741,79.9347,4.73214,0,0,0.70009,-0.714055,300,0,1),
  (6960098,162920,0,1,1,-9291.61,447.963,78.3838,2.24111,0,0,0.900342,0.435182,300,0,1);
