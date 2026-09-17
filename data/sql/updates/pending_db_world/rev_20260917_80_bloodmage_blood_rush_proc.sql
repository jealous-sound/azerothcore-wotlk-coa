-- Blood Rush (560254): "Periodic damage dealt now has a 20% chance to grant you Blood Rush. Can only occur
-- once every 5 sec." Its effect 0 is aura 42 (proc trigger spell) on Blood Rush 504551, but Spell.dbc gives
-- the record ProcFlags 0 and no `spell_proc` row existed, so the generated entry carried no proc flag and the
-- aura could never fire. Proc on periodic damage the caster deals (PROC_FLAG_DONE_PERIODIC, damage type, hit
-- phase); chance and internal cooldown are the ones the record and its own description state.
DELETE FROM `spell_proc` WHERE `SpellId` = 560254;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(560254, 0, 0, 0, 0, 0, 262144, 1, 2, 0, 0, 0, 0, 20, 5000, 0);
