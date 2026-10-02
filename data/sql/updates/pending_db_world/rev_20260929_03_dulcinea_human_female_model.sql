-- Dulcinea (162800) was wearing a broken/wrong display (510). Set to 25554,
-- confirmed by the user in-game as a correct human female model on this
-- client. Note for future NPC-display fixes on this fork: vanilla WotLK
-- display-ID meanings do NOT reliably hold here (Ascension's client DBC
-- remaps them) -- 4176 "Gina Lang" (Undead), 13 "Peasant Woman" (correct
-- race but wrong-era low-poly), 4888 and 2554 were all tried and rejected
-- first. Prefer pulling a working ID via `.npc info` on an in-game reference
-- NPC over trusting a stock-WotLK display-ID lookup. See [[coa_codebase_gotchas]].
UPDATE `creature_template_model` SET `CreatureDisplayID`=25554 WHERE `CreatureID`=162800;
