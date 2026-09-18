-- Erode Armaments (520386): "Your Wand attacks now reduce enemy attack power". Its proc aura triggers
-- 300157, but Spell.dbc gives it ProcFlags 0 and no row existed, so it never procced. Proc on the
-- ranged class damage of Wand of Time (family 28, word1 0x100000) and Artificer's Wand (family 28,
-- word2 0x200), the two "Wand attacks" its tooltip covers.
DELETE FROM `spell_proc` WHERE `SpellId` = 520386;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(520386, 0, 28, 0, 1048576, 512, 256, 1, 2, 0, 0, 0, 0, 100, 0, 0);
