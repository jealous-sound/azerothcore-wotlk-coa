-- Weapon speed as CoA's own server sent it to its clients, from the pooled item query caches the
-- hertigservices/ascension-data project splits per realm (CoA's slice, release
-- data-cache-945cd3b8b0ce4a496586).
--
-- Comparing 437,990 items against that source found our item_template essentially exact: weapon
-- average damage and armour both come out at a median ratio of 1.0000. Only these three weapons
-- disagree on `delay`, and delay is the one field a disagreement cannot be explained away by: CoA
-- scales an item's damage, armour and stats with its item level through ItemStat.dbc, so a captured
-- stat block or item level belongs to the observed instance rather than the template, but weapon
-- speed is not in that table and is fixed per item. Each of these three is corroborated by 18 to 28
-- separate client captures.
--
-- A fourth disagreement, Fishing Pole (1202041, ours 2800 against 3000 recorded), rests on a single
-- capture and is left alone.
START TRANSACTION;
UPDATE `item_template` SET `delay` = 3700 WHERE `entry` = 559980; -- Last Howl: 3300 -> 3700, 28 captures
UPDATE `item_template` SET `delay` = 990 WHERE `entry` = 560135; -- Ealdfrost Claw: 1300 -> 990, 18 captures
UPDATE `item_template` SET `delay` = 3500 WHERE `entry` = 560184; -- Witchbane Crossbow: 3400 -> 3500, 26 captures
COMMIT;
