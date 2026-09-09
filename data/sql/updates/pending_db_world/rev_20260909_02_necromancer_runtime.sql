-- Recovered champion display references have no corresponding client/server display rows.
-- Use existing skeletal warrior, mage and bone wraith appearances for this local reconstruction.
-- Keep the already applied completion migration immutable and preserve other template fields.
UPDATE `creature_template_model` SET `CreatureDisplayID` = 775
WHERE `CreatureID` = 500482 AND `Idx` = 0 AND `CreatureDisplayID` = 561889;
UPDATE `creature_template_model` SET `CreatureDisplayID` = 11396
WHERE `CreatureID` = 500483 AND `Idx` = 0 AND `CreatureDisplayID` = 561890;
UPDATE `creature_template_model` SET `CreatureDisplayID` = 30810
WHERE `CreatureID` = 500484 AND `Idx` = 0 AND `CreatureDisplayID` = 561888;
