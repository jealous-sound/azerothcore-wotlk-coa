// SPDX-License-Identifier: GPL-2.0-or-later

#include "Chat.h"
#include "Config.h"
#include "Creature.h"
#include "DatabaseEnv.h"
#include "GameObject.h"
#include "Item.h"
#include "Log.h"
#include "Map.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "NeedsChallengeBridge.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Weather.h"
#include "World.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <ctime>
#include <mutex>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

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
constexpr uint32 CampRest = 996100;
constexpr uint32 Fellowship = 996103;
constexpr uint32 CampAbility = 996106;
constexpr uint32 PackAbility = 996109;
constexpr uint32 WeatherAura = 996110;
constexpr uint32 InjuryAura = 996115;
constexpr uint32 SplintAbility = 996119;
constexpr uint32 ToolRecipe = 996120;
constexpr uint32 ProfessionAura = 996140;
constexpr uint32 SurvivalistSkill = 9200;
constexpr uint32 ScissorsRecipe = 996136;
constexpr uint32 ScissorsUse = 996137;
std::unordered_set<uint32> BandageSpells;
std::unordered_set<uint32> PotionSpells;

uint32 RecipeSkill(uint32 spell)
{
    if (spell >= CampAbility && spell < PackAbility)
        return std::array<uint32, 3>{ 1, 75, 150 }[spell - CampAbility];
    if (spell >= ToolRecipe && spell < ToolRecipe + 8)
        return std::array<uint32, 8>{ 1, 1, 1, 1, 100, 150, 50, 1 }[spell - ToolRecipe];
    return spell == ScissorsRecipe ? 75 : 0;
}

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
    uint32 campLifetime = 3600;
    uint32 campRestFillSeconds = 300;
    bool weather = true;
    bool injuries = true;
    bool tools = true;
    bool professions = true;
    uint32 wetExposure = 300000;
    uint32 coldExposure = 420000;
    uint32 heatExposure = 180000;
};

struct Treatment
{
    uint32 item = 0;
    uint32 count = 0;
    uint32 spell = 0;
    uint64 started = 0;
    ObjectGuid target;
    bool bandage = false;
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
    uint32 campReady = 0;
    uint32 socialTick = 0;
    int32 restingCamp = -1;
    PendingMeal meal;
    Treatment treatment;
    std::array<uint32, 3> exposure = {};
    std::array<bool, 5> weather = {};
    uint32 coldTick = 0;
    uint32 stormTick = 0;
    uint32 injuries = 0;
    char condition = 'S';
};

struct Camp
{
    uint32 map = 0;
    uint32 expires = 0;
    uint32 tier = 0;
    ObjectGuid fire;
    ObjectGuid tent;
};

Options Config;
bool SpellsReady = false;
std::recursive_mutex Mutex;
std::unordered_map<ObjectGuid::LowType, State> States;
std::unordered_map<ObjectGuid::LowType, Camp> Camps;
std::unordered_map<uint32, WeatherState> ZoneWeather;

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

float SkillScale(Player const* player, uint32 skill)
{
    return std::min(1.0f, player->GetPureSkillValue(skill) / 450.0f);
}

void RefreshProfessions(Player* player)
{
    static std::array<uint32, 13> const skills = { SKILL_MINING, SKILL_BLACKSMITHING, SKILL_SKINNING,
        SKILL_INSCRIPTION, SKILL_TAILORING, SKILL_JEWELCRAFTING, SKILL_ENCHANTING, SKILL_LEATHERWORKING,
        SKILL_HERBALISM, SKILL_ALCHEMY, SKILL_FIRST_AID, SKILL_FISHING, SKILL_ENGINEERING };
    static std::array<float, 13> const bonuses = { 4, 3, 2, 2, 20, 2, 10, 5, 5, 15, 20, 50, 1 };
    for (uint32 index = 0; index < skills.size(); ++index)
    {
        float scale = SkillScale(player, skills[index]);
        bool active = Config.professions && Affects(player) && player->IsAlive() && scale > 0;
        uint32 spell = ProfessionAura + index;
        SetAura(player, spell, active);
        if (!active)
            continue;
        int32 amount = int32(std::lround(bonuses[index] * scale));
        for (uint8 effect = 0; effect < MAX_SPELL_EFFECTS; ++effect)
            if (AuraEffect* aura = player->GetAuraEffect(spell, effect))
                if (aura->GetAmount() != amount)
                    aura->ChangeAmount(amount);
    }
}

void StopSprint(Player* player, State& state)
{
    state.sprint = false;
    player->RemoveAurasDueToSpell(SprintAura);
}

