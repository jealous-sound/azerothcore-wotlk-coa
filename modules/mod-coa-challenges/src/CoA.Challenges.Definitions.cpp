// mod-coa-challenges: per-challenge definitions.
//
// DB-first: the authoritative per-challenge data is realm/world data and lives
// in the WORLD database:
//   coa_challenge_definition  (one row per challenge)
//   coa_challenge_spell       (one row per challenge/level; level 0 = union)
// and is cached in memory (DefCache) at startup and on `.coa challenges reload`.
// The world DB is the single source of truth: there is no generated-.conf
// fallback, so an empty table means an empty catalog (logged as an error).
#include "CoA.Challenges.Review.h"
#include <algorithm>

namespace CoAChallenges
{
    std::mutex DefMutex;
    std::unordered_map<uint32, ChallengeDef> DefCache;

    bool DefHas(uint32 challengeID)
    {
        std::lock_guard<std::mutex> lock(DefMutex);
        return DefCache.find(challengeID) != DefCache.end();
    }

    uint32 DefCount()
    {
        std::lock_guard<std::mutex> lock(DefMutex);
        return uint32(DefCache.size());
    }

    std::vector<uint32> DefIds()
    {
        std::lock_guard<std::mutex> lock(DefMutex);
        std::vector<uint32> out;
        out.reserve(DefCache.size());
        for (auto const& [id, d] : DefCache)
            out.push_back(id);
        std::sort(out.begin(), out.end());
        return out;
    }

