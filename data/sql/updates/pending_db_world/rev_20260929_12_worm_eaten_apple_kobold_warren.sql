-- Worm-Eaten Apple (1660058) had no objective set up at all on our side
-- (RequiredNpcOrGo1 was 0). The real objective is 18x "Kobold Warren"
-- GAMEOBJECT_TYPE_GOOBER (entry 2300579, Data1=1660058 questId per the
-- GOOBER quest-gate mechanism, see coa_codebase_gotchas) scattered around
-- Goldshire, not a creature -- a first pass wrongly pulled RequiredNpcOrGo1
-- as the (unused, zero-spawn) creature 162940 "Kobold Warren Destroyed"
-- from PR #5423's quest_template row; corrected to -2300579 (negative = GO)
-- to match the actual interactable objects, and the dead creature_template
-- row removed.
REPLACE INTO `quest_template` (`ID`, `QuestType`, `QuestLevel`, `MinLevel`, `QuestSortID`, `QuestInfoID`, `SuggestedGroupNum`, `RequiredFactionId1`, `RequiredFactionId2`, `RequiredFactionValue1`, `RequiredFactionValue2`, `RewardNextQuest`, `RewardXPDifficulty`, `RewardMoney`, `RewardMoneyDifficulty`, `RewardDisplaySpell`, `RewardSpell`, `RewardHonor`, `RewardKillHonor`, `StartItem`, `Flags`, `RequiredPlayerKills`, `RewardItem1`, `RewardAmount1`, `RewardItem2`, `RewardAmount2`, `RewardItem3`, `RewardAmount3`, `RewardItem4`, `RewardAmount4`, `ItemDrop1`, `ItemDropQuantity1`, `ItemDrop2`, `ItemDropQuantity2`, `ItemDrop3`, `ItemDropQuantity3`, `ItemDrop4`, `ItemDropQuantity4`, `RewardChoiceItemID1`, `RewardChoiceItemQuantity1`, `RewardChoiceItemID2`, `RewardChoiceItemQuantity2`, `RewardChoiceItemID3`, `RewardChoiceItemQuantity3`, `RewardChoiceItemID4`, `RewardChoiceItemQuantity4`, `RewardChoiceItemID5`, `RewardChoiceItemQuantity5`, `RewardChoiceItemID6`, `RewardChoiceItemQuantity6`, `POIContinent`, `POIx`, `POIy`, `POIPriority`, `RewardTitle`, `RewardTalents`, `RewardArenaPoints`, `RewardFactionID1`, `RewardFactionValue1`, `RewardFactionOverride1`, `RewardFactionID2`, `RewardFactionValue2`, `RewardFactionOverride2`, `RewardFactionID3`, `RewardFactionValue3`, `RewardFactionOverride3`, `RewardFactionID4`, `RewardFactionValue4`, `RewardFactionOverride4`, `RewardFactionID5`, `RewardFactionValue5`, `RewardFactionOverride5`, `TimeAllowed`, `AllowableRaces`, `LogTitle`, `LogDescription`, `QuestDescription`, `AreaDescription`, `QuestCompletionLog`, `RequiredNpcOrGo1`, `RequiredNpcOrGo2`, `RequiredNpcOrGo3`, `RequiredNpcOrGo4`, `RequiredNpcOrGoCount1`, `RequiredNpcOrGoCount2`, `RequiredNpcOrGoCount3`, `RequiredNpcOrGoCount4`, `RequiredItemId1`, `RequiredItemId2`, `RequiredItemId3`, `RequiredItemId4`, `RequiredItemId5`, `RequiredItemId6`, `RequiredItemCount1`, `RequiredItemCount2`, `RequiredItemCount3`, `RequiredItemCount4`, `RequiredItemCount5`, `RequiredItemCount6`, `Unknown0`, `ObjectiveText1`, `ObjectiveText2`, `ObjectiveText3`, `ObjectiveText4`, `VerifiedBuild`) VALUES (1660058,2,-1,5,12,0,0,0,0,0,0,0,5,260,0,0,0,0,0,0,8,0,2302045,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,72,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'Worm-Eaten Apple','Find and destroy the kobold warrens around Goldshire.','Whispers. They think no one\'s noticed,but I sleep with my ear to the floor! Ha!$b$bThey dig and dig and dig. They\'ll strike when you least expect it... unless you do something first.$b$bKobolds and kobolds and more kobolds. Under our feet! Watch where you step. Mind your footing!$b$bFind their warrens and set them alight. Crush them. With a big, heavy hammer!$b$b<She laughs to herself, then stares into the middle distance.>','','Return to Clara the Mad.',-2300579,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'Kobold Warren Destroyed',NULL,NULL,NULL,0);

