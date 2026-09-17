-- Preflight must confirm that entry 994300 is free or already belongs to this module.
-- Lock 43 works with Opening (22810) in the reference Ascension client.
INSERT INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`size`,`Data0`,`Data1`,`ScriptName`)
VALUES (994300,3,259,'High-Risk Spoils',1,43,0,'highrisk_chest')
ON DUPLICATE KEY UPDATE `type`=3,`displayId`=259,`name`='High-Risk Spoils',`size`=1,
  `Data0`=43,`Data1`=0,`ScriptName`='highrisk_chest';
