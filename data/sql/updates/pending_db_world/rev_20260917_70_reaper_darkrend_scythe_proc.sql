-- Darkrend Scythe (805205): "Damage dealt by Doomrend and Crow's Harvest now applies a bleed". Its proc aura
-- triggers 704256, but Spell.dbc gives it ProcFlags 0 and no row existed, so it never procced. Proc on the melee
-- class damage of Doomrend (family 36, mask 0/0/0x400000) and Crow's Harvest (family 36, mask 0x200000/0x100/0).
DELETE FROM `spell_proc` WHERE `SpellId` = 805205;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(805205, 0, 36, 2097152, 256, 4194304, 16, 1, 2, 0, 0, 0, 0, 0, 0, 0);
