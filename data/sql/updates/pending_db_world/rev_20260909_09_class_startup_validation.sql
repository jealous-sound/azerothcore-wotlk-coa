-- Correct the first linked class-follow-up startup; applied SQL03-08 stay immutable.
-- Only existing non-proc aura slots may appear in DisableEffectsMask.
UPDATE `spell_proc` SET `DisableEffectsMask` = 2 WHERE `SpellId` = 705274 AND `DisableEffectsMask` = 6;
UPDATE `spell_proc` SET `DisableEffectsMask` = 0 WHERE `SpellId` = 705290 AND `DisableEffectsMask` = 6;
-- These active spells have cast/hit handling, but no aura lifecycle after contracts.
DELETE FROM `spell_script_names` WHERE `spell_id` = 801125 AND `ScriptName` = 'aura_ascension_starcaller_lifecycle';
DELETE FROM `spell_script_names` WHERE `spell_id` = 807942 AND `ScriptName` = 'aura_ascension_felsworn_lifecycle';
DELETE FROM `spell_script_names` WHERE `spell_id` = 805679 AND `ScriptName` = 'aura_ascension_xoroth_lifecycle';
-- Shrouded Stars reduces incoming damage and shares Sanctuary's native reduction group.
DELETE FROM `spell_group` WHERE `id` IN (1038, 2000184) AND `spell_id` = 704785;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES (1038, 704785);
