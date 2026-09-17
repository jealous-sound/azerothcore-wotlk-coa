/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionWisdomball.h"

#include "Chat.h"
#include "Creature.h"
#include "DBCStores.h"
#include "DatabaseEnv.h"
#include "GossipDef.h"
#include "Log.h"
#include "Map.h"
#include "ObjectAccessor.h"
#include "ObjectGuid.h"
#include "ObjectMgr.h"
#include "PassiveAI.h"
#include "Player.h"
#include "QuestDef.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "Timer.h"
#include <algorithm>
#include <list>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace
{
/// The ball's own gossip sender, for its paging line; the core's GOSSIP_SENDER_MAIN is its own.
constexpr uint32 SenderMore = 79027;

/// The frame's greeting is an npc_text row, supplied by the module's SQL. The ball had none,
/// which is why its window used to open with the text area empty.
constexpr uint32 GreetingTextId = 790250;

/// The icon byte the client groups a gossip menu's quest list by. 2 lands in its "Available"
/// half, drawn with the yellow "!"; 4 lands in its "Active" half, drawn with the "?" - grey
/// while the objectives are unfinished and gold once the quest is ready to hand in. All three
/// markers, and the level colour of each title, are the client's own work (GossipFrame.lua);
/// the server only picks the half a quest belongs to.
constexpr uint8 QuestIconAvailable = 2;
constexpr uint8 QuestIconActive = 4;

/// A dungeon can list more quests than one page of the menu holds, so the list is paged.
constexpr uint32 PageSize = 30;

/// How far a character has to be from the ball for its packets to count.
constexpr float InteractionRange = 30.0f;

/// How often a character's ball is looked at for a changed quest mark.
constexpr uint32 StatusCheckIntervalMs = 2000;

/// What one dungeon hands out, whichever way the quests are anchored to it.
struct DungeonQuests
{
    std::vector<uint32> starters;
};

/// Which half of the frame a quest is listed in.
enum class MenuKind : uint8
{
    Available,  // this dungeon hands it out and the character has not taken it -> "!"
    Carried     // the character is carrying it, in progress or ready to hand in -> "?"
};

/// One line of the menu's quest list.
struct MenuEntry
{
    uint32 quest;
    MenuKind kind;
};

/// The mark last sent for a character, so the periodic check only speaks when it changes.
struct StatusState
{
    uint32 nextCheckMs = 0;
    ObjectGuid ball;
    uint32 mapId = 0;
    uint8 status = 0;
    bool known = false;
};

std::mutex _questLock;
std::unordered_map<uint32, DungeonQuests> _questsByMap;

/// Every quest that belongs to some dungeon, loaded once from the realm's own tables.
std::mutex _dungeonQuestLock;
bool _dungeonQuestsLoaded = false;
std::vector<uint32> _dungeonQuests;

std::mutex _statusLock;
std::unordered_map<uint32, StatusState> _statusByPlayer;

void ReadQuestIds(std::vector<uint32>& out, std::string const& query)
{
    QueryResult result = WorldDatabase.Query(query.c_str());
    if (!result)
        return;

    do
    {
        out.push_back(result->Fetch()[0].Get<uint32>());
    } while (result->NextRow());
}

void SortUnique(std::vector<uint32>& ids)
{
    std::sort(ids.begin(), ids.end());
    ids.erase(std::unique(ids.begin(), ids.end()), ids.end());
}

/// A list of ids for an IN (...) clause. Every value is a number out of the realm's own tables.
std::string IdList(std::vector<uint32> const& ids)
{
    std::string list = std::to_string(ids.front());
    for (size_t i = 1; i < ids.size(); ++i)
        list += "," + std::to_string(ids[i]);

    return list;
}

/// Every area that belongs to a map. A quest's own zone column points at one of these, which is
/// what makes a quest handed out in a city still count as the dungeon's quest.
std::vector<uint32> AreasOfMap(uint32 mapId)
{
    std::vector<uint32> areas;

    for (uint32 i = 0; i < sAreaTableStore.GetNumRows(); ++i)
        if (AreaTableEntry const* area = sAreaTableStore.LookupEntry(i))
            if (area->mapid == mapId)
                areas.push_back(area->ID);

    return areas;
}

DungeonQuests LoadDungeonQuests(uint32 mapId)
{
    DungeonQuests quests;
    std::string const map = std::to_string(mapId);

    // What this dungeon hands out: the NPCs standing in it, and the objects lying in it.
    ReadQuestIds(quests.starters,
        "SELECT DISTINCT qs.quest FROM creature c JOIN creature_queststarter qs ON qs.id = c.id WHERE c.map = " + map);
    ReadQuestIds(quests.starters,
        "SELECT DISTINCT qs.quest FROM gameobject g JOIN gameobject_queststarter qs ON qs.id = g.id WHERE g.map = " + map);

    // And the quests that belong to the dungeon's own ground, even though somebody out in the
    // world hands them out - the ones a character who walks in holding the ball is looking for.
    std::vector<uint32> const areas = AreasOfMap(mapId);
    if (!areas.empty())
        ReadQuestIds(quests.starters, "SELECT ID FROM quest_template WHERE QuestSortID IN (" + IdList(areas) + ")");

    // What is taken back at the ball is whatever the character is carrying, which the log knows
    // and a dungeon's own relation tables do not: a hand-in needs nothing from this loader.
    SortUnique(quests.starters);
    return quests;
}

DungeonQuests QuestsForMap(uint32 mapId)
{
    {
        std::lock_guard<std::mutex> lock(_questLock);
        auto const itr = _questsByMap.find(mapId);
        if (itr != _questsByMap.end())
            return itr->second;
    }

    DungeonQuests loaded = LoadDungeonQuests(mapId);

    std::lock_guard<std::mutex> lock(_questLock);
    auto const itr = _questsByMap.find(mapId);
    if (itr != _questsByMap.end())
        return itr->second;

    _questsByMap.emplace(mapId, loaded);
    return loaded;
}

bool Contains(std::vector<uint32> const& ids, uint32 questId)
{
    return std::binary_search(ids.begin(), ids.end(), questId);
}

/// The ids of every dungeon and raid map the realm knows.
std::vector<uint32> DungeonMaps()
{
    std::vector<uint32> maps;

    for (uint32 i = 0; i < sMapStore.GetNumRows(); ++i)
        if (MapEntry const* map = sMapStore.LookupEntry(i))
            if (map->IsDungeon())
                maps.push_back(map->MapID);

    SortUnique(maps);
    return maps;
}

std::vector<uint32> LoadAllDungeonQuests()
{
    std::vector<uint32> ids;

    std::vector<uint32> const maps = DungeonMaps();
    if (maps.empty())
        return ids;

    std::string const mapList = IdList(maps);

    // The ground each dungeon owns: a quest filed under one of those zones belongs to it, even
    // though somebody out in the world hands it out.
    std::vector<uint32> areas;
    for (uint32 i = 0; i < sAreaTableStore.GetNumRows(); ++i)
        if (AreaTableEntry const* area = sAreaTableStore.LookupEntry(i))
            if (std::binary_search(maps.begin(), maps.end(), area->mapid))
                areas.push_back(area->ID);

    if (!areas.empty())
        ReadQuestIds(ids, "SELECT ID FROM quest_template WHERE QuestSortID IN (" + IdList(areas) + ")");

    // And what the dungeons' own NPCs and objects deal in, which covers quests filed elsewhere.
    ReadQuestIds(ids,
        "SELECT DISTINCT qs.quest FROM creature c JOIN creature_queststarter qs ON qs.id = c.id WHERE c.map IN (" + mapList + ")");
    ReadQuestIds(ids,
        "SELECT DISTINCT qe.quest FROM creature c JOIN creature_questender qe ON qe.id = c.id WHERE c.map IN (" + mapList + ")");
    ReadQuestIds(ids,
        "SELECT DISTINCT qs.quest FROM gameobject g JOIN gameobject_queststarter qs ON qs.id = g.id WHERE g.map IN (" + mapList + ")");
    ReadQuestIds(ids,
        "SELECT DISTINCT qe.quest FROM gameobject g JOIN gameobject_questender qe ON qe.id = g.id WHERE g.map IN (" + mapList + ")");

    SortUnique(ids);
    return ids;
}

/// Every quest that belongs to some dungeon, loaded once. Out in the world the ball has no map
/// of its own to read, and a carried quest is still the dungeon's.
std::vector<uint32> const& AllDungeonQuests()
{
    std::lock_guard<std::mutex> lock(_dungeonQuestLock);
    if (!_dungeonQuestsLoaded)
    {
        _dungeonQuests = LoadAllDungeonQuests();
        _dungeonQuestsLoaded = true;
    }

    return _dungeonQuests;
}

/// A dungeon is the only place the ball reads its list from, which is also why it stays quiet
/// out in the world.
bool InDungeon(Player* player)
{
    Map* map = player->GetMap();
    return map && (map->IsDungeon() || map->IsRaid());
}

/// A title still written as a placeholder - "<NYI>", "<UNUSED>", "<TXT>" - is a quest the realm
/// never wrote, and the client would draw the marker as the quest's name.
bool IsPlaceholder(Quest const* quest)
{
    if (!quest)
        return true;

    std::string const& title = quest->GetTitle();
    return title.empty() || title.front() == '<';
}

/// The dungeon quests this character is carrying, wherever the ball happens to be. Out in the
/// world this is the whole list: the frame still shows what the character holds.
std::vector<uint32> CarriedDungeonQuests(Player* player)
{
    std::vector<uint32> carried;
    std::vector<uint32> const& dungeons = AllDungeonQuests();

    for (auto const& status : player->getQuestStatusMap())
    {
        if (status.second.Status != QUEST_STATUS_INCOMPLETE && status.second.Status != QUEST_STATUS_COMPLETE)
            continue;

        if (!Contains(dungeons, status.first))
            continue;

        if (IsPlaceholder(sObjectMgr->GetQuestTemplate(status.first)))
            continue;

        carried.push_back(status.first);
    }

    std::sort(carried.begin(), carried.end());
    return carried;
}

bool CanOffer(Player* player, uint32 questId)
{
    Quest const* quest = sObjectMgr->GetQuestTemplate(questId);
    if (!quest || IsPlaceholder(quest))
        return false;

    // Carrying it, or having finished it once when it cannot be repeated, is not "available".
    if (player->GetQuestStatus(questId) != QUEST_STATUS_NONE)
        return false;

    if (player->IsQuestRewarded(questId) && !quest->IsRepeatable())
        return false;

    return true;
}

/// The window a name the character already carries opens. This is the packet the core itself
/// answers an active gossip entry with (SendPreparedQuest sends the same one for icon 4): it
/// holds the quest's own progress text and what is still owed on it, completes the quest when
/// the objectives are in fact done, and turns into the reward window once it can be paid out -
/// so an unfinished quest opens its progress page, and a finished one opens its hand-in.
void SendCarriedWindow(Player* player, Creature* ball, Quest const* quest, bool closeOnCancel)
{
    player->PlayerTalkClass->SendQuestGiverRequestItems(quest, ball->GetGUID(),
        player->CanRewardQuest(quest, false), closeOnCancel);
}

/// The one list the ball works from: what the character is carrying, plus - while the ball is
/// inside a dungeon - what that dungeon hands out. The frame, the mark and the accept check all
/// read it, so what is shown and what the ball will honour cannot drift apart.
std::vector<MenuEntry> PlayerEntries(Player* player)
{
    std::vector<MenuEntry> entries;

    if (InDungeon(player))
        for (uint32 questId : QuestsForMap(player->GetMapId()).starters)
            if (CanOffer(player, questId))
                entries.push_back({ questId, MenuKind::Available });

    // Ready to hand in first, then what is still in progress under it. Both are carried quests,
    // so both are listed wherever the ball stands.
    std::vector<MenuEntry> ready;
    std::vector<MenuEntry> ongoing;

    for (uint32 questId : CarriedDungeonQuests(player))
        (player->GetQuestStatus(questId) == QUEST_STATUS_COMPLETE ? ready : ongoing)
            .push_back({ questId, MenuKind::Carried });

    entries.insert(entries.end(), ready.begin(), ready.end());
    entries.insert(entries.end(), ongoing.begin(), ongoing.end());
    return entries;
}

bool Listed(Player* player, uint32 questId)
{
    for (MenuEntry const& entry : PlayerEntries(player))
        if (entry.quest == questId)
            return true;

    return false;
}

void SendPage(Player* player, Creature* ball, uint32 page)
{
    // The same list the mark and the accept check read, so what the gossip shows and what the
    // ball will actually take never drift apart.
    std::vector<MenuEntry> const entries = PlayerEntries(player);

    ClearGossipMenuFor(player);

    // The names go into the menu's quest half, not into option lines. That half is what the
    // client's own frame splits into an available and an active list and draws with the "!", the
    // "?" and the level colour of each title; option lines could only ever be plain text.
    size_t const first = size_t(page) * PageSize;
    size_t const last = std::min(first + PageSize, entries.size());

    for (size_t i = first; i < last; ++i)
        player->PlayerTalkClass->GetQuestMenu().AddMenuItem(entries[i].quest,
            entries[i].kind == MenuKind::Available ? QuestIconAvailable : QuestIconActive);

    if (last < entries.size())
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "More quests...", SenderMore, page + 1);

    SendGossipMenuFor(player, GreetingTextId, ball->GetGUID());
}

