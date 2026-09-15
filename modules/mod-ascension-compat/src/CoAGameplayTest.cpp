/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license:
 * https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "AccountMgr.h"
#include "AsyncCallbackProcessor.h"
#include "CharacterCache.h"
#include "Chat.h"
#include "Config.h"
#include "Creature.h"
#include "DBCStores.h"
#include "DatabaseEnv.h"
#include "GitRevision.h"
#include "Item.h"
#include "ItemPackets.h"
#include "Log.h"
#include "Map.h"
#include "MapMgr.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
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
#include <boost/property_tree/json_parser.hpp>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <list>
#include <limits>
#include <map>
#include <memory>
#include <stdexcept>

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

struct Actor
{
    Tree definition;
    std::string account;
    std::string name;
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
            Require(connection.substr(last + 1) == "coa_test_" + _runId + "_" + suffix,
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
                    location->get<float>("y"), location->get<float>("z"), location->get<float>("o", 0)),
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
        if (metric == "max_health")
            return unit->GetMaxHealth();
        if (metric == "power" || metric == "max_power")
        {
            uint32 power = step.get<uint32>("power", POWER_MANA);
            Require(power < MAX_POWERS, "Invalid power index");
            return metric == "power" ? unit->GetPower(Powers(power)) : unit->GetMaxPower(Powers(power));
        }
        if (metric == "alive")
            return unit->IsAlive();
        if (metric == "combat")
            return unit->IsInCombat();
        if (metric == "casting")
            return unit->IsNonMeleeSpellCast(false);
        if (metric == "level")
            return unit->GetLevel();
        if (metric.rfind("aura", 0) == 0)
        {
            Require(metric == "aura" || metric == "aura_stacks" || metric == "aura_charges"
                || metric == "aura_duration_ms" || metric == "aura_amount", "Unknown aura metric");
            Require(sSpellMgr->GetSpellInfo(spell) != nullptr, "Unknown aura spell");
            ObjectGuid caster;
            if (auto id = step.get_optional<std::string>("caster"))
                caster = GetUnit(*id)->GetGUID();
            Aura* aura = unit->GetAura(spell, caster);
            if (metric == "aura")
                return aura != nullptr;
            if (!aura)
                return 0;
            if (metric == "aura_stacks")
                return aura->GetStackAmount();
            if (metric == "aura_charges")
                return aura->GetCharges();
            if (metric == "aura_duration_ms")
                return aura->GetDuration();
            if (metric == "aura_amount")
            {
                uint32 effect = step.get<uint32>("effect", 0);
                Require(effect < MAX_SPELL_EFFECTS && aura->GetEffect(effect), "Aura effect does not exist");
                return aura->GetEffect(effect)->GetAmount();
            }
        }
        Player* player = unit->ToPlayer();
        Require(player != nullptr, "Metric requires a player: " + metric);
        if (metric == "knows_spell" || metric == "cooldown_ms" || metric == "has_talent")
            Require(sSpellMgr->GetSpellInfo(spell) != nullptr, "Unknown spell in metric");
        if (metric == "knows_spell")
            return player->HasSpell(spell);
        if (metric == "has_talent")
        {
            Require(GetTalentSpellPos(spell) != nullptr, "Metric needs a talent rank's spell ID");
            return player->HasTalent(spell, player->GetActiveSpec());
        }
        if (metric == "talent_points")
            return player->GetFreeTalentPoints();
        if (metric == "bank_bag_slots")
            return player->GetBankBagSlotCount();
        if (metric == "owned_creature_count")
        {
            uint32 entry = step.get<uint32>("entry");
            Require(sObjectMgr->GetCreatureTemplate(entry) != nullptr, "Unknown creature entry in metric");
            std::list<Creature*> creatures;
            player->GetCreatureListWithEntryInGrid(creatures, entry, 100.0f);
            return std::count_if(creatures.begin(), creatures.end(), [player](Creature* creature)
            {
                return creature->IsAlive() && creature->GetOwnerGUID() == player->GetGUID()
                    && player->InSamePhase(creature);
            });
        }
        if (metric == "pet_entry" || metric == "pet_aura_stacks")
        {
            Guardian* pet = player->GetGuardianPet();
            if (metric == "pet_entry")
                return pet ? pet->GetEntry() : 0;
            Require(sSpellMgr->GetSpellInfo(spell) != nullptr, "Unknown pet aura spell");
            ObjectGuid caster;
            if (auto id = step.get_optional<std::string>("caster"))
                caster = GetUnit(*id)->GetGUID();
            Aura* aura = pet ? pet->GetAura(spell, caster) : nullptr;
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
        Player* player = GetPlayer(id);
        uint32 spell = step.get<uint32>("spell", 0);
        if (action == "learn" || action == "unlearn" || action == "cast")
            Require(sSpellMgr->GetSpellInfo(spell) != nullptr, "Unknown spell: " + std::to_string(spell));
        if (action == "learn")
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
        else if (action == "cast" || action == "use_item")
        {
            SpellCastTargets targets;
            Unit* target = GetUnit(step.get<std::string>("target", id));
            targets.SetUnitTarget(target);
            record.put("spell_active", player->HasActiveSpell(spell));
            record.put("line_of_sight", player->IsWithinLOSInMap(target));
            WorldPacket packet(action == "cast" ? CMSG_CAST_SPELL : CMSG_USE_ITEM, 64);
            if (action == "cast")
                packet << uint8(++_castCount) << spell << uint8(0);
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
            else
                player->GetSession()->HandleUseItemOpcode(packet);
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
        else if (action == "set_health")
        {
            uint32 health = step.get<uint32>("value");
            Require(health > 0 && health <= player->GetMaxHealth(), "Health fixture outside valid range");
            player->SetHealth(health);
        }
        else if (action == "set_power")
        {
            uint32 power = step.get<uint32>("power", POWER_MANA);
            Require(power < MAX_POWERS, "Invalid power index");
            uint32 value = step.get<uint32>("value");
            Require(value <= player->GetMaxPower(Powers(power)), "Power fixture exceeds maximum");
            player->SetPower(Powers(power), value);
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
