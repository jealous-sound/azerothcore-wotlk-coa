/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "Pet.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"

namespace
{
Player* Primalist(Unit* unit)
{
    Player* player = unit ? unit->ToPlayer() : nullptr;
    return player && player->getClass() == CLASS_WILDWALKER ? player : nullptr;
}

class primalist_talent_events : public UnitScript
{
public:
    primalist_talent_events() : UnitScript("primalist_talent_events", true,
        {UNITHOOK_ON_DAMAGE, UNITHOOK_ON_AURA_REMOVE}) { }

    void OnDamage(Unit*, Unit* victim, uint32& damage) override
    {
        Player* player = Primalist(victim);
        if (!player || !player->IsAlive() || !damage || damage < player->GetHealth() ||
            !player->HasAura(560157) || player->HasSpellCooldown(560157))
            return;
        // DealDamage reaches this hook after mitigation and absorption. Mark the
        // native saved cooldown before casting the heal, including any nested events.
        player->AddSpellCooldown(560157, 0, 120000);
        damage = 0;
        player->CastSpell(player, 560179, true);
    }

    void OnAuraRemove(Unit* unit, AuraApplication* application, AuraRemoveMode mode) override
    {
        Player* player = Primalist(unit);
        if (!player || !player->IsAlive() || !player->IsInWorld() || !application ||
            mode == AURA_REMOVE_BY_DEATH || !player->HasAura(300728))
            return;
        Aura* aura = application->GetBase();
        if (aura->GetCasterGUID() != player->GetGUID())
            return;
        // Only the visible defenses, not their separately removed SLS helpers.
        if (aura->GetId() == 680421 || aura->GetId() == 800094 || aura->GetId() == 503630)
            player->CastSpell(player, 503716, true);
    }
};

class primalist_talent_casts : public AllSpellScript
{
public:
    primalist_talent_casts() : AllSpellScript("primalist_talent_casts", {ALLSPELLHOOK_ON_CRIT_CHANCE}) { }

    void OnSpellCritChance(Spell* spell, Unit* target, float& chance) override
    {
        Player* player = Primalist(spell->GetCaster());
        SpellInfo const* info = spell->GetSpellInfo();
        if (!player || !target || !player->HasAura(805336) || info->SpellFamilyName != 37 ||
            !(info->SpellFamilyFlags[0] & 8))
            return;
        for (AuraEffect const* effect : target->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE))
            if (effect->GetCasterGUID() == player->GetGUID() && effect->GetSpellInfo()->SpellFamilyName == 37 &&
                (effect->GetSpellInfo()->SpellFamilyFlags[0] & 64))
            {
                chance = 100.0f;
                break;
            }
    }
};

SpellCastResult CheckThroatClamp(Player* player, Unit* target)
{
    Pet* pet = player ? player->GetPet() : nullptr;
    if (!pet || !pet->IsAlive() || pet->GetOwnerGUID() != player->GetGUID())
        return SPELL_FAILED_NO_PET;
    if (!target || !pet->IsInMap(target) || !pet->InSamePhase(target) || !pet->IsValidAttackTarget(target))
        return SPELL_FAILED_BAD_TARGETS;
    if (pet->HasUnitState(UNIT_STATE_NOT_MOVE | UNIT_STATE_LOST_CONTROL))
        return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
    SpellInfo const* helper = sSpellMgr->GetSpellInfo(500811);
    if (!helper || !pet->IsWithinDistInMap(target, helper->GetMaxRange(false, pet)))
        return SPELL_FAILED_OUT_OF_RANGE;
    if (!pet->IsWithinLOSInMap(target))
        return SPELL_FAILED_LINE_OF_SIGHT;
    return SPELL_CAST_OK;
}

class spell_ascension_throat_clamp : public SpellScript
{
    PrepareSpellScript(spell_ascension_throat_clamp);

    bool Validate(SpellInfo const*) override { return ValidateSpellInfo({500811}); }
    bool Load() override { return Primalist(GetCaster()) != nullptr; }

    SpellCastResult CheckCast() { return CheckThroatClamp(Primalist(GetCaster()), GetExplTargetUnit()); }

    void Handle(SpellEffIndex)
    {
        Player* player = Primalist(GetCaster());
        Unit* target = GetHitUnit();
        if (CheckThroatClamp(player, target) == SPELL_CAST_OK)
            player->GetPet()->CastSpell(target, 500811, false); // Native dash, interrupt and school lockout.
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_ascension_throat_clamp::CheckCast);
        OnEffectHitTarget += SpellEffectFn(spell_ascension_throat_clamp::Handle, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};
}

void AddSC_AscensionPrimalistTalents()
{
    new primalist_talent_events();
    new primalist_talent_casts();
    RegisterSpellScript(spell_ascension_throat_clamp);
}
