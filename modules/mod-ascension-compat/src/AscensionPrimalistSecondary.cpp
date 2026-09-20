/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include <algorithm>
#include <limits>

namespace
{
enum PrimalistSecondarySpells : uint32
{
    SPELL_ANCIENT_OF_LORE = 805105,
    SPELL_ANCIENT_SLOW = 504369,
    SPELL_VOLCANIC_BLAST = 681353,
    SPELL_HAMMER_OF_LIFE_HEAL = 806073,
    SPELL_HAMMER_OF_LIFE_DAMAGE = 807651,
    SPELL_CRACKING_EARTH = 560145,
    SPELL_CRACKING_STACK = 560146,
    SPELL_EARTHS_EMBRACE = 806143,
    SPELL_EMBRACED_BY_EARTH = 561037,
    SPELL_EMBRACE_DISORIENT = 706200,
    SPELL_GAZE = 805919,
    SPELL_GAZE_SLOW = 572908
};

class primalist_secondary_auras : public UnitScript
{
public:
    primalist_secondary_auras() : UnitScript("primalist_secondary_auras", true,
        {UNITHOOK_ON_AURA_APPLY, UNITHOOK_ON_AURA_REMOVE}) { }

    void OnAuraApply(Unit* unit, Aura* aura) override
    {
        Player* player = unit ? unit->ToPlayer() : nullptr;
        if (player && player->getClass() == CLASS_WILDWALKER && aura &&
            aura->GetId() == SPELL_ANCIENT_OF_LORE && aura->GetCasterGUID() == player->GetGUID() &&
            !player->HasAura(SPELL_ANCIENT_SLOW, player->GetGUID()))
            player->AddAura(SPELL_ANCIENT_SLOW, player);
    }

    void OnAuraRemove(Unit* unit, AuraApplication* application, AuraRemoveMode mode) override
    {
        Player* player = unit ? unit->ToPlayer() : nullptr;
        if (!player || player->getClass() != CLASS_WILDWALKER || !application)
            return;
        Aura* aura = application->GetBase();
        if (aura->GetCasterGUID() != player->GetGUID())
            return;
        if (aura->GetId() == SPELL_ANCIENT_OF_LORE)
            player->RemoveAurasDueToSpell(SPELL_ANCIENT_SLOW, player->GetGUID());
        if (aura->GetId() == SPELL_CRACKING_EARTH)
            player->RemoveAurasDueToSpell(SPELL_CRACKING_STACK, player->GetGUID());
        if (aura->GetId() == SPELL_EARTHS_EMBRACE && mode == AURA_REMOVE_BY_EXPIRE &&
            player->IsAlive() && player->IsInWorld() && player->HasAura(SPELL_EMBRACED_BY_EARTH, player->GetGUID()))
            player->CastSpell(player, SPELL_EMBRACE_DISORIENT, true);
    }
};

class aura_ascension_volcanic_blast : public AuraScript
{
    PrepareAuraScript(aura_ascension_volcanic_blast);

    bool Check(ProcEventInfo& event)
    {
        Unit* player = GetTarget();
        Unit* victim = event.GetActionTarget();
        DamageInfo const* damage = event.GetDamageInfo();
        return player->IsPlayer() && player->getClass() == CLASS_WILDWALKER && player->IsAlive() &&
            GetCaster() == player && event.GetActor() == player && victim && victim != player &&
            !player->IsFriendlyTo(victim) && damage && damage->GetDamage() &&
            (event.GetHitMask() & PROC_HIT_CRITICAL) &&
            (damage->GetSchoolMask() & (SPELL_SCHOOL_MASK_NORMAL | SPELL_SCHOOL_MASK_NATURE));
    }

    void Proc(AuraEffect const* effect, ProcEventInfo& event)
    {
        PreventDefaultAction();
        uint64 amount = uint64(event.GetDamageInfo()->GetDamage()) * std::clamp(effect->GetAmount(), 0, 100) / 100;
        if (amount)
            GetTarget()->CastCustomSpell(SPELL_VOLCANIC_BLAST, SPELLVALUE_BASE_POINT0,
                int32(std::min<uint64>(amount, std::numeric_limits<int32>::max())),
                event.GetActionTarget(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_volcanic_blast::Check);
        OnEffectProc += AuraEffectProcFn(aura_ascension_volcanic_blast::Proc, EFFECT_0, AuraType(354));
    }
};

class aura_ascension_natures_blessing : public AuraScript
{
    PrepareAuraScript(aura_ascension_natures_blessing);

    bool Validate(SpellInfo const* info) override
    {
        return info->SpellFamilyName == 37 && info->Effects[EFFECT_0].ApplyAuraName == 354 &&
            info->Effects[EFFECT_0].TriggerSpell == 807561 && ValidateSpellInfo({807561});
    }

