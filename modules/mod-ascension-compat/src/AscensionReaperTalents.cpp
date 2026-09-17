/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionReaperTalents.h"
#include "CellImpl.h"
#include "GridNotifiersImpl.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include <algorithm>
#include <limits>
#include <list>

namespace
{
enum ReaperTalentSpells : uint32
{
    SPELL_HARVESTER_AMOUNT = 500283,
    SPELL_BLOOD_HARVEST = 504565,
    SPELL_UNDERWALK = 800797,
    SPELL_FROM_THE_SHADOWS = 561099,
    SPELL_FROM_THE_SHADOWS_CRIT = 561128,
    SPELL_REAPED_SOUL = 500363,
    SPELL_SOUL_CAPTURED = 572887,
    SPELL_SOUL_SPLINTERS = 805719,
    SPELL_SOUL_SPLINTER = 805720
};

class spell_ascension_soul_capture : public SpellScript
{
    PrepareSpellScript(spell_ascension_soul_capture);
    ObjectGuid _corpse;

    bool Validate(SpellInfo const*) override
    {
        return ValidateSpellInfo({SPELL_SOUL_CAPTURED, SPELL_REAPED_SOUL});
    }

    bool Eligible(Unit* unit)
    {
        Unit* caster = GetCaster();
        return unit && unit != caster && !unit->IsAlive() && !caster->IsFriendlyTo(unit) &&
            !unit->HasAura(SPELL_SOUL_CAPTURED) && caster->IsInMap(unit) && caster->InSamePhase(unit) &&
            caster->IsWithinDistInMap(unit, GetSpellInfo()->Effects[EFFECT_0].CalcRadius(caster)) &&
            caster->IsWithinLOSInMap(unit);
    }

    SpellCastResult CheckCorpse()
    {
        _corpse.Clear();
        Unit* caster = GetCaster();
        std::list<Unit*> corpses;
        Acore::AnyDeadUnitCheck check;
        Acore::UnitListSearcher<Acore::AnyDeadUnitCheck> searcher(caster, corpses, check);
        Cell::VisitObjects(caster, searcher, GetSpellInfo()->Effects[EFFECT_0].CalcRadius(caster));
        for (Unit* corpse : corpses)
            if (Eligible(corpse))
            {
                _corpse = corpse->GetGUID();
                return SPELL_CAST_OK;
            }
        return SPELL_FAILED_NO_EDIBLE_CORPSES;
    }

    void SuppressTrigger(SpellEffIndex index)
    {
        PreventHitDefaultEffect(index);
    }

    void Capture(SpellEffIndex index)
    {
        Unit* caster = GetCaster();
        Unit* corpse = ObjectAccessor::GetUnit(*caster, _corpse);
        // Recheck at impact: another cast can consume the same corpse after CheckCast.
        if (!Eligible(corpse) || !caster->AddAura(SPELL_SOUL_CAPTURED, corpse))
        {
            PreventHitDefaultEffect(index);
            return;
        }
        caster->CastSpell(caster, SPELL_REAPED_SOUL, true);
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_ascension_soul_capture::CheckCorpse);
        OnEffectLaunch += SpellEffectFn(spell_ascension_soul_capture::SuppressTrigger,
            EFFECT_ALL, SPELL_EFFECT_TRIGGER_SPELL);
        OnEffectLaunchTarget += SpellEffectFn(spell_ascension_soul_capture::SuppressTrigger,
            EFFECT_ALL, SPELL_EFFECT_TRIGGER_SPELL);
        OnEffectHitTarget += SpellEffectFn(spell_ascension_soul_capture::Capture, EFFECT_2, SPELL_EFFECT_HEAL_PCT);
    }
};

class aura_ascension_harvester : public AuraScript
{
    PrepareAuraScript(aura_ascension_harvester);

    bool Validate(SpellInfo const*) override
    {
        return ValidateSpellInfo({SPELL_HARVESTER_AMOUNT, SPELL_BLOOD_HARVEST});
    }

    bool CheckProc(ProcEventInfo& event)
    {
        Unit* owner = GetTarget();
        Unit* victim = event.GetActionTarget();
        return owner->IsPlayer() && owner->getClass() == CLASS_REAPER && owner->IsAlive() &&
            event.GetActor() == owner && victim && victim != owner && !owner->IsFriendlyTo(victim) &&
            event.GetDamageInfo() && event.GetDamageInfo()->GetDamage();
    }

