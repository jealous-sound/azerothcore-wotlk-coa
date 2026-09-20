/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license:
 * https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "AccountMgr.h"
#include "AscensionWisdomball.h"
#include "AsyncCallbackProcessor.h"
#include "Bag.h"
#include "CharacterCache.h"
#include "CharmInfo.h"
#include "Chat.h"
#include "Config.h"
#include "Creature.h"
#include "DBCStores.h"
#include "DatabaseEnv.h"
#include "DynamicObject.h"
#include "GameObject.h"
#include "GameTime.h"
#include "GitRevision.h"
#include "GossipDef.h"
#include "Group.h"
#include "GroupMgr.h"
#include "Item.h"
#include "ItemPackets.h"
#include "Log.h"
#include "Map.h"
#include "MapMgr.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Pet.h"
#include "Player.h"
#include "QuestDef.h"
#include "QueryCallback.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "TemporarySummon.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "WhoListCacheMgr.h"
// BOOST_BIND_NO_PLACEHOLDERS (deps/boost) stops boost/bind/bind.hpp from including
// placeholders.hpp, but the Boost.PropertyTree JSON parser uses boost::placeholders (e.g. Boost 1.83).
#include <boost/bind/placeholders.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <list>
#include <limits>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

namespace
{
using Tree = boost::property_tree::ptree;
using Clock = std::chrono::steady_clock;
constexpr uint32 TestPhase = 1u << 30;
constexpr uint32 MaximumActors = 8;

void Require(bool condition, std::string const& message)
{
    if (!condition)
        throw std::runtime_error(message);
}

std::list<GameObject*> OwnedGameObjects(Player* player, uint32 entry)
{
    Require(sObjectMgr->GetGameObjectTemplate(entry) != nullptr, "Unknown gameobject entry");
    std::list<GameObject*> objects;
    player->GetGameObjectListWithEntryInGrid(objects, entry, 100.0f);
    objects.remove_if([player](GameObject* object)
    {
        return !object->IsInWorld() || object->GetOwnerGUID() != player->GetGUID() || !player->InSamePhase(object);
    });
    return objects;
}

void WriteResult(std::string const& path, Tree const& result)
{
    Require(!path.empty() && !std::filesystem::exists(path), "Output path must be new");
    std::string temporary = path + ".tmp";
    boost::property_tree::write_json(temporary, result);
    std::filesystem::rename(temporary, path);
}

uint64 Elapsed(Clock::time_point start)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count();
}

enum class ActorStage
{
    Account,
    Creating,
    Enumerating,
    LoggingIn,
    Transfer,
    Ready
};

struct SpellDamageEvent
{
    ObjectGuid caster;
    ObjectGuid target;
    uint32 spell = 0;
    uint32 damage = 0;
    bool critical = false;
};

struct SpellHealEvent
{
    ObjectGuid caster;
    ObjectGuid target;
    uint32 spell = 0;
    uint32 heal = 0;
    uint32 overheal = 0;
    bool critical = false;
};

struct SpellEnergizeEvent
{
    ObjectGuid caster;
    ObjectGuid target;
    uint32 spell = 0;
    uint32 power = 0;
    uint32 amount = 0;
};

struct Actor
{
    Tree definition;
    std::string account;
    std::string name;
    std::map<std::string, uint32> whoClasses;
    uint32 whoResponses = 0;
    uint32 lootReceived = 0;
    std::array<uint32, 2> meleeAttacksByHand{};
    std::array<uint32, 2> meleeDamageByHand{};
    uint64 castPushbackMs = 0;
    std::vector<SpellDamageEvent> spellDamage;
    std::vector<SpellHealEvent> spellHeals;
    std::vector<SpellEnergizeEvent> spellEnergizes;
    Tree castFailures;
    uint32 lastQuestWindow = 0; // the last quest window this session sent, by opcode
    std::unique_ptr<WorldSession> session;
    ObjectGuid guid;
    ActorStage stage = ActorStage::Account;
};

struct Target
{
    uint32 map;
    uint32 instance;
    ObjectGuid guid;
};

void ObserveSpellDamage(Actor& actor, WorldPacket const& packet)
{
    if (packet.GetOpcode() != SMSG_SPELLNONMELEEDAMAGELOG && packet.GetOpcode() != SMSG_PERIODICAURALOG)
        return;

    WorldPacket response(packet);
    SpellDamageEvent event;
    response >> event.target.ReadAsPacked() >> event.caster.ReadAsPacked() >> event.spell;
    if (packet.GetOpcode() == SMSG_SPELLNONMELEEDAMAGELOG)
    {
        // Unit::SendSpellNonMeleeDamageLog: damage, overkill, school, absorb,
        // resist, physical-log flag, unused flag, block, hit info.
        response >> event.damage;
        response.read_skip<uint32>();
        response.read_skip<uint8>();
        response.read_skip<uint32>();
        response.read_skip<uint32>();
        response.read_skip<uint8>();
        response.read_skip<uint8>();
        response.read_skip<uint32>();
        uint32 hitInfo;
        response >> hitInfo;
        event.critical = (hitInfo & SPELL_HIT_TYPE_CRIT) != 0;
    }
    else
    {
        uint32 count, aura;
        response >> count >> aura;
        Require(count == 1, "Expected one native periodic aura event");
        if (aura != SPELL_AURA_PERIODIC_DAMAGE && aura != SPELL_AURA_PERIODIC_DAMAGE_PERCENT)
            return;
        response >> event.damage;
        response.read_skip<uint32>(); // overkill
        response.read_skip<uint32>(); // school
        response.read_skip<uint32>(); // absorb
        response.read_skip<uint32>(); // resist
        uint8 critical;
        response >> critical;
        event.critical = critical != 0;
    }
    if (event.damage)
        actor.spellDamage.push_back(event);
}

void ObserveSpellHealing(Actor& actor, WorldPacket const& packet)
{
    if (packet.GetOpcode() != SMSG_SPELLHEALLOG && packet.GetOpcode() != SMSG_PERIODICAURALOG)
        return;

    WorldPacket response(packet);
    SpellHealEvent event;
    response >> event.target.ReadAsPacked() >> event.caster.ReadAsPacked() >> event.spell;
    if (packet.GetOpcode() == SMSG_PERIODICAURALOG)
    {
        uint32 count, aura;
        response >> count >> aura;
        Require(count == 1, "Expected one native periodic aura event");
        if (aura != SPELL_AURA_PERIODIC_HEAL && aura != SPELL_AURA_OBS_MOD_HEALTH)
            return;
    }
    response >> event.heal >> event.overheal;
    response.read_skip<uint32>(); // absorb; both log formats already exclude it from healing
    uint8 critical;
    response >> critical;
    event.critical = critical != 0;
    Require(event.overheal <= event.heal, "Native overhealing exceeds healing");
    if (event.heal)
        actor.spellHeals.push_back(event);
}

void ObserveSpellEnergize(Actor& actor, WorldPacket const& packet)
{
    if (packet.GetOpcode() != SMSG_SPELLENERGIZELOG && packet.GetOpcode() != SMSG_PERIODICAURALOG)
        return;
    WorldPacket response(packet);
    SpellEnergizeEvent event;
    response >> event.target.ReadAsPacked() >> event.caster.ReadAsPacked() >> event.spell;
    if (packet.GetOpcode() == SMSG_PERIODICAURALOG)
    {
        uint32 count, aura;
        response >> count >> aura;
        Require(count == 1, "Expected one native periodic aura event");
        if (aura != SPELL_AURA_OBS_MOD_POWER && aura != SPELL_AURA_PERIODIC_ENERGIZE)
            return;
    }
    response >> event.power >> event.amount;
    Require(event.power < MAX_POWERS, "Invalid native energize power");
    actor.spellEnergizes.push_back(event);
}

// Sessions are owned here, outside the network session manager. Character creation,
// enumeration, DB loading and spell/item use run through the existing session handlers.
// No socket/authentication, client rendering or packet-delivery coverage is implied.
class CoAGameplayTest final : public WorldScript
{
public:
    CoAGameplayTest() : WorldScript("CoAGameplayTest", { WORLDHOOK_ON_STARTUP,
        WORLDHOOK_ON_UPDATE, WORLDHOOK_ON_SHUTDOWN }) { }

    void OnStartup() override
    {
        if (!sConfigMgr->GetOption<bool>("CoAGameplayTest.Enable", false))
            return;

        _enabled = true;
        _started = Clock::now();
        try
        {
            _runId = sConfigMgr->GetOption<std::string>("CoAGameplayTest.RunId", "");
            Require(_runId.size() == 12 && _runId.find_first_not_of("0123456789abcdef") == std::string::npos,
                "RunId must be twelve lowercase hexadecimal characters");
            CheckIsolation();
            _resultPath = sConfigMgr->GetOption<std::string>("CoAGameplayTest.ResultFile", "");
            Require(!std::filesystem::exists(_resultPath), "Result file already exists");
            _startFile = sConfigMgr->GetOption<std::string>("CoAGameplayTest.StartFile", "");
            Require(_startFile.empty() || !std::filesystem::exists(_startFile), "Start file already exists");
            boost::property_tree::read_json(
                sConfigMgr->GetOption<std::string>("CoAGameplayTest.ScenarioFile", ""), _scenario);
            Require(_scenario.get<uint32>("schema") == 1, "Unsupported scenario schema");
            _timeout = _scenario.get<uint32>("timeout_ms", 90000);
            Require(_timeout > 0 && _timeout <= 600000, "Invalid scenario timeout");
            _report.put("schema", 1);
            _report.put("run_id", _runId);
            _report.put("scenario", _scenario.get<std::string>("name"));
            _report.put("server_version", GitRevision::GetFullVersion());
            _report.put("execution", "socketless-session-handlers");
            _report.put("data_dir", sWorld->GetDataPath());
            _steps = _scenario.get_child("steps");
            Require(!_steps.empty() && _steps.size() <= 10000, "Scenario needs 1..10000 steps");
            _nextStep = _steps.begin();

            auto const& players = _scenario.get_child("players");
            Require(!players.empty() && players.size() <= MaximumActors, "Scenario needs 1..8 players");
            uint32 index = 0;
            for (auto const& entry : players)
            {
                std::string id = entry.second.get<std::string>("id");
                Require(!id.empty() && !_actors.count(id), "Duplicate or empty player id");
                auto& actor = _actors[id];
                actor.definition = entry.second;
                actor.account = "CT" + _runId + std::to_string(index);
                // Character names contain letters only and are unique inside the fresh test database.
                actor.name = "Harness" + std::string(1, char('a' + index++));
                Require(AccountMgr::GetId(actor.account) == 0, "Test account already exists");
                Require(sAccountMgr->CreateAccount(actor.account, _runId) == AOR_OK, "Account creation failed");
            }

            Tree ready;
            ready.put("run_id", _runId);
            ready.put("status", "ready");
            ready.put("waiting_for_start", !_startFile.empty());
            WriteResult(sConfigMgr->GetOption<std::string>("CoAGameplayTest.ReadyFile", ""), ready);
            LOG_INFO("module.gameplay_test", "Gameplay harness ready: {}", _runId);
        }
        catch (std::exception const& error)
        {
            Finish(false, error.what());
        }
    }

