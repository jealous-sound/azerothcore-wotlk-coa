START TRANSACTION;
DELETE FROM `spell_proc` WHERE `SpellId` IN (630868, 805731, 805775, 805776, 805777, 805778);
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(630868, 0, 0, 0, 0, 0, 87312, 7, 1, 0, 0, 0, 0, 0, 0, 0),
(805731, 0, 0, 0, 0, 0, 87312, 7, 1, 0, 0, 0, 0, 0, 0, 0),
(805775, 0, 0, 0, 0, 0, 87312, 7, 1, 0, 0, 0, 0, 0, 0, 0),
(805776, 0, 0, 0, 0, 0, 87312, 7, 1, 0, 0, 0, 0, 0, 0, 0),
(805777, 0, 0, 0, 0, 0, 87312, 7, 1, 0, 0, 0, 0, 0, 0, 0),
(805778, 0, 0, 0, 0, 0, 87312, 7, 1, 0, 0, 0, 0, 0, 0, 0);
COMMIT;
