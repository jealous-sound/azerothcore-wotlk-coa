/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "AscensionManastorm.h"
#include "AscensionManastormData.h"
#include "AscensionManastormProtocol.h"
#include "AscensionManastormRules.h"
#include "AllCreatureScript.h"
#include "AllMapScript.h"
#include "Chat.h"
#include "CommandScript.h"
#include "Config.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "DatabaseEnv.h"
#include "EventMap.h"
#include "GameTime.h"
#include "GossipDef.h"
#include "InstanceScript.h"
#include "Log.h"
#include "Mail.h"
#include "MailMgr.h"
#include "Map.h"
#include "MapMgr.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "PlayerScript.h"
#include "ScriptedGossip.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "TemporarySummon.h"
#include "UnitScript.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldScript.h"
#include "WorldSession.h"
#include <atomic>
#include <cmath>
#include <deque>
#include <list>
#include <map>
#include <mutex>
#include <set>

namespace
{
    using namespace Ascension::Manastorm;
    using namespace Acore::ChatCommands;
    constexpr uint32 MapId = 389;
    constexpr uint32 GuideEntry = 80919;
    constexpr uint32 SafetyBubble = 93306;
    constexpr uint32 SafetyBubbleHelper = 93307;
    constexpr uint32 LinkSpell = 93459;
    constexpr uint32 HeartyHeal = 93312;
    constexpr uint32 Leeching = 93344;
    constexpr uint32 UnrelentingSpeed = 93353;
    constexpr uint32 TribalFury = 93422;
    constexpr uint32 PortalAura = 93338;
    constexpr uint32 EventCapability = 1;

    struct Request
    {
        uint64 token;
        uint16 opcode;
        uint32 depth;
    };

    struct Run
    {
        uint64 token = 0;
        Progress progress;
        Phase phase = Phase::Idle;
        uint32 depth = 0;
        uint32 instanceId = 0;
        uint32 level = 0;
        uint32 bossBaseHealth = 0;
        uint32 guideAction = 0;
        ObjectGuid boss;
        ObjectGuid guide;
        ObjectGuid portal;
        bool portalArmed = false;
        std::set<ObjectGuid> guards;
        WorldLocation returnLocation;
        EventMap uiEvents;
        time_t lastSeen = 0;
        time_t lastRequest = 0;
        time_t transferStarted = 0;
        bool initialized = false;
        bool databaseReady = false;
        bool commitReady = false;
        bool commitSucceeded = false;
        bool progressDirty = false;

        Scene const& GetScene() const { return Scenes.at((depth - 1) % Scenes.size()); }
        uint32 MaxCompleted() const { return progress[0].empty() ? 0 : progress[0].back(); }
    };

    class ManastormService
    {
    public:
        static ManastormService& Get()
        {
            static ManastormService service;
            return service;
        }

        void Configure()
        {
            // Startup only: disabling a live run halfway through must not discard its return location.
            enabled.store(sConfigMgr->GetOption<bool>("Ascension.Manastorm.Enable", false));
        }

        bool Queue(WorldSession* session, WorldPacket const& packet)
        {
            uint16 const opcode = packet.GetOpcode();
            if (opcode != Enter && opcode != Leave && opcode != SetSlot)
                return false;
            uint64 const token = session->GetScriptPacketToken();
            if (!token || !enabled.load())
                return true;
            if ((opcode == Enter && packet.size() != 4) || (opcode == Leave && packet.size() != 0))
                return true;
            // Loadouts are outside this pilot; no unchecked slot index ever reaches game objects.
            if (opcode == SetSlot)
                return true;
            uint32 const depth = opcode == Enter ? packet.read<uint32>(0) : 0;
            std::lock_guard<std::mutex> lock(queueMutex);
            auto& queue = requests[session->GetAccountId()];
            if (queue.size() < MaxQueuedRequests)
                queue.push_back({token, opcode, depth});
            return true;
        }

        void Login(Player* player)
        {
            if (!enabled.load())
                return;
            std::lock_guard<std::recursive_mutex> lock(mutex);
            auto& run = runs[player->GetGUID()];
            run.token = ++nextToken;
            run.uiEvents.Reset();
            run.uiEvents.ScheduleEvent(EventCapability, 2s);
            player->GetSession()->SetScriptPacketToken(run.token);
            auto* statement = CharacterDatabase.GetPreparedStatement(CHAR_SEL_MANASTORM_CLEARS);
            statement->SetData(0, player->GetGUID().GetCounter());
            PreparedQueryResult result = CharacterDatabase.Query(statement);
            run.progress = {};
            run.databaseReady = false;
            if (result)
            {
                do
                {
                    Field* fields = result->Fetch();
                    uint8 const mode = fields[0].Get<uint8>();
                    uint32 const depth = fields[1].Get<uint32>();
                    if (mode == 255 && !depth)
                        // The always-present sentinel distinguishes an empty journal from a failed read.
                        run.databaseReady = true;
                    else if (mode < run.progress.size() && depth)
                        run.progress[mode].push_back(depth);
                } while (result->NextRow());
            }
            SendProgress(player, run, true);
            SendCapability(player, run.databaseReady);
            WorldPacket loadout(LoadoutData, 4);
            loadout << uint32(0);
            player->SendDirectMessage(&loadout);

            if (run.depth && run.instanceId && GameTime::GetGameTime().count() - run.lastSeen <= ReconnectSeconds)
            {
                Map* map = sMapMgr->FindMap(MapId, run.instanceId);
                if (map && map->IsScriptedPrivateInstance()
                    && map->ToInstanceMap()->GetScriptedPrivateOwner() == player->GetGUID()
                    && run.phase != Phase::Failed && run.phase != Phase::Leaving)
                {
                    player->PrepareScriptedPrivateInstance(MapId, run.returnLocation);
                    player->SetScriptedPrivateInstanceId(run.instanceId);
                    auto const& entry = run.GetScene().entrance;
                    if (player->TeleportTo(MapId, entry.x, entry.y, entry.z, entry.o, 0, nullptr, true))
                        return;
                    player->ClearScriptedPrivateInstance();
                }
            }
            // SaveToDB stored the outside position; a server crash never recreates a half-finished scene.
            run.phase = Phase::Idle;
            run.depth = run.instanceId = 0;
            SendActive(player, run);
        }

