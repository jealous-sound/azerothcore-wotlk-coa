-- Aldia Crayon and Lady Agria Spada go back to the revamp's (#5764) Ascension displays 652414 and 652415. They are not
-- in the client's CreatureDisplayInfo.dbc, but the server now streams the rows the client lacks (#5794).
UPDATE `creature_template_model` SET `CreatureDisplayID` = 652414 WHERE `CreatureID` = 162802 AND `Idx` = 0;
UPDATE `creature_template_model` SET `CreatureDisplayID` = 652415 WHERE `CreatureID` = 162803 AND `Idx` = 0;
