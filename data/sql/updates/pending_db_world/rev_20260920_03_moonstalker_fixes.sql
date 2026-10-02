-- CoA/Ascension: Moonstalker Matriarch (2071) + Runt (2070) -- Darkshore
-- Issue #4348: Matriarch currently summons 1 Runt on aggro; on CoA she had 2 Runts
-- pre-spawned before combat. The Exiles DB confirms the Matriarch casts spell 8594
-- (Summon Moonstalker Runt) and is Normal-rank level 19-20 (unchanged).
-- Source: hertigservices/ascension-data Exiles DB, NPC 2071.
-- Issue #4347: Summoned Runts persisted after Matriarch died; on CoA they despawned.
--
-- Fix: replace the on-aggro summon with two on-respawn summons (MANUAL_DESPAWN) and
-- add an on-death handler that force-despawns all Moonstalker Runts within 20 yards.

DELETE FROM `smart_scripts` WHERE `entryorguid` = 2071 AND `source_type` = 0;
INSERT INTO `smart_scripts` (
    `entryorguid`, `source_type`, `id`, `link`, `event_type`,
    `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`,
    `event_param3`, `event_param4`, `event_param5`, `event_param6`, `action_type`,
    `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`,
    `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`,
    `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`,
    `comment`) VALUES
(2071, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 0, 12, 2070,
 8, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
 'Moonstalker Matriarch - On Respawn - Summon Moonstalker Runt'),
(2071, 0, 1, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 0, 12, 2070,
 8, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
 'Moonstalker Matriarch - On Respawn - Summon Moonstalker Runt'),
(2071, 0, 2, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 0, 41, 0,
 0, 0, 0, 0, 0, 9, 2070, 0, 20, 0, 0, 0, 0, 0,
 'Moonstalker Matriarch - On Just Died - Despawn Nearby Moonstalker Runts');
