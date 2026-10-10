-- Two Stormwind quests transcribed from in-game captures (2026-10-04):
-- White-Glove Mischief (Little Mickey) and Hookweed for the Wounded (Injured Fisherman).
-- Items 559856-559858 and 559971 already exist. Flask spell 365022 is handled by spell_coa_open_little_mickeys_flask.
INSERT INTO `creature_template` (`entry`, `name`, `subname`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `rank`, `unit_class`, `unit_flags`, `type`, `lootid`, `AIName`, `MovementType`, `HealthModifier`, `flags_extra`, `VerifiedBuild`)
VALUES
(164026, 'Little Mickey', '', 1, 1, 12, 2, 1, 1.14286, 0, 1, 768, 7, 0, '', 0, 1, 2, 0),
(164220, 'Little Paul', '', 1, 1, 12, 2, 1, 1.14286, 0, 1, 768, 7, 0, '', 0, 1, 2, 0),
(9920001, 'Injured Fisherman', '', 18, 18, 12, 2, 1, 1.14286, 0, 1, 768, 7, 0, '', 0, 1, 2, 0),
(9920002, 'Shaderwell Murloc Seer', '', 18, 19, 18, 0, 1, 1.14286, 0, 2, 0, 7, 9920002, '', 1, 1, 0, 0),
(9920003, 'Petitioner''s Chamber Credit', '', 1, 1, 35, 0, 1, 1.14286, 0, 1, 33554434, 10, 0, 'SmartAI', 0, 1, 128, 0)
ON DUPLICATE KEY UPDATE `name` = VALUES(`name`), `subname` = VALUES(`subname`), `minlevel` = VALUES(`minlevel`),
  `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `npcflag` = VALUES(`npcflag`),
  `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `rank` = VALUES(`rank`),
  `unit_class` = VALUES(`unit_class`), `unit_flags` = VALUES(`unit_flags`), `type` = VALUES(`type`),
  `lootid` = VALUES(`lootid`), `AIName` = VALUES(`AIName`), `MovementType` = VALUES(`MovementType`),
  `HealthModifier` = VALUES(`HealthModifier`), `flags_extra` = VALUES(`flags_extra`), `VerifiedBuild` = VALUES(`VerifiedBuild`);
