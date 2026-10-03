#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>
#include <type_traits>
#include <unordered_map>
#include <vector>

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
namespace ObjectGuid { using LowType = std::uint32_t; }
namespace GameTime
{
std::chrono::seconds GetGameTime() { return std::chrono::seconds(1000); }
}

// NATIVE_FLAGS

struct FormationInfo
{
    uint16 groupAI = 0;
    bool HasGroupFlag(uint16 flag) const { return groupAI & flag; }
};

struct Map
{
    std::map<ObjectGuid::LowType, time_t> respawns;
    unsigned writes = 0;
    time_t GetCreatureRespawnTime(ObjectGuid::LowType id) { return respawns[id]; }
    void SaveCreatureRespawnTime(ObjectGuid::LowType id, time_t time) { respawns[id] = time; ++writes; }
};

struct CreatureAI
{
    unsigned evades = 0;
    void EnterEvadeMode() { ++evades; }
};

struct Creature
{
    Creature(ObjectGuid::LowType spawnId, Map* ownerMap) : id(spawnId), map(ownerMap) { }
    ObjectGuid::LowType id;
    Map* map;
    bool alive = true;
    bool combat = false;
    bool evading = false;
    bool IsAIEnabled = true;
    unsigned respawns = 0;
    CreatureAI ai;
    ObjectGuid::LowType GetSpawnId() const { return id; }
    bool IsAlive() const { return alive; }
    bool IsInCombat() const { return combat; }
    bool IsInEvadeMode() const { return evading; }
    CreatureAI* AI() { return &ai; }
    Map* GetMap() { return map; }
    void Respawn() { ++respawns; }
};

struct FormationMgr
{
    std::unordered_map<ObjectGuid::LowType, FormationInfo> CreatureGroupMap;
    std::unordered_map<ObjectGuid::LowType, std::vector<ObjectGuid::LowType>> CreatureGroupMembers;
};
FormationMgr manager;
FormationMgr* sFormationMgr = &manager;

struct CreatureGroup
{
    Creature* m_leader = nullptr;
    std::map<Creature*, FormationInfo> m_members;
    ObjectGuid::LowType m_groupID = 1;
    void MemberEvaded(Creature* member);
    void RespawnRemovedMembers(Map* map);
};

// NATIVE_METHODS

unsigned failures = 0;
void Check(bool condition, char const* name)
{
    if (!condition)
    {
        std::cerr << "FAIL: " << name << '\n';
        ++failures;
    }
}

int main()
{
    for (uint16 flags : {0, 4, 8, 12, 24, 28, 15, 527})
    {
        manager = {};
        Map map;
        Creature survivor{1, &map};
        Creature corpse{2, &map};
        corpse.alive = false;
        Creature living{3, &map};
        living.combat = true;
        Creature idle{4, &map};
        CreatureGroup group;
        group.m_leader = &corpse;
        group.m_groupID = 2;
        manager.CreatureGroupMap[1].groupAI = flags;
        for (Creature* member : {&survivor, &corpse, &living, &idle})
            group.m_members[member].groupAI = flags;
        group.MemberEvaded(&survivor);
        Check(corpse.respawns == ((flags & 8) && !(flags & 16) ? 1u : 0u),
            "corpse respawn honors independent flags and protected leader");
        Check(living.ai.evades == (flags & 4 ? 1u : 0u), "combat member honors EVADE_TOGETHER");
        Check(idle.ai.evades == 0, "idle member does not evade");
        Check(survivor.ai.evades == 0, "evading member is skipped");
        Check(map.writes == 0, "in-world members do not write removed-member timers");
    }

    for (uint16 flags : {0, 4, 8, 12, 24, 28, 15, 527})
    {
        manager = {};
        Map map;
        Creature survivor{2, &map};
        CreatureGroup group;
        group.m_members[&survivor].groupAI = flags;
        manager.CreatureGroupMap[2].groupAI = flags;
        manager.CreatureGroupMembers[1] = {1, 2, 3, 4, 5, 6, 7};
        for (unsigned id : {1, 2, 3, 4, 5, 6})
            manager.CreatureGroupMap[id].groupAI = flags;
        manager.CreatureGroupMap[6].groupAI = 4;
        map.respawns = {{1, 8000}, {2, 8000}, {3, 8000}, {4, 1000}, {6, 8000}, {7, 8000}};
        group.MemberEvaded(&survivor);
        bool respawn = flags & 8;
        Check(map.respawns[3] == (respawn ? 1000 : 8000), "decayed member pending timer advances");
        Check(map.respawns[1] == (respawn && !(flags & 16) ? 1000 : 8000),
            "removed leader honors DONT_RESPAWN_LEADER");
        Check(map.respawns[2] == 8000, "present member pending timer is not duplicated");
        Check(map.respawns[4] == 1000, "already due timer remains due");
        Check(map.respawns[5] == 0, "never loaded member is not spawned");
        Check(map.respawns[6] == 8000, "member without respawn flag stays dead");
        Check(map.respawns[7] == 8000, "member without formation data stays dead");
        Check(map.writes == (respawn ? (flags & 16 ? 1u : 2u) : 0u), "only eligible pending timers saved");
        unsigned writes = map.writes;
        group.MemberEvaded(&survivor);
        Check(map.writes == writes, "same-tick evade cascade does not repeat timer writes");
        manager.CreatureGroupMap[2].groupAI = 0;
        map.respawns[3] = 9000;
        group.MemberEvaded(&survivor);
        Check(map.respawns[3] == 9000, "survivor without evade flags cannot reset formation");
    }
    if (failures)
        return EXIT_FAILURE;
    std::cout << "PASS: 8 flag combinations, live members, decayed members, protected leaders and timer idempotence\n";
}
