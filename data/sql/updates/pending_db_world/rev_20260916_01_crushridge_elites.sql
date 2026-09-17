-- The Crushridge ogres holding the Ruins of Alterac are mostly elite on Ascension. The live creature
-- cache reports rank 1 for the Mauler (2254), Mage (2255), Enforcer (2256), Warmonger (2287) and
-- Plunderer (2416), with health modifiers of 3.1-3.2 and a 1.2 mana modifier on the Mage, while this
-- world carried all of them as rank 0 with no modifiers.
-- Crushridge Ogre (2252) and Crushridge Brute (2253) stay normal; the cache reports rank 0 for both.
UPDATE `creature_template` SET `rank` = 1, `HealthModifier` = 3.1 WHERE `entry` IN (2254, 2287, 2416);
UPDATE `creature_template` SET `rank` = 1, `HealthModifier` = 3.2, `ManaModifier` = 1.2 WHERE `entry` = 2255;
UPDATE `creature_template` SET `rank` = 1, `HealthModifier` = 3.2 WHERE `entry` = 2256;