/// Whether this character may work this ball: close enough to reach it, on its map, and in its
/// phase. Whose summon it is does not matter - one ball serves everyone standing by it, and
/// everything it shows and every window it opens comes from the quest log of the character
/// looking at it, never from the summoner's.
bool CanUseBall(Player* player, Creature const* ball)
{
    if (!player || !AscensionWisdomball::IsWisdomball(ball) || !ball->IsAlive())
        return false;

    if (ball->GetMapId() != player->GetMapId())
        return false;

    if (!player->IsWithinDistInMap(ball, InteractionRange))
        return false;

    return player->InSamePhase(ball);
}

/// The ball a quest packet names, when the character that sent it can work that ball.
Creature* UsableBallAt(Player* player, ObjectGuid guid)
{
    Creature* ball = ObjectAccessor::GetCreatureOrPetOrVehicle(*player, guid);
    return CanUseBall(player, ball) ? ball : nullptr;
}

void PushStatus(Player* player, Creature* ball, uint8 status)
{
    player->PlayerTalkClass->SendQuestGiverStatus(status, ball->GetGUID());

    std::lock_guard<std::mutex> lock(_statusLock);
    StatusState& state = _statusByPlayer[player->GetGUID().GetCounter()];
    state.ball = ball->GetGUID();
    state.mapId = player->GetMapId();
    state.status = status;
    state.known = true;
}

