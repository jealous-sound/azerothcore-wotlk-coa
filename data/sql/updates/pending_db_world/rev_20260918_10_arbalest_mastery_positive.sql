-- Arbalest Mastery progress (706241) is the caster's own damage bonus: each Witchbane channel shot adds a stack
-- and every stack raises the next shot's damage by 15%. The client was showing it among the player's debuffs
-- because the server sent it as a negative aura, which is what issue #3935 reports as "the debuff shows on you".
-- Mark the aura positive so it is displayed as a buff on the caster.
DELETE FROM `spell_custom_attr` WHERE `spell_id` = 706241;
INSERT INTO `spell_custom_attr` (`spell_id`, `attributes`) VALUES
(706241, 234881024);
