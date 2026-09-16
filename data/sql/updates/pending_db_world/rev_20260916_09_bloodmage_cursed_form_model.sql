-- Bloodmage Eternal Curse (800157) is supposed to turn the caster into a worgen. Its third effect is
-- SPELL_AURA_TRANSFORM (aura 56) with misc value 56332, and shapeshift form 57 "Cursed Form" carries no
-- CreatureDisplayID of its own, so that transform supplies the entire model. Ascension's client Creature.dbc
-- records entry 56332 as "Tank Worgen Form" with CreatureDisplayID[0] = 574, which resolves through
-- CreatureDisplayInfo 574 to CreatureModelData 44, Creature\Worgen\Worgen.mdx. This world had no
-- creature_template row for 56332 at all, so AuraEffect::HandleAuraTransform hit the stock unknown-creature
-- fallback and set display 16358, the pink pig. creature_model_info already carries DisplayID 574
-- (BoundingRadius 1.0491, CombatReach 1.95), so the model survives ObjectMgr::CheckCreatureTemplate as is.
-- The entry exists purely as a display lookup for the transform aura and is never spawned.
DELETE FROM `creature_template_model` WHERE `CreatureID` = 56332;
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`)
SELECT 56332, 'Tank Worgen Form', 1, 1, 35, 1, 7
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = 56332);
DELETE FROM `creature_template_model` WHERE `CreatureID` = 56332;
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) VALUES
(56332, 0, 574, 1, 1);