class npc_wondrous_wisdomball_ai : public PassiveAI
{
public:
    explicit npc_wondrous_wisdomball_ai(Creature* creature) : PassiveAI(creature) { }
};

class npc_wondrous_wisdomball : public CreatureScript
{
public:
    npc_wondrous_wisdomball() : CreatureScript("npc_wondrous_wisdomball") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_wondrous_wisdomball_ai(creature);
    }

    /// The "!" and "?" over the ball. Returning anything but the scripted "no status" value
    /// keeps the core from reading the quest relations, which the ball deliberately has none of.
    uint32 GetDialogStatus(Player* player, Creature* creature) override
    {
        if (!player)
            return DIALOG_STATUS_NONE;

        return AscensionWisdomball::DialogStatus(player, creature);
    }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (!AscensionWisdomball::IsWisdomball(creature))
            return false;

        SendPage(player, creature, 0);
        return true;
    }

    /// A click on one of the names arrives as an ordinary quest packet, because the names are the
    /// menu's quest half rather than option lines; HandlePacket answers those. The one option
    /// line the menu carries is the paging line.
    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
    {
        if (!AscensionWisdomball::IsWisdomball(creature))
            return false;

        if (sender != SenderMore)
            return false;

        SendPage(player, creature, action);
        return true;
    }
};