        void Logout(Player* player)
        {
            player->GetSession()->SetScriptPacketToken(0);
            std::lock_guard<std::recursive_mutex> lock(mutex);
            auto itr = runs.find(player->GetGUID());
            if (itr != runs.end())
            {
                itr->second.token = 0;
                itr->second.lastSeen = GameTime::GetGameTime().count();
            }
            std::lock_guard<std::mutex> queueLock(queueMutex);
            requests.erase(player->GetSession()->GetAccountId());
        }

        void MapChanged(Player* player)
        {
            if (player->IsBeingTeleported())
                return;
            std::lock_guard<std::recursive_mutex> lock(mutex);
            auto itr = runs.find(player->GetGUID());
            if (itr == runs.end())
                return;
            Run& run = itr->second;
            if (run.depth && !OwnsScene(player, run) && run.phase != Phase::Transferring
                && run.phase != Phase::Leaving && run.phase != Phase::Committing)
            {
                ClearBubble(player);
                player->ClearScriptedPrivateInstance();
                run.phase = Phase::Idle;
                run.depth = run.instanceId = 0;
                SendActive(player, run);
            }
        }

        void PollTransactions()
        {
            std::lock_guard<std::recursive_mutex> lock(mutex);
            for (auto itr = transactions.begin(); itr != transactions.end();)
            {
                if (itr->InvokeIfReady())
                    itr = transactions.erase(itr);
                else
                    ++itr;
            }
            time_t const now = GameTime::GetGameTime().count();
            for (auto itr = runs.begin(); itr != runs.end();)
            {
                if (!itr->second.token && now - itr->second.lastSeen > ReconnectSeconds
                    && (itr->second.phase != Phase::Committing || itr->second.commitReady))
                    itr = runs.erase(itr);
                else
                    ++itr;
            }
            for (auto itr = readyMails.begin(); itr != readyMails.end();)
            {
                auto run = runs.find(itr->first);
                if (run == runs.end() || !run->second.token)
                    std::erase_if(itr->second, [now](auto const& mail)
                    {
                        return now - mail->deliver_time > ReconnectSeconds;
                    });
                if (itr->second.empty())
                    itr = readyMails.erase(itr);
                else
                    ++itr;
            }
        }

        void UpdatePlayer(Player* player, uint32 diff)
        {
            if (!enabled.load())
                return;
            std::deque<Request> incoming;
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                auto itr = requests.find(player->GetSession()->GetAccountId());
                if (itr != requests.end())
                {
                    incoming.swap(itr->second);
                    requests.erase(itr);
                }
            }
            std::lock_guard<std::recursive_mutex> lock(mutex);
            auto itr = runs.find(player->GetGUID());
            if (itr == runs.end())
                return;
            Run& run = itr->second;
            auto delivered = readyMails.find(player->GetGUID());
            if (delivered != readyMails.end())
            {
                for (auto const& mail : delivered->second)
                    if (!player->GetMail(mail->messageID))
                    {
                        player->AddMail(new Mail(*mail));
                        player->AddNewMailDeliverTime(mail->deliver_time);
                    }
                readyMails.erase(delivered);
            }
            if (run.progressDirty)
            {
                run.progressDirty = false;
                SendProgress(player, run, false);
            }
            if (run.phase == Phase::Committing && run.commitReady && !player->IsBeingTeleported()
                && !OwnsScene(player, run))
            {
                // An external teleport must not strand the journal callback in a departed instance.
                if (!run.commitSucceeded)
                    ChatHandler(player->GetSession()).SendSysMessage(
                        "Manastorm could not save this clear. No reward was issued.");
                run.commitReady = false;
                ClearBubble(player);
                player->ClearScriptedPrivateInstance();
                run.phase = Phase::Idle;
                run.depth = run.instanceId = 0;
                SendActive(player, run);
            }
            run.uiEvents.Update(diff);
            if (run.uiEvents.ExecuteEvent() == EventCapability)
            {
                SendCapability(player, run.databaseReady);
                run.uiEvents.ScheduleEvent(EventCapability, 5s);
            }
            for (auto const& request : incoming)
            {
                if (request.token != run.token || !player->IsInWorld() || player->IsBeingTeleported())
                    continue;
                time_t const now = GameTime::GetGameTime().count();
                if (now == run.lastRequest)
                    continue;
                run.lastRequest = now;
                if (request.opcode == Enter)
                    Start(player, run, request.depth);
                else
                    Exit(player, run);
            }
            if (run.phase == Phase::Leaving && !player->IsBeingTeleported() && player->GetMapId() != MapId)
            {
                run.phase = Phase::Idle;
                run.depth = run.instanceId = 0;
                player->ClearScriptedPrivateInstance();
                SendActive(player, run);
                SendResult(player, LeaveResult, "LEAVE_MANASTORM_OK");
                player->SaveToDB(false, false);
            }
            if (run.phase == Phase::Transferring && !player->IsBeingTeleported() && !OwnsScene(player, run)
                && GameTime::GetGameTime().count() - run.transferStarted > 10)
            {
                player->ClearScriptedPrivateInstance();
                run.phase = Phase::Idle;
                run.depth = run.instanceId = 0;
                SendActive(player, run);
                SendResult(player, EnterResult, "ENTER_MANASTORM_UNKNOWN");
            }
        }

