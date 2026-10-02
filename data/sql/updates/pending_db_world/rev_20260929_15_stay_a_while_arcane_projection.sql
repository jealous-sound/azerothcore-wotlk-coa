-- Stay a While (1660060): quest_template had no objective set up
-- (RequiredNpcOrGo1 was 0). Full user-provided quest data applied
-- (credit target: virtual id 162921 "Listen to Aliscar Lend", never
-- spawned -- see the KillCredit1 trick below).
-- Root cause of the auto-complete bug found after this was first built:
-- ObjectMgr::LoadQuests() (see Globals/ObjectMgr.cpp ~line 5579) validates
-- every positive RequiredNpcOrGo entry against `GetCreatureTemplate(id)` at
-- boot, and SILENTLY RESETS the requirement to 0 in memory (DB row is left
-- untouched, so this survives .reload and even looks fine on inspection)
-- if no matching creature_template row exists. 162921 was designed as a
-- purely virtual kill-credit target (per the KillCredit1 redirect trick)
-- and was never meant to be spawned, but the loader still requires a row to
-- exist. This minimal, type_flags=1024/flags_extra=2 (never-spawn) row
-- satisfies that check without adding an actual interactable creature.
INSERT IGNORE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `speed_swim`, `speed_flight`, `detection_range`, `rank`, `dmgschool`, `DamageModifier`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `CreatureImmunitiesId`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
(162921,0,0,0,0,0,'Listen to Aliscar Lend','',NULL,0,8,8,0,35,0,1,1.14286,1,1,18,0,0,1,2000,2000,1,1,1,512,2048,0,0,10,1024,0,0,0,0,0,0,0,'',0,1,1,1,1,1,0,0,1,0,2,'',0);

UPDATE `quest_template` SET
  `QuestLevel`=8, `MinLevel`=5, `QuestSortID`=12,
  `RewardXPDifficulty`=3, `RewardMoney`=0, `Flags`=8,
  `RewardFactionID1`=72, `RewardFactionValue1`=5,
  `LogTitle`='Stay a While',
  `LogDescription`='Take a moment from the noise and haste and stay a while to listen to Aliscar Lend.',
  `QuestDescription`='As it turns out, I\'m the leading authority around here when it comes to the village\'s history. Care for a short lesson?$b$b<The aging sorcerer seems eager, almost desperate, to talk.>$b$b<Perhaps he has something worth sharing.>',
  `QuestCompletionLog`='Say goodbye to Aliscar Lend.',
  `RequiredNpcOrGo1`=162921, `RequiredNpcOrGoCount1`=1,
  `ObjectiveText1`='Listen to Aliscar Lend'
WHERE `ID`=1660060;

-- Mechanic (all coords user-confirmed via in-game .gps):
--  1. Player accepts 1660060 from Aliscar Lend (162806, unchanged, stays at
--     his normal spot so others can still take the quest from him too).
--  2. "Arcane Projection of Aliscar" (162943 -- pre-existing base
--     creature_template row, VerifiedBuild 12340, previously unconfigured/
--     unused; reused here rather than creating a new entry) is a
--     PERMANENT spawn (guid 9780530) floating at coord1
--     (-9406.413,-7.2774053,79.7, map 0 -- Z raised ~1.5yd above the
--     .gps-captured 78.219315 so his feet clear the floor). He does not
--     despawn; every player can walk up to him independently.
--  3. Aliscar's SmartAI (ACCEPTED_QUEST event) teleports the accepting
--     player to coord2 (-9410.737,-8.331479,78.219315, map 0), a few yards
--     from the projection on the same floating platform.
--  4. 162943 hovers via SMART_ACTION_SET_FLY (disable gravity) on
--     SMART_EVENT_AI_INIT (fires on both normal load and respawn, unlike
--     JUST_SUMMONED which only fires for temp summons -- needed once this
--     became a permanent DB spawn instead of a per-accept summon).
--  5. Right-clicking 162943 opens a gossip menu with one option ("This
--     village looks quiet and welcoming."); selecting it stores the
--     invoking player, has 162943 speak his line (creature_text), closes
--     the gossip window, and sets SmartAI phase 1.
--  6. 5000ms later (phase-gated SMART_EVENT_UPDATE, NOT_REPEATABLE),
--     162943 grants quest credit via SMART_ACTION_CALL_KILLEDMONSTER
--     against the stored invoker -- no actual combat/kill happens.
--     IMPORTANT correction vs the Hidden Path/Ruined Estate stalker
--     precedent (coa_codebase_gotchas): that gotcha's "CreatureId param
--     must be the trigger's own entry, not 0" only applies when the
--     objective entry and the credit-granting creature's own entry are the
--     SAME id. Here they differ (162943 vs 162921), and
--     SMART_ACTION_CALL_KILLEDMONSTER -> Player::RewardPlayerAndGroupAtEvent
--     -> Player::KilledMonsterCredit(entry, guid) compares that `entry`
--     param DIRECTLY against RequiredNpcOrGo -- it does NOT apply the
--     KillCredit1 template-field redirect (that redirect only happens in
--     the real combat-kill path, Player::KilledMonster, which this action
--     never goes through). So action_param1 must be 162921 (the quest's
--     actual RequiredNpcOrGo1) directly, not 162943's own entry; the
--     KillCredit1=162921 set on 162943's template below ends up unused by
--     this particular mechanism but is harmless to leave in place.

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=162806;

UPDATE `creature_template` SET `KillCredit1`=162921, `AIName`='SmartAI', `npcflag`=1, `gossip_menu_id`=162943, `unit_flags`=2, `MovementType`=0 WHERE `entry`=162943;
UPDATE `creature_template_model` SET `CreatureDisplayID`=18928 WHERE `CreatureID`=162943;

INSERT IGNORE INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`, `CreateObject`, `Comment`) VALUES
(9780530,162943,0,12,87,1,1,0,-9406.413,-7.2774053,79.7,3.4325914,300,0,0,0,0,0,0,0,0,'',0,0,'Stay a While - permanent Arcane Projection of Aliscar');

-- Permanent visual aura (applied automatically at spawn, no SmartAI cast needed).
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `visibilityDistanceType`, `auras`) VALUES
(162943, 0, 0, 0, 0, 0, 0, '103921')
ON DUPLICATE KEY UPDATE `auras`='103921';

INSERT IGNORE INTO `npc_text` (`ID`, `text0_0`, `lang0`, `Probability0`) VALUES
(162943, 'A shimmering, arcane figure of Aliscar Lend stands before you, waiting.', 0, 1);
INSERT IGNORE INTO `gossip_menu` (`MenuID`, `TextID`) VALUES (162943, 162943);
-- OptionType=1 (GOSSIP_OPTION_GOSSIP), not 0 -- GOSSIP_OPTION_NONE(0) has no
-- switch case in PlayerGossip.cpp, falls to default:, logs "unknown
-- OptionType" and sets canTalk=false, silently hiding the option.
INSERT IGNORE INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionType`, `OptionNpcFlag`) VALUES
(162943, 0, 0, 'This village looks quiet and welcoming.', 1, 1);

