-- Bianca's (161700) periodic yell was using Type=1 (CHAT_MSG_SAY, a PLAYER
-- say type) instead of Type=14 (CHAT_MSG_MONSTER_YELL). Same root cause as
-- the Arcane Projection of Aliscar dialogue bug: creature_text.Type casts
-- directly to the full ChatMsg protocol enum, not a simplified 0/1/2
-- SAY/YELL/EMOTE scheme. See rev_20260929_15's comment for the source
-- reference (CreatureTextMgr::LoadCreatureTexts).
UPDATE `creature_text` SET `Type`=14 WHERE `CreatureID`=161700 AND `GroupID`=0 AND `ID`=0;
