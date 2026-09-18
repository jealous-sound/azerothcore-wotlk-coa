-- Swarm of Flies (704646): "Your Bloodbolt now refreshes the duration of Taldaram's Torment." Its effect 0
-- is aura 42 (proc trigger spell) on Spread Taldarams 704647 (which itself triggers the Taldaram's Torment
-- refresh chain, 800988/800772), but Spell.dbc gives the record ProcFlags 0 and no `spell_proc` row existed,
-- so 704647 could never fire. Proc restricted to Bloodbolt by SpellFamilyMask1 131072, confirmed via
-- Spell.dbc: every Bloodbolt/Bloodbolt (Empowered) rank and cast-form spell (578304, 578305, 804685,
-- 806928-806932, 681396, 681405, 681520, 681550, 681551) carries SpellFamilyFlags/ClassMask (0, 131072, 0)
-- and no other family-26 spell does. Chance is the record's own ProcChance (100 - always, as the tooltip
-- states no percentage).
-- HitMask is 0 (unset): a DONE proc with HitMask unset already defaults to NORMAL | CRITICAL | ABSORB
-- (SpellMgr::CanSpellTriggerProcOnEvent), the usual "damage done" set; BLOCK and FULL_BLOCK are
-- deliberately left out because a (fully) blocked hit deals no damage. AttributesMask is 0
-- (PROC_ATTR_TRIGGERED_CAN_PROC not set): no helper spell was identified that casts Bloodbolt as a
-- triggered effect of something else.
DELETE FROM `spell_proc` WHERE `SpellId` = 704646;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(704646, 0, 26, 0, 131072, 0, 69972, 1, 2, 0, 0, 0, 0, 100, 0, 0);
