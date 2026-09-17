-- Shaman Training (805445): "Damage dealt by Wildclaw now reduces the enemy's armor". Its proc aura triggers
-- Bioerosion (575848), but Spell.dbc gives it ProcFlags 0 and no row existed, so it never procced. Proc on the
-- melee class damage of Wildclaw (family 37, mask bit 0x80000; bit 0 is shared with Bountiful Boons).
DELETE FROM `spell_proc` WHERE `SpellId` = 805445;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(805445, 0, 37, 524288, 0, 0, 16, 1, 2, 0, 0, 0, 0, 0, 0, 0);
