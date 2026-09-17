-- Norgannon's Wrath (524739) deals ${524740m1 + ppl + 100% Holy spell power + 100% attack power}. The module now adds
-- both terms to the blast helper 524740, so no core coefficient may apply on top of them.
DELETE FROM `spell_bonus_data` WHERE `entry` = 524740;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(524740, 0, 0, 0, 0, 'Templar: explicit coefficient or actual result');
