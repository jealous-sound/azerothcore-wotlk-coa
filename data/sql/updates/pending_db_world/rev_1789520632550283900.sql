-- CoA Stoneform: trigger the copied 10% armor helper, matching the stock racial's spell link.
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 1120594 AND `spell_effect` = 1165116 AND `type` = 0;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(1120594, 1165116, 0, 'CoA Stoneform - armor bonus');
