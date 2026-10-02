-- New Goldshire questline found in Questie-X: "Bandit Bastion" (east of
-- Goldshire), a 3-quest Defias-kill/collect chain, prereq quest 47
-- (stock vanilla, already in game). Data pulled from PR #5423's test
-- server, which had it partially built; fixed one real bug on the way.
--
-- 1. Defias Disruption (100071) -- kill 6x Defias Bandit (116, stock) +
--    4x Defias Rogue Wizard (474, stock). Giver/ender: Melika Isenstrider
--    (6778, stock, already spawned near Goldshire).
-- 2. Supply Run (100073) -- collect 4x "Stolen Goods" (item 5055564,
--    stock item) from the Stolen Supply Crate. Giver/ender: Remy "Two
--    Times" (241, stock, already spawned near Goldshire).
-- 3. A Betrayal Within (100074) -- starts via item 5055565 "Tattered
--    Orders" (no NPC starter), turned in to Marshal Dughan (240, stock,
--    already spawned at Goldshire).
--
-- Stolen Supply Crate (9900730) was GAMEOBJECT_TYPE_CHEST (3) on the PR
-- server -- doesn't work in this codebase at all (see coa_codebase_gotchas,
-- no case in GameObject::Use()). Rebuilt as GAMEOBJECT_TYPE_GOOBER (10),
-- consumable (Data5=1, 60s respawn), granting the quest item directly via
-- SmartAI SMART_ACTION_ADD_ITEM on use (GOOBER has no lootId field at all
-- -- GetLootId() returns 0 for every type except CHEST/FISHINGHOLE, so a
-- gameobject_loot_template row would silently never fire).
--
-- Tattered Orders (5055565, the quest-starting item for 100074) drops from
-- Suspicious Guard (991516), a new hostile mob hidden in the outpost --
-- matches the quest text ("a man dressed as a soldier stationed deep in a
-- Defias outpost"). Single spawn, guid 9780531.
REPLACE INTO `quest_template` (`ID`, `QuestType`, `QuestLevel`, `MinLevel`, `QuestSortID`, `QuestInfoID`, `SuggestedGroupNum`, `RequiredFactionId1`, `RequiredFactionId2`, `RequiredFactionValue1`, `RequiredFactionValue2`, `RewardNextQuest`, `RewardXPDifficulty`, `RewardMoney`, `RewardMoneyDifficulty`, `RewardDisplaySpell`, `RewardSpell`, `RewardHonor`, `RewardKillHonor`, `StartItem`, `Flags`, `RequiredPlayerKills`, `RewardItem1`, `RewardAmount1`, `RewardItem2`, `RewardAmount2`, `RewardItem3`, `RewardAmount3`, `RewardItem4`, `RewardAmount4`, `ItemDrop1`, `ItemDropQuantity1`, `ItemDrop2`, `ItemDropQuantity2`, `ItemDrop3`, `ItemDropQuantity3`, `ItemDrop4`, `ItemDropQuantity4`, `RewardChoiceItemID1`, `RewardChoiceItemQuantity1`, `RewardChoiceItemID2`, `RewardChoiceItemQuantity2`, `RewardChoiceItemID3`, `RewardChoiceItemQuantity3`, `RewardChoiceItemID4`, `RewardChoiceItemQuantity4`, `RewardChoiceItemID5`, `RewardChoiceItemQuantity5`, `RewardChoiceItemID6`, `RewardChoiceItemQuantity6`, `POIContinent`, `POIx`, `POIy`, `POIPriority`, `RewardTitle`, `RewardTalents`, `RewardArenaPoints`, `RewardFactionID1`, `RewardFactionValue1`, `RewardFactionOverride1`, `RewardFactionID2`, `RewardFactionValue2`, `RewardFactionOverride2`, `RewardFactionID3`, `RewardFactionValue3`, `RewardFactionOverride3`, `RewardFactionID4`, `RewardFactionValue4`, `RewardFactionOverride4`, `RewardFactionID5`, `RewardFactionValue5`, `RewardFactionOverride5`, `TimeAllowed`, `AllowableRaces`, `LogTitle`, `LogDescription`, `QuestDescription`, `AreaDescription`, `QuestCompletionLog`, `RequiredNpcOrGo1`, `RequiredNpcOrGo2`, `RequiredNpcOrGo3`, `RequiredNpcOrGo4`, `RequiredNpcOrGoCount1`, `RequiredNpcOrGoCount2`, `RequiredNpcOrGoCount3`, `RequiredNpcOrGoCount4`, `RequiredItemId1`, `RequiredItemId2`, `RequiredItemId3`, `RequiredItemId4`, `RequiredItemId5`, `RequiredItemId6`, `RequiredItemCount1`, `RequiredItemCount2`, `RequiredItemCount3`, `RequiredItemCount4`, `RequiredItemCount5`, `RequiredItemCount6`, `Unknown0`, `ObjectiveText1`, `ObjectiveText2`, `ObjectiveText3`, `ObjectiveText4`, `VerifiedBuild`) VALUES (100071,2,8,4,12,0,0,0,0,0,0,0,5,130,135,0,0,0,0,0,0,0,1397885,1,500813,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'Defias Disruption','Thin the ranks of the Defias at the Bandit Bastion, east of Goldshire.','Oh, um... are you one of those brave types? The ones who deal with danger and monsters and... all that?  It\'s just that things have not been the same here lately. The taproom\'s quieter, the roads feel emptier, and even the usual loud sorts have stopped passing through. I keep trying to tell myself it is just a slow season, but... I do not think that\'s it.  I heard a rumor, just a rumor, that some of those Defias bandits have built up a camp somewhere east of here. People are saying they\'ve been stopping travelers, stealing supplies, or maybe worse. I do not really know for sure, but... maybe if someone went out there and gave them a reason to think twice, things might calm down a bit?  That might bring folks back.',NULL,'Return to Melika Isenstrider at Goldshire in Elwynn Forest.',116,474,0,0,6,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,NULL,NULL,NULL,NULL,0),(100073,2,8,5,12,0,0,0,0,0,0,0,5,0,90,0,0,0,0,0,0,0,1397885,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,500814,1,500815,1,500816,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'Supply Run','Recover 4 Stolen Supply Crates from the Bandit Bastion, east of Goldshire.','Listen, I have a bit of a follow up for you, if you\'re still feeling up to the task.  There\'s talk that the Defias have gathered in a proper hideout east of here. It\'s a large encampment, far more organized than the usual rabble.  Some local merchants were hit in a raid not long ago. Crates of supplies were taken. Useful things like tools, cloth, even some rare spirits. The sort of goods folks rely on around here.  It\'s not without risk, but if someone were to head in and recover what was lost, I know a few people who would be grateful. I would see to it that you\'re rewarded for your effort.',NULL,'Return to Remy \"Two Times\" at Goldshire in Elwynn Forest.',0,0,0,0,0,0,0,0,5055564,0,0,0,0,0,4,0,0,0,0,0,0,NULL,NULL,NULL,NULL,0),(100074,2,8,5,12,0,0,0,0,0,0,0,5,130,135,0,0,0,0,5055565,0,0,1397884,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'A Betrayal Within','Return to Marshal Dughan.','You\'ve found what looks like a genuine Alliance document, carried by a man dressed as a soldier stationed deep in a Defias outpost. The contents point to collusion between Alliance forces and local bandits.  This should not be happening. Someone loyal needs to see this.  Deliver the document to an appropriate Alliance official. With any luck, they will know what to do with it.',NULL,'Return to Marshal Dughan at Goldshire in Elwynn Forest.',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,NULL,NULL,NULL,NULL,0);

INSERT IGNORE INTO `creature_queststarter` (`id`, `quest`) VALUES (6778,100071),(241,100073);
INSERT IGNORE INTO `creature_questender` (`id`, `quest`) VALUES (6778,100071),(241,100073),(240,100074);

-- Suspicious Guard (991516) -- drops the quest-starting item for A Betrayal Within.
INSERT IGNORE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `speed_swim`, `speed_flight`, `detection_range`, `rank`, `dmgschool`, `DamageModifier`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `CreatureImmunitiesId`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
(991516,0,0,0,0,0,'Suspicious Guard',NULL,NULL,0,8,8,0,17,0,1,1.14286,1,1,18,0,0,1,2000,2000,1,1,1,0,2048,0,0,7,0,991516,0,0,0,0,0,0,'',0,1,1.44,1,1,1,0,0,1,0,0,'',0);
INSERT IGNORE INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES (991516,0,1984,1,1,0);
INSERT IGNORE INTO `creature_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
(991516,5055565,0,100,0,1,0,1,1,'Suspicious Guard - Tattered Orders');
INSERT IGNORE INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`, `CreateObject`, `Comment`) VALUES
(9780531,991516,0,12,87,1,1,0,-9791.93,-481.764,29.191,1.65585,300,0,0,0,0,0,0,0,0,'',0,0,'Bandit Bastion - Suspicious Guard');

