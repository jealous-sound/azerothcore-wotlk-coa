-- Brood Trap (803525) summons 52121, which live shows as the aqir egg cluster 412059 (client creature cache), not the spider egg 23058.
UPDATE `creature_template_model` SET `CreatureDisplayID` = 412059 WHERE `CreatureID` = 52121 AND `Idx` = 0;
DELETE FROM `creature_model_info` WHERE `DisplayID` = 412059;
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `Gender`) VALUES
(412059, 0.5, 1, 2);
