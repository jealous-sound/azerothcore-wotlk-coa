/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include <algorithm>
#include <limits>
#include <vector>

namespace
{
enum ChronomancerSecondarySpells : uint32
{
    SPELL_MELT_REALITY = 806335,
    SPELL_MIND_MELT = 572851,
    SPELL_MELT_COPY_VALUE = 504727,
    SPELL_MELT_COPY = 807570,
    SPELL_DESYNCHRONIZATION = 561310,
    SPELL_RESYNCHRONIZATION = 561388,
    SPELL_AHEAD_OF_THE_GAME = 560948,
    SPELL_AHEAD_COUNTER = 592009,
    SPELL_AHEAD_POWER = 592010,
    SPELL_RIPPLE = 806296,
    SPELL_RIPPLE_CHARGE = 806297,
    SPELL_RIPPLE_HEAL = 806298,
    SPELL_ECHO_FRAGMENT = 804455,
    SPELL_ARC_COLLISION = 524853,
    SPELL_ECHO_DURATION = 807711
};

Player* SecondaryChronomancer(Unit* unit)
{
    Player* player = unit ? unit->ToPlayer() : nullptr;
    return player && player->getClass() == CLASS_CHRONOMANCER && player->IsAlive() && player->IsInWorld()
        ? player : nullptr;
}

class chronomancer_melt_periodic : public UnitScript
{
public:
    chronomancer_melt_periodic() : UnitScript("chronomancer_melt_periodic", true,
        {UNITHOOK_ON_PERIODIC_DAMAGE_RESULT}) { }

    void OnPeriodicDamageResult(Unit* target, Unit*, uint32 damage, SpellInfo const*) override
    {
        if (!target || !target->IsInWorld() || !damage)
            return;
        // Allied periodic damage also contributes. Snapshot owners before a copy can remove another mark.
        std::vector<ObjectGuid> owners;
        for (auto const& pair : target->GetAppliedAuras())
        {
            Aura* aura = pair.second->GetBase();
            if (sSpellMgr->GetFirstSpellInChain(aura->GetId()) == SPELL_MELT_REALITY &&
                std::find(owners.begin(), owners.end(), aura->GetCasterGUID()) == owners.end())
                owners.push_back(aura->GetCasterGUID());
        }
        SpellInfo const* value = sSpellMgr->GetSpellInfo(SPELL_MELT_COPY_VALUE);
        if (!value)
            return;
        for (ObjectGuid const& guid : owners)
            if (Player* player = SecondaryChronomancer(ObjectAccessor::GetUnit(*target, guid));
                player && player->GetMap() == target->GetMap() && player->InSamePhase(target) &&
                player->IsValidAttackTarget(target))
            {
                uint64 amount = uint64(damage) * std::max(0, value->Effects[EFFECT_0].CalcValue(player)) / 100;
                if (amount)
                    player->CastCustomSpell(SPELL_MELT_COPY, SPELLVALUE_BASE_POINT0,
                        int32(std::min<uint64>(amount, std::numeric_limits<int32>::max())), target, true);
            }
    }
};

class chronomancer_mind_melt_taken : public UnitScript
{
public:
    chronomancer_mind_melt_taken() : UnitScript("chronomancer_mind_melt_taken", true,
        {UNITHOOK_MODIFY_PERIODIC_DAMAGE_AURAS_TICK}) { }

    // End of Time 704490 raises Mind Melt's first effect from 0 to 6 through SPELLMOD_EFFECT1, and
    // AuraEffect::CalculateAmount already multiplies that amount by the stack count. That effect is
    // aura 214, which this core maps to AuraEffect::HandleNULL, so nothing reads it. Deliver the
    // talent's "increases the target's periodic damage taken from you by $s1% per stack" here, the
    // way Templar's Light's Ward delivers its own periodic damage taken clause.
    void ModifyPeriodicDamageAurasTick(Unit* target, Unit* attacker, uint32& damage,
        SpellInfo const* info) override
    {
        if (!target || !attacker || !damage || !info ||
            info->HasAttribute(SPELL_ATTR4_IGNORE_DAMAGE_TAKEN_MODIFIERS))
            return;
        // This hook also reports periodic healing, which a damage taken clause must not increase.
        if (!info->HasAura(SPELL_AURA_PERIODIC_DAMAGE) && !info->HasAura(SPELL_AURA_PERIODIC_DAMAGE_PERCENT) &&
            !info->HasAura(SPELL_AURA_PERIODIC_LEECH))
            return;
        // "from you": only the Chronomancer who applied this Mind Melt benefits from it.
        AuraEffect const* melt = target->GetAuraEffect(SPELL_MIND_MELT, EFFECT_0, attacker->GetGUID());
        if (!melt || melt->GetAmount() <= 0)
            return;
        // EffectMiscValue 127 selects every school; honour the authored mask instead of assuming it.
        if (int32 const schoolMask = melt->GetMiscValue();
            schoolMask && !(uint32(schoolMask) & uint32(info->GetSchoolMask())))
            return;
        // Integer arithmetic keeps whole percentages exact; clamp so a large tick cannot wrap.
        uint64 const bonus = uint64(damage) * uint64(melt->GetAmount()) / 100;
        damage = uint32(std::min<uint64>(uint64(damage) + bonus,
            std::numeric_limits<uint32>::max()));
    }
};

class spell_ascension_melt_copy : public SpellScript
{
    PrepareSpellScript(spell_ascension_melt_copy);

