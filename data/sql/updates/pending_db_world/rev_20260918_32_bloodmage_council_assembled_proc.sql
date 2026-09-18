-- The Council Assembled (504103): "Valanar's Vengeance and Keleseth's Calamity now reduce the cooldown of
-- Malediction by 5 sec." Effect 0 is aura 42 (proc trigger spell) on Council Assembled CD Reduc 505153,
-- which is a correctly-built SPELL_EFFECT_ASCENSION_MODIFY_COOLDOWN (-5000ms) targeting Malediction 800776,
-- but Spell.dbc gives 504103's record ProcFlags 0 and no `spell_proc` row existed, so 505153 could never
-- fire. Effect 1 (Atherann's Anguish -25 Rage, SPELLMOD_COST) is untouched here - it already works natively
-- and is not part of this fix. Proc restricted to Valanar's Vengeance and Keleseth's Calamity by
-- SpellFamilyMask0 393216 (131072 + 262144), confirmed via Spell.dbc: Keleseth's Calamity (560249) carries
-- SpellFamilyFlags (131072, 0, 0) and Valanar's Vengeance (560315) carries (262144, 0, 0) - the same split
-- independently cross-checked against Enthraller 706619's mask 393216 in this batch's investigation. Chance
-- is the record's own ProcChance (100 - always, as the tooltip states no percentage).
-- The tooltip's third clause (a stacking damage buff to the caster's next Valanar's Vengeance/Keleseth's
-- Calamity) was not traced to a concrete spell ID during this batch's investigation and is NOT fixed by this
-- row; it remains unresolved and is not covered by this migration.
-- HitMask is 0 (unset): a DONE proc with HitMask unset already defaults to NORMAL | CRITICAL | ABSORB
-- (SpellMgr::CanSpellTriggerProcOnEvent), the usual "damage done" set; BLOCK and FULL_BLOCK are
-- deliberately left out because a (fully) blocked hit deals no damage. AttributesMask is 0
-- (PROC_ATTR_TRIGGERED_CAN_PROC not set): no helper spell was identified that casts Valanar's
-- Vengeance/Keleseth's Calamity as a triggered effect of something else.
DELETE FROM `spell_proc` WHERE `SpellId` = 504103;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(504103, 0, 26, 393216, 0, 0, 69972, 1, 2, 0, 0, 0, 0, 100, 0, 0);