-- Stolen Supply Crate (9900730) -- rebuilt as GOOBER (10), not CHEST (3)
-- which the source PR used and does not work in this codebase at all.
-- Consumable, 60s respawn, grants item 5055564 directly via SmartAI
-- (GOOBER has no lootId field, gameobject_loot_template would never fire).
INSERT IGNORE INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild`) VALUES
(9900730,10,31,'Stolen Supply Crate','','','',1,43,100073,0,2000,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'SmartGameObjectAI','',0);

INSERT IGNORE INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `ScriptName`, `VerifiedBuild`, `Comment`) VALUES
(9780532,9900730,0,12,87,1,1,-9804.89,-434.225,29.773,0,0,0,0,1,60,100,1,'',0,'Bandit Bastion - Stolen Supply Crate'),
(9780533,9900730,0,12,87,1,1,-9790.07,-439.43,29.697,0,0,0,0,1,60,100,1,'',0,'Bandit Bastion - Stolen Supply Crate'),
(9780534,9900730,0,12,87,1,1,-9822.72,-438.736,29.85,0,0,0,0,1,60,100,1,'',0,'Bandit Bastion - Stolen Supply Crate'),
(9780535,9900730,0,12,87,1,1,-9815.08,-446.717,29.613,0,0,0,0,1,60,100,1,'',0,'Bandit Bastion - Stolen Supply Crate'),
(9780536,9900730,0,12,87,1,1,-9784.75,-461.985,30.815,0,0,0,0,1,60,100,1,'',0,'Bandit Bastion - Stolen Supply Crate'),
(9780537,9900730,0,12,87,1,1,-9803.04,-469.966,28.886,0,0,0,0,1,60,100,1,'',0,'Bandit Bastion - Stolen Supply Crate'),
(9780538,9900730,0,12,87,1,1,-9811.14,-456.433,29.119,0,0,0,0,1,60,100,1,'',0,'Bandit Bastion - Stolen Supply Crate'),
(9780539,9900730,0,12,87,1,1,-9769.24,-443.594,31.83,0,0,0,0,1,60,100,1,'',0,'Bandit Bastion - Stolen Supply Crate'),
(9780540,9900730,0,12,87,1,1,-9757.66,-442.206,32.799,0,0,0,0,1,60,100,1,'',0,'Bandit Bastion - Stolen Supply Crate');

DELETE FROM `smart_scripts` WHERE `entryorguid`=9900730 AND `source_type`=1;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(9900730,1,0,0,70,0,100,0,2,0,0,0, 56,5055564,1,0,0,0,0, 7,0,0,0,0,0,'Stolen Supply Crate - on use, give Stolen Goods item');

