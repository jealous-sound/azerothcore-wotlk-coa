-- Soul Harvester (804311) triggers its runic power buff (804312) when its owner lands a killing blow on a target that
-- yields experience or honor. The spell carries no proc flags, so nothing ever triggered it.
DELETE FROM `spell_proc` WHERE `SpellId` = 804311;
INSERT INTO `spell_proc` (`SpellId`, `ProcFlags`, `Chance`) VALUES (804311, 2, 100);
