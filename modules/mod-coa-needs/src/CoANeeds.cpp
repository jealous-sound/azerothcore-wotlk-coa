// SPDX-License-Identifier: GPL-2.0-or-later

#include "Chat.h"
#include "Config.h"
#include "DatabaseEnv.h"
#include "Item.h"
#include "Log.h"
#include "NeedsChallengeBridge.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <ctime>
#include <mutex>
#include <sstream>
#include <unordered_map>

namespace
{
constexpr char Storage[] = "core.coa.needs";
constexpr uint32 Starving = 996009;
constexpr uint32 Dehydrated = 996010;
constexpr uint32 Exhausted = 996011;
constexpr uint32 SprintAura = 996012;
constexpr uint32 DodgeAura = 996013;
constexpr uint32 WindAura = 996014;
constexpr uint32 SprintAbility = 996015;
constexpr uint32 DodgeAbility = 996016;
constexpr uint32 WindAbility = 996017;

struct Options
{
    bool enabled = false;
    bool bots = false;
    float foodDrain = 4.5f;
    float waterDrain = 6.0f;
    float meal = 45.0f;
    float drink = 55.0f;
    float regen = 8.0f;
    float sprintDrain = 10.0f;
};

struct PendingMeal
{
    uint32 item = 0;
    uint32 count = 0;
    uint32 spell = 0;
    uint32 level = 0;
    uint32 elapsed = 0;
    bool food = false;
    bool water = false;
};

struct State
{
    float food = 100.0f;
    float water = 100.0f;
    float vigor = 100.0f;
    float displayFood = 100.0f;
    float displayWater = 100.0f;
    float cap = 100.0f;
    uint32 dodgeUntil = 0;
    uint32 windUntil = 0;
    uint32 exhaustedUntil = 0;
    uint32 attackLockUntil = 0;
    uint32 windBuffUntil = 0;
    uint32 tick = 0;
    uint32 dehydrationTick = 0;
    uint32 saveTick = 0;
    uint64 lastSpend = 0;
    uint64 lastAttack = 0;
    uint64 lastCombat = 0;
    bool sprint = false;
    bool delegated = false;
    PendingMeal meal;
};

Options Config;
bool SpellsReady = false;
std::recursive_mutex Mutex;
std::unordered_map<ObjectGuid::LowType, State> States;

uint64 Milliseconds()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();
}

bool Affects(Player const* player)
{
    return player && Config.enabled && SpellsReady && player->GetSession() &&
        (Config.bots || !player->GetSession()->IsBot());
}

float Clamp(float value)
{
    return std::clamp(value, 0.0f, 100.0f);
}

void SetAura(Player* player, uint32 spell, bool active)
{
    if (!active)
        player->RemoveAurasDueToSpell(spell);
    else if (!player->HasAura(spell))
        player->AddAura(spell, player);
}

void StopSprint(Player* player, State& state)
{
    state.sprint = false;
    player->RemoveAurasDueToSpell(SprintAura);
}

void Save(Player* player, State const& state)
{
    std::array<uint32, 9> values = { 1, uint32(Clamp(state.food) * 1000.0f),
        uint32(Clamp(state.water) * 1000.0f), uint32(Clamp(state.vigor) * 1000.0f),
        state.dodgeUntil, state.windUntil, state.exhaustedUntil, state.attackLockUntil, state.windBuffUntil };
    for (uint32 index = 0; index < values.size(); ++index)
        player->UpdatePlayerSetting(Storage, index, values[index]);
}

void Send(Player* player, State const& state)
{
    if (player->GetSession()->IsBot())
        return;
    uint32 now = uint32(std::time(nullptr));
    auto remaining = [now](uint32 until) { return until > now ? until - now : 0; };
    std::ostringstream wire;
    wire << "HXN\tSTATE~" << uint32(state.displayFood) << '~' << uint32(state.displayWater)
         << '~' << uint32(state.vigor) << '~' << uint32(state.cap) << '~' << state.sprint
         << '~' << remaining(state.dodgeUntil) << '~' << remaining(state.windUntil)
         << '~' << remaining(state.exhaustedUntil) << '~' << state.delegated;
    WorldPacket packet;
    ChatHandler::BuildChatPacket(packet, CHAT_MSG_WHISPER, LANG_ADDON, player, nullptr, wire.str());
    player->SendDirectMessage(&packet);
}

void Unlock(Player* player)
{
    if (!Affects(player))
        return;
    player->learnSpell(SprintAbility);
    if (player->GetLevel() >= 30)
        player->learnSpell(DodgeAbility);
    if (player->GetLevel() >= 60)
        player->learnSpell(WindAbility);
}

