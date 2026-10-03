#include "CoALegendaryCatalog.h"
#include "Config.h"
#include "Creature.h"
#include "Item.h"
#include "Log.h"
#include "LootMgr.h"
#include "Map.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "Random.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include <array>
#include <atomic>
#include <cmath>

namespace
{
    using namespace CoALegendary;

    char const* const PlayerStateKey = "CoALegendaryItems.Player";
    char const* const LootRollKey = "CoALegendaryItems.LootRoll";

    std::atomic<bool> enabled{ true };
    std::atomic<bool> dataReady{ false };
    std::atomic<float> dropChance{ 0.5f };
    std::atomic<uint32> stopDropLevel{ 60 };

    struct PlayerState : DataMap::Base
    {
        std::array<uint8, DesignCount> appliedLevels{};
        uint32 killPowerMs = 0;
        uint32 updateMs = 0;
    };

    struct LootRoll : DataMap::Base
    {
        bool attempted = false;
    };

    using EquippedLevels = std::array<uint8, DesignCount>;

    EquippedLevels GetEquippedLevels(Player* player)
    {
        EquippedLevels levels{};
        if (!enabled.load() || !dataReady.load() || !player->IsAlive() || !IsCustomClass(player->getClass()))
            return levels;

        for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
            if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                if (!item->IsBroken())
                    if (auto variant = DecodeEntry(item->GetEntry()))
                        if (FitsClass(Catalog[variant->design], player->getClass()) &&
                            variant->requiredLevel <= player->GetLevel())
                            levels[variant->design] = std::max(levels[variant->design],
                                uint8(variant->requiredLevel));
        return levels;
    }

    void ApplyPower(Player* player, uint32 designIndex, uint32 level)
    {
        Design const& design = Catalog[designIndex];
        int32 first = design.magnitude;
        int32 second = 0;
        int32 third = 0;
        if (design.power == Power::Offense)
        {
            first = AttackPowerBonus(level);
            second = first;
            if (design.profile == Profile::Caster)
                first = second = SpellPowerBonus(level);
            else if (design.profile == Profile::StrengthHybrid || design.profile == Profile::AgilityHybrid ||
                design.profile == Profile::IntellectHybrid)
                second = third = SpellPowerBonus(level);
        }
        player->CastCustomSpell(player, AuraEntryBase + designIndex, &first, &second, &third, true);
    }

    void RefreshPowers(Player* player, PlayerState& state)
    {
        EquippedLevels levels = GetEquippedLevels(player);
        for (uint32 index = 0; index < DesignCount; ++index)
        {
            Design const& design = Catalog[index];
            if (design.power == Power::Signature)
                continue;

            bool const active = levels[index] && ConditionActive(design.condition,
                player->GetHealthPct(), player->IsInCombat(), state.killPowerMs);
            uint32 const auraId = AuraEntryBase + index;
            if (!active)
            {
                if (state.appliedLevels[index])
                    player->RemoveAurasDueToSpell(auraId);
                state.appliedLevels[index] = 0;
                continue;
            }

            if (state.appliedLevels[index] != levels[index] || !player->HasAura(auraId))
            {
                player->RemoveAurasDueToSpell(auraId);
                ApplyPower(player, index, levels[index]);
                state.appliedLevels[index] = levels[index];
            }
        }
    }

    uint32 SignatureBonus(Unit* attacker, SpellInfo const* spell)
    {
        Player* player = attacker ? attacker->ToPlayer() : nullptr;
        if (!player || !spell || !enabled.load() || !dataReady.load() || !player->IsAlive() ||
            !IsCustomClass(player->getClass()))
            return 0;

        uint32 const firstRank = sSpellMgr->GetFirstSpellInChain(spell->Id);
        for (uint32 index = 0; index < DesignCount; ++index)
        {
            Design const& design = Catalog[index];
            if (design.power != Power::Signature || design.classId != player->getClass() ||
                spell->SpellFamilyName != uint32(design.classId) + 6 ||
                firstRank != sSpellMgr->GetFirstSpellInChain(design.signatureSpell))
                continue;

            EquippedLevels const levels = GetEquippedLevels(player);
            return levels[index] ? design.magnitude : 0;
        }
        return 0;
    }

