-- Time Means Nothing (572357): "Increases your magic damage dealt by 3% and damage dealt by Chromatic
-- Shard now increases the target's spell damage taken by 10% for 30 sec." The first clause is effect 2
-- (SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, school mask 126 = all magic schools) - already native, no code
-- change. Effect 0 (ADD_FLAT_MODIFIER, class-masked to word0 0x8000000/word1 0x3000000) is also a
-- native, always-on spellmod and is untouched here. Effect 1 is aura 42 (proc trigger spell) triggering
-- 572405 (a target debuff applying SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, school mask 126, +10% per its
-- own EffectBasePoints 9), but Spell.dbc gives 572357's record ProcFlags 0 and no `spell_proc` row
-- existed, so 572405 could never fire. Proc on the magic class damage of Chromatic Shard (family 28,
-- word1 0x1000 = 4096, word2 0x2000000 = 33554432), the ability its second clause names. Chance is the
-- record's own ProcChance (100 - always, as the tooltip states no percentage for this clause).
DELETE FROM `spell_proc` WHERE `SpellId` = 572357;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(572357, 0, 28, 0, 4096, 33554432, 65536, 1, 2, 0, 0, 0, 0, 100, 0, 0);
