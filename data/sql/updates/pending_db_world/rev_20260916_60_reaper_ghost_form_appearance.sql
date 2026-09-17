-- Ghost Form (561083) is a plain SPELL_AURA_TRANSFORM to creature 841213, and that entry has no
-- creature_template row. AuraEffect::HandleAuraTransform falls back to SetDisplayId(16358) for a
-- missing template, so the form showed the pink pig placeholder and logged
--   "Auras: unknown creature id = 841213 (only need its modelid) From Spell Aura Transform in Spell ID = 561083"
--
-- Entry -> display comes from the copied client's Creature.dbc: 841213 has no row of its own, and the
-- ghost the tooltip describes is display 5430 (creature 4308 "Unfettered Spirit"), CreatureModelData 9
-- 'Creature\Ghost\Ghost.mdx' with texture GhostSkin at 0.75 scale. Both the display and its
-- creature_model_info row already ship with the server, so nothing has to be registered for it.
--
-- flags_extra stays 0 on purpose. ObjectMgr::ChooseDisplayId short-circuits to
-- GetFirstInvisibleModel() for CREATURE_FLAG_EXTRA_TRIGGER (0x80), which would hand the transform
-- CreatureModel::DefaultInvisibleModel and make the caster vanish instead of turning into a ghost.
DELETE FROM `creature_template_model` WHERE `CreatureID` = 841213;
DELETE FROM `creature_template` WHERE `entry` = 841213;

INSERT INTO `creature_template` (`entry`, `name`, `minlevel`, `maxlevel`, `faction`, `unit_class`, `type`) VALUES
(841213, 'Ghost Form', 1, 1, 35, 1, 6);

INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) VALUES
(841213, 0, 5430, 1, 1);

-- Ghost Form is authored as two spells. 561083 carries the transform, the water walking and the
-- movement speed; the hidden companion 561087 (rank "SLS") carries SPELL_AURA_FEATHER_FALL, which is
-- the "slowing your falling speed" half of 561083's own tooltip. Nothing applied it, so the form took
-- full falling damage. Ride it on the form with SPELL_LINK_AURA (type 2) so it applies and drops with it.
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 561083 AND `spell_effect` = 561087 AND `type` = 2;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(561083, 561087, 2, 'CoA Ghost Form - feather fall helper so the form does not take falling damage');
