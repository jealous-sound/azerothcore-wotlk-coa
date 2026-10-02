-- The Ruins of Northshire (1660002): Flags was 0, RequiredNpcOrGo1 was 0
-- (quest had no objective at all -- would auto-complete), QuestDescription
-- was NULL, QuestCompletionLog stale ("Speak with Sister Alma" -- the real
-- target is a new "spectral priestess" NPC, entry 161714, not yet built).
UPDATE `quest_template` SET
  `Flags`=8,
  `RequiredNpcOrGo1`=161714, `RequiredNpcOrGoCount1`=1,
  `QuestDescription`='A visit to the dungeon where the abbess was tried might shed some light...$b$bAccording to the book, the now-abandoned town of Northshire held a secret entrance to the Inquisitorial Dungeon.$b$bI\'ll mark it on your map, but tread carefully. No one\'s set foot in that place for years... I\'d rather not imagine what vermin have claimed its ruins.',
  `QuestCompletionLog`='Speak with the spectral priestess.'
WHERE `ID`=1660002;

-- The "spectral priestess" / "Ancient Priestess of Northshire" turned out
-- to be Sister Alma (161702, already spawned, already the questender for
-- this quest per creature_questender) -- not a new NPC. Fixed
-- RequiredNpcOrGo1 to point to her real entry instead of the invented/
-- never-built 161714.
UPDATE `quest_template` SET `RequiredNpcOrGo1`=161702 WHERE `id`=1660002;

-- "Speak to" (SPEAKTO) quest credit is dead code in this codebase --
-- Player::TalkedToCreature() is never actually called (the one call site,
-- NPCHandler.cpp:190, is commented out) -- so simply opening gossip with
-- her would never satisfy this objective on its own. Added a SmartAI
-- gossip-hello credit instead (same pattern as Stay a While's Arcane
-- Projection of Aliscar): granting KILLEDMONSTER-style credit for her own
-- entry as soon as she's interacted with.
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=161702;
DELETE FROM `smart_scripts` WHERE `entryorguid`=161702 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(161702,0,0,0,64,0,100,0,0,0,0,0, 33,161702,0,0,0,0,0, 7,0,0,0,0,0,'Sister Alma - on gossip hello, credit The Ruins of Northshire (speak-to objective)');