        bool Command(Player* player, std::string const& action, uint32 depth = 1)
        {
            std::lock_guard<std::recursive_mutex> lock(mutex);
            auto itr = runs.find(player->GetGUID());
            if (itr == runs.end() || !enabled.load())
            {
                ChatHandler(player->GetSession()).SendSysMessage("Manastorm pilot is not enabled on this server.");
                return true;
            }
            Run& run = itr->second;
            if (action == "enter")
                Start(player, run, depth);
            else if (action == "leave")
                Exit(player, run);
            else if ((action == "next" || action == "start") && OwnsScene(player, run))
                run.guideAction = action == "next" ? 2 : 1;
            else
            {
                SendCapability(player, run.databaseReady);
                SendProgress(player, run, true);
                SendActive(player, run);
                ChatHandler(player->GetSession()).PSendSysMessage(
                    "Manastorm: depth {}, completed {}, pilot 1-15 (solo).", run.depth, run.MaxCompleted());
            }
            return true;
        }

        void Entered(InstanceMap* map, Player* player)
        {
            std::lock_guard<std::recursive_mutex> lock(mutex);
            auto itr = runs.find(player->GetGUID());
            if (itr == runs.end() || map->GetScriptedPrivateOwner() != player->GetGUID())
                return;
            Run& run = itr->second;
            run.instanceId = map->GetInstanceId();
            if (!run.initialized)
            {
                run.guards.clear();
                auto const& scene = run.GetScene();
                Creature* boss = SpawnEnemy(map, scene.boss, run, true);
                if (!boss)
                {
                    FailRun(player, run);
                    return;
                }
                run.boss = boss->GetGUID();
                for (auto const& spawn : scene.guards)
                    if (spawn.entry)
                    {
                        Creature* guard = SpawnEnemy(map, spawn, run, false);
                        if (!guard)
                        {
                            FailRun(player, run);
                            return;
                        }
                        run.guards.insert(guard->GetGUID());
                    }
                run.phase = Phase::Preparing;
                run.initialized = true;
                auto const& entry = scene.entrance;
                player->CastSpell(entry.x, entry.y, entry.z, SafetyBubble, true);
                ChatHandler(player->GetSession()).SendSysMessage(
                    "Manastorm: leave the safety bubble to begin. Defeat the boss; nearby guards empower Chaotic Link.");
            }
            SendActive(player, run);
            if (run.phase == Phase::Completed)
            {
                WorldPacket completed(CompletedLevel, 4);
                completed << run.depth;
                player->SendDirectMessage(&completed);
            }
            else if (run.phase == Phase::Preparing || run.phase == Phase::Running)
                UpdateLink(player, run);
            SendProgress(player, run, false);
            SendResult(player, EnterResult, "ENTER_MANASTORM_OK");
            SendCapability(player, run.databaseReady);
            player->SaveToDB(false, false);
        }

        void UpdateInstance(InstanceMap* map, uint32 /*diff*/)
        {
            Player* player = FindOwner(map);
            if (!player || !player->IsInWorld())
                return;
            std::lock_guard<std::recursive_mutex> lock(mutex);
            auto itr = runs.find(player->GetGUID());
            if (itr == runs.end() || !OwnsScene(player, itr->second))
                return;
            Run& run = itr->second;
            if (player->IsBeingTeleported())
                return;
            if (run.phase == Phase::Committing && run.commitReady)
            {
                run.commitReady = false;
                if (run.commitSucceeded)
                {
                    Finish(player, run);
                }
                else
                {
                    run.phase = Phase::Failed;
                    ChatHandler(player->GetSession()).SendSysMessage("Manastorm could not save this clear. "
                        "No reward was issued; leave and retry from your saved checkpoint.");
                }
            }
            if (!player->IsAlive() || player->GetGroup())
            {
                FailRun(player, run);
                return;
            }
            if (run.guideAction == 2)
            {
                run.guideAction = 0;
                if (CanAdvance(run.phase, run.depth))
                    Transfer(player, run, run.depth + 1);
                return;
            }
            if (run.phase == Phase::Preparing)
            {
                auto const& entry = run.GetScene().entrance;
                if (run.guideAction == 1 || player->GetExactDist(entry.x, entry.y, entry.z) > 8.0f)
                {
                    run.guideAction = 0;
                    ClearBubble(player);
                    run.phase = Phase::Running;
                    if (Creature* boss = map->GetCreature(run.boss))
                        boss->SetReactState(REACT_AGGRESSIVE);
                    for (auto const& guid : run.guards)
                        if (Creature* guard = map->GetCreature(guid))
                            guard->SetReactState(REACT_AGGRESSIVE);
                }
            }
            if (run.phase == Phase::Completed && CanAdvance(run.phase, run.depth))
                if (Creature* portal = map->GetCreature(run.portal))
                {
                    float const distance = player->GetExactDist(portal);
                    if (distance > 6.0f)
                        run.portalArmed = true;
                    else if (run.portalArmed && distance <= 2.5f)
                        run.guideAction = 2;
                }
        }

