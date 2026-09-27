-- The Ranger's two "Adaptation" buff pairs are each one buff with a single-target and a raid-wide
-- version, and every single-target rank's own tooltip says "Does not stack with other similar effects"
-- (Footpad's Adaptation 523493 and its ranks 523489-523493). At max rank 523493 grants exactly what
-- Greater Footpad's Adaptation 523513 grants (armor 285 and 12 to every primary attribute), and
-- 803666 (Woodsman's Adaptation rank 7) exactly what Greater Woodsman's Adaptation 680294 grants
-- (232 melee and ranged attack power), so casting both on the same target doubled the buff. #5000 and
-- #5011 are the two halves of that: Woodsman's plus Greater, and Footpad's plus Greater.
--
-- aura_ascension_ranger_adaptation drops the rival from the buffed unit whenever one of the four lands
-- there, which is the same "newest buff wins" rule ApplyRangerQuiver already applies to the quivers.
-- It runs from AfterEffectApply, so a single-target cast clears the raid-wide buff from that one target
-- and a raid-wide cast clears it from every target it reaches, because an area aura applies once per
-- target. The two families are independent - Footpad's grants armor and stats, Woodsman's grants attack
-- power - so only the within-pair rival is removed.
--
-- The two rank chains are bound with a negative spell_id, which ObjectMgr::LoadSpellScriptNames expands to
-- every rank from the first one, so a new rank needs no row here.
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'aura_ascension_ranger_adaptation';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(-523489, 'aura_ascension_ranger_adaptation'),
(523513, 'aura_ascension_ranger_adaptation'),
(-800266, 'aura_ascension_ranger_adaptation'),
(680294, 'aura_ascension_ranger_adaptation');
