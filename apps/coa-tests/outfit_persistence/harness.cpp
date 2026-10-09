#include <algorithm>
#include <array>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <functional>
#include <future>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using TransactionFuture = std::future<bool>;
using namespace std::chrono_literals;

// ACTUAL_CONSTANTS
// ACTUAL_TRANSACTION_CALLBACK
// ACTUAL_INVOKE_CALLBACK
// ACTUAL_CALLBACK_PROCESSOR

enum CharacterDatabaseStatements
{
    CHAR_REP_APPEARANCE_OUTFIT,
    CHAR_DEL_APPEARANCE_OUTFIT
};

struct CharacterDatabasePreparedStatement
{
    CharacterDatabaseStatements id;
    uint32 guid = 0;
    std::string name;
    std::string appearances;

    void SetData(uint32, uint32 value) { guid = value; }
    void SetData(uint32 index, std::string const& value)
    {
        (index == 1 ? name : appearances) = value;
    }
};

struct Transaction
{
    std::unique_ptr<CharacterDatabasePreparedStatement> statement;
    void Append(CharacterDatabasePreparedStatement* value) { statement.reset(value); }
};

using CharacterDatabaseTransaction = std::shared_ptr<Transaction>;
using OutfitKey = std::pair<uint32, std::string>;
using StoredOutfits = std::map<OutfitKey, std::string>;

struct Database
{
    StoredOutfits persisted;
    uint32 queued = 0;
    uint32 transactions = 0;
    bool fail = false;
    bool hold = false;
    std::mutex mutex;
    std::condition_variable submitted;

    struct Job
    {
        CharacterDatabaseTransaction transaction;
        std::promise<bool> result;
    };
    std::deque<Job> jobs;

    void EscapeString(std::string&) { }

    template <typename... Args>
    void Execute(std::string const&, Args const&...) { ++queued; }

    CharacterDatabasePreparedStatement* GetPreparedStatement(CharacterDatabaseStatements id)
    {
        return new CharacterDatabasePreparedStatement{id, 0, {}, {}};
    }

    CharacterDatabaseTransaction BeginTransaction() { return std::make_shared<Transaction>(); }

    void CompleteNext()
    {
        Job job;
        {
            std::lock_guard lock(mutex);
            job = std::move(jobs.front());
            jobs.pop_front();
        }
        if (!fail)
        {
            auto const& statement = *job.transaction->statement;
            OutfitKey key{statement.guid, statement.name};
            if (statement.id == CHAR_REP_APPEARANCE_OUTFIT)
                persisted[key] = statement.appearances;
            else
                persisted.erase(key);
        }
        job.result.set_value(!fail);
    }

    TransactionCallback AsyncCommitTransaction(CharacterDatabaseTransaction const& transaction)
    {
        std::promise<bool> result;
        auto future = result.get_future();
        {
            std::lock_guard lock(mutex);
            ++transactions;
            jobs.push_back({transaction, std::move(result)});
        }
        submitted.notify_one();
        if (!hold)
            CompleteNext();
        return TransactionCallback(std::move(future));
    }
} CharacterDatabase;

struct WorldPacket
{
    uint16 opcode;
    std::vector<std::variant<std::string, uint32>> fields;
    std::size_t position = 0;

    WorldPacket(uint16 value = 0, uint32 = 0) : opcode(value) { }
    uint16 GetOpcode() const { return opcode; }

    WorldPacket& operator<<(char const* value) { fields.emplace_back(std::string(value)); return *this; }
    WorldPacket& operator<<(std::string const& value) { fields.emplace_back(value); return *this; }
    WorldPacket& operator<<(uint32 value) { fields.emplace_back(value); return *this; }

    template <typename T>
    WorldPacket& operator>>(T& value)
    {
        value = std::get<T>(fields.at(position++));
        return *this;
    }
};

struct Player;

struct WorldSession
{
    Player* player = nullptr;
    std::string response;
    StoredOutfits persistedAtResponse;
    AsyncCallbackProcessor<TransactionCallback> callbacks;

