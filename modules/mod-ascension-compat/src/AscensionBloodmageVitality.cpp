/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionPooledVitality.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuras.h"
#include "SpellScript.h"
#include <algorithm>
#include <cmath>
#include <limits>

namespace
{
using namespace AscensionBloodmage;
constexpr uint32 VitalityCost = 10;

bool IsBloodmage(Player const* player)
{
    return player && player->getClass() == CLASS_SON_OF_ARUGAL && player->IsAlive() && player->IsInWorld();
}

bool CanEmpower(Player* player)
{
    if (!IsBloodmage(player) || !player->HasAura(PooledVitalityTalent) || player->HasAura(CursedFormCheck) ||
        player->HasAura(CursedForm))
        return false;
    Aura const* pool = player->GetAura(PooledVitality, player->GetGUID());
    return pool && uint32(pool->GetStackAmount()) >= VitalityCost;
}

class bloodmage_vitality_casts : public AllSpellScript
{
public:
    bloodmage_vitality_casts() : AllSpellScript("bloodmage_vitality_casts",
        {ALLSPELLHOOK_CAN_PREPARE, ALLSPELLHOOK_ON_SPELL_CHECK_CAST, ALLSPELLHOOK_ON_BEFORE_EFFECTS,
            ALLSPELLHOOK_ON_CAST, ALLSPELLHOOK_ON_HIT_RESULT}) { }

    bool CanPrepare(Spell* spell, SpellCastTargets const*, AuraEffect const*) override
    {
        Player* player = spell->GetCaster()->ToPlayer();
        SpellInfo const* info = spell->GetSpellInfo();
        if (!spell->IsTriggered() && info->SpellFamilyName == 26 && info->PowerType == POWER_RAGE &&
            (info->ManaCost || info->ManaCostPercentage) && CanEmpower(player))
            spell->SetScriptValue(PooledVitalityTalent, GetEmpowerment(info->Id));
        return true;
    }

    void OnSpellCheckCast(Spell* spell, bool, SpellCastResult& result) override
    {
        // A cast prepared with an instant/free benefit cannot finish with stacks
        // that expired or were spent by a different cast during its cast time.
        if (spell->GetScriptValue(PooledVitalityTalent) && !CanEmpower(spell->GetCaster()->ToPlayer()))
            result = SPELL_FAILED_CASTER_AURASTATE;
    }

    void OnSpellBeforeEffects(Spell* spell, Unit* caster, SpellInfo const*) override
    {
        Player* player = caster->ToPlayer();
        if (!spell->GetScriptValue(PooledVitalityTalent) || spell->GetScriptValue(PooledVitality) ||
            !CanEmpower(player))
            return;
        spell->SetScriptValue(PooledVitality, 1);
        player->GetAura(PooledVitality, player->GetGUID())->ModStackAmount(-int32(VitalityCost));
    }

    void OnSpellCast(Spell* spell, Unit* caster, SpellInfo const* info, bool) override
    {
        Player* player = caster->ToPlayer();
        if (!IsBloodmage(player) || spell->IsTriggered() || info->SpellFamilyName != 26 ||
            !spell->GetScriptValue(PooledVitality) || spell->GetScriptValue(VitalityForLater))
            return;
        spell->SetScriptValue(VitalityForLater, 1);
        if (spell->GetScriptValue(PooledVitalityTalent))
        {
            if (player->HasAura(VitalityForLater))
                player->CastSpell(player, VitalityHeal, true);
        }
        else if (info->PowerType == POWER_HEALTH && player->HasAura(PooledVitalityTalent))
            player->CastSpell(player, PooledVitality, true);
    }

