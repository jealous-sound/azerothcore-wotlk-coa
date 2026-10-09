/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionRunemasterTalents.h"
#include "Creature.h"
#include "EventMap.h"
#include "GameTime.h"
#include "Map.h"
#include "MotionMaster.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Spell.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "TemporarySummon.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include <algorithm>
#include <map>
#include <mutex>
#include <vector>

namespace
{
enum RunemasterTravelSpells : uint32
{
    SPELL_ECHO_RUNE = 500270,
    SPELL_ECHO_RETURN = 500272,
    SPELL_WARPDAGGER = 500287,
    SPELL_RUNESHROUD = 500288,
    SPELL_SHROUDWALKER = 705563,
    SPELL_WARP_READY = 500289,
    SPELL_WARP_DAMAGE = 500495,
    SPELL_WARP = 500587,
    SPELL_WARP_VISUAL = 500588,
    SPELL_WARP_SUMMON = 500606
};

enum RunemasterTravelCreatures : uint32
{
    NPC_ECHO_RUNE = 50063,
    NPC_WARPDAGGER = 51335
};

enum RunemasterTravelEvents : uint32
{
    EVENT_TRAVEL_OWNER_CHECK = 1,
    POINT_WARP_DESTINATION = 1
};

constexpr uint32 SMSG_SET_ACTION_BUTTON_SPELL = 0x076A;
constexpr uint8 kClientPossessionFirstButton = 120, kClientPossessionLastButton = 131;

std::mutex travelMutex;
std::map<std::pair<ObjectGuid, uint32>, ObjectGuid> travelMarkers;

struct ButtonSwapWindow
{
    uint32 untilMs = 0;
    uint32 refreshDueMs = 0;
    std::vector<uint8> swapped;
};

constexpr uint32 kButtonStateRefreshDelayMs = 300;

std::mutex buttonSwapMutex;
std::map<ObjectGuid, ButtonSwapWindow> buttonSwapWindows;

void ArmButtonSwap(Player* player)
{
    if (!player->IsInWorld())
        return;
    std::lock_guard<std::mutex> lock(buttonSwapMutex);
    uint32 now = uint32(GameTime::GetGameTimeMS().count());
    for (auto itr = buttonSwapWindows.begin(); itr != buttonSwapWindows.end();)
    {
        if (itr->second.untilMs + 10000 < now)
            itr = buttonSwapWindows.erase(itr);
        else
            ++itr;
    }
    ButtonSwapWindow& window = buttonSwapWindows[player->GetGUID()];
    window.untilMs = now + 2000;
    window.swapped.clear();
}

void SweepButtonSwapCopies(Player* player)
{
    if (!player->IsInWorld())
        return;
    constexpr uint8 kPage = 12;
    for (uint8 first = 0; first < MAX_ACTION_BUTTONS; first += kPage)
        for (uint8 slot = first; slot < first + kPage && slot < MAX_ACTION_BUTTONS; ++slot)
        {
            if (slot >= kClientPossessionFirstButton && slot <= kClientPossessionLastButton)
                continue;
            ActionButton const* button = player->GetActionButton(slot);
            if (button && button->GetAction())
                continue;
            WorldPacket data(SMSG_SET_ACTION_BUTTON_SPELL, 5);
            data << uint8(slot) << uint32(0);
            player->GetSession()->SendPacket(&data);
            break;
        }
}

void FlipTravelButtons(Player* player, uint32 fromSpell, uint32 toSpell)
{
    if (!player->IsInWorld())
        return;
    std::lock_guard<std::mutex> lock(buttonSwapMutex);
    ButtonSwapWindow& window = buttonSwapWindows[player->GetGUID()];
    bool flipped = false;
    for (uint8 slot = 0; slot < MAX_ACTION_BUTTONS; ++slot)
    {
        ActionButton const* button = player->GetActionButton(slot);
        if (!button || button->GetType() != ACTION_BUTTON_SPELL || button->GetAction() != fromSpell)
            continue;
        player->addActionButton(slot, toSpell, ACTION_BUTTON_SPELL);
        window.swapped.push_back(slot);
        WorldPacket data(SMSG_SET_ACTION_BUTTON_SPELL, 5);
        data << uint8(slot) << uint32(toSpell);
        player->GetSession()->SendPacket(&data);
        flipped = true;
    }
    if (flipped)
        window.refreshDueMs = uint32(GameTime::GetGameTimeMS().count()) + kButtonStateRefreshDelayMs;
}

void RefreshButtonSwapState(Player* player)
{
    if (!player->IsInWorld())
        return;
    bool refresh = false;
    {
        std::lock_guard<std::mutex> lock(buttonSwapMutex);
        auto itr = buttonSwapWindows.find(player->GetGUID());
        if (itr != buttonSwapWindows.end() && itr->second.refreshDueMs &&
            uint32(GameTime::GetGameTimeMS().count()) >= itr->second.refreshDueMs)
        {
            itr->second.refreshDueMs = 0;
            refresh = true;
        }
    }
    if (refresh)
        player->SendActionButtons(1);
}

uint32 ReturnSpell(uint32 spell) { return spell == SPELL_ECHO_RUNE ? SPELL_ECHO_RETURN : SPELL_WARP; }
uint32 TravelAura(uint32 spell) { return spell == SPELL_ECHO_RUNE ? SPELL_ECHO_RUNE : SPELL_WARP_READY; }
uint32 TravelEntry(uint32 spell) { return spell == SPELL_ECHO_RUNE ? NPC_ECHO_RUNE : NPC_WARPDAGGER; }

uint32 TravelButtonPartner(uint32 spell)
{
    switch (spell)
    {
        case SPELL_ECHO_RUNE: return SPELL_ECHO_RETURN;
        case SPELL_ECHO_RETURN: return SPELL_ECHO_RUNE;
        case SPELL_WARPDAGGER: return SPELL_WARP;
        case SPELL_WARP: return SPELL_WARPDAGGER;
        default: return 0;
    }
}

ObjectGuid MarkerGuid(ObjectGuid owner, uint32 spell)
{
    std::lock_guard<std::mutex> lock(travelMutex);
    auto itr = travelMarkers.find({owner, spell});
    return itr == travelMarkers.end() ? ObjectGuid::Empty : itr->second;
}

void ForgetMarker(ObjectGuid owner, uint32 spell, ObjectGuid marker)
{
    std::lock_guard<std::mutex> lock(travelMutex);
    auto itr = travelMarkers.find({owner, spell});
    if (itr != travelMarkers.end() && itr->second == marker)
        travelMarkers.erase(itr);
}

bool CanTravel(Player* player)
{
    return player && player->getClass() == CLASS_SPIRIT_MAGE && player->IsAlive() && player->IsInWorld() &&
        !player->IsBeingTeleported() && !player->IsInFlight() && !player->GetTransport() && !player->GetVehicle();
}

Creature* FindMarker(Player* player, uint32 spell)
{
    ObjectGuid guid = MarkerGuid(player->GetGUID(), spell);
    Creature* marker = guid && player->IsInWorld() ? player->GetMap()->GetCreature(guid) : nullptr;
    return marker && marker->GetOwnerGUID() == player->GetGUID() && marker->GetEntry() == TravelEntry(spell)
        ? marker : nullptr;
}

void ClearTravel(Player* player, uint32 spell)
{
    ObjectGuid guid = MarkerGuid(player->GetGUID(), spell);
    Creature* marker = FindMarker(player, spell);
    bool traveling = player->GetTemporarySpellReplacement(spell) != spell;
    ForgetMarker(player->GetGUID(), spell, guid);
    player->SetTemporarySpellReplacement(spell, 0, false);
    if (traveling)
    {
        ArmButtonSwap(player);
        FlipTravelButtons(player, ReturnSpell(spell), spell);
        SweepButtonSwapCopies(player);
    }
    player->RemoveAurasDueToSpell(TravelAura(spell), player->GetGUID());
    if (marker)
        marker->DespawnOrUnsummon();
}

bool HasTravelMarker(Player* player, uint32 spell)
{
    if (!CanTravel(player) || !player->HasActiveSpell(spell))
        return false;
    Creature* marker = FindMarker(player, spell);
    return marker && marker->IsAlive() && marker->IsInWorld() && player->InSamePhase(marker);
}

bool CanReturn(Player* player, uint32 spell)
{
    return HasTravelMarker(player, spell);
}

bool ReturnToMarker(Player* player, uint32 spell, Position* arrival = nullptr)
{
    if (!CanReturn(player, spell))
        return false;
    Position destination = FindMarker(player, spell)->GetPosition();
    if (arrival)
        *arrival = destination;
    ClearTravel(player, spell);
    player->NearTeleportTo(destination, true);
    return true;
}

bool StartTravel(Player* player, uint32 spell)
{
    if (!CanTravel(player) || !player->HasActiveSpell(spell))
        return false;
    SpellInfo const* info = sSpellMgr->GetSpellInfo(spell == SPELL_ECHO_RUNE ? spell : SPELL_WARP_SUMMON);
    if (!info)
        return false;
    int32 duration = info->GetDuration();
    player->ApplySpellMod(info->Id, SPELLMOD_DURATION, duration);
    if (duration <= 0)
        return false;
    TempSummon* marker = player->SummonCreature(TravelEntry(spell), player->GetPosition(),
        TEMPSUMMON_MANUAL_DESPAWN);
    if (!marker)
        return false;
    ClearTravel(player, spell);
    marker->SetUInt32Value(UNIT_CREATED_BY_SPELL, spell);
    {
        std::lock_guard<std::mutex> lock(travelMutex);
        travelMarkers[{player->GetGUID(), spell}] = marker->GetGUID();
    }
    uint32 child = ReturnSpell(spell);
    if (!player->HasActiveSpell(child))
        player->learnSpell(child, false);
    player->SetTemporarySpellReplacement(spell, child, false);
    if (player->GetTemporarySpellReplacement(spell) != child)
    {
        ClearTravel(player, spell);
        return false;
    }
    ArmButtonSwap(player);
    FlipTravelButtons(player, spell, child);
    SweepButtonSwapCopies(player);
    if (spell == SPELL_WARPDAGGER)
    {
        player->CastSpell(player, SPELL_WARP_READY, true);
        marker->CastSpell(marker, SPELL_WARP_VISUAL, true);
        Position destination = player->GetFirstCollisionPosition(30.0f, 0.0f);
        marker->GetMotionMaster()->MovePoint(POINT_WARP_DESTINATION, destination,
            FORCED_MOVEMENT_RUN, 0.0f, false);
    }
    return true;
}

struct npc_ascension_runemaster_marker : ScriptedAI
{
    explicit npc_ascension_runemaster_marker(Creature* creature) : ScriptedAI(creature) { }
    ObjectGuid ownerGuid;
    uint32 spell = 0;
    EventMap events;

