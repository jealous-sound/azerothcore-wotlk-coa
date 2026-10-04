// SPDX-License-Identifier: GPL-2.0-or-later

#include "Chat.h"
#include "CommandScript.h"
#include "Config.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "Player.h"
#include "RBAC.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuras.h"
#include "NourishmentSlotLogic.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#if __has_include("Playerbots.h")
#include "Playerbots.h"
#define HOMEBREW_NOURISHMENT_HAS_PLAYERBOTS 1
#else
#define HOMEBREW_NOURISHMENT_HAS_PLAYERBOTS 0
#endif

#include <algorithm>
#include <array>
#include <chrono>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <mutex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace Acore::ChatCommands;

namespace
{
constexpr char kAddonPrefix[] = "HBN";
constexpr char kProtocolVersion[] = "3";
constexpr char kFieldSeparator = '~';

struct NourishmentProfile
{
    uint32 itemId = 0;
    std::string itemName;
    uint32 sourceSpell = 0;
    uint32 mealAuraSpell = 0;
    uint8 tier = 0;
    uint8 grade = 0;
    std::string family;
    uint32 markerSpell = 0;
    int32 amount1 = 0;
    int32 amount2 = 0;
    int32 amount3 = 0;
    uint8 minimumLevel = 1;
    uint32 durationSeconds = 1800;
    std::string recoveryKind;
    uint32 nativeAura = 0;
    bool cooked = false;
    uint32 cookedBonusPercent = 0;
};

struct PendingMeal
{
    uint32 itemId = 0;
    uint32 itemCountBefore = 0;
    uint32 mealAuraSpell = 0;
    uint32 mapId = 0;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    std::chrono::steady_clock::time_point started;
};

struct ActiveNourishment
{
    NourishmentProfile profile;
    uint64 expiresAt = 0;
    uint8 slot = 0;
    bool suppressRemoval = false;
    bool castingMarker = false;
    bool needsRepair = false;
};

std::unordered_map<uint32, NourishmentProfile> gProfiles;
std::unordered_set<uint32> gStockAuras;
std::unordered_map<uint32, PendingMeal> gPendingMeals;
using ActiveNourishmentSlots = std::array<ActiveNourishment, kNourishmentSlotCount>;
std::unordered_map<uint32, ActiveNourishmentSlots> gActiveNourishment;
std::recursive_mutex gNourishmentStateMutex;

using NourishmentStateLock = std::lock_guard<std::recursive_mutex>;

bool IsEnabled()
{
    return sConfigMgr->GetOption<bool>("HomebrewNourishment.Enable", true);
}

bool IncludePlayerbots()
{
    return sConfigMgr->GetOption<bool>("HomebrewNourishment.IncludePlayerbots", true);
}

bool PersistThroughDeath()
{
    return sConfigMgr->GetOption<bool>("HomebrewNourishment.PersistThroughDeath", false);
}

bool AllowInBattlegrounds()
{
    return sConfigMgr->GetOption<bool>("HomebrewNourishment.AllowInBattlegrounds", true);
}

bool AllowInArenas()
{
    return sConfigMgr->GetOption<bool>("HomebrewNourishment.AllowInArenas", false);
}

bool NotifyPlayers()
{
    return sConfigMgr->GetOption<bool>("HomebrewNourishment.NotifyPlayers", true);
}

uint32 ProfileDuration(NourishmentProfile const& profile)
{
    uint32 configured = profile.grade
        ? sConfigMgr->GetOption<uint32>("HomebrewNourishment.PremiumDurationSeconds", 3600)
        : sConfigMgr->GetOption<uint32>("HomebrewNourishment.OrdinaryDurationSeconds", 1800);
    uint32 duration = configured ? configured : profile.durationSeconds;
    return std::max<uint32>(60, duration);
}

uint32 GuidLow(Player const* player)
{
    return player ? player->GetGUID().GetCounter() : 0;
}

bool IsPlayerbot(Player* player)
{
#if HOMEBREW_NOURISHMENT_HAS_PLAYERBOTS
    return player && sPlayerbotsMgr.GetPlayerbotAI(player);
#else
    (void)player;
    return false;
#endif
}

std::string FamilyLabel(std::string const& family)
{
    if (family == "hearty") return "Hearty";
    if (family == "nimble") return "Nimble";
    if (family == "fortifying") return "Fortifying";
    if (family == "insightful") return "Insightful";
    if (family == "keen") return "Keen";
    if (family == "precise") return "Precise";
    if (family == "energizing") return "Energizing";
    if (family == "clear") return "Clear-Minded";
    if (family == "banquet") return "Banquet";
    return family;
}

uint32 SlotAuraSpell(NourishmentProfile const& profile, uint8 slot)
{
    uint32 family = 9;
    if (profile.family == "hearty") family = 0;
    else if (profile.family == "nimble") family = 1;
    else if (profile.family == "fortifying") family = 2;
    else if (profile.family == "insightful") family = 3;
    else if (profile.family == "keen") family = 4;
    else if (profile.family == "precise") family = 5;
    else if (profile.family == "energizing") family = 6;
    else if (profile.family == "clear") family = 7;
    else if (profile.family == "banquet") family = 8;
    if (family >= 9 || slot < 1 || slot > kNourishmentSlotCount)
        return 0;
    // CoA already assigns 991001-991027 to live Ascension spells. The CoA
    // profile's patched Spell.dbc reserves 995186-995212 for these meal auras.
    return 995186 + family * kNourishmentSlotCount + slot - 1;
}

std::string EffectText(NourishmentProfile const& profile)
{
    std::ostringstream text;
    if (profile.family == "hearty")
        text << "+" << profile.amount1 << " Strength and +" << profile.amount2 << " Stamina";
    else if (profile.family == "nimble")
        text << "+" << profile.amount1 << " Agility and +" << profile.amount2 << " Stamina";
    else if (profile.family == "fortifying")
        text << "+" << profile.amount1 << " Spirit and +" << profile.amount2 << " Stamina";
    else if (profile.family == "insightful")
        text << "+" << profile.amount1 << " spell power, +" << profile.amount2 << " Spirit";
    else if (profile.family == "keen")
        text << "+" << profile.amount1 << " critical strike rating and +" << profile.amount2 << " Stamina";
    else if (profile.family == "precise")
        text << "+" << profile.amount1 << " hit rating and +" << profile.amount2 << " Stamina";
    else if (profile.family == "energizing")
        text << "+" << profile.amount1 << " haste rating and +" << profile.amount2 << " Stamina";
    else if (profile.family == "clear")
        text << "+" << profile.amount1 << " mana per 5 seconds and +" << profile.amount2 << " Stamina";
    else if (profile.family == "banquet")
        text << "+" << profile.amount1 << " attack power, +" << profile.amount3
             << " spell power, and +" << profile.amount2 << " Stamina";
    return text.str();
}

std::string PercentEncode(std::string const& value)
{
    static char const digits[] = "0123456789ABCDEF";
    std::string output;
    for (unsigned char character : value)
    {
        if (character == '%' || character == '~' || character == '\r' || character == '\n')
        {
            output.push_back('%');
            output.push_back(digits[character >> 4]);
            output.push_back(digits[character & 0x0F]);
        }
        else
            output.push_back(static_cast<char>(character));
    }
    return output;
}

void SendAddonPacket(Player* player, std::string const& opcode, std::string const& payload = "")
{
    if (!player || !player->GetSession() || IsPlayerbot(player))
        return;

    std::string wire = std::string(kAddonPrefix) + "\t" + opcode;
    if (!payload.empty())
        wire += std::string(1, kFieldSeparator) + payload;

    WorldPacket packet;
    ChatHandler::BuildChatPacket(packet, CHAT_MSG_WHISPER, LANG_ADDON, player, nullptr, wire.c_str());
    player->SendDirectMessage(&packet);
}

void Notify(Player* player, std::string const& message)
{
    if (NotifyPlayers() && player && player->GetSession() && !IsPlayerbot(player))
        ChatHandler(player->GetSession()).PSendSysMessage("Nourishment: {}", message);
}

void SendProfile(Player* player, NourishmentProfile const& profile)
{
    std::ostringstream payload;
    payload << profile.itemId << kFieldSeparator << PercentEncode(profile.itemName) << kFieldSeparator
            << profile.family << kFieldSeparator << PercentEncode(FamilyLabel(profile.family)) << kFieldSeparator
            << static_cast<uint32>(profile.tier) << kFieldSeparator << static_cast<uint32>(profile.grade)
            << kFieldSeparator << profile.markerSpell << kFieldSeparator << profile.amount1
            << kFieldSeparator << profile.amount2 << kFieldSeparator << profile.amount3
            << kFieldSeparator << ProfileDuration(profile) << kFieldSeparator
            << PercentEncode(EffectText(profile)) << kFieldSeparator << profile.cookedBonusPercent;
    SendAddonPacket(player, "PROFILE", payload.str());
}

void SendState(Player* player)
{
    NourishmentStateLock stateLock(gNourishmentStateMutex);
    SendAddonPacket(player, "STATE", "3");
    auto active = gActiveNourishment.find(GuidLow(player));
    for (uint8 slot = 1; slot <= kNourishmentSlotCount; ++slot)
    {
        std::ostringstream payload;
        payload << static_cast<uint32>(slot);
        ActiveNourishment const* activeSlot = active == gActiveNourishment.end()
            ? nullptr : &active->second[slot - 1];
        if (activeSlot && activeSlot->profile.itemId)
        {
            NourishmentProfile const& profile = activeSlot->profile;
            payload << kFieldSeparator << profile.itemId << kFieldSeparator << PercentEncode(profile.itemName)
                    << kFieldSeparator << profile.family << kFieldSeparator << PercentEncode(FamilyLabel(profile.family))
                    << kFieldSeparator << static_cast<uint32>(profile.tier) << kFieldSeparator
                    << static_cast<uint32>(profile.grade) << kFieldSeparator << SlotAuraSpell(profile, slot)
                    << kFieldSeparator << profile.amount1 << kFieldSeparator << profile.amount2
                    << kFieldSeparator << profile.amount3 << kFieldSeparator << activeSlot->expiresAt
                    << kFieldSeparator << PercentEncode(EffectText(profile)) << kFieldSeparator
                    << profile.cookedBonusPercent;
        }
        else
            payload << kFieldSeparator << "0";
        SendAddonPacket(player, "SLOT", payload.str());
    }
}

void PersistActive(uint32 guid, uint8 slot, ActiveNourishment const& active)
{
    CharacterDatabase.Execute(
        "REPLACE INTO mod_homebrew_nourishment_active (guid, slot, item_id, expires_at) VALUES ({}, {}, {}, {})",
        guid, uint32(slot), active.profile.itemId, active.expiresAt);
}

void DeletePersisted(uint32 guid, uint8 slot)
{
    CharacterDatabase.Execute("DELETE FROM mod_homebrew_nourishment_active WHERE guid = {} AND slot = {}",
        guid, uint32(slot));
}

void DeleteAllPersisted(uint32 guid)
{
    CharacterDatabase.Execute("DELETE FROM mod_homebrew_nourishment_active WHERE guid = {}", guid);
}

void LoadProfiles()
{
    std::unordered_map<uint32, NourishmentProfile> profiles;
    std::unordered_set<uint32> stockAuras;

    QueryResult result = WorldDatabase.Query(
        "SELECT item_id, item_name, source_spell, meal_aura_spell, tier, grade, family, marker_spell, "
        "amount_1, amount_2, amount_3, minimum_level, duration_seconds, recovery_kind, native_well_fed_aura, cooked "
        "FROM mod_homebrew_nourishment_profile WHERE enabled = 1 ORDER BY item_id");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            NourishmentProfile profile;
            profile.itemId = fields[0].Get<uint32>();
            profile.itemName = fields[1].Get<std::string>();
            profile.sourceSpell = fields[2].Get<uint32>();
            profile.mealAuraSpell = fields[3].Get<uint32>();
            profile.tier = fields[4].Get<uint8>();
            profile.grade = fields[5].Get<uint8>();
            profile.family = fields[6].Get<std::string>();
            profile.markerSpell = fields[7].Get<uint32>();
            profile.amount1 = fields[8].Get<int32>();
            profile.amount2 = fields[9].Get<int32>();
            profile.amount3 = fields[10].Get<int32>();
            profile.minimumLevel = fields[11].Get<uint8>();
            profile.durationSeconds = fields[12].Get<uint32>();
            profile.recoveryKind = fields[13].Get<std::string>();
            profile.nativeAura = fields[14].Get<uint32>();
            profile.cooked = fields[15].Get<uint8>() != 0 && profile.recoveryKind == "food";
            if (profile.cooked)
            {
                float multiplier = std::clamp(
                    sConfigMgr->GetOption<float>("HomebrewNourishment.CookedFoodBonusMultiplier", 1.30f), 1.0f, 5.0f);
                auto ApplyCookedBonus = [multiplier](int32 amount)
                {
                    int32 improved = static_cast<int32>(std::lround(amount * multiplier));
                    if (amount > 0 && multiplier > 1.0f && improved <= amount)
                        ++improved;
                    return improved;
                };
                profile.amount1 = ApplyCookedBonus(profile.amount1);
                profile.amount2 = ApplyCookedBonus(profile.amount2);
                profile.amount3 = ApplyCookedBonus(profile.amount3);
                profile.cookedBonusPercent = static_cast<uint32>(std::lround((multiplier - 1.0f) * 100.0f));
            }
            profiles.emplace(profile.itemId, std::move(profile));
        } while (result->NextRow());
    }

    QueryResult auraResult = WorldDatabase.Query(
        "SELECT spell_id FROM mod_homebrew_nourishment_stock_aura ORDER BY spell_id");
    if (auraResult)
    {
        do
            stockAuras.insert(auraResult->Fetch()[0].Get<uint32>());
        while (auraResult->NextRow());
    }

    NourishmentStateLock stateLock(gNourishmentStateMutex);
    gProfiles.swap(profiles);
    gStockAuras.swap(stockAuras);
    LOG_INFO("server.loading", "mod-homebrew-nourishment loaded {} item profiles and {} stock Well Fed auras.",
        gProfiles.size(), gStockAuras.size());
}