    void OnSpellHitResult(Spell* spell, Unit*, uint8 miss, uint32, uint32 healing, bool) override
    {
        Player* player = spell->GetCaster()->ToPlayer();
        if (!IsBloodmage(player) || spell->IsTriggered() || miss != SPELL_MISS_NONE || !healing ||
            spell->GetScriptValue(PooledVitalityTalent) != Mend || spell->GetScriptValue(MendSelfHeal))
            return;
        spell->SetScriptValue(MendSelfHeal, 1);
        // Custom basepoints pass through native float arithmetic before returning to int32.
        uint32 maximum = uint32(std::nextafter(float(std::numeric_limits<int32>::max()), 0.0f));
        int32 amount = int32(std::min(healing / 2, maximum));
        if (amount)
            player->CastCustomSpell(player, MendSelfHeal, &amount, nullptr, nullptr, true);
    }
};

class bloodmage_vitality_scaling : public UnitScript
{
public:
    bloodmage_vitality_scaling() : UnitScript("bloodmage_vitality_scaling", true,
        {UNITHOOK_MODIFY_SPELL_EFFECT_BASE_VALUE}) { }

    void ModifySpellEffectBaseValue(Unit const* caster, SpellInfo const* info, uint8 index, float& value) override
    {
        if (!caster || !caster->IsPlayer() || caster->getClass() != CLASS_SON_OF_ARUGAL ||
            info->Id != VitalityHeal || info->SpellFamilyName != 26 || index != EFFECT_0 ||
            info->Effects[EFFECT_0].Effect != SPELL_EFFECT_HEAL)
            return;
        double amount = double(value) + caster->GetStat(STAT_SPIRIT) * 0.5;
        if (std::isfinite(amount) && amount >= 0 &&
            double(float(amount)) <= std::numeric_limits<int32>::max())
            value = float(amount);
    }
};

class spell_ascension_bloodmage_empowered : public SpellScript
{
    PrepareSpellScript(spell_ascension_bloodmage_empowered);
    bool _heartbreak = false;

    bool Load() override
    {
        return GetCaster()->IsPlayer() && GetCaster()->getClass() == CLASS_SON_OF_ARUGAL &&
            GetSpellInfo()->SpellFamilyName == 26;
    }

    bool Empowered(Empowerment kind)
    {
        return !GetSpell()->IsTriggered() && GetSpell()->GetScriptValue(PooledVitalityTalent) == kind;
    }

    void HeartbreakPower(SpellEffIndex index)
    {
        if (GetSpellInfo()->Effects[index].TriggerSpell != HeartbreakBuff)
            return;
        // Native effect 142 otherwise grants the empowered buff unconditionally,
        // and its old enemy selector gives the party buff the wrong anchor.
        PreventHitDefaultEffect(index);
        if (!Empowered(Heartbreak) || _heartbreak)
            return;
        _heartbreak = true;
        int32 amount = GetEffectValue();
        Unit* player = GetCaster();
        player->CastCustomSpell(player, HeartbreakBuff, &amount, &amount, nullptr, true);
        player->CastSpell(player, VisceralPower, true);
    }

    void ModifyHit()
    {
        if (Empowered(Bloodbolt) && GetHitUnit() == GetExplTargetUnit() && GetHitDamage() > 0)
            SetHitDamage(int32(std::min<int64>(int64(GetHitDamage()) * 2, std::numeric_limits<int32>::max())));
        if (Empowered(Fleshcraft) && GetHitUnit() && GetHitUnit()->IsAlive() && GetHitHeal() > 0)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            uint32 bonus = caster->SpellHealingBonusDone(target, GetSpellInfo(),
                caster->CountPctFromMaxHealth(25), HEAL, EFFECT_0);
            bonus = target->SpellHealingBonusTaken(caster, GetSpellInfo(), bonus, HEAL);
            SetHitHeal(int32(std::min<int64>(int64(GetHitHeal()) + bonus, std::numeric_limits<int32>::max())));
        }
    }

    void Register() override
    {
        if (GetEmpowerment(m_scriptSpellId) == Heartbreak)
            OnEffectLaunchTarget += SpellEffectFn(spell_ascension_bloodmage_empowered::HeartbreakPower,
                EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL_WITH_VALUE);
        OnHit += SpellHitFn(spell_ascension_bloodmage_empowered::ModifyHit);
    }
};
}

void AddSC_AscensionBloodmageVitality()
{
    new bloodmage_vitality_casts();
    new bloodmage_vitality_scaling();
    RegisterSpellScript(spell_ascension_bloodmage_empowered);
}