void UpdateNeeds(Player* player, State& state)
{
    state.displayFood = state.food;
    state.displayWater = state.water;
    state.delegated = CoANeeds::ReadChallengeNeeds &&
        CoANeeds::ReadChallengeNeeds(player->GetGUID().GetCounter(), state.displayFood, state.displayWater);
    state.cap = 100.0f * (0.5f + state.displayFood / 200.0f) * (0.5f + state.displayWater / 200.0f);
    state.vigor = std::min(state.vigor, state.cap);
}

void Exhaust(Player* player, State& state)
{
    StopSprint(player, state);
    uint32 now = uint32(std::time(nullptr));
    state.exhaustedUntil = now + 15;
    state.attackLockUntil = now + 5;
}

class NeedsWorld : public WorldScript
{
public:
    NeedsWorld() : WorldScript("CoANeedsWorld", { WORLDHOOK_ON_AFTER_CONFIG_LOAD, WORLDHOOK_ON_STARTUP }) { }

    void OnAfterConfigLoad(bool) override
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        Config.enabled = sConfigMgr->GetOption<bool>("CoANeeds.Enable", true);
        Config.bots = sConfigMgr->GetOption<bool>("CoANeeds.IncludeBots", false);
        Config.foodDrain = std::max(0.0f, sConfigMgr->GetOption<float>("CoANeeds.HungerDrainPerMinute", 4.5f));
        Config.waterDrain = std::max(0.0f, sConfigMgr->GetOption<float>("CoANeeds.HydrationDrainPerMinute", 6.0f));
        Config.meal = std::max(0.0f, sConfigMgr->GetOption<float>("CoANeeds.MealFill", 45.0f));
        Config.drink = std::max(0.0f, sConfigMgr->GetOption<float>("CoANeeds.DrinkFill", 55.0f));
        Config.regen = std::max(0.0f, sConfigMgr->GetOption<float>("CoANeeds.VigorRegenPerSecond", 8.0f));
        Config.sprintDrain = std::max(0.0f, sConfigMgr->GetOption<float>("CoANeeds.SprintDrainPerSecond", 10.0f));
    }

    void OnStartup() override
    {
        SpellsReady = true;
        for (uint32 id = Starving; id <= WindAbility; ++id)
            if (!sSpellMgr->GetSpellInfo(id))
            {
                SpellsReady = false;
                LOG_ERROR("module.coa_needs", "Missing client/server needs spell {}; module disabled", id);
            }
        LOG_INFO("module.coa_needs", "Hunger, hydration and vigor: enabled={}, spells ready={}, include bots={}",
            Config.enabled, SpellsReady, Config.bots);
    }
};

class NeedsPlayer : public PlayerScript
{
public:
    NeedsPlayer() : PlayerScript("CoANeedsPlayer", { PLAYERHOOK_ON_LOGIN, PLAYERHOOK_ON_BEFORE_LOGOUT,
        PLAYERHOOK_ON_SAVE, PLAYERHOOK_ON_UPDATE, PLAYERHOOK_ON_SPELL_CAST, PLAYERHOOK_ON_LEVEL_CHANGED,
        PLAYERHOOK_ON_PLAYER_ENTER_COMBAT, PLAYERHOOK_CAN_PLAYER_USE_PRIVATE_CHAT }) { }

