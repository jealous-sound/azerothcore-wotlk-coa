/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionReaperTalentProcs.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellInfo.h"
#include "SpellScript.h"
#include "Unit.h"
#include <algorithm>
#include <limits>

namespace
{
using AscensionReaperTalentProcs::Rules;

constexpr uint32 JailersWill = 524939;
constexpr uint32 JailersWillHelper = 578264;
constexpr float JailersWillStrengthCoefficient = 0.3f;
constexpr uint32 SiphonAnimaAura = 572768;
constexpr uint32 SiphonAnimaHeal = 504306;

class aura_ascension_reaper_jailers_will_helper : public AuraScript
{
    PrepareAuraScript(aura_ascension_reaper_jailers_will_helper);

    bool Validate(SpellInfo const* info) override
    {
        SpellEffectInfo const& effect = info->Effects[EFFECT_0];
        return info->Id == JailersWillHelper && info->SpellFamilyName == 36 &&
            effect.Effect == SPELL_EFFECT_APPLY_AURA && effect.ApplyAuraName == SPELL_AURA_ADD_FLAT_MODIFIER &&
            effect.MiscValue == SPELLMOD_EFFECT2 && effect.MiscValueB == 0 &&
            effect.BasePoints == 4 && effect.DieSides == 1 && !effect.RealPointsPerLevel &&
            effect.SpellClassMask == flag96(0, 2048, 0) &&
            effect.TargetA.GetTarget() == TARGET_UNIT_CASTER && effect.TargetB.GetTarget() == 0;
    }

    bool Load() override
    {
        Unit* owner = GetUnitOwner();
        return owner && owner->IsPlayer() && owner->getClass() == CLASS_REAPER &&
            GetCasterGUID() == owner->GetGUID() && owner->HasAura(JailersWill);
    }

    void Calculate(AuraEffect const*, int32& amount, bool& recalculate)
    {
        amount = int32(std::max(0.0f, GetUnitOwner()->GetStat(STAT_STRENGTH)) * JailersWillStrengthCoefficient);
        recalculate = true;
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(aura_ascension_reaper_jailers_will_helper::Calculate,
            EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER);
    }
};

class spell_ascension_reaper_talent_proc : public AuraScript
{
    PrepareAuraScript(spell_ascension_reaper_talent_proc);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        auto rule = std::find_if(Rules.begin(), Rules.end(),
            [this](AscensionReaperTalentProcs::Rule const& entry)
            {
                return entry.Talent == GetId();
            });
        if (rule == Rules.end())
            return false;

        if (!rule->Spells[0])
            return true;

        SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
        if (!spellInfo)
            return false;

        return std::find(rule->Spells.begin(), rule->Spells.end(), spellInfo->Id) !=
            rule->Spells.end();
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_ascension_reaper_talent_proc::CheckProc);
    }
};

class aura_ascension_reaper_soulrot : public AuraScript
{
    PrepareAuraScript(aura_ascension_reaper_soulrot);

    void HandleDispel(DispelInfo* dispelInfo)
    {
        if (!dispelInfo)
            return;

        Unit* caster = GetCaster();
        Unit* target = GetUnitOwner();
        Unit* dispeller = dispelInfo->GetDispeller();
        if (!caster || !target || !dispeller || !caster->IsInWorld() || !dispeller->IsInWorld())
            return;

        if (target->IsPvP() && dispeller->IsPlayer())
            dispeller->ToPlayer()->UpdatePvP(true);

        caster->CastSpell(dispeller, 805089, true);
    }

    void Register() override
    {
        AfterDispel += AuraDispelFn(aura_ascension_reaper_soulrot::HandleDispel);
    }
};

class aura_ascension_reaper_siphon_anima : public AuraScript
{
    PrepareAuraScript(aura_ascension_reaper_siphon_anima);

    bool Validate(SpellInfo const* info) override
    {
        return info->Id == SiphonAnimaAura &&
            info->Effects[EFFECT_0].TriggerSpell == SiphonAnimaHeal &&
            ValidateSpellInfo({SiphonAnimaHeal});
    }

    bool CheckProc(ProcEventInfo& event)
    {
        Unit* owner = GetTarget();
        Unit* victim = event.GetActionTarget();
        DamageInfo const* damage = event.GetDamageInfo();
        return owner->IsAlive() && event.GetActor() == owner && victim && victim != owner &&
            !owner->IsFriendlyTo(victim) && damage && damage->GetDamage();
    }

    void Heal(AuraEffect const* effect, ProcEventInfo& event)
    {
        PreventDefaultAction();
        uint64 amount = uint64(event.GetDamageInfo()->GetDamage()) * std::max(0, effect->GetAmount()) / 100;
        if (amount)
            GetTarget()->CastCustomSpell(SiphonAnimaHeal, SPELLVALUE_BASE_POINT0,
                int32(std::min<uint64>(amount, std::numeric_limits<int32>::max())), GetTarget(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_reaper_siphon_anima::CheckProc);
        OnEffectProc += AuraEffectProcFn(aura_ascension_reaper_siphon_anima::Heal, EFFECT_0, AuraType(354));
    }
};
}

void AddSC_AscensionReaperTalentProcs()
{
    RegisterSpellScript(aura_ascension_reaper_jailers_will_helper);
    RegisterSpellScript(spell_ascension_reaper_talent_proc);
    RegisterSpellScript(aura_ascension_reaper_soulrot);
    RegisterSpellScript(aura_ascension_reaper_siphon_anima);
}
