START TRANSACTION;
DELETE FROM `spell_group` WHERE `id` = 1140;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES
(1140, 300255),
(1140, 680301);
DELETE FROM `spell_group_stack_rules` WHERE `group_id` = 1140;
INSERT INTO `spell_group_stack_rules` (`group_id`, `stack_rule`, `description`) VALUES
(1140, 2, 'Local CoA: one Celestial Mind per caster on each recipient');
DELETE FROM `spell_group` WHERE `id` = 1141;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES
(1141, 573343),
(1141, 573348);
DELETE FROM `spell_group_stack_rules` WHERE `group_id` = 1141;
INSERT INTO `spell_group_stack_rules` (`group_id`, `stack_rule`, `description`) VALUES
(1141, 2, 'Local CoA: one Arcane Protection per caster on each recipient');
COMMIT;
