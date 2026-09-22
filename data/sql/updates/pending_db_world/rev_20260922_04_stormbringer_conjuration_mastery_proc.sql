-- Stormbringer Conjuration Mastery 300595 (#1673): "Your Conjure Storm now refunds 10 Static."
--
-- Spell.dbc gives 300595 a SPELL_AURA_PROC_TRIGGER_SPELL (42) effect on 804084 "Add 10 Static", whose
-- SPELL_EFFECT_ASCENSION_MODIFY_AURA_STACKS effect carries MiscValue 10 and TriggerSpell 803102 (Static) on
-- TARGET_UNIT_CASTER, so the refund itself is fully authored. The record has ProcFlags 0 and no `spell_proc`
-- row existed, so SpellMgr::LoadSpellProcs skipped it ("Skip if no proc flags in DBC",
-- src/server/game/Spells/SpellMgr.cpp) and Aura::GetProcEffectMask returned a zero mask
-- ("only auras with spell proc entry can trigger proc", src/server/game/Spells/Auras/SpellAuras.cpp):
-- the aura could never fire. Same defect and same shape as rev_1789944986203842691.sql (Altered Course,
-- class 16, family 22, cast phase).
--
-- SpellFamilyMask2 16 is the Conjure Storm bit: 800227 and its ranks 501415-501420 all carry
-- SpellFamilyFlags[2] 48 in family 22. The same bit is on the Lesser Conjure Storm 802701 and Conjure
-- Rainstorm 430870 variants, which therefore refund as well; 300595 itself carries no class mask.
-- ProcFlags 65536 (PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_NEG) is what Spell::cast raises for this record
-- (DmgClass 1 magic, harmful), and SpellPhaseMask 1 (PROC_SPELL_PHASE_CAST) is the phase the tooltip
-- describes: the refund happens on the cast, not per struck enemy. HitMask stays 0, which CAST phase with
-- DONE flags skips (SpellMgr::CanSpellTriggerProcOnEvent). Chance 100 is the record's own ProcChance.
--
-- The tooltip's other branch, $?s707615[Updraft], is not covered here: Updraft carries a different family
-- mask and the issue reports only the Conjure Storm branch.
START TRANSACTION;
DELETE FROM `spell_proc` WHERE `SpellId` = 300595;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`,
    `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`,
    `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(300595, 0, 22, 0, 0, 16, 65536, 0, 1, 0, 0, 0, 0, 100, 0, 0);
COMMIT;
