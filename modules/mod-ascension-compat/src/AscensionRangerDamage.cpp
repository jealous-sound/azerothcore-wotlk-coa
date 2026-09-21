/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionRangerDamage.h"
#include "Log.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellScript.h"
#include "WorldSession.h"
#include <algorithm>
#include <limits>

namespace
{
constexpr uint32 SPELL_RANGER_RUSTY_SHIV = 561315;
constexpr uint32 SPELL_RANGER_RUSTY_SHIV_DAMAGE = 681459;
constexpr uint32 RANGER_SPELL_FAMILY = 27;
constexpr uint32 SPELL_RANGER_WILD_STRIKE_OFF_HAND = 560962;
// The copied helper payload: BasePoints 318 with DieSides 1 resolves to +319.
constexpr int32 WILD_STRIKE_OFF_HAND_COPIED_BASE_POINTS = 318;
// CoA changelog 71878 raises the accumulated share from 10% to 20%.
constexpr uint32 RUSTY_SHIV_DAMAGE_DIVISOR = 5;

bool IsRustyShivContract(SpellInfo const* spellInfo)
{
    return spellInfo && spellInfo->Id == SPELL_RANGER_RUSTY_SHIV &&
        spellInfo->SpellFamilyName == RANGER_SPELL_FAMILY &&
        spellInfo->HasAttribute(SPELL_ATTR3_DOT_STACKING_RULE) &&
        spellInfo->Effects[EFFECT_0].IsAura(SPELL_AURA_DUMMY) &&
        spellInfo->Effects[EFFECT_0].TriggerSpell == SPELL_RANGER_RUSTY_SHIV_DAMAGE &&
        spellInfo->Effects[EFFECT_1].IsAura(SPELL_AURA_DUMMY) &&
        spellInfo->Effects[EFFECT_1].MiscValueB == 20;
}

class spell_ascension_ranger_rusty_shiv : public AuraScript
{
    PrepareAuraScript(spell_ascension_ranger_rusty_shiv);

    bool _paid = false;

    bool Validate(SpellInfo const* spellInfo) override
    {
        return IsRustyShivContract(spellInfo) && ValidateSpellInfo({ SPELL_RANGER_RUSTY_SHIV_DAMAGE });
    }

    bool Load() override
    {
        Unit* caster = GetCaster();
        return caster && caster->IsPlayer() && caster->ToPlayer()->getClass() == CLASS_RANGER;
    }

    void CalculateAmount(AuraEffect const* /*effect*/, int32& amount, bool& canBeRecalculated)
    {
        amount = 0;
        canBeRecalculated = false;
    }

    void ResetWindow(AuraEffect const* /*effect*/, AuraEffectHandleModes /*mode*/)
    {
        // Only recasting starts a new window. REAL application also runs when a
        // saved aura is loaded; its non-recalculable amounts must survive that.
        GetEffect(EFFECT_0)->SetAmount(0);
        GetEffect(EFFECT_1)->SetAmount(0);
        _paid = false;
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        DamageInfo const* damage = eventInfo.GetDamageInfo();
        Unit* caster = GetCaster();
        return caster && damage && damage->GetDamage() &&
            eventInfo.GetActor() == caster && damage->GetAttacker() == caster &&
            damage->GetVictim() == GetTarget() &&
            (!damage->GetSpellInfo() || damage->GetSpellInfo()->Id != SPELL_RANGER_RUSTY_SHIV_DAMAGE);
    }

    void Accumulate(AuraEffect const* effect, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        if (!CheckProc(eventInfo))
            return;

        AuraEffect* remainder = GetEffect(EFFECT_1);
        // Store the quotient and remainder in the aura, so native aura saving
        // retains the window. Round once at payout, not once per hit or tick.
        uint64 total = uint64(std::max(0, effect->GetAmount())) * RUSTY_SHIV_DAMAGE_DIVISOR;
        total += std::clamp(remainder->GetAmount(), 0, int32(RUSTY_SHIV_DAMAGE_DIVISOR - 1));
        total += eventInfo.GetDamageInfo()->GetDamage();
        uint64 maximum = uint64(std::numeric_limits<int32>::max()) * RUSTY_SHIV_DAMAGE_DIVISOR;
        total = std::min(total, maximum);
        GetEffect(EFFECT_0)->SetAmount(int32(total / RUSTY_SHIV_DAMAGE_DIVISOR));
        remainder->SetAmount(int32(total % RUSTY_SHIV_DAMAGE_DIVISOR));
    }

