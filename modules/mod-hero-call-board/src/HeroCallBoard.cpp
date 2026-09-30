/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AllBattlegroundScript.h"
#include "Battleground.h"
#include "Config.h"
#include "GameObject.h"
#include "GameObjectScript.h"
#include "GlobalScript.h"
#include "GossipDef.h"
#include "Group.h"
#include "LFGMgr.h"
#include "Map.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "PlayerScript.h"
#include "QuestDef.h"
#include "ScriptedGossip.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldScript.h"
#include "WorldSession.h"
#include "WorldState.h"
#include "WorldStateDefines.h"

#include <atomic>
#include <optional>

namespace
{
    enum BoardCategory : uint32
    {
        CategoryPvE = 1,
        CategoryReturn = 2,
        CategoryProfessions = 3,
        CategoryPvP = 4
    };

    enum BoardQuestSort : int32
    {
        SortPvE = -380,
        SortPvP = -381,
        SortProfessions = -382
    };

    constexpr uint32 WorldStateWeeklyReset = 20002;
    constexpr uint32 WorldStateDailyReset = 20010;
    constexpr uint32 BoardCurrency = 975001;
    constexpr uint32 DuelQuest = 80651;
    constexpr uint32 DuelCredit = 80027;
    constexpr uint32 BattlegroundQuest = 81260;
    constexpr uint32 NormalDungeonCredit = 81042;
    constexpr uint32 HeroicDungeonCredit = 81041;
    constexpr uint32 MythicDungeonCredit = 80652;

    std::atomic<bool> enabled{true};

    bool IsCallBoard(uint32 entry)
    {
        switch (entry)
        {
            case 108606: case 402000: case 402001: case 402002: case 402003:
            case 412000: case 412001: case 413000: case 422000: case 1008002:
            case 1008008: case 1804480: case 2250000:
                return true;
            default:
                return false;
        }
    }

    std::optional<uint32> CategoryOf(Quest const* quest)
    {
        switch (quest->GetZoneOrSort())
        {
            case SortPvE: return CategoryPvE;
            case SortPvP: return CategoryPvP;
            case SortProfessions: return CategoryProfessions;
            default: return std::nullopt;
        }
    }

    int32 SortOf(uint32 category)
    {
        switch (category)
        {
            case CategoryPvP: return SortPvP;
            case CategoryProfessions: return SortProfessions;
            default: return SortPvE;
        }
    }

    void SendBoardMenu(Player* player, GameObject* board)
    {
        // Questgiver Data3 is a gossip menu ID. Its empty text makes the client clear its gossip GUID before
        // GOSSIP_SHOW, which CallBoardUI requires.
        player->PlayerTalkClass->GetGossipMenu().SetMenuId(board->GetGOInfo()->questgiver.gossipID);
        board->LastUsedScriptID = board->GetScriptId();
        SendGossipMenuFor(player, board->GetEntry(), board->GetGUID());
    }

