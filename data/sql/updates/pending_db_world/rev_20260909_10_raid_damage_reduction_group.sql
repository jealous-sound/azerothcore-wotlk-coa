-- Dedicated 3-percent damage-reduction group. Group1038 is the native stat-percent group.
-- Install only when group2000185 is absent or exactly matches these definitions.
-- Keep every applied predecessor immutable and every unrelated group/member intact.
DELETE FROM `spell_group` WHERE `id` IN (1038, 2000184) AND `spell_id` = 704785;
INSERT INTO `spell_group` (`id`, `spell_id`) SELECT 2000185, 704785
WHERE NOT EXISTS (SELECT 1 FROM `spell_group` WHERE `id` = 2000185 AND `spell_id` = 704785);
INSERT INTO `spell_group` (`id`, `spell_id`) SELECT 2000185, 67480
WHERE NOT EXISTS (SELECT 1 FROM `spell_group` WHERE `id` = 2000185 AND `spell_id` = 67480);
INSERT INTO `spell_group` (`id`, `spell_id`) SELECT 2000185, 50720
WHERE NOT EXISTS (SELECT 1 FROM `spell_group` WHERE `id` = 2000185 AND `spell_id` = 50720);
INSERT INTO `spell_group_stack_rules` (`group_id`, `stack_rule`) SELECT 2000185, 3
WHERE NOT EXISTS (SELECT 1 FROM `spell_group_stack_rules` WHERE `group_id` = 2000185);
