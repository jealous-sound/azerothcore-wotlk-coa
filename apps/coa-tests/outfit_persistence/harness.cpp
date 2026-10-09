#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <future>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

using uint16 = std::uint16_t;
using uint32 = std::uint32_t;

// ACTUAL_CONSTANTS

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

    void EscapeString(std::string&) { }

    template <typename... Args>
    void Execute(std::string const&, Args const&...) { ++queued; }

    CharacterDatabasePreparedStatement* GetPreparedStatement(CharacterDatabaseStatements id)
    {
        return new CharacterDatabasePreparedStatement{id, 0, {}, {}};
    }

    CharacterDatabaseTransaction BeginTransaction() { return std::make_shared<Transaction>(); }

    struct CommitResult
    {
        std::future<bool> m_future;
    };

    CommitResult AsyncCommitTransaction(CharacterDatabaseTransaction const& transaction)
    {
        ++transactions;
        return {std::async(std::launch::deferred, [this, transaction]()
        {
            if (!fail)
            {
                auto const& statement = *transaction->statement;
                OutfitKey key{statement.guid, statement.name};
                if (statement.id == CHAR_REP_APPEARANCE_OUTFIT)
                    persisted[key] = statement.appearances;
                else
                    persisted.erase(key);
            }
            return !fail;
        })};
    }
} CharacterDatabase;

struct WorldPacket
{
    uint16 opcode;
    std::vector<std::variant<std::string, uint32>> fields;
    std::size_t position = 0;

    WorldPacket(uint16 value = 0, uint32 = 0) : opcode(value) { }

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

struct Session
{
    std::string response;
    StoredOutfits persistedAtResponse;

    void SendPacket(WorldPacket const* packet)
    {
        response = std::get<std::string>(packet->fields.front());
        persistedAtResponse = CharacterDatabase.persisted;
    }
};

struct Player
{
    struct Guid
    {
        uint32 value;
        uint32 GetCounter() const { return value; }
    } guid{1};
    Session session;

    Guid GetGUID() const { return guid; }
    Session* GetSession() { return &session; }
};

struct PlayerCollectionState
{
    std::unordered_set<uint32> CollectedAppearances{42, 99};
    std::map<std::string, std::vector<uint32>> Outfits;
};

struct CollectionService
{
    std::shared_ptr<PlayerCollectionState> state = std::make_shared<PlayerCollectionState>();
    std::shared_ptr<PlayerCollectionState> GetState(Player*) { return state; }

    // ACTUAL_SEND_RESULT
    // ACTUAL_VALID_NAME
    // ACTUAL_SAVE
    // ACTUAL_DELETE
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
    };
    auto remove = [&]()
    {
        WorldPacket packet;
        packet << name;
        service.HandleDeleteOutfit(&player, packet);
    };

    save(name, {42, 0, 99});
    Require(player.session.response == "SAVE_APPEARANCE_OUTFIT_OK", "a valid save succeeds");
    Require(player.session.persistedAtResponse.contains(key), "save success requires a committed outfit");
    Require(player.session.persistedAtResponse.at(key) == "42 0 99", "all appearance categories commit before success");
    CharacterDatabase.queued = 0;
    service.state = std::make_shared<PlayerCollectionState>();
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
    std::cout << "PASS: commit-before-acknowledgement, crash durability, replacement, character isolation and failure handling\n";
}
