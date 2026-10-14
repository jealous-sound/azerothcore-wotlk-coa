-- Pale Reach chain start in Stormwind, transcribed from in-game captures (2026-10-04):
-- Pale Reach's Calling, The House of Nobles (voiced council scene), The Family Estate, Voices at the North Gate.
-- Quest and NPC ids and quest texts come from the Exiles DB export (2026-09-13). Its 652xxx/653xxx model ids exist
-- in no client or server DBC (Ascension streamed them at runtime), so stock stand-in displays are used.
INSERT INTO `creature_template` (`entry`, `name`, `subname`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `rank`, `unit_class`, `unit_flags`, `type`, `AIName`, `MovementType`, `HealthModifier`, `flags_extra`, `ScriptName`, `VerifiedBuild`)
VALUES
(164001, 'Lord Servin Darengar', '', 30, 30, 12, 2, 1, 1.14286, 0, 1, 768, 7, 'SmartAI', 0, 1, 2, '', 0),
(164003, 'Lord Arathon Darengar', '', 30, 30, 12, 2, 1, 1.14286, 0, 1, 768, 7, '', 0, 1, 2, '', 0),
(164210, 'Cadmilla', '', 30, 30, 12, 2, 1, 1.14286, 0, 1, 768, 7, '', 0, 1, 2, '', 0),
(164211, 'Lord Aldous Bruck', '', 30, 30, 12, 0, 1, 1.14286, 0, 1, 768, 7, '', 0, 1, 2, '', 0),
(164212, 'Countess Cecilia Clessington', '', 30, 30, 12, 0, 1, 1.14286, 0, 1, 768, 7, '', 0, 1, 2, '', 0),
(164213, 'Lord Derin Renvar', '', 30, 30, 12, 0, 1, 1.14286, 0, 1, 768, 7, '', 0, 1, 2, '', 0),
(164215, 'Lord Sermon Spada', '', 30, 30, 12, 2, 1, 1.14286, 0, 1, 768, 7, '', 0, 1, 2, '', 0),
(164217, 'Lord Broldin Feandor', '', 30, 30, 12, 0, 1, 1.14286, 0, 1, 768, 7, '', 0, 1, 2, '', 0),
(164218, 'Lady Asia Mortel', '', 30, 30, 12, 0, 1, 1.14286, 0, 1, 768, 7, '', 0, 1, 2, '', 0),
(164490, 'Stormwind Lumberjack', '', 10, 10, 12, 0, 1, 1.14286, 0, 1, 768, 7, '', 0, 1, 2, '', 0),
(164980, 'Guard Thair Spada', '', 30, 30, 12, 2, 1, 1.14286, 0, 1, 768, 7, '', 0, 1, 2, '', 0),
(164981, 'Baldur', '', 15, 15, 12, 1, 1, 1.14286, 0, 1, 768, 7, '', 0, 1, 2, 'npc_north_gate_citizen', 0),
(164982, 'Jelena Goret', '', 15, 15, 12, 1, 1, 1.14286, 0, 1, 768, 7, '', 0, 1, 2, 'npc_north_gate_citizen', 0),
(164983, 'Gareth Lindgren', '', 15, 15, 12, 1, 1, 1.14286, 0, 1, 768, 7, '', 0, 1, 2, 'npc_north_gate_citizen', 0),
(164984, 'Sereniel', '', 15, 15, 12, 1, 1, 1.14286, 0, 1, 768, 7, '', 0, 1, 2, 'npc_north_gate_citizen', 0),
(164985, 'Brother Caesar', '', 15, 15, 12, 1, 1, 1.14286, 0, 1, 768, 7, '', 0, 1, 2, 'npc_north_gate_citizen', 0),
(9920020, 'Concerned Citizen Informed', '', 1, 1, 35, 0, 1, 1.14286, 0, 1, 33554434, 10, '', 0, 1, 128, '', 0)
ON DUPLICATE KEY UPDATE `name` = VALUES(`name`), `subname` = VALUES(`subname`), `minlevel` = VALUES(`minlevel`),
  `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `npcflag` = VALUES(`npcflag`),
  `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `rank` = VALUES(`rank`),
  `unit_class` = VALUES(`unit_class`), `unit_flags` = VALUES(`unit_flags`), `type` = VALUES(`type`),
  `AIName` = VALUES(`AIName`), `MovementType` = VALUES(`MovementType`), `HealthModifier` = VALUES(`HealthModifier`),
  `flags_extra` = VALUES(`flags_extra`), `ScriptName` = VALUES(`ScriptName`), `VerifiedBuild` = VALUES(`VerifiedBuild`);
