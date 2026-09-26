/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "DBCStores.h"
#include "Item.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include <algorithm>
#include <array>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace
{
constexpr std::array<std::string_view, 3> QualityTiers{ "@Refined@", "@Superior@", "@Masterwork@" };
constexpr int32 BaseTier = -1;

using AppearanceKey = std::tuple<std::string, uint32, uint32, uint32, uint32>;

struct Recipe
{
    uint32 skill;
    std::vector<uint32> reagents;
};

struct UpgradeCatalog
{
    std::unordered_map<uint32, uint32> next;
    std::unordered_map<uint32, uint32> baseOf;
    std::unordered_map<uint32, std::vector<Recipe>> recipes;
};

int32 TierOf(ItemTemplate const& proto)
{
    auto tier = std::find(QualityTiers.begin(), QualityTiers.end(), proto.Description);
    return tier == QualityTiers.end() ? BaseTier : int32(std::distance(QualityTiers.begin(), tier));
}

AppearanceKey AppearanceOf(ItemTemplate const& proto)
{
    return { proto.Name1, proto.DisplayInfoID, proto.Class, proto.SubClass, proto.InventoryType };
}

void CollectRecipes(UpgradeCatalog& catalog)
{
    for (SkillLineAbilityEntry const* ability : sSkillLineAbilityStore)
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(ability->Spell);
        if (!spellInfo)
            continue;

        std::vector<uint32> reagents;
        for (int32 reagent : spellInfo->Reagent)
            if (reagent > 0)
                reagents.push_back(uint32(reagent));

        for (SpellEffectInfo const& effect : spellInfo->Effects)
            if (effect.Effect == SPELL_EFFECT_CREATE_ITEM && effect.ItemType)
                catalog.recipes[effect.ItemType].push_back({ ability->SkillLine, reagents });
    }
}

void LinkQualityChains(UpgradeCatalog& catalog)
{
    ItemTemplateContainer const* templates = sObjectMgr->GetItemTemplateStore();
    std::map<AppearanceKey, std::vector<std::pair<int32, uint32>>> variants;
    for (auto const& [entry, proto] : *templates)
        if (int32 tier = TierOf(proto); tier != BaseTier)
            variants[AppearanceOf(proto)].emplace_back(tier, entry);

    std::map<AppearanceKey, std::set<uint32>> bases;
    for (auto const& [entry, recipes] : catalog.recipes)
    {
        ItemTemplate const* proto = sObjectMgr->GetItemTemplate(entry);
        if (proto && TierOf(*proto) == BaseTier && variants.count(AppearanceOf(*proto)))
            bases[AppearanceOf(*proto)].insert(entry);
    }

    for (auto const& [key, candidates] : bases)
    {
        if (candidates.size() != 1)
            continue;

        std::vector<std::pair<int32, uint32>> chain = variants.at(key);
        std::sort(chain.begin(), chain.end());
        auto duplicateTier = std::adjacent_find(chain.begin(), chain.end(),
            [](auto const& left, auto const& right) { return left.first == right.first; });
        if (duplicateTier != chain.end())
            continue;

        uint32 const base = *candidates.begin();
        uint32 previous = base;
        for (auto const& [tier, entry] : chain)
        {
            catalog.next[previous] = entry;
            catalog.baseOf[entry] = base;
            previous = entry;
        }
    }
}

UpgradeCatalog BuildCatalog()
{
    UpgradeCatalog catalog;
    CollectRecipes(catalog);
    LinkQualityChains(catalog);
    return catalog;
}

UpgradeCatalog const& Catalog()
{
    static UpgradeCatalog const catalog = BuildCatalog();
    return catalog;
}

bool SharesReagent(Recipe const& left, Recipe const& right)
{
    return std::any_of(left.reagents.begin(), left.reagents.end(), [&right](uint32 reagent)
    {
        return std::find(right.reagents.begin(), right.reagents.end(), reagent) != right.reagents.end();
    });
}

bool KitMatches(UpgradeCatalog const& catalog, uint32 kitEntry, uint32 targetEntry)
{
    auto base = catalog.baseOf.find(targetEntry);
    auto kitRecipes = catalog.recipes.find(kitEntry);
    auto baseRecipes = catalog.recipes.find(base == catalog.baseOf.end() ? targetEntry : base->second);
    if (kitRecipes == catalog.recipes.end() || baseRecipes == catalog.recipes.end())
        return false;

    for (Recipe const& kitRecipe : kitRecipes->second)
        for (Recipe const& baseRecipe : baseRecipes->second)
            if (kitRecipe.skill == baseRecipe.skill && SharesReagent(kitRecipe, baseRecipe))
                return true;
    return false;
}