bool ApplyActiveAura(Player* player, ActiveNourishment& active)
{
    NourishmentStateLock stateLock(gNourishmentStateMutex);
    if (!player || !player->IsAlive())
        return false;

    for (uint32 spellId : gStockAuras)
        if (player->HasAura(spellId))
            player->RemoveAurasDueToSpell(spellId);

    uint32 auraSpell = SlotAuraSpell(active.profile, active.slot);
    if (!auraSpell)
        return false;

    active.castingMarker = true;
    int32 amount1 = active.profile.amount1;
    int32 amount2 = active.profile.amount2;
    int32 amount3 = active.profile.amount3;
    player->CastCustomSpell(player, auraSpell, &amount1, &amount2, &amount3, true);
    active.castingMarker = false;

    Aura* aura = player->GetAura(auraSpell);
    if (!aura)
    {
        LOG_ERROR("module", "mod-homebrew-nourishment failed to apply marker spell {} to {}.",
            auraSpell, player->GetName());
        return false;
    }

    uint64 now = static_cast<uint64>(std::time(nullptr));
    uint64 seconds = active.expiresAt > now ? active.expiresAt - now : 1;
    int32 duration = static_cast<int32>(std::min<uint64>(seconds, static_cast<uint64>(INT32_MAX / 1000)) * 1000);
    aura->SetMaxDuration(duration);
    aura->SetDuration(duration);
    active.needsRepair = false;
    return true;
}