DELETE FROM `creature_template_model` WHERE `CreatureID` IN (164001, 164003, 164210, 164211, 164212, 164213, 164215, 164217, 164218, 164490, 164980, 164981, 164982, 164983, 164984, 164985, 9920020);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES
(164001, 0, 2051, 1, 1, 0),
(164003, 0, 49, 1, 1, 0),
(164210, 0, 50, 1, 1, 0),
(164211, 0, 1758, 1, 1, 0),
(164212, 0, 8769, 1, 1, 0),
(164213, 0, 5075, 1, 1, 0),
(164215, 0, 4885, 1, 1, 0),
(164217, 0, 4998, 1, 1, 0),
(164218, 0, 5129, 1, 1, 0),
(164490, 0, 310, 1, 1, 0),
(164980, 0, 3167, 1, 1, 0),
(164981, 0, 1892, 1, 1, 0),
(164982, 0, 7330, 1, 1, 0),
(164983, 0, 1423, 1, 1, 0),
(164984, 0, 2865, 1, 1, 0),
(164985, 0, 3277, 1, 1, 0),
(9920020, 0, 11686, 1, 1, 0);
DELETE FROM `creature_text` WHERE `CreatureID` IN (164001, 164217, 164218, 164981, 164985);
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(164001, 0, 0, 'I believe I have made my stance clear. Trolls, bandits, Ealdir... what more must happen before we resolve to intervene?', 12, 0, 100, 0, 0, 215200, 0, 0, 'Lord Servin Darengar - House of Nobles'),
(164217, 0, 0, 'And we, I believe, have made ours clear, Lord Darengar.', 12, 0, 100, 0, 0, 215201, 0, 0, 'Lord Broldin Feandor - House of Nobles'),
(164217, 1, 0, 'Even if there were unanimity, this council cannot simply march the army into Scadeald without the consent of House Coldmere.', 12, 0, 100, 0, 0, 215202, 0, 0, 'Lord Broldin Feandor - House of Nobles'),
(164218, 0, 0, 'And do remember that it is the Coldmeres, and not yourself, who rule Scadeald.', 12, 0, 100, 0, 0, 215203, 0, 0, 'Lady Asia Mortel - House of Nobles'),
(164001, 1, 0, 'If we do nothing, there will be nothing left to rule! Tell me, do we only help the drowning if they still have the voice to beg?', 12, 0, 100, 0, 0, 215204, 0, 0, 'Lord Servin Darengar - House of Nobles'),
(164001, 2, 0, 'Lady Serenya''s pride will be our undoing. We must--!', 12, 0, 100, 0, 0, 215205, 0, 0, 'Lord Servin Darengar - House of Nobles'),
(164217, 2, 0, 'You are wasting your time barking up the wrong tree, Lord Darengar. Our laws exist for a reason. This session is adjourned.', 12, 0, 100, 0, 0, 215206, 0, 0, 'Lord Broldin Feandor - House of Nobles'),
(164985, 0, 0, 'The Light guides us even in times of uncertainty. I trust that.', 12, 0, 100, 0, 0, 0, 0, 0, 'Brother Caesar - informed'),
(164981, 0, 0, 'At the rate those lot take to lift a finger, I''ll starve to death first...', 12, 0, 100, 0, 0, 0, 0, 0, 'Baldur - informed');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 164001 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 16400100 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `event_param6`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(164001, 0, 0, 0, 19, 0, 100, 0, 175001, 0, 0, 0, 0, 0, 80, 16400100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Lord Servin Darengar - On quest The House of Nobles accepted - Start council scene'),
(164001, 0, 1, 0, 64, 0, 100, 0, 0, 0, 0, 0, 0, 0, 80, 16400100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Lord Servin Darengar - On gossip hello while The House of Nobles is incomplete - Restart council scene'),
(16400100, 9, 0, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'House of Nobles scene - Lord Servin Darengar line 0'),
(16400100, 9, 1, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 164217, 30, 0, 0, 0, 0, 0, 0, 'House of Nobles scene - Lord Broldin Feandor line 0'),
(16400100, 9, 2, 0, 0, 0, 100, 0, 7000, 7000, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 19, 164217, 30, 0, 0, 0, 0, 0, 0, 'House of Nobles scene - Lord Broldin Feandor line 1'),
(16400100, 9, 3, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 164218, 30, 0, 0, 0, 0, 0, 0, 'House of Nobles scene - Lady Asia Mortel line 0'),
(16400100, 9, 4, 0, 0, 0, 100, 0, 9000, 9000, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'House of Nobles scene - Lord Servin Darengar line 1'),
(16400100, 9, 5, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'House of Nobles scene - Lord Servin Darengar line 2'),
(16400100, 9, 6, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 19, 164217, 30, 0, 0, 0, 0, 0, 0, 'House of Nobles scene - Lord Broldin Feandor line 2'),
(16400100, 9, 7, 0, 0, 0, 100, 0, 12000, 12000, 0, 0, 0, 0, 15, 175001, 0, 0, 0, 0, 0, 18, 40, 0, 0, 0, 0, 0, 0, 0, 'House of Nobles scene - Session adjourned - Complete for nearby players');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceEntry` = 164001;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 2, 164001, 0, 0, 9, 0, 175001, 0, 0, 0, 0, 0, '', 'Restart the council scene only while the quest is taken');
DELETE FROM `npc_text` WHERE `ID` BETWEEN 9920030 AND 9920034;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `Probability0`, `VerifiedBuild`) VALUES
(9920032, 'I was meant to meet my cousin from Ironforge in Dun Kazad for roast goat... but with the mess on the Kingsway, there''s no getting through. I''m starting to feel my stomach more than my impatience, and that is a very bad sign.$B$BAnd if they expect me to ride that gnomish contraption they call a tram, they''ve got another thing coming. That thing is crawling with rats, and I don''t trust it one bit. Not. One. Bit!', 'I was meant to meet my cousin from Ironforge in Dun Kazad for roast goat... but with the mess on the Kingsway, there''s no getting through. I''m starting to feel my stomach more than my impatience, and that is a very bad sign.$B$BAnd if they expect me to ride that gnomish contraption they call a tram, they''ve got another thing coming. That thing is crawling with rats, and I don''t trust it one bit. Not. One. Bit!', 1, 0),
(9920034, 'Ever since they enlisted my husband to defend the border at Andrastre, I''ve had no one to travel with...$B$BI need to reach Shadewell, but I don''t dare go alone. I should have gone with him...', 'Ever since they enlisted my husband to defend the border at Andrastre, I''ve had no one to travel with...$B$BI need to reach Shadewell, but I don''t dare go alone. I should have gone with him...', 1, 0),
(9920031, 'The mineral shipment from the Dunshire mine still hasn''t arrived. I want to go and demand an answer in person, but I''ve been warned the road is dangerous.', 'The mineral shipment from the Dunshire mine still hasn''t arrived. I want to go and demand an answer in person, but I''ve been warned the road is dangerous.', 1, 0),
(9920033, 'I should never have left Namarien Sanctuary in times like these. This city is in absolute chaos...', 'I should never have left Namarien Sanctuary in times like these. This city is in absolute chaos...', 1, 0),
(9920030, 'Every year, I make the pilgrimage to Hallow Wind Abbey to pay tribute to the Holy Light. I will not return to Northshire without fulfilling that duty.', 'Every year, I make the pilgrimage to Hallow Wind Abbey to pay tribute to the Holy Light. I will not return to Northshire without fulfilling that duty.', 1, 0);
INSERT INTO `quest_template` (`ID`, `QuestType`, `QuestLevel`, `MinLevel`, `QuestSortID`, `RewardXPDifficulty`, `RewardMoney`, `StartItem`, `Flags`, `RewardItem1`, `RewardAmount1`, `RewardItem2`, `RewardAmount2`, `LogTitle`, `LogDescription`, `QuestDescription`, `QuestCompletionLog`, `RequiredNpcOrGo1`, `RequiredNpcOrGoCount1`, `ObjectiveText1`, `RequiredItemId1`, `RequiredItemCount1`, `VerifiedBuild`)
VALUES
(175000, 2, -1, 18, 10302, 1, 250, 0, 8, 0, 0, 0, 0, 'Pale Reach''s Calling', 'Find the stairs in the Stormwind Keep courtyard and speak with Lord Servin Darengar in the Petitioner''s Chamber.', 'Are you bound for the Pale Reach?$b$bMy lord, Lord Servin Darengar, charged me with recruiting all the help I could find in these lands. And judging by your appearance, you could make a splendid addition.$b$bYou see, my lord''s homeland, the province of Scadeald, is facing its darkest hour: Frostmane trolls descending from the mountains, bandits waylaying travelers in broad daylight... and the savage Ealdir, of course. The worst of the bunch.$b$bReport to Lord Darengar in the House of Nobles, within Stormwind Keep, beneath the cloister courtyard. Tell him I sent you.$b$bHe will amply reward any aid you can provide.', 'Speak with Lord Servin Darengar', 0, 0, '', 0, 0, 0),
(175001, 2, -1, 18, 10302, 2, 350, 0, 8, 0, 0, 0, 0, 'The House of Nobles', 'Attend the gathering of the nobles.', '<The tolling of bells echoes through the House of Nobles. Lord Servin Darengar responds with a sigh, straightening his back in his uncomfortable seat.>$b$bIt seems our respite is at an end.$b$bStand aside for a moment, while we nobles finish debating the fate of my beloved province.$b$bObserve, listen, and judge. Then you shall understand my despair.', 'Speak with Lord Servin Darengar', 0, 0, '', 0, 0, 0),
(175002, 2, -1, 18, 10302, 1, 250, 559850, 8, 0, 0, 0, 0, 'The Family Estate', 'Report to Arathon Darengar at the Darengar Estate in Scadeald.', 'If Stormwind''s precious laws condemn the House of Nobles to inaction, then so be it. They were not my only recourse; as far as I am aware, nothing forbids me from recruiting the aid of mercenaries and adventurers in a private capacity.$b$bAnd so, I shall. Consider yourself ''hired.''$b$bTake my seal. My son, Arathon, remained in Scadeald to try and alleviate the province''s woes. Present yourself to him. Offer him your services. I will endeavor to send more men as soon as possible; for now, you will have to suffice.', 'Talk to Cadmilla', 0, 0, '', 559850, 1, 0),
(175198, 2, -1, 18, 10302, 2, 350, 0, 8, 375250, 100, 1397884, 1, 'Voices at the North Gate', 'Inform the concerned citizens in Stormwind''s Dwarven District that the House of Nobles has heard their pleas.', 'I saw you speaking with Lord Darengar. The rabble did not send you, I trust? <Sermon Spada gives you a haughty, appraising look.>$b$bBut listen to me. Of course they did not. You are something closer to a mercenary, I suppose.$b$bAs it happens, we have an angry mob at Stormwind''s north gate, demanding answers about the situation in Scadeald. Nothing we have tried so far has managed to calm them, but it occurs to me that the words of someone of their “station” may serve better than an official document.$b$bSpeak with them. Intimidate them, if necessary. And when you are finished, find my nephew, Thair Spada, who stands guard at the gate.', 'Speak with Guard Thair Spada.', 9920020, 5, 'Concerned citizens informed', 0, 0, 0)
ON DUPLICATE KEY UPDATE `QuestType` = VALUES(`QuestType`), `QuestLevel` = VALUES(`QuestLevel`), `MinLevel` = VALUES(`MinLevel`),
  `QuestSortID` = VALUES(`QuestSortID`), `RewardXPDifficulty` = VALUES(`RewardXPDifficulty`), `RewardMoney` = VALUES(`RewardMoney`),
  `StartItem` = VALUES(`StartItem`), `Flags` = VALUES(`Flags`), `RewardItem1` = VALUES(`RewardItem1`),
  `RewardAmount1` = VALUES(`RewardAmount1`), `RewardItem2` = VALUES(`RewardItem2`), `RewardAmount2` = VALUES(`RewardAmount2`),
  `LogTitle` = VALUES(`LogTitle`), `LogDescription` = VALUES(`LogDescription`), `QuestDescription` = VALUES(`QuestDescription`),
  `QuestCompletionLog` = VALUES(`QuestCompletionLog`), `RequiredNpcOrGo1` = VALUES(`RequiredNpcOrGo1`), `RequiredNpcOrGoCount1` = VALUES(`RequiredNpcOrGoCount1`),
  `ObjectiveText1` = VALUES(`ObjectiveText1`), `RequiredItemId1` = VALUES(`RequiredItemId1`), `RequiredItemCount1` = VALUES(`RequiredItemCount1`),
  `VerifiedBuild` = VALUES(`VerifiedBuild`);
DELETE FROM `quest_template_addon` WHERE `ID` IN (175000, 175001, 175002, 175198);
INSERT INTO `quest_template_addon` (`ID`, `PrevQuestID`, `SpecialFlags`, `ProvidedItemCount`) VALUES
(175000, 0, 0, 0),
(175001, 0, 2, 0),
(175002, 175001, 0, 1),
(175198, 175001, 0, 0);
DELETE FROM `quest_offer_reward` WHERE `ID` IN (175000, 175002, 175198);
INSERT INTO `quest_offer_reward` (`ID`, `RewardText`, `VerifiedBuild`) VALUES
(175000, 'Sent by one of my knights, you say?$B$BVery well. In these matters, I trust their judgment more than my own. I accept your services as... a squire? A mercenary?$B$B<Servin Darengar waves a hand, as if dismissing the matter of titles.>$B$BForgive me, I find it difficult to think clearly. I''ve been shut in here for hours, trying to convince the House of Nobles to send the army to Scadeald... with little success.', 0),
(175002, '<The handmaid clenches her fists against her apron, her knuckles white.>$B$BAre you here for my lord Arathon?$B$BYou are too late, then. Those brutes from Graysky, sent by that magpie Lady Serenya, arrested him hours ago. They wouldn''t even let him put on his boots.$B$BAnd now they are turning the manor upside down! Can you believe it? They have accused the Darengar family of high treason!', 0),
(175198, 'My uncle...? There was no need to send anyone. I am performing my duty as expected.$B$BThe situation at the gate is under control... though the word "control" is starting to feel generous.$B$BIn any case, thank you for taking the trouble to lend a hand in all this.$B$BI hope my uncle and the House of Nobles deign to act soon.', 0);
DELETE FROM `quest_request_items` WHERE `ID` = 175002;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `CompletionText`, `VerifiedBuild`) VALUES
(175002, 0, 0, 'Welcome to the Darengar Family Estate.', 0);
DELETE FROM `creature_queststarter` WHERE `quest` IN (175000, 175001, 175002, 175198);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(164000, 175000),
(164001, 175001),
(164001, 175002),
(164215, 175198);
DELETE FROM `creature_questender` WHERE `quest` IN (175000, 175001, 175002, 175198);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(164001, 175000),
(164001, 175001),
(164210, 175002),
(164980, 175198);
DELETE FROM `creature` WHERE `guid` BETWEEN 9920200 AND 9920299;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `MovementType`, `Comment`) VALUES
(9920200, 164001, 0, 0, 0, 1, 1, -8323.28, 292.67365, 127.579, 5.4567847, 300, 0, 0, 'Lord Servin Darengar, council seat (captured in game)'),
(9920201, 164211, 0, 0, 0, 1, 1, -8322.774, 288.70053, 127.57895, 0.88497573, 300, 0, 0, 'Lord Aldous Bruck, council seat (captured in game)'),
(9920202, 164218, 0, 0, 0, 1, 1, -8320.684, 286.15857, 127.57896, 0.9368185, 300, 0, 0, 'Lady Asia Mortel, council seat (captured in game)'),
(9920203, 164212, 0, 0, 0, 1, 1, -8317.406, 285.51804, 127.71165, 2.1777396, 300, 0, 0, 'Countess Cecilia Clessington, council seat (captured in game)'),
(9920204, 164217, 0, 0, 0, 1, 1, -8317.615, 288.67917, 127.57849, 3.9284072, 300, 0, 0, 'Lord Broldin Feandor, council seat (captured in game)'),
(9920205, 164213, 0, 0, 0, 1, 1, -8318.61, 290.36197, 126.93257, 2.9191692, 300, 0, 0, 'Lord Derin Renvar, standing (captured in game)'),
(9920206, 164215, 0, 0, 0, 1, 1, -8316.5, 291.6, 126.93, 3.5, 300, 0, 0, 'Lord Sermon Spada (estimated position) (captured in game)'),
(9920207, 164985, 0, 0, 0, 1, 1, -8351.108, 643.58215, 95.35577, 3.079374, 300, 0, 0, 'Brother Caesar, north gate (captured in game)'),
(9920208, 164983, 0, 0, 0, 1, 1, -8349.853, 645.7321, 95.502396, 0.3760355, 300, 0, 0, 'Gareth Lindgren, north gate (captured in game)'),
(9920209, 164981, 0, 0, 0, 1, 1, -8345.105, 641.38916, 95.34406, 1.997884, 300, 0, 0, 'Baldur, north gate (captured in game)'),
(9920210, 164984, 0, 0, 0, 1, 1, -8354.477, 636.42035, 95.249374, 1.0035762, 300, 0, 0, 'Sereniel, north gate (captured in game)'),
(9920211, 164982, 0, 0, 0, 1, 1, -8373.633, 647.41187, 95.57947, 5.643712, 300, 0, 0, 'Jelena Goret, north gate (captured in game)'),
(9920212, 164980, 0, 0, 0, 1, 1, -8345.57, 646.98, 95.4, 3.6, 300, 0, 0, 'Guard Thair Spada (position from map coordinates, Z and facing estimated) (captured in game)'),
(9920213, 68, 0, 0, 0, 1, 1, -8380.038, 663.9207, 95.28752, 4.461685, 300, 0, 0, 'Stormwind City Guard, north gate (captured in game)'),
(9920214, 68, 0, 0, 0, 1, 1, -8335.09, 656.0635, 95.724495, 2.3599546, 300, 0, 0, 'Stormwind City Guard, north gate (captured in game)'),
(9920215, 68, 0, 0, 0, 1, 1, -8341.817, 664.8034, 95.724495, 5.4292893, 300, 0, 0, 'Stormwind City Guard, north gate (captured in game)'),
(9920216, 164490, 0, 0, 0, 1, 1, -8280.616, 688.4052, 84.53471, 4.793913, 300, 0, 0, 'Stormwind Lumberjack (captured in game)'),
(9920217, 164490, 0, 0, 0, 1, 1, -8252.148, 761.82623, 78.57619, 0.31871375, 300, 0, 0, 'Stormwind Lumberjack (captured in game)'),
(9920218, 164490, 0, 0, 0, 1, 1, -8210.388, 756.6324, 69.4144, 2.376458, 300, 0, 0, 'Stormwind Lumberjack (captured in game)');
DELETE FROM `creature_addon` WHERE `guid` BETWEEN 9920200 AND 9920299;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `visibilityDistanceType`, `auras`) VALUES
(9920200, 0, 0, 5, 1, 0, 0, ''),
(9920201, 0, 0, 5, 1, 0, 0, ''),
(9920202, 0, 0, 5, 1, 0, 0, ''),
(9920203, 0, 0, 5, 1, 0, 0, ''),
(9920204, 0, 0, 5, 1, 0, 0, ''),
(9920209, 0, 0, 1, 1, 0, 0, ''),
(9920216, 0, 0, 0, 1, 234, 0, ''),
(9920217, 0, 0, 0, 1, 234, 0, ''),
(9920218, 0, 0, 0, 1, 234, 0, '');