        void Death(Unit* unit)
        {
            if (!unit || !unit->IsInWorld() || !unit->FindMap() || !unit->GetMap()->IsScriptedPrivateInstance())
                return;
            InstanceMap* map = unit->GetMap()->ToInstanceMap();
            Player* player = FindOwner(map);
            if (!player)
                return;
            std::lock_guard<std::recursive_mutex> lock(mutex);
            auto itr = runs.find(player->GetGUID());
            if (itr == runs.end() || !OwnsScene(player, itr->second))
                return;
            Run& run = itr->second;
            if (run.phase != Phase::Running || !player->IsAlive())
                return;
            if (unit->GetGUID() == run.boss)
                Complete(player, run);
            else if (run.guards.erase(unit->GetGUID()))
            {
                UpdateLink(player, run);
                // Authored Hearty Heal formula; no triggered helper or implicit class SP coefficient.
                uint32 const heal = uint32(10 + player->GetLevel() * 7
                    + player->SpellBaseHealingBonusDone(SPELL_SCHOOL_MASK_ALL) * 0.13247f
                    + player->GetTotalAttackPowerValue(BASE_ATTACK) * 0.092729f);
                if (SpellInfo const* info = sSpellMgr->GetSpellInfo(HeartyHeal))
                {
                    HealInfo healInfo(player, player, heal, info, info->GetSchoolMask());
                    player->HealBySpell(healInfo, false);
                }
                player->EnergizeBySpell(player, HeartyHeal, player->GetMaxPower(POWER_MANA) * 15 / 100, POWER_MANA);
            }
        }

        void Damage(Unit* attacker, Unit* victim, uint32& damage)
        {
            if (!victim || !victim->IsInWorld() || !victim->FindMap() || !victim->GetMap()->IsScriptedPrivateInstance())
                return;
            std::lock_guard<std::recursive_mutex> lock(mutex);
            ObjectGuid const owner = victim->GetMap()->ToInstanceMap()->GetScriptedPrivateOwner();
            auto itr = runs.find(owner);
            if (itr == runs.end() || itr->second.instanceId != victim->GetInstanceId())
                return;
            Run const& run = itr->second;
            if (run.phase != Phase::Running)
                damage = 0;
            else if (attacker && attacker->GetGUID() == run.boss)
                damage = uint32(float(damage) * LinkedDamage(uint32(run.guards.size())));
            if (damage && attacker && attacker->IsAlive() && attacker->HasAura(Leeching)
                && (attacker->GetGUID() == run.boss || run.guards.contains(attacker->GetGUID())))
                if (SpellInfo const* info = sSpellMgr->GetSpellInfo(Leeching))
                {
                    uint32 const amount = std::min(damage, victim->GetHealth()) * 30 / 100;
                    HealInfo healInfo(attacker, attacker, amount, info, info->GetSchoolMask());
                    attacker->HealBySpell(healInfo, false);
                }
        }