    void OnPlayerLogin(Player* player) override
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        if (!Affects(player))
            return;
        State state;
        auto const* saved = player->FindPlayerSettings(Storage);
        if (saved && saved->size() >= 9 && (*saved)[0].value == 1)
        {
            state.food = Clamp((*saved)[1].value / 1000.0f);
            state.water = Clamp((*saved)[2].value / 1000.0f);
            state.vigor = Clamp((*saved)[3].value / 1000.0f);
            state.dodgeUntil = (*saved)[4].value;
            state.windUntil = (*saved)[5].value;
            state.exhaustedUntil = (*saved)[6].value;
            state.attackLockUntil = (*saved)[7].value;
            state.windBuffUntil = (*saved)[8].value;
        }
        StopSprint(player, state);
        UpdateNeeds(player, state);
        States[player->GetGUID().GetCounter()] = state;
        Unlock(player);
        Send(player, state);
    }

    void OnPlayerLevelChanged(Player* player, uint8) override
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        Unlock(player);
    }

    void OnPlayerSave(Player* player) override
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        auto const found = States.find(player->GetGUID().GetCounter());
        if (found != States.end())
            Save(player, found->second);
    }

    void OnPlayerBeforeLogout(Player* player) override
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        auto found = States.find(player->GetGUID().GetCounter());
        if (found == States.end())
            return;
        StopSprint(player, found->second);
        Save(player, found->second);
        States.erase(found);
    }

    void OnPlayerEnterCombat(Player* player, Unit*) override
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        auto found = States.find(player->GetGUID().GetCounter());
        if (!Affects(player) || found == States.end())
            return;
        uint64 now = Milliseconds();
        if (now - found->second.lastCombat >= 10000)
        {
            found->second.vigor = std::max(0.0f, found->second.vigor - 8.0f);
            found->second.lastSpend = now;
        }
        found->second.lastCombat = now;
    }

    void OnPlayerSpellCast(Player* player, Spell* spell, bool) override
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        auto found = States.find(player->GetGUID().GetCounter());
        if (!Affects(player) || !spell || found == States.end())
            return;
        State& state = found->second;
        uint32 id = spell->GetSpellInfo()->Id;
        uint32 now = uint32(std::time(nullptr));
        if (id >= SprintAbility && id <= WindAbility)
        {
            UpdateNeeds(player, state);
            if (!player->IsAlive() || player->IsMounted() || player->IsInFlight() || player->IsSitState() ||
                player->HasUnitState(UNIT_STATE_CONTROLLED))
                return;
            if (id == SprintAbility)
            {
                if (state.sprint)
                {
                    StopSprint(player, state);
                    if (state.vigor <= 15.0f)
                        Exhaust(player, state);
                }
                else if (state.vigor > 25.0f && state.displayFood > 5.0f && state.exhaustedUntil <= now &&
                    !player->InBattleground() && !player->InArena())
                {
                    state.vigor -= 10.0f;
                    state.sprint = true;
                    state.lastSpend = Milliseconds();
                    SetAura(player, SprintAura, true);
                }
            }
            else if (id == DodgeAbility && player->GetLevel() >= 30 && state.vigor >= 25.0f &&
                state.dodgeUntil <= now && state.attackLockUntil <= now)
            {
                state.vigor -= 25.0f;
                state.lastSpend = Milliseconds();
                state.dodgeUntil = now + 20;
                if (!state.delegated)
                {
                    state.food = Clamp(state.food - 0.25f);
                    state.water = Clamp(state.water - 0.5f);
                }
                if (Aura* aura = player->AddAura(DodgeAura, player))
                {
                    aura->SetMaxDuration(3000);
                    aura->SetDuration(3000);
                }
            }
            else if (id == WindAbility && player->GetLevel() >= 60 && state.windUntil <= now)
            {
                state.vigor = std::min(state.cap, state.vigor + 25.0f);
                state.windUntil = now + 120;
                state.windBuffUntil = now + 6;
            }
            Save(player, state);
            Send(player, state);
            return;
        }
        if (!spell->m_CastItem)
            return;
        uint32 category = spell->GetSpellInfo()->GetCategory();
        if (category != SPELL_CATEGORY_FOOD && category != SPELL_CATEGORY_DRINK)
            return;
        PendingMeal meal;
        meal.item = spell->m_CastItem->GetEntry();
        meal.count = player->GetItemCount(meal.item, false);
        meal.spell = id;
        meal.level = spell->m_CastItem->GetTemplate()->ItemLevel;
        meal.food = category == SPELL_CATEGORY_FOOD;
        meal.water = category == SPELL_CATEGORY_DRINK || spell->GetSpellInfo()->HasAura(SPELL_AURA_MOD_POWER_REGEN);
        state.meal = meal;
    }

    void OnPlayerUpdate(Player* player, uint32 diff) override
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        auto found = States.find(player->GetGUID().GetCounter());
        if (found == States.end())
            return;
        State& state = found->second;
        if (!Affects(player))
        {
            StopSprint(player, state);
            for (uint32 id : { Starving, Dehydrated, Exhausted, WindAura })
                player->RemoveAurasDueToSpell(id);
            return;
        }
        UpdateNeeds(player, state);
        if (state.meal.item)
        {
            state.meal.elapsed += diff;
            if (!player->IsAlive() || player->IsInCombat() || player->isMoving() || state.meal.elapsed > 3000)
                state.meal = {};
            else if (player->GetItemCount(state.meal.item, false) < state.meal.count)
            {
                if (!state.delegated && player->HasAura(state.meal.spell))
                {
                    if (state.meal.food)
                        state.food = Clamp(state.food + Config.meal + player->GetSkillValue(SKILL_COOKING) / 90.0f);
                    if (state.meal.water)
                        state.water = Clamp(state.water + Config.drink + (state.meal.level >= 60 ? 20.0f :
                            state.meal.level >= 30 ? 10.0f : 0.0f));
                }
                state.meal = {};
            }
        }
        state.tick += diff;
        if (state.tick < 1000)
            return;
        uint32 elapsed = state.tick;
        float seconds = elapsed / 1000.0f;
        state.tick = 0;
        uint32 now = uint32(std::time(nullptr));
        uint64 steady = Milliseconds();
        bool alive = player->IsAlive();
        bool pvp = player->InBattleground() || player->InArena();
        if (alive && !pvp && !state.delegated)
        {
            float cooking = player->GetSkillValue(SKILL_COOKING) / 450.0f;
            state.food = Clamp(state.food - Config.foodDrain * (1.0f - 0.05f * cooking) * seconds / 60.0f);
            state.water = Clamp(state.water - Config.waterDrain * seconds / 60.0f);
        }
        UpdateNeeds(player, state);
        if (!alive || player->IsMounted() || player->IsInFlight() || player->IsSitState() || pvp ||
            state.displayFood <= 5.0f || player->HasUnitState(UNIT_STATE_CONTROLLED))
            StopSprint(player, state);
        if (state.exhaustedUntil > now && player->IsInWater())
        {
            state.exhaustedUntil = now;
            state.attackLockUntil = now;
        }
        if (alive && state.sprint && player->isMoving())
        {
            state.vigor = std::max(0.0f, state.vigor - Config.sprintDrain * seconds);
            state.lastSpend = steady;
            if (!state.delegated)
            {
                state.food = Clamp(state.food - 2.0f * seconds / 60.0f);
                state.water = Clamp(state.water - 3.0f * seconds / 60.0f);
            }
            if (state.vigor <= 15.0f)
                Exhaust(player, state);
        }
        else if (alive && player->IsInWater() && player->isMoving())
        {
            state.vigor = std::max(0.0f, state.vigor - 4.0f * seconds);
            state.lastSpend = steady;
        }
        else if (alive && steady - state.lastSpend >= 1500)
        {
            float regen = Config.regen + 4.0f * player->GetSkillValue(SKILL_ENGINEERING) / 450.0f;
            if (state.displayFood <= 35.0f)
                regen *= 0.65f;
            if (state.displayWater <= 30.0f)
                regen *= 0.65f;
            if (player->IsInCombat())
                regen *= 0.25f;
            else if (player->IsSitState())
                regen *= 2.0f;
            if (state.windBuffUntil > now)
                regen *= 2.5f;
            state.vigor = std::min(state.cap, state.vigor + regen * seconds);
        }
        SetAura(player, SprintAura, alive && state.sprint);
        SetAura(player, Starving, alive && !state.delegated && state.displayFood <= 5.0f);
        SetAura(player, Dehydrated, alive && !state.delegated && state.displayWater <= 30.0f);
        SetAura(player, Exhausted, alive && state.exhaustedUntil > now);
        SetAura(player, WindAura, alive && state.windBuffUntil > now);
        state.dehydrationTick += elapsed;
        if (state.dehydrationTick >= 10000)
        {
            state.dehydrationTick = 0;
            if (alive && !pvp && !state.delegated && state.displayWater <= 10.0f &&
                !player->HasPlayerFlag(PLAYER_FLAGS_RESTING))
            {
                uint32 damage = std::max(1u, uint32(player->GetMaxHealth() * 0.02f));
                player->EnvironmentalDamage(DAMAGE_EXHAUSTED, damage);
            }
        }
        UpdateNeeds(player, state);
        Send(player, state);
        state.saveTick += elapsed;
        if (state.saveTick >= 30000)
        {
            state.saveTick = 0;
            Save(player, state);
        }
    }

    bool OnPlayerCanUseChat(Player* player, uint32, uint32 language, std::string& message, Player*) override
    {
        if (language != LANG_ADDON || message.rfind("HXN\t", 0) != 0)
            return true;
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        auto const found = States.find(player->GetGUID().GetCounter());
        if (found != States.end())
            Send(player, found->second);
        return false;
    }
};

