-- Necromancer: Undead Assault, Pacify and Protect are exclusive per caster.
START TRANSACTION;
DELETE FROM `spell_group` WHERE `id` = 1137;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES
(1137, 500982),
(1137, 500983),
(1137, 500985);
DELETE FROM `spell_group_stack_rules` WHERE `group_id` = 1137;
INSERT INTO `spell_group_stack_rules` (`group_id`, `stack_rule`, `description`) VALUES
(1137, 2, 'Necromancer: one active Undead Stance per caster');
COMMIT;
