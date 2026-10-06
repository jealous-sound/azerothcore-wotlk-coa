#include "CoACampingMapping.h"
#include "CoACampingRules.h"
#include "Chat.h"
#include "CharacterDatabase.h"
#include "Config.h"
#include "Creature.h"
#include "DBCStores.h"
#include "DataMap.h"
#include "GameObject.h"
#include "GameObjectModel.h"
#include "GameTime.h"
#include "Log.h"
#include "Map.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "StringConvert.h"
#include "TemporarySummon.h"
#include "Tokenize.h"
#include "World.h"
#include <algorithm>
#include <cmath>
#include <map>
#include <mutex>
#include <set>
#include <tuple>
#include <vector>

namespace
{
using namespace CoACamping;

struct CampingSettings
{
    bool Enabled = false;
    bool AllowSoloContributions = true;
    std::set<uint32> AllowedMaps;
    std::set<uint32> AllowedAreas;
    uint32 LifetimeSeconds = 900;
    uint32 RestSeconds = 60;
    uint32 TentRestSeconds = 30;
    uint32 TentCooldownSeconds = 3600;
    float TentRestedPercent = 5.0f;
    uint32 BenefitSeconds = 3600;
    uint32 ContributionCooldownSeconds = 3600;
    uint32 ScanMilliseconds = 1000;
    uint32 Capacity = 5;
    float BenefitRadius = 20.0f;
    float ContributionDistance = 5.0f;
    float MinimumSpacing = 40.0f;
};

CampingSettings Settings;

class ObjectCollisionScope
{
public:
    explicit ObjectCollisionScope(GameObject* object) : Object(object),
        Enabled(object && object->m_model && object->m_model->isEnabled())
    {
        if (Object)
            Object->EnableCollision(false);
    }

