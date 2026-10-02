-- 4 more "collect X" quest interactables were instant-use with no cast bar
-- (empty castBarCaption, near-zero Data3), matching the same missing-cast-
-- time issue found and fixed on Mirror Shard/Kobold Warren
-- (rev_20260929_19/20). Fixed with a proper 2s cast bar + made them
-- consumable (Data5=1) so they despawn/respawn on the object's own
-- spawntimesecs instead of instantly resetting.
--
-- Tried Data0 (lockId) for a visible cast bar; broke working respawn
-- behavior instead and was reverted -- see rev_20260929_19's comment.
--
-- Goldshire's Generosity (1660059) harvest nodes: 1s cast.
UPDATE `gameobject_template` SET `castBarCaption`='Picking', `Data3`=1000, `Data5`=1 WHERE `entry` IN (999013,999014,999015);

-- Accursed Sisterhood (1660003) purify-items: 2s cast.
UPDATE `gameobject_template` SET `castBarCaption`='Collecting', `Data3`=2000, `Data5`=1 WHERE `entry` IN (2300520,2300521,2300523);
UPDATE `gameobject_template` SET `castBarCaption`='Purifying', `Data3`=2000, `Data5`=1 WHERE `entry`=2300522;

-- Abbess's Staff (2300521) had spawntimesecs=0 (would never respawn once
-- consumable); normalized to 300s like its siblings.
UPDATE `gameobject` SET `spawntimesecs`=300 WHERE `id`=2300521;
