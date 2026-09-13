-- Restore the captured Primal Weapons selector and its two exclusive weapon bonuses.
-- Requires the matching source and client DBC edit; no helper is a spellbook grant.
DELETE FROM `spell_script_names` WHERE `spell_id` = 537218 AND `ScriptName` = 'spell_ascension_primal_weapons';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (537218, 'spell_ascension_primal_weapons');
DELETE FROM `spell_script_names` WHERE `spell_id` IN (801242, 704098) AND `ScriptName` = 'aura_ascension_primal_weapon';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(801242, 'aura_ascension_primal_weapon'),
(704098, 'aura_ascension_primal_weapon');

DELETE FROM `spell_proc` WHERE `SpellId` IN (801242, 806070, 806071);
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`,
`HitMask`, `DisableEffectsMask`, `Chance`, `Charges`) VALUES
(801242, 1, 332116, 1, 2, 2, 3, 100, 0),
(806070, 0, 4, 1, 2, 3, 0, 100, 2),
(806071, 0, 4, 1, 2, 3, 0, 100, 2);
