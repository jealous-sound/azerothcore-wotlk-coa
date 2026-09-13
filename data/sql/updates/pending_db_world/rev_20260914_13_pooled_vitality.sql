-- Pooled Vitality's completed empowerment casts release the native nearby-ally heal.
DELETE FROM `spell_bonus_data` WHERE `entry` = 681025;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(681025, 0.25, 0, 0, 0, 'Vitality for Later - Spirit added before native healing bonuses');

DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_ascension_bloodmage_empowered';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(520314, 'spell_ascension_bloodmage_empowered'),
(520836, 'spell_ascension_bloodmage_empowered'),
(520837, 'spell_ascension_bloodmage_empowered'),
(520838, 'spell_ascension_bloodmage_empowered'),
(572895, 'spell_ascension_bloodmage_empowered'),
(572896, 'spell_ascension_bloodmage_empowered'),
(572897, 'spell_ascension_bloodmage_empowered'),
(572898, 'spell_ascension_bloodmage_empowered'),
(801952, 'spell_ascension_bloodmage_empowered'),
(804685, 'spell_ascension_bloodmage_empowered'),
(578304, 'spell_ascension_bloodmage_empowered'),
(578305, 'spell_ascension_bloodmage_empowered'),
(806928, 'spell_ascension_bloodmage_empowered'),
(806929, 'spell_ascension_bloodmage_empowered'),
(806930, 'spell_ascension_bloodmage_empowered'),
(806931, 'spell_ascension_bloodmage_empowered'),
(806932, 'spell_ascension_bloodmage_empowered');