    Player* GetPlayer() const { return player; }
    uint32 GetAccountId() const { return 1; }
    TransactionCallback& AddTransactionCallback(TransactionCallback&& callback)
    {
        return callbacks.AddCallback(std::move(callback));
    }

    void SendPacket(WorldPacket const* packet)
    {
        response = std::get<std::string>(packet->fields.front());
        persistedAtResponse = CharacterDatabase.persisted;
    }
};

struct ObjectGuid
{
    uint32 value;
    uint32 GetCounter() const { return value; }
    bool operator==(ObjectGuid const&) const = default;
};

struct Player
{
    ObjectGuid guid{1};
    WorldSession session;

    Player() { session.player = this; }

    ObjectGuid GetGUID() const { return guid; }
    WorldSession* GetSession() { return &session; }
};

// ACTUAL_COLLECTION_STATE

std::shared_ptr<PlayerCollectionState> NewState()
{
    auto state = std::make_shared<PlayerCollectionState>();
    state->CollectedAppearances = {42, 99};
    return state;
}

bool ReceivesClientRequests(Player*) { return true; }

struct CollectionService
{
    std::shared_ptr<PlayerCollectionState> state = NewState();
    std::mutex _packetMutex;
    std::map<uint32, std::deque<WorldPacket>> _pendingPackets;
    std::shared_ptr<PlayerCollectionState> GetState(Player*) { return state; }
    static std::size_t ExpectedReplies(WorldPacket const&) { return 1; }
    void ProcessPendingAppearanceAdds(Player*, uint32) { }
    void ProcessPendingCompanionSpells(Player*, uint32) { }
    void ProcessCompanionLoot(Player*, uint32, bool = false) { }
    void RefreshCosmetics(Player*, PlayerCollectionState&) { }
    void HandleClientPacket(Player* player, WorldPacket& packet)
    {
        if (packet.GetOpcode() == CMSG_SAVE_APPEARANCE_OUTFIT)
            HandleSaveOutfit(player, packet);
        else if (packet.GetOpcode() == CMSG_DELETE_APPEARANCE_OUTFIT)
            HandleDeleteOutfit(player, packet);
    }

    // ACTUAL_SEND_RESULT
    // ACTUAL_VALID_NAME
    // ACTUAL_SAVE
    // ACTUAL_DELETE
    // ACTUAL_TAKE_CLIENT_PACKETS
    // ACTUAL_UPDATE
};

