-- Restore the attack-power coefficients encoded in the copied Ascension
-- client's Ram and Pulverize tooltips. Without these rows AzerothCore applies
-- only the small base points from Spell.dbc (for example, 4 instead of the
-- roughly 32 shown for rank-one Pulverize on the level-10 test character).

DELETE FROM `spell_bonus_data` WHERE `entry` IN (573204, 573205, 573206, 573207, 573208, 573209, 573210, 573211, 802284, 573286, 573292, 800311, 802439, 802440, 802441, 802442, 802443);
INSERT INTO `spell_bonus_data`
    (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`)
VALUES
    (573204, 0, 0, 0.385, 0, 'Guardian - Ram'),
    (573205, 0, 0, 0.385, 0, 'Guardian - Ram'),
    (573206, 0, 0, 0.385, 0, 'Guardian - Ram'),
    (573207, 0, 0, 0.385, 0, 'Guardian - Ram'),
    (573208, 0, 0, 0.385, 0, 'Guardian - Ram'),
    (573209, 0, 0, 0.385, 0, 'Guardian - Ram'),
    (573210, 0, 0, 0.385, 0, 'Guardian - Ram'),
    (573211, 0, 0, 0.385, 0, 'Guardian - Ram'),
    (802284, 0, 0, 0.385, 0, 'Guardian - Ram'),
    (573286, 0, 0, 0.355, 0, 'Guardian - Pulverize'),
    (573292, 0, 0, 0.355, 0, 'Guardian - Pulverize'),
    (800311, 0, 0, 0.355, 0, 'Guardian - Pulverize'),
    (802439, 0, 0, 0.355, 0, 'Guardian - Pulverize'),
    (802440, 0, 0, 0.355, 0, 'Guardian - Pulverize'),
    (802441, 0, 0, 0.355, 0, 'Guardian - Pulverize'),
    (802442, 0, 0, 0.355, 0, 'Guardian - Pulverize'),
    (802443, 0, 0, 0.355, 0, 'Guardian - Pulverize');
