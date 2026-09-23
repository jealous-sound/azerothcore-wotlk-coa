#include "WorldPacket.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
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

class WorldSession
{
public:
    std::vector<WorldPacket> Sent;

    void SendPacket(WorldPacket const* packet) { Sent.push_back(*packet); }
};

namespace
{
// ACTUAL_OPCODES

enum class AscensionCompatConfig
{
    REALM_TYPE,
    CLASS_MODEL
};

struct CompatConfig
{
    std::string RealmType = "live";
    std::string ClassModel = "coa";

    template<class T>
    T GetConfigValue(AscensionCompatConfig key) const
    {
        static_assert(std::is_same_v<T, std::string>);
        return key == AscensionCompatConfig::REALM_TYPE ? RealmType : ClassModel;
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

struct RealmInfoService
{
    // ACTUAL_SEND_REALM_INFO
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
    RealmInfoService().SendRealmInfo(&session);
    assert(session.Sent.size() == 1 && session.Sent[0].GetOpcode() == SMSG_REALM_INFO);
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
}

int main()
{
    TestRealmInfo();
    std::cout << checks - failures << '/' << checks << " checks passed\n";
    return failures ? 1 : 0;
}
