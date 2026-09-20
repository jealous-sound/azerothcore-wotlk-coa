-- Thirst (92112), the Bloodmage's Level 10 Passive: "Spells that cost health now grant |cffffffffThirst|r.
-- Reaching $706613u stacks makes you |cffffffffInsatiable|r, reducing your damage dealt by $706663s1% every
-- $706621t1 sec until |cffffffffThirst|r ends."
--
-- Issue #1852 was filed on Thirst 500107, a record no acquisition table grants (its only effects are three
-- aura 108 spell modifiers, and it carries no trigger at all). The obtainable Thirst is 92112:
-- CharacterAdvancement.dbc row 4025 names it for class 20 (SONOFARUGAL), and the live-client capture holds
-- it. This row therefore resolves the issue against 92112, not against 500107.
--
-- 92112's effect 0 is SPELL_AURA_PROC_TRIGGER_SPELL (aura 42) on Thirst 706613, TargetA 1 (caster) - the
-- Blood thirst resource the UI already reads (AscensionCustomResourceData.h:58, display maximum 10, which
-- is also 706613's own StackAmount). Effect 1 is aura 149 SPELL_AURA_REDUCE_PUSHBACK and is native.
-- Spell.dbc gives 92112 ProcFlags 0 and no `spell_proc` row existed, so SpellMgr::LoadSpellProcs generated
-- no entry ("Skip if no proc flags in DBC"), SpellMgr::GetSpellProcEntry returned nullptr and
-- Aura::GetProcEffectMask returned 0. No `ResourceGainRule` in AscensionCustomResourceData.h names 706613
-- either, so nothing on the server ever granted a stack of Thirst.
--
-- SpellPhaseMask 1 (PROC_SPELL_PHASE_CAST): the cost is paid at cast, and 706613's three effects all use
-- TargetA 1 (caster), so the CAST phase - where Spell::cast passes no victim - is usable here.
-- ProcFlags 87056 are the DONE spell-class flags the core actually raises at that phase for the records in
-- question: PROC_FLAG_DONE_SPELL_MELEE_DMG_CLASS 0x10, PROC_FLAG_DONE_SPELL_NONE_DMG_CLASS_POS 0x400,
-- PROC_FLAG_DONE_SPELL_NONE_DMG_CLASS_NEG 0x1000, PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS 0x4000 and
-- PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_NEG 0x10000. Spell::PrepareDataForTriggerSystem sets the MELEE bit
-- for DmgClass MELEE, and Spell::cast falls back to the MAGIC or NONE polarity pair otherwise. Both
-- polarities are set because positivity is a runtime SpellInfo::IsPositive() result. The 176 family-26
-- records with PowerType POWER_HEALTH split into 117 DmgClass MAGIC, 36 MELEE and 23 NONE and none is
-- DmgClass RANGED, which is why PROC_FLAG_DONE_SPELL_RANGED_DMG_CLASS is deliberately left out.
-- SpellFamilyName 26 with all three family masks 0: SpellInfo::IsAffected skips the flag test when the mask
-- is empty, so the row covers every Bloodmage record rather than one named ability, which is what "spells
-- that cost health" asks for. SpellTypeMask 0: Unit::ProcSkillsAndAuras uses PROC_SPELL_TYPE_MASK_ALL at
-- CAST phase, so no type restriction is usable. HitMask 0: SpellMgr::CanSpellTriggerProcOnEvent skips the
-- hit check at CAST phase with DONE flags unless HitMask is set. Chance 0 defers to 92112's own
-- ProcChance 100. SchoolMask 0 - no tooltip clause restricts the school.
--
-- AttributesMask 4 = PROC_ATTR_REQ_MANA_COST, which is SpellMgr::CanSpellTriggerProcOnEvent's
-- "(!spellInfo->ManaCost && !spellInfo->ManaCostPercentage) -> false" test: it is the "cost" half of
-- "spells that cost health" and keeps the free family-26 health records (Accursed Form 562722, Vampyr's
-- Kiss copy 504785, Health Funneler 681401 and the like) from granting a stack.
-- PROC_ATTR_TRIGGERED_CAN_PROC is deliberately NOT set, so only spells the player casts herself count.
-- The half no column can express is *which* resource the cost is paid from; the script
-- `aura_ascension_bloodmage_thirst` (AscensionBloodmageSecondary.cpp) is the PowerType POWER_HEALTH test
-- and nothing else.
--
-- Not covered here: the tooltip's second sentence, "Reaching 10 stacks makes you Insatiable". 92112 has no
-- effect slot for it and no record casts Insatiable 706621 on a stack threshold, so that clause has no data
-- path at all from 92112 and is recorded as a separate finding rather than invented here.
START TRANSACTION;
DELETE FROM `spell_proc` WHERE `SpellId` = 92112;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(92112, 0, 26, 0, 0, 0, 87056, 0, 1, 0, 4, 0, 0, 0, 0, 0);
DELETE FROM `spell_script_names` WHERE `spell_id` = 92112
  AND `ScriptName` = 'aura_ascension_bloodmage_thirst';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(92112, 'aura_ascension_bloodmage_thirst');
COMMIT;
