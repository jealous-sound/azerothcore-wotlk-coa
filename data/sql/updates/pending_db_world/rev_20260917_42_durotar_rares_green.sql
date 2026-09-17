-- Watch Commander Zalaphil (5809), Warlord Kolkanis (5808) and Geolord Mottle (5826) drew one reference among
-- two white tables and the green table (group 6). Roll the green table on its own so every kill gives a green,
-- still with one white from group 6 and the world loot from group 5.
UPDATE `creature_loot_template` SET `GroupId` = 0, `Chance` = 100
WHERE `Entry` IN (5808, 5809, 5826) AND `Item` = 4 AND `Reference` = 1020812;
