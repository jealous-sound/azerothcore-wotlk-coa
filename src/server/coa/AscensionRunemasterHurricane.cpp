/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"

namespace
{
enum HurricaneSpells : uint32
{
    SPELL_HURRICANE = 645435,
    SPELL_HURRICANE_HIT = 645437,
    SPELL_HURRICANE_DODGE = 645440,
    SPELL_WAVEFORGED = 705565,
    SPELL_WAVEFORGED_READY = 500469
};

bool StrikeHurricane(Unit* player, Aura* aura)
{
    Unit* target = ObjectAccessor::GetUnit(*player, ObjectGuid(aura->GetScriptValue(SPELL_HURRICANE)));
    if (!player->IsAlive() || !player->IsInWorld() || !target || !target->IsInWorld() ||
        player->GetMap() != target->GetMap() || !player->InSamePhase(target) ||
        !player->IsValidAttackTarget(target))
        return false;
    player->CastSpell(target, SPELL_HURRICANE_HIT, true);
    return true;
}

class runemaster_hurricane_cast : public AllSpellScript
{
public:
    runemaster_hurricane_cast() : AllSpellScript("runemaster_hurricane_cast", {ALLSPELLHOOK_ON_CAST}) { }

    void OnSpellCast(Spell* spell, Unit* caster, SpellInfo const* info, bool) override
    {
        if (!caster->IsPlayer() || caster->getClass() != CLASS_SPIRIT_MAGE ||
            info->Id != SPELL_HURRICANE || spell->IsTriggered())
            return;
        Aura* aura = caster->GetAura(SPELL_HURRICANE, caster->GetGUID());
        Unit* target = spell->m_targets.GetUnitTarget();
        if (!aura || !target)
            return;
        aura->SetScriptValue(SPELL_HURRICANE, target->GetGUID().GetRawValue());
        if (Aura* dodge = caster->AddAura(SPELL_HURRICANE_DODGE, caster))
        {
            dodge->SetMaxDuration(-1);
            dodge->SetDuration(-1);
        }
        StrikeHurricane(caster, aura);
    }
};

class aura_ascension_runemaster_hurricane : public AuraScript
{
    PrepareAuraScript(aura_ascension_runemaster_hurricane);

    bool Load() override
    {
        return GetCaster() && GetCaster() == GetUnitOwner() && GetCaster()->IsPlayer() &&
            GetCaster()->getClass() == CLASS_SPIRIT_MAGE;
    }

    void Tick(AuraEffect const*)
    {
        if (!StrikeHurricane(GetTarget(), GetAura()))
            GetAura()->Remove();
    }

    void End(AuraEffect const*, AuraEffectHandleModes)
    {
        Unit* player = GetTarget();
        player->RemoveAurasDueToSpell(SPELL_HURRICANE_DODGE, player->GetGUID());
        if (player->IsAlive() && player->IsInWorld() &&
            GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_DEATH &&
            player->HasAura(SPELL_WAVEFORGED, player->GetGUID()))
            player->CastSpell(player, SPELL_WAVEFORGED_READY, true);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(aura_ascension_runemaster_hurricane::Tick,
            EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        AfterEffectRemove += AuraEffectRemoveFn(aura_ascension_runemaster_hurricane::End,
            EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_ascension_hurricane_damage : public SpellScript
{
    PrepareSpellScript(spell_ascension_hurricane_damage);

    bool Load() override { return GetCaster()->IsPlayer() && GetCaster()->getClass() == CLASS_SPIRIT_MAGE; }

    void Scale()
    {
        double level = GetCaster()->GetLevel();
        double scale = 0.0267291844060354 + 0.0048541098014737 * level +
            0.0001859597762293 * level * level;
        GetSpell()->SetSpellValue(SPELLVALUE_BASE_POINT2,
            int32((GetSpellInfo()->Effects[EFFECT_2].BasePoints + 1) * scale));
    }

    void Hit()
    {
        if (GetHitUnit()->IsPlayer())
            SetHitDamage(CalculatePct(GetHitDamage(), 80));
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_ascension_hurricane_damage::Scale);
        OnHit += SpellHitFn(spell_ascension_hurricane_damage::Hit);
    }
};

class runemaster_hurricane_metadata : public GlobalScript
{
public:
    runemaster_hurricane_metadata() : GlobalScript("runemaster_hurricane_metadata",
        {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* info) override
    {
        if (info->SpellFamilyName != 38)
            return;
        if (info->Id == SPELL_HURRICANE)
            info->AttributesEx5 &= ~SPELL_ATTR5_EXTRA_INITIAL_PERIOD;
        if (info->Id == SPELL_HURRICANE || info->Id == SPELL_HURRICANE_DODGE || info->Id == SPELL_WAVEFORGED_READY)
        {
            info->AttributesCu &= ~SPELL_ATTR0_CU_FORCE_AURA_SAVING;
            info->AttributesCu |= SPELL_ATTR0_CU_AURA_CANNOT_BE_SAVED;
        }
    }
};
}

void AddSC_AscensionRunemasterHurricane()
{
    new runemaster_hurricane_cast();
    new runemaster_hurricane_metadata();
    RegisterSpellScript(aura_ascension_runemaster_hurricane);
    RegisterSpellScript(spell_ascension_hurricane_damage);
}
