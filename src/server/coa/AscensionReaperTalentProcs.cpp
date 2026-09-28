/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionReaperTalentProcs.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellInfo.h"
#include "SpellScript.h"
#include "Unit.h"
#include <algorithm>

namespace
{
using AscensionReaperTalentProcs::Rules;

constexpr uint32 JailersWill = 524939;
constexpr uint32 JailersWillHelper = 578264;
constexpr float JailersWillStrengthCoefficient = 0.3f;
constexpr uint32 DeathchaserExtender = 807546;
constexpr uint32 DeathchaserFirstRank = 805190;

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

class spell_ascension_reaper_chasing_death_extender : public SpellScript
{
    PrepareSpellScript(spell_ascension_reaper_chasing_death_extender);

    bool Validate(SpellInfo const* info) override
    {
        SpellEffectInfo const& effect = info->Effects[EFFECT_0];
        return info->Id == DeathchaserExtender && info->SpellFamilyName == 36 &&
            effect.Effect == SPELL_EFFECT_ASCENSION_MODIFY_AURA_DURATION && effect.BasePoints > 0 &&
            effect.MiscValue == int32(DeathchaserFirstRank) &&
            effect.TargetA.GetTarget() == TARGET_UNIT_TARGET_ENEMY &&
            ValidateSpellInfo({DeathchaserFirstRank});
    }

    void SustainDeathchaser(SpellEffIndex index)
    {
        PreventHitDefaultEffect(index);
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        int32 const sustainedDuration = GetEffectValue();
        if (!caster || !target || sustainedDuration <= 0)
            return;

        Aura* deathchaser = target->GetAuraOfRankedSpell(DeathchaserFirstRank, caster->GetGUID());
        if (!deathchaser || deathchaser->IsRemoved() || deathchaser->IsExpired())
            return;

        int32 const remaining = deathchaser->GetDuration();
        if (remaining <= 0 || remaining >= sustainedDuration)
            return;

        deathchaser->SetMaxDuration(deathchaser->GetMaxDuration() + sustainedDuration - remaining);
        deathchaser->SetDuration(sustainedDuration);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_ascension_reaper_chasing_death_extender::SustainDeathchaser,
            EFFECT_0, SPELL_EFFECT_ASCENSION_MODIFY_AURA_DURATION);
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
}

void AddSC_AscensionReaperTalentProcs()
{
    RegisterSpellScript(aura_ascension_reaper_jailers_will_helper);
    RegisterSpellScript(spell_ascension_reaper_talent_proc);
    RegisterSpellScript(spell_ascension_reaper_chasing_death_extender);
    RegisterSpellScript(aura_ascension_reaper_soulrot);
}
