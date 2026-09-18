-- Quest 11180: the Witch Hill Information Credit must be cast by the killer, not by the dying Risen Spirit/Husk,
-- since the spell credits its caster (TARGET_UNIT_CASTER). SMART_ACTION_INVOKER_CAST (134) makes the invoker cast it.
START TRANSACTION;
UPDATE `smart_scripts` SET `action_type` = 134
WHERE `source_type` = 0 AND `entryorguid` IN (23554, 23555) AND `id` = 2 AND `action_type` = 11 AND `action_param1` = 42512;
COMMIT;
