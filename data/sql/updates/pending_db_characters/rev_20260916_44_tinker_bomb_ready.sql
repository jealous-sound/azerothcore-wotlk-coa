-- Bomb Ready (500354) used to be cast by the Tinker on themselves, where its owner area aura could never
-- apply to anyone. Each cast still left an inert, never-expiring owned aura that was saved on logout.
-- The deployed mine owns the aura now, and a creature's auras are never written to character_aura, so every
-- surviving row for this spell is residue from the old behaviour.
DELETE FROM `character_aura` WHERE `spell` = 500354;