    void ShowCategories(Player* player, GameObject* board)
    {
        ClearGossipMenuFor(player);

        // CallBoardTimers.lua reads these reset times as Unix world states.
        player->SendUpdateWorldState(WorldStateWeeklyReset,
            static_cast<uint32>(sWorld->GetNextWeeklyQuestsResetTime().count()));
        player->SendUpdateWorldState(WorldStateDailyReset,
            static_cast<uint32>(sWorld->GetNextDailyQuestsResetTime().count()));
        uint64 const arenaReset = sWorld->getBoolConfig(CONFIG_ARENA_AUTO_DISTRIBUTE_POINTS)
            ? sWorldState->getWorldState(WORLD_STATE_CUSTOM_ARENA_DISTRIBUTION_TIME) : 0;
        player->SendUpdateWorldState(WORLD_STATE_CUSTOM_ARENA_DISTRIBUTION_TIME, static_cast<uint32>(arenaReset));

        WorldPacket tokenQuery(CMSG_ITEM_QUERY_SINGLE, sizeof(uint32));
        tokenQuery << BoardCurrency;
        player->GetSession()->HandleItemQuerySingleOpcode(tokenQuery);

        // CallBoardUI binds its category buttons to these exact option labels.
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "PvE Quests", GOSSIP_SENDER_MAIN, CategoryPvE);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Profession Quests", GOSSIP_SENDER_MAIN, CategoryProfessions);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "PvP Quests", GOSSIP_SENDER_MAIN, CategoryPvP);
        SendBoardMenu(player, board);
    }

    void ShowCategory(Player* player, GameObject* board, uint32 category)
    {
        ClearGossipMenuFor(player);
        QuestMenu& menu = player->PlayerTalkClass->GetQuestMenu();
        int32 const sort = SortOf(category);

        // Filter before adding: both categories together can exceed the 32-entry menu limit.
        auto const involved = sObjectMgr->GetGOQuestInvolvedRelationBounds(board->GetEntry());
        for (auto itr = involved.first; itr != involved.second; ++itr)
        {
            Quest const* quest = sObjectMgr->GetQuestTemplate(itr->second);
            if (!quest || quest->GetZoneOrSort() != sort || menu.HasItem(itr->second))
                continue;

            QuestStatus const status = player->GetQuestStatus(itr->second);
            if ((status == QUEST_STATUS_INCOMPLETE || status == QUEST_STATUS_COMPLETE)
                && menu.GetMenuItemCount() < GOSSIP_MAX_MENU_ITEMS)
                menu.AddMenuItem(itr->second, 4);
        }

        auto const available = sObjectMgr->GetGOQuestRelationBounds(board->GetEntry());
        for (auto itr = available.first; itr != available.second; ++itr)
        {
            Quest const* quest = sObjectMgr->GetQuestTemplate(itr->second);
            if (!quest || quest->GetZoneOrSort() != sort || menu.HasItem(itr->second)
                || !player->CanSeeStartQuest(quest) || !player->CanTakeQuest(quest, false))
                continue;

            if (menu.GetMenuItemCount() < GOSSIP_MAX_MENU_ITEMS)
                menu.AddMenuItem(itr->second, 2);
        }

        // CallBoardLayout reads the quests' QuestSort synchronously, even on a cold client cache.
        for (uint16 index = 0; index < menu.GetMenuItemCount(); ++index)
            if (Quest const* quest = sObjectMgr->GetQuestTemplate(menu.GetItem(index).QuestId))
                player->PlayerTalkClass->SendQuestQueryResponse(quest);

        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Return", GOSSIP_SENDER_MAIN, CategoryReturn);
        SendBoardMenu(player, board);
    }

    bool RefreshBoard(Player* player, Object* object, Quest const* quest)
    {
        if (!enabled.load() || !player || !object || !quest || !object->IsGameObject())
            return false;

        GameObject* board = object->ToGameObject();
        if (!IsCallBoard(board->GetEntry())
            || (!board->hasQuest(quest->GetQuestId()) && !board->hasInvolvedQuest(quest->GetQuestId())))
            return false;

        std::optional<uint32> const category = CategoryOf(quest);
        if (!category)
            return false;

        ShowCategory(player, board, *category);
        return true;
    }
}

class go_hero_call_board final : public GameObjectScript
{
public:
    go_hero_call_board() : GameObjectScript("native_ascension_callboard") { }

    uint32 GetDialogStatus(Player* /*player*/, GameObject* board) override
    {
        // The board is a permanent service: daily quest flags must not turn its marker blue.
        return enabled.load() && IsCallBoard(board->GetEntry()) ? DIALOG_STATUS_AVAILABLE
            : DIALOG_STATUS_SCRIPTED_NO_STATUS;
    }

    bool OnGossipHello(Player* player, GameObject* board) override
    {
        if (!enabled.load() || !IsCallBoard(board->GetEntry()))
            return false;

        ShowCategories(player, board);
        return true;
    }

    bool OnGossipSelect(Player* player, GameObject* board, uint32 sender, uint32 action) override
    {
        if (!enabled.load() || !IsCallBoard(board->GetEntry()) || sender != GOSSIP_SENDER_MAIN)
            return false;

        if (action == CategoryReturn)
            ShowCategories(player, board);
        else if (action == CategoryPvE || action == CategoryProfessions || action == CategoryPvP)
            ShowCategory(player, board, action);
        else
            return false;

        return true;
    }

    bool OnQuestReward(Player* player, GameObject* board, Quest const* quest, uint32 /*reward*/) override
    {
        return RefreshBoard(player, board, quest);
    }
};

class HeroCallBoardPlayers final : public PlayerScript
{
public:
    HeroCallBoardPlayers() : PlayerScript("HeroCallBoardPlayers",
        { PLAYERHOOK_ON_REFRESH_QUEST_GIVER, PLAYERHOOK_ON_QUEST_ABANDON, PLAYERHOOK_ON_DUEL_END }) { }

    bool OnPlayerRefreshQuestGiver(Player* player, Object* questGiver, Quest const* quest) override
    {
        // Accept All submits a batch through the ordinary accept opcode: the category stays open, and every
        // quest still passes the core's checks.
        return RefreshBoard(player, questGiver, quest);
    }