    void Select(std::list<WorldObject*>& targets)
    {
        Unit* original = GetExplTargetUnit();
        targets.remove_if([original](WorldObject* object)
        {
            Unit* target = object ? object->ToUnit() : nullptr;
            return !target || target == original || !target->IsAlive();
        });
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ascension_melt_copy::Select,
            EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

class aura_ascension_desynchronization : public AuraScript
{
    PrepareAuraScript(aura_ascension_desynchronization);

    bool Validate(SpellInfo const*) override { return ValidateSpellInfo({SPELL_RESYNCHRONIZATION}); }

    void End(AuraEffect const*, AuraEffectHandleModes)
    {
        Player* player = SecondaryChronomancer(GetCaster());
        Unit* target = GetTarget();
        if (player && target->IsAlive() && target->IsInWorld() && player->GetMap() == target->GetMap() &&
            player->InSamePhase(target) && player->IsValidAttackTarget(target) &&
            GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_DEATH)
            player->CastSpell(target, SPELL_RESYNCHRONIZATION, true);
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(aura_ascension_desynchronization::End,
            EFFECT_0, SPELL_AURA_MOD_DISARM, AURA_EFFECT_HANDLE_REAL);
    }
};

class aura_ascension_ahead_of_the_game : public AuraScript
{
    PrepareAuraScript(aura_ascension_ahead_of_the_game);

    bool Validate(SpellInfo const*) override { return ValidateSpellInfo({SPELL_AHEAD_COUNTER, SPELL_AHEAD_POWER}); }

    bool Check(ProcEventInfo& event)
    {
        return SecondaryChronomancer(GetTarget()) && event.GetActor() == GetTarget() &&
            ((event.GetDamageInfo() && event.GetDamageInfo()->GetDamage()) ||
                (event.GetHealInfo() && event.GetHealInfo()->GetEffectiveHeal()));
    }

    void Count(AuraEffect const*, ProcEventInfo&)
    {
        PreventDefaultAction();
        Unit* player = GetTarget();
        player->CastSpell(player, SPELL_AHEAD_COUNTER, true);
        if (Aura const* counter = player->GetAura(SPELL_AHEAD_COUNTER, player->GetGUID());
            counter && counter->GetStackAmount() >= 10)
        {
            player->RemoveAurasDueToSpell(SPELL_AHEAD_COUNTER, player->GetGUID());
            player->CastSpell(player, SPELL_AHEAD_POWER, true);
        }
    }

    void Clear(AuraEffect const*, AuraEffectHandleModes)
    {
        GetTarget()->RemoveAurasDueToSpell(SPELL_AHEAD_COUNTER, GetTarget()->GetGUID());
        GetTarget()->RemoveAurasDueToSpell(SPELL_AHEAD_POWER, GetTarget()->GetGUID());
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_ahead_of_the_game::Check);
        OnEffectProc += AuraEffectProcFn(aura_ascension_ahead_of_the_game::Count,
            EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        AfterEffectRemove += AuraEffectRemoveFn(aura_ascension_ahead_of_the_game::Clear,
            EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

class aura_ascension_ripple_release : public AuraScript
{
    PrepareAuraScript(aura_ascension_ripple_release);

    bool Validate(SpellInfo const*) override { return ValidateSpellInfo({SPELL_RIPPLE_CHARGE, SPELL_RIPPLE_HEAL}); }

    void Start(AuraEffect const*, AuraEffectHandleModes)
    {
        GetTarget()->RemoveAurasDueToSpell(SPELL_RIPPLE_CHARGE, GetTarget()->GetGUID());
    }

    void End(AuraEffect const*, AuraEffectHandleModes)
    {
        Unit* player = GetTarget();
        AuraEffect const* timer = GetEffect(EFFECT_1);
        if (SecondaryChronomancer(player) && player == GetCaster() && timer && timer->GetTickNumber() >= 6 &&
            GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_DEATH)
            // Six native 250 ms ticks meet the minimum. Their authored spell modifier scales this heal.
            player->CastSpell(player, SPELL_RIPPLE_HEAL, true);
        player->RemoveAurasDueToSpell(SPELL_RIPPLE_CHARGE, player->GetGUID());
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(aura_ascension_ripple_release::Start,
            EFFECT_0, SPELL_AURA_MECHANIC_IMMUNITY_MASK, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(aura_ascension_ripple_release::End,
            EFFECT_0, SPELL_AURA_MECHANIC_IMMUNITY_MASK, AURA_EFFECT_HANDLE_REAL);
    }
};

class aura_ascension_echo_duration : public AuraScript
{
    PrepareAuraScript(aura_ascension_echo_duration);

    bool Validate(SpellInfo const*) override { return ValidateSpellInfo({SPELL_ECHO_DURATION}); }

    bool Load() override
    {
        return GetCaster() == GetUnitOwner() && GetCaster() && GetCaster()->IsPlayer() &&
            GetCaster()->getClass() == CLASS_CHRONOMANCER;
    }

    void Sync(AuraEffect const*, AuraEffectHandleModes)
    {
        Unit* player = GetTarget();
        Aura* companion = player->GetAura(SPELL_ECHO_DURATION, player->GetGUID());
        if (!companion)
            companion = player->AddAura(SPELL_ECHO_DURATION, player);
        if (companion && companion->GetStackAmount() != GetStackAmount())
            companion->SetStackAmount(GetStackAmount());
    }

    void Clear(AuraEffect const*, AuraEffectHandleModes)
    {
        GetTarget()->RemoveAurasDueToSpell(SPELL_ECHO_DURATION, GetTarget()->GetGUID());
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(aura_ascension_echo_duration::Sync,
            EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        AfterEffectRemove += AuraEffectRemoveFn(aura_ascension_echo_duration::Clear,
            EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class chronomancer_secondary_casts : public AllSpellScript
{
public:
    chronomancer_secondary_casts() : AllSpellScript("chronomancer_secondary_casts", {ALLSPELLHOOK_ON_CAST}) { }

    void OnSpellCast(Spell* spell, Unit* caster, SpellInfo const* info, bool) override
    {
        if (SecondaryChronomancer(caster) && !spell->IsTriggered() && info->Id == SPELL_ARC_COLLISION)
            // Arc Collision is immediate: every target has received its duration before this callback.
            caster->RemoveAurasDueToSpell(SPELL_ECHO_FRAGMENT, caster->GetGUID());
    }
};

class chronomancer_secondary_metadata : public GlobalScript
{
public:
    chronomancer_secondary_metadata() : GlobalScript("chronomancer_secondary_metadata",
        {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* info) override
    {
        if (info->Id == SPELL_MELT_COPY && info->SpellFamilyName == 28)
        {
            info->AttributesEx2 |= SPELL_ATTR2_CANT_CRIT;
            info->AttributesEx3 |= SPELL_ATTR3_IGNORE_CASTER_MODIFIERS;
            info->AttributesEx4 |= SPELL_ATTR4_IGNORE_DAMAGE_TAKEN_MODIFIERS;
            info->AscensionInheritsResolvedAmount = true;
            info->Effects[EFFECT_0].BonusMultiplier = 0.0f;
        }
        if (info->Id == SPELL_ECHO_DURATION || info->Id == SPELL_AHEAD_COUNTER || info->Id == SPELL_RIPPLE_CHARGE)
        {
            info->AttributesCu &= ~SPELL_ATTR0_CU_FORCE_AURA_SAVING;
            info->AttributesCu |= SPELL_ATTR0_CU_AURA_CANNOT_BE_SAVED;
        }
    }
};
}

void AddSC_AscensionChronomancerSecondary()
{
    new chronomancer_melt_periodic();
    new chronomancer_mind_melt_taken();
    new chronomancer_secondary_casts();
    new chronomancer_secondary_metadata();
    RegisterSpellScript(spell_ascension_melt_copy);
    RegisterSpellScript(aura_ascension_desynchronization);
    RegisterSpellScript(aura_ascension_ahead_of_the_game);
    RegisterSpellScript(aura_ascension_ripple_release);
    RegisterSpellScript(aura_ascension_echo_duration);
}
