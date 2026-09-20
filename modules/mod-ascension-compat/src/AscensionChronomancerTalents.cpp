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
    SPELL_SHIMMERING_SHARD = 806302,
    SPELL_SHIMMER = 806303,
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
    SPELL_TIMEGUARD = 804441
};

// Timeguard's ">20% of their total health" clause lives only in the record's
// description text; no Spell.dbc field carries it.
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
        // Snapshot both live positions before either native teleport changes one.
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

// Unmaker of Realities (706107) is a bare SPELL_AURA_DUMMY. The extra strike it promises is described by
// the unobtainable Hasten variant 803382 (ProcChance 25, Effect[0] CalcValue 30) whose aura 354 has no
// handler here, so the mechanic is attached to the Hasten players actually learn, 801304. The chance
// lives in 801304's `spell_proc` row; this script owns the gate and the forwarded amount.
class spell_ascension_unmaker_of_realities : public AuraScript
{
    PrepareAuraScript(spell_ascension_unmaker_of_realities);

    bool Validate(SpellInfo const*) override
    {
        return ValidateSpellInfo({SPELL_HASTEN_STRIKE_SOURCE, SPELL_HASTY_STRIKE});
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        // The talent belongs to the Chronomancer who cast Hasten, not to the ally carrying it.
        Player* caster = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
        if (!caster || caster->getClass() != CLASS_CHRONOMANCER || !caster->HasSpell(SPELL_UNMAKER_OF_REALITIES))
            return false;
        DamageInfo* damage = eventInfo.GetDamageInfo();
        if (!damage || !damage->GetDamage() || !eventInfo.GetActionTarget())
            return false;
        // The extra strike is itself a damaging cast by the buff holder: never let it feed its own proc.
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
        // Hasty Strike forwards the amount unchanged: it cannot crit, ignores caster modifiers and
        // ignores damage-taken modifiers, and its DieSides of 1 cancels SetSpellValue's subtraction.
        striker->CastCustomSpell(SPELL_HASTY_STRIKE, SPELLVALUE_BASE_POINT0,
            int32(std::min<uint64>(amount, uint64(std::numeric_limits<int32>::max()))), victim, true);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_ascension_unmaker_of_realities::CheckProc);
        OnProc += AuraProcFn(spell_ascension_unmaker_of_realities::HandleProc);
    }
};

// Timeguard (804441) ships a two-point absorb placeholder plus two SPELL_AURA_DUMMY effects carrying the
// threshold (effect 1, CalcValue 35) and the reduction (effect 2, CalcValue 50) its description states.
// Spend one of the record's three ProcCharges per qualifying instance and nothing on the rest.
class spell_ascension_timeguard : public AuraScript
{
    PrepareAuraScript(spell_ascension_timeguard);

    void Amount(AuraEffect const*, int32& amount, bool& recalculate)
    {
        // -1 is the core's unbounded sentinel: Unit::CalcAbsorbResist leaves the effect to the script
        // and never drains it, so the shield lasts for the charges rather than for two points.
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
        // Either clause of the description admits the instance: more than a fifth of the target's total
        // health, or enough to leave it under the effect-1 percent.
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
        if (player && player->getClass() == CLASS_CHRONOMANCER && info->SpellFamilyName == 28 &&
            !spell->IsTriggered() && IsAeonActivation(info->Id) && player->HasAura(SPELL_SHIMMERING_SHARD))
            player->CastSpell(player, SPELL_SHIMMER, true);
    }
};
}

void ApplyAscensionChronomancerTalentContracts(SpellInfo* info)
{
    if (info->SpellFamilyName != 28)
        return;
    if (info->Id == SPELL_DIMENSIONAL_DIVERGENCE)
    {
        // The copied client-destination and caster-front teleports do not swap
        // players. The effect-0 script uses authoritative live server positions.
        info->Effects[EFFECT_1].Effect = 0;
        info->Effects[EFFECT_2].Effect = 0;
        info->_InitializeExplicitTargetMask();
    }
    if (info->Id != SPELL_SHIMMER)
        return;
    // The talent promises the same percentage for damage and healing. Retain
    // the native stack cap and duration, and match healing to the displayed amount.
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
