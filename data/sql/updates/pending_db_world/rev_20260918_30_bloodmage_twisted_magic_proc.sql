-- Twisted Magic (704634): "Your direct harmful critical strikes increase the target's Shadow damage taken
-- from you by 2%, stacking up to 5 times, for 12 sec." Its effect 0 is aura 42 (proc trigger spell) on
-- Twisted Magic 504138, but Spell.dbc gives the record ProcFlags 0 in `spell_proc` terms (no row existed at
-- all), so the generated entry carried no proc flag and the aura could never fire. 504138 itself is a
-- correctly-built stacking Shadow-damage-taken debuff (School 32, BasePoints 1 -> 2%); only the trigger was
-- dead. Proc on any direct harmful damage the caster deals (no spell-family restriction is stated), gated to
-- critical hits only per the tooltip; chance is the record's own ProcChance (100, i.e. every crit qualifies).
-- AttributesMask is 0 (PROC_ATTR_TRIGGERED_CAN_PROC not set): the tooltip's "direct harmful critical
-- strikes" is exercised by the player's own direct casts, not by a helper spell cast as a triggered effect
-- of something else, and no such helper was identified that would otherwise fail to proc this row.
DELETE FROM `spell_proc` WHERE `SpellId` = 704634;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(704634, 0, 0, 0, 0, 0, 69972, 1, 2, 2, 0, 0, 0, 100, 0, 0);
