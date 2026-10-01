/*
 * Flex loot: Molten Core bosses guarantee two Tier 1 token picks per kill
 * (the creature_loot_template reference already rolls that baseline on every
 * difficulty). Above the 20-player flex threshold a raid gets a third pick,
 * mirroring the health scaling in FlexHealth.cpp.
 *
 * coa_mc_token_loot maps each boss's exact creature entry (one row per
 * difficulty, since the reference id differs by difficulty) to the
 * reference_loot_template id already used for its guaranteed token picks, so
 * the bonus roll always draws from the same pool, data-driven rather than a
 * hard-coded item list per boss.
 */

#include "FlexLoot.h"

#include "Creature.h"
#include "DatabaseEnv.h"
#include "Field.h"
#include "Log.h"
#include "LootMgr.h"
#include "Map.h"
#include "Player.h"
#include "QueryResult.h"
#include "ScriptMgr.h"

#include "FlexHealth.h"

#include <unordered_map>
#include <vector>

namespace
{
    constexpr uint32 FLEX_LOOT_BONUS_THRESHOLD = 20;

    std::unordered_map<uint32, uint32> g_tokenRefByEntry;
    std::unordered_map<uint32, std::vector<uint32>> g_tokenItemsByRef;

    void LoadFlexLoot()
    {
        g_tokenRefByEntry.clear();
        g_tokenItemsByRef.clear();

        if (QueryResult result = WorldDatabase.Query("SELECT creature_entry, reference_id FROM coa_mc_token_loot"))
        {
            do
            {
                Field* f = result->Fetch();
                g_tokenRefByEntry[f[0].Get<uint32>()] = f[1].Get<uint32>();
            } while (result->NextRow());
        }

        if (!g_tokenRefByEntry.empty())
        {
            if (QueryResult result = WorldDatabase.Query(
                    "SELECT Entry, Item FROM reference_loot_template WHERE Entry IN "
                    "(SELECT DISTINCT reference_id FROM coa_mc_token_loot)"))
            {
                do
                {
                    Field* f = result->Fetch();
                    g_tokenItemsByRef[f[0].Get<uint32>()].push_back(f[1].Get<uint32>());
                } while (result->NextRow());
            }
        }

        LOG_INFO("server.loading", ">> Loaded {} Molten Core token loot entries", uint32(g_tokenRefByEntry.size()));
    }
}

namespace coa_flex
{
    uint32 BonusTokenRolls(Map* map)
    {
        return CountPlayers(map) >= FLEX_LOOT_BONUS_THRESHOLD ? 1 : 0;
    }
}

namespace
{
    class coa_flex_loot : public MiscScript
    {
    public:
        coa_flex_loot() : MiscScript("coa_flex_loot", { MISCHOOK_ON_AFTER_LOOT_TEMPLATE_PROCESS }) { }

        void OnAfterLootTemplateProcess(Loot* loot, LootTemplate const* /*tab*/, LootStore const& store,
            Player* lootOwner, bool /*personal*/, bool /*noEmptyError*/, uint16 /*lootMode*/) override
        {
            if (!loot || !lootOwner || &store != &LootTemplates_Creature || !lootOwner->GetMap())
                return;

            auto refIt = g_tokenRefByEntry.end();
            if (Creature const* creature = lootOwner->GetMap()->GetCreature(loot->sourceWorldObjectGUID))
                refIt = g_tokenRefByEntry.find(creature->GetEntry());

            if (refIt == g_tokenRefByEntry.end())
                return;

            uint32 const bonusRolls = coa_flex::BonusTokenRolls(lootOwner->GetMap());
            if (!bonusRolls)
                return;

            auto itemsIt = g_tokenItemsByRef.find(refIt->second);
            if (itemsIt == g_tokenItemsByRef.end() || itemsIt->second.empty())
                return;

            std::vector<uint32> const& items = itemsIt->second;
            for (uint32 i = 0; i < bonusRolls && loot->items.size() < MAX_NR_LOOT_ITEMS; ++i)
            {
                uint32 const itemId = items[urand(0, uint32(items.size()) - 1)];
                loot->AddItem(LootStoreItem(itemId, 0, 100.0f, false, LOOT_MODE_DEFAULT, 0, 1, 1));
            }
        }
    };

    class coa_flex_loot_loader : public WorldScript
    {
    public:
        coa_flex_loot_loader() : WorldScript("coa_flex_loot_loader") { }

        void OnAfterConfigLoad(bool /*reload*/) override
        {
            LoadFlexLoot();
        }
    };
}

void AddCoaFlexLootScripts()
{
    new coa_flex_loot();
    new coa_flex_loot_loader();
}
