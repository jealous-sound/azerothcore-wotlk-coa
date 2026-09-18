-- Reverse Engineering (520875): "Periodic damage and healing now has a $h% chance to increase your
-- haste ... and grant allies Replenishment". Its two proc effects trigger 1257670 and 520936, but
-- Spell.dbc gives it ProcFlags 0 and no row existed, so it never procced. Proc on any of the caster's
-- own periodic damage or healing ticks, with no spell family restriction, matching the generic wording.
DELETE FROM `spell_proc` WHERE `SpellId` = 520875;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(520875, 0, 0, 0, 0, 0, 262144, 3, 2, 0, 0, 0, 0, 15, 0, 0);
