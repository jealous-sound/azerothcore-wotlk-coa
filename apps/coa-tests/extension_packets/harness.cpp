#include "AscensionCollectibleSpellData.h"
#include "ItemTemplate.h"
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
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
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

struct Player;

class WorldSession
{
public:
    uint32 AccountId = 1;
    int LocaleIndex = -1;
    Player* PlayerObject = nullptr;
    std::vector<WorldPacket> Sent;
    std::vector<std::string> Messages;

    uint32 GetAccountId() const { return AccountId; }
    int GetSessionDbLocaleIndex() const { return LocaleIndex; }
    void SendPacket(WorldPacket const* packet) { Sent.push_back(*packet); }
    void HandleItemQuerySingleOpcode(WorldPacket& recvData);
    void SendItemQuerySingleResponse(uint32 item);
};

struct ObjectMgr
{
    std::unordered_map<uint32, ItemTemplate> Items;
    std::unordered_map<uint32, ItemLocale> Locales;

    ItemTemplate const* GetItemTemplate(uint32 entry) const
    {
        auto const itr = Items.find(entry);
        return itr != Items.end() ? &itr->second : nullptr;
    }

    ItemLocale const* GetItemLocale(uint32 entry) const
    {
        auto const itr = Locales.find(entry);
        return itr != Locales.end() ? &itr->second : nullptr;
    }

    // ACTUAL_GET_LOCALE_STRING
} objectMgr;

ObjectMgr* sObjectMgr = &objectMgr;

struct SpellInfo
{
    uint32 RecoveryTime = 1500;
    uint32 CategoryRecoveryTime = 0;

    uint32 GetCategory() const { return 0; }
};

struct SpellMgr
{
    SpellInfo Spell;
    std::unordered_set<uint32> Known = {133, 200001, 200002};

    SpellInfo const* GetSpellInfo(uint32 spellId) const { return Known.contains(spellId) ? &Spell : nullptr; }
} spellMgr;

SpellMgr* sSpellMgr = &spellMgr;

// ACTUAL_ITEM_QUERY

struct Item
{
    uint32 Entry = 0;
};

using ItemPosCountVec = std::vector<uint32>;

enum InventoryResult
{
    EQUIP_ERR_OK = 0,
    EQUIP_ERR_INVENTORY_FULL = 50
};

constexpr uint8 NULL_BAG = 0;
constexpr uint8 NULL_SLOT = 255;

struct Player
{
    WorldSession* Session = nullptr;
    uint32 ChargeSnapshots = 0;
    uint32 EchoSnapshots = 0;
    bool BagsFull = false;
    std::deque<Item> Items;
    std::vector<uint32> Stored;
    std::vector<uint32> Learned;
    uint32 EquipErrors = 0;
    uint32 NewItemNotices = 0;

    WorldSession* GetSession() const { return Session; }
    std::string GetName() const { return "Tester"; }
    void SendAllSpellChargeStates() { ++ChargeSnapshots; }

    InventoryResult CanStoreNewItem(uint8, uint8, ItemPosCountVec&, uint32, uint32) const
    {
        return BagsFull ? EQUIP_ERR_INVENTORY_FULL : EQUIP_ERR_OK;
    }

    void SendEquipError(InventoryResult, Item*, Item*, uint32) { ++EquipErrors; }

    Item* StoreNewItem(ItemPosCountVec const&, uint32 itemId, bool)
    {
        Stored.push_back(itemId);
        Items.push_back({itemId});
        return &Items.back();
    }

    void SendNewItem(Item*, uint32, bool, bool) { ++NewItemNotices; }
    void learnSpell(uint32 spellId, bool = false) { Learned.push_back(spellId); }
    bool HasSpell(uint32 spellId) const { return std::count(Learned.begin(), Learned.end(), spellId); }
    bool HasItemCount(uint32 itemId) const { return std::count(Stored.begin(), Stored.end(), itemId); }
    void SendInitialSpells() { }
};

class ChatHandler
{
public:
    explicit ChatHandler(WorldSession* session) : _session(session) { }

    Player* GetPlayer() const { return _session->PlayerObject; }
    void SendSysMessage(std::string_view text) { _session->Messages.emplace_back(text); }