    ~ObjectCollisionScope()
    {
        if (Object)
            Object->EnableCollision(Enabled);
    }

private:
    GameObject* const Object;
    bool const Enabled;
};

struct Attachment
{
    Feature Kind;
    ObjectGuid Guid;
    TeamId Team;
};

struct Camp
{
    uint64 Id;
    ObjectGuid Fire;
    ObjectGuid Owner;
    ObjectGuid Controller;
    ObjectGuid RewardCaster;
    uint32 Phase;
    uint64 ExpiresAt;
    bool LifetimeSynchronized = false;
    std::vector<Attachment> Attachments;
    std::set<ObjectGuid> Contributors;
};

struct ContributionRequest
{
    ObjectGuid Player;
    ObjectGuid Controller;
    uint64 CampId;
    Feature Kind;
};

struct MapState : DataMap::Base
{
    uint64 NextId = 1;
    uint64 NextScan = 0;
    std::map<ObjectGuid, Camp> Camps;
    std::mutex RequestsMutex;
    std::vector<ContributionRequest> Requests;
    std::vector<ObjectGuid> Removals;
};

struct PlayerState : DataMap::Base
{
    ObjectGuid ActiveFire;
    ObjectGuid MenuController;
    uint64 MenuCampId = 0;
    RestProgress Rest;
    RestProgress TentRest;
    Position RestPosition;
    uint32 RestPhase = 0;
};

MapState* FindState(Map* map)
{
    return map ? map->CustomData.Get<MapState>(MapStateKey) : nullptr;
}

PlayerState& StateFor(Player* player)
{
    return *player->CustomData.GetDefault<PlayerState>(PlayerStateKey);
}

uint64 NowMs()
{
    return GameTime::GetGameTimeMS().count();
}

uint64 NowSeconds()
{
    return GameTime::GetCalendarTime().count();
}

Player* FindPlayerOnMap(Map* map, ObjectGuid guid)
{
    Player* player = ObjectAccessor::FindPlayer(guid);
    return player && player->IsInWorld() && player->FindMap() == map ? player : nullptr;
}

void Message(Player* player, char const* text)
{
    ChatHandler(player->GetSession()).SendSysMessage(text);
}

uint64 ReadSettingPair(Player const* player, uint32 index)
{
    PlayerSettingVector const* values = player->FindPlayerSettings(CooldownSettings);
    if (!values || values->size() <= index + 1)
        return 0;
    return uint64((*values)[index].value) | (uint64((*values)[index + 1].value) << 32);
}

void WriteSettingPair(Player* player, uint32 index, uint64 value)
{
    player->UpdatePlayerSetting(CooldownSettings, index, uint32(value));
    player->UpdatePlayerSetting(CooldownSettings, index + 1, uint32(value >> 32));
}

bool Available(Player const* player)
{
    return player->IsInWorld() && player->IsAlive() && !player->IsInCombat() && !player->IsFlying() &&
        !player->IsInFlight() && !player->GetTransport() && !player->GetVehicle() && !player->IsUnderWater();
}

bool AllowedLocation(Player const* player)
{
    Map* map = player->FindMap();
    return map && !map->Instanceable() && Settings.AllowedMaps.count(map->GetId()) &&
        (Settings.AllowedAreas.empty() || Settings.AllowedAreas.count(player->GetAreaId())) && player->IsOutdoors();
}

bool VisibleObject(Player const* player, GameObject* object)
{
    if (!object || !object->IsInWorld() || !player->IsInMap(object))
        return false;
    ObjectCollisionScope scope(object);
    return player->CanSeeOrDetect(object) &&
        player->IsWithinLOS(object->GetPositionX(), object->GetPositionY(), object->GetPositionZ());
}

bool Accessible(Player const* player, Camp const& camp, GameObject* fire)
{
    if (!fire || !fire->IsInWorld() || player->FindMap() != fire->FindMap() ||
        player->GetPhaseMask() != camp.Phase || fire->GetPhaseMask() != camp.Phase)
        return false;
    return VisibleObject(player, fire);
}

Camp* FindCamp(MapState* state, ObjectGuid controller, uint64 id = 0)
{
    if (!state)
        return nullptr;
    for (auto& [guid, camp] : state->Camps)
        if (camp.Controller == controller && (!id || camp.Id == id))
            return &camp;
    return nullptr;
}

void RemoveCamp(Map* map, ObjectGuid fireGuid, bool removeFire)
{
    MapState* state = FindState(map);
    if (!state)
        return;
    auto const found = state->Camps.find(fireGuid);
    if (found == state->Camps.end())
        return;
    Camp camp = std::move(found->second);
    state->Camps.erase(found);

    for (auto const& reference : map->GetPlayers())
    {
        Player* player = reference.GetSource();
        PlayerState* playerState = player->CustomData.Get<PlayerState>(PlayerStateKey);
        if (!playerState)
            continue;
        if (playerState->ActiveFire == fireGuid)
            playerState->ActiveFire.Clear();
        if (playerState->Rest.CampId == camp.Id)
            playerState->Rest.Reset();
        if (playerState->TentRest.CampId == camp.Id)
            playerState->TentRest.Reset();
        if (playerState->MenuController == camp.Controller)
        {
            playerState->MenuController.Clear();
            playerState->MenuCampId = 0;
        }
    }

    for (Attachment const& attachment : camp.Attachments)
    {
        if (IsService(attachment.Kind))
        {
            if (Creature* creature = map->GetCreature(attachment.Guid))
                creature->DespawnOrUnsummon();
        }
        else if (GameObject* object = map->GetGameObject(attachment.Guid))
            object->Delete();
    }
    if (GameObject* controller = map->GetGameObject(camp.Controller))
        controller->Delete();
    if (Creature* creature = map->GetCreature(camp.RewardCaster))
        creature->DespawnOrUnsummon();
    if (GameObject* fire = map->GetGameObject(fireGuid))
    {
        if (GameObject* trap = fire->GetLinkedTrap())
            trap->Delete();
        if (removeFire)
            fire->Delete();
    }
}

bool HasNearbyCamp(Player const* player, WorldObject const* position)
{
    MapState* state = FindState(player->FindMap());
    if (!state)
        return false;
    for (auto const& [guid, camp] : state->Camps)
        if (camp.ExpiresAt > NowMs() && camp.Phase == player->GetPhaseMask())
            if (GameObject* fire = player->FindMap()->GetGameObject(guid))
                if (position->IsWithinDistInMap(fire, Settings.MinimumSpacing))
                    return true;
    return false;
}

bool PlacementAllowed(Player* player, WorldObject* position)
{
    if (!Available(player) || !AllowedLocation(player) || StateFor(player).ActiveFire ||
        HasNearbyCamp(player, position))
        return false;
    ObjectCollisionScope scope(position->ToGameObject());
    Map* map = player->FindMap();
    float const height = map->GetHeight(player->GetPhaseMask(), position->GetPositionX(),
        position->GetPositionY(), position->GetPositionZ() + 2.0f);
    return std::isfinite(height) && std::abs(height - position->GetPositionZ()) <= 3.0f &&
        player->IsWithinLOS(position->GetPositionX(), position->GetPositionY(), position->GetPositionZ());
}

bool PropPosition(GameObject* fire, Player* owner, float offset, float angle, Position& position)
{
    ObjectCollisionScope scope(fire);
    position = fire->GetNearPosition(offset, angle);
    float const height = fire->GetMap()->GetHeight(fire->GetPhaseMask(), position.GetPositionX(),
        position.GetPositionY(), position.GetPositionZ() + 2.0f);
    if (!std::isfinite(height) || std::abs(height - position.GetPositionZ()) > 3.0f)
        return false;
    position.m_positionZ = height;
    float const eyeHeight = owner->GetCollisionHeight();
    if (!fire->GetMap()->isInLineOfSight(fire->GetPositionX(), fire->GetPositionY(),
        fire->GetPositionZ() + eyeHeight, position.GetPositionX(), position.GetPositionY(), height + eyeHeight,
        fire->GetPhaseMask(), LINEOFSIGHT_ALL_CHECKS, VMAP::ModelIgnoreFlags::Nothing))
        return false;
    position.SetOrientation(fire->GetOrientation() + angle + float(M_PI));
    return true;
}

GameObject* SummonProp(GameObject* fire, Player* owner, uint32 entry, float offset, float angle, uint32 seconds)
{
    Position position;
    if (!PropPosition(fire, owner, offset, angle, position))
        return nullptr;
    float const height = position.GetPositionZ();
    GameObject* prop = fire->SummonGameObject(entry, position.GetPositionX(), position.GetPositionY(), height,
        position.GetOrientation(), 0.0f, 0.0f, std::sin(position.GetOrientation() / 2.0f),
        std::cos(position.GetOrientation() / 2.0f), seconds);
    if (prop)
    {
        prop->SetOwnerGUID(owner->GetGUID());
        prop->SetSpellId(0);
    }
    return prop;
}

void CreateCamp(GameObject* fire)
{
    Player* owner = FindPlayerOnMap(fire->GetMap(), fire->GetOwnerGUID());
    if (!owner || !PlacementAllowed(owner, fire))
        return;
    MapState* state = FindState(fire->GetMap());
    if (!state || state->Camps.count(fire->GetGUID()))
        return;
    GameObject* controller = SummonProp(fire, owner, ControllerEntry, 1.6f, 2.0f,
        Settings.LifetimeSeconds);
    if (!controller)
    {
        Message(owner, "The campsite supplies could not be placed. This remains an ordinary campfire.");
        return;
    }
    Creature* rewardCaster = fire->SummonTrigger(fire->GetPositionX(), fire->GetPositionY(),
        fire->GetPositionZ(), fire->GetOrientation(), Settings.LifetimeSeconds * IN_MILLISECONDS);
    if (!rewardCaster)
    {
        controller->Delete();
        Message(owner, "The campsite could not be created. This remains an ordinary campfire.");
        return;
    }
    rewardCaster->SetUnitFlag(UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
    rewardCaster->SetReactState(REACT_PASSIVE);
    fire->SetRespawnTime(Settings.LifetimeSeconds);
    ObjectGuid const fireGuid = fire->GetGUID();
    state->Camps.emplace(fireGuid, Camp{state->NextId++, fireGuid, owner->GetGUID(), controller->GetGUID(),
        rewardCaster->GetGUID(), fire->GetPhaseMask(), NowMs() + uint64(Settings.LifetimeSeconds) * IN_MILLISECONDS});
    StateFor(owner).ActiveFire = fireGuid;
    ChatHandler(owner->GetSession()).PSendSysMessage(
        "Campsite ready for {} seconds. Use the campsite supplies to add a tent, chair, banner, candle or service bot.",
        Settings.LifetimeSeconds);
}

char const* FailureMessage(ContributionFailure failure)
{
    switch (failure)
    {
        case ContributionFailure::Expired: return "This campsite has expired.";
        case ContributionFailure::Inaccessible: return "This campsite is no longer accessible.";
        case ContributionFailure::TooFar: return "Move closer to the campsite supplies.";
        case ContributionFailure::Busy: return "Contribute while alive, out of combat, and on the ground.";
        case ContributionFailure::Skill: return "Your profession rank is too low for this contribution.";
        case ContributionFailure::Materials: return "You do not carry the materials listed for this contribution.";
        case ContributionFailure::Cooldown: return "Your shared camping contribution cooldown is still active.";
        case ContributionFailure::AlreadyContributed: return "You have already contributed to this campsite.";
        case ContributionFailure::Full: return "This campsite has no attachment slots remaining.";
        case ContributionFailure::FeaturePresent: return "This campsite already has this feature family.";
        default: return "The contribution could not be placed.";
    }
}

void Contribute(Map* map, ContributionRequest const& request)
{
    Player* player = FindPlayerOnMap(map, request.Player);
    FeatureDefinition const* feature = Definition(uint32(request.Kind));
    if (!player || !feature)
        return;
    Camp* camp = FindCamp(FindState(map), request.Controller, request.CampId);
    if (!camp)
    {
        Message(player, FailureMessage(ContributionFailure::Expired));
        return;
    }
    GameObject* fire = map->GetGameObject(camp->Fire);
    GameObject* controller = map->GetGameObject(camp->Controller);
    ObjectCollisionScope fireScope(fire);
    ContributionCheck check;
    check.Expired = NowMs() >= camp->ExpiresAt;
    check.Accessible = Accessible(player, *camp, fire) && VisibleObject(player, controller);
    check.InRange = controller && player->IsWithinDistInMap(controller, Settings.ContributionDistance);
    check.Available = Available(player);
    check.HasSkill = player->GetBaseSkillValue(feature->Skill) >= feature->Rank;
    check.HasMaterials = std::all_of(feature->Materials.begin(), feature->Materials.end(), [player](Material material)
    {
        return !material.Count || player->HasItemCount(material.Item, material.Count, false);
    });
    check.OnCooldown = CooldownActive(ReadSettingPair(player, 0), NowSeconds());
    check.AlreadyContributed = camp->Contributors.count(player->GetGUID());
    check.AllowSoloContributions = Settings.AllowSoloContributions;
    check.FeaturePresent = std::any_of(camp->Attachments.begin(), camp->Attachments.end(),
        [feature](Attachment const& attachment) { return Family(attachment.Kind) == Family(feature->Kind); });
    check.UsedSlots = uint32(camp->Attachments.size());
    check.Capacity = Settings.Capacity;
    ContributionFailure const failure = CheckContribution(check);
    if (failure != ContributionFailure::None)
    {
        Message(player, FailureMessage(failure));
        return;
    }
    uint32 const seconds = uint32((camp->ExpiresAt - NowMs() + IN_MILLISECONDS - 1) / IN_MILLISECONDS);
    ObjectGuid attached;
    if (IsService(feature->Kind))
    {
        Position position;
        if (PropPosition(fire, player, feature->Offset, feature->Angle, position))
            if (Creature* bot = fire->SummonCreature(feature->Entry, position, TEMPSUMMON_TIMED_DESPAWN,
                seconds * IN_MILLISECONDS))
            {
                bot->SetPhaseMask(camp->Phase, true);
                bot->SetReactState(REACT_PASSIVE);
                bot->SetUnitFlag(UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                attached = bot->GetGUID();
            }
    }
    else
    {
        uint32 const entry = feature->Kind == Feature::Banner && player->GetTeamId() == TEAM_HORDE
            ? HordeBannerEntry : feature->Entry;
        if (GameObject* prop = SummonProp(fire, player, entry, feature->Offset, feature->Angle, seconds))
            attached = prop->GetGUID();
    }
    if (!attached)
    {
        Message(player, "The contribution could not be placed. Your materials and camping cooldown are unchanged.");
        return;
    }
    camp->Attachments.push_back({feature->Kind, attached, player->GetTeamId()});
    camp->Contributors.insert(player->GetGUID());
    for (Material material : feature->Materials)
        if (material.Count)
            player->DestroyItemCount(material.Item, material.Count, true);
    if (!Settings.AllowSoloContributions)
        WriteSettingPair(player, 0, NowSeconds() + Settings.ContributionCooldownSeconds);
    CharacterDatabaseTransaction transaction = CharacterDatabase.BeginTransaction();
    player->SaveInventoryAndGoldToDB(transaction);
    if (!Settings.AllowSoloContributions)
        transaction->Append(PlayerSettingsStore::PrepareReplaceStatement(player->GetGUID().GetCounter(),
            CooldownSettings, *player->FindPlayerSettings(CooldownSettings)));
    CharacterDatabase.CommitTransaction(transaction);
    if (Settings.AllowSoloContributions)
        ChatHandler(player->GetSession()).PSendSysMessage("{} placed.", feature->Name);
    else
        ChatHandler(player->GetSession()).PSendSysMessage("{} placed. The shared camping cooldown has started.",
            feature->Name);
}

template<std::size_t Size>
bool Contains(std::array<uint32, Size> const& family, uint32 root)
{
    return std::find(family.begin(), family.end(), root) != family.end();
}

uint32 SpellRoot(uint32 spell)
{
    uint32 root = sSpellMgr->GetFirstSpellInChain(spell);
    return root ? root : spell;
}

bool EquivalentReward(uint32 spell, uint32 reward)
{
    uint32 const root = SpellRoot(spell);
    return reward == RewardSpell ? Contains(IntellectFamilies, root) :
        reward == SpiritSpell ? Contains(SpiritFamilies, root) : Contains(CritFamilies, root);
}

uint32 CasterSetting(uint32 reward)
{
    return reward == RewardSpell ? 2 : reward == SpiritSpell ? 6 : 8;
}

Aura* CampingAura(Player* player, uint32 reward)
{
    uint64 const caster = ReadSettingPair(player, CasterSetting(reward));
    if (!caster)
        return nullptr;
    return player->GetAura(reward, ObjectGuid(caster));
}

void RewardAura(Player* player, Camp const& camp, uint32 reward, int32 amount)
{
    Aura* campingAura = CampingAura(player, reward);
    for (auto const& [spell, application] : player->GetAppliedAuras())
        if (application->GetBase() != campingAura && EquivalentReward(spell, reward))
            return;
    if (!campingAura)
    {
        Creature* caster = player->FindMap()->GetCreature(camp.RewardCaster);
        if (!caster)
            return;
        campingAura = caster->AddAura(sSpellMgr->GetSpellInfo(reward), RewardEffectMask, player);
        if (!campingAura || campingAura->IsRemoved())
            return;
        WriteSettingPair(player, CasterSetting(reward), campingAura->GetCasterGUID().GetRawValue());
    }
    if (AuraEffect* effect = campingAura->GetEffect(EFFECT_0))
        effect->ChangeAmount(amount);
    campingAura->SetMaxDuration(Settings.BenefitSeconds * IN_MILLISECONDS);
    campingAura->SetDuration(Settings.BenefitSeconds * IN_MILLISECONDS);
}

void Reward(Player* player, Camp const& camp)
{
    for (Attachment const& attachment : camp.Attachments)
        switch (attachment.Kind)
        {
            case Feature::Candle:
                RewardAura(player, camp, RewardSpell, RewardIntellect);
                break;
            case Feature::Chair:
                RewardAura(player, camp, CritSpell, RewardCrit);
                break;
            case Feature::Banner:
                if (attachment.Team == player->GetTeamId())
                    RewardAura(player, camp, SpiritSpell, BannerSpirit(player->GetLevel()));
                break;
            default:
                break;
        }
    Message(player, "You finish resting at the campsite.");
}

bool HasFeature(Camp const& camp, Feature feature)
{
    return std::any_of(camp.Attachments.begin(), camp.Attachments.end(),
        [feature](Attachment const& attachment) { return attachment.Kind == feature; });
}

bool HasRestReward(Player const* player, Camp const& camp)
{
    return std::any_of(camp.Attachments.begin(), camp.Attachments.end(), [player](Attachment const& attachment)
    {
        return attachment.Kind == Feature::Candle || attachment.Kind == Feature::Chair ||
            (attachment.Kind == Feature::Banner && attachment.Team == player->GetTeamId());
    });
}

void RewardTent(Player* player)
{
    if (CooldownActive(ReadSettingPair(player, 4), NowSeconds()))
        return;
    float const current = player->GetRestBonus();
    float const floor = float(player->GetUInt32Value(PLAYER_NEXT_LEVEL_XP)) * Settings.TentRestedPercent / 100.0f;
    if (current >= floor || player->GetLevel() >= sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL))
        return;
    player->SetRestBonus(floor);
    if (player->GetRestBonus() <= current)
        return;
    WriteSettingPair(player, 4, NowSeconds() + Settings.TentCooldownSeconds);
    CharacterDatabaseTransaction transaction = CharacterDatabase.BeginTransaction();
    player->SaveToDB(transaction, false, false);
    CharacterDatabase.CommitTransaction(transaction);
    Message(player, "Resting at the camp tent replenishes your rested experience.");
}

void SuppressConflictingRewards(Player* player, Aura const* applied = nullptr)
{
    for (uint32 reward : {RewardSpell, SpiritSpell, CritSpell})
    {
        Aura* campingAura = CampingAura(player, reward);
        if (!campingAura || campingAura == applied)
            continue;
        if (applied)
        {
            if (EquivalentReward(applied->GetId(), reward))
                campingAura->Remove();
            continue;
        }
        for (auto const& [spell, application] : player->GetAppliedAuras())
            if (application->GetBase() != campingAura && EquivalentReward(spell, reward))
            {
                campingAura->Remove();
                break;
            }
    }
}

bool RestEligible(Player const* player)
{
    return Available(player) && player->IsSitState() && !player->isMoving();
}

void UpdateRest(Map* map, MapState& state, uint64 now)
{
    for (auto const& reference : map->GetPlayers())
    {
        Player* player = reference.GetSource();
        PlayerState& playerState = StateFor(player);
        SuppressConflictingRewards(player);
        Camp const* nearest = nullptr;
        Camp const* nearestTent = nullptr;
        float nearestDistance = Settings.BenefitRadius;
        float nearestTentDistance = Settings.BenefitRadius;
        if (RestEligible(player))
            for (auto const& [guid, camp] : state.Camps)
            {
                GameObject* fire = map->GetGameObject(guid);
                if (!Accessible(player, camp, fire))
                    continue;
                float const distance = player->GetDistance(fire);
                if (distance <= nearestDistance && HasRestReward(player, camp))
                {
                    nearest = &camp;
                    nearestDistance = distance;
                }
                if (distance <= nearestTentDistance && HasFeature(camp, Feature::Tent))
                {
                    nearestTent = &camp;
                    nearestTentDistance = distance;
                }
            }
        if (playerState.Rest.Observe(nearest ? nearest->Id : 0, now,
            uint64(Settings.RestSeconds) * IN_MILLISECONDS, nearest != nullptr))
            Reward(player, *nearest);
        if (playerState.TentRest.Observe(nearestTent ? nearestTent->Id : 0, now,
            uint64(Settings.TentRestSeconds) * IN_MILLISECONDS, nearestTent != nullptr))
            RewardTent(player);
        playerState.RestPosition = player->GetPosition();
        playerState.RestPhase = player->GetPhaseMask();
    }
}

void UpdateMap(Map* map)
{
    MapState* state = FindState(map);
    if (!state)
        return;
    std::vector<ObjectGuid> departures;
    {
        std::lock_guard<std::mutex> lock(state->RequestsMutex);
        departures.swap(state->Removals);
    }
    for (ObjectGuid guid : departures)
        RemoveCamp(map, guid, true);
    uint64 const now = NowMs();
    std::vector<ObjectGuid> removed;
    for (auto& [guid, camp] : state->Camps)
    {
        GameObject* fire = map->GetGameObject(guid);
        Player* owner = FindPlayerOnMap(map, camp.Owner);
        GameObject* controller = map->GetGameObject(camp.Controller);
        if (!owner || owner->GetPhaseMask() != camp.Phase || !fire || !fire->IsInWorld() ||
            fire->GetPhaseMask() != camp.Phase || !controller || !controller->IsInWorld() ||
            !map->GetCreature(camp.RewardCaster) || now >= camp.ExpiresAt)
        {
            removed.push_back(guid);
            continue;
        }
        if (!camp.LifetimeSynchronized)
        {
            if (GameObject* trap = fire->GetLinkedTrap())
                trap->SetRespawnTime(Settings.LifetimeSeconds);
            camp.LifetimeSynchronized = true;
        }
        std::erase_if(camp.Attachments, [map](Attachment const& attachment)
        {
            WorldObject* object = IsService(attachment.Kind)
                ? static_cast<WorldObject*>(map->GetCreature(attachment.Guid)) : map->GetGameObject(attachment.Guid);
            return !object || !object->IsInWorld();
        });
    }
    for (ObjectGuid guid : removed)
        RemoveCamp(map, guid, true);
    std::vector<ContributionRequest> requests;
    {
        std::lock_guard<std::mutex> lock(state->RequestsMutex);
        requests.swap(state->Requests);
    }
    for (ContributionRequest const& request : requests)
        Contribute(map, request);
    if (now >= state->NextScan)
    {
        state->NextScan = now + Settings.ScanMilliseconds;
        UpdateRest(map, *state, now);
    }
}

void RequireMapping(bool valid, char const* mapping)
{
    if (valid)
        return;
    LOG_FATAL("server.loading", "CoA Camping: invalid or missing {}. Check camping mappings and pending SQL.", mapping);
    ABORT();
}

std::set<uint32> ReadAllowlist(char const* option, char const* fallback)
{
    std::set<uint32> values;
    std::string const setting = sConfigMgr->GetOption<std::string>(option, fallback);
    for (std::string_view value : Acore::Tokenize(setting, ',', false))
    {
        auto const parsed = Acore::StringTo<uint32>(value);
        RequireMapping(parsed.has_value(), option);
        values.insert(*parsed);
    }
    return values;
}

class camping_world : public WorldScript
{
public:
    camping_world() : WorldScript("camping_world", {WORLDHOOK_ON_BEFORE_WORLD_INITIALIZED}) { }

    void OnBeforeWorldInitialized() override
    {
        Settings.Enabled = sConfigMgr->GetOption<bool>("CoACamping.Enable", false);
        if (!Settings.Enabled)
            return;
        Settings.AllowSoloContributions = sConfigMgr->GetOption<bool>("CoACamping.AllowSoloContributions", true);
        Settings.AllowedMaps = ReadAllowlist("CoACamping.AllowedMaps", "0,1,530,571");
        Settings.AllowedAreas = ReadAllowlist("CoACamping.AllowedAreas", "");
        Settings.LifetimeSeconds = sConfigMgr->GetOption<uint32>("CoACamping.LifetimeSeconds", 900);
        Settings.RestSeconds = sConfigMgr->GetOption<uint32>("CoACamping.RestSeconds", 60);
        Settings.TentRestSeconds = sConfigMgr->GetOption<uint32>("CoACamping.TentRestSeconds", 30);
        Settings.TentCooldownSeconds = sConfigMgr->GetOption<uint32>("CoACamping.TentCooldownSeconds", 3600);
        Settings.TentRestedPercent = sConfigMgr->GetOption<float>("CoACamping.TentRestedPercent", 5.0f);
        Settings.BenefitSeconds = sConfigMgr->GetOption<uint32>("CoACamping.BenefitSeconds", 3600);
        Settings.ContributionCooldownSeconds =
            sConfigMgr->GetOption<uint32>("CoACamping.ContributionCooldownSeconds", 3600);
        Settings.ScanMilliseconds = sConfigMgr->GetOption<uint32>("CoACamping.ScanMilliseconds", 1000);
        Settings.Capacity = sConfigMgr->GetOption<uint32>("CoACamping.Capacity", 5);
        Settings.BenefitRadius = sConfigMgr->GetOption<float>("CoACamping.BenefitRadius", 20.0f);
        Settings.ContributionDistance = sConfigMgr->GetOption<float>("CoACamping.ContributionDistance", 5.0f);
        Settings.MinimumSpacing = sConfigMgr->GetOption<float>("CoACamping.MinimumSpacing", 40.0f);
        RequireMapping(!Settings.AllowedMaps.empty(), "allowed maps");
        RequireMapping(Settings.LifetimeSeconds > 0 && Settings.LifetimeSeconds <= 86400, "camp lifetime");
        RequireMapping(Settings.RestSeconds > 0 && Settings.RestSeconds < Settings.LifetimeSeconds, "rest duration");
        RequireMapping(Settings.TentRestSeconds > 0 && Settings.TentRestSeconds < Settings.LifetimeSeconds,
            "tent rest duration");
        RequireMapping(Settings.TentCooldownSeconds > 0 && Settings.TentCooldownSeconds <= 604800,
            "tent cooldown");
        RequireMapping(std::isfinite(Settings.TentRestedPercent) && Settings.TentRestedPercent > 0.0f &&
            Settings.TentRestedPercent <= 100.0f, "tent rested percent");
        RequireMapping(Settings.BenefitSeconds > 0 && Settings.BenefitSeconds <= 86400, "benefit duration");
        RequireMapping(Settings.ContributionCooldownSeconds > 0 && Settings.ContributionCooldownSeconds <= 604800,
            "contribution cooldown");
        RequireMapping(Settings.ScanMilliseconds > 0 && Settings.ScanMilliseconds <= 1000, "scan interval");
        RequireMapping(Settings.Capacity > 0 && Settings.Capacity <= 10, "capacity");
        RequireMapping(std::isfinite(Settings.BenefitRadius) && Settings.BenefitRadius > 0.0f &&
            Settings.BenefitRadius <= 100.0f, "benefit radius");
        RequireMapping(std::isfinite(Settings.ContributionDistance) && Settings.ContributionDistance > 0.0f &&
            Settings.ContributionDistance <= INTERACTION_DISTANCE, "contribution distance");
        RequireMapping(std::isfinite(Settings.MinimumSpacing) &&
            Settings.MinimumSpacing >= Settings.BenefitRadius * 2.0f, "minimum spacing");
        for (uint32 id : Settings.AllowedMaps)
            RequireMapping(sMapStore.LookupEntry(id) && !sMapStore.LookupEntry(id)->Instanceable(), "outdoor map");
        for (uint32 id : Settings.AllowedAreas)
            RequireMapping(sAreaTableStore.LookupEntry(id), "outdoor area");
        GameObjectTemplate const* fire = sObjectMgr->GetGameObjectTemplate(FireEntry);
        GameObjectTemplate const* controller = sObjectMgr->GetGameObjectTemplate(ControllerEntry);
        GameObjectTemplate const* candle = sObjectMgr->GetGameObjectTemplate(CandleEntry);
        RequireMapping(fire && fire->type == GAMEOBJECT_TYPE_SPELL_FOCUS && fire->displayId == FireDisplay,
            "spell-818 campfire template");
        RequireMapping(controller && controller->type == GAMEOBJECT_TYPE_GOOBER &&
            controller->displayId == ControllerDisplay && controller->name == "Campsite Supplies",
            "controller template");
        RequireMapping(candle && candle->type == GAMEOBJECT_TYPE_GENERIC &&
            candle->displayId == CandleDisplay && candle->name == "Incense Candle", "incense candle template");
        for (uint32 id : {FireDisplay, ControllerDisplay, CandleDisplay})
            RequireMapping(sGameObjectDisplayInfoStore.LookupEntry(id), "stock gameobject display");
        for (auto const& [entry, display, type, name] : std::array<std::tuple<uint32, uint32, uint32, char const*>, 4>{{
            {TentEntry, TentDisplay, GAMEOBJECT_TYPE_GENERIC, "Camp Tent"},
            {ChairEntry, ChairDisplay, GAMEOBJECT_TYPE_CHAIR, "Camp Chair"},
            {AllianceBannerEntry, AllianceBannerDisplay, GAMEOBJECT_TYPE_GENERIC, "Alliance Camp Banner"},
            {HordeBannerEntry, HordeBannerDisplay, GAMEOBJECT_TYPE_GENERIC, "Horde Camp Banner"}}})
        {
            GameObjectTemplate const* object = sObjectMgr->GetGameObjectTemplate(entry);
            RequireMapping(object && object->displayId == display && object->type == type && object->name == name,
                "camp furniture template");
            RequireMapping(sGameObjectDisplayInfoStore.LookupEntry(display), "camp furniture display");
            if (entry == ChairEntry)
                RequireMapping(object->chair.slots == 1 && object->chair.height == 1, "chair seat");
        }
        for (uint32 entry : {ReagentBotEntry, RepairBotEntry})
        {
            CreatureTemplate const* bot = sObjectMgr->GetCreatureTemplate(entry);
            uint32 const flags = UNIT_NPC_FLAG_VENDOR | (entry == RepairBotEntry ? UNIT_NPC_FLAG_REPAIR : 0);
            RequireMapping(bot && bot->Name == (entry == RepairBotEntry ? "Camp Repair Bot" : "Camp Reagent Bot") &&
                bot->faction == 190 && bot->npcflag == flags && bot->Models.size() == 1 &&
                bot->Models.front().CreatureDisplayID == BotDisplay, "camp service template");
            VendorItemData const* vendor = sObjectMgr->GetNpcVendorItemList(entry);
            RequireMapping(vendor && vendor->GetItemCount() == VendorItems.size(), "camp reagent inventory");
            for (uint32 item : VendorItems)
                RequireMapping(vendor->FindItemCostPair(item, 0) &&
                    !vendor->FindItemCostPair(item, 0)->maxcount && !vendor->FindItemCostPair(item, 0)->incrtime,
                    "camp reagent row");
        }
        RequireMapping(sCreatureDisplayInfoStore.LookupEntry(BotDisplay), "camp service display");
        RequireMapping(sObjectMgr->GetCreatureTemplate(WORLD_TRIGGER), "reward caster template");
        RequireMapping(sSkillLineStore.LookupEntry(Herbalism), "Herbalism");
        for (Material material : CandleMaterials)
            RequireMapping(sObjectMgr->GetItemTemplate(material.Item), "incense materials");
        for (FeatureDefinition const& feature : Features)
        {
            RequireMapping(sSkillLineStore.LookupEntry(feature.Skill), "camp profession");
            for (Material material : feature.Materials)
                if (material.Count)
                    RequireMapping(sObjectMgr->GetItemTemplate(material.Item), "camp contribution materials");
        }
        SpellInfo const* fireSpell = sSpellMgr->GetSpellInfo(FireSpell);
        SpellInfo const* reward = sSpellMgr->GetSpellInfo(RewardSpell);
        RequireMapping(fireSpell && fireSpell->Effects[EFFECT_0].Effect == SPELL_EFFECT_TRANS_DOOR &&
            fireSpell->Effects[EFFECT_0].MiscValue == int32(FireEntry), "stock campfire spell");
        RequireMapping(reward && reward->Effects[EFFECT_0].ApplyAuraName == SPELL_AURA_MOD_STAT &&
            reward->Effects[EFFECT_0].MiscValue == STAT_INTELLECT, "stock Intellect aura");
        SpellInfo const* spirit = sSpellMgr->GetSpellInfo(SpiritSpell);
        SpellInfo const* crit = sSpellMgr->GetSpellInfo(CritSpell);
        RequireMapping(spirit && spirit->Effects[EFFECT_0].ApplyAuraName == SPELL_AURA_MOD_STAT &&
            spirit->Effects[EFFECT_0].MiscValue == STAT_SPIRIT, "Spirit aura");
        RequireMapping(crit && crit->Effects[EFFECT_0].Effect == SPELL_EFFECT_APPLY_AURA &&
            crit->Effects[EFFECT_0].ApplyAuraName == SPELL_AURA_MOD_CRIT_PCT, "all-crit aura");
        LOG_INFO("server.loading", "CoA Camping enabled: tents, chairs, faction banners, incense and service bots; "
            "{} second camps, {} second rests, {} second tent rests; solo contributions {}, {} feature slots.",
            Settings.LifetimeSeconds, Settings.RestSeconds, Settings.TentRestSeconds,
            Settings.AllowSoloContributions, Settings.Capacity);
    }
};

class camping_objects : public AllGameObjectScript
{
public:
    camping_objects() : AllGameObjectScript("camping_objects") { }

    void OnGameObjectAddWorld(GameObject* object) override
    {
        if (Settings.Enabled && object->GetEntry() == FireEntry && object->GetSpellId() == FireSpell &&
            object->GetOwnerGUID().IsPlayer() && !object->GetSpawnId())
            CreateCamp(object);
    }

    void OnGameObjectRemoveWorld(GameObject* object) override
    {
        if (Settings.Enabled && object->GetEntry() == FireEntry)
            RemoveCamp(object->GetMap(), object->GetGUID(), false);
    }

    bool CanGameObjectGossipHello(Player* player, GameObject* object) override
    {
        if (!Settings.Enabled || object->GetEntry() != ControllerEntry)
            return false;
        ClearGossipMenuFor(player);
        PlayerState& playerState = StateFor(player);
        playerState.MenuController.Clear();
        playerState.MenuCampId = 0;
        Camp* camp = FindCamp(FindState(player->FindMap()), object->GetGUID());
        if (!camp || NowMs() >= camp->ExpiresAt ||
            !Accessible(player, *camp, player->FindMap()->GetGameObject(camp->Fire)) ||
            !player->IsWithinDistInMap(object, Settings.ContributionDistance))
        {
            Message(player, "This campsite is not within reach.");
            return true;
        }
        playerState.MenuController = object->GetGUID();
        playerState.MenuCampId = camp->Id;
        for (FeatureDefinition const& feature : Features)
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, feature.Menu, GOSSIP_SENDER_MAIN, uint32(feature.Kind));
        uint64 const expires = ReadSettingPair(player, 0);
        if (!Settings.AllowSoloContributions && CooldownActive(expires, NowSeconds()))
            ChatHandler(player->GetSession()).PSendSysMessage("Camping contribution available in {} seconds.",
                expires - NowSeconds());
        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, object->GetGUID());
        return true;
    }

