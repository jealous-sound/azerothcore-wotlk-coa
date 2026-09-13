-- The active Manuscription description specifies 38% spell power on each native DoT tick.
DELETE FROM `spell_bonus_data` WHERE `entry` = 525302;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(525302, 0, 0.38, 0, 0, 'Manuscription: 38% spell power per tick');