class NeedsUnit : public UnitScript
{
public:
    NeedsUnit() : UnitScript("CoANeedsUnit", true, { UNITHOOK_ON_DAMAGE }) { }

    void OnDamage(Unit* attacker, Unit* victim, uint32& damage) override
    {
        Player* player = attacker ? attacker->ToPlayer() : nullptr;
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        if (!Affects(player) || !damage || attacker == victim)
            return;
        auto found = States.find(player->GetGUID().GetCounter());
        if (found == States.end())
            return;
        State& state = found->second;
        float modifier = 1.0f;
        if (!state.delegated)
        {
            if (state.displayFood <= 35.0f)
                modifier *= 0.85f;
            if (state.displayWater <= 30.0f)
                modifier *= 0.9f;
        }
        if (state.vigor <= 0.0f)
            modifier *= 0.5f;
        if (state.attackLockUntil > uint32(std::time(nullptr)))
            modifier = 0.0f;
        damage = uint32(damage * modifier);
        uint64 now = Milliseconds();
        if (damage && now - state.lastAttack >= 1000)
        {
            state.vigor = std::max(0.0f, state.vigor - 3.0f);
            state.lastSpend = now;
            state.lastAttack = now;
        }
    }
};
}

void AddSC_coa_needs()
{
    new NeedsWorld();
    new NeedsPlayer();
    new NeedsUnit();
}
