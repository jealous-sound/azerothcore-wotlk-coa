-- Remove only the player-level gates on two apply spells and the Minor Mount Speed self helper.
-- Match the exact bindings, allowing missing/already-removed rows and unrelated custom scripts.
-- Preserve Blade Ward/Blood Draining proc mechanics and Minor Mount Speed lifecycle links.
DELETE FROM `spell_script_names` WHERE
(`spell_id` IN (64441, 64579) AND BINARY `ScriptName` = BINARY 'spell_gen_proc_above_75')
OR (`spell_id` = 59917 AND BINARY `ScriptName` = BINARY 'spell_gen_disabled_above_70');
