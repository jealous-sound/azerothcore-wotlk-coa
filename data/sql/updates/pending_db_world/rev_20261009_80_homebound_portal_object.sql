-- #6101: Summon Homebound Portal 93186 (item 97758) is SPELL_EFFECT_TRANS_DOOR with EffectMiscValue 80775, but no
-- gameobject_template row for 80775 exists, so Spell::EffectTransmitted returns without placing anything.
--
-- Source: hertigservices/ascension-data, release exiles-db-export-2026-09-13 (asset
-- coa-public-2026-09-13-tables.tar), table `gameobject`, row id 80775: name "Homebound Portal", type 22
-- (GAMEOBJECT_TYPE_SPELLCASTER), display_id 1026702. The export records no on-use spell for it, so Data0 is the
-- client spell 93187 "Homebound Portal": a dummy effect whose implicit target is the nearest gameobject
-- (TARGET_GAMEOBJECT_NEARBY_ENTRY), which spell_ascension_homebound_portal resolves to this portal. Summon
-- Homebound Portal's description says heroes who step through are sent to the portal owner's Hearthstone.
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`,
`name`, `size`, `Data0`, `Data1`, `Data2`) VALUES
(80775, 22, 1026702, 'Homebound Portal', 1, 93187, 0, 0)
ON DUPLICATE KEY UPDATE `type` = VALUES(`type`), `displayId` = VALUES(`displayId`), `name` = VALUES(`name`),
`size` = VALUES(`size`), `Data0` = VALUES(`Data0`), `Data1` = VALUES(`Data1`), `Data2` = VALUES(`Data2`);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1 AND `SourceEntry` = 93187;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`,
`ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`,
`NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 93187, 0, 0, 31, 0, 5, 80775, 0, 0, 0, 0, '', 'Homebound Portal - targets the Homebound Portal object');

DELETE FROM `spell_script_names` WHERE `spell_id` = 93187 AND `ScriptName` = 'spell_ascension_homebound_portal';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(93187, 'spell_ascension_homebound_portal');