    template<class... Arguments>
    void PSendSysMessage(std::string_view text, Arguments&&...)
    {
        _session->Messages.emplace_back(text);
    }

private:
    WorldSession* _session;
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
    bool UnlockAllVanity = true;
    bool LearnedSpellDelivery = true;

    template<class T>
    T GetConfigValue(AscensionCompatConfig key) const
    {
        if constexpr (std::is_same_v<T, std::string>)
            return key == AscensionCompatConfig::REALM_TYPE ? RealmType : ClassModel;
        else if constexpr (std::is_same_v<T, uint32>)
            return key == AscensionCompatConfig::FIRST_EXTENSION_OPCODE ? 0x051F : 0x09D3;
        else if (key == AscensionCompatConfig::UNLOCK_ALL_VANITY)
            return UnlockAllVanity;
        else if (key == AscensionCompatConfig::ALLOW_LEARNED_SPELL_DELIVERY)
            return LearnedSpellDelivery;
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

struct PlayerCollectionState
{
    std::unordered_set<uint32> OwnedVanityItems;
    uint32 CosmeticTimer = 0;
};

struct VanityInfo
{
    uint32 LearnedSpell = 0;
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
    void HandleSetAppearanceVisibility(Player*, WorldPacket& packet)
    {
        AppearancePackets.push_back(packet.GetOpcode());
    }
    void ProcessPendingAppearanceAdds(Player*, uint32) { }
    void ProcessPendingCompanionSpells(Player*, uint32) { }
    void ProcessCompanionLoot(Player*, uint32, bool = false) { }
    std::shared_ptr<PlayerCollectionState> GetState(Player*) { return State; }
    void RefreshCosmetics(Player*, PlayerCollectionState&) { }

    // ACTUAL_SEND_REALM_INFO
    // ACTUAL_QUEUE_CLIENT_PACKET
    // ACTUAL_ON_PLAYER_UPDATE
    // ACTUAL_HANDLE_CLIENT_PACKET
    // ACTUAL_POINT_SPEND
    // ACTUAL_DELIVER_VANITY
    // ACTUAL_BANK_VANITY

    std::shared_ptr<PlayerCollectionState> State;
    std::unordered_map<uint32, VanityInfo> _vanityItems;
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

struct AscensionCompatCommandScript
{
    // ACTUAL_LOCAL_VANITY_COMMAND
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

WorldPacket BulkQuery(std::vector<uint32> const& entries, uint32 count, uint16 opcode = 0x061B)
{
    WorldPacket packet(opcode, sizeof(uint32) * (entries.size() + 1));
    packet << count;
    for (uint32 entry : entries)
        packet << entry;
    return packet;
}

WorldPacket BulkQuery(std::vector<uint32> const& entries)
{
    return BulkQuery(entries, uint32(entries.size()));
}

std::vector<uint8> Bytes(WorldPacket const& packet)
{
    std::vector<uint8> bytes(packet.size());
    for (std::size_t index = 0; index < packet.size(); ++index)
        bytes[index] = packet[index];
    return bytes;
}

std::vector<uint8> SingleQueryReply(uint32 entry, int localeIndex)
{
    WorldSession session;
    session.LocaleIndex = localeIndex;
    WorldPacket query(0x0056, 4);
    query << entry;
    session.HandleItemQuerySingleOpcode(query);
    assert(session.Sent.size() == 1 && session.Sent[0].GetOpcode() == SMSG_ITEM_QUERY_SINGLE_RESPONSE);
    return Bytes(session.Sent[0]);
}

std::vector<std::vector<uint8>> BulkReplies(WorldSession& session, Player& player, WorldPacket const& query)
{
    session.Sent.clear();
    bool const passedOn = Receive(session, query);
    bool const answeredOnSocket = !session.Sent.empty();
    AscensionCollectionService::Instance().OnPlayerUpdate(&player, 1);
    std::vector<std::vector<uint8>> replies;
    for (WorldPacket const& packet : session.Sent)
        replies.push_back(packet.GetOpcode() == SMSG_ITEM_QUERY_SINGLE_RESPONSE ? Bytes(packet) : std::vector<uint8>{});
    if (passedOn || answeredOnSocket)
        replies.push_back({});
    return replies;
}

void TestItemQueries()
{
    ItemTemplate& blade = objectMgr.Items[35];
    blade.ItemId = 35;
    blade.Class = 2;
    blade.SubClass = 7;
    blade.SoundOverrideSubclass = -1;
    blade.Name1 = "Test Blade";
    blade.Description = "Sharp";
    blade.Spells[0].SpellId = 133;
    blade.Spells[0].SpellCooldown = -1;
    blade.Spells[0].SpellCategoryCooldown = -1;
    ItemTemplate& cloak = objectMgr.Items[135522];
    cloak.ItemId = 135522;
    cloak.Class = 4;
    cloak.Name1 = "Ascension Appearance 135522";
    objectMgr.Locales[35].Name = {"", "", "Testklinge"};

    WorldSession session;
    Player player;
    player.Session = &session;

    std::vector<std::vector<uint8>> const replies = BulkReplies(session, player, BulkQuery({35, 999999, 135522}));
    std::vector<uint8> const unknown = {0x3F, 0x42, 0x0F, 0x80};
    Check(replies.size() == 3 && replies[0] == SingleQueryReply(35, -1) && replies[1] == unknown &&
        replies[2] == SingleQueryReply(135522, -1),
        "a bulk item query answers each entry in order with the stock single-item response");

    WorldPacket first(SMSG_ITEM_QUERY_SINGLE_RESPONSE, 0);
    if (!replies.empty())
        first.append(replies[0].data(), replies[0].size());
    first.rpos(0);
    bool const stockLayout = first.size() > 16 && first.read<uint32>() == 35 && first.read<uint32>() == 2 &&
        first.read<uint32>() == 7 && first.read<int32>() == -1 && ReadString(first) == "Test Blade";
    Check(stockLayout, "the reply starts with the stock entry, class, subclass, sound and name fields");

    session.LocaleIndex = 2;
    std::vector<std::vector<uint8>> const localized = BulkReplies(session, player, BulkQuery({35}));
    Check(localized.size() == 1 && localized[0] == SingleQueryReply(35, 2) && localized[0] != SingleQueryReply(35, -1),
        "bulk replies use the session locale like single queries");
    session.LocaleIndex = -1;

    std::vector<uint32> full(256);
    for (uint32 index = 0; index < full.size(); ++index)
        full[index] = index % 2 ? 35 : 135522;
    Check(BulkReplies(session, player, BulkQuery(full)).size() == 256, "the largest accepted batch is answered");

    bool rejected = true;
    for (WorldPacket const& malformed : {BulkQuery({}), BulkQuery(std::vector<uint32>(257, 35)),
            BulkQuery({35}, 2), BulkQuery({35, 36}, 1), WorldPacket(0x061B, 0)})
        rejected &= BulkReplies(session, player, malformed).empty();
    WorldPacket shortCount(0x061B, 3);
    shortCount << uint8(1) << uint8(0) << uint8(0);
    rejected &= BulkReplies(session, player, shortCount).empty();
    Check(rejected, "empty, oversized, truncated and padded batches are consumed without replies");

    answeredCreatures.clear();
    session.Sent.clear();
    bool const creaturesPassedOn = Receive(session, BulkQuery({44472, 1234}, 2, 0x061A));
    bool const malformedPassedOn = Receive(session, BulkQuery({44472}, 2, 0x061A));
    Check(!creaturesPassedOn && !malformedPassedOn && answeredCreatures == std::vector<uint32>{44472, 1234},
        "creature bulk queries keep their validation and answers");
}

WorldPacket PointSpend(uint8 kind, uint32 itemId)
{
    WorldPacket packet(0x0523, 5);
    packet << kind << itemId;
    return packet;
}

struct Delivery
{
    std::vector<uint32> Stored;
    std::vector<uint32> Learned;
    std::vector<std::string> Messages;
    uint32 EquipErrors = 0;
    uint32 NewItemNotices = 0;

    bool operator==(Delivery const&) const = default;
};

struct VanitySetup
{
    bool UnlockAll = true;
    bool LearnedSpellDelivery = true;
    bool BagsFull = false;
};

Delivery Deliver(VanitySetup const& setup, std::vector<WorldPacket> const& requests, uint32 commandItem = 0)
{
    ascensionCompatConfig.UnlockAllVanity = setup.UnlockAll;
    ascensionCompatConfig.LearnedSpellDelivery = setup.LearnedSpellDelivery;
    AscensionCollectionService& service = AscensionCollectionService::Instance();
    service.State = std::make_shared<PlayerCollectionState>();
    service.State->OwnedVanityItems = {1001, 1003, 1004, 56925, 134985};
    WorldSession session;
    Player player;
    player.Session = &session;
    player.BagsFull = setup.BagsFull;
    session.PlayerObject = &player;
    if (commandItem)
    {
        ChatHandler handler(&session);
        AscensionCompatCommandScript::HandleLocalVanityCommand(&handler, commandItem);
    }
    bool consumed = true;
    for (WorldPacket const& request : requests)
        consumed &= !Receive(session, request);
    service.OnPlayerUpdate(&player, 1);
    service.State.reset();
    assert(consumed);
    return {player.Stored, player.Learned, session.Messages, player.EquipErrors, player.NewItemNotices};
}

void TestVanityDelivery()
{
    for (uint32 itemId : {1001u, 1002u, 56925u, 110000u, 134985u})
    {
        ItemTemplate& item = objectMgr.Items[itemId];
        item.ItemId = itemId;
        item.Name1 = "Vanity";
    }
    objectMgr.Items[110000].Spells[0].SpellId = 200001;
    objectMgr.Items[134985].Spells[0].SpellId = 200002;
    AscensionCollectionService& service = AscensionCollectionService::Instance();
    for (uint32 itemId : {1001u, 1002u, 1003u, 1004u, 56925u, 110000u, 134985u})
        service._vanityItems[itemId] = {};
    service._vanityItems[1003].LearnedSpell = 133;

    bool matches = true;
    for (bool unlockAll : {true, false})
        for (bool learnedSpells : {true, false})
            for (bool bagsFull : {false, true})
                for (uint32 itemId : {1001u, 1002u, 1003u, 1004u, 56925u, 110000u, 134985u, 424242u})
                {
                    VanitySetup const setup{unlockAll, learnedSpells, bagsFull};
                    matches &= Deliver(setup, {PointSpend(2, itemId)}) == Deliver(setup, {}, itemId);
                }
    Check(matches, "every native vanity delivery request ends exactly like .localvanity for the same item");

    Delivery const owned = Deliver({}, {PointSpend(2, 1001)});
    Delivery const bank = Deliver({}, {PointSpend(2, 134985)});
    Delivery const spell = Deliver({}, {PointSpend(2, 1003)});
    Check(owned.Stored == std::vector<uint32>{1001} && owned.NewItemNotices == 1 &&
        bank.Stored == std::vector<uint32>{134985} && bank.Learned == std::vector<uint32>{200002} &&
        spell.Learned == std::vector<uint32>{133}, "native requests deliver owned items, banks and learned spells");

    VanitySetup const locked{false, true, false};
    Delivery const refused = Deliver(locked, {PointSpend(2, 1002), PointSpend(2, 56925), PointSpend(2, 110000),
        PointSpend(2, 424242), PointSpend(2, 1004)});
    Delivery const full = Deliver({true, true, true}, {PointSpend(2, 1001)});
    Check(refused.Stored.empty() && refused.Learned.empty() && refused.Messages.size() == 5 &&
        full.Stored.empty() && full.EquipErrors == 1,
        "locked, sigil, unowned bank, unknown and templateless items and full bags are refused");

    WorldPacket shortRequest(0x0523, 4);
    shortRequest << uint32(1001);
    WorldPacket longRequest = PointSpend(2, 1001);
    longRequest << uint8(0);
    Delivery const ignored = Deliver({}, {PointSpend(1, 1001), PointSpend(0, 1001), PointSpend(3, 1001),
        shortRequest, longRequest, WorldPacket(0x0523, 0)});
    Check(ignored == Delivery{}, "other point-spend kinds and malformed requests deliver nothing");
}
}

int main()
{
    TestRealmInfo();
    TestWorldEntryResend();
    TestItemQueries();
    TestVanityDelivery();
    std::cout << checks - failures << '/' << checks << " checks passed\n";
    return failures ? 1 : 0;
}