void Save(Player* player, State const& state)
{
    std::array<uint32, 11> values = { 1, uint32(Clamp(state.food) * 1000.0f),
        uint32(Clamp(state.water) * 1000.0f), uint32(Clamp(state.vigor) * 1000.0f),
        state.dodgeUntil, state.windUntil, state.exhaustedUntil, state.attackLockUntil, state.windBuffUntil,
        state.campReady, state.injuries };
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
         << '~' << remaining(state.exhaustedUntil) << '~' << state.delegated << '~' << state.restingCamp
         << '~' << remaining(state.campReady) << '~' << state.condition;
    std::array<uint32, 3> thresholds = { Config.wetExposure, Config.coldExposure, Config.heatExposure };
    for (uint32 index = 0; index < 3; ++index)
        wire << '~' << (thresholds[index] > state.exposure[index] ?
            (thresholds[index] - state.exposure[index] + 999) / 1000 : 0);
    uint32 flags = 0;
    for (uint32 index = 0; index < 5; ++index)
        if (state.weather[index])
            flags |= 1u << index;
    wire << '~' << flags << '~' << state.injuries;
    WorldPacket packet;
    ChatHandler::BuildChatPacket(packet, CHAT_MSG_WHISPER, LANG_ADDON, player, nullptr, wire.str());
    player->SendDirectMessage(&packet);
}

void Unlock(Player* player)
{
    if (!Affects(player))
        return;
    if (!player->HasSkill(SurvivalistSkill))
        player->SetSkill(SurvivalistSkill, 1, 1, 300);
    player->learnSpell(SprintAbility);
    if (player->HasSkill(SKILL_FIRST_AID))
        player->learnSpell(SplintAbility);
    if (Config.tools)
    {
        for (uint32 index = 0; index < 8; ++index)
            if (player->GetPureSkillValue(SurvivalistSkill) >= RecipeSkill(ToolRecipe + index))
                player->learnSpell(ToolRecipe + index);
        if (player->GetPureSkillValue(SurvivalistSkill) >= RecipeSkill(ScissorsRecipe))
            player->learnSpell(ScissorsRecipe);
    }
    if (player->GetLevel() >= 30)
        player->learnSpell(DodgeAbility);
    if (player->GetLevel() >= 60)
        player->learnSpell(WindAbility);
    for (uint32 tier = 0; tier < 3; ++tier)
        if (player->GetPureSkillValue(SurvivalistSkill) >= RecipeSkill(CampAbility + tier))
            player->learnSpell(CampAbility + tier);
    player->learnSpell(PackAbility);
}

void PackCamp(Player* player)
{
    auto found = Camps.find(player->GetGUID().GetCounter());
    if (found == Camps.end())
        return;
    if (Map* map = player->FindMap(); map && map->GetId() == found->second.map)
    {
        if (GameObject* fire = map->GetGameObject(found->second.fire))
            fire->Delete();
        if (GameObject* tent = map->GetGameObject(found->second.tent))
            tent->Delete();
    }
    Camps.erase(found);
}

int32 NearbyCamp(Player* player)
{
    Map* map = player->FindMap();
    if (!map)
        return -1;
    uint32 now = uint32(std::time(nullptr));
    int32 tier = -1;
    for (auto camp = Camps.begin(); camp != Camps.end();)
    {
        if (camp->second.expires <= now)
        {
            camp = Camps.erase(camp);
            continue;
        }
        if (camp->second.map == map->GetId())
        {
            GameObject* fire = map->GetGameObject(camp->second.fire);
            if (!fire)
            {
                camp = Camps.erase(camp);
                continue;
            }
            if (player->InSamePhase(fire) && player->IsWithinDistInMap(fire, 15.0f + 5.0f * camp->second.tier))
                tier = std::max(tier, int32(camp->second.tier));
        }
        ++camp;
    }
    return tier;
}

void PlaceCamp(Player* player, State& state, uint32 tier)
{
    Map* map = player->FindMap();
    uint32 now = uint32(std::time(nullptr));
    if (!map || map->Instanceable() || !player->IsAlive() || player->IsInCombat() || player->isMoving() ||
        player->IsMounted() || player->IsInFlight() || player->IsInWater() || player->IsFalling() ||
        player->HasUnitState(UNIT_STATE_CONTROLLED))
    {
        ChatHandler(player->GetSession()).SendSysMessage("Place camps on dry land outside combat and instances.");
        return;
    }
    if (tier > 2 || player->GetPureSkillValue(SurvivalistSkill) < RecipeSkill(CampAbility + tier) || state.campReady > now)
        return;
    uint32 leather = tier == 1 ? 2318 : 4234;
    if (!player->HasItemCount(4470, 2, false) || (tier && !player->HasItemCount(leather, 4, false)))
    {
        ChatHandler(player->GetSession()).SendSysMessage(tier == 0 ? "Campfire requires 2 Simple Wood." :
            tier == 1 ? "Shelter requires 2 Simple Wood and 4 Light Leather." :
            "Hearthstead requires 2 Simple Wood and 4 Heavy Leather.");
        return;
    }
    float orientation = player->GetOrientation();
    GameObject* fire = player->SummonGameObject(tier == 2 ? 1831 : 1798,
        player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), orientation,
        0, 0, std::sin(orientation / 2), std::cos(orientation / 2), Config.campLifetime);
    if (!fire)
        return;
    GameObject* tent = nullptr;
    if (tier)
    {
        float x, y, z;
        player->GetNearPoint(nullptr, x, y, z, 0.0f, tier == 1 ? 5.0f : 8.0f, orientation + 1.5707963f);
        tent = player->SummonGameObject(tier == 1 ? 184592 : 184593, x, y, z, orientation,
            0, 0, std::sin(orientation / 2), std::cos(orientation / 2), Config.campLifetime);
        if (!tent)
        {
            fire->Delete();
            return;
        }
    }
    PackCamp(player);
    player->DestroyItemCount(4470, 2, true);
    if (tier)
        player->DestroyItemCount(leather, 4, true);
    Camp camp;
    camp.map = map->GetId();
    camp.expires = now + Config.campLifetime;
    camp.tier = tier;
    camp.fire = fire->GetGUID();
    if (tent)
        camp.tent = tent->GetGUID();
    Camps[player->GetGUID().GetCounter()] = camp;
    state.campReady = now + 60;
    player->UpdateCraftSkill(CampAbility + tier);
    Unlock(player);
    Save(player, state);
    ChatHandler(player->GetSession()).SendSysMessage("Camp placed. Sit nearby to recover and gain rested experience.");
}

