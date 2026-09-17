-- Red Burlap Bandana (752) always drops from Defias Thug (38) while the quest needs it, as on Ascension
-- (db.exil.es export of 2026-09-13: 100%; AzerothCore data: 60%).
UPDATE `creature_loot_template` SET `Chance` = 100 WHERE `Entry` = 38 AND `Item` = 752;