        bool Gossip(Player* player, Creature* creature, uint32 action)
        {
            std::lock_guard<std::recursive_mutex> lock(mutex);
            auto itr = runs.find(player->GetGUID());
            if (itr == runs.end() || !OwnsScene(player, itr->second) || creature->GetGUID() != itr->second.guide
                || !player->IsAlive() || !player->IsWithinDistInMap(creature, INTERACTION_DISTANCE))
                return false;
            if (!action)
            {
                ClearGossipMenuFor(player);
                if (CanAdvance(itr->second.phase, itr->second.depth))
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Continue the Manastorm", GOSSIP_SENDER_MAIN, 2);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Leave the Manastorm", GOSSIP_SENDER_MAIN, 3);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            }
            else
            {
                CloseGossipMenuFor(player);
                if (action == 3)
                    Exit(player, itr->second);
                else if (action == 2)
                    itr->second.guideAction = 2;
            }
            return true;
        }

    private:
        Player* FindOwner(InstanceMap* map) const
        {
            for (auto const& reference : map->GetPlayers())
                if (Player* player = reference.GetSource())
                    if (player->GetGUID() == map->GetScriptedPrivateOwner())
                        return player;
            return nullptr;
        }

        bool OwnsScene(Player const* player, Run const& run) const
        {
            Map* map = player->FindMap();
            return map && run.instanceId && player->GetMapId() == MapId && player->GetInstanceId() == run.instanceId
                && map->IsScriptedPrivateInstance()
                && map->ToInstanceMap()->GetScriptedPrivateOwner() == player->GetGUID();
        }

        void SendResult(Player* player, uint16 opcode, char const* result)
        {
            WorldPacket packet(opcode, 64);
            packet << result;
            player->SendDirectMessage(&packet);
        }

        void SendCapability(Player* player, bool ready)
        {
            WorldPacket packet;
            std::string const message = ready ? "LOCAL_MANASTORM\t1:15:10:59" : "LOCAL_MANASTORM\t0:0:0:0";
            ChatHandler::BuildChatPacket(packet, CHAT_MSG_WHISPER, LANG_ADDON, player->GetGUID(), player->GetGUID(),
                message, 0, player->GetName(), player->GetName(), 0, false);
            player->SendDirectMessage(&packet);
        }

        void SendProgress(Player* player, Run const& run, bool initial)
        {
            WorldPacket packet(initial ? Data : ProgressUpdate, 128);
            if (!initial)
                packet << player->GetGUID().GetRawValue();
            WriteProgress(packet, run.progress);
            player->SendDirectMessage(&packet);
        }

        void SendActive(Player* player, Run const& run)
        {
            bool const active = run.depth && run.phase != Phase::Idle && run.phase != Phase::Leaving;
            WorldPacket packet(ActiveData, 40);
            WriteActive(packet, active ? run.depth : 0, active ? run.GetScene().stage : 0, 0);
            player->SendDirectMessage(&packet);
        }

        void Start(Player* player, Run& run, uint32 depth)
        {
            char const* error = nullptr;
            if (!run.databaseReady)
                error = "ENTER_MANASTORM_UNKNOWN";
            else if (run.phase != Phase::Idle)
                error = "ENTER_MANASTORM_ALREADY_ACTIVE";
            else if (player->GetLevel() < MinPlayerLevel)
                error = "ENTER_MANASTORM_TOO_LOW_PLAYER_LEVEL";
            else if (player->GetLevel() > MaxPilotPlayerLevel)
                error = "ENTER_MANASTORM_END_GAME_NOT_ENABLED";
            else if (player->GetGroup())
                error = "ENTER_MANASTORM_BAD_GROUP_SIZE";
            else if (!CanStart(depth, run.MaxCompleted()))
                error = "ENTER_MANASTORM_BAD_LEVEL";
            else if (!player->IsInWorld() || !player->FindMap() || player->GetMap()->Instanceable()
                || !player->IsAlive() || player->IsInCombat()
                || player->IsBeingTeleported() || player->GetTransport() || player->GetVehicle() || player->duel)
                error = "ENTER_MANASTORM_UNKNOWN";
            else if (player->IsInFlight())
                error = "ENTER_MANASTORM_ON_FLIGHT_PATH";
            if (error)
            {
                SendResult(player, EnterResult, error);
                return;
            }
            run.returnLocation = WorldLocation(player->GetMapId(), player->GetPositionX(), player->GetPositionY(),
                player->GetPositionZ(), player->GetOrientation());
            run.level = player->GetLevel();
            Transfer(player, run, depth);
        }

        void Transfer(Player* player, Run& run, uint32 depth)
        {
            if (player->IsBeingTeleported() || player->IsInCombat() || !player->IsAlive())
                return;
            Phase const previousPhase = run.phase;
            uint32 const previousDepth = run.depth;
            uint32 const previousInstance = run.instanceId;
            ClearBubble(player);
            run.depth = depth;
            run.phase = Phase::Transferring;
            run.transferStarted = GameTime::GetGameTime().count();
            player->PrepareScriptedPrivateInstance(MapId, run.returnLocation);
            auto const& entry = run.GetScene().entrance;
            if (!player->TeleportTo(MapId, entry.x, entry.y, entry.z, entry.o, 0, nullptr, true))
            {
                run.phase = previousPhase;
                run.depth = previousDepth;
                player->SetScriptedPrivateInstanceId(previousInstance);
                if (!previousInstance)
                    player->ClearScriptedPrivateInstance();
                SendResult(player, EnterResult, "ENTER_MANASTORM_UNKNOWN");
                return;
            }
            if (Map* previous = sMapMgr->FindMap(MapId, previousInstance))
                if (previous->IsScriptedPrivateInstance())
                    previous->ToInstanceMap()->RequestScriptedPrivateUnload();
            run.instanceId = 0;
            run.initialized = false;
            run.guide.Clear();
            run.portal.Clear();
            run.portalArmed = false;
            run.boss.Clear();
            run.guards.clear();
            run.commitReady = false;
        }

        Creature* SpawnEnemy(InstanceMap* map, Spawn const& spawn, Run& run, bool boss)
        {
            if (!sObjectMgr->GetCreatureTemplate(spawn.entry))
                return nullptr;
            float const ground = map->GetHeight(PHASEMASK_NORMAL, spawn.x, spawn.y, spawn.z + 2.0f, true, 10.0f);
            if (!std::isfinite(ground) || std::abs(ground - spawn.z) > 4.0f)
            {
                LOG_ERROR("module.ascension_compat", "Manastorm rejected geometry for scene {}, creature {}",
                    run.GetScene().stage, spawn.entry);
                return nullptr;
            }
            Creature* creature = map->SummonCreature(spawn.entry, Position(spawn.x, spawn.y, spawn.z, spawn.o));
            if (!creature)
                return nullptr;
            creature->SetLevel(run.level);
            creature->SetFaction(16);
            creature->SetReactState(REACT_PASSIVE);
            creature->SetLootRewardDisabled(true);
            creature->SetReputationRewardDisabled(true);
            creature->SetLootMode(0);
            float const scale = 1.0f + float(run.depth - 1) * 0.06f;
            uint32 const health = uint32((140 + 35 * run.level) * scale * (boss ? 4.0f : 1.0f));
            creature->SetMaxHealth(health);
            creature->SetHealth(health);
            creature->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, (5.0f + run.level) * scale);
            creature->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, (8.0f + run.level * 1.4f) * scale);
            creature->UpdateDamagePhysical(BASE_ATTACK);
            if (run.depth >= 6)
            {
                constexpr std::array<uint32, 3> affixes = {UnrelentingSpeed, Leeching, TribalFury};
                // Only the boss carries the nearby-allies emitter; do not multiply emitters on each guard.
                uint32 const affix = affixes[(run.depth - 6) % affixes.size()];
                if (affix != TribalFury || boss)
                    creature->AddAura(affix, creature);
            }
            if (boss)
                run.bossBaseHealth = health;
            return creature;
        }

        void UpdateLink(Player* player, Run& run)
        {
            uint32 const stacks = uint32(run.guards.size());
            if (Creature* boss = player->GetMap()->GetCreature(run.boss))
            {
                float const fraction = boss->GetHealthPct() / 100.0f;
                uint32 const health = LinkedHealth(run.bossBaseHealth, stacks);
                boss->SetMaxHealth(health);
                boss->SetHealth(std::max(1u, uint32(health * fraction)));
                if (stacks)
                {
                    if (Aura* aura = boss->AddAura(LinkSpell, boss))
                        aura->SetStackAmount(uint8(stacks));
                }
                else
                    boss->RemoveAurasDueToSpell(LinkSpell);
            }
            WorldPacket packet(ChaoticLink, 4);
            packet << stacks;
            player->SendDirectMessage(&packet);
        }

        void Complete(Player* player, Run& run)
        {
            run.phase = Phase::Committing;
            // Replays earn no additional first-clear mail.
            if (std::binary_search(run.progress[0].begin(), run.progress[0].end(), run.depth))
            {
                Finish(player, run);
                return;
            }
            auto mail = std::make_shared<Mail>();
            mail->messageID = sObjectMgr->GenerateMailID();
            mail->messageType = MAIL_CREATURE;
            mail->stationery = MAIL_STATIONERY_DEFAULT;
            mail->sender = GuideEntry;
            mail->receiver = player->GetGUID().GetCounter();
            mail->subject = "Manastorm: first clear " + std::to_string(run.depth);
            mail->body = "Local Manastorm pilot reward. Your checkpoint progress has been saved.";
            mail->money = RewardCopper;
            mail->deliver_time = GameTime::GetGameTime().count();
            mail->expire_time = mail->deliver_time + 30 * DAY;
            mail->checked = MAIL_CHECK_MASK_HAS_BODY;
            mail->state = MAIL_STATE_UNCHANGED;
            CharacterDatabaseTransaction transaction = CharacterDatabase.BeginTransaction();
            auto* clear = CharacterDatabase.GetPreparedStatement(CHAR_INS_MANASTORM_CLEAR);
            clear->SetData(0, player->GetGUID().GetCounter());
            clear->SetData(1, uint8(0));
            clear->SetData(2, run.depth);
            clear->SetData(3, run.GetScene().stage);
            clear->SetData(4, mail->messageID);
            clear->SetData(5, uint32(mail->deliver_time));
            transaction->Append(clear);
            // Native mail persistence, with in-memory publication postponed until COMMIT succeeds.
            auto* statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_MAIL);
            statement->SetData(0, mail->messageID);
            statement->SetData(1, mail->messageType);
            statement->SetData(2, int8(mail->stationery));
            statement->SetData(3, uint16(0));
            statement->SetData(4, mail->sender);
            statement->SetData(5, mail->receiver);
            statement->SetData(6, mail->subject);
            statement->SetData(7, mail->body);
            statement->SetData(8, false);
            statement->SetData(9, uint32(mail->expire_time));
            statement->SetData(10, uint32(mail->deliver_time));
            statement->SetData(11, mail->money);
            statement->SetData(12, uint32(0));
            statement->SetData(13, uint8(mail->checked));
            transaction->Append(statement);
            ObjectGuid const guid = player->GetGUID();
            uint32 const instanceId = run.instanceId;
            uint32 const depth = run.depth;
            transactions.emplace_back(CharacterDatabase.AsyncCommitTransaction(transaction));
            transactions.back().AfterComplete([this, guid, instanceId, depth, mail](bool success)
            {
                auto itr = runs.find(guid);
                if (success)
                {
                    sMailMgr->OnMailSent(guid.GetCounter());
                    readyMails[guid].push_back(mail);
                    if (itr != runs.end())
                    {
                        auto& progress = itr->second.progress[0];
                        if (!std::binary_search(progress.begin(), progress.end(), depth))
                        {
                            progress.push_back(depth);
                            std::sort(progress.begin(), progress.end());
                        }
                        itr->second.progressDirty = true;
                    }
                }
                if (itr != runs.end() && itr->second.instanceId == instanceId && itr->second.depth == depth)
                {
                    itr->second.commitReady = true;
                    itr->second.commitSucceeded = success;
                }
            });
        }

        void Finish(Player* player, Run& run)
        {
            if (!std::binary_search(run.progress[0].begin(), run.progress[0].end(), run.depth))
            {
                run.progress[0].push_back(run.depth);
                std::sort(run.progress[0].begin(), run.progress[0].end());
            }
            run.phase = Phase::Completed;
            for (auto const& guid : run.guards)
                if (Creature* guard = player->GetMap()->GetCreature(guid))
                    guard->DespawnOrUnsummon();
            run.guards.clear();
            player->CombatStop(true);
            WorldPacket completed(CompletedLevel, 4);
            completed << run.depth;
            player->SendDirectMessage(&completed);
            SendProgress(player, run, false);
            auto const& pos = run.GetScene().boss;
            if (Creature* portal = player->GetMap()->SummonCreature(12999, Position(pos.x, pos.y, pos.z, pos.o)))
            {
                portal->SetReactState(REACT_PASSIVE);
                portal->AddAura(PortalAura, portal);
                run.portal = portal->GetGUID();
            }
            if (Creature* guide = player->GetMap()->SummonCreature(GuideEntry, Position(pos.x, pos.y, pos.z, pos.o)))
            {
                guide->SetFaction(35);
                guide->SetReactState(REACT_PASSIVE);
                guide->ReplaceAllNpcFlags(UNIT_NPC_FLAG_GOSSIP);
                run.guide = guide->GetGUID();
            }
            ChatHandler(player->GetSession()).SendSysMessage(run.depth < MaxDepth
                ? "Manastorm complete. Speak to Cogsley to continue or leave. First-clear rewards arrive by mail."
                : "Manastorm pilot complete: depth 15. Speak to Cogsley to leave. Your progress is saved.");
        }

        void ClearBubble(Player* player)
        {
            player->RemoveDynObject(SafetyBubble);
            player->RemoveAurasDueToSpell(SafetyBubble);
            player->RemoveAurasDueToSpell(SafetyBubbleHelper);
        }

        void FailRun(Player* player, Run& run)
        {
            if (run.phase == Phase::Committing)
                return; // Preserve the pending durable completion until its outcome is known.
            if (run.phase != Phase::Failed)
            {
                WorldPacket failed(Fail, 0);
                player->SendDirectMessage(&failed);
            }
            run.phase = Phase::Failed;
            Exit(player, run);
        }

        void Exit(Player* player, Run& run)
        {
            if (run.phase == Phase::Idle)
            {
                SendResult(player, LeaveResult, "LEAVE_MANASTORM_NOT_ACTIVE");
                return;
            }
            if (player->IsBeingTeleported() || run.phase == Phase::Committing)
            {
                SendResult(player, LeaveResult, "LEAVE_MANASTORM_TRANSITION");
                return;
            }
            ClearBubble(player);
            player->CombatStop(true);
            if (!player->IsAlive())
            {
                player->ResurrectPlayer(0.5f);
                player->SpawnCorpseBones();
            }
            Phase const previous = run.phase;
            run.phase = Phase::Leaving;
            player->ClearScriptedPrivateInstance();
            if (!player->TeleportTo(run.returnLocation))
            {
                player->PrepareScriptedPrivateInstance(MapId, run.returnLocation);
                player->SetScriptedPrivateInstanceId(run.instanceId);
                run.phase = previous;
                SendResult(player, LeaveResult, "LEAVE_MANASTORM_UNKNOWN");
            }
            else if (Map* previousMap = sMapMgr->FindMap(MapId, run.instanceId))
                if (previousMap->IsScriptedPrivateInstance())
                    previousMap->ToInstanceMap()->RequestScriptedPrivateUnload();
        }

        std::atomic<bool> enabled{false};
        std::atomic<uint64> nextToken{0};
        std::mutex queueMutex;
        std::recursive_mutex mutex;
        std::map<uint32, std::deque<Request>> requests;
        std::map<ObjectGuid, Run> runs;
        std::map<ObjectGuid, std::vector<std::shared_ptr<Mail>>> readyMails;
        std::list<TransactionCallback> transactions;
    };

    class ManastormInstance final : public InstanceScript
    {
    public:
        explicit ManastormInstance(InstanceMap* map) : InstanceScript(map) { }
        void OnPlayerEnter(Player* player) override
        {
            ManastormService::Get().Entered(instance->ToInstanceMap(), player);
        }
        void Update(uint32 diff) override { ManastormService::Get().UpdateInstance(instance->ToInstanceMap(), diff); }
    };

    class ManastormMaps final : public AllMapScript
    {
    public:
        ManastormMaps() : AllMapScript("AscensionManastormMaps") { }
        void OnBeforeCreateInstanceScript(InstanceMap* map, InstanceScript** script, bool, std::string, uint32) override
        {
            if (map->GetId() == MapId && map->IsScriptedPrivateInstance())
                *script = new ManastormInstance(map);
        }
    };

    class ManastormPlayers final : public PlayerScript
    {
    public:
        ManastormPlayers() : PlayerScript("AscensionManastormPlayers") { }
        void OnPlayerLogin(Player* player) override { ManastormService::Get().Login(player); }
        void OnPlayerLogout(Player* player) override { ManastormService::Get().Logout(player); }
        void OnPlayerDeleteFromDB(CharacterDatabaseTransaction transaction, uint32 guid) override
        {
            auto* statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_MANASTORM_CLEARS);
            statement->SetData(0, guid);
            transaction->Append(statement);
        }
        void OnPlayerMapChanged(Player* player) override { ManastormService::Get().MapChanged(player); }
        void OnPlayerUpdate(Player* player, uint32 diff) override
        {
            ManastormService::Get().UpdatePlayer(player, diff);
        }
        void OnPlayerGiveXP(Player* player, uint32& amount, Unit*, uint8) override
        {
            Map const* map = player->FindMap();
            if (map && map->IsScriptedPrivateInstance())
                amount = 0;
        }
        bool OnPlayerPassedQuestKilledMonsterCredit(Player* player, Quest const*, uint32, uint32, ObjectGuid) override
        {
            Map const* map = player->FindMap();
            return !map || !map->IsScriptedPrivateInstance();
        }
        bool OnPlayerBeforeCriteriaProgress(Player* player, AchievementCriteriaEntry const* criteria) override
        {
            // LoadFromDB updates criteria before SetMap, including ordinary character logins.
            Map const* map = player->FindMap();
            return !map || !map->IsScriptedPrivateInstance()
                || criteria->requiredType != ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE;
        }
    };

    class ManastormUnits final : public UnitScript
    {
    public:
        ManastormUnits() : UnitScript("AscensionManastormUnits") { }
        void OnUnitDeath(Unit* unit, Unit*) override { ManastormService::Get().Death(unit); }
        void OnDamage(Unit* attacker, Unit* victim, uint32& damage) override
        {
            ManastormService::Get().Damage(attacker, victim, damage);
        }
    };

    class ManastormGuides final : public AllCreatureScript
    {
    public:
        ManastormGuides() : AllCreatureScript("AscensionManastormGuides") { }
        void OnCreatureAddWorld(Creature* creature) override
        {
            if (creature->GetMap()->IsScriptedPrivateInstance())
            {
                creature->SetLootRewardDisabled(true);
                creature->SetReputationRewardDisabled(true);
                creature->SetLootMode(0);
            }
        }
        bool CanCreatureGossipHello(Player* player, Creature* creature) override
        {
            return ManastormService::Get().Gossip(player, creature, 0);
        }
        bool CanCreatureGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
        {
            return sender == GOSSIP_SENDER_MAIN && ManastormService::Get().Gossip(player, creature, action);
        }
    };

    class ManastormWorld final : public WorldScript
    {
    public:
        ManastormWorld() : WorldScript("AscensionManastormWorld") { }
        void OnAfterConfigLoad(bool reload) override { if (!reload) ManastormService::Get().Configure(); }
        void OnUpdate(uint32) override { ManastormService::Get().PollTransactions(); }
    };

    class ManastormCommands final : public CommandScript
    {
    public:
        ManastormCommands() : CommandScript("AscensionManastormCommands") { }
        ChatCommandTable GetCommands() const override
        {
            // ChatCommandBuilder retains a reference to its child table.
            static ChatCommandTable const manastormCommands = {
                {"enter", EnterCommand, SEC_PLAYER, Console::No},
                {"leave", LeaveCommand, SEC_PLAYER, Console::No},
                {"next", NextCommand, SEC_PLAYER, Console::No},
                {"start", StartCommand, SEC_PLAYER, Console::No},
                {"status", StatusCommand, SEC_PLAYER, Console::No}
            };
            static ChatCommandTable const commands = {
                {"manastorm", manastormCommands}
            };
            return commands;
        }
        static bool EnterCommand(ChatHandler* handler, uint32 depth)
        {
            return ManastormService::Get().Command(handler->GetPlayer(), "enter", depth);
        }
        static bool LeaveCommand(ChatHandler* handler)
        {
            return ManastormService::Get().Command(handler->GetPlayer(), "leave");
        }
        static bool NextCommand(ChatHandler* handler)
        {
            return ManastormService::Get().Command(handler->GetPlayer(), "next");
        }
        static bool StartCommand(ChatHandler* handler)
        {
            return ManastormService::Get().Command(handler->GetPlayer(), "start");
        }
        static bool StatusCommand(ChatHandler* handler)
        {
            return ManastormService::Get().Command(handler->GetPlayer(), "status");
        }
    };
}

bool QueueAscensionManastormPacket(WorldSession* session, WorldPacket const& packet)
{
    return ManastormService::Get().Queue(session, packet);
}

void AddAscensionManastormScripts()
{
    new ManastormWorld();
    new ManastormMaps();
    new ManastormPlayers();
    new ManastormUnits();
    new ManastormGuides();
    new ManastormCommands();
}