    bool Load() override
    {
        return GetUnitOwner()->IsPlayer() && GetUnitOwner()->getClass() == CLASS_WILDWALKER;
    }

    bool Check(ProcEventInfo& event)
    {
        SpellInfo const* info = event.GetSpellInfo();
        HealInfo const* heal = event.GetHealInfo();
        Unit* target = event.GetActionTarget();
        bool seismicWave = info && (info->Id == 805462 || (info->Id >= 572873 && info->Id <= 572878));
        return seismicWave && event.GetActor() == GetTarget() && target && target->IsAlive() &&
            GetTarget()->IsFriendlyTo(target) && heal && heal->GetHeal();
    }

    void Proc(AuraEffect const* effect, ProcEventInfo& event)
    {
        PreventDefaultAction();
        // As with native Living Seed, use the completed heal (including its crit),
        // before overhealing. The tooltip specifies 20% on each of three ticks.
        uint64 amount = uint64(event.GetHealInfo()->GetHeal()) * std::clamp(effect->GetAmount(), 0, 100) / 100;
        if (amount)
            GetTarget()->CastCustomSpell(807561, SPELLVALUE_BASE_POINT0,
                int32(std::min<uint64>(amount, std::numeric_limits<int32>::max())),
                event.GetActionTarget(), TRIGGERED_FULL_MASK, nullptr, effect);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_natures_blessing::Check);
        OnEffectProc += AuraEffectProcFn(aura_ascension_natures_blessing::Proc, EFFECT_0, AuraType(354));
    }
};

// Hammer of Life (803973): melee attacks and abilities heal nearby allies and damage nearby enemies for $s1% of the
// damage dealt. Both helpers use effect 0's percentage, as the tooltip does; effect 2's own 30% is not described.
// The helpers pick their targets (up to 3 each, around the caster) from their own Spell.dbc records.
class aura_ascension_hammer_of_life : public AuraScript
{
    PrepareAuraScript(aura_ascension_hammer_of_life);

    bool Check(ProcEventInfo& event)
    {
        Unit* player = GetTarget();
        Unit* victim = event.GetActionTarget();
        DamageInfo const* damage = event.GetDamageInfo();
        return player->IsPlayer() && player->getClass() == CLASS_WILDWALKER && player->IsAlive() &&
            GetCaster() == player && event.GetActor() == player && victim && victim != player &&
            !player->IsFriendlyTo(victim) && damage && damage->GetDamage();
    }

    void Proc(AuraEffect const* effect, ProcEventInfo& event)
    {
        PreventDefaultAction();
        uint64 amount = uint64(event.GetDamageInfo()->GetDamage()) * std::clamp(effect->GetAmount(), 0, 100) / 100;
        if (!amount)
            return;
        int32 value = int32(std::min<uint64>(amount, std::numeric_limits<int32>::max()));
        GetTarget()->CastCustomSpell(SPELL_HAMMER_OF_LIFE_HEAL, SPELLVALUE_BASE_POINT0, value, GetTarget(),
            TRIGGERED_FULL_MASK);
        GetTarget()->CastCustomSpell(SPELL_HAMMER_OF_LIFE_DAMAGE, SPELLVALUE_BASE_POINT0, value, GetTarget(),
            TRIGGERED_FULL_MASK);
    }

    void IgnoreSecondTrigger(AuraEffect const* /*effect*/, ProcEventInfo& /*event*/)
    {
        PreventDefaultAction();
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_hammer_of_life::Check);
        OnEffectProc += AuraEffectProcFn(aura_ascension_hammer_of_life::Proc, EFFECT_0, AuraType(354));
        OnEffectProc += AuraEffectProcFn(aura_ascension_hammer_of_life::IgnoreSecondTrigger, EFFECT_2, AuraType(354));
    }
};

class primalist_volcanic_targets : public AllSpellScript
{
public:
    primalist_volcanic_targets() : AllSpellScript("primalist_volcanic_targets", {ALLSPELLHOOK_ON_HIT_RESULT}) { }

    void OnSpellHitResult(Spell* spell, Unit* target, uint8 miss, uint32, uint32, bool) override
    {
        Player* player = spell->GetCaster()->ToPlayer();
        if (!player || player->getClass() != CLASS_WILDWALKER || !player->IsAlive() ||
            spell->GetSpellInfo()->Id != SPELL_VOLCANIC_BLAST || miss != SPELL_MISS_NONE ||
            !target || target == player || player->IsFriendlyTo(target) ||
            !player->HasAura(SPELL_CRACKING_EARTH, player->GetGUID()))
            return;
        // The native result callback visits each unique target once, after all its effects.
        uint64 count = spell->GetScriptValue(SPELL_CRACKING_STACK) + 1;
        spell->SetScriptValue(SPELL_CRACKING_STACK, count);
        if (count == 5)
            player->CastSpell(player, SPELL_CRACKING_STACK, true);
    }
};

