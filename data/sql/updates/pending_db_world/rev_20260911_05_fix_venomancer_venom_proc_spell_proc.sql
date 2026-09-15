-- Give the six Venomancer venom Activation auras a `spell_proc` row.
-- `Aura::GetProcEffectMask()` unconditionally returns 0 for any aura absent from
-- `spell_proc` ("only auras with spell proc entry can trigger proc"), which runs
-- before the custom AuraScript's own CheckCast/Trigger logic is ever reached. This is
-- a second, independent bug from rev_20260911_04 (missing spell_script_names row):
-- even with the script correctly linked, these auras could never proc at all without
-- this row. ProcFlags covers every DmgClass/positivity combination a player's own
-- non-triggered spell cast can produce at PROC_SPELL_PHASE_CAST (Spell::cast(),
-- Spell.cpp ~4001-4013): DONE_SPELL_MELEE_DMG_CLASS | DONE_SPELL_RANGED_DMG_CLASS |
-- DONE_SPELL_NONE_DMG_CLASS_POS/NEG | DONE_SPELL_MAGIC_DMG_CLASS_POS/NEG, matching the
-- tooltip's unrestricted "your casts". SpellPhaseMask is restricted to CAST (1) only,
-- matching aura_ascension_venomancer_venom_proc::CheckCast's exact phase check.
-- SpellFamilyName/Mask, SchoolMask, SpellTypeMask, HitMask, Chance and Charges are
-- left at 0 so they stay unrestricted / inherit from the spell's own DBC fields
-- (ProcChance, ProcCharges), per SpellMgr::LoadSpellProcs().
START TRANSACTION;
DELETE FROM `spell_proc` WHERE `SpellId` IN (630868, 805731, 805775, 805776, 805777, 805778);
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(630868, 0, 0, 0, 0, 0, 87312, 0, 1, 0, 0, 0, 0, 0, 0, 0),
(805731, 0, 0, 0, 0, 0, 87312, 0, 1, 0, 0, 0, 0, 0, 0, 0),
(805775, 0, 0, 0, 0, 0, 87312, 0, 1, 0, 0, 0, 0, 0, 0, 0),
(805776, 0, 0, 0, 0, 0, 87312, 0, 1, 0, 0, 0, 0, 0, 0, 0),
(805777, 0, 0, 0, 0, 0, 87312, 0, 1, 0, 0, 0, 0, 0, 0, 0),
(805778, 0, 0, 0, 0, 0, 87312, 0, 1, 0, 0, 0, 0, 0, 0, 0);
COMMIT;