void RestAtCamp(Player* player, State& state, int32 tier, uint32 elapsed)
{
    state.restingCamp = player->IsAlive() && player->IsSitState() && !player->IsInCombat() ? tier : -1;
    for (uint32 index = 0; index < 3; ++index)
        SetAura(player, CampRest + index, state.restingCamp == int32(index));
    if (state.restingCamp < 0)
    {
        state.socialTick = 0;
        return;
    }
    float seconds = elapsed / 1000.0f;
    float restedCap = player->GetUInt32Value(PLAYER_NEXT_LEVEL_XP) * sWorld->getRate(RATE_REST_MAX_BONUS) / 2.0f;
    float rested = restedCap * seconds / Config.campRestFillSeconds;
    player->SetRestBonus(player->GetRestBonus() + rested);
    state.socialTick += elapsed;
    if (state.socialTick < 30000)
        return;
    state.socialTick = 0;
    for (uint32 index = uint32(tier); index < 3; ++index)
        if (player->HasAura(Fellowship + index))
            return;
    for (uint32 index = 0; index < 3; ++index)
        player->RemoveAurasDueToSpell(Fellowship + index);
    if (Aura* aura = player->AddAura(Fellowship + uint32(tier), player))
    {
        aura->SetMaxDuration(3600000);
        aura->SetDuration(3600000);
    }
    ChatHandler(player->GetSession()).SendSysMessage("Campfire Fellowship: a lasting bonus to all primary attributes.");
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

void RefreshInjuries(Player* player, State const& state)
{
    bool active = Config.injuries && player->IsAlive() && !player->InBattleground() && !player->InArena();
    for (uint32 index = 0; index < 4; ++index)
        SetAura(player, InjuryAura + index, active && (state.injuries & (1u << index)));
}

void ChangeInjury(Player* player, State& state, uint32 index, bool active)
{
    uint32 mask = 1u << index;
    if (bool(state.injuries & mask) == active)
        return;
    if (active)
        state.injuries |= mask;
    else
        state.injuries &= ~mask;
    Save(player, state);
    static std::array<char const*, 4> const notices = {
        "Lingering Wound: complete a bandage to treat it.",
        "Lingering Venom: consume a healing potion or use a poison cure.",
        "Broken Leg: use Field Splint outside combat. Requires 4 Simple Wood and 2 Linen Cloth.",
        "Broken Arm: use Field Splint outside combat. Requires 4 Simple Wood and 2 Linen Cloth."
    };
    ChatHandler(player->GetSession()).SendSysMessage(active ? notices[index] : "Your injury has been treated.");
}

void TickWeather(Player* player, State& state, int32 campTier, uint32 elapsed)
{
    Map* map = player->FindMap();
    bool sheltered = !Config.weather || !map || !player->IsAlive() || !player->IsOutdoors() ||
        map->Instanceable() || player->InBattleground() || player->InArena() || campTier >= 0 ||
        player->HasPlayerFlag(PLAYER_FLAGS_RESTING);
    if (sheltered)
    {
        state.exposure = {};
        state.weather = {};
        state.coldTick = 0;
        state.stormTick = 0;
        state.condition = 'S';
    }
    else
    {
        map->GetOrGenerateZoneDefaultWeather(player->GetZoneId());
        auto current = ZoneWeather.find(player->GetZoneId());
        WeatherState sky = current != ZoneWeather.end() ? current->second : WEATHER_STATE_FINE;
        bool storm = sky == WEATHER_STATE_THUNDERS || sky == WEATHER_STATE_BLACKRAIN;
        bool rain = storm || sky == WEATHER_STATE_LIGHT_RAIN || sky == WEATHER_STATE_MEDIUM_RAIN ||
            sky == WEATHER_STATE_HEAVY_RAIN;
        bool snow = sky == WEATHER_STATE_LIGHT_SNOW || sky == WEATHER_STATE_MEDIUM_SNOW ||
            sky == WEATHER_STATE_HEAVY_SNOW || sky == WEATHER_STATE_BLACKSNOW;
        static std::array<uint32, 13> const hotZones = {
            14, 17, 25, 440, 405, 400, 490, 51, 46, 1377, 3483, 3520, 3535
        };
        static std::array<uint32, 9> const coldZones = { 1, 618, 2817, 65, 394, 495, 210, 67, 66 };
        bool hot = std::find(hotZones.begin(), hotZones.end(), player->GetZoneId()) != hotZones.end();
        bool cold = snow || (player->GetMapId() == 571 && player->GetZoneId() != 3711) ||
            std::find(coldZones.begin(), coldZones.end(), player->GetZoneId()) != coldZones.end();
        std::array<bool, 3> exposed = { rain || snow, cold, hot && !rain && !snow };
        std::array<uint32, 3> limits = { Config.wetExposure, Config.coldExposure, Config.heatExposure };
        for (uint32 index = 0; index < 3; ++index)
        {
            uint64 amount = exposed[index] ? uint64(state.exposure[index]) + elapsed :
                (uint64(elapsed) * 2 >= state.exposure[index] ? 0 : state.exposure[index] - uint64(elapsed) * 2);
            state.exposure[index] = uint32(std::min<uint64>(amount, limits[index]));
            state.weather[index] = state.exposure[index] >= limits[index];
        }
        state.weather[3] = storm;
        state.weather[4] = sky == WEATHER_STATE_FINE && !cold && !hot &&
            !state.weather[0] && !state.weather[1] && !state.weather[2];
        state.condition = storm ? 'T' : snow ? 'N' : rain ? 'R' : hot ? 'H' : cold ? 'C' : 'F';
        if (state.weather[0] && state.weather[1])
        {
            state.coldTick += elapsed;
            if (state.coldTick >= 60000)
            {
                state.coldTick %= 60000;
                player->EnvironmentalDamage(DAMAGE_EXHAUSTED, std::max(1u, player->GetMaxHealth() / 100));
            }
        }
        else
            state.coldTick = 0;
        if (storm)
        {
            state.stormTick += elapsed;
            if (state.stormTick >= 60000)
            {
                state.stormTick %= 60000;
                state.vigor = std::max(0.0f, state.vigor - 5.0f);
                state.lastSpend = Milliseconds();
            }
        }
        else
            state.stormTick = 0;
    }
    for (uint32 index = 0; index < 5; ++index)
        SetAura(player, WeatherAura + index, state.weather[index]);
}

void TickTreatment(Player* player, State& state)
{
    Treatment& use = state.treatment;
    if (!use.item)
        return;
    bool consumed = player->GetItemCount(use.item, false) < use.count;
    if (Milliseconds() - use.started >= 30000)
        use = {};
    else if (!use.bandage && consumed)
    {
        Player* target = ObjectAccessor::FindPlayer(use.target);
        if (Affects(target) && target->GetMapId() == player->GetMapId())
        {
            auto found = States.find(target->GetGUID().GetCounter());
            if (found != States.end())
            {
                ChangeInjury(target, found->second, 1, false);
                RefreshInjuries(target, found->second);
                target->RemoveAppliedAuras([](AuraApplication const* application)
                {
                    return application->GetBase()->GetSpellInfo()->Dispel == DISPEL_POISON;
                });
            }
        }
        use = {};
    }
}

class NeedsWeather : public ALEScript
{
public:
    NeedsWeather() : ALEScript("CoANeedsWeather") { }

    void OnWeatherChange(Weather* weather, WeatherState state, float) override
    {
        if (!weather)
            return;
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        ZoneWeather[weather->GetZone()] = state;
    }
};

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
        Config.campLifetime = std::clamp(sConfigMgr->GetOption<uint32>("CoANeeds.CampLifetimeSeconds", 3600),
            60u, 86400u);
        Config.campRestFillSeconds = std::clamp(
            sConfigMgr->GetOption<uint32>("CoANeeds.CampRestFillSeconds", 300), 1u, 86400u);
        Config.weather = sConfigMgr->GetOption<bool>("CoANeeds.Weather.Enable", true);
        Config.injuries = sConfigMgr->GetOption<bool>("CoANeeds.Injuries.Enable", true);
        Config.tools = sConfigMgr->GetOption<bool>("CoANeeds.Tools.Enable", true);
        Config.professions = sConfigMgr->GetOption<bool>("CoANeeds.Professions.Enable", true);
        Config.wetExposure = std::clamp(sConfigMgr->GetOption<uint32>("CoANeeds.Weather.WetExposureMs", 300000),
            1000u, 3600000u);
        Config.coldExposure = std::clamp(sConfigMgr->GetOption<uint32>("CoANeeds.Weather.ColdExposureMs", 420000),
            1000u, 3600000u);
        Config.heatExposure = std::clamp(sConfigMgr->GetOption<uint32>("CoANeeds.Weather.HeatExposureMs", 180000),
            1000u, 3600000u);
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
        for (uint32 id = CampRest; id <= PackAbility; ++id)
            if (!sSpellMgr->GetSpellInfo(id))
            {
                SpellsReady = false;
                LOG_ERROR("module.coa_needs", "Missing camp spell {}; module disabled", id);
            }
        for (uint32 id = WeatherAura; id <= ScissorsUse; ++id)
            if (!sSpellMgr->GetSpellInfo(id))
            {
                SpellsReady = false;
                LOG_ERROR("module.coa_needs", "Missing weather/injury spell {}; module disabled", id);
            }
        for (uint32 id = ProfessionAura; id <= ProfessionAura + 12; ++id)
            if (!sSpellMgr->GetSpellInfo(id))
            {
                SpellsReady = false;
                LOG_ERROR("module.coa_needs", "Missing profession spell {}; module disabled", id);
            }
        BandageSpells.clear();
        PotionSpells.clear();
        for (auto const& [entry, item] : *sObjectMgr->GetItemTemplateStore())
            if (item.Class == ITEM_CLASS_CONSUMABLE)
                for (auto const& itemSpell : item.Spells)
                    if (itemSpell.SpellId > 0)
                    {
                        if (item.SubClass == ITEM_SUBCLASS_BANDAGE)
                            BandageSpells.insert(itemSpell.SpellId);
                        else if (item.SubClass == ITEM_SUBCLASS_POTION)
                            PotionSpells.insert(itemSpell.SpellId);
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
        PLAYERHOOK_ON_PLAYER_ENTER_COMBAT, PLAYERHOOK_CAN_PLAYER_USE_PRIVATE_CHAT, PLAYERHOOK_ON_UPDATE_SKILL, PLAYERHOOK_ON_SET_SKILL }) { }

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
            if (saved->size() >= 10)
                state.campReady = (*saved)[9].value;
            if (saved->size() >= 11)
                state.injuries = (*saved)[10].value & 15u;
        }
        StopSprint(player, state);
        for (uint32 index = 0; index < 3; ++index)
            player->RemoveAurasDueToSpell(CampRest + index);
        UpdateNeeds(player, state);
        States[player->GetGUID().GetCounter()] = state;
        for (uint32 index = 0; index < 5; ++index)
            player->RemoveAurasDueToSpell(WeatherAura + index);
        RefreshInjuries(player, state);
        Unlock(player);
        RefreshProfessions(player);
        Send(player, state);
    }

    void OnPlayerLevelChanged(Player* player, uint8) override
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        Unlock(player);
    }

    void OnPlayerUpdateSkill(Player* player, uint32 skill, uint32, uint32, uint32, uint32) override
    {
        if (skill == SurvivalistSkill || skill == SKILL_FIRST_AID)
        {
            std::lock_guard<std::recursive_mutex> lock(Mutex);
            Unlock(player);
        }
    }

    void OnPlayerSetSkill(Player* player, uint32 skill, uint32 value, uint32 maximum, uint32 step,
        uint32 newValue) override
    {
        OnPlayerUpdateSkill(player, skill, value, maximum, step, newValue);
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
        PackCamp(player);
        for (uint32 index = 0; index < 3; ++index)
            player->RemoveAurasDueToSpell(CampRest + index);
        for (uint32 index = 0; index < 5; ++index)
            player->RemoveAurasDueToSpell(WeatherAura + index);
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
        if (id == SplintAbility)
        {
            if (!Config.injuries || !player->IsAlive() || player->IsInCombat() || player->isMoving() ||
                player->IsMounted() || player->IsInFlight() || player->InBattleground() || player->InArena())
                return;
            if (!(state.injuries & 12u))
                ChatHandler(player->GetSession()).SendSysMessage("You have no broken arm or leg to splint.");
            else if (player->GetItemCount(4470, false) < 4 || player->GetItemCount(2589, false) < 2)
                ChatHandler(player->GetSession()).SendSysMessage("Field Splint requires 4 Simple Wood and 2 Linen Cloth.");
            else
            {
                player->DestroyItemCount(4470, 4, true);
                player->DestroyItemCount(2589, 2, true);
                ChangeInjury(player, state, (state.injuries & 4u) ? 2 : 3, false);
                RefreshInjuries(player, state);
                player->UpdateCraftSkill(SplintAbility);
            }
            Send(player, state);
            return;
        }
        if (id >= CampAbility && id <= PackAbility)
        {
            if (id == PackAbility)
            {
                if (!player->IsInCombat())
                    PackCamp(player);
            }
            else
                PlaceCamp(player, state, id - CampAbility);
            Send(player, state);
            return;
        }
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
        ItemTemplate const* item = spell->m_CastItem->GetTemplate();
        bool bandage = item->Class == ITEM_CLASS_CONSUMABLE && item->SubClass == ITEM_SUBCLASS_BANDAGE;
        bool healingPotion = spell->m_CastItem->IsPotion() &&
            (spell->GetSpellInfo()->HasEffect(SPELL_EFFECT_HEAL) ||
                spell->GetSpellInfo()->HasEffect(SPELL_EFFECT_HEAL_PCT));
        if (Config.injuries && (bandage || healingPotion))
        {
            state.treatment = { item->ItemId, player->GetItemCount(item->ItemId, false), id,
                Milliseconds(), spell->m_targets.GetUnitTargetGUID(), bandage };
            if (state.treatment.target.IsEmpty())
                state.treatment.target = player->GetGUID();
        }
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
            for (uint32 index = 0; index < 3; ++index)
                player->RemoveAurasDueToSpell(CampRest + index);
            for (uint32 index = 0; index < 5; ++index)
                player->RemoveAurasDueToSpell(WeatherAura + index);
            for (uint32 index = 0; index < 4; ++index)
                player->RemoveAurasDueToSpell(InjuryAura + index);
            RefreshProfessions(player);
            PackCamp(player);
            return;
        }
        UpdateNeeds(player, state);
        TickTreatment(player, state);
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
        int32 campTier = NearbyCamp(player);
        RestAtCamp(player, state, campTier, elapsed);
        TickWeather(player, state, campTier, elapsed);
        RefreshInjuries(player, state);
        RefreshProfessions(player);
        if (alive && !pvp && !state.delegated && campTier < 0)
        {
            float cooking = player->GetSkillValue(SKILL_COOKING) / 450.0f;
            float foodDrain = Config.foodDrain * (1.0f - 0.05f * cooking) * (state.weather[2] ? 1.1f : 1.0f);
            float waterDrain = Config.waterDrain * (state.weather[2] ? 1.25f : 1.0f) *
                (state.weather[3] ? 1.1f : 1.0f) * (Config.injuries && (state.injuries & 2u) ? 1.15f : 1.0f);
            state.food = Clamp(state.food - foodDrain * seconds / 60.0f);
            state.water = Clamp(state.water - waterDrain * seconds / 60.0f);
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
            float fishing = Config.professions ? SkillScale(player, SKILL_FISHING) : 0.0f;
            state.vigor = std::max(0.0f, state.vigor - 4.0f * (1.0f - 0.5f * fishing) * seconds);
            state.lastSpend = steady;
        }
        else if (alive && steady - state.lastSpend >= 1500)
        {
            float regen = Config.regen;
            if (Config.professions)
                regen *= 1.0f + 0.05f * SkillScale(player, SKILL_HERBALISM);
            for (uint32 index = 0; index < 3; ++index)
                if (state.weather[index])
                    regen *= 0.9f;
            if (state.weather[3])
                regen *= 0.95f;
            if (state.weather[4])
                regen *= 1.05f;
            if (state.displayFood <= 35.0f)
                regen *= 0.65f;
            if (state.displayWater <= 30.0f)
                regen *= 0.65f;
            if (player->IsInCombat())
                regen *= 0.25f;
            else if (player->IsSitState())
                regen *= 2.0f;
            if (state.restingCamp >= 0)
                regen *= 1.5f + 0.5f * state.restingCamp;
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
            if (alive && !pvp && !state.delegated && campTier < 0 && state.displayWater <= 10.0f &&
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
    NeedsUnit() : UnitScript("CoANeedsUnit", true,
        { UNITHOOK_ON_DAMAGE, UNITHOOK_ON_BEFORE_HEAL_ABSORB, UNITHOOK_ON_AURA_APPLY, UNITHOOK_ON_AURA_REMOVE }) { }

    void OnBeforeHealAbsorb(HealInfo& heal) override
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        Player* player = heal.GetHealer() ? heal.GetHealer()->ToPlayer() : nullptr;
        SpellInfo const* info = heal.GetSpellInfo();
        if (!Config.professions || !Affects(player) || !info || !heal.GetHeal())
            return;
        float bonus = 0.0f;
        if (BandageSpells.count(info->Id))
            bonus = 0.2f * SkillScale(player, SKILL_FIRST_AID);
        else if (PotionSpells.count(info->Id))
            bonus = 0.15f * SkillScale(player, SKILL_ALCHEMY);
        heal.SetHeal(uint32(heal.GetHeal() * (1.0f + bonus)));
    }

    void OnAuraApply(Unit* unit, Aura* aura) override
    {
        Player* player = unit ? unit->ToPlayer() : nullptr;
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        if (!Config.injuries || !Affects(player) || !aura || !player->IsAlive() ||
            player->InBattleground() || player->InArena() || !player->FindMap())
            return;
        Unit* caster = aura->GetCaster();
        if (!caster || !caster->IsCreature() || caster->GetCharmerOrOwnerPlayerOrPlayerItself())
            return;
        auto found = States.find(player->GetGUID().GetCounter());
        if (found == States.end())
            return;
        SpellInfo const* info = aura->GetSpellInfo();
        if (info->HasEffectMechanic(MECHANIC_BLEED))
            ChangeInjury(player, found->second, 0, true);
        else if (info->Dispel == DISPEL_POISON)
            ChangeInjury(player, found->second, 1, true);
    }

    void OnAuraRemove(Unit* unit, AuraApplication* application, AuraRemoveMode mode) override
    {
        Player* player = unit ? unit->ToPlayer() : nullptr;
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        if (!Config.injuries || !Affects(player) || !application)
            return;
        auto found = States.find(player->GetGUID().GetCounter());
        if (found == States.end())
            return;
        Aura* aura = application->GetBase();
        if (aura->GetId() == InjuryAura + 1 && mode == AURA_REMOVE_BY_ENEMY_SPELL)
            ChangeInjury(player, found->second, 1, false);
        if (mode != AURA_REMOVE_BY_EXPIRE || !aura->GetCasterGUID().IsPlayer())
            return;
        auto healer = States.find(aura->GetCasterGUID().GetCounter());
        if (healer == States.end())
            return;
        Treatment& use = healer->second.treatment;
        Player* caster = ObjectAccessor::FindPlayer(aura->GetCasterGUID());
        if (!caster || !use.bandage || use.spell != aura->GetId() || use.target != player->GetGUID() ||
            caster->GetItemCount(use.item, false) >= use.count)
            return;
        use = {};
        ChangeInjury(player, found->second, 0, false);
        player->RemoveAppliedAuras([](AuraApplication const* applied)
        {
            return applied->GetBase()->GetCasterGUID().IsCreature() &&
                applied->GetBase()->GetSpellInfo()->HasEffectMechanic(MECHANIC_BLEED);
        });
    }

    void OnDamage(Unit* attacker, Unit* victim, uint32& damage) override
    {
        Player* player = attacker ? attacker->ToPlayer() : nullptr;
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        Player* injured = victim ? victim->ToPlayer() : nullptr;
        if (Config.injuries && Affects(injured) && attacker && attacker->IsCreature() && damage &&
            !attacker->GetCharmerOrOwnerPlayerOrPlayerItself() && attacker->IsHostileTo(injured) &&
            injured->IsAlive() && !injured->InBattleground() && !injured->InArena() && injured->FindMap() &&
            (injured->IsOutdoors() || injured->FindMap()->Instanceable()) && damage < injured->GetHealth())
        {
            auto target = States.find(injured->GetGUID().GetCounter());
            if (target != States.end())
            {
                uint64 hit = uint64(damage) * 100;
                uint64 health = injured->GetMaxHealth();
                if (hit >= health * 30)
                    ChangeInjury(injured, target->second, 2, true);
                else if (hit >= health * 20)
                    ChangeInjury(injured, target->second, 3, true);
            }
        }
        if (!Affects(player) || !damage || attacker == victim)
            return;
        auto found = States.find(player->GetGUID().GetCounter());
        if (found == States.end())
            return;
        State& state = found->second;
        float modifier = 1.0f;
        if (Config.professions && victim && victim->IsCreature() &&
            victim->ToCreature()->GetCreatureTemplate()->type == CREATURE_TYPE_BEAST)
            modifier *= 1.0f + 0.05f * SkillScale(player, SKILL_LEATHERWORKING);
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

class NeedsRecipeGate : public AllSpellScript
{
public:
    NeedsRecipeGate() : AllSpellScript("CoANeedsRecipeGate", { ALLSPELLHOOK_ON_SPELL_CHECK_CAST }) { }

    void OnSpellCheckCast(Spell* spell, bool, SpellCastResult& result) override
    {
        Player* player = spell->GetCaster() ? spell->GetCaster()->ToPlayer() : nullptr;
        if (!Affects(player) || result != SPELL_CAST_OK)
            return;
        uint32 id = spell->GetSpellInfo()->Id;
        uint32 requirement = RecipeSkill(id);
        if (requirement && player->GetPureSkillValue(SurvivalistSkill) < requirement)
            result = SPELL_FAILED_LOW_CASTLEVEL;
        else if (id == SplintAbility && !player->HasSkill(SKILL_FIRST_AID))
            result = SPELL_FAILED_LOW_CASTLEVEL;
    }
};

class NeedsTool : public ItemScript
{
public:
    NeedsTool() : ItemScript("item_coa_survival_tool") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const&) override
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        Map* map = player ? player->FindMap() : nullptr;
        if (!Config.tools || !Affects(player) || !item || !map)
            return true;
        if (!player->IsAlive() || player->IsInCombat() || player->IsMounted() || player->IsInFlight() ||
            player->HasUnitState(UNIT_STATE_CONTROLLED))
        {
            ChatHandler(player->GetSession()).SendSysMessage("Use survival tools alive, unmounted and outside combat.");
            return true;
        }
        uint32 entry = item->GetEntry();
        if (entry == 996210)
        {
            if (!player->IsOutdoors() || map->Instanceable())
            {
                ChatHandler(player->GetSession()).SendSysMessage("Use Barber Scissors outdoors outside instances.");
                return true;
            }
            float orientation = player->GetOrientation();
            float x = player->GetPositionX() + std::cos(orientation) * 2.5f;
            float y = player->GetPositionY() + std::sin(orientation) * 2.5f;
            float z = map->GetHeight(player->GetPhaseMask(), x, y, player->GetPositionZ());
            if (z > INVALID_HEIGHT)
                player->SummonGameObject(190683, x, y, z, orientation, 0, 0,
                    std::sin(orientation * 0.5f), std::cos(orientation * 0.5f), 60, false, GO_SUMMON_TIMED_DESPAWN);
            return true;
        }
        if (entry == 996200 || entry == 996202)
        {
            bool valid = player->IsInWater();
            if (entry == 996202)
            {
                map->GetOrGenerateZoneDefaultWeather(player->GetZoneId());
                auto weather = ZoneWeather.find(player->GetZoneId());
                valid = weather != ZoneWeather.end() && (weather->second == WEATHER_STATE_LIGHT_RAIN ||
                    weather->second == WEATHER_STATE_MEDIUM_RAIN || weather->second == WEATHER_STATE_HEAVY_RAIN ||
                    weather->second == WEATHER_STATE_THUNDERS || weather->second == WEATHER_STATE_BLACKRAIN);
                valid = valid && player->IsOutdoors() && !map->Instanceable();
            }
            if (!valid)
            {
                ChatHandler(player->GetSession()).SendSysMessage(entry == 996200 ?
                    "Stand in water to refill your flask." : "Collect rainwater outdoors while it is raining.");
                return true;
            }
            ItemPosCountVec destination;
            InventoryResult result = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, destination, entry + 1, 1);
            if (result != EQUIP_ERR_OK)
            {
                player->SendEquipError(result, nullptr, nullptr);
                return true;
            }
            if (Item* filled = player->StoreNewItem(destination, entry + 1, true))
            {
                player->DestroyItemCount(entry, 1, true);
                player->SendNewItem(filled, 1, true, false);
            }
        }
        else if (entry == 996204)
        {
            uint16 position = 0;
            uint32 loss = 0;
            for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
                if (Item* equipped = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                {
                    uint32 maximum = equipped->GetUInt32Value(ITEM_FIELD_MAXDURABILITY);
                    uint32 current = equipped->GetUInt32Value(ITEM_FIELD_DURABILITY);
                    if (maximum > current && maximum - current > loss)
                    {
                        loss = maximum - current;
                        position = (INVENTORY_SLOT_BAG_0 << 8) | slot;
                    }
                }
            if (!loss)
                ChatHandler(player->GetSession()).SendSysMessage("No equipped item needs repair. The kit was kept.");
            else
            {
                player->DurabilityRepair(position, false, 0.0f, false);
                player->DestroyItemCount(entry, 1, true);
                ChatHandler(player->GetSession()).SendSysMessage("Repaired your most damaged equipped item.");
            }
        }
        else if (entry == 996208)
        {
            if (!player->IsOutdoors() || map->Instanceable() || player->isMoving() || player->IsFalling())
                return true;
            float orientation = player->GetOrientation();
            float x = player->GetPositionX() + std::cos(orientation) * 4.0f;
            float y = player->GetPositionY() + std::sin(orientation) * 4.0f;
            float ground = player->GetPositionZ();
            float water = map->GetWaterOrGroundLevel(player->GetPhaseMask(), x, y, ground, &ground, true);
            if (water <= ground + 0.5f || !map->IsInWater(player->GetPhaseMask(), x, y, water - 0.25f,
                player->GetCollisionHeight()))
                ChatHandler(player->GetSession()).SendSysMessage("Face open water to deploy your raft.");
            else if (player->SummonGameObject(996200, x, y, water, orientation, 0, 0,
                std::sin(orientation * 0.5f), std::cos(orientation * 0.5f), 1800, false, GO_SUMMON_TIMED_DESPAWN))
            {
                player->DestroyItemCount(entry, 1, true);
                ChatHandler(player->GetSession()).SendSysMessage("Your raft will remain here for 30 minutes.");
            }
        }
        return true;
    }
};
}

void AddSC_coa_needs()
{
    new NeedsWorld();
    new NeedsWeather();
    new NeedsPlayer();
    new NeedsUnit();
    new NeedsTool();
    new NeedsRecipeGate();
}
