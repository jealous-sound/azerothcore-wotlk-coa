/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionRunemasterBrand.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include <set>

namespace
{
constexpr uint32 SPELL_RUNIC_BRAND_MARK = 712323;
constexpr uint32 SPELL_RUNIC_EXPLOSION = 712324;

bool IsRunicBrand(uint32 id)
{
    return id == 712299 || (id >= 712301 && id <= 712307);
}

bool IsBrandRuneblade(uint32 id)
{
    return id == 707141 || (id >= 707143 && id <= 707148) || (id >= 573444 && id <= 573447);
}

class spell_ascension_runemaster_brand : public SpellScript
{
    PrepareSpellScript(spell_ascension_runemaster_brand);

    bool Validate(SpellInfo const* info) override
    {
        return info && IsRunicBrand(info->Id) && info->SpellFamilyName == uint32(CLASS_SPIRIT_MAGE) + 6 &&
            info->Effects[EFFECT_2].Effect == SPELL_EFFECT_TRIGGER_SPELL &&
            info->Effects[EFFECT_2].TriggerSpell == SPELL_RUNIC_BRAND_MARK &&
            ValidateSpellInfo({SPELL_RUNIC_BRAND_MARK});
    }

    bool Load() override
    {
        Unit* caster = GetCaster();
        return caster && caster->IsPlayer() && caster->getClass() == CLASS_SPIRIT_MAGE;
    }

    void PreventEarlyMark(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);
    }

    void MarkSuccessfulTarget()
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!target || target == caster || caster->IsFriendlyTo(target) || !_marked.insert(target->GetGUID()).second)
            return;

        // Reapplication uses native aura charge and duration refresh. Leyborn's
        // existing CHARGES modifier supplies two extra uses to this one-use mark.
        caster->CastSpell(target, SPELL_RUNIC_BRAND_MARK, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectLaunch += SpellEffectFn(spell_ascension_runemaster_brand::PreventEarlyMark,
            EFFECT_2, SPELL_EFFECT_TRIGGER_SPELL);
        OnEffectLaunchTarget += SpellEffectFn(spell_ascension_runemaster_brand::PreventEarlyMark,
            EFFECT_2, SPELL_EFFECT_TRIGGER_SPELL);
        AfterHit += SpellHitFn(spell_ascension_runemaster_brand::MarkSuccessfulTarget);
    }

    std::set<ObjectGuid> _marked;
};

class spell_ascension_runemaster_brand_runeblade : public SpellScript
{
    PrepareSpellScript(spell_ascension_runemaster_brand_runeblade);

    bool Validate(SpellInfo const* info) override
    {
        return info && IsBrandRuneblade(info->Id) && info->SpellFamilyName == uint32(CLASS_SPIRIT_MAGE) + 6 &&
            info->Effects[EFFECT_0].Effect == SPELL_EFFECT_SCHOOL_DAMAGE &&
            info->Effects[EFFECT_1].Effect == SPELL_EFFECT_TRIGGER_SPELL &&
            info->Effects[EFFECT_1].TriggerSpell == SPELL_RUNIC_EXPLOSION &&
            ValidateSpellInfo({SPELL_RUNIC_BRAND_MARK, SPELL_RUNIC_EXPLOSION});
    }

    bool Load() override
    {
        Unit* caster = GetCaster();
        return caster && caster->IsPlayer() && caster->getClass() == CLASS_SPIRIT_MAGE;
    }

    void PreventUnconditionalExplosion(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);
    }

    void SpendMarkOnSuccessfulHit(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!target || target == caster || caster->IsFriendlyTo(target) || !_processed.insert(target->GetGUID()).second)
            return;

        // Instant mixed-effect spells can reach HIT_TARGET before the native
        // damage-immunity check. Match its caster/piercing/friendly semantics;
        // mark ownership still belongs to this spell's immediate caster.
        Unit* damageCaster = GetOriginalCaster();
        if (target->IsImmunedToDamage(damageCaster ? damageCaster : caster, GetSpellInfo()))
            return;

        Aura* mark = target->GetAura(SPELL_RUNIC_BRAND_MARK, caster->GetGUID());
        if (!mark || !mark->IsUsingCharges() || !mark->GetCharges())
            return;

        // HIT_TARGET is after miss and effect-immunity validation, but before
        // health damage can remove the mark on death. A fully absorbed hit is
        // still a successful Runeblade. Spend now; explode after native damage.
        mark->ModCharges(-1);
        _pending.insert(target->GetGUID());
    }

    void ExplodeAfterHit()
    {
        Unit* target = GetHitUnit();
        if (!target || !_pending.erase(target->GetGUID()))
            return;

        // A killing hit still explodes at the impact position. Explicit ground
        // targeting does not require the now-dead primary target to be alive.
        GetCaster()->CastSpell(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(),
            SPELL_RUNIC_EXPLOSION, true);
    }

    void Register() override
    {
        OnEffectLaunch += SpellEffectFn(spell_ascension_runemaster_brand_runeblade::PreventUnconditionalExplosion,
            EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
        OnEffectLaunchTarget += SpellEffectFn(spell_ascension_runemaster_brand_runeblade::PreventUnconditionalExplosion,
            EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
        OnEffectHitTarget += SpellEffectFn(spell_ascension_runemaster_brand_runeblade::SpendMarkOnSuccessfulHit,
            EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        AfterHit += SpellHitFn(spell_ascension_runemaster_brand_runeblade::ExplodeAfterHit);
    }

    std::set<ObjectGuid> _processed;
    std::set<ObjectGuid> _pending;
};
}

void ApplyAscensionRunemasterBrandContracts(SpellInfo* info)
{
    if (!info || info->SpellFamilyName != uint32(CLASS_SPIRIT_MAGE) + 6)
        return;

    SpellEffectInfo& effect = info->Effects[EFFECT_0];
    if (info->Id == SPELL_RUNIC_BRAND_MARK && !info->ProcFlags && !info->ProcCharges && !info->StackAmount &&
        effect.IsAura(SPELL_AURA_DUMMY) && effect.TargetA.GetTarget() == TARGET_UNIT_TARGET_ENEMY &&
        !effect.TargetB.GetTarget() && info->SpellFamilyFlags == flag96(1048576, 0, 0) &&
        !info->Effects[EFFECT_1].Effect && !info->Effects[EFFECT_2].Effect)
        info->ProcCharges = 1;

    if (info->Id == SPELL_RUNIC_EXPLOSION && effect.Effect == SPELL_EFFECT_SCHOOL_DAMAGE &&
        effect.TargetA.GetTarget() == TARGET_DEST_TARGET_ENEMY && effect.TargetB.GetTarget() == TARGET_UNIT_DEST_AREA_ENEMY &&
        info->SchoolMask == SPELL_SCHOOL_MASK_FIRE && info->SpellFamilyFlags == flag96(0, 32, 0) &&
        !info->Effects[EFFECT_1].Effect && !info->Effects[EFFECT_2].Effect)
    {
        // The current visible mark promises Spellfire; the hidden helper still
        // carries its older Fire school and description.
        info->SchoolMask = SPELL_SCHOOL_MASK_FIRE | SPELL_SCHOOL_MASK_ARCANE;
        effect.TargetA = SpellImplicitTargetInfo(TARGET_DEST_DEST);
        info->_InitializeExplicitTargetMask();
    }
}

void AddAscensionRunemasterBrandScripts()
{
    RegisterSpellScript(spell_ascension_runemaster_brand);
    RegisterSpellScript(spell_ascension_runemaster_brand_runeblade);
}