    void OnUpdate(uint32 /*diff*/) override
    {
        if (!_enabled || _finished)
            return;

        try
        {
            // The runner records the world DB after startup migrations, before scenario actions can write it.
            if (!_startFile.empty())
            {
                Require(Elapsed(_started) < 600000, "Runner did not release the startup barrier");
                if (!std::filesystem::exists(_startFile))
                    return;
                Tree start;
                boost::property_tree::read_json(_startFile, start);
                Require(start.get<std::string>("run_id") == _runId, "Start file belongs to another run");
                _startFile.clear();
                _started = Clock::now();
            }
            Require(Elapsed(_started) < _timeout, "Scenario timed out during setup or execution");
            _queries.ProcessReadyCallbacks();
            bool ready = true;
            for (auto& [id, actor] : _actors)
            {
                PumpActor(id, actor);
                ready = ready && actor.stage == ActorStage::Ready;
            }
            if (!ready)
                return;
            if (!_targetsCreated)
                CreateTargets();
            if (_nextStep == _steps.end())
            {
                Require(_assertions > 0, "Scenario completed without assertions");
                Finish(true, "All assertions passed");
                return;
            }
            RunStep(_nextStep->second);
        }
        catch (std::exception const& error)
        {
            Finish(false, error.what());
        }
    }

    void OnShutdown() override
    {
        if (_enabled && !_finished)
            Finish(false, "Server shut down before the scenario completed");
    }

private:
    void CheckIsolation()
    {
        std::string worldId = sConfigMgr->GetOption<std::string>("CoAGameplayTest.WorldDatabaseId", _runId);
        Require(worldId.size() == 12 && worldId.find_first_not_of("0123456789abcdef") == std::string::npos,
            "WorldDatabaseId must be twelve lowercase hexadecimal characters");
        for (auto const& [key, suffix] : std::map<std::string, std::string>{
            { "LoginDatabaseInfo", "auth" }, { "CharacterDatabaseInfo", "characters" },
            { "WorldDatabaseInfo", "world" } })
        {
            std::string connection = sConfigMgr->GetOption<std::string>(key, "");
            auto first = connection.find(';');
            auto last = connection.rfind(';');
            Require(first != std::string::npos && last != first, "Invalid database connection");
            std::string host = connection.substr(0, first);
            Require(host == "127.0.0.1" || host == "localhost" || host == "::1", "Test DB must be local");
            std::string databaseId = suffix == "world" ? worldId : _runId;
            Require(connection.substr(last + 1) == "coa_test_" + databaseId + "_" + suffix,
                "Harness requires its own named test databases");
        }
        Require(sConfigMgr->GetOption<std::string>("BindIP", "") == "127.0.0.1", "BindIP must be loopback");
        Require(sConfigMgr->GetOption<uint32>("MapUpdate.Threads", 1) == 0, "Map workers must be disabled");
    }

    void PumpActor(std::string const& id, Actor& actor)
    {
        if (actor.stage == ActorStage::Account)
        {
            // AccountMgr queues its writes. Do not assume CreateAccount means the row is committed.
            uint32 accountId = AccountMgr::GetId(actor.account);
            if (!accountId)
                return;
            actor.session = std::make_unique<WorldSession>(accountId, std::string(actor.account), 0, nullptr,
                SEC_PLAYER, EXPANSION_WRATH_OF_THE_LICH_KING, 0, LOCALE_enUS, 0, false, false, 0);
            actor.session->SetSocketlessPacketObserver([&actor](WorldPacket const& packet)
            {
                ObserveSpellDamage(actor, packet);
                ObserveSpellHealing(actor, packet);
                ObserveSpellEnergize(actor, packet);
                if (packet.GetOpcode() == SMSG_SPELL_DELAYED)
                {
                    WorldPacket response(packet);
                    ObjectGuid caster;
                    uint32 delay;
                    response >> caster.ReadAsPacked() >> delay;
                    if (caster == actor.guid)
                        actor.castPushbackMs += delay;
                }
                if (packet.GetOpcode() == SMSG_CAST_FAILED)
                {
                    WorldPacket response(packet);
                    uint8 count, reason;
                    uint32 spell;
                    response >> count >> spell >> reason;
                    Tree failure;
                    failure.put("cast_count", uint32(count));
                    failure.put("spell", spell);
                    failure.put("reason", uint32(reason));
                    actor.castFailures.push_back({"", failure});
                }
                if (packet.GetOpcode() == SMSG_ATTACKERSTATEUPDATE)
                {
                    WorldPacket response(packet);
                    uint32 hitInfo;
                    uint32 damage;
                    ObjectGuid attacker, victim;
                    response >> hitInfo >> attacker.ReadAsPacked() >> victim.ReadAsPacked() >> damage;
                    if (attacker == actor.guid)
                    {
                        uint8 hand = hitInfo & HITINFO_OFFHAND ? OFF_ATTACK : BASE_ATTACK;
                        ++actor.meleeAttacksByHand[hand];
                        if (damage)
                            ++actor.meleeDamageByHand[hand];
                    }
                }
                // Which window a click is answered with is the part the client would draw, and
                // the part a click that answers with the wrong one leaves looping. Record it.
                if (packet.GetOpcode() == SMSG_QUESTGIVER_OFFER_REWARD ||
                    packet.GetOpcode() == SMSG_QUESTGIVER_REQUEST_ITEMS ||
                    packet.GetOpcode() == SMSG_QUESTGIVER_QUEST_DETAILS)
                    actor.lastQuestWindow = packet.GetOpcode();

                if (packet.GetOpcode() != SMSG_WHO)
                    return;
                WorldPacket response(packet);
                uint32 displayed, matches;
                response >> displayed >> matches;
                Require(displayed <= matches, "Invalid Who response counts");
                actor.whoClasses.clear();
                for (uint32 index = 0; index < displayed; ++index)
                {
                    std::string name, guild;
                    uint32 level, playerClass, race, zone;
                    uint8 gender;
                    response >> name >> guild >> level >> playerClass >> race >> gender >> zone;
                    actor.whoClasses.emplace(name, playerClass);
                }
                Require(response.rpos() == response.size(), "Unexpected Who response fields");
                ++actor.whoResponses;
            });
            actor.session->InitializeSession();
            WorldPacket create(CMSG_CHAR_CREATE, 32);
            uint32 race = actor.definition.get<uint32>("race");
            uint32 playerClass = actor.definition.get<uint32>("class");
            Require(race > 0 && race <= 255 && playerClass > 0 && playerClass <= 255,
                "Race/class must fit the character creation packet");
            create << actor.name << uint8(race) << uint8(playerClass);
            for (uint8 i = 0; i < 7; ++i)
                create << uint8(0); // gender, skin, face, hair style/color, facial hair, outfit
            actor.session->HandleCharCreateOpcode(create);
            actor.stage = ActorStage::Creating;
        }

        // Maps update logged-in sessions. Before entering a map, pump the same public
        // update path so asynchronous character creation and login callbacks can finish.
        if (!actor.session->GetPlayer() || !actor.session->GetPlayer()->IsInWorld())
        {
            MapSessionFilter filter(actor.session.get());
            actor.session->Update(0, filter);
        }
        Require(!actor.session->IsKicked(), "Test session was kicked: " + id);
        if (actor.stage == ActorStage::Creating)
        {
            actor.guid = sCharacterCache->GetCharacterGuidByName(actor.name);
            if (!actor.guid)
                return;
            auto* statement = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ENUM);
            statement->SetData(0, PET_SAVE_AS_CURRENT);
            statement->SetData(1, actor.session->GetAccountId());
            actor.stage = ActorStage::Enumerating;
            _queries.AddCallback(CharacterDatabase.AsyncQuery(statement).WithPreparedCallback(
                [this, id](PreparedQueryResult result)
                {
                    Require(bool(result), "Created character missing from enumeration");
                    auto& current = _actors.at(id);
                    current.session->HandleCharEnum(result);
                    WorldPacket login(CMSG_PLAYER_LOGIN, 8);
                    login << current.guid;
                    current.session->HandlePlayerLoginOpcode(login);
                    current.stage = ActorStage::LoggingIn;
                }));
        }

        Player* player = actor.session->GetPlayer();
        if (!player)
        {
            Require(actor.stage != ActorStage::Ready, "Test player logged out: " + id);
            return;
        }

