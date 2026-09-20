-- Gift of the Timeways (704487): "Casting Melt Reality now causes your Timerend and Unmake's periodic
-- damage to benefit from haste for 10 sec." Its effect 0 is aura 42 (proc trigger spell) triggering
-- 503903 (a self-buff applying SPELL_AURA_PERIODIC_HASTE scoped by class mask to Timerend 801291's and
-- Unmake 804418's own SpellFamilyFlags word1 bits, confirmed via Spell.dbc: 503903's
-- EffectSpellClassMask word1 is 50331648 = 16777216 (Timerend) + 33554432 (Unmake)), but Spell.dbc
-- gives 704487's record ProcFlags 0 and no `spell_proc` row existed, so 503903 could never fire.
-- Proc on the magic class damage of Melt Reality (family 28, word1 0x200 = 512, confirmed against
-- Melt Reality 806335 and every rank in its chain), the ability its tooltip names. Melt Reality has
-- no SPELL_EFFECT_SCHOOL_DAMAGE effect (only APPLY_AURA), so Unit::ProcSkillsAndAuras never sees a
-- direct damage/heal amount for its own cast event and computes SpellTypeMask as
-- PROC_SPELL_TYPE_NO_DMG_HEAL (4), not DAMAGE (1) - verified both by reading Unit.cpp's
-- `spellTypeMask = damageInfo && (damage||absorb) ? DAMAGE : (procSpellInfo ? NO_DMG_HEAL : 0)` for a
-- HIT-phase event, and empirically via temporary instrumentation of Aura::GetProcEffectMask on a live
-- slot-2 run: the real ProcEventInfo for Melt Reality's HIT-phase event reads
-- typeMask=65536 (PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_NEG) and spellTypeMask=4, confirming both the
-- NEG proc-flag variant and the NO_DMG_HEAL type mask empirically, not just by static analysis. Chance
-- is the record's own ProcChance (100 - always, as the tooltip states no percentage).
DELETE FROM `spell_proc` WHERE `SpellId` = 704487;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(704487, 0, 28, 0, 512, 0, 65536, 4, 2, 0, 0, 0, 0, 100, 0, 0);