void ClearActive(Player* player, bool deleteDatabase, bool sendState)
{
    NourishmentStateLock stateLock(gNourishmentStateMutex);
    uint32 guid = GuidLow(player);
    auto active = gActiveNourishment.find(guid);
    if (active != gActiveNourishment.end())
    {
        for (ActiveNourishment& slot : active->second)
        {
            if (!slot.profile.itemId)
                continue;
            slot.suppressRemoval = true;
            uint32 auraSpell = SlotAuraSpell(slot.profile, slot.slot);
            if (player && auraSpell && player->HasAura(auraSpell))
                player->RemoveAurasDueToSpell(auraSpell);
        }
        gActiveNourishment.erase(active);
    }
    if (deleteDatabase)
        DeleteAllPersisted(guid);
    if (sendState)
        SendState(player);
}

void ClearActiveSlot(Player* player, uint32 guid, ActiveNourishmentSlots& slots, uint8 slotIndex,
    bool deleteDatabase)
{
    if (slotIndex >= kNourishmentSlotCount)
        return;
    ActiveNourishment& active = slots[slotIndex];
    if (active.profile.itemId)
    {
        active.suppressRemoval = true;
        uint32 auraSpell = SlotAuraSpell(active.profile, active.slot);
        if (player && auraSpell && player->HasAura(auraSpell))
            player->RemoveAurasDueToSpell(auraSpell);
        active = {};
    }
    if (deleteDatabase)
        DeletePersisted(guid, slotIndex + 1);
}

