-- Character Advancement spec-switch spells: "Specialization I" ... "Specialization XX"
-- (SPEC_SWAP_SPELLS in the client's FrameXML Constants.lua). The client casts one of these to
-- switch its specification slot; spell_ascension_spec_swap performs the switch server-side and
-- answers with the advancement state (0x0725 + 0x0726).
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_ascension_spec_swap';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(979993, 'spell_ascension_spec_swap'),
(979994, 'spell_ascension_spec_swap'),
(979995, 'spell_ascension_spec_swap'),
(979996, 'spell_ascension_spec_swap'),
(979997, 'spell_ascension_spec_swap'),
(979986, 'spell_ascension_spec_swap'),
(979987, 'spell_ascension_spec_swap'),
(979988, 'spell_ascension_spec_swap'),
(84874, 'spell_ascension_spec_swap'),
(84876, 'spell_ascension_spec_swap'),
(84878, 'spell_ascension_spec_swap'),
(84880, 'spell_ascension_spec_swap'),
(84882, 'spell_ascension_spec_swap'),
(84884, 'spell_ascension_spec_swap'),
(84886, 'spell_ascension_spec_swap'),
(84888, 'spell_ascension_spec_swap'),
(84890, 'spell_ascension_spec_swap'),
(84892, 'spell_ascension_spec_swap'),
(84894, 'spell_ascension_spec_swap'),
(84896, 'spell_ascension_spec_swap');
