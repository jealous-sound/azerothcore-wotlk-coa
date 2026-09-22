/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionChronomancerTalents.h"
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
enum ChronomancerTalentSpells : uint32
{
    SPELL_ROLL_BACK = 804490,
    SPELL_TIME_SKIP = 804451,
    SPELL_SHIMMERING_SHARD = 806302,
    SPELL_SHIMMER = 806303,
    SPELL_THROUGH_THE_AEONS = 560310,
    SPELL_THROUGH_THE_AEONS_BUFF = 560311,
    SPELL_AEON_RENEWAL = 806290,
    SPELL_AEON_RESILIENCE = 806291,
    SPELL_AEON_PROTECTION = 806292,
    SPELL_AEON_OBLIVION = 806293,
    SPELL_DIMENSIONAL_DIVERGENCE = 802790,
    SPELL_DIVERGENCE_SLOW = 803301,
    SPELL_DIVERGENCE_SPEED = 803703,
    SPELL_UNMAKER_OF_REALITIES = 706107,
    SPELL_HASTEN = 801304,
    SPELL_HASTEN_STRIKE_SOURCE = 803382,
    SPELL_HASTY_STRIKE = 803706,
    SPELL_TIMEGUARD = 804441,
    SPELL_MARK_OF_ORDER_ADD_STACK = 806270,
    SPELL_IDEAL_TIME_BUFF = 807210,
    SPELL_NOZDORMUS_GAZE = 807691
};

constexpr uint32 TimeguardHeavyHitPercent = 20;

bool IsAeonActivation(uint32 id)
{
    return id == SPELL_AEON_RENEWAL || id == SPELL_AEON_RESILIENCE ||
        id == SPELL_AEON_PROTECTION || id == SPELL_AEON_OBLIVION;
}

bool CanSwapPlayers(Player* player, Player* target)
{
    if (!player || !target || player == target || player->getClass() != CLASS_CHRONOMANCER ||
        !player->IsAlive() || !target->IsAlive() || !player->IsInWorld() || !target->IsInWorld() ||
        player->GetMap() != target->GetMap() || !player->InSamePhase(target))
        return false;
    if (player->IsBeingTeleported() || target->IsBeingTeleported() || player->IsInFlight() || target->IsInFlight() ||
        player->GetTransport() || target->GetTransport() || player->GetVehicle() || target->GetVehicle())
        return false;
    return player->IsWithinLOSInMap(target) &&
        (player->IsValidAttackTarget(target) || player->IsValidAssistTarget(target));
}

class spell_ascension_dimensional_divergence : public SpellScript
{
    PrepareSpellScript(spell_ascension_dimensional_divergence);

    bool Validate(SpellInfo const*) override
    {
        return ValidateSpellInfo({SPELL_DIVERGENCE_SLOW, SPELL_DIVERGENCE_SPEED});
    }

    SpellCastResult CheckSwap()
    {
        Unit* target = GetExplTargetUnit();
        return CanSwapPlayers(GetCaster()->ToPlayer(), target ? target->ToPlayer() : nullptr)
            ? SPELL_CAST_OK : SPELL_FAILED_BAD_TARGETS;
    }

    void Swap(SpellEffIndex)
    {
        Player* player = GetCaster()->ToPlayer();
        Unit* hit = GetHitUnit();
        Player* target = hit ? hit->ToPlayer() : nullptr;
        if (!CanSwapPlayers(player, target))
            return;
        Position origin = player->GetPosition();
        Position destination = target->GetPosition();
        bool hostile = player->IsValidAttackTarget(target);
        target->NearTeleportTo(origin);
        player->NearTeleportTo(destination, true);
        if (hostile)
        {
            player->CastSpell(target, SPELL_DIVERGENCE_SLOW, true);
            if (target->HasAura(SPELL_DIVERGENCE_SLOW, player->GetGUID()))
                player->CastSpell(player, SPELL_DIVERGENCE_SPEED, true);
        }
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_ascension_dimensional_divergence::CheckSwap);
        OnEffectHitTarget += SpellEffectFn(spell_ascension_dimensional_divergence::Swap, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

class spell_ascension_unmaker_of_realities : public AuraScript
{
    PrepareAuraScript(spell_ascension_unmaker_of_realities);

    bool Validate(SpellInfo const*) override
    {
        return ValidateSpellInfo({SPELL_HASTEN_STRIKE_SOURCE, SPELL_HASTY_STRIKE});
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Player* caster = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
        if (!caster || caster->getClass() != CLASS_CHRONOMANCER || !caster->HasSpell(SPELL_UNMAKER_OF_REALITIES))
            return false;
        DamageInfo* damage = eventInfo.GetDamageInfo();
        if (!damage || !damage->GetDamage() || !eventInfo.GetActionTarget())
            return false;
        SpellInfo const* procSpell = eventInfo.GetSpellInfo();
        return !procSpell || procSpell->Id != SPELL_HASTY_STRIKE;
    }

    void HandleProc(ProcEventInfo& eventInfo)
    {
        Unit* striker = GetTarget();
        Unit* victim = eventInfo.GetActionTarget();
        DamageInfo* damage = eventInfo.GetDamageInfo();
        SpellInfo const* source = sSpellMgr->GetSpellInfo(SPELL_HASTEN_STRIKE_SOURCE);
        if (!striker || !victim || !damage || !source)
            return;
        int32 percent = std::clamp(source->Effects[EFFECT_0].CalcValue(), 0, 100);
        uint64 amount = uint64(damage->GetDamage()) * uint64(percent) / 100;
        if (!amount)
            return;
        striker->CastCustomSpell(SPELL_HASTY_STRIKE, SPELLVALUE_BASE_POINT0,
            int32(std::min<uint64>(amount, uint64(std::numeric_limits<int32>::max()))), victim, true);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_ascension_unmaker_of_realities::CheckProc);
        OnProc += AuraProcFn(spell_ascension_unmaker_of_realities::HandleProc);
    }
};

class spell_ascension_timeguard : public AuraScript
{
    PrepareAuraScript(spell_ascension_timeguard);

