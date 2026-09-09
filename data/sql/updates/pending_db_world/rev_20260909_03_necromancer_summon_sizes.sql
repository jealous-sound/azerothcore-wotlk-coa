-- Necromancer summon sizes. Apply with the Life Force follow-up source/UI package.
-- Local appearance policy: model geometry and authored display scale were reviewed.
-- Change only these summon template links; shared display/model-info rows stay intact.
-- Before installation require each row to match its previous or target value.
-- Foul Invocation: approximate height 13.71 -> 4.11 model units.
UPDATE `creature_template_model` SET `DisplayScale` = 0.3
WHERE `CreatureID` = 444914 AND `Idx` = 0 AND `CreatureDisplayID` = 310061
    AND `DisplayScale` = 1;
-- Icebound Champion: approximate height 7.55 -> 3.02 model units.
UPDATE `creature_template_model` SET `DisplayScale` = 0.4
WHERE `CreatureID` = 500484 AND `Idx` = 0 AND `CreatureDisplayID` = 30810
    AND `DisplayScale` = 1;
-- Abomination: approximate height 7.09 -> 3.55 model units.
UPDATE `creature_template_model` SET `DisplayScale` = 0.5
WHERE `CreatureID` = 50068 AND `Idx` = 0 AND `CreatureDisplayID` = 15958
    AND `DisplayScale` = 1;
-- Decaying Colossus: approximate height 16.35 -> 4.09 model units.
UPDATE `creature_template_model` SET `DisplayScale` = 0.25
WHERE `CreatureID` = 50115 AND `Idx` = 0 AND `CreatureDisplayID` = 95426
    AND `DisplayScale` = 1;
-- Frost Wyrm: approximate height 5.43 -> 2.72 model units.
UPDATE `creature_template_model` SET `DisplayScale` = 0.5
WHERE `CreatureID` = 50177 AND `Idx` = 0 AND `CreatureDisplayID` = 27064
    AND `DisplayScale` = 1;
-- Bone Wraith: approximate height 7.55 -> 3.02 model units.
UPDATE `creature_template_model` SET `DisplayScale` = 0.4
WHERE `CreatureID` = 50309 AND `Idx` = 0 AND `CreatureDisplayID` = 30810
    AND `DisplayScale` = 1;
-- Tomb King: approximate height 4.56 -> 3.42 model units.
UPDATE `creature_template_model` SET `DisplayScale` = 0.75
WHERE `CreatureID` = 50320 AND `Idx` = 0 AND `CreatureDisplayID` = 94932
    AND `DisplayScale` = 1;
-- Crypt Fiend: approximate height 5.53 -> 2.77 model units.
UPDATE `creature_template_model` SET `DisplayScale` = 0.5
WHERE `CreatureID` = 50323 AND `Idx` = 0 AND `CreatureDisplayID` = 17308
    AND `DisplayScale` = 1;
-- Plaguefather: approximate height 11.26 -> 3.94 model units.
UPDATE `creature_template_model` SET `DisplayScale` = 0.35
WHERE `CreatureID` = 50385 AND `Idx` = 0 AND `CreatureDisplayID` = 94473
    AND `DisplayScale` = 1;
-- Rotting Frost Giant: approximate height 22.01 -> 4.40 model units.
UPDATE `creature_template_model` SET `DisplayScale` = 0.2
WHERE `CreatureID` = 51275 AND `Idx` = 0 AND `CreatureDisplayID` = 402059
    AND `DisplayScale` = 1;
-- Zombie: approximate height 8.23 -> 2.47 model units.
UPDATE `creature_template_model` SET `DisplayScale` = 0.3
WHERE `CreatureID` = 523032 AND `Idx` = 0 AND `CreatureDisplayID` = 25495
    AND `DisplayScale` = 1;
