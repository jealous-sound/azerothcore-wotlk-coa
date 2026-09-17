-- Grove Training (92150): "All melee damage, or healing done, now has a 10% chance to grant you Aftershock".
-- Spell.dbc gives it ProcFlags 0 and no row existed. Melee auto attacks and melee abilities that deal damage,
-- and positive magic or no-class spells that heal; the 10% chance comes from the DBC ProcChance.
START TRANSACTION;
DELETE FROM `spell_proc` WHERE `SpellId` = 92150;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(92150, 0, 0, 0, 0, 0, 17428, 3, 2, 0, 0, 0, 0, 0, 0, 0);
COMMIT;