Item* CreateUpgradedCopy(Player* player, Item* item, uint32 entry)
{
    Item* upgraded = Item::CreateItem(entry, 1, player);
    if (!upgraded)
        return nullptr;

    player->UpdateEnchantmentDurations();
    for (uint8 slot = PERM_ENCHANTMENT_SLOT; slot < MAX_INSPECTED_ENCHANTMENT_SLOT; ++slot)
    {
        EnchantmentSlot const enchantSlot = EnchantmentSlot(slot);
        if (uint32 enchant = item->GetEnchantmentId(enchantSlot))
            upgraded->SetEnchantment(enchantSlot, enchant, item->GetEnchantmentDuration(enchantSlot),
                item->GetEnchantmentCharges(enchantSlot));
    }

    upgraded->SetGuidValue(ITEM_FIELD_CREATOR, item->GetGuidValue(ITEM_FIELD_CREATOR));
    if (item->IsSoulBound())
        upgraded->SetBinding(true);

    uint32 const durability = std::min(item->GetUInt32Value(ITEM_FIELD_DURABILITY),
        upgraded->GetUInt32Value(ITEM_FIELD_MAXDURABILITY));
    if (item->GetUInt32Value(ITEM_FIELD_MAXDURABILITY))
        upgraded->SetUInt32Value(ITEM_FIELD_DURABILITY, durability);
    return upgraded;
}

bool ReplaceItem(Player* player, Item* item, uint32 entry, Spell* spell)
{
    Item* upgraded = CreateUpgradedCopy(player, item, entry);
    if (!upgraded)
        return false;

    uint8 const bag = item->GetBagSlot();
    uint8 const slot = item->GetSlot();
    uint16 const pos = item->GetPos();
    if (player->IsEquipmentPos(pos))
    {
        player->DestroyItem(bag, slot, true);
        spell->m_targets.SetItemTarget(nullptr);
        player->EquipItem(pos, upgraded, true);
        player->AutoUnequipOffhandIfNeed();
        return true;
    }

    bool const banked = player->IsBankPos(pos);
    ItemPosCountVec dest;
    InventoryResult result = EQUIP_ERR_ITEM_NOT_FOUND;
    if (banked)
        result = player->CanBankItem(bag, slot, dest, upgraded, true);
    else if (player->IsInventoryPos(pos))
        result = player->CanStoreItem(bag, slot, dest, upgraded, true);

    if (result != EQUIP_ERR_OK)
    {
        player->SendEquipError(result, item, nullptr);
        delete upgraded;
        return false;
    }

    player->DestroyItem(bag, slot, true);
    spell->m_targets.SetItemTarget(nullptr);
    if (banked)
        player->BankItem(dest, upgraded, true);
    else
    {
        player->StoreItem(dest, upgraded, true);
        player->ItemAddedQuestCheck(entry, 1);
    }
    return true;
}

class spell_ascension_upgrade_kit : public SpellScript
{
    PrepareSpellScript(spell_ascension_upgrade_kit);

    SpellCastResult CheckTarget()
    {
        Player* player = GetCaster()->ToPlayer();
        Item* kit = GetCastItem();
        Item* target = GetExplTargetItem();
        if (!player || !kit || !target || target->GetOwnerGUID() != player->GetGUID() || target->IsInTrade())
            return SPELL_FAILED_BAD_TARGETS;

        UpgradeCatalog const& catalog = Catalog();
        auto next = catalog.next.find(target->GetEntry());
        if (next == catalog.next.end() || !KitMatches(catalog, kit->GetEntry(), target->GetEntry()))
            return SPELL_FAILED_BAD_TARGETS;

        if (target->IsNotEmptyBag())
        {
            player->SendEquipError(EQUIP_ERR_CAN_ONLY_DO_WITH_EMPTY_BAGS, target, nullptr);
            return SPELL_FAILED_DONT_REPORT;
        }

        if (!target->IsEquipped())
            return SPELL_CAST_OK;

        ItemTemplate const* upgraded = sObjectMgr->GetItemTemplate(next->second);
        if (!upgraded || player->CanUseItem(upgraded) != EQUIP_ERR_OK)
            return SPELL_FAILED_BAD_TARGETS;
        if (player->IsInCombat() && !upgraded->CanChangeEquipStateInCombat())
            return SPELL_FAILED_AFFECTING_COMBAT;
        return SPELL_CAST_OK;
    }

    void Upgrade(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);
        Player* player = GetCaster()->ToPlayer();
        Item* target = GetHitItem();
        if (!player || !target)
            return;

        UpgradeCatalog const& catalog = Catalog();
        auto next = catalog.next.find(target->GetEntry());
        if (next != catalog.next.end())
            ReplaceItem(player, target, next->second, GetSpell());
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_ascension_upgrade_kit::CheckTarget);
        OnEffectHitTarget += SpellEffectFn(spell_ascension_upgrade_kit::Upgrade, EFFECT_0,
            SPELL_EFFECT_ENCHANT_ITEM);
    }
};
}

void AddSC_AscensionUpgradeKits()
{
    RegisterSpellScript(spell_ascension_upgrade_kit);
}
