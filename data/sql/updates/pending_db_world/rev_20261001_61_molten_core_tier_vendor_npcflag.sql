-- Major Mattingly (14394) and Overlord Runthak (14392) carry npcflag=3 (GOSSIP 0x1 +
-- QUESTGIVER 0x2 -- UnitDefines.h) and no UNIT_NPC_FLAG_VENDOR (0x80): diag-P-token-exchange.md's
-- "already has the gossip+vendor npcflag" was checked directly against the flag values and was
-- wrong -- the vendor bit was never set, so the vendor window could not open regardless of
-- npc_vendor content. Adds 0x80, keeping the existing gossip/questgiver bits (3 | 128 = 131).
UPDATE `creature_template` SET `npcflag` = 131 WHERE `entry` IN (14392, 14394);
