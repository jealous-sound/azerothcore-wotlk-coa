-- Golemagg's Massive Stomp (2105817) drops the same Magmadar-head ground-fire puddle under a
-- random player (spell_golemagg_massive_stomp_ground_fire_coa, boss_golemagg_coa.cpp); designed
-- from the user's own CoA play memory, no corpus log confirms it (docs/coa/molten-core.md).
DELETE FROM `spell_script_names` WHERE `spell_id` = 2105817 AND `ScriptName` = 'spell_golemagg_massive_stomp_ground_fire_coa';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(2105817, 'spell_golemagg_massive_stomp_ground_fire_coa');
