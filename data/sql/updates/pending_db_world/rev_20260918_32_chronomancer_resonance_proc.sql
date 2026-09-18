-- Resonance (706079): "Casting Artificer's Wand or Crystal Cannon now has a $h% chance to reduce the
-- cooldown of Hasten". Its proc aura triggers 570084, but Spell.dbc gives it ProcFlags 0 and no row
-- existed, so it never procced. Proc on the ranged class damage of Artificer's Wand (family 28, word2
-- 0x200) and Crystal Cannon (family 28, word2 0x1000), the two abilities its tooltip names.
DELETE FROM `spell_proc` WHERE `SpellId` = 706079;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(706079, 0, 28, 0, 0, 4608, 256, 1, 2, 0, 0, 0, 0, 35, 0, 0);
