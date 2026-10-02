-- Knowledge Corrupts (1660001) text/flags/money fields.
UPDATE `quest_template` SET
  `Flags`=8,
  `RewardMoney`=15,
  `QuestDescription`='Go on, take a look.$b$b<Moroi gestures toward the book holding his attention: a thin, battered manuscript, not unlike its reader.>$b$bI stumbled across it in the library; it\'s not listed in the abbey\'s records. I think it\'s some sort of chronicle about an old abbess accused of heresy. In my haste, I\'ve misplaced several pages. I\'ve been trying to piece them back together, but some are still missing.$b$bCould you have a look around the abbey and recover the pages I lost? This is a banned book; if anyone finds out I\'ve been poking my nose where it doesn\'t belong, they\'ll ship me back home... to my sister.$b$bDo me a mercy, will you?',
  `QuestCompletionLog`='Return to Moroi.'
WHERE `ID`=1660001;

-- Switched the Lost Pages mechanic from silent GO-click credit to real
-- item pickup, now that Supply Run proved the working cast-bar pattern
-- (Data0=43 lock + empty castBarCaption + SmartGameObjectAI). Each of the
-- 4 Lost Page gameobjects (2300500/2300503/2300504/2300505) now grants its
-- matching item (559130-133) directly via SmartAI on use, and the quest
-- objective switched from RequiredNpcOrGo (GO click credit) to
-- RequiredItemId (must actually be carrying the 4 items to turn in).
UPDATE `gameobject_template` SET `Data0`=43, `Data3`=2000, `castBarCaption`='', `AIName`='SmartGameObjectAI' WHERE `entry` IN (2300500,2300503,2300504,2300505);

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (2300500,2300503,2300504,2300505) AND `source_type`=1;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2300500,1,0,0,70,0,100,0,2,0,0,0, 56,559130,1,0,0,0,0, 7,0,0,0,0,0,'Lost Page I - on use, give item'),
(2300503,1,0,0,70,0,100,0,2,0,0,0, 56,559131,1,0,0,0,0, 7,0,0,0,0,0,'Lost Page II - on use, give item'),
(2300504,1,0,0,70,0,100,0,2,0,0,0, 56,559132,1,0,0,0,0, 7,0,0,0,0,0,'Lost Page III - on use, give item'),
(2300505,1,0,0,70,0,100,0,2,0,0,0, 56,559133,1,0,0,0,0, 7,0,0,0,0,0,'Lost Page IV - on use, give item');

UPDATE `quest_template` SET
  `RequiredNpcOrGo1`=0,`RequiredNpcOrGoCount1`=0,`RequiredNpcOrGo2`=0,`RequiredNpcOrGoCount2`=0,`RequiredNpcOrGo3`=0,`RequiredNpcOrGoCount3`=0,`RequiredNpcOrGo4`=0,`RequiredNpcOrGoCount4`=0,
  `RequiredItemId1`=559130,`RequiredItemCount1`=1,
  `RequiredItemId2`=559131,`RequiredItemCount2`=1,
  `RequiredItemId3`=559132,`RequiredItemCount3`=1,
  `RequiredItemId4`=559133,`RequiredItemCount4`=1
WHERE `id`=1660001;