    void Amount(AuraEffect const*, int32& amount, bool& recalculate)
    {
        amount = -1;
        recalculate = false;
    }

    void Absorb(AuraEffect*, DamageInfo& damage, uint32& amount)
    {
        amount = 0;
        Unit* target = GetTarget();
        AuraEffect const* threshold = GetEffect(EFFECT_1);
        AuraEffect const* reduction = GetEffect(EFFECT_2);
        if (!target || !threshold || !reduction)
            return;
        if (damage.GetDamageType() != DIRECT_DAMAGE && damage.GetDamageType() != SPELL_DIRECT_DAMAGE)
            return;
        uint64 incoming = damage.GetDamage();
        uint64 maxHealth = target->GetMaxHealth();
        if (!incoming || !maxHealth)
            return;
        uint64 floorHealth = maxHealth * uint64(std::clamp(threshold->GetAmount(), 0, 100)) / 100;
        bool heavy = incoming * 100 > maxHealth * uint64(TimeguardHeavyHitPercent);
        bool lethal = uint64(target->GetHealth()) < incoming + floorHealth;
        if (!heavy && !lethal)
            return;
        amount = uint32(std::min<uint64>(incoming * uint64(std::clamp(reduction->GetAmount(), 0, 100)) / 100,
            uint64(std::numeric_limits<int32>::max())));
        if (amount)
            GetAura()->DropCharge();
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_ascension_timeguard::Amount,
            EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_ascension_timeguard::Absorb, EFFECT_0);
    }
};

class chronomancer_talent_casts : public AllSpellScript
{
public:
    chronomancer_talent_casts() : AllSpellScript("chronomancer_talent_casts", {ALLSPELLHOOK_ON_CAST}) { }

    void OnSpellCast(Spell* spell, Unit* caster, SpellInfo const* info, bool) override
    {
        Player* player = caster ? caster->ToPlayer() : nullptr;
        if (!player || player->getClass() != CLASS_CHRONOMANCER || info->SpellFamilyName != 28 ||
            spell->HasTriggeredCastFlag(TRIGGERED_IGNORE_GCD) || !IsAeonActivation(info->Id))
            return;
        if (player->HasAura(SPELL_SHIMMERING_SHARD))
            player->CastSpell(player, SPELL_SHIMMER, true);
        if (player->HasAura(SPELL_THROUGH_THE_AEONS))
            player->CastSpell(player, SPELL_THROUGH_THE_AEONS_BUFF, true);
    }
};
}

void ApplyAscensionChronomancerTalentContracts(SpellInfo* info)
{
    if (info->SpellFamilyName != 28)
        return;
    if (info->Id == SPELL_ROLL_BACK)
    {
        info->Effects[EFFECT_0].Effect = SPELL_EFFECT_DISPEL;
        info->Effects[EFFECT_0].BasePoints = 0;
        info->Effects[EFFECT_0].DieSides = 1;
        info->Effects[EFFECT_0].MiscValue = DISPEL_ALL;
        info->_InitializeExplicitTargetMask();
    }
    if (info->Id == SPELL_TIME_SKIP)
    {
        info->ProcFlags = 0;
        info->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
        info->Effects[EFFECT_0].TriggerSpell = 0;
    }
    if (info->Id == SPELL_THROUGH_THE_AEONS)
    {
        info->ProcFlags = 0;
        info->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
        info->Effects[EFFECT_0].TriggerSpell = 0;
    }
    if (info->Id == SPELL_DIMENSIONAL_DIVERGENCE)
    {
        info->Effects[EFFECT_1].Effect = 0;
        info->Effects[EFFECT_2].Effect = 0;
        info->_InitializeExplicitTargetMask();
    }
    if (info->Id == SPELL_MARK_OF_ORDER_ADD_STACK)
    {
        info->Effects[EFFECT_0].MiscValue = info->Effects[EFFECT_0].MiscValueB;
    }
    if (info->Id == SPELL_NOZDORMUS_GAZE)
    {
        info->Effects[EFFECT_0].MiscValue = SPELLMOD_EFFECT2;
    }
    if (info->Id == SPELL_IDEAL_TIME_BUFF)
    {
        info->ProcCharges = 1;
    }
    if (info->Id != SPELL_SHIMMER)
        return;
    info->Effects[EFFECT_1].BasePoints = info->Effects[EFFECT_0].BasePoints;
    info->Effects[EFFECT_1].DieSides = info->Effects[EFFECT_0].DieSides;
}

void AddSC_AscensionChronomancerTalents()
{
    new chronomancer_talent_casts();
    RegisterSpellScript(spell_ascension_dimensional_divergence);
    RegisterSpellScript(spell_ascension_unmaker_of_realities);
    RegisterSpellScript(spell_ascension_timeguard);
}
