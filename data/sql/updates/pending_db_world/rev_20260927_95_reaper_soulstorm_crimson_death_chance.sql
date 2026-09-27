-- rev_20260920_17 gave every talent row Chance 100, but two of those talents state a chance of their own:
-- Soulstorm 705403 refunds a Reaped Soul on 40% of Requiem and Soulrend hits, and Crimson Death 705414 repeats
-- Slaughter on 20% of casts. At 100 every Slaughter repeated for free and every Soulrend refunded a soul.
-- Both rows are rewritten with their final ProcFlags (705414 was widened to 87060 by rev_20260921_20).
DELETE FROM `spell_proc` WHERE `SpellId` IN (705403, 705414);
INSERT INTO `spell_proc`
  (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`,
   `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`,
   `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`)
VALUES
  (705403, 0, 0, 0, 0, 0, 69652, 1, 2, 0, 0, 0, 0, 40, 0, 0),
  (705414, 0, 0, 0, 0, 0, 87060, 7, 1, 0, 0, 0, 0, 20, 0, 0);