DELETE FROM `creature_template_model` WHERE `CreatureID` IN (164026, 164220, 9920001, 9920002, 9920003);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES
(164026, 0, 141426, 1, 1, 0),
(164220, 0, 141416, 1, 1, 0),
(9920001, 0, 14533, 1, 1, 0),
(9920002, 0, 1079, 1, 1, 0),
(9920003, 0, 11686, 1, 1, 0);
DELETE FROM `creature_template_addon` WHERE `entry` = 9920001;
INSERT INTO `creature_template_addon` (`entry`, `bytes1`) VALUES (9920001, 1);
INSERT INTO `quest_template` (`ID`, `QuestType`, `QuestLevel`, `MinLevel`, `QuestSortID`, `RewardXPDifficulty`, `RewardMoney`, `StartItem`, `Flags`, `RewardItem1`, `RewardAmount1`, `LogTitle`, `LogDescription`, `QuestDescription`, `QuestCompletionLog`, `RequiredNpcOrGo1`, `RequiredNpcOrGoCount1`, `RequiredItemId1`, `RequiredItemCount1`, `ObjectiveText1`, `VerifiedBuild`)
VALUES
(9920010, 2, -1, 10, 1519, 7, 550, 559857, 0, 0, 0, 'White-Glove Mischief', 'Venture into the Petitioner''s Chamber within Stormwind Keep and open Little Mickey''s flask.', 'Uh... er... hello.$B$BI''m not up to anything, okay?$B$BIt''s just... I was given a task and... the truth is, I''m not as brave as I thought. I feel like I''m going to mess it up. I always mess things up...$B$BHuh? You... you would help me? Really? Let''s see... you have to do this:$B$BThe boy begins to list the tasks, lifting his tiny fingers one by one. As he does, you notice that his pinky finger is missing.$B$BOne: Sneak into the Castle. Two: Reach the Petitioner''s Chamber. Three: Open this flask. Four: Return to Cer... Return to Little Paul!', 'Return to Little Mickey in Stormwind.', 9920003, 1, 559858, 1, 'Access the Petitioner''s Chamber', 0),
(9920011, 2, -1, 10, 1519, 7, 550, 0, 8, 559971, 5, 'Hookweed for the Wounded', 'Collect Hookweed plants at Lake Landen and deliver them to the wounded fisherman.', 'Those damned murlocs... I''ll have my revenge yet!$B$B<The fisherman''s wounds are fresh. Though they do not seem fatal, he needs medical attention before his condition worsens.>$B$BA murloc ambushed me and made off with the little catch I had managed to land. While I''m lucky to be alive to tell the tale, I can barely stand. I think its weapon was poisoned...$B$BA plant grows in this lake that we fishermen often use to treat wounds from venomous fish. Hookweed, we call it. Help me gather a few sprouts, would you?', 'Return to the Injured Fisherman at Lake Landen.', 0, 0, 559856, 6, '', 0)
ON DUPLICATE KEY UPDATE `QuestType` = VALUES(`QuestType`), `QuestLevel` = VALUES(`QuestLevel`), `MinLevel` = VALUES(`MinLevel`),
  `QuestSortID` = VALUES(`QuestSortID`), `RewardXPDifficulty` = VALUES(`RewardXPDifficulty`), `RewardMoney` = VALUES(`RewardMoney`),
  `StartItem` = VALUES(`StartItem`), `Flags` = VALUES(`Flags`), `RewardItem1` = VALUES(`RewardItem1`),
  `RewardAmount1` = VALUES(`RewardAmount1`), `LogTitle` = VALUES(`LogTitle`), `LogDescription` = VALUES(`LogDescription`),
  `QuestDescription` = VALUES(`QuestDescription`), `QuestCompletionLog` = VALUES(`QuestCompletionLog`), `RequiredNpcOrGo1` = VALUES(`RequiredNpcOrGo1`),
  `RequiredNpcOrGoCount1` = VALUES(`RequiredNpcOrGoCount1`), `RequiredItemId1` = VALUES(`RequiredItemId1`), `RequiredItemCount1` = VALUES(`RequiredItemCount1`),
  `ObjectiveText1` = VALUES(`ObjectiveText1`), `VerifiedBuild` = VALUES(`VerifiedBuild`);
DELETE FROM `quest_template_addon` WHERE `ID` IN (9920010, 9920011);
INSERT INTO `quest_template_addon` (`ID`, `ProvidedItemCount`) VALUES
(9920010, 1),
(9920011, 0);
DELETE FROM `creature_queststarter` WHERE `quest` IN (9920010, 9920011);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(164026, 9920010),
(9920001, 9920011);
DELETE FROM `creature_questender` WHERE `quest` IN (9920010, 9920011);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(164026, 9920010),
(9920001, 9920011);
DELETE FROM `creature_loot_template` WHERE `Entry` = 9920002;
INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
(9920002, 559856, 0, 45, 1, 1, 0, 1, 1, 'Shaderwell Murloc Seer - Hookweed');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 9920003 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`,
  `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `event_param6`, `action_type`, `action_param1`,
  `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`,
  `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(9920003, 0, 0, 0, 10, 0, 100, 0, 2, 20, 1000, 1000, 1, 0, 33, 9920003, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0,
  'Petitioner''s Chamber Credit - Player in range - Give White-Glove Mischief credit');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceEntry` = 9920003;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`,
  `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 1, 9920003, 0, 0, 9, 0, 9920010, 0, 0, 0, 0, 0, '', 'Chamber credit only while White-Glove Mischief is taken');