    void OnPlayerQuestAbandon(Player* player, uint32 questId) override
    {
        if (!enabled.load())
            return;

        // Refresh only a still-open board that listed this quest.
        ObjectGuid const boardGuid = player->PlayerTalkClass->GetGossipMenu().GetSenderGUID();
        if (boardGuid.IsEmpty() || !player->PlayerTalkClass->GetQuestMenu().HasItem(questId))
            return;

        Object* board = ObjectAccessor::GetObjectByTypeMask(*player, boardGuid, TYPEMASK_GAMEOBJECT);
        if (board && player->CanInteractWithQuestGiver(board))
            RefreshBoard(player, board, sObjectMgr->GetQuestTemplate(questId));
    }

    void OnPlayerDuelEnd(Player* winner, Player* loser, DuelCompleteType type) override
    {
        // Only victories count; requests, losses and interrupted duels do not.
        if (!enabled.load() || type != DUEL_WON || !winner || !loser || winner == loser || !winner->IsInWorld()
            || !loser->IsInWorld() || winner->GetQuestStatus(DuelQuest) != QUEST_STATUS_INCOMPLETE)
            return;

        winner->KilledMonsterCredit(DuelCredit);
    }
};

class HeroCallBoardBattlegrounds final : public AllBattlegroundScript
{
public:
    HeroCallBoardBattlegrounds() : AllBattlegroundScript("HeroCallBoardBattlegrounds",
        { ALLBATTLEGROUNDHOOK_ON_BATTLEGROUND_END_REWARD }) { }

    void OnBattlegroundEndReward(Battleground* battleground, Player* player, TeamId winner) override
    {
        if (!enabled.load() || battleground->isArena() || battleground->IsWargame() || winner == TEAM_NEUTRAL
            || player->GetBgTeamId() != winner || player->GetQuestStatus(BattlegroundQuest) != QUEST_STATUS_INCOMPLETE)
            return;

        player->AreaExploredOrEventHappens(BattlegroundQuest);
    }
};

class HeroCallBoardDungeons final : public GlobalScript
{
public:
    HeroCallBoardDungeons() : GlobalScript("HeroCallBoardDungeons", { GLOBALHOOK_ON_AFTER_UPDATE_ENCOUNTER_STATE }) { }

    void OnAfterUpdateEncounterState(Map* map, EncounterCreditType /*type*/, uint32 /*creditEntry*/, Unit* source,
        Difficulty /*difficulty*/, std::list<DungeonEncounter const*> const* /*encounters*/, uint32 dungeonId,
        bool updated) override
    {
        if (!enabled.load() || !updated || !dungeonId || !map || !map->IsNonRaidDungeon()
            || map->IsScriptedPrivateInstance() || !source || source->FindMap() != map)
            return;

        auto const* dungeon = sLFGMgr->GetLFGDungeon(dungeonId);
        if (!dungeon || dungeon->map != map->GetId() || dungeon->difficulty != map->GetDifficulty())
            return;

        uint32 credit = 0;
        switch (map->GetDifficulty())
        {
            case DUNGEON_DIFFICULTY_NORMAL: credit = NormalDungeonCredit; break;
            case DUNGEON_DIFFICULTY_HEROIC: credit = HeroicDungeonCredit; break;
            case DUNGEON_DIFFICULTY_EPIC: credit = MythicDungeonCredit; break;
            default: return;
        }

        for (auto const& reference : map->GetPlayers())
        {
            Player* player = reference.GetSource();
            if (!player || !player->IsInWorld() || !player->IsAtGroupRewardDistance(source))
                continue;

            // The dailies require a dungeon finder group assigned to this dungeon; the durable state keeps the
            // assignment while the group looks for a replacement.
            Group const* group = player->GetGroup();
            if (group && group->isLFGGroup() && sLFGMgr->GetDungeon(group->GetGUID()) == dungeonId
                && sLFGMgr->GetOldState(group->GetGUID()) == lfg::LFG_STATE_DUNGEON
                && sLFGMgr->GetOldState(player->GetGUID()) == lfg::LFG_STATE_DUNGEON)
                player->KilledMonsterCredit(credit);
        }
    }
};

class HeroCallBoardWorld final : public WorldScript
{
public:
    HeroCallBoardWorld() : WorldScript("HeroCallBoardWorld", { WORLDHOOK_ON_AFTER_CONFIG_LOAD }) { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        enabled.store(sConfigMgr->GetOption<bool>("HeroCallBoard.Enable", true));
    }
};

void AddHeroCallBoardScripts()
{
    new go_hero_call_board();
    new HeroCallBoardPlayers();
    new HeroCallBoardBattlegrounds();
    new HeroCallBoardDungeons();
    new HeroCallBoardWorld();
}
