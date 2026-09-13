DELETE FROM `spell_script_names` WHERE `ScriptName` = 'aura_ascension_spiritual_reflexes';
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'aura_ascension_blessed_by_earth';
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'aura_ascension_mountain_threshold';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(705436, 'aura_ascension_spiritual_reflexes'),
(300695, 'aura_ascension_blessed_by_earth'),
(806068, 'aura_ascension_mountain_threshold');

DELETE FROM `spell_proc` WHERE `SpellId` IN (705436, 300695);
INSERT INTO `spell_proc`
(`SpellId`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `Chance`) VALUES
(705436, 40, 1, 2, 16, 0, 100),
(300695, 332116, 1, 2, 3, 2, 100);

DELETE FROM `spell_bonus_data` WHERE `entry` = 573050;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(573050, 0.1, 0, 0.05, 0, 'Spiritual Reflexes - Soul Harvest leech');