bool GrantNourishment(Player* player, NourishmentProfile const& profile, bool bypassLevel = false)
{
    NourishmentStateLock stateLock(gNourishmentStateMutex);
    if (!player)
        return false;
    if (!bypassLevel && player->GetLevel() < profile.minimumLevel)
    {
        Notify(player, "this meal's lasting benefit requires level " + std::to_string(profile.minimumLevel) + ".");
        return false;
    }
    if ((!AllowInArenas() && player->InArena()) ||
        (!AllowInBattlegrounds() && player->InBattleground()))
    {
        Notify(player, "lasting meal benefits are disabled here.");
        return false;
    }

    uint32 guid = GuidLow(player);
    uint64 now = static_cast<uint64>(std::time(nullptr));
    auto existing = gActiveNourishment.find(guid);
    ActiveNourishmentSlots emptySlots{};
    ActiveNourishmentSlots const& currentSlots = existing == gActiveNourishment.end()
        ? emptySlots : existing->second;
    // A recipe family represents one buff. Different food with the same effect cannot stack it.
    for (ActiveNourishment const& active : currentSlots)
        if (active.profile.itemId && active.expiresAt > now && active.profile.family == profile.family)
            return false;
    std::array<NourishmentSlotEntry, kNourishmentSlotCount> selectionSlots{};
    for (std::size_t i = 0; i < currentSlots.size(); ++i)
        if (currentSlots[i].profile.itemId)
            selectionSlots[i] = { currentSlots[i].profile.itemId, currentSlots[i].expiresAt };

    NourishmentSlotDecision const decision = SelectNourishmentSlot(selectionSlots, profile.itemId, now);
    if (decision.index >= kNourishmentSlotCount)
        return false;

    uint8 slotNumber = static_cast<uint8>(decision.index + 1);
    ActiveNourishment replacement;
    replacement.profile = profile;
    replacement.expiresAt = now + ProfileDuration(profile);
    replacement.slot = slotNumber;
    if (!ApplyActiveAura(player, replacement))
        return false;

    ActiveNourishmentSlots& slots = gActiveNourishment[guid];
    ActiveNourishment& previous = slots[decision.index];
    uint32 previousAura = previous.profile.itemId ? SlotAuraSpell(previous.profile, previous.slot) : 0;
    uint32 replacementAura = SlotAuraSpell(replacement.profile, replacement.slot);
    if (previous.profile.itemId && previousAura != replacementAura)
    {
        previous.suppressRemoval = true;
        if (previousAura && player->HasAura(previousAura))
            player->RemoveAurasDueToSpell(previousAura);
    }
    previous = std::move(replacement);

    PersistActive(guid, slotNumber, previous);
    SendState(player);
    Notify(player, FamilyLabel(profile.family) + " tier " + std::to_string(profile.tier) +
        (profile.grade ? " premium in meal slot " : " in meal slot ") + std::to_string(slotNumber) +
        ": " + EffectText(profile) + ".");
    LOG_INFO("module", "Nourishment grant player={} item={} family={} tier={} grade={} expires={}",
        player->GetName(), profile.itemId, profile.family, profile.tier, profile.grade, previous.expiresAt);
    return true;
}