class spell_ascension_gaze_of_theradras : public SpellScript
{
    PrepareSpellScript(spell_ascension_gaze_of_theradras);

    bool Load() override { return GetCaster()->IsPlayer() && GetCaster()->getClass() == CLASS_WILDWALKER; }

    void Before(SpellMissInfo miss)
    {
        Unit* target = GetHitUnit();
        _newVictim = miss == SPELL_MISS_NONE && target && target->CanHaveThreatList() &&
            target->GetVictim() != GetCaster();
    }

    void After()
    {
        Unit* target = GetHitUnit();
        if (_newVictim && target && target->IsAlive() && target->HasAura(SPELL_GAZE, GetCaster()->GetGUID()))
            GetCaster()->CastSpell(target, SPELL_GAZE_SLOW, true);
    }

    void Register() override
    {
        BeforeHit += BeforeSpellHitFn(spell_ascension_gaze_of_theradras::Before);
        AfterHit += SpellHitFn(spell_ascension_gaze_of_theradras::After);
    }

    bool _newVictim = false;
};

class primalist_secondary_metadata : public GlobalScript
{
public:
    primalist_secondary_metadata() : GlobalScript("primalist_secondary_metadata",
        {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* info) override
    {
        if (info->SpellFamilyName != 37)
            return;
        if (info->Id == SPELL_VOLCANIC_BLAST)
        {
            info->AttributesEx2 |= SPELL_ATTR2_CANT_CRIT;
            info->AttributesEx3 |= SPELL_ATTR3_IGNORE_CASTER_MODIFIERS;
            info->AttributesEx4 |= SPELL_ATTR4_IGNORE_DAMAGE_TAKEN_MODIFIERS;
            info->AscensionInheritsResolvedAmount = true;
            info->Effects[EFFECT_0].BonusMultiplier = 0.0f;
        }
        if (info->Id == SPELL_CRACKING_EARTH)
        {
            info->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
            info->ProcFlags = 0;
        }
        if (info->Id == SPELL_CRACKING_STACK)
            // E2 supplies native chain jumps. E1 is the separate area target-cap modifier.
            info->Effects[EFFECT_1].ApplyAuraName = SPELL_AURA_MOD_MAX_AFFECTED_TARGETS;
        if (info->Id == SPELL_EMBRACE_DISORIENT)
            info->AuraInterruptFlags |= AURA_INTERRUPT_FLAG_TAKE_DAMAGE;
        if (info->Id == SPELL_GAZE_SLOW)
            info->ProcCharges = 1; // Native proc data spends this on the next melee/ranged swing, including misses.
        if (info->Id == SPELL_ANCIENT_SLOW || info->Id == SPELL_CRACKING_STACK || info->Id == SPELL_GAZE_SLOW)
        {
            info->AttributesCu &= ~SPELL_ATTR0_CU_FORCE_AURA_SAVING;
            info->AttributesCu |= SPELL_ATTR0_CU_AURA_CANNOT_BE_SAVED;
        }
    }
};

// The Primalist spends Rage - 123 of its family-37 records carry a Rage cost, Seismic Smash among
// them - but ChrClasses.dbc gives class 31 Mana as its display power. Unit::DealDamage grants Rage for
// melee damage dealt and for damage received only when HasActivePowerType(POWER_RAGE) holds, and that
// helper asks the player scripts first, otherwise comparing the display power, so auto attacks built
// no Rage at all.
class primalist_resources : public PlayerScript
{
public:
    primalist_resources()
        : PlayerScript("primalist_resources", {PLAYERHOOK_ON_PLAYER_HAS_ACTIVE_POWER_TYPE})
    {
    }

    bool OnPlayerHasActivePowerType(Player const* player, Powers power) override
    {
        return player && player->getClass() == CLASS_WILDWALKER && power == POWER_RAGE;
    }
};
}

void AddSC_AscensionPrimalistSecondary()
{
    new primalist_secondary_auras();
    new primalist_resources();
    new primalist_volcanic_targets();
    new primalist_secondary_metadata();
    RegisterSpellScript(aura_ascension_volcanic_blast);
    RegisterSpellScript(aura_ascension_natures_blessing);
    RegisterSpellScript(aura_ascension_hammer_of_life);
    RegisterSpellScript(spell_ascension_gaze_of_theradras);
}
