/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionGuardianStandardData.h"
#include "Creature.h"
#include "EventMap.h"
#include "Map.h"
#include "MotionMaster.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "TemporarySummon.h"
#include <cmath>
#include <map>
#include <mutex>

namespace
{
constexpr uint32 STANDARD_RECOVERY_HEAL = 500248;
constexpr uint32 STANDARD_VALIANCE_DAMAGE = 800335;
constexpr uint32 STANDARD_OWNER_CHECK = 1;
constexpr uint32 STANDARD_OWNER_CHECK_MS = 500;
std::mutex standardMutex;
std::map<ObjectGuid, ObjectGuid> activeStandards;

GuardianStandards::Contract const* FindStandard(uint32 id, bool byCreature = false)
{
    for (auto const& contract : GuardianStandards::Contracts)
        if ((byCreature ? contract.creature : contract.spell) == id)
            return &contract;
    return nullptr;
}

void ForgetStandard(ObjectGuid owner, ObjectGuid standard)
{
    std::lock_guard<std::mutex> lock(standardMutex);
    auto itr = activeStandards.find(owner);
    if (itr != activeStandards.end() && itr->second == standard)
        activeStandards.erase(itr);
}

void ReplaceStandard(Player* player, ObjectGuid replacement = ObjectGuid::Empty)
{
    ObjectGuid old;
    {
        std::lock_guard<std::mutex> lock(standardMutex);
        auto itr = activeStandards.find(player->GetGUID());
        if (itr != activeStandards.end())
        {
            old = itr->second;
            activeStandards.erase(itr);
        }
        if (replacement)
            activeStandards[player->GetGUID()] = replacement;
    }
    if (old && old != replacement)
        if (Creature* creature = player->GetMap()->GetCreature(old))
            if (creature->GetOwnerGUID() == player->GetGUID() && FindStandard(creature->GetEntry(), true))
            {
                creature->RemoveAllAuras();
                creature->DespawnOrUnsummon();
            }
}

Player* StandardOwner(Unit* caster)
{
    if (!caster || !caster->IsCreature() || !FindStandard(caster->GetEntry(), true))
        return nullptr;
    Player* owner = caster->GetCharmerOrOwnerPlayerOrPlayerItself();
    return owner && owner->getClass() == CLASS_GUARDIAN && owner->IsAlive() && owner->IsInWorld() &&
        owner->GetMap() == caster->GetMap() ? owner : nullptr;
}
}

struct npc_ascension_guardian_standard : ScriptedAI
{
    explicit npc_ascension_guardian_standard(Creature* creature) : ScriptedAI(creature)
    {
    }

    ~npc_ascension_guardian_standard() override
    {
        ForgetStandard(ownerGuid, me->GetGUID());
    }

    ObjectGuid ownerGuid;
    EventMap events;

    void AttackStart(Unit* /*target*/) override { }
    void MoveInLineOfSight(Unit* /*target*/) override { }
    void EnterEvadeMode(EvadeReason /*why*/) override { }

    void IsSummonedBy(WorldObject* summoner) override
    {
        Player* owner = summoner ? summoner->ToPlayer() : nullptr;
        auto const* contract = FindStandard(me->GetEntry(), true);
        if (!owner || owner->getClass() != CLASS_GUARDIAN || !contract)
        {
            me->DespawnOrUnsummon();
            return;
        }
        ownerGuid = owner->GetGUID();
        me->SetOwnerGUID(ownerGuid);
        me->SetFaction(owner->GetFaction());
        me->SetLevel(owner->GetLevel());
        me->SetReactState(REACT_PASSIVE);
        me->GetMotionMaster()->Clear();
        me->GetMotionMaster()->MoveIdle();
        // The native area-aura owner is the stationary standard, not the player.
        // Keep this caster GUID so range/cleanup and multiple owners stay native.
        me->CastSpell(me, contract->field, true);
        events.ScheduleEvent(STANDARD_OWNER_CHECK, Milliseconds(STANDARD_OWNER_CHECK_MS));
    }

    void JustDied(Unit* /*killer*/) override
    {
        me->RemoveAllAuras();
        ForgetStandard(ownerGuid, me->GetGUID());
        me->DespawnOrUnsummon();
    }

    void UpdateAI(uint32 diff) override
    {
        events.Update(diff);
        if (events.ExecuteEvent() == STANDARD_OWNER_CHECK)
        {
            if (!StandardOwner(me))
            {
                me->RemoveAllAuras();
                ForgetStandard(ownerGuid, me->GetGUID());
                me->DespawnOrUnsummon();
                return;
            }
            events.ScheduleEvent(STANDARD_OWNER_CHECK, Milliseconds(STANDARD_OWNER_CHECK_MS));
        }
    }
};

class spell_ascension_guardian_standard : public SpellScript
{
    PrepareSpellScript(spell_ascension_guardian_standard);

    bool Load() override
    {
        Player* player = GetCaster()->ToPlayer();
        return player && player->getClass() == CLASS_GUARDIAN && FindStandard(GetSpellInfo()->Id);
    }

    void Summon(SpellEffIndex index)
    {
        PreventHitDefaultEffect(index);
        auto const* contract = FindStandard(GetSpellInfo()->Id);
        Player* owner = GetCaster()->ToPlayer();
        WorldLocation const* destination = GetHitDest();
        if (!destination || !contract)
            return;
        int32 duration = GetSpellInfo()->GetDuration();
        owner->ApplySpellMod(GetSpellInfo()->Id, SPELLMOD_DURATION, duration);
        if (duration <= 0)
            return;
        // Do not use SummonProperties 61: the stock Guardian path forces Follow
        // after IsSummonedBy, making a banner follow its owner like a pet.
        if (TempSummon* standard = owner->SummonCreature(contract->creature, *destination,
                TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, uint32(duration)))
        {
            standard->SetUInt32Value(UNIT_CREATED_BY_SPELL, GetSpellInfo()->Id);
            ReplaceStandard(owner, standard->GetGUID());
        }
    }

