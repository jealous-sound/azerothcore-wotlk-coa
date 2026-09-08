-- Native proc-mask normalization for the installed Guardian, Barbarian and Witch Hunter packages.
-- Clear fields the native loader cannot use; preserve event flags, chance, cooldown, charges and bindings.
UPDATE `spell_proc` SET `SpellTypeMask` = 0 WHERE `SpellTypeMask` = 1 AND `SpellId` IN (524932, 562027, 573064, 573077, 573224, 573225, 681329, 681488, 706637, 707064, 801774, 804192, 805780, 805804);
UPDATE `spell_proc` SET `SpellPhaseMask` = 0 WHERE `SpellPhaseMask` = 2 AND `SpellId` IN (300927, 500061, 503659, 504910, 524932, 560093, 560364, 562027, 573064, 573077, 573224, 573225, 578336, 582901, 653281, 653386, 653410, 680497, 681092, 681152, 681329, 681488, 705534, 706514, 706637, 707064, 707137, 707170, 707764, 801121, 801549, 801774, 802188, 803432, 804192, 804383, 804891, 805780, 805804, 805821, 806195);
UPDATE `spell_proc` SET `HitMask` = 12287 WHERE `HitMask` = 16383 AND `SpellId` IN (504910, 680494, 681181, 681218, 681219, 681220, 681221, 681540, 707170, 802188);