void RestoreActive(Player* player)
{
    NourishmentStateLock stateLock(gNourishmentStateMutex);
    if (!player)
        return;
    uint32 guid = GuidLow(player);
    QueryResult result = CharacterDatabase.Query(
        "SELECT slot, item_id, expires_at FROM mod_homebrew_nourishment_active WHERE guid = {} ORDER BY slot", guid);
    if (!result)
    {
        gActiveNourishment.erase(guid);
        SendState(player);
        return;
    }

    uint64 now = static_cast<uint64>(std::time(nullptr));
    ActiveNourishmentSlots& slots = gActiveNourishment[guid];
    slots = {};
    do
    {
        Field* fields = result->Fetch();
        uint32 slot = fields[0].Get<uint32>();
        uint32 itemId = fields[1].Get<uint32>();
        uint64 expiresAt = fields[2].Get<uint64>();
        auto profile = gProfiles.find(itemId);
        if (slot < 1 || slot > kNourishmentSlotCount || profile == gProfiles.end() || expiresAt <= now ||
            (!AllowInArenas() && player->InArena()) || (!AllowInBattlegrounds() && player->InBattleground()))
        {
            if (slot >= 1 && slot <= kNourishmentSlotCount)
                DeletePersisted(guid, static_cast<uint8>(slot));
            continue;
        }

        ActiveNourishment& active = slots[slot - 1];
        active.profile = profile->second;
        active.expiresAt = expiresAt;
        active.slot = static_cast<uint8>(slot);
        if (!ApplyActiveAura(player, active))
            active.needsRepair = true;
    } while (result->NextRow());
    SendState(player);
}

bool ExtractAddonPayload(uint32 language, std::string const& message, std::string& payload)
{
    if (language != LANG_ADDON)
        return false;
    payload = message;
    if (payload.rfind(kAddonPrefix, 0) == 0)
    {
        payload.erase(0, sizeof(kAddonPrefix) - 1);
        while (!payload.empty() && (payload.front() == '\t' || payload.front() == ' '))
            payload.erase(payload.begin());
    }
    return !payload.empty();
}

std::pair<std::string, std::string> SplitOnce(std::string const& value)
{
    size_t position = value.find(kFieldSeparator);
    if (position == std::string::npos)
        return {value, ""};
    return {value.substr(0, position), value.substr(position + 1)};
}

class HomebrewNourishmentWorldScript final : public WorldScript
{
public:
    HomebrewNourishmentWorldScript() : WorldScript("HomebrewNourishmentWorldScript", {
        WORLDHOOK_ON_AFTER_CONFIG_LOAD,
        WORLDHOOK_ON_STARTUP
    }) { }

