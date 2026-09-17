-- Discerning Eye of the Beast (59915): restore the stock 3.3.5a kill proc flag. The CoA Spell.dbc copy has
-- ProcFlags 0, so the base row (all zero, which falls back to the DBC) never triggered the mana restore.
START TRANSACTION;
DELETE FROM `spell_proc` WHERE `SpellId` = 59915;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(59915, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0);
COMMIT;