class wisdomball_player_script : public PlayerScript
{
public:
    wisdomball_player_script() : PlayerScript("wisdomball_player_script") { }

    /// The mark over the ball is pushed here because the core only marks creatures that own
    /// quest relations. It is only recomputed twice a second, and only for characters that
    /// know the summon.
    void OnPlayerUpdate(Player* player, uint32 /*diff*/) override
    {
        if (!player->HasSpell(AscensionWisdomball::SummonSpell))
            return;

        uint32 const now = getMSTime();
        {
            std::lock_guard<std::mutex> lock(_statusLock);
            StatusState& state = _statusByPlayer[player->GetGUID().GetCounter()];
            if (now < state.nextCheckMs)
                return;

            state.nextCheckMs = now + StatusCheckIntervalMs;
        }

        Creature* ball = AscensionWisdomball::ActiveBall(player);
        uint8 const status = ball ? AscensionWisdomball::DialogStatus(player, ball) : uint8(DIALOG_STATUS_NONE);
        ObjectGuid const ballGuid = ball ? ball->GetGUID() : ObjectGuid::Empty;
        uint32 const mapId = player->GetMapId();

        bool changed = false;
        {
            std::lock_guard<std::mutex> lock(_statusLock);
            StatusState& state = _statusByPlayer[player->GetGUID().GetCounter()];
            changed = !state.known || state.ball != ballGuid || state.mapId != mapId || state.status != status;
            state.ball = ballGuid;
            state.mapId = mapId;
            state.status = status;
            state.known = true;
        }

        if (changed && ball)
            player->PlayerTalkClass->SendQuestGiverStatus(status, ballGuid);
    }

