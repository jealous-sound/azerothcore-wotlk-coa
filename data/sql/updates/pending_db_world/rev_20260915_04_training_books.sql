-- Class-training companions use the existing level/spec-aware progression service.
UPDATE `creature_template` SET `npcflag` = `npcflag` | 1, `ScriptName` = 'npc_ascension_training_book'
WHERE `entry` IN (73427, 75115, 75119, 75136, 75137, 75139, 80054, 80890, 98499, 98500, 98501,
    98598, 108312, 108586, 108587, 988501);

DELETE FROM `npc_text` WHERE `ID` = 900370;
INSERT INTO `npc_text` (`ID`, `text0_0`, `Probability0`) VALUES
(900370, 'Class abilities advance automatically. Restore missing spells here; select talents in Character Advancement.', 1);
