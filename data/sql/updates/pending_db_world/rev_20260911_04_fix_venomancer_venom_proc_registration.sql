-- Register the venom proc-trigger AuraScript against its six Activation spells.
-- `aura_ascension_venomancer_venom_proc` (AscensionVenomancerVenomProcs.cpp) exists and
-- compiles, but was never linked via `spell_script_names`, unlike its sibling
-- `aura_ascension_venomancer_venom_selection` (see rev_20260907_39_venomancer_venoms.sql,
-- which registers that other script for these same six spell ids). Without this row the
-- proc's OnEffectProc/Trigger() handler never runs, so none of the six Venomancer venom
-- effects (heal/haste/slow/damage/casting-speed/attack-power) can ever apply -- confirmed
-- against the live Spell.dbc data for 805731/805895 (and siblings), whose shapes already
-- match what AscensionVenomancerVenomProcs.cpp expects.
START TRANSACTION;
DELETE FROM `spell_script_names` WHERE `spell_id` IN (630868, 805731, 805775, 805776, 805777, 805778) AND `ScriptName` = 'aura_ascension_venomancer_venom_proc';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(630868, 'aura_ascension_venomancer_venom_proc'),
(805731, 'aura_ascension_venomancer_venom_proc'),
(805775, 'aura_ascension_venomancer_venom_proc'),
(805776, 'aura_ascension_venomancer_venom_proc'),
(805777, 'aura_ascension_venomancer_venom_proc'),
(805778, 'aura_ascension_venomancer_venom_proc');
COMMIT;
