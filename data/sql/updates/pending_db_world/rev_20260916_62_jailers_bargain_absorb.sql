-- Jailer's Bargain (805718) promises "a shield that absorbs damage equal to 30% of your maximum
-- health". Its SPELL_AURA_SCHOOL_ABSORB effect carries EffectBasePoints 0 and no scaling of any
-- kind, so the aura landed with one point of absorption and popped on the first hit taken.
--
-- Nothing in the DBC can express "percentage of the caster's maximum health", so the amount is
-- computed in spell_ascension_jailers_bargain.
DELETE FROM `spell_script_names` WHERE `spell_id` = 805718 AND `ScriptName` = 'spell_ascension_jailers_bargain';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(805718, 'spell_ascension_jailers_bargain');