    bool CanGameObjectGossipSelect(Player* player, GameObject* object, uint32 sender, uint32 action) override
    {
        if (!Settings.Enabled || object->GetEntry() != ControllerEntry)
            return false;
        CloseGossipMenuFor(player);
        PlayerState& playerState = StateFor(player);
        MapState* state = FindState(player->FindMap());
        if (state && sender == GOSSIP_SENDER_MAIN && Definition(action) &&
            playerState.MenuController == object->GetGUID() && playerState.MenuCampId)
        {
            std::lock_guard<std::mutex> lock(state->RequestsMutex);
            state->Requests.push_back({player->GetGUID(), object->GetGUID(), playerState.MenuCampId, Feature(action)});
        }
        playerState.MenuController.Clear();
        playerState.MenuCampId = 0;
        return true;
    }
};

class camping_maps : public AllMapScript
{
public:
    camping_maps() : AllMapScript("camping_maps", {ALLMAPHOOK_ON_CREATE_MAP, ALLMAPHOOK_ON_DESTROY_MAP,
        ALLMAPHOOK_ON_MAP_UPDATE, ALLMAPHOOK_ON_PLAYER_LEAVE_ALL}) { }

    void OnCreateMap(Map* map) override
    {
        map->CustomData.GetDefault<MapState>(MapStateKey);
    }

