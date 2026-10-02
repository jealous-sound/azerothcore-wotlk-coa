-- Mirror Shard (162920, rev_20260927_02) never actually summoned Curse
-- Shard (162919) on use -- same two-bug combo found and fixed for the
-- Kobold Warren (rev_20260929_19):
-- 1. `Data3` (goober.autoCloseTime) was 0, so GameObject::Use()'s GOOBER
--    case never called SetLootState(GO_ACTIVATED) at all (that call is
--    gated behind `info->GetAutoCloseTime()` being nonzero -- see
--    GameObject.cpp ~line 1680), meaning the SmartAI GO_STATE_CHANGED hook
--    never fired on use no matter what state value it listened for.
-- 2. The listening smart_scripts row used event_param1=0, but the value
--    GameObject::SetLootState() actually passes through on use is the
--    LootState `GO_ACTIVATED` = 2 (0 would be GO_NOT_READY, never reached
--    from a normal use).
-- Tried Data0 (lockId) for a visible cast bar; broke working respawn
-- behavior instead and was reverted -- see rev_20260929_19's comment.
UPDATE `gameobject_template` SET `Data3`=2000, `castBarCaption`='Reaching' WHERE `entry`=162920;
UPDATE `smart_scripts` SET `event_param1`=2 WHERE `entryorguid`=162920 AND `source_type`=1 AND `id`=0;
