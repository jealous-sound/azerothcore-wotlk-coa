-- Standardized respawn timers across all collect/interact objects (they had
-- inconsistent leftover values -- Mirror Shard 17s, Lost Pages 19-42s each).
-- All set to 10s except Apple, which stays at 5s per user request.
UPDATE `gameobject` SET `spawntimesecs`=10 WHERE `id` IN (162920,999013,999014,9900740,9900741,9900742,9900743,9900730);
UPDATE `gameobject` SET `spawntimesecs`=5 WHERE `id`=999015;