    bool TryAddLegendary(Creature* creature, Player* owner, Loot* loot)
    {
        if (!owner || !enabled.load() || !dataReady.load() || !creature->GetLootMode() ||
            creature->IsLootRewardDisabled() || creature->GetOwnerGUID() || creature->GetCharmerGUID() ||
            loot->items.size() >= MAX_NR_LOOT_ITEMS)
            return false;

        uint32 const level = creature->getLevelForTarget(owner);
        if (!CanDrop(owner->getClass(), owner->GetLevel(), level, stopDropLevel.load(),
            owner->isHonorOrXPTarget(creature)) || !roll_chance_f(dropChance.load()))
            return false;

        std::array<uint32, 4> candidates{};
        uint32 count = 0;
        for (uint32 index = 0; index < DesignCount; ++index)
            if (FitsClass(Catalog[index], owner->getClass()))
                candidates[count++] = index;
        if (!count)
            return false;

        uint32 const entry = EntryForLevel(candidates[urand(0, count - 1)], level);
        loot->lootOwnerGUID = owner->GetGUID();
        std::size_t const before = loot->items.size();
        loot->AddItem(LootStoreItem(entry, 0, 100.0f, false, creature->GetLootMode(), 0, 1, 1));
        return loot->items.size() > before;
    }

    class LegendaryWorldScript final : public WorldScript
    {
    public:
        LegendaryWorldScript() : WorldScript("coa_legendary_items_world",
            { WORLDHOOK_ON_AFTER_CONFIG_LOAD, WORLDHOOK_ON_STARTUP }) { }

        void OnAfterConfigLoad(bool) override
        {
            enabled.store(sConfigMgr->GetOption<bool>("CoALegendaryItems.Enable", true));
            float const configuredChance = sConfigMgr->GetOption<float>("CoALegendaryItems.DropChance", 0.5f);
            dropChance.store(std::isfinite(configuredChance) ? std::clamp(configuredChance, 0.0f, 100.0f) : 0.0f);
            stopDropLevel.store(std::clamp(
                sConfigMgr->GetOption<uint32>("CoALegendaryItems.StopDropLevel", 60), 1u, 81u));
        }

        void OnStartup() override
        {
            uint32 invalid = 0;
            for (uint32 index = 0; index < DesignCount; ++index)
            {
                Design const& design = Catalog[index];
                for (uint32 level = 1; level <= MaximumCreatureLevel; ++level)
                {
                    ItemTemplate const* item = sObjectMgr->GetItemTemplate(EntryForLevel(index, level));
                    if (!item || item->RequiredLevel != level || item->ItemLevel != ItemLevel(level) ||
                        item->Quality != ITEM_QUALITY_LEGENDARY || item->ItemSet ||
                        item->InventoryType != design.inventoryType)
                        ++invalid;
                }
                if (design.power != Power::Signature && !sSpellMgr->GetSpellInfo(AuraEntryBase + index))
                    ++invalid;
                if (design.power == Power::Signature && !sSpellMgr->GetSpellInfo(design.signatureSpell))
                    ++invalid;
            }
            dataReady.store(invalid == 0);
            if (invalid)
                LOG_ERROR("module", "CoA Legendary Items: {} missing or invalid records; apply the pending world "
                    "migration. Drops and powers are disabled until the data is available.", invalid);
            else
                LOG_INFO("module", "CoA Legendary Items: {} designs, {} level variants; drop chance {}%, cutoff {}.",
                    DesignCount, DesignCount * MaximumCreatureLevel, dropChance.load(), stopDropLevel.load());
        }
    };

    class LegendaryLootScript final : public MiscScript
    {
    public:
        LegendaryLootScript() : MiscScript("coa_legendary_items_loot",
            { MISCHOOK_ON_AFTER_LOOT_TEMPLATE_PROCESS }) { }