    void SkipAutomaticReclaim(SpellEffIndex index)
    {
        // Replacement is atomic at summon success, not the parent's earlier
        // launch trigger. Field Commander is checked separately below.
        PreventHitDefaultEffect(index);
    }

    void RequireFieldCommander(SpellEffIndex index)
    {
        if (!GetCaster()->HasAura(705320))
            PreventHitDefaultEffect(index);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_ascension_guardian_standard::Summon, EFFECT_0, SPELL_EFFECT_SUMMON);
        OnEffectLaunch += SpellEffectFn(spell_ascension_guardian_standard::SkipAutomaticReclaim,
            EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
        OnEffectLaunchTarget += SpellEffectFn(spell_ascension_guardian_standard::SkipAutomaticReclaim,
            EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
        OnEffectLaunch += SpellEffectFn(spell_ascension_guardian_standard::RequireFieldCommander,
            EFFECT_2, SPELL_EFFECT_TRIGGER_SPELL);
        OnEffectLaunchTarget += SpellEffectFn(spell_ascension_guardian_standard::RequireFieldCommander,
            EFFECT_2, SPELL_EFFECT_TRIGGER_SPELL);
    }
};

class spell_ascension_guardian_reclaim : public SpellScript
{
    PrepareSpellScript(spell_ascension_guardian_reclaim);
    bool Load() override
    {
        Player* owner = GetCaster()->ToPlayer();
        return owner && owner->getClass() == CLASS_GUARDIAN;
    }
    void Reclaim(SpellEffIndex index)
    {
        PreventHitDefaultEffect(index);
        ReplaceStandard(GetCaster()->ToPlayer());
    }
    void Register() override
    {
        OnEffectLaunch += SpellEffectFn(spell_ascension_guardian_reclaim::Reclaim, EFFECT_0, SPELL_EFFECT_TRIGGER_SPELL);
        OnEffectLaunchTarget += SpellEffectFn(spell_ascension_guardian_reclaim::Reclaim,
            EFFECT_0, SPELL_EFFECT_TRIGGER_SPELL);
    }
};

class aura_ascension_guardian_recovery : public AuraScript
{
    PrepareAuraScript(aura_ascension_guardian_recovery);
    void Tick(AuraEffect const* effect)
    {
        PreventDefaultAction();
        Player* owner = StandardOwner(GetCaster());
        if (!owner)
            return;
        SpellInfo const* heal = sSpellMgr->GetSpellInfo(STANDARD_RECOVERY_HEAL);
        if (!heal || !GetTarget()->IsAlive())
            return;
        // Active ability text takes precedence over the stale percent-max-HP
        // hidden helper description. Native healing modifiers still run once.
        int32 amount = heal->Effects[EFFECT_0].CalcValue(owner) + int32(owner->GetStat(STAT_STRENGTH) * 0.25f);
        GetCaster()->CastCustomSpell(GetTarget(), STANDARD_RECOVERY_HEAL, &amount, nullptr, nullptr,
            true, nullptr, effect, owner->GetGUID());
    }
    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(aura_ascension_guardian_recovery::Tick,
            EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

class aura_ascension_guardian_valiance : public AuraScript
{
    PrepareAuraScript(aura_ascension_guardian_valiance);
    void Tick(AuraEffect const* effect)
    {
        PreventDefaultAction();
        Player* owner = StandardOwner(GetCaster());
        if (!owner)
            return;
        int32 amount = effect->GetSpellInfo()->Effects[EFFECT_2].CalcValue(owner) +
            int32(owner->GetTotalAttackPowerValue(BASE_ATTACK) * 0.04f);
        GetCaster()->CastCustomSpell(GetCaster(), STANDARD_VALIANCE_DAMAGE, &amount, nullptr, nullptr,
            true, nullptr, effect, owner->GetGUID());
    }
    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(aura_ascension_guardian_valiance::Tick,
            EFFECT_2, SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE);
    }
};

class aura_ascension_guardian_tower : public AuraScript
{
    PrepareAuraScript(aura_ascension_guardian_tower);
    void UpdateArmor(AuraEffect const* /*effect*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* player = GetTarget()->ToPlayer())
            player->UpdateArmor();
    }
    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(aura_ascension_guardian_tower::UpdateArmor,
            EFFECT_1, SPELL_AURA_DUMMY, AuraEffectHandleModes(
                AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK | AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK));
        AfterEffectRemove += AuraEffectRemoveFn(aura_ascension_guardian_tower::UpdateArmor,
            EFFECT_1, SPELL_AURA_DUMMY, AuraEffectHandleModes(
                AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK | AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK));
    }
};

void AddAscensionGuardianStandardScripts()
{
    RegisterCreatureAI(npc_ascension_guardian_standard);
    RegisterSpellScript(spell_ascension_guardian_standard);
    RegisterSpellScript(spell_ascension_guardian_reclaim);
    RegisterSpellScript(aura_ascension_guardian_recovery);
    RegisterSpellScript(aura_ascension_guardian_valiance);
    RegisterSpellScript(aura_ascension_guardian_tower);
}
