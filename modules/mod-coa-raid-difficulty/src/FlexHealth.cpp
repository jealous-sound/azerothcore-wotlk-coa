/*
 * Flex health: a boss's health follows the number of players in the instance.
 *
 * The difficulty labels read "(10-25 Players)", and the combat logs show what
 * that meant. Garr had 30.8 million health on Ascended with 13 players and
 * 43.1 million in another raid; divided by the player count, every boss lands
 * on the same ratio between difficulties. coa_boss_flex holds health PER
 * PLAYER, per difficulty, and this file multiplies it out.
 *
 * When:
 *  - at spawn, so a boss standing idle shows a sensible number, and
 *  - at the pull, which is the one that counts. The count is taken once and
 *    holds for the fight; someone who zones in halfway does not change it.
 *
 * Who counts: every player in the instance who is not a game master. That
 * includes bots, which are players to the core.
 *
 * The count is clamped to 10..25. The 25 is in the label. The 10 is an
 * assumption read off the same label - the logs only hold raids of 13 to 18,
 * so neither end was ever observed.
 *
 * A column of 0 means no flex for that difficulty: the boss keeps the health
 * its template gives it. Normal is 0 for every boss, because Normal appears in
 * no log.
 *
 * This hangs off a general combat hook rather than the boss AI on purpose, so
 * it applies equally to bosses that keep their stock script, like Ragnaros.
 */

#include "Creature.h"
#include "DBCEnums.h"
#include "DatabaseEnv.h"
#include "Field.h"
#include "Log.h"
#include "Map.h"
#include "Player.h"
#include "QueryResult.h"
#include "ScriptMgr.h"

#include <algorithm>
#include <limits>
#include <unordered_map>

namespace
{
    constexpr uint32 FLEX_MIN_PLAYERS = 10;
    constexpr uint32 FLEX_MAX_PLAYERS = 25;

    struct FlexRow
    {
        uint32 perPlayer[MAX_RAID_DIFFICULTY];
    };

    std::unordered_map<uint32, FlexRow> g_flex;

    uint32 BaseEntry(uint32 entry)
    {
        return entry % 100000;
    }

    void LoadFlex()
    {
        g_flex.clear();
        if (QueryResult result = WorldDatabase.Query("SELECT entry, hp_d0, hp_d1, hp_d2, hp_d3 FROM coa_boss_flex"))
        {
            do
            {
                Field* f = result->Fetch();
                FlexRow& row = g_flex[f[0].Get<uint32>()];
                for (uint8 i = 0; i < MAX_RAID_DIFFICULTY; ++i)
                    row.perPlayer[i] = f[1 + i].Get<uint32>();
            } while (result->NextRow());
        }
        LOG_INFO("server.loading", ">> Loaded flex health for {} bosses", uint32(g_flex.size()));
    }

    uint32 CountPlayers(Map* map)
    {
        uint32 count = 0;
        map->DoForAllPlayers([&count](Player* player)
        {
            if (!player->IsGameMaster())
                ++count;
        });
        return std::clamp(count, FLEX_MIN_PLAYERS, FLEX_MAX_PLAYERS);
    }

    // Scales health and keeps the current percentage, so a boss that is
    // already hurt stays exactly as hurt.
    void ApplyFlex(Creature* creature)
    {
        if (!creature || !creature->GetMap() || !creature->GetMap()->IsRaid())
            return;

        auto it = g_flex.find(BaseEntry(creature->GetEntry()));
        if (it == g_flex.end())
            return;

        uint8 const mode = uint8(creature->GetMap()->GetSpawnMode());
        if (mode >= MAX_RAID_DIFFICULTY || !it->second.perPlayer[mode])
            return;

        uint32 const players = CountPlayers(creature->GetMap());
        uint64 const wanted = uint64(it->second.perPlayer[mode]) * players;
        uint32 const health = uint32(std::min<uint64>(wanted, std::numeric_limits<uint32>::max()));

        float const pct = creature->GetMaxHealth() ? creature->GetHealthPct() : 100.0f;

        creature->SetCreateHealth(health);
        creature->SetStatFlatModifier(UNIT_MOD_HEALTH, BASE_VALUE, float(health));
        creature->UpdateMaxHealth();
        creature->SetHealth(std::max<uint32>(1, uint32(creature->GetMaxHealth() * pct / 100.0f)));

        LOG_DEBUG("scripts", "coa flex: {} on difficulty {} with {} players -> {} health",
                  creature->GetEntry(), mode, players, creature->GetMaxHealth());
    }

    class coa_flex_health_creature : public AllCreatureScript
    {
    public:
        coa_flex_health_creature() : AllCreatureScript("coa_flex_health_creature") { }

        void OnCreatureSelectLevel(CreatureTemplate const* /*cinfo*/, Creature* creature) override
        {
            ApplyFlex(creature);
        }
    };

    class coa_flex_health_unit : public UnitScript
    {
    public:
        coa_flex_health_unit() : UnitScript("coa_flex_health_unit", true, { UNITHOOK_ON_UNIT_ENTER_COMBAT }) { }

        void OnUnitEnterCombat(Unit* unit, Unit* /*victim*/) override
        {
            if (Creature* creature = unit->ToCreature())
                ApplyFlex(creature);
        }
    };

    class coa_flex_health_loader : public WorldScript
    {
    public:
        coa_flex_health_loader() : WorldScript("coa_flex_health_loader") { }

        void OnAfterConfigLoad(bool /*reload*/) override
        {
            LoadFlex();
        }
    };
}

void AddCoaFlexHealthScripts()
{
    new coa_flex_health_creature();
    new coa_flex_health_unit();
    new coa_flex_health_loader();
}
