-- #159: Accursed Form (562572) shapeshifts into SpellShapeshiftForm 57 "Cursed Form", which the client
-- ships with CreatureDisplayID 0, so Unit::GetModelForForm returns 0 and the caster keeps their own model.
-- The appearance is authored in the hidden companion 562722 ("Accursed Form", rank "SLS"): an infinite
-- SPELL_AURA_TRANSFORM to creature 55332 plus MECHANIC_POLYMORPH immunity, and nothing ever applied it.
-- Entry/display joins come from the copied client's Creature.dbc (f1/f19): entry 55332 -> display 7654.

-- Display 7654 and model 8088 are in the copied client but absent from the server DBC extract.
-- The native DBC SQL loader reads these overrides; no local binary DBC replacement is needed.
DELETE FROM `creaturemodeldata_dbc` WHERE `ID` = 8088;
INSERT INTO `creaturemodeldata_dbc`
(`ID`, `Flags`, `ModelName`, `ModelScale`, `CollisionWidth`, `CollisionHeight`, `MountHeight`) VALUES
(8088, 0, 'creature\\gargoyle2\\gargoyle2_notusks.mdx', 1, 2.03128004, 1, 0);

DELETE FROM `creaturedisplayinfo_dbc` WHERE `ID` = 7654;
INSERT INTO `creaturedisplayinfo_dbc`
(`ID`, `ModelID`, `CreatureModelScale`, `CreatureModelAlpha`, `TextureVariation_1`, `TextureVariation_2`) VALUES
(7654, 8088, 0.75, 255, 'gargoyle2_black', 'gargoyle2_black_armor');

-- Use the core's default bounds/reach for the newly registered display.
DELETE FROM `creature_model_info` WHERE `DisplayID` = 7654;
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) VALUES
(7654, 0.389, 1.5, 2);

-- SPELL_AURA_TRANSFORM resolves its MiscValue through creature_template and ObjectMgr::ChooseDisplayId.
-- Entry 55332 is a transform-only placeholder; without it the aura falls back to display 16358 (pink pig).
-- Creature type 3 matches SpellShapeshiftForm 57, which already treats the caster as a Demon.
DELETE FROM `creature_template_model` WHERE `CreatureID` = 55332;
INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`)
SELECT `form`.`entry`, `form`.`name`, 1, 1, 35, 1, 3
FROM (SELECT 55332 AS `entry`, 'Accursed Form' AS `name`) AS `form`
WHERE NOT EXISTS (SELECT 1 FROM `creature_template` WHERE `entry` = `form`.`entry`);

DELETE FROM `creature_template_model` WHERE `CreatureID` = 55332;
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) VALUES
(55332, 0, 7654, 1, 1);

-- Ride the companion on the form aura (type 2 = SPELL_LINK_AURA) so the gargoyle model and the Polymorph
-- protection the tooltip promises apply with Accursed Form and drop with it. 562722 has no duration of its
-- own, so the linked removal is what ends it when the 30 second form expires. Form 57 carries no model, and
-- Unit::RestoreDisplayId now falls through such forms to the surviving transform aura, so the gargoyle also
-- holds when Accursed Form replaces a standing Eternal Curse (800157) shapeshift.
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 562572 AND `spell_effect` = 562722 AND `type` = 2;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(562572, 562722, 2, 'CoA Accursed Form - gargoyle transform and Polymorph immunity helper');