    void OnAfterConfigLoad(bool reload) override
    {
        if (reload && IsEnabled())
            LoadProfiles();
    }

    void OnStartup() override
    {
        if (IsEnabled())
            LoadProfiles();
    }
};

class HomebrewNourishmentPlayerScript final : public PlayerScript
{
public:
    HomebrewNourishmentPlayerScript() : PlayerScript("HomebrewNourishmentPlayerScript") { }

    void OnPlayerSpellCast(Player* player, Spell* spell, bool /*skipCheck*/) override
    {
        if (!IsEnabled() || !player || !spell || !spell->m_CastItem)
            return;
        if (!IncludePlayerbots() && IsPlayerbot(player))
            return;

        NourishmentStateLock stateLock(gNourishmentStateMutex);
        uint32 itemId = spell->m_CastItem->GetEntry();
        auto profile = gProfiles.find(itemId);
        if (profile == gProfiles.end() || spell->GetSpellInfo()->Id != profile->second.sourceSpell)
            return;

        PendingMeal pending;
        pending.itemId = itemId;
        pending.itemCountBefore = player->GetItemCount(itemId, false);
        pending.mealAuraSpell = profile->second.mealAuraSpell;
        pending.mapId = player->GetMapId();
        pending.x = player->GetPositionX();
        pending.y = player->GetPositionY();
        pending.z = player->GetPositionZ();
        pending.started = std::chrono::steady_clock::now();
        gPendingMeals[GuidLow(player)] = pending;
    }

    void OnPlayerUpdate(Player* player, uint32 /*diff*/) override
    {
        if (!IsEnabled() || !player)
            return;
        NourishmentStateLock stateLock(gNourishmentStateMutex);
        uint32 guid = GuidLow(player);
        uint64 nowSeconds = static_cast<uint64>(std::time(nullptr));

        auto active = gActiveNourishment.find(guid);
        if (active != gActiveNourishment.end())
        {
            if ((!AllowInArenas() && player->InArena()) ||
                (!AllowInBattlegrounds() && player->InBattleground()))
            {
                ClearActive(player, true, true);
            }
            else
            {
                bool changed = false;
                for (std::size_t i = 0; i < active->second.size(); ++i)
                {
                    ActiveNourishment& slot = active->second[i];
                    if (!slot.profile.itemId)
                        continue;
                    if (slot.expiresAt <= nowSeconds)
                    {
                        ClearActiveSlot(player, guid, active->second, static_cast<uint8>(i), true);
                        changed = true;
                        continue;
                    }
                    uint32 auraSpell = SlotAuraSpell(slot.profile, slot.slot);
                    if (player->IsAlive() && (slot.needsRepair || !player->HasAura(auraSpell)))
                    {
                        ApplyActiveAura(player, slot);
                        changed = true;
                    }
                }
                if (changed)
                    SendState(player);
            }
        }

        auto pending = gPendingMeals.find(guid);
        if (pending == gPendingMeals.end())
            return;

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - pending->second.started).count();
        if (!player->IsAlive() || player->IsInCombat() ||
            player->GetMapId() != pending->second.mapId)
        {
            gPendingMeals.erase(pending);
            return;
        }

        if (player->GetItemCount(pending->second.itemId, false) >= pending->second.itemCountBefore)
        {
            if (elapsed >= 15000)
                gPendingMeals.erase(pending);
            return;
        }

