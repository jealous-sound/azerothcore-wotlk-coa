-- Discovery (500116): "Casting Artificer's Wand and Wand of Time now grants Discovery". Its two proc
-- effects trigger 704197 and 806201, but Spell.dbc gives it ProcFlags 0 and no row existed, so it never
-- procced. Proc on the ranged class damage of Wand of Time (family 28, word1 0x100000) and Artificer's
-- Wand (family 28, word2 0x200), the two abilities its tooltip names.
DELETE FROM `spell_proc` WHERE `SpellId` = 500116;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(500116, 0, 28, 0, 1048576, 512, 256, 1, 2, 0, 0, 0, 0, 100, 0, 0);