        void OnAfterLootTemplateProcess(Loot* loot, LootTemplate const*, LootStore const& store,
            Player* owner, bool, bool, uint16) override
        {
            if (&store != &LootTemplates_Creature || !owner)
                return;
            Creature* creature = owner->GetMap()->GetCreature(loot->sourceWorldObjectGUID);
            if (!creature || !creature->IsAlive() || loot != &creature->loot)
                return;

            creature->CustomData.GetDefault<LootRoll>(LootRollKey)->attempted = true;
            TryAddLegendary(creature, owner, loot);
        }
    };

    class LegendaryPlayerScript final : public PlayerScript
    {
    public:
        LegendaryPlayerScript() : PlayerScript("coa_legendary_items_player",
            { PLAYERHOOK_ON_UPDATE, PLAYERHOOK_ON_LOGOUT }) { }

        void OnPlayerUpdate(Player* player, uint32 diff) override
        {
            PlayerState* state = player->CustomData.Get<PlayerState>(PlayerStateKey);
            if (!state)
            {
                if (!enabled.load() || !dataReady.load() || !IsCustomClass(player->getClass()))
                    return;
                state = player->CustomData.GetDefault<PlayerState>(PlayerStateKey);
            }
            state->killPowerMs = AdvanceTimer(state->killPowerMs, diff);
            state->updateMs = AdvanceTimer(state->updateMs, diff);
            if (!state->updateMs)
            {
                state->updateMs = PowerUpdateMs;
                RefreshPowers(player, *state);
            }
        }

        void OnPlayerLogout(Player* player) override
        {
            if (PlayerState* state = player->CustomData.Get<PlayerState>(PlayerStateKey))
                for (uint32 index = 0; index < DesignCount; ++index)
                    if (state->appliedLevels[index])
                        player->RemoveAurasDueToSpell(AuraEntryBase + index);
            player->CustomData.Erase(PlayerStateKey);
        }
    };

    class LegendaryUnitScript final : public UnitScript
    {
    public:
        LegendaryUnitScript() : UnitScript("coa_legendary_items_unit", true,
            { UNITHOOK_ON_UNIT_DEATH, UNITHOOK_MODIFY_SPELL_DAMAGE_TAKEN }) { }

        void OnUnitDeath(Unit* unit, Unit* killer) override
        {
            Creature* creature = unit->ToCreature();
            if (!creature)
                return;

            LootRoll* roll = creature->CustomData.Get<LootRoll>(LootRollKey);
            if ((!roll || !roll->attempted) && creature->GetLootRecipient() &&
                creature->IsDamageEnoughForLootingAndReward())
            {
                Player* owner = ObjectAccessor::FindPlayer(creature->loot.lootOwnerGUID);
                if (!owner)
                    owner = creature->GetLootRecipient();
                if (TryAddLegendary(creature, owner, &creature->loot))
                {
                    creature->loot.FillNotNormalLootFor(owner);
                    creature->RemoveUnitFlag(UNIT_FLAG_SKINNABLE);
                    creature->SetCorpseRemoveTime(creature->GetCorpseDelay());
                    creature->SetDynamicFlag(UNIT_DYNFLAG_LOOTABLE);
                }
            }
            creature->CustomData.Erase(LootRollKey);

            Player* player = killer ? killer->GetCharmerOrOwnerPlayerOrPlayerItself() : nullptr;
            if (player && enabled.load() && dataReady.load() && IsCustomClass(player->getClass()) &&
                player->isHonorOrXPTarget(creature) && creature->GetLootRecipient() &&
                !creature->GetOwnerGUID() && !creature->GetCharmerGUID())
            {
                PlayerState* state = player->CustomData.GetDefault<PlayerState>(PlayerStateKey);
                state->killPowerMs = KillPowerDurationMs;
                RefreshPowers(player, *state);
            }
        }

        void ModifySpellDamageTaken(Unit*, Unit* attacker, int32& damage, SpellInfo const* spell) override
        {
            damage = BoostSignature(damage, SignatureBonus(attacker, spell));
        }

    };
}

void AddSC_coa_legendary_items()
{
    new LegendaryWorldScript();
    new LegendaryLootScript();
    new LegendaryPlayerScript();
    new LegendaryUnitScript();
}