        if (actor.stage == ActorStage::LoggingIn)
        {
            if (actor.session->PlayerLoading() || !player->IsInWorld())
                return;
            uint32 level = actor.definition.get<uint32>("level", 80);
            Require(level > 0 && level <= uint32(sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL)),
                "Invalid player level");
            player->SetPhaseMask(TestPhase, true);
            player->GiveLevel(uint8(level));
            Require(player->GetLevel() == level, "Fixture level change rejected");
            if (auto hitRating = actor.definition.get_optional<int32>("spell_hit_rating"))
                player->ApplyRatingMod(CR_HIT_SPELL, *hitRating, true);
            if (auto critRating = actor.definition.get_optional<int32>("spell_crit_rating"))
                player->ApplyRatingMod(CR_CRIT_SPELL, *critRating, true);
            if (auto hitRating = actor.definition.get_optional<int32>("ranged_hit_rating"))
                player->ApplyRatingMod(CR_HIT_RANGED, *hitRating, true);
            if (auto hitRating = actor.definition.get_optional<int32>("melee_hit_rating"))
                player->ApplyRatingMod(CR_HIT_MELEE, *hitRating, true);
            if (auto expertise = actor.definition.get_optional<int32>("expertise_rating"))
                player->ApplyRatingMod(CR_EXPERTISE, *expertise, true);
            player->SetHealth(player->GetMaxHealth());
            for (uint8 power = 0; power < MAX_POWERS; ++power)
                player->SetPower(Powers(power), player->GetMaxPower(Powers(power)));
            actor.stage = ActorStage::Transfer;
            if (auto location = _scenario.get_child_optional("location"))
                Require(player->TeleportTo(location->get<uint32>("map"), location->get<float>("x"),
                    location->get<float>("y"), location->get<float>("z"), location->get<float>("o", 0),
                    location->get<bool>("ignore_access", false) ? TELE_TO_GM_MODE : 0),
                    "Fixture teleport failed");
        }

        // A socketless test actor supplies the acknowledgements a client would send.
        // Resolve it from its owning session while it is between maps.
        player = actor.session->GetPlayer();
        if (player && player->IsBeingTeleportedFar())
            actor.session->HandleMoveWorldportAck();
        player = actor.session->GetPlayer();
        if (player && player->IsInWorld() && player->IsBeingTeleportedNear())
        {
            WorldPacket ack(MSG_MOVE_TELEPORT_ACK, 20);
            ack << player->GetPackGUID() << uint32(0) << uint32(0);
            actor.session->HandleMoveTeleportAck(ack);
        }
        if (actor.stage == ActorStage::Transfer && player && player->IsInWorld()
            && !player->IsBeingTeleported())
            actor.stage = ActorStage::Ready;
    }

    Player* GetPlayer(std::string const& id)
    {
        auto itr = _actors.find(id);
        Require(itr != _actors.end(), "Unknown player: " + id);
        Player* player = ObjectAccessor::FindPlayer(itr->second.guid);
        Require(player && player->FindMap() && !player->IsBeingTeleported(), "Player unavailable: " + id);
        return player;
    }

    Unit* GetUnit(std::string const& id)
    {
        if (_actors.count(id))
            return GetPlayer(id);
        auto itr = _targets.find(id);
        Require(itr != _targets.end(), "Unknown actor: " + id);
        Map* map = sMapMgr->FindMap(itr->second.map, itr->second.instance);
        Creature* creature = map ? map->GetCreature(itr->second.guid) : nullptr;
        Require(creature != nullptr, "Creature disappeared: " + id);
        return creature;
    }

    // The creature of that entry this player owns and has out. A summoned companion is not a
    // scenario fixture, so quest steps address it by entry instead of by actor name.
    Creature* GetOwnedCreature(Player* player, uint32 entry)
    {
        std::list<Creature*> creatures;
        player->GetCreatureListWithEntryInGrid(creatures, entry, 100.0f);
        for (Creature* creature : creatures)
            if (creature->IsAlive() && creature->GetOwnerGUID() == player->GetGUID()
                && player->InSamePhase(creature))
                return creature;

        return nullptr;
    }

    /// The summoned giver a quest step is aimed at: the player's own first, then any summon of
    /// that entry standing within reach - a ball another player put out serves whoever is at it.
    Creature* GetGiver(Player* player, uint32 entry)
    {
        if (Creature* owned = GetOwnedCreature(player, entry))
            return owned;

        std::list<Creature*> creatures;
        player->GetCreatureListWithEntryInGrid(creatures, entry, 30.0f);
        for (Creature* creature : creatures)
            if (creature->IsAlive() && player->InSamePhase(creature))
                return creature;

        return nullptr;
    }

    void CreateTargets()
    {
        if (auto creatures = _scenario.get_child_optional("creatures"))
        {
            Require(creatures->size() <= MaximumActors, "Too many creatures");
            for (auto const& entry : *creatures)
            {
                Tree const& definition = entry.second;
                std::string id = definition.get<std::string>("id");
                std::string owner = definition.get<std::string>("owner");
                Require(!_actors.count(id) && !_targets.count(id), "Duplicate actor id");
                Player* player = GetPlayer(owner);
                Position position = player->GetPosition();
                position.m_positionX += definition.get<float>("distance", 3);
                TempSummon* creature = player->SummonCreature(definition.get<uint32>("entry"), position);
                Require(creature != nullptr, "Could not summon fixture creature: " + id);
                _targets.emplace(id, Target{ creature->GetMapId(), creature->GetInstanceId(), creature->GetGUID() });
                creature->SetPhaseMask(TestPhase, true);
                creature->SetReactState(REACT_PASSIVE);
                creature->SetRegeneratingHealth(false);
                creature->SetFaction(definition.get<uint32>("faction", 14));
                creature->SetLevel(uint8(definition.get<uint32>("level", 80)));
                creature->SetMaxHealth(definition.get<uint32>("health", 100000));
                creature->SetHealth(creature->GetMaxHealth());
                // Summoning runs line-of-sight AI before returning and can already engage nearby actors.
                // End those initial references on both sides before beginning the passive fixture's steps.
                creature->CombatStop(true, true);
                creature->SetReactState(REACT_PASSIVE);
            }
        }
        _targetsCreated = true;
    }

    double Measure(Tree const& step)
    {
        Unit* unit = GetUnit(step.get<std::string>("actor"));
        std::string metric = step.get<std::string>("metric");
        uint32 spell = step.get<uint32>("spell", 0);
        if (metric == "health")
            return unit->GetHealth();
        if (metric == "health_pct")
            return unit->GetHealthPct();
        if (metric == "max_health")
            return unit->GetMaxHealth();
        if (metric == "power" || metric == "max_power" || metric == "pet_power" || metric == "pet_max_power")
        {
            if (metric == "pet_power" || metric == "pet_max_power")
            {
                Require(unit->IsPlayer(), "Pet power query needs a player");
                unit = unit->ToPlayer()->GetPet();
                Require(unit != nullptr, "Pet power query needs a current pet");
            }
            uint32 power = step.get<uint32>("power", POWER_MANA);
            Require(power < MAX_POWERS, "Invalid power index");
            return metric == "power" || metric == "pet_power" ?
                unit->GetPower(Powers(power)) : unit->GetMaxPower(Powers(power));
        }
        if (metric == "alive")
            return unit->IsAlive();
        if (metric == "combat")
            return unit->IsInCombat();
        if (metric == "casting")
            return unit->IsNonMeleeSpellCast(false);
        if (metric == "moving")
            return unit->isMoving();
        if (metric == "forced_forward")
            return unit->HasUnitFlag2(UNIT_FLAG2_FORCE_MOVEMENT);
        if (metric == "cast_pushback_ms")
        {
            Require(unit->IsPlayer(), "Cast pushback observation needs a player");
            return double(_actors.at(step.get<std::string>("actor")).castPushbackMs);
        }
        if (metric == "distance_2d")
            return unit->GetExactDist2d(GetUnit(step.get<std::string>("target")));
        if (metric == "cast_remaining_ms")
        {
            for (CurrentSpellTypes type : {CURRENT_GENERIC_SPELL, CURRENT_CHANNELED_SPELL})
                if (Spell* current = unit->GetCurrentSpell(type))
                    if (current->GetSpellInfo()->Id == spell && current->getState() != SPELL_STATE_FINISHED)
                        return std::max(0, current->GetCastTimeRemaining());
            return 0;
        }
        if (metric == "level")
            return unit->GetLevel();
        if (metric == "stat")
        {
            uint32 stat = step.get<uint32>("stat");
            Require(stat < MAX_STATS, "Invalid stat index");
            return unit->GetStat(Stats(stat));
        }
        if (metric == "attack_power" || metric == "ranged_attack_power")
            return unit->GetTotalAttackPowerValue(metric == "attack_power" ? BASE_ATTACK : RANGED_ATTACK);
        if (metric == "armor")
            return unit->GetArmor();
        if (metric == "weapon_damage_min")
        {
            uint32 hand = step.get<uint32>("hand", BASE_ATTACK);
            Require(hand < MAX_ATTACK, "Invalid weapon damage hand");
            uint16 field = hand == BASE_ATTACK ? UNIT_FIELD_MINDAMAGE :
                (hand == OFF_ATTACK ? UNIT_FIELD_MINOFFHANDDAMAGE : UNIT_FIELD_MINRANGEDDAMAGE);
            return unit->GetFloatValue(field);
        }
        if (metric == "resistance")
        {
            uint32 school = step.get<uint32>("school");
            Require(school > SPELL_SCHOOL_NORMAL && school < MAX_SPELL_SCHOOL, "Invalid resistance school");
            return unit->GetResistance(SpellSchools(school));
        }
        if (metric == "attack_time_ms")
        {
            uint32 hand = step.get<uint32>("hand", BASE_ATTACK);
            Require(hand < MAX_ATTACK, "Invalid attack hand");
            // The update field holds the hasted swing time; GetAttackTime divides haste back out.
            return unit->GetFloatValue(static_cast<uint16>(UNIT_FIELD_BASEATTACKTIME) + hand);
        }
        if (metric == "run_speed_rate")
            return unit->GetSpeedRate(MOVE_RUN);
        if (metric == "spell_damage_taken" || metric == "melee_damage_taken")
        {
            // Any unit can be the victim; `target` is the attacker. A melee `spell` selects a weapon strike.
            Unit* attacker = GetUnit(step.get<std::string>("target"));
            SpellInfo const* info = spell ? sSpellMgr->GetSpellInfo(spell) : nullptr;
            Require(!spell || info != nullptr, "Unknown spell for incoming damage calculation");
            if (metric == "melee_damage_taken")
                return unit->MeleeDamageBonusTaken(attacker, 1000, BASE_ATTACK, info,
                    info ? info->GetSchoolMask() : SPELL_SCHOOL_MASK_NORMAL);
            Require(info != nullptr, "Incoming spell damage needs a spell");
            return unit->SpellDamageBonusTaken(attacker, info, 1000, SPELL_DIRECT_DAMAGE);
        }
        if (metric.rfind("aura", 0) == 0)
        {
            Require(metric == "aura" || metric == "aura_stacks" || metric == "aura_charges"
                || metric == "aura_duration_ms" || metric == "aura_amount" || metric == "aura_positive"
                || metric == "aura_amplitude_ms" || metric == "aura_crit_chance" || metric == "aura_script_value",
                "Unknown aura metric");
            Require(sSpellMgr->GetSpellInfo(spell) != nullptr, "Unknown aura spell");
            ObjectGuid caster;
            if (auto id = step.get_optional<std::string>("caster"))
                caster = GetUnit(*id)->GetGUID();
            Aura* aura = unit->GetAura(spell, caster);
            if (metric == "aura")
                return aura != nullptr;
            if (!aura)
                return 0;
            if (metric == "aura_positive")
            {
                AuraApplication const* application = aura->GetApplicationOfTarget(unit->GetGUID());
                return application && application->IsPositive();
            }
            if (metric == "aura_stacks")
                return aura->GetStackAmount();
            if (metric == "aura_charges")
                return aura->GetCharges();
            if (metric == "aura_duration_ms")
                return aura->GetDuration();
            if (metric == "aura_script_value")
                return double(aura->GetScriptValue(step.get<uint32>("key")));
            uint32 effect = step.get<uint32>("effect", 0);
            Require(effect < MAX_SPELL_EFFECTS && aura->GetEffect(effect), "Aura effect does not exist");
            if (metric == "aura_amplitude_ms")
                return aura->GetEffect(effect)->GetAmplitude();
            if (metric == "aura_crit_chance")
                return aura->GetEffect(effect)->GetCritChance();
            return aura->GetEffect(effect)->GetAmount();
        }
        Player* player = unit->ToPlayer();
        Require(player != nullptr, "Metric requires a player: " + metric);
        if (metric == "knows_spell" || metric == "cooldown_ms" || metric == "has_talent")
            Require(sSpellMgr->GetSpellInfo(spell) != nullptr, "Unknown spell in metric");
        if (metric == "knows_spell")
            return player->HasSpell(spell);
        if (metric == "quest_rewarded")
        {
            uint32 quest = step.get<uint32>("quest");
            Require(sObjectMgr->GetQuestTemplate(quest) != nullptr, "Unknown quest template");
            return player->IsQuestRewarded(quest);
        }
        if (metric == "gossip_options")
            return player->PlayerTalkClass->GetGossipMenu().GetMenuItemCount();
        if (metric == "loot_received")
            return _actors.at(step.get<std::string>("actor")).lootReceived;
        if (metric == "loot_count" || metric == "loot_entry")
        {
            Item* container = player->GetItemByGuid(player->GetLootGUID());
            if (!container)
                return 0;
            uint32 count = 0;
            for (LootItem const& item : container->loot.items)
                if (!item.is_looted)
                {
                    if (metric == "loot_entry")
                        return item.itemid;
                    ++count;
                }
            return count;
        }
        if (metric == "who_count" || metric == "who_class")
        {
            Actor const& actor = _actors.at(step.get<std::string>("actor"));
            Require(actor.whoResponses != 0, "No native Who response received");
            if (metric == "who_count")
                return actor.whoClasses.size();
            auto found = actor.whoClasses.find(_actors.at(step.get<std::string>("target")).name);
            return found == actor.whoClasses.end() ? 0 : found->second;
        }
        if (metric == "cast_speed_multiplier")
            return player->GetFloatValue(UNIT_MOD_CAST_SPEED);
        if (metric == "spell_crit_chance")
        {
            uint32 school = step.get<uint32>("school", SPELL_SCHOOL_SHADOW);
            Require(school < MAX_SPELL_SCHOOL, "Invalid spell school");
            return player->GetFloatValue(PLAYER_SPELL_CRIT_PERCENTAGE1 + school);
        }
        if (metric == "melee_crit_chance")
            return player->GetFloatValue(PLAYER_CRIT_PERCENTAGE);
        if (metric == "dodge_chance")
            return player->GetFloatValue(PLAYER_DODGE_PERCENTAGE);
        if (metric == "parry_chance")
            return player->GetFloatValue(PLAYER_PARRY_PERCENTAGE);
        if (metric == "block_chance")
            return player->GetFloatValue(PLAYER_BLOCK_PERCENTAGE);
        if (metric == "block_value")
            return player->GetShieldBlockValue();
        if (metric == "critical_block_chance")
            return player->GetTotalAuraModifier(SPELL_AURA_MOD_BLOCK_CRIT_CHANCE);
        if (metric == "melee_attack_count" || metric == "melee_damage_count")
        {
            Actor const& actor = _actors.at(step.get<std::string>("actor"));
            auto const& counts = metric == "melee_attack_count" ? actor.meleeAttacksByHand : actor.meleeDamageByHand;
            if (auto hand = step.get_optional<uint32>("hand"))
            {
                Require(*hand < 2, "Melee hand must be main hand or off hand");
                return counts[*hand];
            }
            return counts[BASE_ATTACK] + counts[OFF_ATTACK];
        }
        if (metric == "spell_damage_count" || metric == "spell_damage_total")
        {
            ObjectGuid caster = step.get<bool>("pet", false) ? player->GetPetGUID() : player->GetGUID();
            ObjectGuid target;
            if (auto id = step.get_optional<std::string>("target"))
                target = GetUnit(*id)->GetGUID();
            auto critical = step.get_optional<bool>("critical");
            uint64 value = 0;
            for (SpellDamageEvent const& event : _actors.at(step.get<std::string>("actor")).spellDamage)
                if (caster && event.caster == caster && event.spell == spell &&
                    (!target || event.target == target) && (!critical || event.critical == *critical))
                    value += metric == "spell_damage_count" ? 1 : event.damage;
            return double(value);
        }
        if (metric == "spell_heal_count" || metric == "spell_heal_total" || metric == "spell_effective_heal_total")
        {
            ObjectGuid caster = step.get<bool>("pet", false) ? player->GetPetGUID() : player->GetGUID();
            ObjectGuid target;
            if (auto id = step.get_optional<std::string>("target"))
            {
                Unit* victim = GetUnit(*id);
                if (step.get<bool>("target_pet", false))
                {
                    Player* owner = victim->ToPlayer();
                    Require(owner && owner->GetPet(), "Healing target needs a current pet");
                    victim = owner->GetPet();
                }
                target = victim->GetGUID();
            }
            auto critical = step.get_optional<bool>("critical");
            uint64 value = 0;
            for (SpellHealEvent const& event : _actors.at(step.get<std::string>("actor")).spellHeals)
                if (caster && event.caster == caster && event.spell == spell &&
                    (!target || event.target == target) && (!critical || event.critical == *critical))
                    value += metric == "spell_heal_count" ? 1 :
                        event.heal - (metric == "spell_effective_heal_total" ? event.overheal : 0);
            return double(value);
        }
        if (metric == "spell_energize_count" || metric == "spell_energize_total")
        {
            ObjectGuid caster = step.get<bool>("pet", false) ? player->GetPetGUID() : player->GetGUID();
            ObjectGuid target;
            if (auto id = step.get_optional<std::string>("target"))
            {
                Unit* victim = GetUnit(*id);
                if (step.get<bool>("target_pet", false))
                {
                    Player* owner = victim->ToPlayer();
                    Require(owner && owner->GetPet(), "Energize target needs a current pet");
                    victim = owner->GetPet();
                }
                target = victim->GetGUID();
            }
            auto power = step.get_optional<uint32>("power");
            uint64 value = 0;
            for (SpellEnergizeEvent const& event : _actors.at(step.get<std::string>("actor")).spellEnergizes)
                if (caster && event.caster == caster && event.spell == spell &&
                    (!target || event.target == target) && (!power || event.power == *power))
                    value += metric == "spell_energize_count" ? 1 : event.amount;
            return double(value);
        }
        if (metric == "aoe_damage_taken")
        {
            uint32 school = step.get<uint32>("school");
            Require(school < MAX_SPELL_SCHOOL, "Invalid area damage school");
            return player->CalculateAOEDamageReduction(1000, 1u << school, false);
        }
        if (metric == "reputation_gain")
        {
            uint32 faction = step.get<uint32>("id");
            Require(sFactionStore.LookupEntry(faction) != nullptr, "Unknown reputation faction");
            return player->CalculateReputationGain(REPUTATION_SOURCE_SPELL, player->GetLevel(), 1000, int32(faction));
        }
        if (metric == "spell_immune" || metric == "spell_effect_immune")
        {
            SpellInfo const* info = sSpellMgr->GetSpellInfo(spell);
            Require(info != nullptr, "Unknown immunity probe spell");
            Unit* caster = GetUnit(step.get<std::string>("target"));
            if (metric == "spell_immune")
                return player->IsImmunedToSpell(info, caster);
            uint32 effect = step.get<uint32>("effect", EFFECT_0);
            Require(effect < MAX_SPELL_EFFECTS && info->Effects[effect].IsEffect(), "Invalid immunity probe effect");
            return player->IsImmunedToSpellEffect(info, effect, caster);
        }
        if (metric == "expertise")
            return player->GetUInt32Value(PLAYER_EXPERTISE);
        if (metric == "spell_uses_armor")
        {
            SpellInfo const* info = sSpellMgr->GetSpellInfo(spell);
            Require(info != nullptr, "Unknown spell in armor eligibility probe");
            uint32 effect = step.get<uint32>("effect", EFFECT_0);
            Require(effect < MAX_SPELL_EFFECTS && info->Effects[effect].IsEffect(), "Invalid armor probe effect");
            return Unit::IsDamageReducedByArmor(info->GetSchoolMask(), info, uint8(effect));
        }
        if (metric == "melee_hit_chance")
            return player->m_modMeleeHitChance;
        if (metric == "spell_hit_chance")
            return player->m_modSpellHitChance;
        if (metric == "spell_power")
        {
            uint32 school = step.get<uint32>("school");
            Require(school > SPELL_SCHOOL_NORMAL && school < MAX_SPELL_SCHOOL, "Invalid spell power school");
            return player->SpellBaseDamageBonusDone(SpellSchoolMask(1 << school));
        }
        if (metric == "combat_rating")
        {
            uint32 rating = step.get<uint32>("rating");
            Require(rating < MAX_COMBAT_RATING, "Invalid combat rating");
            return player->GetUInt32Value(static_cast<uint16>(PLAYER_FIELD_COMBAT_RATING_1) + rating);
        }
        if (metric.rfind("script_", 0) == 0)
        {
            // Module damage-taken hooks with a fixed base of 1000 and `target` as the attacker.
            Unit* attacker = GetUnit(step.get<std::string>("target"));
            SpellInfo const* info = sSpellMgr->GetSpellInfo(spell);
            if (metric == "script_melee_damage_taken")
            {
                uint32 damage = 1000;
                sScriptMgr->ModifyMeleeDamage(player, attacker, damage);
                return damage;
            }
            Require(info != nullptr, "Unknown spell for scripted damage taken");
            if (metric == "script_spell_damage_taken")
            {
                int32 damage = 1000;
                sScriptMgr->ModifySpellDamageTaken(player, attacker, damage, info);
                return damage;
            }
            Require(metric == "script_periodic_damage_taken", "Unknown scripted damage metric");
            uint32 damage = 1000;
            sScriptMgr->ModifyPeriodicDamageAurasTick(player, attacker, damage, info);
            return damage;
        }
        if (metric == "spell_done_crit_chance" || metric == "melee_spell_damage_done" ||
            metric == "spell_critical_damage" || metric == "armor_reduced_damage")
        {
            Unit* target = GetUnit(step.get<std::string>("target"));
            SpellInfo const* info = sSpellMgr->GetSpellInfo(spell);
            Require(info != nullptr, "Unknown spell in metric");
            if (metric == "spell_done_crit_chance")
                return player->SpellDoneCritChance(target, info, info->GetSchoolMask(), BASE_ATTACK, false);
            if (metric == "spell_critical_damage")
                return Unit::SpellCriticalDamageBonus(player, info, 1000, target);
            if (metric == "armor_reduced_damage")
            {
                Unit* attacker = step.get<bool>("pet", false) ? static_cast<Unit*>(player->GetPet()) : player;
                Require(attacker != nullptr, "Armor probe needs a current pet");
                return Unit::CalcArmorReducedDamage(attacker, target, 1000, info);
            }
            return player->MeleeDamageBonusDone(target, 1000, BASE_ATTACK, info, info->GetSchoolMask());
        }
        if (metric == "spell_modifier" || metric == "spell_cast_time_ms" || metric == "spell_max_range"
            || metric == "spell_max_stacks" || metric == "spell_healing_done" || metric == "spell_effect_value")
        {
            SpellInfo const* info = sSpellMgr->GetSpellInfo(spell);
            Require(info != nullptr, "Unknown spell in metric");
            // Native modifier consumers without submitting a cast; the probe applies no charges.
            if (metric == "spell_modifier")
            {
                uint32 op = step.get<uint32>("op");
                Require(op < MAX_SPELLMOD, "Invalid spell modifier operation");
                float value = step.get<float>("base");
                player->ApplySpellMod(spell, SpellModOp(op), value);
                return value;
            }
            if (metric == "spell_effect_value")
            {
                uint32 effect = step.get<uint32>("effect", EFFECT_0);
                Require(effect < MAX_SPELL_EFFECTS && info->Effects[effect].IsEffect(), "Spell effect does not exist");
                return info->Effects[effect].CalcValue(player);
            }
            if (metric == "spell_cast_time_ms")
                return info->CalcCastTime(player);
            if (metric == "spell_max_range")
                return info->GetMaxRange(info->IsPositive(), player);
            if (metric == "spell_max_stacks")
                return info->CalcMaxAuraStacks(player);
            if (metric == "spell_healing_done")
                return player->SpellHealingBonusDone(GetUnit(step.get<std::string>("target")), info, 1000,
                    step.get<bool>("periodic", false) ? DOT : HEAL,
                    uint8(step.get<uint32>("effect", EFFECT_0)));
        }
        if (metric == "spell_damage_done" || metric == "melee_damage_done")
        {
            Unit* target = GetUnit(step.get<std::string>("target"));
            if (metric == "melee_damage_done")
                return player->MeleeDamageBonusDone(target, 1000, BASE_ATTACK, nullptr);

            SpellInfo const* info = sSpellMgr->GetSpellInfo(spell);
            Require(info != nullptr, "Unknown spell for damage calculation");
            return player->SpellDamageBonusDone(target, info, 1000,
                step.get<bool>("periodic", false) ? DOT : SPELL_DIRECT_DAMAGE,
                uint8(step.get<uint32>("effect", EFFECT_0)));
        }
        if (metric == "spell_power_cost")
        {
            SpellInfo const* info = sSpellMgr->GetSpellInfo(spell);
            Require(info != nullptr, "Unknown spell for power cost");
            return info->CalcPowerCost(player, info->GetSchoolMask());
        }
        if (metric == "has_talent")
        {
            Require(GetTalentSpellPos(spell) != nullptr, "Metric needs a talent rank's spell ID");
            return player->HasTalent(spell, player->GetActiveSpec());
        }
        if (metric == "talent_points")
            return player->GetFreeTalentPoints();
        if (metric == "bank_bag_slots")
            return player->GetBankBagSlotCount();
        if (metric == "taxi_node")
            return player->m_taxi.IsTaximaskNodeKnown(step.get<uint32>("entry"));
        if (metric == "private_instance")
            return player->GetMap()->IsScriptedPrivateInstance();
        if (metric == "controls_self")
            return player->m_mover == player;
        if (metric == "at_homebind")
            return player->GetMapId() == player->m_homebindMapId &&
                player->GetExactDist(player->m_homebindX, player->m_homebindY, player->m_homebindZ) <= 5.0f;
        if (metric == "owned_gameobject_count" || metric == "gameobject_remaining_ms")
        {
            std::list<GameObject*> objects = OwnedGameObjects(player, step.get<uint32>("entry"));
            if (metric == "owned_gameobject_count")
                return objects.size();
            if (objects.empty())
                return 0;
            Require(objects.size() == 1, "Gameobject lifetime needs exactly one owned object");
            time_t expiry = objects.front()->GetRespawnTime();
            return expiry ? std::max<time_t>(0, expiry - GameTime::GetGameTime().count()) * IN_MILLISECONDS : -1;
        }
        if (metric == "dynamic_object" || metric == "dynamic_object_duration_ms")
        {
            Require(sSpellMgr->GetSpellInfo(spell) != nullptr, "Unknown ground-effect spell");
            DynamicObject* object = player->GetDynObject(spell);
            if (metric == "dynamic_object")
                return object && object->IsInWorld();
            return object ? object->GetDuration() : 0;
        }
        if (metric == "charm_entry" || metric == "charm_aura_stacks")
        {
            Unit* charm = player->GetCharm();
            if (metric == "charm_entry")
                return charm ? charm->GetEntry() : 0;
            Require(sSpellMgr->GetSpellInfo(spell) != nullptr, "Unknown charm aura spell");
            ObjectGuid caster;
            if (auto id = step.get_optional<std::string>("caster"))
                caster = GetUnit(*id)->GetGUID();
            Aura* aura = charm ? charm->GetAura(spell, caster) : nullptr;
            return aura ? aura->GetStackAmount() : 0;
        }
        if (metric == "owned_creature_count")
        {
            uint32 entry = step.get<uint32>("entry");
            Require(sObjectMgr->GetCreatureTemplate(entry) != nullptr, "Unknown creature entry in metric");
            Require(!spell || sSpellMgr->GetSpellInfo(spell) != nullptr, "Unknown owned creature aura spell");
            ObjectGuid caster;
            if (auto id = step.get_optional<std::string>("caster"))
                caster = GetUnit(*id)->GetGUID();
            std::list<Creature*> creatures;
            player->GetCreatureListWithEntryInGrid(creatures, entry, 100.0f);
            return std::count_if(creatures.begin(), creatures.end(), [player, spell, caster](Creature* creature)
            {
                return creature->IsAlive() && creature->GetOwnerGUID() == player->GetGUID()
                    && player->InSamePhase(creature) && (!spell || creature->GetAura(spell, caster));
            });
        }
        if (metric == "pet_entry" || metric == "pet_aura_stacks" || metric == "pet_aura_amount" || metric == "pet_aura_amplitude_ms" ||
            metric == "pet_max_health" || metric == "pet_attack_power" || metric == "pet_run_speed_rate")
        {
            Guardian* pet = player->GetGuardianPet();
            if (metric == "pet_entry")
                return pet ? pet->GetEntry() : 0;
            if (!pet && (metric == "pet_aura_stacks" || metric == "pet_aura_amount" || metric == "pet_aura_amplitude_ms"))
                return 0;
            Require(pet != nullptr, "Metric needs a current pet");
            if (metric == "pet_max_health")
                return pet->GetMaxHealth();
            if (metric == "pet_attack_power")
                return pet->GetTotalAttackPowerValue(BASE_ATTACK);
            if (metric == "pet_run_speed_rate")
                return pet->GetSpeedRate(MOVE_RUN);
            Require(sSpellMgr->GetSpellInfo(spell) != nullptr, "Unknown pet aura spell");
            ObjectGuid caster;
            if (auto id = step.get_optional<std::string>("caster"))
                caster = GetUnit(*id)->GetGUID();
            Aura* aura = pet ? pet->GetAura(spell, caster) : nullptr;
            if ((metric == "pet_aura_amount" || metric == "pet_aura_amplitude_ms") && aura)
            {
                uint32 effect = step.get<uint32>("effect", 0);
                Require(effect < MAX_SPELL_EFFECTS && aura->GetEffect(effect), "Pet aura effect does not exist");
                return metric == "pet_aura_amount" ? aura->GetEffect(effect)->GetAmount() :
                    aura->GetEffect(effect)->GetAmplitude();
            }
            return aura ? aura->GetStackAmount() : 0;
        }
        if (metric == "cooldown_ms")
            return player->GetSpellCooldownDelay(spell);
        if (metric == "item_count")
        {
            uint32 item = step.get<uint32>("item");
            Require(sObjectMgr->GetItemTemplate(item) != nullptr, "Unknown item in metric");
            return player->GetItemCount(item);
        }
        if (metric == "carried_item_count")
        {
            uint32 count = 0;
            for (uint8 slot = EQUIPMENT_SLOT_START; slot < INVENTORY_SLOT_ITEM_END; ++slot)
                if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                    count += item->GetCount();
            for (uint8 bag = INVENTORY_SLOT_BAG_START; bag < INVENTORY_SLOT_BAG_END; ++bag)
                if (Bag* container = player->GetBagByPos(bag))
                    for (uint32 slot = 0; slot < container->GetBagSize(); ++slot)
                        if (Item* item = container->GetItemByPos(uint8(slot)))
                            count += item->GetCount();
            return count;
        }
        if (metric == "quest_status" || metric == "quest_takeable")
        {
            uint32 quest = step.get<uint32>("quest");
            Quest const* questTemplate = sObjectMgr->GetQuestTemplate(quest);
            Require(questTemplate != nullptr, "Unknown quest in metric");
            // Takeable is the game's own answer, prerequisites and all; status is what the log holds.
            return metric == "quest_status" ? double(player->GetQuestStatus(quest))
                : double(player->CanTakeQuest(questTemplate, false));
        }
        if (metric == "quest_objective_count")
        {
            // Progress on one objective (zero-based `index`, default 0) of a quest in the log.
            uint16 slot = player->FindQuestSlot(step.get<uint32>("quest"));
            Require(slot < MAX_QUEST_LOG_SIZE, "Quest is not in the quest log");
            uint32 index = step.get<uint32>("index", 0);
            Require(index < QUEST_OBJECTIVES_COUNT, "Invalid quest objective index");
            return double(player->GetQuestSlotCounter(slot, index));
        }
        if (metric == "dialog_status")
        {
            // The mark the client draws over a quest giver, as the server would send it.
            uint32 entry = step.get<uint32>("entry");
            Require(sObjectMgr->GetCreatureTemplate(entry) != nullptr, "Unknown creature entry in metric");
            Creature* giver = GetGiver(player, entry);
            return giver ? double(player->GetQuestDialogStatus(giver)) : 0.0;
        }
        if (metric == "ball_offer_count" || metric == "ball_offers_quest")
        {
            // The list the wisdomball's gossip is drawn from, for the map the player stands in.
            Require(AscensionWisdomball::UsableBall(player) != nullptr, "No wisdomball is within reach of the player");
            std::vector<uint32> const offered = AscensionWisdomball::OfferedQuests(player);
            if (metric == "ball_offer_count")
                return double(offered.size());
            uint32 quest = step.get<uint32>("quest");
            Require(sObjectMgr->GetQuestTemplate(quest) != nullptr, "Unknown quest in metric");
            return std::count(offered.begin(), offered.end(), quest) ? 1.0 : 0.0;
        }
        if (metric == "ball_carried_count" || metric == "ball_carried_quest"
            || metric == "ball_turn_in_count" || metric == "ball_turn_in_quest")
        {
            // The other half of that list: the dungeon quests the character is carrying, which
            // the frame shows with the "?" wherever the ball is, and the ones ready to hand in.
            Require(AscensionWisdomball::UsableBall(player) != nullptr, "No wisdomball is within reach of the player");
            bool const handIn = metric.rfind("ball_turn_in", 0) == 0;
            std::vector<uint32> const listed = handIn ? AscensionWisdomball::TurnInQuests(player)
                : AscensionWisdomball::CarriedQuests(player);
            if (metric == "ball_carried_count" || metric == "ball_turn_in_count")
                return double(listed.size());
            uint32 quest = step.get<uint32>("quest");
            Require(sObjectMgr->GetQuestTemplate(quest) != nullptr, "Unknown quest in metric");
            return std::count(listed.begin(), listed.end(), quest) ? 1.0 : 0.0;
        }
        if (metric == "gossip_text")
        {
            // The frame's greeting is an npc_text row the server has to be able to send, so a
            // module's own greeting is asserted here rather than only eyeballed in game.
            uint32 id = step.get<uint32>("id");
            return sObjectMgr->GetGossipText(id) != nullptr ? 1.0 : 0.0;
        }
        throw std::runtime_error("Unknown metric: " + metric);
    }

    void RunStep(Tree const& step)
    {
        if (!_stepStarted)
        {
            _stepStarted = true;
            _stepTime = Clock::now();
        }
        std::string action = step.get<std::string>("action");
        Tree record;
        record.put("index", _completed);
        record.put("action", action);
        record.put("label", step.get<std::string>("label", action));
        if (action == "wait")
        {
            if (Elapsed(_stepTime) < step.get<uint32>("ms"))
                return;
        }
        else if (action == "snapshot" || action == "assert")
        {
            double actual = Measure(step);
            if (auto relative = step.get_optional<std::string>("relative_to"))
            {
                Require(_snapshots.count(*relative) != 0, "Unknown snapshot: " + *relative);
                actual -= _snapshots.at(*relative);
            }
            if (auto ratio = step.get_optional<std::string>("ratio_to"))
            {
                Require(_snapshots.count(*ratio) && _snapshots.at(*ratio) != 0, "Missing or zero ratio snapshot");
                actual /= _snapshots.at(*ratio);
            }
            record.put("actual", actual);
            record.put("actor", step.get<std::string>("actor"));
            record.put("metric", step.get<std::string>("metric"));
            if (action == "snapshot")
                _snapshots[step.get<std::string>("save_as")] = actual;
            else
            {
                auto equals = step.get_optional<double>("equals");
                auto minimum = step.get_optional<double>("min");
                auto maximum = step.get_optional<double>("max");
                Require(bool(equals) || bool(minimum) || bool(maximum), "Assertion needs an expected value");
                bool passed = std::isfinite(actual) && (!equals || actual == *equals)
                    && (!minimum || actual >= *minimum) && (!maximum || actual <= *maximum);
                if (equals)
                    record.put("expected_equals", *equals);
                if (minimum)
                    record.put("expected_min", *minimum);
                if (maximum)
                    record.put("expected_max", *maximum);
                if (!passed && Elapsed(_stepTime) < step.get<uint32>("within_ms", 0))
                    return;
                record.put("status", passed ? "passed" : "failed");
                record.put("elapsed_ms", Elapsed(_stepTime));
                _records.push_back({ "", record });
                ++_assertions;
                Require(passed, "Assertion failed: " + record.get<std::string>("label"));
                Advance();
                return;
            }
        }
        else
            Act(step, record);
        record.put("status", "completed");
        record.put("elapsed_ms", Elapsed(_stepTime));
        _records.push_back({ "", record });
        Advance();
    }

    void Act(Tree const& step, Tree& record)
    {
        std::string action = step.get<std::string>("action");
        if (action == "console")
        {
            std::string output;
            CliHandler handler(&output, [](void* context, std::string_view text)
            {
                static_cast<std::string*>(context)->append(text);
            });
            bool handled = handler.ParseCommands(step.get<std::string>("command"));
            record.put("output", output);
            Require(handled && !handler.HasSentErrorMessage(), "Console command failed: " + output);
            return;
        }
        std::string id = step.get<std::string>("actor");
        if (action == "set_health" && !_actors.count(id))
        {
            Unit* creature = GetUnit(id);
            uint32 health = step.get<uint32>("value");
            Require(health > 0 && health <= creature->GetMaxHealth(), "Health fixture outside valid range");
            creature->SetHealth(health);
            return;
        }
        Player* player = GetPlayer(id);
        uint32 spell = step.get<uint32>("spell", 0);
        if (action == "learn" || action == "unlearn" || action == "cast" || action == "cast_charm")
            Require(sSpellMgr->GetSpellInfo(spell) != nullptr, "Unknown spell: " + std::to_string(spell));
        if (action == "set_moving")
        {
            // Fixture state for native cast admission and interruption checks.
            if (step.get<bool>("enabled"))
                player->AddUnitMovementFlag(MOVEMENTFLAG_FORWARD);
            else
                player->RemoveUnitMovementFlag(MOVEMENTFLAG_FORWARD);
        }
        else if (action == "stop_attack")
        {
            WorldPacket packet(CMSG_ATTACKSTOP, 0);
            player->GetSession()->HandleAttackStopOpcode(packet);
        }
        else if (action == "pvp")
        {
            bool enabled = step.get<bool>("enabled");
            WorldPacket packet(CMSG_TOGGLE_PVP, 1);
            packet << enabled;
            player->GetSession()->HandleTogglePvP(packet);
            Require(!enabled || player->IsPvP(), "Native PvP enable request did not flag the player");
            record.put("pvp_active", player->IsPvP());
        }
        else if (action == "group")
        {
            Player* member = GetPlayer(step.get<std::string>("target"));
            Require(member != player && !member->GetGroup(), "Group fixture requires an ungrouped other player");
            Group* group = player->GetGroup();
            if (!group)
            {
                group = new Group();
                if (!group->Create(player))
                {
                    delete group;
                    throw std::runtime_error("Could not create fixture group");
                }
                sGroupMgr->AddGroup(group);
            }
            Require(group->AddMember(member), "Could not join fixture group");
        }
        else if (action == "command")
        {
            ChatHandler handler(player->GetSession());
            bool handled = handler.ParseCommands(step.get<std::string>("command"));
            Require(handled && !handler.HasSentErrorMessage(), "Player command failed");
            record.put("result", "submitted; verify effects with assertions");
        }
        else if (action == "prepare_quest" || action == "reward_quest")
        {
            Quest const* quest = sObjectMgr->GetQuestTemplate(step.get<uint32>("quest"));
            Require(quest != nullptr, "Unknown quest template");
            if (action == "prepare_quest")
            {
                Require(!player->IsActiveQuest(quest->GetQuestId()) && player->CanAddQuest(quest, false),
                    "Cannot prepare quest fixture");
                player->AddQuestAndCheckCompletion(quest, nullptr);
                for (uint8 index = 0; index < QUEST_ITEM_OBJECTIVES_COUNT; ++index)
                    if (quest->RequiredItemId[index] && quest->RequiredItemCount[index])
                    {
                        uint32 held = player->GetItemCount(quest->RequiredItemId[index]);
                        if (held < quest->RequiredItemCount[index])
                            Require(player->AddItem(quest->RequiredItemId[index], quest->RequiredItemCount[index] - held),
                                "Cannot grant quest objective item");
                    }
                // Fixture setup skips objective gameplay unless `complete` is false; reward eligibility and delivery
                // remain native.
                if (step.get<bool>("complete", true))
                    player->CompleteQuest(quest->GetQuestId());
            }
            else
            {
                uint32 choice = step.get<uint32>("choice", 0);
                Require(choice < QUEST_REWARD_CHOICES_COUNT && player->CanRewardQuest(quest, choice, false),
                    "Quest reward eligibility rejected");
                player->RewardQuest(quest, choice, player);
            }
        }
        else if (action == "restore_quest_spells")
            player->learnQuestRewardedSpells();
        else if (action == "login_hooks")
            sScriptMgr->OnPlayerLogin(player);
        else if (action == "open_item")
        {
            Item* item = player->GetItemByEntry(step.get<uint32>("item"));
            Require(item != nullptr, "Item must be granted before opening");
            WorldPacket request(CMSG_OPEN_ITEM, 2);
            request << item->GetBagSlot() << item->GetSlot();
            // WorldSession::Update offers every packet to the packet hooks before its handler.
            if (sScriptMgr->CanPacketReceive(player->GetSession(), request))
                player->GetSession()->HandleOpenItemOpcode(request);
        }
        else if (action == "close_loot")
        {
            WorldPacket request(CMSG_LOOT_RELEASE, 8);
            request << player->GetLootGUID();
            player->GetSession()->HandleLootReleaseOpcode(request);
        }
        else if (action == "collect_loot")
        {
            Item* container = player->GetItemByGuid(player->GetLootGUID());
            Require(container && !container->loot.items.empty(), "No open item loot");
            LootItem const& loot = container->loot.items.front();
            Require(!loot.is_looted && loot.count, "First loot slot is unavailable");
            uint32 entry = loot.itemid;
            uint32 expected = loot.count;
            uint32 before = player->GetItemCount(entry);
            WorldPacket request(CMSG_AUTOSTORE_LOOT_ITEM, 1);
            request << uint8(0);
            player->GetSession()->HandleAutostoreLootItemOpcode(request);
            // Collecting the final reward can destroy the container; retain only copied scalar values.
            uint32 after = player->GetItemCount(entry);
            Require(after == before + expected, "Loot did not reach the player's inventory");
            _actors.at(id).lootReceived = after - before;
            record.put("item", entry);
            record.put("received", after - before);
        }
        else if (action == "gossip_hello")
        {
            ObjectGuid guid = step.get_optional<std::string>("target") ?
                GetUnit(step.get<std::string>("target"))->GetGUID() : player->GetCritterGUID();
            Require(!guid.IsEmpty(), "Gossip needs a target or summoned companion");
            // Clear the previous menu so a rejected hello cannot appear to succeed.
            player->PlayerTalkClass->ClearMenus();
            WorldPacket packet(CMSG_GOSSIP_HELLO, 8);
            packet << guid;
            player->GetSession()->HandleGossipHelloOpcode(packet);
        }
        else if (action == "gossip_select")
        {
            auto const& menu = player->PlayerTalkClass->GetGossipMenu();
            WorldPacket packet(CMSG_GOSSIP_SELECT_OPTION, 16);
            packet << menu.GetSenderGUID() << menu.GetMenuId() << step.get<uint32>("option");
            player->GetSession()->HandleGossipSelectOptionOpcode(packet);
        }
        else if (action == "who")
        {
            // Refresh the production cache now instead of depending on its periodic world timer.
            sWhoListCacheMgr->Update();
            WorldPacket request(CMSG_WHO, 32);
            std::string name;
            if (auto target = step.get_optional<std::string>("target"))
                name = _actors.at(*target).name;
            request << uint32(1) << uint32(255) << name << std::string();
            request << step.get<uint32>("race_mask", UINT32_MAX) << step.get<uint32>("class_mask", UINT32_MAX);
            request << uint32(0) << uint32(0); // no zone or free-text filters
            uint32 before = _actors.at(step.get<std::string>("actor")).whoResponses;
            player->GetSession()->HandleWhoOpcode(request);
            Require(_actors.at(step.get<std::string>("actor")).whoResponses == before + 1,
                "Who request did not produce a native response");
        }
        else if (action == "attack")
        {
            Unit* target = GetUnit(step.get<std::string>("target"));
            Require(player->IsValidAttackTarget(target), "Invalid melee attack target");
            if (step.get<bool>("pet", false))
            {
                Pet* pet = player->GetPet();
                Require(pet != nullptr, "Pet attack requires a current pet");
                WorldPacket packet(CMSG_PET_ACTION, 20);
                packet << pet->GetGUID() << uint32(COMMAND_ATTACK | (uint32(ACT_COMMAND) << 24)) << target->GetGUID();
                player->GetSession()->HandlePetAction(packet);
            }
            else
            {
                WorldPacket packet(CMSG_ATTACKSWING, 8);
                packet << target->GetGUID();
                player->GetSession()->HandleAttackSwingOpcode(packet);
            }
        }
        else if (action == "set_aura")
        {
            Unit* recipient = player;
            if (step.get<bool>("pet", false))
                recipient = player->GetGuardianPet();
            Require(recipient != nullptr, "Pet aura fixture requires a current pet");
            SpellInfo const* info = sSpellMgr->GetSpellInfo(spell);
            Require(info != nullptr, "Unknown fixture aura");
            uint32 stacks = step.get<uint32>("stacks");
            Require(stacks <= std::max<uint32>(1, info->CalcMaxAuraStacks(recipient)),
                "Fixture aura exceeds its stack limit");
            if (!stacks)
                recipient->RemoveAurasDueToSpell(spell);
            else
            {
                Aura* aura = recipient->GetAura(spell);
                if (!aura)
                    aura = recipient->AddAura(spell, recipient);
                Require(aura != nullptr, "Could not apply fixture aura");
                aura->SetStackAmount(uint8(stacks));
            }
        }
        else if (action == "learn")
        {
            player->learnSpell(spell);
            Require(player->HasSpell(spell), "Spell learning failed");
        }
        else if (action == "unlearn")
            player->removeSpell(spell, player->GetActiveSpecMask(), false);
        else if (action == "talent")
        {
            uint32 rank = step.get<uint32>("rank");
            auto* talent = sTalentStore.LookupEntry(step.get<uint32>("talent"));
            Require(talent && rank < MAX_TALENT_RANK && talent->RankID[rank], "Invalid talent/rank");
            player->LearnTalent(talent->TalentID, rank); // normal points and prerequisite checks
            Require(player->HasTalent(talent->RankID[rank], player->GetActiveSpec()), "Talent learning rejected");
        }
        else if (action == "reset_talents")
        {
            player->resetTalents(true); // fixture reset through normal removal, without a trainer fee
            Require(player->GetFreeTalentPoints() == player->CalculateTalentsPoints(), "Talent reset rejected");
        }
        else if (action == "cast" || action == "cast_charm" || action == "use_item")
        {
            SpellCastTargets targets;
            Unit* caster = action == "cast_charm" ? player->GetCharm() : player;
            Require(caster != nullptr, "Player has no charmed unit");
            Unit* target = step.get_optional<std::string>("target") ? GetUnit(step.get<std::string>("target")) : caster;
            targets.SetUnitTarget(target);
            if (auto destination = step.get_child_optional("destination"))
                targets.SetDst(destination->get<float>("x"), destination->get<float>("y"),
                    destination->get<float>("z"), caster->GetOrientation());
            record.put("spell_active", caster->IsPlayer() ? caster->ToPlayer()->HasActiveSpell(spell) :
                caster->HasSpell(spell));
            record.put("line_of_sight", caster->IsWithinLOSInMap(target));
            record.put("target_visible", caster->CanSeeOrDetect(target));
            record.put("target_friendly", caster->IsFriendlyTo(target));
            record.put("caster_faction", caster->GetFaction());
            record.put("target_faction", target->GetFaction());
            if (SpellInfo const* info = sSpellMgr->GetSpellInfo(spell))
                record.put("target_check", uint32(info->CheckTarget(caster, target, false)));
            WorldPacket packet(action == "cast" ? CMSG_CAST_SPELL :
                action == "cast_charm" ? CMSG_PET_CAST_SPELL : CMSG_USE_ITEM, 64);
            if (action == "cast" || action == "cast_charm")
            {
                if (action == "cast_charm")
                    packet << caster->GetGUID();
                packet << uint8(++_castCount) << spell << uint8(0);
            }
            else
            {
                Item* item = player->GetItemByEntry(step.get<uint32>("item"));
                Require(item != nullptr, "Item is missing");
                packet << item->GetBagSlot() << item->GetSlot() << uint8(++_castCount) << spell;
                packet << item->GetGUID() << uint32(0) << uint8(0);
            }
            targets.Write(packet);
            if (action == "cast")
                player->GetSession()->HandleCastSpellOpcode(packet);
            else if (action == "cast_charm")
                player->GetSession()->HandlePetCastSpellOpcode(packet);
            else
                player->GetSession()->HandleUseItemOpcode(packet);
            record.put("result", "submitted; verify effects with assertions");
        }
        else if (action == "use_gameobject")
        {
            std::list<GameObject*> objects = OwnedGameObjects(player, step.get<uint32>("entry"));
            Require(objects.size() == 1, "Gameobject use needs exactly one owned object");
            WorldPacket packet(CMSG_GAMEOBJ_USE, 8);
            packet << objects.front()->GetGUID();
            player->GetSession()->HandleGameObjectUseOpcode(packet);
            record.put("result", "submitted; verify effects with assertions");
        }
        else if (action == "add_item")
            Require(player->AddItem(step.get<uint32>("item"), step.get<uint32>("count", 1)), "Item grant failed");
        else if (action == "equip")
        {
            Item* item = player->GetItemByEntry(step.get<uint32>("item"));
            Require(item != nullptr, "Item must be granted before equipping");
            uint32 slot = step.get<uint32>("slot");
            Require(slot < EQUIPMENT_SLOT_END, "Invalid equipment slot");
            WorldPacket packet(CMSG_AUTOEQUIP_ITEM_SLOT, 9);
            packet << item->GetGUID() << uint8(slot);
            WorldPackets::Item::AutoEquipItemSlot request(std::move(packet));
            request.Read();
            Require(request.ItemGuid == item->GetGUID() && request.DestinationSlot == slot,
                "Equipment packet did not round-trip");
            player->GetSession()->HandleAutoEquipItemSlotOpcode(request);
            if (player->GetItemByPos(INVENTORY_SLOT_BAG_0, uint8(slot)) != item)
            {
                uint16 destination = 0;
                InventoryResult equip = player->CanEquipItem(uint8(slot), destination, item, true);
                InventoryResult unequip = player->CanUnequipItem(uint16(INVENTORY_SLOT_BAG_0 << 8) | slot, true);
                throw std::runtime_error("Equipment change rejected: equip error " + std::to_string(equip)
                    + ", unequip error " + std::to_string(unequip) + ", combat "
                    + std::to_string(player->IsInCombat()) + ", casting "
                    + std::to_string(player->IsNonMeleeSpellCast(false)));
            }
        }
        else if (action == "set_level")
        {
            uint32 const level = step.get<uint32>("value");
            Require(level >= 1 && level <= 80, "Invalid fixture level");
            player->GiveLevel(uint8(level));
        }
        else if (action == "set_health")
        {
            Unit* target = player;
            if (step.get<bool>("pet", false))
            {
                target = player->GetPet();
                Require(target != nullptr, "Health fixture needs a current pet");
            }
            uint32 health = step.get<uint32>("value");
            Require(health > 0 && health <= target->GetMaxHealth(), "Health fixture outside valid range");
            target->SetHealth(health);
        }
        else if (action == "set_power")
        {
            Unit* target = player;
            if (step.get<bool>("pet", false))
            {
                target = player->GetPet();
                Require(target != nullptr, "Power fixture needs a current pet");
            }
            uint32 power = step.get<uint32>("power", POWER_MANA);
            Require(power < MAX_POWERS, "Invalid power index");
            uint32 value = step.get<uint32>("value");
            Require(value <= target->GetMaxPower(Powers(power)), "Power fixture exceeds maximum");
            target->SetPower(Powers(power), value);
        }
        else if (action == "teleport")
        {
            uint32 map = step.get<uint32>("map");
            float x = step.get<float>("x");
            float y = step.get<float>("y");
            float z = step.get<float>("z");
            float o = step.get<float>("o", 0.0f);
            Require(sMapStore.LookupEntry(map) != nullptr, "Unknown map to teleport to");
            player->TeleportTo(map, x, y, z, o);
            record.put("result", "teleport sent");
        }
        else if (action == "quest_accept" || action == "quest_turn_in")
        {
            uint32 quest = step.get<uint32>("quest");
            Require(sObjectMgr->GetQuestTemplate(quest) != nullptr, "Unknown quest");

            Creature* giver = GetGiver(player, step.get<uint32>("entry"));
            Require(giver != nullptr, "No giver of that entry is within reach of the player");

            // The server's packet loop asks the receive hook first and only reaches the opcode
            // handler when nothing consumed the packet. Dispatching the same way, with the same
            // hook, is what makes these steps exercise the giver's own answer.
            if (action == "quest_turn_in")
            {
                WorldPacket request(CMSG_QUESTGIVER_REQUEST_REWARD, 16);
                request << giver->GetGUID() << quest;
                _actors.at(id).lastQuestWindow = 0;
                bool const openToCore = sScriptMgr->CanPacketReceive(player->GetSession(), request);
                record.put("request_handled_by_script", !openToCore);
                if (openToCore)
                    player->GetSession()->HandleQuestgiverRequestRewardOpcode(request);

                // Claiming the reward has to open the reward window. Answering it with the
                // progress page instead is a loop: that page's button sends this same opcode, so
                // the player can never hand the quest in.
                if (!openToCore)
                {
                    uint32 const window = _actors.at(id).lastQuestWindow;
                    record.put("window_after_claim", window == SMSG_QUESTGIVER_OFFER_REWARD
                        ? "SMSG_QUESTGIVER_OFFER_REWARD" : "not the reward window");
                    Require(window == SMSG_QUESTGIVER_OFFER_REWARD,
                        "Claiming the reward did not open the reward window");
                }

                WorldPacket choose(CMSG_QUESTGIVER_CHOOSE_REWARD, 16);
                choose << giver->GetGUID() << quest << step.get<uint32>("reward", 0);
                bool const chooseToCore = sScriptMgr->CanPacketReceive(player->GetSession(), choose);
                record.put("reward_handled_by_script", !chooseToCore);
                if (chooseToCore)
                    player->GetSession()->HandleQuestgiverChooseRewardOpcode(choose);
            }
            else
            {
                WorldPacket packet(CMSG_QUESTGIVER_ACCEPT_QUEST, 16);
                packet << giver->GetGUID() << quest << uint8(0);
                bool const openToCore = sScriptMgr->CanPacketReceive(player->GetSession(), packet);
                record.put("accept_handled_by_script", !openToCore);
                if (openToCore)
                    player->GetSession()->HandleQuestgiverAcceptQuestOpcode(packet);
            }

            record.put("result", "dispatched as the server's packet loop does");
        }
        else if (action == "quest_open")
        {
            uint32 quest = step.get<uint32>("quest");
            Require(sObjectMgr->GetQuestTemplate(quest) != nullptr, "Unknown quest");

            Creature* giver = GetGiver(player, step.get<uint32>("entry"));
            Require(giver != nullptr, "No giver of that entry is within reach of the player");

            // A click on a name the ball only offers arrives as this packet. The window it
            // answers with is the client's to draw, so what is asserted here is that the ball's
            // own script answered it: the core would refuse it, the ball owns no quest relations.
            WorldPacket packet(CMSG_QUESTGIVER_QUERY_QUEST, 16);
            packet << giver->GetGUID() << quest << uint8(0);
            bool const openToCore = sScriptMgr->CanPacketReceive(player->GetSession(), packet);
            record.put("query_handled_by_script", !openToCore);
            Require(!openToCore, "A click on a listed name reached the core instead of the giver's script");
        }
        else if (action == "quest_click")
        {
            // The opcode the client really sends when a name in the frame's active half - a quest
            // the character already carries - is clicked. It is not the query above, and the core
            // drops it for the ball, so this is the path an in-game click on such a name takes.
            uint32 quest = step.get<uint32>("quest");
            Require(sObjectMgr->GetQuestTemplate(quest) != nullptr, "Unknown quest");

            Creature* giver = GetGiver(player, step.get<uint32>("entry"));
            Require(giver != nullptr, "No giver of that entry is within reach of the player");

            WorldPacket packet(CMSG_QUESTGIVER_COMPLETE_QUEST, 16);
            packet << giver->GetGUID() << quest;
            _actors.at(id).lastQuestWindow = 0;
            bool const clickToCore = sScriptMgr->CanPacketReceive(player->GetSession(), packet);
            record.put("click_handled_by_script", !clickToCore);
            Require(!clickToCore, "A click on a carried name reached the core instead of the giver's script");

            // A carried name answers with one of the two carried windows - the progress page while
            // it is unfinished or short of items, the reward window once it can be paid out -
            // never with nothing at all, which is what a click that reaches the core does.
            uint32 const window = _actors.at(id).lastQuestWindow;
            record.put("window_after_click", window == SMSG_QUESTGIVER_OFFER_REWARD ? "SMSG_QUESTGIVER_OFFER_REWARD"
                : window == SMSG_QUESTGIVER_REQUEST_ITEMS ? "SMSG_QUESTGIVER_REQUEST_ITEMS" : "no window");
            Require(window == SMSG_QUESTGIVER_OFFER_REWARD || window == SMSG_QUESTGIVER_REQUEST_ITEMS,
                "A click on a carried name was answered with no quest window");
        }
        else if (action == "quest_complete")
        {
            uint32 quest = step.get<uint32>("quest");
            Require(sObjectMgr->GetQuestTemplate(quest) != nullptr, "Unknown quest");
            player->CompleteQuest(quest); // fixture: finish the objectives so a hand-in can be tested
            record.put("result", "quest marked complete as a fixture");
        }
        else
            throw std::runtime_error("Unknown action: " + action);
    }

    void Advance()
    {
        ++_nextStep;
        ++_completed;
        _stepStarted = false;
    }

    void Finish(bool passed, std::string const& message)
    {
        if (_finished)
            return;
        _finished = true;
        if (!passed)
            LOG_ERROR("module.gameplay_test", "Scenario failed at step {}: {}", _completed, message);
        Tree failures;
        for (auto const& [id, actor] : _actors)
            for (auto const& entry : actor.castFailures)
            {
                Tree failure = entry.second;
                failure.put("actor", id);
                failures.push_back({"", failure});
            }
        if (!failures.empty())
            _report.add_child("cast_failures", failures);
        // Normal logout tears down auras, summons, map membership and script state before maps unload.
        for (auto const& [id, target] : _targets)
            if (Map* map = sMapMgr->FindMap(target.map, target.instance))
                if (Creature* creature = map->GetCreature(target.guid))
                    creature->DespawnOrUnsummon();
        for (auto& [id, actor] : _actors)
            if (actor.session && actor.session->GetPlayer())
                actor.session->LogoutPlayer(false);
        _actors.clear();
        _report.put("status", passed ? "passed" : "failed");
        _report.put("message", message);
        _report.put("elapsed_ms", Elapsed(_started));
        _report.put("assertions", _assertions);
        _report.put("completed_steps", _completed);
        _report.add_child("steps", _records);
        try
        {
            WriteResult(_resultPath, _report);
        }
        catch (std::exception const& error)
        {
            passed = false;
            LOG_ERROR("module.gameplay_test", "Could not write gameplay result: {}", error.what());
        }
        LOG_INFO("module.gameplay_test", "Gameplay test {}: {}", passed ? "passed" : "failed", message);
        World::StopNow(passed ? SHUTDOWN_EXIT_CODE : ERROR_EXIT_CODE);
    }

    bool _enabled = false;
    bool _finished = false;
    bool _targetsCreated = false;
    bool _stepStarted = false;
    uint8 _castCount = 0;
    uint32 _timeout = 90000;
    uint32 _assertions = 0;
    uint32 _completed = 0;
    std::string _runId;
    std::string _resultPath;
    std::string _startFile;
    Clock::time_point _started;
    Clock::time_point _stepTime;
    Tree _scenario;
    Tree _steps;
    Tree::const_iterator _nextStep;
    Tree _report;
    Tree _records;
    std::map<std::string, Actor> _actors;
    std::map<std::string, Target> _targets;
    std::map<std::string, double> _snapshots;
    QueryCallbackProcessor _queries;
};
}

void AddCoAGameplayTestScripts()
{
    new CoAGameplayTest();
}
