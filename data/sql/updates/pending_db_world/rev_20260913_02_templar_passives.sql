-- Armor of Faith: the active parent specifies 20% AP plus 50% Holy spell power.
-- Preserve native base/per-level damage, area targeting, mitigation and bonus threat.
DELETE FROM `spell_bonus_data` WHERE `entry` = 801546;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(801546, 0.5, 0, 0.2, 0, 'Armor of Faith: Staffguard break damage');
