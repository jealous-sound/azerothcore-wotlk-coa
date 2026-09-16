-- #114: Reaper "Spectral Scythe" (spell 500484, and its Spirit Culling rank 500576) is a guardian summon of
-- creature 250305, which has no creature_template. Map::SummonCreature therefore returns null,
-- Spell::SummonGuardian bails out, and nothing spawns even though the cast still consumes Reaped Souls.
-- Spell 560351 "Deathchaser" summons the same entry and is fixed by the same row.
-- SummonProperties 61 is SUMMON_CATEGORY_ALLY, so faction 35 is replaced with the caster's faction at summon
-- time; flags_extra 64 is CREATURE_FLAG_EXTRA_NO_XP. TRIGGER (128) is deliberately not set, because
-- SummonGuardian skips MoveFollow for triggers and the scythe would never trail its owner.
-- unit_class 1, type 6 and BaseAttackTime 2000 match the animated weapons "Runebladed Sword"/"Runebladed Axe"
-- (28476/28475). Display 25398 is the floating two-handed weapon those creatures already use; the deployed
-- CreatureDisplayInfo.dbc has no scythe model, and 25398 already has a creature_model_info row.
DELETE FROM `creature_template_model` WHERE `CreatureID` = 250305;
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`,
`BaseAttackTime`, `RangeAttackTime`, `flags_extra`)
SELECT 250305, 'Spectral Scythe', 1, 1, 35, 1, 6, 2000, 2000, 64
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 250305);
DELETE FROM `creature_template_model` WHERE `CreatureID` = 250305;
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) VALUES
(250305, 0, 25398, 1, 1);
