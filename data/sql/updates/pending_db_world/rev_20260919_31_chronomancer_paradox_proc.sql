-- Paradox (680946): "Casting Melt Reality now causes your next Unmake or Reverse Wound within 6 sec
-- to become instant and cost 100% less mana." Its effect 0 is aura 42 (proc trigger spell) triggering
-- 680947 (a self-buff with two ADD_PCT_MODIFIER effects, SPELLMOD_CASTING_TIME and SPELLMOD_COST at
-- -100%, class-masked to word1 0x2000000 = Unmake 804418's own SpellFamilyFlags word1, and word2 0x1 =
-- Reverse Wound 801303's own SpellFamilyFlags word2 - the family mask check ORs across all three words,
-- so one row correctly covers "Unmake or Reverse Wound" as the tooltip states), but Spell.dbc gives
-- 680946's record ProcFlags 0 and no `spell_proc` row existed, so 680947 could never fire. Proc on the
-- magic class damage of Melt Reality (family 28, word1 0x200 = 512), the ability its tooltip names.
-- Melt Reality has no SPELL_EFFECT_SCHOOL_DAMAGE effect (only APPLY_AURA), so
-- Unit::ProcSkillsAndAuras never sees a direct damage/heal amount for its own cast event and computes
-- SpellTypeMask as PROC_SPELL_TYPE_NO_DMG_HEAL (4), not DAMAGE (1) - verified both by reading
-- Unit.cpp's `spellTypeMask = damageInfo && (damage||absorb) ? DAMAGE : (procSpellInfo ? NO_DMG_HEAL :
-- 0)` for a HIT-phase event, and empirically via temporary instrumentation of
-- Aura::GetProcEffectMask on a live slot-2 run: the real ProcEventInfo for Melt Reality's HIT-phase
-- event reads typeMask=65536 (PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_NEG) and spellTypeMask=4,
-- confirming both the NEG proc-flag variant and the NO_DMG_HEAL type mask empirically, not just by
-- static analysis. Chance is the record's own ProcChance (100 - always, as the tooltip states no
-- percentage).
DELETE FROM `spell_proc` WHERE `SpellId` = 680946;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(680946, 0, 28, 0, 512, 0, 65536, 4, 2, 0, 0, 0, 0, 100, 0, 0);