        uint32 itemId = pending->second.itemId;
        gPendingMeals.erase(pending);
        auto profile = gProfiles.find(itemId);
        if (profile != gProfiles.end())
            GrantNourishment(player, profile->second);
    }

    void OnPlayerLogin(Player* player) override
    {
        if (IsEnabled())
            RestoreActive(player);
    }

    void OnPlayerBeforeLogout(Player* player) override
    {
        NourishmentStateLock stateLock(gNourishmentStateMutex);
        uint32 guid = GuidLow(player);
        gPendingMeals.erase(guid);
        gActiveNourishment.erase(guid); // database remains authoritative while offline
    }

    void OnPlayerJustDied(Player* player) override
    {
        NourishmentStateLock stateLock(gNourishmentStateMutex);
        gPendingMeals.erase(GuidLow(player));
        if (!PersistThroughDeath())
            ClearActive(player, true, true);
    }

    void OnPlayerResurrect(Player* player, float /*restorePercent*/, bool& /*applySickness*/) override
    {
        NourishmentStateLock stateLock(gNourishmentStateMutex);
        auto active = gActiveNourishment.find(GuidLow(player));
        if (active != gActiveNourishment.end())
            for (ActiveNourishment& slot : active->second)
                if (slot.profile.itemId)
                    slot.needsRepair = true;
    }

    void OnPlayerJoinArena(Player* player) override
    {
        if (!AllowInArenas())
            ClearActive(player, true, true);
    }

    void OnPlayerDelete(ObjectGuid guid, uint32 /*accountId*/) override
    {
        NourishmentStateLock stateLock(gNourishmentStateMutex);
        uint32 counter = guid.GetCounter();
        gPendingMeals.erase(counter);
        gActiveNourishment.erase(counter);
        DeleteAllPersisted(counter);
    }

    bool TryHandleAddon(Player* player, uint32 language, std::string& message)
    {
        if (!IsEnabled() || !player)
            return false;
        std::string payload;
        if (!ExtractAddonPayload(language, message, payload))
            return false;
        NourishmentStateLock stateLock(gNourishmentStateMutex);
        auto packet = SplitOnce(payload);
        std::transform(packet.first.begin(), packet.first.end(), packet.first.begin(), [](unsigned char character)
        {
            return static_cast<char>(std::toupper(character));
        });
        if (packet.first == "HELLO")
        {
            SendAddonPacket(player, "HELLO_ACK",
                std::string(kProtocolVersion) + kFieldSeparator + std::to_string(0u));
            SendState(player);
            return true;
        }
        if (packet.first == "STATUS")
        {
            SendState(player);
            return true;
        }
        if (packet.first == "QUERY")
        {
            uint32 itemId = static_cast<uint32>(std::strtoul(packet.second.c_str(), nullptr, 10));
            auto profile = gProfiles.find(itemId);
            if (profile != gProfiles.end())
                SendProfile(player, profile->second);
            else
                SendAddonPacket(player, "PROFILE", std::to_string(itemId) + "~0");
            return true;
        }
        return false;
    }

    bool OnPlayerCanUseChat(Player* player, uint32 /*type*/, uint32 language, std::string& message, Player* /*receiver*/) override
    {
        return !TryHandleAddon(player, language, message);
    }
    bool OnPlayerCanUseChat(Player* player, uint32 /*type*/, uint32 language, std::string& message, Group* /*group*/) override
    {
        return !TryHandleAddon(player, language, message);
    }
    bool OnPlayerCanUseChat(Player* player, uint32 /*type*/, uint32 language, std::string& message, Guild* /*guild*/) override
    {
        return !TryHandleAddon(player, language, message);
    }
    bool OnPlayerCanUseChat(Player* player, uint32 /*type*/, uint32 language, std::string& message, Channel* /*channel*/) override
    {
        return !TryHandleAddon(player, language, message);
    }
};

class HomebrewNourishmentUnitScript final : public UnitScript
{
public:
    HomebrewNourishmentUnitScript() : UnitScript("HomebrewNourishmentUnitScript", true, {
        UNITHOOK_ON_AURA_APPLY,
        UNITHOOK_ON_AURA_REMOVE
    }) { }

    void OnAuraApply(Unit* unit, Aura* aura) override
    {
        Player* player = unit ? unit->ToPlayer() : nullptr;
        if (!player || !aura)
            return;
        NourishmentStateLock stateLock(gNourishmentStateMutex);
        if (gStockAuras.find(aura->GetId()) == gStockAuras.end())
            return;
        auto active = gActiveNourishment.find(GuidLow(player));
        if (active != gActiveNourishment.end())
            for (ActiveNourishment& slot : active->second)
                if (slot.profile.itemId && !slot.castingMarker)
                    slot.needsRepair = true;
    }

    void OnAuraRemove(Unit* unit, AuraApplication* application, AuraRemoveMode mode) override
    {
        Player* player = unit ? unit->ToPlayer() : nullptr;
        if (!player || !application || !application->GetBase())
            return;
        NourishmentStateLock stateLock(gNourishmentStateMutex);
        uint32 guid = GuidLow(player);
        auto active = gActiveNourishment.find(guid);
        if (active == gActiveNourishment.end())
            return;
        uint32 removedSpell = application->GetBase()->GetId();
        for (std::size_t i = 0; i < active->second.size(); ++i)
        {
            ActiveNourishment& slot = active->second[i];
            if (!slot.profile.itemId || SlotAuraSpell(slot.profile, slot.slot) != removedSpell)
                continue;
            if (slot.suppressRemoval || slot.castingMarker)
                return;
            if (mode == AURA_REMOVE_BY_DEATH && PersistThroughDeath())
            {
                slot.needsRepair = true;
                return;
            }

            ClearActiveSlot(player, guid, active->second, static_cast<uint8>(i), true);
            SendState(player);
            return;
        }
    }
};

