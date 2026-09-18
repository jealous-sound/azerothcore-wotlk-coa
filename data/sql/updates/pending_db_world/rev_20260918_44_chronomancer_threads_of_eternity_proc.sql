-- Threads of Eternity (806209): "Damage dealt by Discordance now applies Thread of Eternity to
-- enemies". Its proc aura triggers 572127, but Spell.dbc gives it ProcFlags 0 and no row existed, so
-- it never procced. Proc on the magic class damage of Discordance (family 28, word0 0x2000000), the
-- ability its tooltip names.
DELETE FROM `spell_proc` WHERE `SpellId` = 806209;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(806209, 0, 28, 33554432, 0, 0, 65536, 1, 2, 0, 0, 0, 0, 100, 0, 0);
