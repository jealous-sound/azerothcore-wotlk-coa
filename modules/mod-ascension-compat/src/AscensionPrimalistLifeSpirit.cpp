/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellInfo.h"
#include "SpellScript.h"
#include "TemporarySummon.h"

#include <algorithm>

namespace
{
enum LifeSpiritEntries : uint32
{
    SpiritOfLife = 840002,
    PrimordialSummon = 572853,
    LesserSpiritCharge = 572830
};

class spell_ascension_primordial_spirit : public SpellScript
{
    PrepareSpellScript(spell_ascension_primordial_spirit);

    bool Validate(SpellInfo const* info) override
    {
        return info->Id == PrimordialSummon && ValidateSpellInfo({LesserSpiritCharge});
    }

    bool Load() override
    {
        return GetCaster()->IsPlayer() && GetCaster()->getClass() == CLASS_WILDWALKER;
    }

    void Summon(SpellEffIndex index)
    {
        PreventHitDefaultEffect(index);
        Player* owner = GetCaster()->ToPlayer();
        WorldLocation const* destination = GetHitDest();
        int32 duration = GetSpellInfo()->GetDuration();
        if (!destination || duration <= 0)
            return;

        // A plain timed summon avoids the Guardian path's forced Follow after initialization,
        // which would replace the immediate charge movement. No persistent pet slot is used.
        if (TempSummon* spirit = owner->SummonCreature(SpiritOfLife, *destination,
            TEMPSUMMON_TIMED_DESPAWN, uint32(duration)))
        {
            spirit->SetOwnerGUID(owner->GetGUID());
            spirit->SetFaction(owner->GetFaction());
            spirit->SetLevel(owner->GetLevel());
            spirit->SetReactState(REACT_PASSIVE);
            spirit->SetUInt32Value(UNIT_CREATED_BY_SPELL, PrimordialSummon);
            // Native nearby-ally selection, charge, heal and destination-area damage stay together.
            spirit->CastSpell(nullptr, LesserSpiritCharge, TRIGGERED_FULL_MASK, nullptr, nullptr, owner->GetGUID());
        }
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_ascension_primordial_spirit::Summon, EFFECT_0, SPELL_EFFECT_SUMMON);
    }
};

class primalist_life_spirit_scaling : public UnitScript
{
public:
    primalist_life_spirit_scaling() : UnitScript("primalist_life_spirit_scaling",
        true, {UNITHOOK_MODIFY_SPELL_EFFECT_BASE_VALUE}) { }

    void ModifySpellEffectBaseValue(Unit const* caster, SpellInfo const* info, uint8 index,
        float& value) override
    {
        if (!caster || info->Id != LesserSpiritCharge || (index != EFFECT_1 && index != EFFECT_2))
            return;
        Unit const* owner = caster->IsPlayer() ? caster : caster->GetOwner();
        if (!owner || !owner->IsPlayer() || owner->getClass() != CLASS_WILDWALKER)
            return;

        // Primordial Earth's active description specifies Nature spell power and owner AP for both
        // effects. The hidden helper's older description disagrees; SQL prevents a second coefficient.
        // This native stat query has a non-const signature but does not mutate the unit.
        value += 0.2f * (std::max(0, const_cast<Unit*>(owner)->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_NATURE)) +
            std::max(0.0f, owner->GetTotalAttackPowerValue(BASE_ATTACK)));
    }
};

class spell_ascension_lesser_spirit_charge : public SpellScript
{
    PrepareSpellScript(spell_ascension_lesser_spirit_charge);

    void SelectDestination(WorldObject*& target)
    {
        if (!target)
            return;

        // The copied helper selects a nearby party member but never supplies a destination.
        // Both its native charge and its area damage must use the selected ally's position.
        WorldLocation destination(target->GetMapId(), target->GetPosition());
        SetExplTargetDest(destination);
    }

    void Register() override
    {
        OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_ascension_lesser_spirit_charge::SelectDestination,
            EFFECT_0, TARGET_UNIT_NEARBY_PARTY);
    }
};
}

void AddSC_AscensionPrimalistLifeSpirit()
{
    new primalist_life_spirit_scaling();
    RegisterSpellScript(spell_ascension_primordial_spirit);
    RegisterSpellScript(spell_ascension_lesser_spirit_charge);
}
