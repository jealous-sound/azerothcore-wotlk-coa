-- A Reaper carries one Rite and no more. There are six: Rite of Perseverance, Rite of Power and
-- Rite of Resolve, plus the raid-wide Greater form of each. Nothing expressed that anywhere, so a
-- Reaper could hold all six at once and their effects stacked.
--
-- One spell_group with SPELL_GROUP_STACK_RULE_EXCLUSIVE (1): casting any of them replaces whichever
-- Rite is already up, the way Arcane Intellect and Arcane Brilliance do.
--
-- Ranks are deliberately absent. SpellMgr::GetSpellSpellGroupMapBounds resolves its argument through
-- GetFirstSpellInChain before looking the group up, so listing the first spell of each chain already
-- covers every trained rank; the ranks themselves would be dead rows. spell_ranks has the chains:
-- 575839 covers 575840-575841, 578126 covers 578127-578129, 800198 covers 803310-803314. The three
-- Greater Rites have no chain of their own and are listed as themselves.
--
-- Only these three families. Sacrificial Rite, Rite of the Loa, Rite of Possession, Rite Troll Mask
-- and Last Rite of Sholen share the word and nothing else.
DELETE FROM `spell_group_stack_rules` WHERE `group_id` = 2100001;
DELETE FROM `spell_group` WHERE `id` = 2100001;

INSERT INTO `spell_group` (`id`, `spell_id`) VALUES
(2100001, 575839), -- Rite of Perseverance (Rank 1, first in chain)
(2100001, 575842), -- Greater Rite of Perseverance
(2100001, 578126), -- Rite of Power (Rank 1, first in chain)
(2100001, 578130), -- Greater Rite of Power
(2100001, 800198), -- Rite of Resolve (Rank 1, first in chain)
(2100001, 680298); -- Greater Rite of Resolve

INSERT INTO `spell_group_stack_rules` (`group_id`, `stack_rule`) VALUES (2100001, 1);