-- Type=12 (CHAT_MSG_MONSTER_SAY), NOT 0. `creature_text.Type` casts directly
-- to the full protocol `ChatMsg` enum (CreatureTextMgr::LoadCreatureTexts,
-- `temp.type = ChatMsg(fields[4].Get<uint8>())`), where 0 is
-- CHAT_MSG_SYSTEM (plain yellow line, no speaker name, no speech bubble) --
-- not a simplified SAY/YELL/EMOTE=0/1/2 scheme as the Bianca yell precedent
-- (Type=1, actually CHAT_MSG_SAY not YELL) seemed to suggest. Worth
-- rechecking Bianca's (161700) periodic line against this too.
INSERT IGNORE INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `BroadcastTextId`) VALUES
(162943, 0, 0, 'It is, despite its history. It sits at a crossroads of trade routes. Gold runs gladly through its squares and streets. Folks say you can find things for sale in Goldshire you wouldn\'t see even in Stormwind.', 12, 0, 100, 0);

DELETE FROM `smart_scripts` WHERE (`entryorguid`=162806 AND `source_type`=0 AND `id` IN (0,1)) OR (`entryorguid`=162943 AND `source_type`=0 AND `id` IN (0,1,2,3,4,5,6));
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(162806,0,1,0,19,0,100,0,1660060,0,0,0, 62,0,0,0,0,0,0, 7,0,-9410.737,-8.331479,78.219315,0.20678572,'Aliscar Lend - on accept Stay a While - teleport player to coord2'),
(162943,0,0,0,37,0,100,0,0,0,0,0, 60,1,0,1,0,0,0, 1,0,0,0,0,0,'Arcane Projection - on AI init (permanent spawn + respawn) - float/disable gravity'),
(162943,0,1,0,62,0,100,0,162943,0,0,0, 64,1,0,0,0,0,0, 7,0,0,0,0,0,'Arcane Projection - gossip select - store invoker'),
(162943,0,2,0,62,0,100,0,162943,0,0,0, 1,0,0,0,0,0,0, 1,0,0,0,0,0,'Arcane Projection - gossip select - talk'),
(162943,0,3,0,62,0,100,0,162943,0,0,0, 22,1,0,0,0,0,0, 1,0,0,0,0,0,'Arcane Projection - gossip select - set phase 1'),
(162943,0,4,0,62,0,100,0,162943,0,0,0, 72,0,0,0,0,0,0, 7,0,0,0,0,0,'Arcane Projection - gossip select - close gossip window'),
(162943,0,5,0,60,1,100,1,5000,5000,0,0, 33,162921,0,0,0,0,0, 12,1,0,0,0,0,'Arcane Projection - 5s after talk - killcredit via stored invoker (162921 = actual quest RequiredNpcOrGo1, KillCredit1 redirect does not apply to this action path)'),
(162943,0,6,0,60,1,100,1,5000,5000,0,0, 62,0,0,0,0,0,0, 12,1,-9394.112,-15.47909,62.166607,2.7722065,'Arcane Projection - 5s after talk - teleport player back down near Aliscar Lend for turn-in (user-confirmed .gps)');
