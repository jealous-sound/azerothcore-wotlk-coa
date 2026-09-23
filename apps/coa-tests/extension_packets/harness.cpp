#include "WorldPacket.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

#define ASSERT(condition, ...) assert(condition)
// ACTUAL_BYTE_BUFFER

namespace
{
int failures = 0;
int checks = 0;

void Check(bool value, char const* name)
{
    ++checks;
    failures += !value;
    std::cout << (value ? "PASS: " : "FAIL: ") << name << '\n';
}

template<class... Arguments>
void LogSink(Arguments&&...)
{
}
}

#define LOG_INFO(...) LogSink(__VA_ARGS__)
#define LOG_WARN(...) LogSink(__VA_ARGS__)
#define LOG_DEBUG(...) LogSink(__VA_ARGS__)

class WorldSession
{
public:
    uint32 AccountId = 1;
    std::vector<WorldPacket> Sent;

    uint32 GetAccountId() const { return AccountId; }
    void SendPacket(WorldPacket const* packet) { Sent.push_back(*packet); }
};

struct Player
{
    WorldSession* Session = nullptr;
    uint32 ChargeSnapshots = 0;
    uint32 EchoSnapshots = 0;

    WorldSession* GetSession() const { return Session; }
    std::string GetName() const { return "Tester"; }
    void SendAllSpellChargeStates() { ++ChargeSnapshots; }
};

void SendAscensionRunemasterEchoesOwnership(Player* player)
{
    ++player->EchoSnapshots;
}

bool IsAscensionCharacterSelectionOpcode(uint16)
{
    return false;
}

bool HandleAscensionCharacterSelectionPacket(WorldSession*, WorldPacket const&)
{
    return false;
}

void SendAscensionCharacterListInfo(WorldSession*)
{
}

bool QueueAscensionManastormPacket(WorldSession*, WorldPacket const&)
{
    return false;
}

std::vector<uint32> answeredCreatures;

bool SendCollectionCreatureQueryResponse(WorldSession*, uint32 entry)
{
    answeredCreatures.push_back(entry);
    return true;
}

namespace AscensionCompatOpcodes
{
bool Dispatch(WorldSession*, WorldPacket const&)
{
    return false;
}
}

struct ServerScript
{
    virtual ~ServerScript() = default;
    [[nodiscard]] virtual bool CanPacketReceiveEarly(WorldSession*, WorldPacket const&) { return true; }
};

namespace
{
// ACTUAL_OPCODES
// ACTUAL_QUEUE_LIMIT
// ACTUAL_CONFIG_KEYS

struct CompatConfig
{
    std::string RealmType = "live";
    std::string ClassModel = "coa";

    template<class T>
    T GetConfigValue(AscensionCompatConfig key) const
    {
        if constexpr (std::is_same_v<T, std::string>)
            return key == AscensionCompatConfig::REALM_TYPE ? RealmType : ClassModel;
        else if constexpr (std::is_same_v<T, uint32>)
            return key == AscensionCompatConfig::FIRST_EXTENSION_OPCODE ? 0x051F : 0x09D3;
        else
            return true;
    }
} ascensionCompatConfig;

constexpr uint32 EXPANSION_WRATH_OF_THE_LICH_KING = 2;

struct RealmHandle
{
    uint32 Realm = 7;
};

struct
{
    RealmHandle Id;
} realm;

struct World
{
    std::string GetRealmName() const { return "Conquest of Azeroth"; }
} world;

World* sWorld = &world;

struct CollectionState
{
    uint32 CosmeticTimer = 0;
};

class AscensionCollectionService
{
public:
    static AscensionCollectionService& Instance()
    {
        static AscensionCollectionService service;
        return service;
    }

    std::vector<uint16> AppearancePackets;

    void HandleApplyAppearances(Player*, WorldPacket& packet) { AppearancePackets.push_back(packet.GetOpcode()); }
    void HandleSetAppearanceVisibility(Player*, WorldPacket& packet) { AppearancePackets.push_back(packet.GetOpcode()); }
    void ProcessPendingAppearanceAdds(Player*, uint32) { }
    void ProcessPendingCompanionSpells(Player*, uint32) { }
    void ProcessCompanionLoot(Player*, uint32, bool = false) { }
    std::shared_ptr<CollectionState> GetState(Player*) { return nullptr; }
    void RefreshCosmetics(Player*, CollectionState&) { }

    // ACTUAL_SEND_REALM_INFO
    // ACTUAL_QUEUE_CLIENT_PACKET
    // ACTUAL_ON_PLAYER_UPDATE
    // ACTUAL_HANDLE_CLIENT_PACKET

    std::mutex _packetMutex;
    std::unordered_map<uint32, std::deque<WorldPacket>> _pendingPackets;
};

struct AscensionClassService
{
    static AscensionClassService& Instance()
    {
        static AscensionClassService service;
        return service;
    }

