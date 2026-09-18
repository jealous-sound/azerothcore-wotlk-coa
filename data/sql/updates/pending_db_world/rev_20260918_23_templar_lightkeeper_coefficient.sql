-- Lightkeeper (#2021): the burst 561277 has no spell_bonus_data row, so SpellDamageBonusDone adds no attack power
-- term. Its description states "+$AP*.22" and names no spell power term. The helper is SPELL_DAMAGE_CLASS_MAGIC
-- with no ranged mask and no weapon requirement, so the attack power term resolves to melee attack power.
START TRANSACTION;
DELETE FROM `spell_bonus_data` WHERE `entry` = 561277;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(561277, 0, 0, 0.22, 0, 'Local Templar: Lightkeeper - burst damage, attack power term from its description');
COMMIT;
