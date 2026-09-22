/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionCustomResourceData.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"

namespace
{
enum StormbringerTalentSpells : uint32
{
    SPELL_CLOUDBURST = 801838,
    SPELL_CLOUDBURST_KNOCKBACK = 802385,
    SPELL_SHOCK = 804020,
    SPELL_SHOCK_DOT = 560336,
    SPELL_PERPETUAL_SHOCK = 570054,
    SPELL_CALL_LIGHTNING = 500040,
    SPELL_THUNDER_WARD = 800098,
    SPELL_STATIC = 803102,
    SPELL_GENERATE_STATIC_20 = 804086,
    SPELL_BAROMETRIC_SLOW = 803566,
    SPELL_ELECTRICAL_CHARGE = 800299,
    SPELL_CHARGED_CONDUIT = 803790,
    SPELL_ELECTROCUTIONER_PASSIVE = 500068,
    SPELL_ELECTROCUTIONER_TALENT = 92096,
    SPELL_ELECTROCUTIONER = 804592,
    SPELL_DARK_SKIES_BUFF = 680855,
    SPELL_CRITICAL_CIRCUIT = 807314,
    SPELL_REFUND_STATIC_10 = 804084
};

bool SpendsStatic(uint32 spellId)
{
    for (AscensionCompatData::ResourceCostRule const& rule : AscensionCompatData::ResourceCostRules)
        if (rule.ClassId == CLASS_STORMBRINGER && rule.ResourceSpellId == SPELL_STATIC &&
            spellId >= rule.FirstSpellId && spellId <= rule.LastSpellId)
            return rule.Consumption != AscensionCompatData::ResourceConsumption::None;
    return false;
}

uint32 ElectrocutionerChance(Player const* player)
{
    SpellInfo const* talent = sSpellMgr->GetSpellInfo(SPELL_ELECTROCUTIONER_TALENT);
    Aura const* staticAura = player->GetAura(SPELL_STATIC);
    return (talent ? talent->ProcChance : 0) + (staticAura ? staticAura->GetStackAmount() / 5 : 0);
}

class stormbringer_talent_casts : public AllSpellScript
{
public:
    stormbringer_talent_casts() : AllSpellScript("stormbringer_talent_casts",
        {ALLSPELLHOOK_ON_CAST, ALLSPELLHOOK_ON_HIT_RESULT}) { }

    void OnSpellCast(Spell* spell, Unit* caster, SpellInfo const* info, bool) override
    {
        Player* player = caster ? caster->ToPlayer() : nullptr;
        if (player && player->getClass() == CLASS_STORMBRINGER && info->SpellFamilyName == 22 &&
            info->Id == SPELL_CLOUDBURST && !spell->IsTriggered())
            player->CastSpell(player, SPELL_CLOUDBURST_KNOCKBACK, true);
    }

    void OnSpellHitResult(Spell* spell, Unit* target, uint8 miss, uint32 damage, uint32, bool critical) override
    {
        Player* player = spell->GetCaster()->ToPlayer();
        SpellInfo const* info = spell->GetSpellInfo();
        if (!player || player->getClass() != CLASS_STORMBRINGER || info->SpellFamilyName != 22 ||
            !target || target == player || player->IsFriendlyTo(target) || miss != SPELL_MISS_NONE)
            return;

        if (damage && !spell->IsTriggered() &&
            (player->HasSpell(SPELL_ELECTROCUTIONER_PASSIVE) || player->HasSpell(SPELL_ELECTROCUTIONER_TALENT)) &&
            roll_chance_i(ElectrocutionerChance(player)))
            player->CastSpell(player, SPELL_ELECTROCUTIONER, true);

        if (critical && damage && !spell->IsTriggered() && player->HasAura(SPELL_CRITICAL_CIRCUIT) &&
            SpendsStatic(info->Id) && !spell->GetScriptValue(SPELL_CRITICAL_CIRCUIT))
        {
            spell->SetScriptValue(SPELL_CRITICAL_CIRCUIT, 1);
            player->CastSpell(player, SPELL_REFUND_STATIC_10, true);
        }

        bool repeat =info->Id == SPELL_PERPETUAL_SHOCK;
        if (!repeat && (spell->IsTriggered() || sSpellMgr->GetFirstSpellInChain(info->Id) != SPELL_SHOCK))
            return;

        if (damage && !spell->GetScriptValue(SPELL_SHOCK_DOT))
        {
            spell->SetScriptValue(SPELL_SHOCK_DOT, 1);
            player->CastCustomSpell(SPELL_SHOCK_DOT, SPELLVALUE_BASE_POINT0, int32(damage / 10), target, true);
        }
        if (player->HasSpell(SPELL_CALL_LIGHTNING) && !player->HasAura(SPELL_THUNDER_WARD) &&
            !spell->GetScriptValue(SPELL_STATIC))
        {
            spell->SetScriptValue(SPELL_STATIC, 1);
            player->CastSpell(player, SPELL_GENERATE_STATIC_20, true);
        }
    }
};

class stormbringer_resource_contracts : public GlobalScript
{
public:
    stormbringer_resource_contracts() : GlobalScript("stormbringer_resource_contracts",
        {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* info) override
    {
        if (!info || info->SpellFamilyName != 22)
            return;
        if (info->Id == SPELL_SHOCK_DOT)
        {
            info->AttributesEx2 |= SPELL_ATTR2_CANT_CRIT;
            info->AttributesEx3 |= SPELL_ATTR3_IGNORE_CASTER_MODIFIERS;
            info->AttributesEx4 |= SPELL_ATTR4_IGNORE_DAMAGE_TAKEN_MODIFIERS;
            info->AscensionInheritsResolvedAmount = true;
            info->Effects[EFFECT_0].BonusMultiplier = 0.0f;
        }
        if (info->Id == SPELL_PERPETUAL_SHOCK)
            info->Effects[EFFECT_1].Effect = 0;
        if (info->Id == SPELL_CHARGED_CONDUIT)
            info->Effects[EFFECT_2].Effect = 0;
    }
};

class aura_ascension_barometric_pressure : public AuraScript
{
    PrepareAuraScript(aura_ascension_barometric_pressure);