    ~npc_ascension_runemaster_marker() override { ForgetMarker(ownerGuid, spell, me->GetGUID()); }
    void AttackStart(Unit*) override { }
    void MoveInLineOfSight(Unit*) override { }
    void EnterEvadeMode(EvadeReason) override { }

    void IsSummonedBy(WorldObject* summoner) override
    {
        Player* owner = summoner ? summoner->ToPlayer() : nullptr;
        if (!CanTravel(owner))
        {
            me->DespawnOrUnsummon();
            return;
        }
        ownerGuid = owner->GetGUID();
        spell = me->GetEntry() == NPC_ECHO_RUNE ? SPELL_ECHO_RUNE : SPELL_WARPDAGGER;
        me->SetOwnerGUID(ownerGuid);
        me->SetFaction(owner->GetFaction());
        me->SetLevel(owner->GetLevel());
        me->SetReactState(REACT_PASSIVE);
        me->GetMotionMaster()->Clear();
        me->GetMotionMaster()->MoveIdle();
        events.ScheduleEvent(EVENT_TRAVEL_OWNER_CHECK, Milliseconds(500));
    }

    void UpdateAI(uint32 diff) override
    {
        events.Update(diff);
        if (events.ExecuteEvent() != EVENT_TRAVEL_OWNER_CHECK)
            return;
        Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself();
        if (!CanTravel(owner) || !me->IsAlive() || owner->GetMap() != me->GetMap() || !owner->InSamePhase(me) ||
            !owner->HasAura(TravelAura(spell), ownerGuid) ||
            !owner->HasActiveSpell(spell) || MarkerGuid(ownerGuid, spell) != me->GetGUID())
        {
            ForgetMarker(ownerGuid, spell, me->GetGUID());
            me->DespawnOrUnsummon();
            return;
        }
        events.ScheduleEvent(EVENT_TRAVEL_OWNER_CHECK, Milliseconds(500));
    }
};

class spell_ascension_runemaster_travel : public SpellScript
{
    PrepareSpellScript(spell_ascension_runemaster_travel);

