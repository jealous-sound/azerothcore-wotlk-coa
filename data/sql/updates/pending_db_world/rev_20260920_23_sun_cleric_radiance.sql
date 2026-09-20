-- Sun Cleric class-audit batch, group D (radiance).
--
-- #2442 Champion's Arrival (704905): Spell.dbc's own SPELLMOD_DURATION classMask on 704905
-- matches the Chains of Light debuff (806697) but not Champion of the Sun (800612, whose own
-- SpellFamilyFlags_1 0x20 hits neither of 704905's two remaining classMask bits -- see
-- AscensionSunClericRadiance.cpp for the full evidence trail). aura_ascension_champion_of_the_sun_arrival
-- extends 800612's own duration by 5 sec on apply when the caster knows 704905.
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'aura_ascension_champion_of_the_sun_arrival';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(800612, 'aura_ascension_champion_of_the_sun_arrival');
