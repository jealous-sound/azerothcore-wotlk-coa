-- CoA Runeshroud: apply the movement speed reduction its tooltip names ($808090s1) for as long as Runeshroud lasts.
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 500288 AND `spell_effect` = 808090 AND `type` = 2;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(500288, 808090, 2, 'CoA Runeshroud - movement speed reduction');