    bool Validate(SpellInfo const*) override
    {
        return ValidateSpellInfo({SPELL_ECHO_RETURN, SPELL_WARP, SPELL_WARP_READY, SPELL_WARP_VISUAL,
            SPELL_WARP_SUMMON, SPELL_WARP_DAMAGE, SPELL_RUNESHROUD, SPELL_SHROUDWALKER});
    }

    SpellCastResult CheckTravel()
    {
        Player* player = GetCaster()->ToPlayer();
        return CanTravel(player) && player->HasActiveSpell(GetSpellInfo()->Id)
            ? SPELL_CAST_OK : SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
    }

    void SummonEcho(SpellEffIndex index)
    {
        if (GetSpellInfo()->Id != SPELL_ECHO_RUNE)
            return;
        PreventHitDefaultEffect(index);
        StartTravel(GetCaster()->ToPlayer(), SPELL_ECHO_RUNE);
    }

    void SummonDagger(SpellEffIndex index)
    {
        if (GetSpellInfo()->Id != SPELL_WARPDAGGER)
            return;
        PreventHitDefaultEffect(index);
        StartTravel(GetCaster()->ToPlayer(), SPELL_WARPDAGGER);
    }

    void ApplyShroudwalker()
    {
        Player* player = GetCaster()->ToPlayer();
        if (GetSpellInfo()->Id == SPELL_WARPDAGGER && player && player->HasAura(SPELL_SHROUDWALKER) &&
            player->HasAura(SPELL_RUNESHROUD, player->GetGUID()))
            player->RemoveSpellCooldown(SPELL_WARPDAGGER, true);
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_ascension_runemaster_travel::CheckTravel);
        OnEffectHit += SpellEffectFn(spell_ascension_runemaster_travel::SummonEcho, EFFECT_0, SPELL_EFFECT_ANY);
        OnEffectHitTarget += SpellEffectFn(spell_ascension_runemaster_travel::SummonDagger, EFFECT_0, SPELL_EFFECT_ANY);
        AfterCast += SpellCastFn(spell_ascension_runemaster_travel::ApplyShroudwalker);
    }
};

