-- Molten Core boss scripts: Shazzrah's threat-wipe Blink, Garr's own kit, and Ragnaros's
-- like-for-like spell swap for Hand/Wrath of Ragnaros and Magma Blast.

-- Shazzrah (12264): Blink (2105611) already teleports through its own effects; only the
-- threat wipe stock Gate of Shazzrah does is missing (spell_shazzrah_blink_coa).
DELETE FROM `spell_script_names` WHERE `spell_id` = 2105611 AND `ScriptName` = 'spell_shazzrah_blink_coa';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(2105611, 'spell_shazzrah_blink_coa');