    void QueueKnownEntriesUpload(uint32, WorldPacket const&) { }
};

struct AscensionCompatServerScript : ServerScript
{
    // ACTUAL_CAN_PACKET_RECEIVE_EARLY
};

struct RealmInfo
{
    std::vector<uint8> Flags;
    std::string Name;
    std::string Description;
    uint8 AddOnsAllowed = 0;
    bool Complete = false;
};

std::string ReadString(WorldPacket& packet)
{
    std::string value;
    for (char c = char(packet.read<uint8>()); c; c = char(packet.read<uint8>()))
        value += c;
    return value;
}

RealmInfo Decode(WorldPacket packet)
{
    RealmInfo info;
    packet.rpos(0);
    packet.read_skip(2 * sizeof(uint32) + 3 * sizeof(float) + sizeof(uint32) + 2 * sizeof(float) + sizeof(uint32));
    for (int flag = 0; flag < 8; ++flag)
        info.Flags.push_back(packet.read<uint8>());
    info.Name = ReadString(packet);
    info.Description = ReadString(packet);
    info.AddOnsAllowed = packet.read<uint8>();
    info.Complete = packet.rpos() == packet.size();
    return info;
}

RealmInfo SendRealmInfo(std::string const& realmType, std::string const& classModel)
{
    ascensionCompatConfig.RealmType = realmType;
    ascensionCompatConfig.ClassModel = classModel;
    WorldSession session;
    AscensionCollectionService::Instance().SendRealmInfo(&session);
    assert(session.Sent.size() == 1 && session.Sent[0].GetOpcode() == 0x09BC);
    return Decode(session.Sent[0]);
}

void TestRealmInfo()
{
    RealmInfo const live = SendRealmInfo("live", "coa");
    Check(live.Complete && live.Name == "Conquest of Azeroth" && live.Description.empty(),
        "realm info ends one byte after its two strings");
    Check(live.AddOnsAllowed == 1, "realm info tells the stock client that add-ons are allowed");
    Check(live.Flags == std::vector<uint8>{1, 0, 0, 0, 0, 0, 1, 0}, "live CoA realm flags are unchanged");

    bool allowedEverywhere = true;
    for (char const* realmType : {"live", "seasonal", "league", "ptr", "development"})
        for (char const* classModel : {"coa", "wcr", "classic"})
        {
            RealmInfo const info = SendRealmInfo(realmType, classModel);
            allowedEverywhere &= info.Complete && info.AddOnsAllowed == 1;
        }
    Check(allowedEverywhere, "every realm type and class model allows add-ons");
}

bool Receive(WorldSession& session, WorldPacket const& packet)
{
    return AscensionCompatServerScript().CanPacketReceiveEarly(&session, packet);
}

WorldPacket ExtensionInitialized()
{
    WorldPacket packet(0x0561, 8);
    packet << uint32(0) << uint32(1);
    return packet;
}

void TestWorldEntryResend()
{
    AscensionCollectionService& service = AscensionCollectionService::Instance();
    WorldSession session;
    Player player;
    player.Session = &session;

    bool const passedOn = Receive(session, ExtensionInitialized());
    Check(!passedOn && session.Sent.empty() && !player.ChargeSnapshots,
        "the socket hook consumes the world-entry notice and answers nothing itself");
    service.OnPlayerUpdate(&player, 1);
    Check(player.ChargeSnapshots == 1 && player.EchoSnapshots == 1,
        "the next world update resends the charge snapshot and the Runemaster echoes");

    bool consumed = true;
    for (int worldEntry = 0; worldEntry < 3; ++worldEntry)
        consumed &= !Receive(session, ExtensionInitialized());
    service.OnPlayerUpdate(&player, 1);
    Check(consumed && player.ChargeSnapshots == 4 && player.EchoSnapshots == 4,
        "login, loading screens and reloads each get their own resend");

    uint32 const charges = player.ChargeSnapshots;
    uint32 const echoes = player.EchoSnapshots;
    WorldPacket poll(0x0745, 0);
    Check(!Receive(session, poll), "other extension notices stay consumed");
    service.OnPlayerUpdate(&player, 1);
    Check(player.ChargeSnapshots == charges && player.EchoSnapshots == echoes,
        "other extension notices resend nothing");

    WorldPacket apply(0x0697, 4);
    apply << uint32(0);
    WorldPacket visibility(0x06A3, 2);
    visibility << uint8(1) << uint8(1);
    Receive(session, apply);
    Receive(session, visibility);
    service.OnPlayerUpdate(&player, 1);
    Check(service.AppearancePackets == std::vector<uint16>{0x0697, 0x06A3},
        "appearance packets still reach the world thread in order");
}
}

int main()
{
    TestRealmInfo();
    TestWorldEntryResend();
    std::cout << checks - failures << '/' << checks << " checks passed\n";
    return failures ? 1 : 0;
}