class spell_ascension_runemaster_return : public SpellScript
{
    PrepareSpellScript(spell_ascension_runemaster_return);

    SpellCastResult CheckReturn()
    {
        Player* player = GetCaster()->ToPlayer();
        uint32 spell = GetSpellInfo()->Id == SPELL_ECHO_RETURN ? SPELL_ECHO_RUNE : SPELL_WARPDAGGER;
        if (spell == SPELL_ECHO_RUNE && GetSpell()->IsTriggered() && player &&
            player->getClass() == CLASS_SPIRIT_MAGE && player->IsAlive() && player->IsInWorld())
            return SPELL_CAST_OK;
        return CanReturn(player, spell) ? SPELL_CAST_OK : SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
    }

    void Echo(SpellEffIndex index)
    {
        if (GetSpellInfo()->Id != SPELL_ECHO_RETURN)
            return;
        if (!GetSpell()->IsTriggered() && !ReturnToMarker(GetCaster()->ToPlayer(), SPELL_ECHO_RUNE))
            PreventHitDefaultEffect(index);
    }

    void Warp(SpellEffIndex index)
    {
        if (GetSpellInfo()->Id != SPELL_WARP)
            return;
        PreventHitDefaultEffect(index);
        Player* player = GetCaster()->ToPlayer();
        Position destination;
        if (ReturnToMarker(player, SPELL_WARPDAGGER, &destination))
            player->CastSpell(destination.GetPositionX(), destination.GetPositionY(), destination.GetPositionZ(),
                SPELL_WARP_DAMAGE, true);
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_ascension_runemaster_return::CheckReturn);
        OnEffectLaunchTarget += SpellEffectFn(spell_ascension_runemaster_return::Echo, EFFECT_0, SPELL_EFFECT_ANY);
        OnEffectHitTarget += SpellEffectFn(spell_ascension_runemaster_return::Warp, EFFECT_0, SPELL_EFFECT_ANY);
    }
};

class runemaster_travel_auras : public UnitScript
{
public:
    runemaster_travel_auras() : UnitScript("runemaster_travel_auras", true, {UNITHOOK_ON_AURA_REMOVE}) { }

    void OnAuraRemove(Unit* unit, AuraApplication* application, AuraRemoveMode mode) override
    {
        Player* player = unit ? unit->ToPlayer() : nullptr;
        if (!player || player->getClass() != CLASS_SPIRIT_MAGE || !application)
            return;
        Aura* aura = application->GetBase();
        if (aura->GetCasterGUID() != player->GetGUID())
            return;
        uint32 id = aura->GetId();
        if (id == SPELL_ECHO_RUNE)
        {
            if (mode == AURA_REMOVE_BY_EXPIRE && ReturnToMarker(player, SPELL_ECHO_RUNE))
                player->CastSpell(player, SPELL_ECHO_RETURN, true);
            else
                ClearTravel(player, SPELL_ECHO_RUNE);
        }
        else if (id == SPELL_WARP_READY)
            ClearTravel(player, SPELL_WARPDAGGER);
    }
};