void Require(bool condition, char const* message)
{
    if (!condition)
    {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}

int main()
{
    CollectionService service;
    Player player;
    std::string const name = "Knight's armor";
    OutfitKey const key{1, name};
    OutfitKey const otherCharacter{2, name};
    CharacterDatabase.persisted[otherCharacter] = "99";

    auto save = [&](std::string const& outfit, std::vector<uint32> const& appearances)
    {
        WorldPacket packet;
        packet << outfit << uint32(appearances.size());
        for (uint32 appearance : appearances)
            packet << appearance;
        service.HandleSaveOutfit(&player, packet);
        player.session.callbacks.ProcessReadyCallbacks();
    };
    auto remove = [&]()
    {
        WorldPacket packet;
        packet << name;
        service.HandleDeleteOutfit(&player, packet);
        player.session.callbacks.ProcessReadyCallbacks();
    };

    CharacterDatabase.hold = true;
    WorldPacket delayed;
    delayed << "Delayed outfit" << uint32(1) << uint32(42);
    auto handler = std::async(std::launch::async, [&]() { service.HandleSaveOutfit(&player, delayed); });
    bool enqueued;
    {
        std::unique_lock lock(CharacterDatabase.mutex);
        enqueued = CharacterDatabase.submitted.wait_for(lock, 2s, []() { return !CharacterDatabase.jobs.empty(); });
    }
    Require(enqueued, "the delayed save reaches the database worker");
    bool const returnedBeforeCommit = handler.wait_for(1s) == std::future_status::ready;
    if (returnedBeforeCommit)
    {
        player.session.callbacks.ProcessReadyCallbacks();
        Require(player.session.response.empty(), "a pending commit cannot send a success or failure reply");
        Require(!CharacterDatabase.persisted.contains({1, "Delayed outfit"}), "the delayed transaction is still uncommitted");
        Require(!service.state->Outfits.contains("Delayed outfit"), "a pending save cannot update the cache");
    }
    CharacterDatabase.CompleteNext();
    handler.get();
    player.session.callbacks.ProcessReadyCallbacks();
    Require(returnedBeforeCommit, "a pending database commit must not block the player update handler");
    CharacterDatabase.hold = false;
    service.state = NewState();
    player.session.response.clear();
    CharacterDatabase.persisted.erase({1, "Delayed outfit"});

    save(name, {42, 0, 99});
    Require(player.session.response == "SAVE_APPEARANCE_OUTFIT_OK", "a valid save succeeds");
    Require(player.session.persistedAtResponse.contains(key), "save success requires a committed outfit");
    Require(player.session.persistedAtResponse.at(key) == "42 0 99", "all appearance categories commit before success");
    CharacterDatabase.queued = 0;
    service.state = NewState();
    Require(CharacterDatabase.persisted.at(key) == "42 0 99", "a crash after success cannot discard the saved outfit");

    save(name, {99});
    Require(player.session.persistedAtResponse.at(key) == "99", "replacement is durable before its acknowledgement");
    Require(service.state->Outfits.at(name) == std::vector<uint32>{99}, "replacement updates memory after commit");
    Require(CharacterDatabase.persisted.at(otherCharacter) == "99", "another character's outfit is preserved");

    uint32 const commits = CharacterDatabase.transactions;
    save("Stolen", {4000000000});
    Require(player.session.response == "SAVE_APPEARANCE_OUTFIT_UNKNOWN", "uncollected appearances are rejected");
    save("", {42});
    Require(player.session.response == "SAVE_APPEARANCE_OUTFIT_UNKNOWN", "empty outfit names are rejected");
    Require(CharacterDatabase.transactions == commits, "invalid requests do not reach the database");

    CharacterDatabase.fail = true;
    save(name, {42});
    Require(player.session.response == "SAVE_APPEARANCE_OUTFIT_UNKNOWN", "failed saves are not acknowledged as successful");
    Require(service.state->Outfits.at(name) == std::vector<uint32>{99}, "failed replacements preserve the in-memory outfit");
    Require(CharacterDatabase.persisted.at(key) == "99", "failed replacements preserve the committed outfit");
    save("New outfit", {42});
    Require(!service.state->Outfits.contains("New outfit"), "a failed save cannot create a phantom outfit");

    remove();
    Require(player.session.response == "DELETE_APPEARANCE_OUTFIT_UNKNOWN", "failed deletes are not acknowledged as successful");
    Require(service.state->Outfits.contains(name), "a failed delete preserves the in-memory outfit");
    Require(CharacterDatabase.persisted.contains(key), "a failed delete preserves the committed outfit");

    CharacterDatabase.fail = false;
    remove();
    Require(player.session.response == "DELETE_APPEARANCE_OUTFIT_OK", "a persisted outfit can be deleted after a failed attempt");
    Require(!player.session.persistedAtResponse.contains(key), "deletion commits before success");
    Require(!service.state->Outfits.contains(name), "successful deletion updates memory");
    Require(CharacterDatabase.persisted.at(otherCharacter) == "99", "deletion does not remove another character's outfit");
    remove();
    Require(player.session.response == "DELETE_APPEARANCE_OUTFIT_UNKNOWN", "deleting a missing outfit fails");

    CharacterDatabase.hold = true;
    auto queueSave = [&](uint32 appearance)
    {
        WorldPacket packet(CMSG_SAVE_APPEARANCE_OUTFIT);
        packet << name << uint32(1) << appearance;
        service._pendingPackets[1].push_back(std::move(packet));
    };
    auto queueDelete = [&]()
    {
        WorldPacket packet(CMSG_DELETE_APPEARANCE_OUTFIT);
        packet << name;
        service._pendingPackets[1].push_back(std::move(packet));
    };
    queueSave(42);
    queueSave(99);
    queueDelete();
    player.session.response.clear();
    service.OnPlayerUpdate(&player, 1);
    Require(CharacterDatabase.jobs.size() == 1 && service._pendingPackets.at(1).size() == 2,
        "only the first of several queued outfit mutations reaches the database");
    service.OnPlayerUpdate(&player, 1);
    player.session.callbacks.ProcessReadyCallbacks();
    Require(CharacterDatabase.jobs.size() == 1 && service._pendingPackets.at(1).size() == 2,
        "later updates retain queued outfit mutations while a commit is pending");
    Require(player.session.response.empty(), "pending outfit updates send no acknowledgement");
    CharacterDatabase.CompleteNext();
    player.session.callbacks.ProcessReadyCallbacks();
    Require(service.state->Outfits.at(name) == std::vector<uint32>{42}, "the first queued save commits before replacement");
    service.OnPlayerUpdate(&player, 1);
    Require(service._pendingPackets.at(1).size() == 1, "replacement leaves deletion queued until it commits");
    CharacterDatabase.CompleteNext();
    player.session.callbacks.ProcessReadyCallbacks();
    Require(service.state->Outfits.at(name) == std::vector<uint32>{99}, "the replacement commits in request order");
    service.OnPlayerUpdate(&player, 1);
    Require(service._pendingPackets.empty(), "deletion is submitted after replacement commits");
    Require(CharacterDatabase.persisted.contains(key), "pending deletion retains the committed outfit");
    CharacterDatabase.CompleteNext();
    player.session.callbacks.ProcessReadyCallbacks();
    Require(!service.state->Outfits.contains(name) && !CharacterDatabase.persisted.contains(key),
        "the final queued delete wins in both cache and database");

    queueSave(42);
    queueSave(99);
    service.OnPlayerUpdate(&player, 1);
    CharacterDatabase.fail = true;
    CharacterDatabase.CompleteNext();
    player.session.callbacks.ProcessReadyCallbacks();
    Require(!service.state->OutfitCommitPending && player.session.response == "SAVE_APPEARANCE_OUTFIT_UNKNOWN",
        "a failed callback releases the next queued request");
    CharacterDatabase.fail = false;
    service.OnPlayerUpdate(&player, 1);
    CharacterDatabase.CompleteNext();
    player.session.callbacks.ProcessReadyCallbacks();
    Require(service.state->Outfits.at(name) == std::vector<uint32>{99}, "the next queued save recovers after commit failure");

    player.session.response.clear();
    save("Logged out", {42});
    auto oldState = service.state;
    player.session.player = nullptr;
    service.state.reset();
    CharacterDatabase.CompleteNext();
    player.session.callbacks.ProcessReadyCallbacks();
    Require(player.session.response.empty() && !oldState->Outfits.contains("Logged out"),
        "a completion after logout does not reply or update a discarded collection");
    player.session.player = &player;
    service.state = NewState();

    save("Other character", {42});
    player.guid.value = 2;
    CharacterDatabase.CompleteNext();
    player.session.callbacks.ProcessReadyCallbacks();
    Require(player.session.response.empty() && !service.state->Outfits.contains("Other character"),
        "a completion cannot reply to a different character in the session");
    player.guid.value = 1;

    save("Relogged", {42});
    oldState = service.state;
    service.state = NewState();
    CharacterDatabase.CompleteNext();
    player.session.callbacks.ProcessReadyCallbacks();
    Require(player.session.response.empty() && !service.state->Outfits.contains("Relogged") &&
            !oldState->Outfits.contains("Relogged"),
        "a stale completion cannot update either state after the same character relogs");

    save(name, {42});
    CharacterDatabase.CompleteNext();
    player.session.callbacks.ProcessReadyCallbacks();
    player.session.response.clear();
    remove();
    oldState = service.state;
    service.state = NewState();
    service.state->Outfits[name] = {99};
    CharacterDatabase.CompleteNext();
    player.session.callbacks.ProcessReadyCallbacks();
    Require(player.session.response.empty() && service.state->Outfits.at(name) == std::vector<uint32>{99} &&
            oldState->Outfits.contains(name), "a stale delete callback cannot erase a relogged collection");
    std::cout << "PASS: nonblocking commits, ordered outfit mutations, commit-before-acknowledgement, crash durability and stale callback guards\n";
}