    void OnDestroyMap(Map* map) override
    {
        map->CustomData.Erase(MapStateKey);
    }

    void OnMapUpdate(Map* map, uint32) override
    {
        if (Settings.Enabled)
            UpdateMap(map);
    }

    void OnPlayerLeaveAll(Map* map, Player* player) override
    {
        if (PlayerState* state = player->CustomData.Get<PlayerState>(PlayerStateKey))
        {
            if (state->ActiveFire)
                if (MapState* mapState = FindState(map))
                {
                    std::lock_guard<std::mutex> lock(mapState->RequestsMutex);
                    mapState->Removals.push_back(state->ActiveFire);
                }
            state->ActiveFire.Clear();
            state->MenuController.Clear();
            state->MenuCampId = 0;
            state->Rest.Reset();
            state->TentRest.Reset();
        }
    }
};

class camping_players : public PlayerScript
{
public:
    camping_players() : PlayerScript("camping_players", {PLAYERHOOK_ON_BEFORE_UPDATE}) { }

    void OnPlayerBeforeUpdate(Player* player, uint32) override
    {
        PlayerState* state = player->CustomData.Get<PlayerState>(PlayerStateKey);
        if (Settings.Enabled && state && (state->Rest.CampId || state->TentRest.CampId) && (!RestEligible(player) ||
            player->GetPhaseMask() != state->RestPhase ||
            player->GetExactDist(&state->RestPosition) > 0.01f))
        {
            state->Rest.Reset();
            state->TentRest.Reset();
        }
    }
};