class runemaster_travel_lifecycle : public PlayerScript
{
public:
    runemaster_travel_lifecycle() : PlayerScript("runemaster_travel_lifecycle",
        {PLAYERHOOK_ON_LOGIN, PLAYERHOOK_ON_LOGOUT, PLAYERHOOK_ON_MAP_CHANGED,
            PLAYERHOOK_ON_FORGOT_SPELL, PLAYERHOOK_ON_UPDATE}) { }

    void Clear(Player* player)
    {
        if (player->getClass() == CLASS_SPIRIT_MAGE)
            for (uint32 spell : {SPELL_ECHO_RUNE, SPELL_WARPDAGGER})
                ClearTravel(player, spell);
    }

    void OnPlayerLogin(Player* player) override { Clear(player); }
    void OnPlayerLogout(Player* player) override { Clear(player); }
    void OnPlayerMapChanged(Player* player) override { Clear(player); }

    void OnPlayerForgotSpell(Player* player, uint32 spell) override
    {
        if (player->getClass() == CLASS_SPIRIT_MAGE && (spell == SPELL_ECHO_RUNE || spell == SPELL_WARPDAGGER))
        {
            ClearTravel(player, spell);
            player->removeSpell(ReturnSpell(spell), SPEC_MASK_ALL, false);
        }
    }

    void OnPlayerUpdate(Player* player, uint32) override
    {
        if (player->getClass() != CLASS_SPIRIT_MAGE)
            return;
        RefreshButtonSwapState(player);
        for (uint32 spell : {SPELL_ECHO_RUNE, SPELL_WARPDAGGER})
            if ((MarkerGuid(player->GetGUID(), spell) || player->GetTemporarySpellReplacement(spell) != spell) &&
                !HasTravelMarker(player, spell))
                ClearTravel(player, spell);
    }
};
}

void AscensionRunemasterTravelDropButtonCopies(Player* player, uint8 button, uint32 action, uint32 previous)
{
    uint32 partner = TravelButtonPartner(action);
    if (!player || !partner || previous == action)
        return;
    std::lock_guard<std::mutex> lock(buttonSwapMutex);
    auto itr = buttonSwapWindows.find(player->GetGUID());
    if (itr == buttonSwapWindows.end() || uint32(GameTime::GetGameTimeMS().count()) > itr->second.untilMs)
        return;
    ButtonSwapWindow& window = itr->second;
    bool changed = false;
    if (previous == partner)
    {
        window.swapped.push_back(button);
        for (uint8 slot = 0; slot < MAX_ACTION_BUTTONS; ++slot)
        {
            ActionButton const* other = player->GetActionButton(slot);
            if (slot == button || std::find(window.swapped.begin(), window.swapped.end(), slot) != window.swapped.end())
                continue;
            if (other && other->GetType() == ACTION_BUTTON_SPELL && other->GetAction() == action)
            {
                player->removeActionButton(slot);
                changed = true;
            }
        }
    }
    else
    {
        for (uint8 slot = 0; slot < MAX_ACTION_BUTTONS; ++slot)
        {
            ActionButton const* other = player->GetActionButton(slot);
            if (slot != button && other && other->GetType() == ACTION_BUTTON_SPELL && other->GetAction() == action)
            {
                player->removeActionButton(button);
                changed = true;
                break;
            }
        }
    }
    if (changed && player->IsInWorld())
        player->SendActionButtons(1);
}

void ApplyAscensionRunemasterTravelContracts(SpellInfo* info)
{
    if (info->SpellFamilyName != 38)
        return;
    if (info->Id == SPELL_ECHO_RETURN)
        info->Effects[EFFECT_1].Effect = 0;
    if (info->Id == SPELL_WARP)
    {
        info->Effects[EFFECT_0].Effect = SPELL_EFFECT_DUMMY;
        info->Effects[EFFECT_1].Effect = 0;
        info->Effects[EFFECT_2].Effect = 0;
    }
    if (info->Id == SPELL_WARP_DAMAGE)
    {
        info->Effects[EFFECT_0].TargetA = SpellImplicitTargetInfo(TARGET_DEST_DEST);
        info->Effects[EFFECT_0].TargetB = SpellImplicitTargetInfo(TARGET_UNIT_DEST_AREA_ENEMY);
        info->_InitializeExplicitTargetMask();
    }
}

void AddSC_AscensionRunemasterTravel()
{
    RegisterCreatureAI(npc_ascension_runemaster_marker);
    RegisterSpellScript(spell_ascension_runemaster_travel);
    RegisterSpellScript(spell_ascension_runemaster_return);
    new runemaster_travel_auras();
    new runemaster_travel_lifecycle();
}