class HomebrewNourishmentCommands final : public CommandScript
{
public:
    HomebrewNourishmentCommands() : CommandScript("HomebrewNourishmentCommands") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable nourishmentCommands =
        {
            { "status", HandleStatus, rbac::RBAC_PERM_COMMAND_GM, Console::No },
            { "profile", HandleProfile, rbac::RBAC_PERM_COMMAND_GM, Console::No },
            { "grant", HandleGrant, rbac::RBAC_PERM_COMMAND_GM, Console::No },
            { "clear", HandleClear, rbac::RBAC_PERM_COMMAND_GM, Console::No },
            { "reload", HandleReload, rbac::RBAC_PERM_COMMAND_RELOAD_CONFIG, Console::No }
        };
        static ChatCommandTable commands =
        {
            { "nourishment", nourishmentCommands }
        };
        return commands;
    }

    static bool HandleStatus(ChatHandler* handler)
    {
        Player* player = handler->getSelectedPlayerOrSelf();
        if (!player)
            return false;
        NourishmentStateLock stateLock(gNourishmentStateMutex);
        auto active = gActiveNourishment.find(GuidLow(player));
        if (active == gActiveNourishment.end())
        {
            handler->SendSysMessage("No active Homebrew nourishment.");
            return true;
        }
        std::ostringstream status;
        bool any = false;
        status << player->GetName() << " nourishment:";
        for (std::size_t i = 0; i < active->second.size(); ++i)
        {
            ActiveNourishment const& slot = active->second[i];
            if (!slot.profile.itemId)
                continue;
            any = true;
            status << " slot " << (i + 1) << ": " << FamilyLabel(slot.profile.family)
                   << " tier " << static_cast<uint32>(slot.profile.tier)
                   << (slot.profile.grade ? " premium, " : " ordinary, ")
                   << EffectText(slot.profile) << ", expires at Unix " << slot.expiresAt << ";";
        }
        if (!any)
            status << " none active.";
        handler->SendSysMessage(status.str());
        return true;
    }

    static bool HandleProfile(ChatHandler* handler, uint32 itemId)
    {
        NourishmentStateLock stateLock(gNourishmentStateMutex);
        auto profile = gProfiles.find(itemId);
        if (profile == gProfiles.end())
        {
            handler->PSendSysMessage("Item {} has no nourishment profile.", itemId);
            return true;
        }
        std::ostringstream text;
        text << profile->second.itemName << " (" << itemId << "): "
             << FamilyLabel(profile->second.family) << " tier " << static_cast<uint32>(profile->second.tier)
             << (profile->second.grade ? " premium, " : " ordinary, ")
             << EffectText(profile->second) << ", min level " << static_cast<uint32>(profile->second.minimumLevel)
             << ", duration " << ProfileDuration(profile->second) << " sec, source "
             << profile->second.sourceSpell << ", meal aura " << profile->second.mealAuraSpell << ".";
        handler->SendSysMessage(text.str());
        return true;
    }

    static bool HandleGrant(ChatHandler* handler, uint32 itemId)
    {
        Player* player = handler->getSelectedPlayerOrSelf();
        NourishmentStateLock stateLock(gNourishmentStateMutex);
        auto profile = gProfiles.find(itemId);
        if (!player || profile == gProfiles.end())
        {
            handler->SendSysMessage("Select an online player and provide a profiled item ID.");
            return true;
        }
        bool granted = GrantNourishment(player, profile->second, true);
        handler->PSendSysMessage("Nourishment grant for {}: {}.", player->GetName(), granted ? "applied" : "blocked");
        return true;
    }

    static bool HandleClear(ChatHandler* handler)
    {
        Player* player = handler->getSelectedPlayerOrSelf();
        if (!player)
            return false;
        ClearActive(player, true, true);
        handler->PSendSysMessage("Cleared Homebrew nourishment for {}.", player->GetName());
        return true;
    }

    static bool HandleReload(ChatHandler* handler)
    {
        LoadProfiles();
        NourishmentStateLock stateLock(gNourishmentStateMutex);
        handler->PSendSysMessage("Reloaded {} nourishment profiles and {} stock aura IDs.",
            gProfiles.size(), gStockAuras.size());
        return true;
    }
};
} // namespace

void AddSC_homebrew_nourishment()
{
    if (!IsEnabled())
        return;

    new HomebrewNourishmentWorldScript();
    new HomebrewNourishmentPlayerScript();
    new HomebrewNourishmentUnitScript();
    new HomebrewNourishmentCommands();
    LOG_INFO("server.loading", "mod-homebrew-nourishment registered (protocol {}).", kProtocolVersion);
}