    bool Validate(SpellInfo const*) override { return ValidateSpellInfo({SPELL_BAROMETRIC_SLOW}); }

    void Apply(AuraEffect const*, AuraEffectHandleModes)
    {
        if (GetCaster() && GetCaster() == GetTarget())
            GetCaster()->AddAura(SPELL_BAROMETRIC_SLOW, GetTarget());
    }

    void OnRemove(AuraEffect const*, AuraEffectHandleModes)
    {
        if (GetCasterGUID() == GetTarget()->GetGUID())
            GetTarget()->RemoveAurasDueToSpell(SPELL_BAROMETRIC_SLOW, GetCasterGUID());
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(aura_ascension_barometric_pressure::Apply,
            EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(aura_ascension_barometric_pressure::OnRemove,
            EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class aura_ascension_electrical_charge : public AuraScript
{
    PrepareAuraScript(aura_ascension_electrical_charge);

    bool Validate(SpellInfo const*) override
    {
        return ValidateSpellInfo({SPELL_ELECTRICAL_CHARGE, SPELL_CHARGED_CONDUIT});
    }

    void Tick(AuraEffect const*)
    {
        PreventDefaultAction();
        Unit* owner = GetTarget();
        if (owner->isMoving() || owner->HasAura(SPELL_CHARGED_CONDUIT))
            return;
        owner->CastSpell(owner, SPELL_ELECTRICAL_CHARGE, true);
        if (Aura* charges = owner->GetAura(SPELL_ELECTRICAL_CHARGE))
            if (charges->GetStackAmount() >= sSpellMgr->GetSpellInfo(SPELL_ELECTRICAL_CHARGE)->StackAmount)
                owner->CastSpell(owner, SPELL_CHARGED_CONDUIT, true);
    }

    void OnRemove(AuraEffect const*, AuraEffectHandleModes)
    {
        GetTarget()->RemoveAurasDueToSpell(SPELL_CHARGED_CONDUIT);
        GetTarget()->RemoveAurasDueToSpell(SPELL_ELECTRICAL_CHARGE);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(aura_ascension_electrical_charge::Tick,
            EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        AfterEffectRemove += AuraEffectRemoveFn(aura_ascension_electrical_charge::OnRemove,
            EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

class aura_ascension_dark_skies : public AuraScript
{
    PrepareAuraScript(aura_ascension_dark_skies);

    bool Validate(SpellInfo const*) override { return ValidateSpellInfo({SPELL_DARK_SKIES_BUFF}); }

    bool Check(ProcEventInfo& event)
    {
        Unit* player = GetTarget();
        Unit* victim = event.GetActionTarget();
        return player->IsPlayer() && player->getClass() == CLASS_STORMBRINGER && player->IsAlive() &&
            GetCaster() == player && event.GetActor() == player && victim && victim != player &&
            !player->IsFriendlyTo(victim) && event.GetDamageInfo() && event.GetDamageInfo()->GetDamage();
    }

    void Proc(AuraEffect const*, ProcEventInfo& event)
    {
        PreventDefaultAction();
        Unit* player = GetTarget();
        if (event.GetHitMask() & PROC_HIT_CRITICAL)
            player->RemoveAurasDueToSpell(SPELL_DARK_SKIES_BUFF);
        else
            player->CastSpell(player, SPELL_DARK_SKIES_BUFF, true);
    }

    void Remove(AuraEffect const*, AuraEffectHandleModes)
    {
        GetTarget()->RemoveAurasDueToSpell(SPELL_DARK_SKIES_BUFF);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_dark_skies::Check);
        OnEffectProc += AuraEffectProcFn(aura_ascension_dark_skies::Proc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        AfterEffectRemove += AuraEffectRemoveFn(aura_ascension_dark_skies::Remove,
            EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

class aura_ascension_charged_conduit : public AuraScript
{
    PrepareAuraScript(aura_ascension_charged_conduit);

    void OnRemove(AuraEffect const*, AuraEffectHandleModes)
    {
        GetTarget()->RemoveAurasDueToSpell(SPELL_ELECTRICAL_CHARGE);
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(aura_ascension_charged_conduit::OnRemove,
            EFFECT_1, SPELL_AURA_HASTE_SPELLS, AURA_EFFECT_HANDLE_REAL);
    }
};
}

void AddSC_AscensionStormbringerTalents()
{
    new stormbringer_talent_casts();
    new stormbringer_resource_contracts();
    RegisterSpellScript(aura_ascension_barometric_pressure);
    RegisterSpellScript(aura_ascension_electrical_charge);
    RegisterSpellScript(aura_ascension_charged_conduit);
    RegisterSpellScript(aura_ascension_dark_skies);
}
