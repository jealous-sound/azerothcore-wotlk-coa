/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionQuestLog.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptMgr.h"

namespace AscensionQuestLog
{
namespace
{
WorldPacket BuildField(ObjectGuid guid, uint32 field, uint32 value)
{
    WorldPacket data(UpdateObjectAddonOpcode, 16);
    data << guid << field << value;
    return data;
}

uint32 OfferedRewardXP(Player* player, Quest const* quest)
{
    uint32 xp = sScriptMgr->OnPlayerShouldBeRewardedWithMoneyInsteadOfExp(player)
        ? 0 : player->CalculateQuestRewardXP(quest);
    sScriptMgr->OnPlayerQuestComputeXP(player, quest, xp);
    return xp;
}
}

WorldPacket BuildRewardXP(ObjectGuid guid, uint16 slot, uint32 rewardXP)
{
    return BuildField(guid, RewardXPField + slot, rewardXP);
}

WorldPacket BuildLevel(ObjectGuid guid, uint16 slot, uint32 level)
{
    return BuildField(guid, LevelField + slot, level);
}

void SendSlot(Player* player, uint16 slot)
{
    Quest const* quest = sObjectMgr->GetQuestTemplate(player->GetQuestSlotQuestId(slot));
    if (!quest)
        return;

    WorldPacket rewardXP = BuildRewardXP(player->GetGUID(), slot, OfferedRewardXP(player, quest));
    player->SendDirectMessage(&rewardXP);
    WorldPacket level = BuildLevel(player->GetGUID(), slot, uint32(player->GetQuestLevel(quest)));
    player->SendDirectMessage(&level);
}

void SendAll(Player* player)
{
    for (uint16 slot = 0; slot < MAX_QUEST_LOG_SIZE; ++slot)
        SendSlot(player, slot);
}
}
