-- mod-playerbots core hooks load CharSections.dbc and EmotesTextSound.dbc with a database override table.
-- Create the two empty override tables so the server starts without the module.
CREATE TABLE IF NOT EXISTS `charsections_dbc` (
  `Id` INT NOT NULL DEFAULT '0',
  `Race` INT NOT NULL DEFAULT '0',
  `Gender` INT NOT NULL DEFAULT '0',
  `GenType` INT NOT NULL DEFAULT '0',
  `TexturePath1` VARCHAR(100) DEFAULT NULL,
  `TexturePath2` VARCHAR(100) DEFAULT NULL,
  `TexturePath3` VARCHAR(100) DEFAULT NULL,
  `Flags` INT NOT NULL DEFAULT '0',
  `Type` INT NOT NULL DEFAULT '0',
  `Color` INT NOT NULL DEFAULT '0',
  PRIMARY KEY (`Id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 ROW_FORMAT=DYNAMIC;

CREATE TABLE IF NOT EXISTS `emotetextsound_dbc` (
  `Id` INT NOT NULL DEFAULT '0',
  `EmotesTextId` INT NOT NULL DEFAULT '0',
  `RaceId` INT NOT NULL DEFAULT '0',
  `SexId` INT NOT NULL DEFAULT '0',
  `SoundId` INT NOT NULL DEFAULT '0',
  PRIMARY KEY (`Id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 ROW_FORMAT=DYNAMIC;
