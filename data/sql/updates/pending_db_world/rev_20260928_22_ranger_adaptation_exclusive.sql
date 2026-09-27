-- The Ranger's two "Adaptation" buff pairs are each one buff with a single-target and a raid-wide
-- version, and every single-target rank's own tooltip says "Does not stack with other similar effects"
-- (Footpad's Adaptation 523489-523493). At max rank 523493 grants exactly what Greater Footpad's
-- Adaptation 523513 grants (armor 285 and 12 to every primary attribute), and 803666 (Woodsman's
-- Adaptation rank 7) exactly what Greater Woodsman's Adaptation 680294 grants (232 melee and ranged
-- attack power), so casting both on the same target doubled the buff. #5000 and #5011 are the two halves
-- of that: Woodsman's plus Greater, and Footpad's plus Greater.
--
-- "Does not stack" is a rule about the buffed target, not the caster, so each pair is one spell_group with
-- SPELL_GROUP_STACK_RULE_EXCLUSIVE (1), the same rule rev_20260916_61_rite_buffs_exclusive.sql uses for the
-- Rites: Aura::CanStackWith refuses any two members regardless of caster, Unit::_RemoveNoStackAurasDueToAura
-- removes the existing one when a new one lands, and a raid-wide cast does the same on every target it
-- reaches. SpellMgr resolves both sides through the first rank of their chain (spell_ranks 523489 and 800266),
-- so every rank of the single-target buff, including 523493 and 803666, is covered by one row and replaces or
-- is replaced by the Greater version and by another Ranger's copy. Footpad's grants armor and stats and
-- Woodsman's grants attack power, so the two pairs are separate groups and stay independent.
--
-- The earlier revision of this file bound aura_ascension_ranger_adaptation, which only knew the rank 1 ids and
-- only removed the rival cast by the buffed unit itself; that script is gone, so its rows are removed here.
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'aura_ascension_ranger_adaptation';
DELETE FROM `spell_group_stack_rules` WHERE `group_id` IN (2100002, 2100003);
DELETE FROM `spell_group` WHERE `id` IN (2100002, 2100003);
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES
(2100002, 523489),
(2100002, 523513),
(2100003, 800266),
(2100003, 680294);
DELETE FROM `spell_group_stack_rules` WHERE `group_id` IN (2100002, 2100003);
INSERT INTO `spell_group_stack_rules` (`group_id`, `stack_rule`) VALUES
(2100002, 1),
(2100003, 1);
