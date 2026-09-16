-- #118: every War Falcon summon fails because the summoned creatures do not exist in this world.
-- Falcon's Call (804715) triggers Falcon's Call (800251), whose SPELL_EFFECT_SUMMON asks for creature
-- 50264 with SummonProperties 61 (Category 1 ally, Type 2 guardian). Falconstrike Summon (520588),
-- Falcon's Aid (705075), Falcon Dive (806341) and Falcon Diving (807119) summon creature 50393 the same
-- way. Neither template exists here, so Creature::InitEntry rejects the creature, Spell::SummonGuardian
-- returns on the null summon and only the cast, the 800252 slow and the 800250 taunt remain visible.
-- The live creature cache reports both entries as 'War Falcon', creature type 1 (Beast), display 22633
-- at scale 1. Display 22633 already ships with this world (Fjord Hawk 24747) and already has its
-- creature_model_info row, so no model or client data is added. A guardian takes the summoner's faction
-- and level, so faction 35 and the 1-80 range only have to be valid; unit_class 1 is what
-- Guardian::InitStatsForLevel uses to read creature_classlevelstats.
DELETE FROM `creature_template_model` WHERE `CreatureID` = 50264;
INSERT INTO `creature_template`
(`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`)
SELECT 50264, 'War Falcon', 1, 80, 35, 1, 1, 2000, 2000
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50264);
DELETE FROM `creature_template_model` WHERE `CreatureID` = 50393;
INSERT INTO `creature_template`
(`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`, `BaseAttackTime`, `RangeAttackTime`)
SELECT 50393, 'War Falcon', 1, 80, 35, 1, 1, 2000, 2000
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 50393);
DELETE FROM `creature_template_model` WHERE `CreatureID` IN (50264, 50393);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) VALUES
(50264, 0, 22633, 1, 1),
(50393, 0, 22633, 1, 1);