DELETE FROM `spell_script_names` WHERE `spell_id` = 365022;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (365022, 'spell_coa_open_little_mickeys_flask');
DELETE FROM `creature` WHERE `guid` BETWEEN 9920100 AND 9920199;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`,
  `orientation`, `spawntimesecs`, `wander_distance`, `MovementType`, `Comment`) VALUES
(9920100, 164026, 0, 0, 0, 1, 1, -8245.042, 409.0107, 118.86457, 5.499274, 300, 0, 0, 'Little Mickey (captured in game)'),
(9920101, 9920001, 0, 0, 0, 1, 1, -8214.718, 400.10953, 117.337625, 2.5281098, 300, 0, 0, 'Injured Fisherman (captured in game)'),
(9920102, 9920003, 0, 0, 0, 1, 1, -8314.038, 292.43518, 126.93212, 3.617409, 300, 0, 0, 'Petitioner''s Chamber Credit (captured in game)'),
(9920103, 9920002, 0, 0, 0, 1, 1, -8186.6187, 393.19376, 116.88852, 4.110673, 240, 5, 1, 'Shaderwell Murloc Seer (captured in game)'),
(9920104, 9920002, 0, 0, 0, 1, 1, -8213.872, 379.43088, 117.57982, 4.909419, 240, 5, 1, 'Shaderwell Murloc Seer (captured in game)'),
(9920105, 9920002, 0, 0, 0, 1, 1, -8210.764, 351.98996, 101.16349, 2.026214, 240, 5, 1, 'Shaderwell Murloc Seer (captured in game)'),
(9920106, 9920002, 0, 0, 0, 1, 1, -8240.477, 354.56747, 104.39036, 2.2437823, 240, 5, 1, 'Shaderwell Murloc Seer (captured in game)'),
(9920107, 9920002, 0, 0, 0, 1, 1, -8227.397, 339.89185, 107.35488, 0.04466776, 240, 5, 1, 'Shaderwell Murloc Seer (captured in game)'),
(9920108, 9920002, 0, 0, 0, 1, 1, -8183.84, 386.74847, 115.69952, 5.660249, 240, 5, 1, 'Shaderwell Murloc Seer (captured in game)'),
(9920109, 9920002, 0, 0, 0, 1, 1, -8184.8257, 278.79202, 116.364746, 2.3191574, 240, 5, 1, 'Shaderwell Murloc Seer (captured in game)'),
(9920110, 9920002, 0, 0, 0, 1, 1, -8159.2324, 346.29218, 107.7084, 1.4332373, 240, 5, 1, 'Shaderwell Murloc Seer (captured in game)'),
(9920111, 9920002, 0, 0, 0, 1, 1, -8169.422, 285.41296, 119.826584, 2.1770096, 240, 5, 1, 'Shaderwell Murloc Seer (captured in game)'),
(9920112, 9920002, 0, 0, 0, 1, 1, -8129.5776, 361.0641, 118.81857, 3.1461933, 240, 5, 1, 'Shaderwell Murloc Seer (captured in game)'),
(9920113, 9920002, 0, 0, 0, 1, 1, -8157.978, 308.91367, 119.05455, 3.779988, 240, 5, 1, 'Shaderwell Murloc Seer (captured in game)'),
(9920114, 9920002, 0, 0, 0, 1, 1, -8112.2344, 322.04068, 132.54231, 1.050726, 240, 5, 1, 'Shaderwell Murloc Seer (captured in game)'),
(9920115, 9920002, 0, 0, 0, 1, 1, -8243.617, 262.0109, 116.73305, 1.2140878, 240, 5, 1, 'Shaderwell Murloc Seer (captured in game)'),
(9920116, 9920002, 0, 0, 0, 1, 1, -8218.638, 306.0725, 104.78145, 1.4261456, 240, 5, 1, 'Shaderwell Murloc Seer (captured in game)'),
(9920117, 9920002, 0, 0, 0, 1, 1, -8305.788, 364.31882, 105.72611, 1.237649, 240, 5, 1, 'Shaderwell Murloc Seer (captured in game)'),
(9920118, 9920002, 0, 0, 0, 1, 1, -8311.438, 430.1458, 117.42528, 5.4866548, 240, 5, 1, 'Shaderwell Murloc Seer (captured in game)'),
(9920119, 9920002, 0, 0, 0, 1, 1, -8252.487, 444.3206, 113.48136, 5.717553, 240, 5, 1, 'Shaderwell Murloc Seer (captured in game)'),
(9920120, 9920002, 0, 0, 0, 1, 1, -8271.878, 427.4335, 106.97596, 5.518847, 240, 5, 1, 'Shaderwell Murloc Seer (captured in game)');