-- "Kobold Warren" gameobject (2300579, GOOBER, quest-gated to 1660058)
INSERT IGNORE INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild`) VALUES (2300579,10,1017889,'Kobold Warren','','','',0.3,0,1660058,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'SmartGameObjectAI','',0);

-- All 18 spawns around Goldshire, guids remapped into our own 9780xxx
-- sequence (source was PR #5423's 9001xxx).
INSERT IGNORE INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `ScriptName`, `VerifiedBuild`, `Comment`) VALUES
(9780512,2300579,0,0,0,1,1,-9633.58,133.12,45.926,2.88376,0,0,0.991702,0.128559,120,100,1,'',0,'Goldshire WB1 content'),
(9780513,2300579,0,0,0,1,1,-9712.29,209.46,49.913,5.28372,0,0,0.47919,-0.877711,120,100,1,'',0,'Goldshire WB1 content'),
(9780514,2300579,0,0,0,1,1,-9723.87,171.29,50.955,1.4005,0,0,0.644408,0.764682,120,100,1,'',0,'Goldshire WB1 content'),
(9780515,2300579,0,0,0,1,1,-9721.55,-47.32,37.418,3.80046,0,0,0.946226,-0.323505,120,100,1,'',0,'Goldshire WB1 content'),
(9780516,2300579,0,0,0,1,1,-9730.81,-12.62,36.739,6.20042,0,0,0.0413727,-0.999144,120,100,1,'',0,'Goldshire WB1 content'),
(9780517,2300579,0,0,0,1,1,-9763.22,-16.09,31.695,2.31719,0,0,0.916241,0.400627,120,100,1,'',0,'Goldshire WB1 content'),
(9780518,2300579,0,0,0,1,1,-9772.48,32.49,33.784,4.71715,0,0,0.705421,-0.708788,120,100,1,'',0,'Goldshire WB1 content'),
(9780519,2300579,0,0,0,1,1,-9638.21,67.19,61.117,0.83393,0,0,0.404985,0.914323,120,100,1,'',0,'Goldshire WB1 content'),
(9780520,2300579,0,0,0,1,1,-9744.7,67.19,39.95,3.23389,0,0,0.998935,-0.0461301,120,100,1,'',0,'Goldshire WB1 content'),
(9780521,2300579,0,0,0,1,1,-9749.33,129.65,49.4,5.63385,0,0,0.318996,-0.947756,120,100,1,'',0,'Goldshire WB1 content'),
(9780522,2300579,0,0,0,1,1,-9763.22,67.19,38.946,1.75062,0,0,0.767742,0.640759,120,100,1,'',0,'Goldshire WB1 content'),
(9780523,2300579,0,0,0,1,1,-9797.95,129.65,49.78,4.15058,0,0,0.875419,-0.483364,120,100,1,'',0,'Goldshire WB1 content'),
(9780524,2300579,0,0,0,1,1,-9661.36,105.36,45.527,0.26736,0,0,0.13328,0.991078,120,100,1,'',0,'Goldshire WB1 content'),
(9780525,2300579,0,0,0,1,1,-9714.61,25.55,40.946,2.66732,0,0,0.972014,0.234922,120,100,1,'',0,'Goldshire WB1 content'),
(9780526,2300579,0,0,0,1,1,-9698.4,164.35,50.249,5.06728,0,0,0.57119,-0.820818,120,100,1,'',0,'Goldshire WB1 content'),
(9780527,2300579,0,0,0,1,1,-9647.47,202.52,49.271,1.18405,0,0,0.558042,0.829812,120,100,1,'',0,'Goldshire WB1 content'),
(9780528,2300579,0,0,0,1,1,-9714.61,112.3,46.251,3.58401,0,0,0.975633,-0.219409,120,100,1,'',0,'Goldshire WB1 content'),
(9780529,2300579,0,0,0,1,1,-9698.4,63.72,56.651,5.98397,0,0,0.14905,-0.98883,120,100,1,'',0,'Goldshire WB1 content');
