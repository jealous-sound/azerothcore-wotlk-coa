-- Soulbender 804004 is a SPELL_AURA_PROC_TRIGGER_SPELL passive with no spell_proc row, like the other Reaper
-- proc talents whose DBC ProcTypeMask is 0. It fires on a landed Deathchaser, keyed the same way as
-- Chasing Death 707455 (SpellFamilyName 36, SpellFamilyFlags[0] 0x4, melee-class spell damage).
DELETE FROM `spell_proc` WHERE `SpellId` = 804004;
INSERT INTO `spell_proc`
  (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`,
   `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`,
   `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`)
VALUES
  (804004, 0, 36, 4, 0, 0, 16, 7, 2, 0, 0, 0, 0, 100, 0, 0);
