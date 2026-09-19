-- ScalingStatValues.dbc carries a stale record for character level 20 (ID 61, between the level 10 and level 11
-- records in file order; the level 20 record the rest of the table expects, ID 140, does not exist). Its budget,
-- DPS and spell power columns were never updated past ~level 10 while its armour and trinket columns were, so at
-- level 20 it reads ShoulderBudget 6 (19 -> 11, 21 -> 12), SpellcasterDPS1H 5 (10 / 11), SpellPower 8 (15 / 17),
-- PrimaryBudget 7 (14 / 15), TertiaryBudget 6 (10 / 11) and chest armour 14/23/44/95/127 (level 19 is
-- 18/29/55/119/158). The client never reads it -- its lookup lands on the level 19 record instead, which is what
-- heirloom tooltips show -- but the core indexes the store by Charlevel and picks it up, so every level 20
-- character wearing scaling (heirloom) gear silently loses roughly half of the item's spell power, weapon DPS,
-- intellect/stamina budget and armour, and regains it at 21.
--
-- Override the record with the level 19 values the client itself uses, so applied stats match the tooltip.
DELETE FROM `scalingstatvalues_dbc` WHERE `ID` = 61;
INSERT INTO `scalingstatvalues_dbc` (`ID`, `Charlevel`, `ShoulderBudget`, `TrinketBudget`, `WeaponBudget1H`, `RangedBudget`, `ClothShoulderArmor`, `LeatherShoulderArmor`, `MailShoulderArmor`, `PlateShoulderArmor`, `WeaponDPS1H`, `WeaponDPS2H`, `SpellcasterDPS1H`, `SpellcasterDPS2H`, `RangedDPS`, `WandDPS`, `SpellPower`, `PrimaryBudget`, `TertiaryBudget`, `ClothCloakArmor`, `ClothChestArmor`, `LeatherChestArmor`, `MailChestArmor`, `PlateChestArmor`) VALUES
(61, 20, 11, 11, 7, 6, 22, 41, 89, 118, 11, 16, 10, 11, 12, 11, 15, 14, 10, 18, 29, 55, 119, 158);
