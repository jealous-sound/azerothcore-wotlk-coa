-- Skeletal Smith (Animate: Skeletal Smith) is a repairer: right-clicking it opens a vendor window with
-- no goods, only the repair buttons. It was inserted without an npcflag, so it could not be clicked.
-- npc_ascension_necromancer opens the window itself, as an empty goods list has no gossip option.
UPDATE `creature_template` SET `npcflag` = `npcflag` | 4224 WHERE `entry` = 50261;
