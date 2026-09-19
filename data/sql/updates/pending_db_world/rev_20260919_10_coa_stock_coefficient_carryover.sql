-- Companion to AscensionStockCoefficients.cpp, which stops CoA spells reading Spell.dbc's stock
-- EffectBonusMultiplier column. Most of the slots it clears state no stat coefficient at all, so
-- clearing them is the whole fix. These ten abilities do state one in their own description, and
-- would otherwise lose the only scaling they have. Each row carries the coefficient that description
-- states, not the stock column's value.
--
-- Stat selection: spell_bonus_data.direct_bonus is spell power for damage and bonus healing for heals;
-- ap_bonus resolves to ranged attack power only when the record is a ranged weapon spell outside
-- SPELL_DAMAGE_CLASS_MELEE. Snapseed carries the bow/gun/crossbow equipment mask and is
-- SPELL_DAMAGE_CLASS_RANGED, matching its "$RAP*.3"; every other attack power row here is
-- SPELL_DAMAGE_CLASS_MAGIC with no ranged mask and resolves to melee attack power, matching "$AP".
--
-- Rank coverage: SpellMgr::GetSpellBonusData looks up the exact id first and then the chain's first
-- rank, so a single row on a chain head covers its ranks. Hodir's Wrath ranks 2-7 have no spell_ranks
-- chain in this database and are listed individually; its separate root record 800152 states a
-- different coefficient ($AP*1.2) and is deliberately left alone.
START TRANSACTION;
DELETE FROM `spell_bonus_data` WHERE `entry` IN (503313, 503314, 503315, 503316, 503317, 503318, 560315, 572352, 572855, 800144, 800180, 801292, 804027, 805730, 806299);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(503313, 0, 0, 0.4, 0, 'Local Barbarian: Hodir''s Wrath rank 2 - "$AP*0.40", no spell_ranks chain'),
(503314, 0, 0, 0.4, 0, 'Local Barbarian: Hodir''s Wrath rank 3 - "$AP*0.40", no spell_ranks chain'),
(503315, 0, 0, 0.4, 0, 'Local Barbarian: Hodir''s Wrath rank 4 - "$AP*0.40", no spell_ranks chain'),
(503316, 0, 0, 0.4, 0, 'Local Barbarian: Hodir''s Wrath rank 5 - "$AP*0.40", no spell_ranks chain'),
(503317, 0, 0, 0.4, 0, 'Local Barbarian: Hodir''s Wrath rank 6 - "$AP*0.40", no spell_ranks chain'),
(503318, 0, 0, 0.4, 0, 'Local Barbarian: Hodir''s Wrath rank 7 - "$AP*0.40", no spell_ranks chain'),
(560315, 1.2775, 0, 0, 0, 'Local Bloodmage: Valanar''s Vengeance chain head - "$sp*1.2775"'),
(572352, 0.42, 0, 0, 0, 'Local Chronomancer: Correct the Mistake - heal, "$BH*0.42"'),
(572855, 0.5, 0, 0.65, 0, 'Local Bloodmage: Hemoburst - "$AP*.65+$SP*.5"'),
(800144, 0.2, 0, 0.2, 0, 'Local Primalist: Spirit Charge chain head - heal and splash, "$BH*0.2+$AP*.2"'),
(800180, 0, 0, 1, 0, 'Local Primalist: Sacred Grove - instant heal, "$AP*1"'),
(801292, 2, 0, 0, 0, 'Local Chronomancer: Chromatic Shard chain head - "$SP*2"'),
(804027, 0, 0, 0.3, 0, 'Local Ranger: Snapseed chain head - "$RAP*.3", ranged mask selects ranged AP'),
(805730, 0.3, 0, 0, 0, 'Local Runemaster: Glacial Rune damage - "$SP*.3"'),
(806299, 0.85, 0, 0, 0, 'Local Chronomancer: Fabric of Time chain head - heal, "$BH*0.85"');
COMMIT;
