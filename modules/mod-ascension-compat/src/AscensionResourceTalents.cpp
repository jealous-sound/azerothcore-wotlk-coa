/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"

namespace
{
enum ResourceTalentSpells : uint32
{
    Felheart = 301287,
    Felfury = 800058,
    RecklessAbandon = 504252,
    Thirst = 706613,
    Insatiable = 706621,
    InsatiablePenalty = 706663,
    DeepSecrets = 582307,
    CharmOfWarding = 705967,
    Superconductor = 705646,
    Static = 803102,
    ArmOfThorim = 801847,
    ChargedConduit = 803790,
    Replenishment = 1257670
};

uint32 Stacks(Unit const* unit, uint32 id)
{
    Aura const* aura = unit ? unit->GetAura(id) : nullptr;
    return aura ? aura->GetStackAmount() : 0;
}

class resource_talent_contracts : public GlobalScript
{
public:
    resource_talent_contracts() : GlobalScript("resource_talent_contracts",
        {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* info) override
    {
        switch (info->Id)
        {
            case Replenishment:
                // The shared CoA helper promises 5% of each recipient's maximum Mana, not five Mana.
                if (info->Effects[EFFECT_0].ApplyAuraName == SPELL_AURA_PERIODIC_ENERGIZE &&
                    info->Effects[EFFECT_0].MiscValue == POWER_MANA)
                    info->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_OBS_MOD_POWER;
                return;
            case Felheart:
            case RecklessAbandon:
            case DeepSecrets:
            case CharmOfWarding:
            case Superconductor:
                // The live talent-tree descriptions specify 1%; normalize the marker before adding its behavior.
                info->Effects[EFFECT_0].BasePoints = 1;
                info->Effects[EFFECT_0].DieSides = 0;
                info->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            default:
                return;
        }
        if (info->Id == Felheart)
        {
            info->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_HASTE_SPELLS;
            info->Effects[EFFECT_1] = info->Effects[EFFECT_0];
            info->Effects[EFFECT_1].EffectIndex = EFFECT_1;
            info->Effects[EFFECT_1].ApplyAuraName = SPELL_AURA_MOD_MELEE_RANGED_HASTE;
        }
        else if (info->Id == RecklessAbandon)
            info->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_MOD_CRIT_PCT;
    }
};

class aura_ascension_resource_talent : public AuraScript
{
    PrepareAuraScript(aura_ascension_resource_talent);

    void Calculate(AuraEffect const* /*effect*/, int32& amount, bool& /*canRecalculate*/)
    {
        Unit* owner = GetUnitOwner();
        bool felsworn = GetId() == Felheart && owner->IsPlayer() && owner->getClass() == CLASS_DEMON_HUNTER;
        bool bloodmage = GetId() == RecklessAbandon && owner->IsPlayer() && owner->getClass() == CLASS_SON_OF_ARUGAL;
        amount *= felsworn ? Stacks(owner, Felfury) : bloodmage ? Stacks(owner, Thirst) : 0;
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(aura_ascension_resource_talent::Calculate,
            EFFECT_ALL, SPELL_AURA_ANY);
    }
};

class aura_ascension_resource_talent_refresh : public AuraScript
{
    PrepareAuraScript(aura_ascension_resource_talent_refresh);

    bool Validate(SpellInfo const* info) override
    {
        return info->Id != Thirst || ValidateSpellInfo({Insatiable, InsatiablePenalty});
    }

    void Refresh(uint32 stacks)
    {
        Unit* owner = GetTarget();
        if (!owner->IsPlayer() || (GetId() == Felfury ? owner->getClass() != CLASS_DEMON_HUNTER :
            owner->getClass() != CLASS_SON_OF_ARUGAL))
            return;
        uint32 talent = GetId() == Felfury ? Felheart : RecklessAbandon;
        if (Aura* aura = owner->GetAura(talent))
            for (uint8 index = EFFECT_0; index < MAX_SPELL_EFFECTS; ++index)
                if (AuraEffect* effect = aura->GetEffect(index))
                    effect->ChangeAmount(int32(stacks) * aura->GetSpellInfo()->Effects[index].CalcValue(owner));
    }

    void Apply(AuraEffect const* /*effect*/, AuraEffectHandleModes /*mode*/)
    {
        Refresh(GetStackAmount());
        Unit* owner = GetTarget();
        // Do not refresh an existing Insatiable: further gains at the cap must
        // not postpone its native periodic damage penalty.
        if (GetId() == Thirst && owner->IsPlayer() && owner->getClass() == CLASS_SON_OF_ARUGAL &&
            GetStackAmount() >= GetSpellInfo()->CalcMaxAuraStacks(owner) &&
            !owner->HasAura(Insatiable, owner->GetGUID()))
            owner->CastSpell(owner, Insatiable, true);
    }

    void Remove(AuraEffect const* /*effect*/, AuraEffectHandleModes /*mode*/)
    {
        Refresh(0);
        Unit* owner = GetTarget();
        if (GetId() == Thirst && owner->IsPlayer() && owner->getClass() == CLASS_SON_OF_ARUGAL)
        {
            // Insatiable lasts until Thirst ends, including consumption, expiry and death.
            owner->RemoveAurasDueToSpell(Insatiable, owner->GetGUID());
            owner->RemoveAurasDueToSpell(InsatiablePenalty, owner->GetGUID());
        }
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(aura_ascension_resource_talent_refresh::Apply,
            EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        AfterEffectRemove += AuraEffectRemoveFn(aura_ascension_resource_talent_refresh::Remove,
            EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

class stormbringer_superconductor : public AllSpellScript
{
public:
    stormbringer_superconductor() : AllSpellScript("stormbringer_superconductor",
        {ALLSPELLHOOK_ON_CALCULATED_TARGET}) { }

    void OnSpellCalculatedTarget(Spell* spell, Unit* /*target*/, TargetInfo& result) override
    {
        Unit* caster = spell->GetCaster();
        if (!caster->IsPlayer() || caster->getClass() != CLASS_STORMBRINGER || spell->IsTriggered() ||
            sSpellMgr->GetFirstSpellInChain(spell->GetSpellInfo()->Id) != ArmOfThorim ||
            caster->HasAura(ChargedConduit))
            return;
        // Launch damage is calculated before the resource service depletes Static at successful cast completion.
        if (AuraEffect const* talent = caster->GetAuraEffect(Superconductor, EFFECT_0))
        {
            float multiplier = 1.0f + Stacks(caster, Static) * talent->GetAmount() / 100.0f;
            result.damage = uint32(result.damage * multiplier);
            result.damageBeforeTakenMods = uint32(result.damageBeforeTakenMods * multiplier);
        }
    }
};
}

void AddSC_AscensionResourceTalents()
{
    new resource_talent_contracts();
    RegisterSpellScript(aura_ascension_resource_talent);
    RegisterSpellScript(aura_ascension_resource_talent_refresh);
    new stormbringer_superconductor();
}
