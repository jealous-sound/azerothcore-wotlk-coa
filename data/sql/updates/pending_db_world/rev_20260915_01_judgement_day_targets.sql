-- Judgement Day: exclude critters from effect 0 while retaining the normal enemy checks.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1 AND `SourceEntry` = 806121 AND `SourceId` = 0 AND `ElseGroup` = 0 AND `ConditionTypeOrReference` = 24 AND `ConditionTarget` = 0 AND `ConditionValue1` = 8;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 806121, 0, 0, 24, 0, 8, 0, 0, 1, 0, 0, '', 'Judgement Day - target must not be a critter');
