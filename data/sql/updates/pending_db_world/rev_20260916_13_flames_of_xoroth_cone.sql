-- Flames of Xoroth is a frontal cone: every rank (801059, 802411-802415, 573448) uses DBC implicit
-- target 24 (TARGET_UNIT_CONE_ENEMY_24) with a 12 yd radius. The suffix in that enum is the DBC
-- implicit-target id, not an angle, but Spell::SelectImplicitConeTargets falls back to reading it as
-- 24 degrees when `spell_cone` has no row, which narrows the ability to +/-12 degrees and makes it
-- skip enemies standing plainly in front of the caster. Before `spell_cone` existed the core used a
-- fixed M_PI/2 arc for every cone spell, and none of the 430 sniffed rows for target-24 spells uses
-- 24 degrees (53/60/71/82/90 dominate), so restore the previous 90. SpellMgr::GetSpellCone and
-- SpellMgr::LoadSpellCones both normalise through GetFirstSpellInChain, so the first-rank row below
-- covers ranks 2-7 as well.
DELETE FROM `spell_cone` WHERE `ID` = 801059;
INSERT INTO `spell_cone` (`ID`, `ConeDegrees`) VALUES
(801059, 90);
