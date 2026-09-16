-- #118: every War Falcon summon fails because the summoned creatures do not exist in this world.
-- Falcon's Call (804715) triggers Falcon's Call (800251), whose SPELL_EFFECT_SUMMON asks for creature
-- 50264 with SummonProperties 61 (Category 1 ally, Type 2 guardian). Falconstrike Summon (520588),
-- Falcon's Aid (705075), Falcon Dive (806341), Falcon Diving (807119 and 289371) and Phoenix Plumes
-- (520558) summon creature 50393 the same way; an exhaustive Spell.dbc scan for SPELL_EFFECT_SUMMON
-- finds no other spell pointing at either entry. Neither template exists here, so Creature::InitEntry
-- rejects the creature, Spell::SummonGuardian returns on the null summon and only the cast, the 800252
-- slow and the 800250 taunt remain visible.
-- The live creature cache reports both entries as 'War Falcon', creature type 1 (Beast), display 22633
-- at scale 1. Display 22633 already ships with this world (Fjord Hawk 24747) and already has its
-- creature_model_info row, so no model or client data is added.
-- minlevel and maxlevel must be pinned, not a range. Creature::UpdateEntry always calls
-- Creature::SelectLevel, which rolls urand(minlevel, maxlevel) and writes SetBaseWeaponDamage from
-- creature_classlevelstats for the rolled level. Guardian::InitStatsForLevel then re-derives health,
-- armor and attack power from the owner's level, but its petType default branch only re-derives weapon
-- damage for a hardcoded entry list that does not include 50264 or 50393, and Spell::SummonGuardian
-- skips a second InitStatsForLevel when the summon level already equals the caster's. A 1-80 band would
-- therefore freeze a different random white-hit value (damage_base 0.1321 to 47.2377 for unit_class 1)
-- into every falcon. 1/1 with exp 0 is the band every other custom summon template in this fork uses,
-- including the Forest and Brown Falcon companions 116128/116129; actual falcon tuning is the separate
-- Ranger damage-parity audit. unit_class 1 is what Guardian::InitStatsForLevel uses to read
-- creature_classlevelstats; faction 35 is cosmetic because a guardian takes the summoner's faction.
-- apps/codestyle/codestyle-sql.py forbids DELETE FROM `creature_template` and requires a DELETE on the
-- line before every INSERT, so the template rows use the guarded INSERT ... WHERE NOT EXISTS form and
-- the creature_template_model deletes below double as that required preceding statement.
DELETE FROM `creature_template_model` WHERE `CreatureID` = 50264;
INSERT INTO `creature_template`
(`entry`, `name`, `minlevel`, `maxlevel`, `exp`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`)
SELECT 50264, 'War Falcon', 1, 1, 0, 35, 1, 1, 2000, 2000
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50264);
DELETE FROM `creature_template_model` WHERE `CreatureID` = 50393;
INSERT INTO `creature_template`
(`entry`, `name`, `minlevel`, `maxlevel`, `exp`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`)
SELECT 50393, 'War Falcon', 1, 1, 0, 35, 1, 1, 2000, 2000
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50393);
DELETE FROM `creature_template_model` WHERE `CreatureID` IN (50264, 50393);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) VALUES
(50264, 0, 22633, 1, 1),
(50393, 0, 22633, 1, 1);