    void EnsureDefinitionTables()
    {
        // Dev fallback only: the authoritative schema ships as versioned SQL
        // (data/sql/db-world/...). Set CoAChallenges.AutoCreateSchema=1 to
        // (re)create the tables at runtime on a dev box.
        if (!sConfigMgr->GetOption<bool>("CoAChallenges.AutoCreateSchema", false))
            return;

        WorldDatabase.DirectExecute(
            "CREATE TABLE IF NOT EXISTS coa_challenge_definition ("
            " id INT UNSIGNED NOT NULL PRIMARY KEY,"
            " name VARCHAR(190) NOT NULL DEFAULT '',"
            " icon VARCHAR(190) NOT NULL DEFAULT '',"
            " levelCount INT UNSIGNED NOT NULL DEFAULT 1,"
            " isTrial TINYINT UNSIGNED NOT NULL DEFAULT 0,"
            " isPrestige TINYINT UNSIGNED NOT NULL DEFAULT 0,"
            " exclusiveGroup INT UNSIGNED NOT NULL DEFAULT 0,"
            " requiredGameMode INT UNSIGNED NOT NULL DEFAULT 0,"
            " requiredGameEvent INT UNSIGNED NOT NULL DEFAULT 0,"
            " noRewards TINYINT UNSIGNED NOT NULL DEFAULT 0,"
            " noResurrect TINYINT UNSIGNED NOT NULL DEFAULT 0,"
            " lives INT UNSIGNED NOT NULL DEFAULT 0,"
            " sharedFate TINYINT UNSIGNED NOT NULL DEFAULT 0,"
            " survivalist TINYINT UNSIGNED NOT NULL DEFAULT 0,"
            " rules TEXT,"
            " conditions TEXT,"
            " objectives TEXT"
            ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4");
        WorldDatabase.DirectExecute(
            "CREATE TABLE IF NOT EXISTS coa_challenge_spell ("
            " challengeId INT UNSIGNED NOT NULL,"
            " level INT UNSIGNED NOT NULL DEFAULT 0,"
            " pve TEXT,"
            " pvp TEXT,"
            " PRIMARY KEY (challengeId, level)"
            ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4");
        WorldDatabase.DirectExecute(
            "CREATE TABLE IF NOT EXISTS coa_challenge_reward ("
            " challengeId INT UNSIGNED NOT NULL,"
            " level INT UNSIGNED NOT NULL DEFAULT 1,"
            " itemId INT UNSIGNED NOT NULL DEFAULT 0,"
            " amount INT UNSIGNED NOT NULL DEFAULT 1,"
            " achievement INT UNSIGNED NOT NULL DEFAULT 0,"
            " isSpecial TINYINT UNSIGNED NOT NULL DEFAULT 0,"
            " isFirst TINYINT UNSIGNED NOT NULL DEFAULT 1,"
            " PRIMARY KEY (challengeId, level, itemId, achievement)"
            ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4");
    }

    void LoadChallengeDefinitions()
    {
        EnsureDefinitionTables();

        std::unordered_map<uint32, ChallengeDef> fresh;
        if (QueryResult r = WorldDatabase.Query(
                "SELECT id, name, icon, levelCount, isTrial, isPrestige, exclusiveGroup,"
                " requiredGameMode, requiredGameEvent, noRewards, noResurrect, lives,"
                " sharedFate, survivalist, rules, conditions, objectives"
                " FROM coa_challenge_definition"))
        {
            do
            {
                Field* f = r->Fetch();
                ChallengeDef d;
                d.id = f[0].Get<uint32>();
                d.name = f[1].Get<std::string>();
                d.icon = f[2].Get<std::string>();
                d.levelCount = f[3].Get<uint32>();
                d.isTrial = f[4].Get<uint8>() != 0;
                d.isPrestige = f[5].Get<uint8>() != 0;
                d.exclusiveGroup = f[6].Get<uint32>();
                d.requiredGameMode = f[7].Get<uint32>();
                d.requiredGameEvent = f[8].Get<uint32>();
                d.noRewards = f[9].Get<uint8>() != 0;
                d.noResurrect = f[10].Get<uint8>() != 0;
                d.lives = f[11].Get<uint32>();
                d.sharedFate = f[12].Get<uint8>() != 0;
                d.survivalist = f[13].Get<uint8>() != 0;
                d.rules = f[14].Get<std::string>();
                d.conditions = f[15].Get<std::string>();
                d.objectives = f[16].Get<std::string>();
                fresh.emplace(d.id, std::move(d));
            } while (r->NextRow());
        }

        if (QueryResult r = WorldDatabase.Query(
                "SELECT challengeId, level, pve, pvp FROM coa_challenge_spell"))
        {
            do
            {
                Field* f = r->Fetch();
                auto it = fresh.find(f[0].Get<uint32>());
                if (it == fresh.end())
                    continue;
                it->second.spells[f[1].Get<uint32>()] = f[2].Get<std::string>();
            } while (r->NextRow());
        }

        if (QueryResult r = WorldDatabase.Query(
                "SELECT challengeId, level, itemId, amount, achievement, isSpecial, isFirst"
                " FROM coa_challenge_reward"))
        {
            do
            {
                Field* f = r->Fetch();
                auto it = fresh.find(f[0].Get<uint32>());
                if (it == fresh.end())
                    continue;
                RewardDef reward;
                reward.itemId = f[2].Get<uint32>();
                reward.amount = f[3].Get<uint32>() ? f[3].Get<uint32>() : 1;
                reward.achievement = f[4].Get<uint32>();
                reward.isSpecial = f[5].Get<uint8>() != 0;
                reward.isFirst = f[6].Get<uint8>() != 0;
                it->second.rewards[f[1].Get<uint32>()].push_back(reward);
            } while (r->NextRow());
        }

        {
            std::lock_guard<std::mutex> lock(DefMutex);
            DefCache.swap(fresh);
        }

        // Derived caches must be rebuilt so the new data is picked up.
        {
            std::lock_guard<std::mutex> lock(ObjectiveCacheMutex);
            ObjectiveCache.clear();
        }
        BuildGameModeBaseMap();

        LOG_INFO("module.coa_challenges", "Challenge definitions loaded from world DB: {} row(s)",
            DefCount());
        if (DefCount() == 0)
        {
            LOG_ERROR("module.coa_challenges",
                "No challenge definitions in the world DB (coa_challenge_definition is empty). "
                "Seed the definitions (coa-analyze-export.ps1 -EmitSql -> acore_world) or set "
                "CoAChallenges.AutoCreateSchema=1 for a dev box; the challenge catalog is empty.");
        }
    }
}
