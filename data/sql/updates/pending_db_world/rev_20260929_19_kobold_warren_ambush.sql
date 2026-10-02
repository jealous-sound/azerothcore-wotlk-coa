-- Worm-Eaten Apple's Kobold Warren (2300579) enhancements:
--
-- 1. Kobold Prospector (162915) added as a real hostile mob -- ambush that
--    can pop out of the warren when clicked. Modeled after stock Kobold
--    Miner (entry 40, same theme/zone level range): faction 26, level 6-7,
--    lootid 40 (reused, thematically identical loot table), two weighted
--    display variants (139/373). Fields the user gave directly (KillCredit,
--    modelids, HealthModifier/ManaModifier, RacialLeader, movementId 999)
--    are preserved as specified.
INSERT IGNORE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `speed_swim`, `speed_flight`, `detection_range`, `rank`, `dmgschool`, `DamageModifier`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `CreatureImmunitiesId`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
(162915,0,0,0,0,0,'Kobold Prospector','',NULL,0,6,7,0,26,0,1,1.14286,1,1,20,0,0,1,2000,2000,1,1,1,0,0,0,0,7,0,40,0,0,0,0,1,12,'',0,1,1.0,1.0,1,1,0,999,1,0,0,'',0);
INSERT IGNORE INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES
(162915,0,139,1,0.5,0),(162915,1,373,1,0.5,0);

-- 2. Kobold Warren made a proper "consumable" GOOBER (Data5=1) so it
--    actually despawns/respawns on use instead of instantly resetting to
--    GO_READY (GameObject::Update's GAMEOBJECT_TYPE_GOOBER branch skips
--    the respawn-delay path entirely when IsDespawnAtAction()==false, i.e.
--    Data5==0 -- see GameObject.cpp ~line 846). Respawn timer set to 30s
--    (raised from an initial 20s) on all 18 spawns so it can't be spammed.
--    Also had to set Data3 (autoCloseTime, in ms) to a nonzero value --
--    GameObject::Use()'s GOOBER case only calls SetLootState(GO_ACTIVATED)
--    at all when `info->GetAutoCloseTime()` is nonzero (see GameObject.cpp
--    ~line 1680); with Data3=0 the SmartAI GO_STATE_CHANGED hook (below)
--    would never fire on use no matter what state value it listened for.
--    3000ms also gives a short GO_FLAG_IN_USE window that blocks
--    instant-reclick spam on its own, on top of the 20s respawn.
-- Tried setting Data0 (lockId) to make a real client-visible cast bar show
-- (copying stock WotLK quest objects) -- this broke working behavior
-- instead (even the correct up-close respawn timing stopped working) and
-- was reverted. castBarCaption/Data3 alone do NOT produce a visible cast
-- bar without a lock, and a lock changes the interaction flow in ways that
-- aren't understood yet; left as a known limitation for a later session.
UPDATE `gameobject_template` SET `Data3`=2000, `Data5`=1, `castBarCaption`='Searching' WHERE `entry`=2300579;
UPDATE `gameobject` SET `spawntimesecs`=30 WHERE `id`=2300579;

-- 3. On use, 50% chance (event_chance) to summon a Kobold Prospector at the
--    warren's own position, hostile/attacking immediately (attackInvoker=1).
--    event_param1=2 (GO_ACTIVATED, the LootState -- not GOState -- value
--    GameObject::SetLootState() passes into the SmartAI hook on use; 0
--    would be GO_NOT_READY, which is never reached from a normal use and
--    never fired -- same latent bug also found and fixed in the Mirror
--    Shard -> Curse Shard script, see rev_20260929_20).
--    summonType=6 (TEMPSUMMON_CORPSE_TIMED_DESPAWN) so it leaves a normal
--    lootable corpse for 30s after death (enough time to loot) instead of
--    vanishing instantly.
DELETE FROM `smart_scripts` WHERE `entryorguid`=2300579 AND `source_type`=1;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2300579,1,0,0,70,0,50,0,2,0,0,0, 12,162915,6,30000,1,0,0, 1,0,0,0,0,0,'Kobold Warren - on use, 50% chance to ambush-summon Kobold Prospector (lootable corpse for 30s)');