    void Heal(AuraEffect const*, ProcEventInfo& event)
    {
        PreventDefaultAction();
        Unit* owner = GetTarget();
        int32 percent = sSpellMgr->GetSpellInfo(SPELL_HARVESTER_AMOUNT)->Effects[EFFECT_0].CalcValue(owner);
        uint64 amount = uint64(event.GetDamageInfo()->GetDamage()) * std::clamp(percent, 0, 100) / 100;
        if (amount)
            owner->CastCustomSpell(SPELL_BLOOD_HARVEST, SPELLVALUE_BASE_POINT0,
                int32(std::min<uint64>(amount, std::numeric_limits<int32>::max())), owner, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_harvester::CheckProc);
        OnEffectProc += AuraEffectProcFn(aura_ascension_harvester::Heal, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// The Jailer's Call: attacks against enemies below 20% health trigger its extra Shadow damage.
class aura_ascension_jailers_call : public AuraScript
{
    PrepareAuraScript(aura_ascension_jailers_call);

    bool CheckProc(ProcEventInfo& event)
    {
        Unit* victim = event.GetActionTarget();
        return victim && victim != GetTarget() && victim->IsAlive() && victim->HealthBelowPct(20);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_jailers_call::CheckProc);
    }
};

class reaper_talent_events : public UnitScript
{
public:
    reaper_talent_events() : UnitScript("reaper_talent_events", true,
        {UNITHOOK_ON_AURA_REMOVE, UNITHOOK_MODIFY_SPELL_EFFECT_BASE_VALUE}) { }

    void OnAuraRemove(Unit* unit, AuraApplication* application, AuraRemoveMode mode) override
    {
        Player* player = unit ? unit->ToPlayer() : nullptr;
        if (!player || player->getClass() != CLASS_REAPER || !application || !player->IsAlive() ||
            !player->IsInWorld() || mode == AURA_REMOVE_BY_DEATH)
            return;
        Aura* aura = application->GetBase();
        if (aura->GetId() == SPELL_UNDERWALK && aura->GetCasterGUID() == player->GetGUID() &&
            player->HasAura(SPELL_FROM_THE_SHADOWS))
            player->CastSpell(player, SPELL_FROM_THE_SHADOWS_CRIT, true);
    }

    void ModifySpellEffectBaseValue(Unit const* caster, SpellInfo const* info, uint8 index, float& value) override
    {
        if (caster && caster->IsPlayer() && caster->getClass() == CLASS_REAPER && info->Id == SPELL_SOUL_SPLINTER &&
            info->SpellFamilyName == 36 && index == EFFECT_0 && info->Effects[index].IsAura(SPELL_AURA_PERIODIC_DAMAGE))
            value += std::max(0.0f, caster->GetStat(STAT_STAMINA)) * 0.035f;
    }
};
}

bool HandleAscensionReaperResource(Player* player, uint32 spellId, int32 amount)
{
    if (player->getClass() != CLASS_REAPER || spellId != SPELL_REAPED_SOUL)
        return false;
    Aura* aura = player->GetAura(spellId, player->GetGUID());
    uint8 previous = aura ? aura->GetStackAmount() : 0;
    if (aura)
        aura->ModStackAmount(amount);
    else if (amount > 0)
        if (Aura* created = player->AddAura(spellId, player); created && amount > 1)
            created->ModStackAmount(amount - 1);
    // Re-resolve: spending the last stack removes the aura. A capped award,
    // loss, load or refresh is not a newly harvested soul.
    aura = player->GetAura(spellId, player->GetGUID());
    if (aura && aura->GetStackAmount() > previous && player->IsAlive() && player->HasAura(SPELL_SOUL_SPLINTERS))
        player->CastSpell(player, SPELL_SOUL_SPLINTER, true);
    return true;
}

void AddSC_AscensionReaperTalents()
{
    RegisterSpellScript(spell_ascension_soul_capture);
    RegisterSpellScript(aura_ascension_harvester);
    RegisterSpellScript(aura_ascension_jailers_call);
    new reaper_talent_events();
}
