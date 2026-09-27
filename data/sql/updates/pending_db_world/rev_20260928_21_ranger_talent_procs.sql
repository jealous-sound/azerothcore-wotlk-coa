-- Frenzy (520492, Farstrider row 9 col 11) reads "Enrage all nearby allies, increasing their haste by 30%
-- for 20 sec. After being affected by this spell, allies cannot benefit from similar effects for 5 min."
-- Its effect 1 is the 30% haste (SPELL_AURA_MOD_MELEE_RANGED_HASTE) and its effect 2 is
-- SPELL_EFFECT_TRIGGER_SPELL on 804457 "Worn Out", the 5 min exclusivity marker. 804457 is a bare
-- SPELL_AURA_DUMMY with BasePoints 0 and nothing anywhere in src/ or data/ reads it, so the exclusivity
-- clause was pure dead data and every Frenzy in the window stacked.
--
-- aura_ascension_ranger_frenzy zeroes effect 1's amount while the buffed ally already carries Worn Out,
-- which is the only reading that matches a 5 min cooldown and a 5 min marker. Effect 1 applies before
-- effect 2 fires the trigger, so the check sees the Worn Out from a previous Frenzy, not the one this
-- cast is about to apply.
--
-- Pilfering (705087, Brigand row 7 col 9) reads "Adds 10% to the damage dealt by Dirty Blades and causes
-- its damage dealt to heal you equal to 50% of the value." The 10% is effect 0, SPELL_AURA_ADD_FLAT_MODIFIER
-- (aura 107), which AuraEffect::CalculateSpellMod applies natively. The 50% is effect 1, an unnamed
-- Ascension aura 354 whose handler table entry is nullptr (SpellAuraEffects.cpp), so it is only ever driven
-- by a bound AuraScript plus a `spell_proc` row: twenty other talents already use exactly that pair, most
-- closely the shipped Red Dream 521451 -> 521643 heal in rev_20260914_11_ranger_red_flowers.sql.
--
-- The proc event is the Ranger's own melee auto attack while Dirty Blades (680276) is up, which is what
-- Dirty Blades itself keys on ("your melee auto attacks deal an additional 60% of the damage dealt").
-- CheckProc tests that in code against the aura rather than through SpellFamilyMask, so the row does not
-- depend on a family bit this revision cannot read. ProcFlags 4 = PROC_FLAG_DONE_MELEE_AUTO_ATTACK
-- (SpellMgr.h:118); the melee/ranged spell and periodic bits are left out because the tooltip says the
-- damage Dirty Blades adds to auto attacks. SpellTypeMask 1 keeps healing ticks out. SpellPhaseMask 2 is
-- mandatory because 0x4 is inside REQ_SPELL_PHASE_PROC_FLAG_MASK (SpellMgr.h:184). HitMask 0 takes the DONE
-- default PROC_HIT_NORMAL | PROC_HIT_CRITICAL | PROC_HIT_ABSORB; the tooltip asks for no hit result.
-- AttributesMask 0: a white swing is the Ranger's own, never a triggered cast, and the AUTO_ATTACK branch
-- of Aura::GetProcEffectMask exempts white swings from PROC_ATTR_TRIGGERED_CAN_PROC anyway.
-- Chance 0 so the record's own ProcChance applies, per rev_20260919_20_coa_proc_chance_parity.sql.
DELETE FROM `spell_script_names` WHERE `ScriptName` IN
    ('aura_ascension_ranger_frenzy', 'aura_ascension_ranger_pilfering');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(520492, 'aura_ascension_ranger_frenzy'),
(705087, 'aura_ascension_ranger_pilfering');
DELETE FROM `spell_proc` WHERE `SpellId` IN (705087);
INSERT INTO `spell_proc`
    (`SpellId`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `Chance`)
VALUES
(705087, 4, 1, 2, 0, 0, 0);
