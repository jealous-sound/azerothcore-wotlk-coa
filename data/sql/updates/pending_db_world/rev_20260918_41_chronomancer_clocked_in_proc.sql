-- Clocked In (706076): "Casting Discordance now makes your next Artificer's Wand within [...] instant
-- cast and causes it to generate an additional Echo Fragment". Its proc aura triggers 520168, but
-- Spell.dbc gives it ProcFlags 0 and no row existed, so it never procced. Proc on the magic class
-- damage of Discordance (family 28, word0 0x2000000), the ability its tooltip names.
DELETE FROM `spell_proc` WHERE `SpellId` = 706076;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(706076, 0, 28, 33554432, 0, 0, 65536, 1, 2, 0, 0, 0, 0, 100, 0, 0);