    void OnPlayerMapChanged(Player* player) override
    {
        AscensionWisdomball::Forget(player);
    }

    void OnPlayerLogout(Player* player) override
    {
        AscensionWisdomball::Forget(player);
    }

    void OnPlayerCompleteQuest(Player* player, Quest const* /*quest*/) override
    {
        AscensionWisdomball::Forget(player);
    }
};

class wisdomball_packet_script : public ServerScript
{
public:
    wisdomball_packet_script()
        : ServerScript("wisdomball_packet_script", { SERVERHOOK_CAN_PACKET_RECEIVE })
    {
    }

    /// The core refuses all five for the ball, because it asks the quest relation tables who
    /// hands out a quest and who takes it back; the ball is in neither. Answering them here is
    /// what lets a quest the game considers unavailable be taken at the ball, and what lets a
    /// click on one of the names in its list open the window that name stands for.
    bool CanPacketReceive(WorldSession* session, WorldPacket const& packet) override
    {
        if (packet.GetOpcode() != CMSG_QUESTGIVER_QUERY_QUEST &&
            packet.GetOpcode() != CMSG_QUESTGIVER_COMPLETE_QUEST &&
            packet.GetOpcode() != CMSG_QUESTGIVER_ACCEPT_QUEST &&
            packet.GetOpcode() != CMSG_QUESTGIVER_REQUEST_REWARD &&
            packet.GetOpcode() != CMSG_QUESTGIVER_CHOOSE_REWARD)
            return true;

        Player* player = session ? session->GetPlayer() : nullptr;
        if (!player)
            return true;

        return !AscensionWisdomball::HandlePacket(player, packet);
    }
};
}