    void PayAtExpiry(AuraEffect const* effect, AuraEffectHandleModes /*mode*/)
    {
        if (_paid || GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
            return;
        _paid = true;

        Unit* caster = GetCaster();
        Unit* target = GetTarget();
        int32 amount = effect->GetAmount();
        if (!caster || !caster->IsPlayer() || caster->ToPlayer()->getClass() != CLASS_RANGER ||
            !caster->IsAlive() || !caster->IsInWorld() || !target->IsAlive() || !target->IsInWorld() ||
            caster->GetMap() != target->GetMap() || !caster->ToPlayer()->GetSession() ||
            caster->ToPlayer()->GetSession()->PlayerLogout() || amount <= 0)
            return;

        caster->CastCustomSpell(SPELL_RANGER_RUSTY_SHIV_DAMAGE, SPELLVALUE_BASE_POINT0,
            amount, target, TRIGGERED_FULL_MASK, nullptr, effect);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_ascension_ranger_rusty_shiv::CalculateAmount,
            EFFECT_ALL, SPELL_AURA_DUMMY);
        AfterEffectApply += AuraEffectApplyFn(spell_ascension_ranger_rusty_shiv::ResetWindow,
            EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAPPLY);
        DoCheckProc += AuraCheckProcFn(spell_ascension_ranger_rusty_shiv::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_ascension_ranger_rusty_shiv::Accumulate,
            EFFECT_0, SPELL_AURA_DUMMY);
        AfterEffectRemove += AuraEffectRemoveFn(spell_ascension_ranger_rusty_shiv::PayAtExpiry,
            EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_ascension_ranger_rusty_shiv_damage : public SpellScript
{
    PrepareSpellScript(spell_ascension_ranger_rusty_shiv_damage);

    bool Validate(SpellInfo const* spellInfo) override
    {
        SpellEffectInfo const& effect = spellInfo->Effects[EFFECT_0];
        return spellInfo->Id == SPELL_RANGER_RUSTY_SHIV_DAMAGE &&
            spellInfo->SpellFamilyName == RANGER_SPELL_FAMILY &&
            effect.Effect == SPELL_EFFECT_SCHOOL_DAMAGE && effect.DieSides == 1 &&
            !effect.RealPointsPerLevel && !effect.PointsPerComboPoint;
    }

    bool Load() override
    {
        return GetCaster()->IsPlayer() && GetCaster()->ToPlayer()->getClass() == CLASS_RANGER &&
            GetSpell()->IsTriggered();
    }

    void SetAccumulatedDamage(SpellEffIndex index)
    {
        PreventHitDefaultEffect(index);
        // This helper receives already-scaled damage. Bypass the old 10% SP
        // coefficient and a second pass of done/taken damage bonuses. Decode
        // CastCustomSpell's fixed-die payload without a lossy float conversion.
        int64 amount = int64(GetSpellValue()->EffectBasePoints[EFFECT_0]) + 1;
        SetHitDamage(int32(std::clamp<int64>(amount, 0, std::numeric_limits<int32>::max())));
        // Native hit resolution still owns immunity, resilience, resist, absorb,
        // combat logging and the helper's existing cannot-crit attribute.
    }

    void Register() override
    {
        OnEffectLaunchTarget += SpellEffectFn(spell_ascension_ranger_rusty_shiv_damage::SetAccumulatedDamage,
            EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

void ApplyWildStrikeOffHandContract(SpellInfo* spellInfo)
{
    if (spellInfo->Id != SPELL_RANGER_WILD_STRIKE_OFF_HAND ||
        spellInfo->SpellFamilyName != RANGER_SPELL_FAMILY)
        return;

    SpellEffectInfo& offHand = spellInfo->Effects[EFFECT_0];
    bool const copied = offHand.Effect == SPELL_EFFECT_NORMALIZED_WEAPON_DMG &&
        offHand.BasePoints == WILD_STRIKE_OFF_HAND_COPIED_BASE_POINTS && offHand.DieSides == 1;
    if (copied && !spellInfo->Effects[EFFECT_1].IsEffect() && !spellInfo->Effects[EFFECT_2].IsEffect())
    {
        // All twelve Wild Strike ranks (800083 and 501724-501734) trigger this one
        // shared helper, and its single slot is NORMALIZED_WEAPON_DMG. Spell::EffectWeaponDmg
        // accumulates such a slot's value into fixed_bonus, so the copied payload added a
        // flat +319 to the off-hand hit at every rank and every level -- +159 after the
        // native off-hand factor, which dwarfs a levelling weapon's own damage. The visible
        // parent promises only "additional Weapon Damage with your off-hand weapon" and this
        // helper carries no weapon-percent slot, so normalized weapon damage on its own is
        // the whole contract. Keep the slot; remove only the flat term.
        offHand.BasePoints = 0;
        offHand.DieSides = 0;
    }
    else
        LOG_ERROR("module.ascension_compat", "Skipped unexpected Wild Strike off-hand record {}", spellInfo->Id);
}
}

void ApplyAscensionRangerDamageContracts(SpellInfo* spellInfo)
{
    if (!spellInfo)
        return;

    ApplyWildStrikeOffHandContract(spellInfo);

    if (spellInfo->Id != SPELL_RANGER_RUSTY_SHIV ||
        spellInfo->SpellFamilyName != RANGER_SPELL_FAMILY)
        return;

    SpellEffectInfo& effect = spellInfo->Effects[EFFECT_1];
    if (spellInfo->Effects[EFFECT_0].IsAura(SPELL_AURA_DUMMY) &&
        spellInfo->Effects[EFFECT_0].TriggerSpell == SPELL_RANGER_RUSTY_SHIV_DAMAGE &&
        effect.IsAura(SPELL_AURA_SCHOOL_ABSORB) && effect.BasePoints == 0 &&
        effect.DieSides == 1 && effect.MiscValue == SPELL_SCHOOL_MASK_ALL && effect.MiscValueB == 10)
    {
        // The copied absorb slot belongs to the private accumulator, not to an
        // enemy shield. Two dummy amounts hold the stored damage and fraction.
        effect.ApplyAuraName = SPELL_AURA_DUMMY;
        effect.BasePoints = -1;
        effect.MiscValueB = 20;
    }

    // A damaging debuff leaves Elude on its successful cast, just like Toxic
    // Dart. Its later triggered payout must not interrupt newly gained stealth.
    if (IsRustyShivContract(spellInfo))
        spellInfo->AttributesEx &= ~SPELL_ATTR1_ALLOW_WHILE_STEALTHED;
}

void AddAscensionRangerDamageScripts()
{
    RegisterSpellScript(spell_ascension_ranger_rusty_shiv);
    RegisterSpellScript(spell_ascension_ranger_rusty_shiv_damage);
}
