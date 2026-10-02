-- SUPERSEDED / reverted same session: attempted to clone the full
-- mechanical field set from Dreadsaber Track (190700, a real WotLK object
-- with a working cast bar) onto our 9 collect-quest interactables, hoping
-- to get a visible cast bar. Even an exact clone (including swapping in
-- Dreadsaber Track's own displayId and removing our AIName) still stayed
-- instant -- ruled out gameobject_template data, the display model, and
-- AIName as the cause. Root cause still unknown; left as an open item for
-- a future session (see coa_codebase_gotchas / goldshire_northshire_restoration
-- memory). This file intentionally reverts everything back to the last
-- known-working state (correct per-object respawn timing close up) rather
-- than applying the failed experiment.
UPDATE `gameobject_template` SET `displayId`=1017889, `size`=0.3, `Data0`=0, `Data3`=2000, `Data5`=1, `Data10`=0, `Data14`=0, `castBarCaption`='Searching', `AIName`='SmartGameObjectAI' WHERE `entry`=2300579;
UPDATE `gameobject_template` SET `displayId`=1061017, `size`=1, `Data0`=0, `Data3`=2000, `Data10`=0, `Data14`=0, `castBarCaption`='Reaching', `AIName`='SmartGameObjectAI' WHERE `entry`=162920;
UPDATE `gameobject_template` SET `size`=1, `Data0`=0, `Data3`=1000, `Data10`=0, `Data14`=0, `castBarCaption`='Picking' WHERE `entry` IN (999013,999014,999015);
UPDATE `gameobject_template` SET `size`=1, `Data0`=0, `Data3`=2000, `Data10`=0, `Data14`=0, `castBarCaption`='Collecting' WHERE `entry` IN (2300520,2300521,2300523);
UPDATE `gameobject_template` SET `size`=1, `Data0`=0, `Data3`=2000, `Data10`=0, `Data14`=0, `castBarCaption`='Purifying' WHERE `entry`=2300522;