namespace AscensionWisdomball
{
bool IsWisdomball(Creature const* creature)
{
    return creature && creature->GetEntry() == CreatureEntry;
}

Creature* ActiveBall(Player* player)
{
    std::list<Creature*> creatures;
    player->GetCreatureListWithEntryInGrid(creatures, CreatureEntry, InteractionRange);

    for (Creature* creature : creatures)
        if (creature->IsAlive() && creature->GetOwnerGUID() == player->GetGUID() && player->InSamePhase(creature))
            return creature;

    return nullptr;
}

Creature* UsableBall(Player* player)
{
    std::list<Creature*> creatures;
    player->GetCreatureListWithEntryInGrid(creatures, CreatureEntry, InteractionRange);

    for (Creature* creature : creatures)
        if (CanUseBall(player, creature))
            return creature;

    return nullptr;
}

std::vector<uint32> OfferedQuests(Player* player)
{
    std::vector<uint32> offered;
    for (MenuEntry const& entry : PlayerEntries(player))
        if (entry.kind == MenuKind::Available)
            offered.push_back(entry.quest);

    return offered;
}

std::vector<uint32> CarriedQuests(Player* player)
{
    std::vector<uint32> carried;
    for (MenuEntry const& entry : PlayerEntries(player))
        if (entry.kind == MenuKind::Carried)
            carried.push_back(entry.quest);

    return carried;
}

std::vector<uint32> TurnInQuests(Player* player)
{
    std::vector<uint32> ready;
    for (uint32 questId : CarriedDungeonQuests(player))
        if (player->GetQuestStatus(questId) == QUEST_STATUS_COMPLETE)
            ready.push_back(questId);

    return ready;
}

uint8 DialogStatus(Player* player, Creature* ball)
{
    if (!player || !IsWisdomball(ball))
        return DIALOG_STATUS_NONE;

    bool handIn = false;
    bool takeable = false;

    for (MenuEntry const& entry : PlayerEntries(player))
    {
        if (entry.kind == MenuKind::Available)
            takeable = true;
        else if (player->GetQuestStatus(entry.quest) == QUEST_STATUS_COMPLETE)
            handIn = true;
    }

    if (handIn)
        return DIALOG_STATUS_REWARD;

    if (takeable)
        return DIALOG_STATUS_AVAILABLE;

    return DIALOG_STATUS_NONE;
}

bool Accept(Player* player, Creature* ball, Quest const* quest)
{
    if (!player || !IsWisdomball(ball) || !quest)
        return false;

    // A ball somebody else summoned takes quests for whoever is standing at it.
    if (!CanUseBall(player, ball))
        return false;

    uint32 const questId = quest->GetQuestId();

    // Only the dungeon's own quests, and only the ones the list would have offered.
    if (!InDungeon(player) || !Contains(QuestsForMap(player->GetMapId()).starters, questId) || !CanOffer(player, questId))
        return false;

    // Quest log space and the quest's own source item still have to be there.
    if (!player->CanAddQuest(quest, true))
        return false;

    LOG_DEBUG("scripts", "Wisdomball: {} takes quest {} in map {} from the ball",
        player->GetName(), questId, player->GetMapId());

    player->AddQuestAndCheckCompletion(quest, ball);
    PushStatus(player, ball, DialogStatus(player, ball));
    return true;
}

bool HandlePacket(Player* player, WorldPacket const& packet)
{
    switch (packet.GetOpcode())
    {
        case CMSG_QUESTGIVER_ACCEPT_QUEST:
        {
            WorldPacket copy(packet);
            ObjectGuid guid;
            uint32 questId = 0;
            uint8 unk = 0;
            copy >> guid >> questId >> unk;

            Creature* ball = UsableBallAt(player, guid);
            if (!ball)
                return false;

            if (Quest const* quest = sObjectMgr->GetQuestTemplate(questId))
                Accept(player, ball, quest);

            player->PlayerTalkClass->SendCloseGossip();
            return true;
        }
        case CMSG_QUESTGIVER_QUERY_QUEST:
        {
            WorldPacket copy(packet);
            ObjectGuid guid;
            uint32 questId = 0;
            uint8 unk = 0;
            copy >> guid >> questId >> unk;

            Creature* ball = UsableBallAt(player, guid);
            if (!ball)
                return false;

            // Only a name the ball itself listed opens anything, whatever the client claims to
            // have clicked.
            Quest const* quest = sObjectMgr->GetQuestTemplate(questId);
            if (!quest || !Listed(player, questId))
            {
                player->PlayerTalkClass->SendCloseGossip();
                return true;
            }

            if (player->GetQuestStatus(questId) == QUEST_STATUS_NONE)
            {
                // The ordinary quest window, exactly as the NPC that owns the quest would show
                // it, except that this one can be accepted whatever the prerequisites say.
                player->PlayerTalkClass->SendQuestGiverQuestDetails(quest, ball->GetGUID(), true);
            }
            else
            {
                // Carried already: its own progress page while it is unfinished, its hand-in
                // once it is done.
                SendCarriedWindow(player, ball, quest, true);
            }

            return true;
        }
        case CMSG_QUESTGIVER_COMPLETE_QUEST:
        {
            // This - not the query above - is the opcode the client sends when a name in the
            // frame's active half is clicked: it asks the giver for the state of a quest the
            // character is already carrying. The core drops it for the ball for the same reason
            // it drops the others - the ball owns no quest relations - which is why a carried
            // name looked clickable and answered with nothing.
            WorldPacket copy(packet);
            ObjectGuid guid;
            uint32 questId = 0;
            copy >> guid >> questId;

            Creature* ball = UsableBallAt(player, guid);
            if (!ball)
                return false;

            Quest const* quest = sObjectMgr->GetQuestTemplate(questId);
            if (!quest || !Listed(player, questId))
            {
                player->PlayerTalkClass->SendCloseGossip();
                return true;
            }

            if (player->GetQuestStatus(questId) == QUEST_STATUS_NONE)
            {
                // A name the ball only offers: the window that asks for it, as the query path does.
                player->PlayerTalkClass->SendQuestGiverQuestDetails(quest, ball->GetGUID(), true);
            }
            else
            {
                // Carried: the same window the core's own quest giver answers this opcode with -
                // the progress page while the quest is unfinished, completing it on the spot when
                // the objectives are in fact met, and the reward window once it can be paid out.
                SendCarriedWindow(player, ball, quest, false);
            }

            return true;
        }
        case CMSG_QUESTGIVER_REQUEST_REWARD:
        {
            WorldPacket copy(packet);
            ObjectGuid guid;
            uint32 questId = 0;
            copy >> guid >> questId;

            Creature* ball = UsableBallAt(player, guid);
            if (!ball)
                return false;

            Quest const* quest = sObjectMgr->GetQuestTemplate(questId);
            if (!quest || !Listed(player, questId))
            {
                player->PlayerTalkClass->SendCloseGossip();
                return true;
            }

            // This is the client claiming the reward: the button in the window a carried name
            // opens presses it. The core's own answer is to finish the quest on the spot when its
            // objectives are in fact met and then open the reward window - answering with the
            // progress page instead leaves the player clicking the same page for ever, because
            // that page's button sends exactly this opcode again. Only a quest that is still short
            // stays on the progress page.
            if (player->CanCompleteQuest(questId))
                player->CompleteQuest(questId);

            if (player->GetQuestStatus(questId) == QUEST_STATUS_COMPLETE)
                player->PlayerTalkClass->SendQuestGiverOfferReward(quest, ball->GetGUID(), true);
            else
                SendCarriedWindow(player, ball, quest, false);

            return true;
        }
        case CMSG_QUESTGIVER_CHOOSE_REWARD:
        {
            WorldPacket copy(packet);
            ObjectGuid guid;
            uint32 questId = 0;
            uint32 reward = 0;
            copy >> guid >> questId >> reward;

            Creature* ball = UsableBallAt(player, guid);
            if (!ball)
                return false;

            if (reward >= QUEST_REWARD_CHOICES_COUNT)
                return true;

            Quest const* quest = sObjectMgr->GetQuestTemplate(questId);
            if (!quest)
                return true;

            // The same two guards the core's own quest giver path applies.
            if (player->GetQuestStatus(questId) != QUEST_STATUS_COMPLETE &&
                !quest->IsAutoComplete() && quest->GetQuestMethod())
                return true;

            if (player->CanRewardQuest(quest, reward, true))
            {
                player->RewardQuest(quest, reward, ball);
                PushStatus(player, ball, DialogStatus(player, ball));
            }

            return true;
        }
        default:
            return false;
    }
}

void Forget(Player* player)
{
    if (!player)
        return;

    std::lock_guard<std::mutex> lock(_statusLock);
    _statusByPlayer.erase(player->GetGUID().GetCounter());
}
}

void AddSC_AscensionWisdomball()
{
    new npc_wondrous_wisdomball();
    new wisdomball_player_script();
    new wisdomball_packet_script();
}