class camping_spells : public AllSpellScript
{
public:
    camping_spells() : AllSpellScript("camping_spells",
        {ALLSPELLHOOK_ON_SPELL_CHECK_CAST, ALLSPELLHOOK_ON_CALCULATED_TARGET}) { }

    void OnSpellCheckCast(Spell* spell, bool, SpellCastResult& result) override
    {
        if (!Settings.Enabled || result != SPELL_CAST_OK || spell->GetSpellInfo()->Id != FireSpell)
            return;
        if (Player* player = spell->GetCaster()->ToPlayer())
            if (!PlacementAllowed(player, player))
                result = SPELL_FAILED_NOT_HERE;
    }

    void OnSpellCalculatedTarget(Spell* spell, Unit* target, TargetInfo&) override
    {
        if (!Settings.Enabled || !target->IsPlayer())
            return;
        for (uint32 reward : {RewardSpell, SpiritSpell, CritSpell})
            if (EquivalentReward(spell->GetSpellInfo()->Id, reward))
                if (Aura* aura = CampingAura(target->ToPlayer(), reward))
                    aura->Remove();
    }
};

class camping_auras : public UnitScript
{
public:
    camping_auras() : UnitScript("camping_auras", true, {UNITHOOK_ON_AURA_APPLY}) { }

    void OnAuraApply(Unit* unit, Aura* aura) override
    {
        if (Settings.Enabled)
            if (Player* player = unit->ToPlayer())
                SuppressConflictingRewards(player, aura);
    }
};
}

void AddSC_coa_camping()
{
    new camping_world();
    new camping_objects();
    new camping_maps();
    new camping_players();
    new camping_spells();
    new camping_auras();
}
