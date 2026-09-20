/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
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
enum TimeSpells : uint32
{
    Epoch = 801270,
    Recovery = 800857,
    RenewalAeon = 806290,
    ResilienceAeon = 806291,
    ProtectionAeon = 806292,
    OblivionAeon = 806293,
    Renewal = 560355,
    Protection = 560374,
    Oblivion = 583921,
    ResilienceSpread = 560373,
    Sands = 804488,
    EndlessSandsTalent = 806338,
    EndlessSands = 806728,
    KeepAccelerating = 520042,
    KeepAcceleratingSpread = 520118,
    CadenceTalent = 560397,
    Cadence = 560398,
    OrderlyTalent = 806162,
    Orderly = 704482,
    TimelineTether = 804505,
    TetherCooldown = 804506,
    Fortify = 804491,
    Chronicler = 706134,
    EpicRecovery = 706098,
    EpicRecoveryTargets = 542725,
    Accelerate = 572633,
    Decelerate = 572632,
    TimeOut = 802229,
    TimeOutRankTwo = 803896,
    TimeOutRankThree = 803897,
    TimeOutStasis = 802228,
    ExpeditingTime = 706055
};

Player* Chronomancer(Unit* caster)
{
    Player* player = caster ? caster->ToPlayer() : nullptr;
    return player && player->getClass() == CLASS_CHRONOMANCER ? player : nullptr;
}

bool IsRank(uint32 spell, uint32 root)
{
    return sSpellMgr->GetFirstSpellInChain(spell) == root;
}

int32 Amount(uint32 spell, uint8 index = EFFECT_0)
{
    SpellInfo const* info = sSpellMgr->GetSpellInfo(spell);
    return info ? info->Effects[index].CalcValue() : 0;
}

std::list<Unit*> Nearby(Player* player, Unit* center, uint32 helper, bool friendly)
{
    std::list<Unit*> targets;
    SpellInfo const* info = sSpellMgr->GetSpellInfo(helper);
    if (!info || !center || !center->IsInWorld())
        return targets;
    float radius = info->Effects[EFFECT_0].CalcRadius(player);
    Acore::AnyUnitInObjectRangeCheck check(center, radius);
    Acore::UnitListSearcher<Acore::AnyUnitInObjectRangeCheck> search(center, targets, check);
    Cell::VisitObjects(center, search, radius);
    targets.remove_if([player, center, friendly](Unit* target)
    {
        return target == center || !target->IsAlive() || !target->InSamePhase(player) ||
            !center->IsWithinLOSInMap(target) || !player->IsWithinLOSInMap(target) ||
            !(friendly ? player->IsValidAssistTarget(target) : player->IsValidAttackTarget(target));
    });
    targets.sort([center](Unit* first, Unit* second)
    {
        float left = center->GetDistance(first), right = center->GetDistance(second);
        return left == right ? first->GetGUID() < second->GetGUID() : left < right;
    });
    return targets;
}

void SpreadRecovery(Player* player, Unit* source, uint32 helper)
{
    Aura* original = source->GetAuraOfRankedSpell(Recovery, player->GetGUID());
    if (!original)
        return;
    uint32 spell = original->GetId();
    int32 remaining = original->GetDuration();
    for (Unit* target : Nearby(player, source, helper, true))
    {
        if (target->GetAuraOfRankedSpell(Recovery, player->GetGUID()))
            continue;
        if (player->CastSpell(target, spell, true) != SPELL_CAST_OK)
            continue;
        if (Aura* copy = target->GetAura(spell, player->GetGUID()))
            copy->SetDuration(std::min(copy->GetDuration(), remaining));
        break;
    }
}

void ExtendRecovery(Player* player, Unit* target)
{
    Aura* recovery = target->GetAuraOfRankedSpell(Recovery, player->GetGUID());
    if (!recovery)
        return;
    int32 extension = Amount(Fortify, EFFECT_1);
    int32 limit = Amount(Fortify, EFFECT_2);
    if (player->HasAura(Chronicler))
    {
        extension += Amount(Chronicler, EFFECT_0);
        limit += Amount(Chronicler, EFFECT_1);
    }
    int32 used = int32(std::min<uint64>(recovery->GetScriptValue(Fortify), INT32_MAX));
    extension = std::clamp(extension, 0, std::max(0, limit - used));
    if (extension)
    {
        recovery->SetScriptValue(Fortify, used + extension);
        recovery->SetMaxDuration(recovery->GetMaxDuration() + extension);
        recovery->SetDuration(recovery->GetDuration() + extension);
    }
}

void ApplyEpochAeon(Player* player, Unit* target, uint32 healing)
{
    if (player->HasAura(ResilienceAeon))
        SpreadRecovery(player, target, ResilienceSpread);
    if (!healing)
        return;
    if (player->HasAura(RenewalAeon))
    {
        SpellInfo const* info = sSpellMgr->GetSpellInfo(Renewal);
        if (!info)
            return;
        int32 duration = player->CalcSpellDuration(info);
        player->ApplySpellMod(Renewal, SPELLMOD_DURATION, duration);
        uint32 ticks = std::max(1, duration / int32(std::max(1u, info->Effects[EFFECT_0].Amplitude)));
        int32 tick = int32(std::min<uint64>(uint64(healing) * 75 / 100 / ticks, INT32_MAX));
        player->CastCustomSpell(Renewal, SPELLVALUE_BASE_POINT0, tick, target, true);
    }
    else if (player->HasAura(ProtectionAeon))
        player->CastCustomSpell(Protection, SPELLVALUE_BASE_POINT0,
            int32(std::min<uint64>(uint64(healing) * Amount(ProtectionAeon) / 100, INT32_MAX)), target, true);
    else if (player->HasAura(OblivionAeon))
    {
        auto enemies = Nearby(player, target, Oblivion, false);
        if (!enemies.empty())
            player->CastCustomSpell(Oblivion, SPELLVALUE_BASE_POINT0,
                int32(std::min<uint32>(healing, INT32_MAX)), enemies.front(), true);
    }
}

void CastEpicRecovery(Player* player, Unit* primary)
{
    if (!player->HasAura(EpicRecovery))
        return;
    SpellInfo const* highest = nullptr;
    for (auto const& [id, state] : player->GetSpellMap())
        if (player->HasSpell(id) && IsRank(id, Epoch))
            if (SpellInfo const* info = sSpellMgr->GetSpellInfo(id))
                if (!highest || info->GetRank() > highest->GetRank())
                    highest = info;
    SpellInfo const* helper = sSpellMgr->GetSpellInfo(EpicRecoveryTargets);
    if (!highest || !helper)
        return;
    uint32 count = 0;
    for (Unit* target : Nearby(player, primary, EpicRecoveryTargets, true))
    {
        if (!target->GetAuraOfRankedSpell(Recovery, player->GetGUID()))
            continue;
        SpellCastTargets targets;
        targets.SetUnitTarget(target);
        Spell* echo = new Spell(player, highest, TRIGGERED_FULL_MASK);
        echo->SetScriptValue(EpicRecovery, Amount(EpicRecovery));
        if (echo->prepare(&targets) == SPELL_CAST_OK && ++count >= helper->MaxAffectedTargets)
            break;
    }
}

class chronomancer_time_casts : public AllSpellScript
{
public:
    chronomancer_time_casts() : AllSpellScript("chronomancer_time_casts",
        {ALLSPELLHOOK_ON_CAST, ALLSPELLHOOK_ON_HIT_RESULT, ALLSPELLHOOK_ON_CALCULATED_TARGET}) { }

    void OnSpellCalculatedTarget(Spell* spell, Unit*, TargetInfo& target) override
    {
        if (uint64 percent = spell->GetScriptValue(EpicRecovery); percent &&
            Chronomancer(spell->GetCaster()) && IsRank(spell->GetSpellInfo()->Id, Epoch))
        {
            target.damage = int32(int64(target.damage) * int64(std::min<uint64>(percent, 100)) / 100);
            target.damageBeforeTakenMods =
                int32(int64(target.damageBeforeTakenMods) * int64(std::min<uint64>(percent, 100)) / 100);
        }
    }

    void OnSpellCast(Spell* spell, Unit* caster, SpellInfo const* info, bool) override
    {
        Player* player = Chronomancer(caster);
        if (!player || spell->IsTriggered())
            return;
        for (uint32 aeon : {RenewalAeon, ResilienceAeon, ProtectionAeon, OblivionAeon})
            if (info->Id == aeon)
                for (uint32 other : {RenewalAeon, ResilienceAeon, ProtectionAeon, OblivionAeon})
                    if (other != aeon)
                        player->RemoveAurasDueToSpell(other);
        if (!IsRank(info->Id, Epoch))
            return;
        Aura* sands = player->GetAura(Sands);
        if (sands && sands->GetStackAmount() >= 5)
            sands->Remove();
        else
            player->CastSpell(player, Sands, true);
        if (player->HasAura(EndlessSandsTalent))
            player->CastSpell(player, EndlessSands, true);
        if (Unit* target = spell->m_targets.GetUnitTarget())
            CastEpicRecovery(player, target);
    }

    void OnSpellHitResult(Spell* spell, Unit* target, uint8 miss, uint32, uint32 healing, bool critical) override
    {
        Player* player = Chronomancer(spell->GetCaster());
        if (!player || !target || miss != SPELL_MISS_NONE)
            return;
        uint32 id = spell->GetSpellInfo()->Id;
        if (IsRank(id, Recovery))
        {
            if (Aura* aura = target->GetAuraOfRankedSpell(Recovery, player->GetGUID()))
                aura->SetScriptValue(Fortify, 0);
            if (!spell->IsTriggered() && player->HasAura(KeepAccelerating))
                SpreadRecovery(player, target, KeepAcceleratingSpread);
        }
        else if (IsRank(id, Fortify))
            ExtendRecovery(player, target);
        else if (IsRank(id, Epoch))
        {
            ApplyEpochAeon(player, target, healing);
            if (critical && player->HasAura(CadenceTalent))
                player->CastSpell(player, Cadence, true);
            if (healing && player->HasAura(OrderlyTalent))
                player->CastSpell(player, Orderly, true);
        }
        else if (IsRank(id, Accelerate) || IsRank(id, Decelerate))
        {
            uint32 opposite = IsRank(id, Accelerate) ? Decelerate : Accelerate;
            while (Aura* aura = target->GetAuraOfRankedSpell(opposite))
                aura->Remove();
        }
    }
};

class aura_ascension_timeline_tether : public AuraScript
{
    PrepareAuraScript(aura_ascension_timeline_tether);

    bool CheckProc(ProcEventInfo& event)
    {
        return Chronomancer(GetTarget()) && event.GetActor() == GetTarget() &&
            (event.GetTypeMask() & PROC_FLAG_DONE_PERIODIC) &&
            ((event.GetHealInfo() && event.GetHealInfo()->GetHeal()) ||
             (event.GetDamageInfo() && event.GetDamageInfo()->GetDamage()));
    }

    void ReduceCooldown(AuraEffect const*, ProcEventInfo&)
    {
        PreventDefaultAction();
        Player* player = GetTarget()->ToPlayer();
        // The copied helper names only rank one. Apply its delta to every learned Fortify rank.
        for (auto const& [id, state] : player->GetSpellMap())
            if (player->HasSpell(id) && IsRank(id, Fortify))
                player->ModifySpellCooldown(id, Amount(TetherCooldown));
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_timeline_tether::CheckProc);
        OnEffectProc += AuraEffectProcFn(aura_ascension_timeline_tether::ReduceCooldown,
            EFFECT_0, SPELL_AURA_DUMMY);
    }
};

void ApplyTimeContracts(SpellInfo* info)
{
    if (!info || info->SpellFamilyName != 28)
        return;
    if (IsRank(info->Id, Fortify))
        info->Effects[EFFECT_1].Effect = 0;
    if (info->Id == Sands)
        info->StackAmount = 5;
    if (info->Id == TimeOut || info->Id == TimeOutRankTwo || info->Id == TimeOutRankThree)
        info->Effects[EFFECT_1].ApplyAuraName = SPELL_AURA_OBS_MOD_POWER;
    if (info->Id == ExpeditingTime)
    {
        // "Reduces the channel time of your Time Out! by 40%". The time the player is actually held
        // is the stasis 802228, which the ranks' own tooltip cites as $802228d and which carries the
        // stun; the ranks only carry the mana regeneration. The talent's class mask (32768, 0, 0)
        // reaches the ranks but not the stasis, whose family flags are (0, 2, 0), so today the talent
        // shortens the regeneration while leaving the stun at its full six seconds. Widen the talent's
        // own mask rather than the stasis' flags: 802228 is the only family-28 record carrying that
        // bit, so no other modifier inherits it. Both effects are widened together, which keeps the
        // periodic interval scaling with the duration. The bit is read from the stasis' own record so
        // the two stay in step if a client update moves it.
        if (SpellInfo const* stasis = sSpellMgr->GetSpellInfo(TimeOutStasis))
        {
            info->Effects[EFFECT_0].SpellClassMask |= stasis->SpellFamilyFlags;
            info->Effects[EFFECT_1].SpellClassMask |= stasis->SpellFamilyFlags;
        }
    }
    for (uint32 id : {RenewalAeon, ResilienceAeon, ProtectionAeon, KeepAccelerating, CadenceTalent,
        OrderlyTalent, EndlessSandsTalent, EpicRecovery, TimelineTether})
        if (info->Id == id)
        {
            info->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
            info->Effects[EFFECT_0].TriggerSpell = 0;
        }
    if (info->Id == OblivionAeon)
    {
        info->Effects[EFFECT_2].ApplyAuraName = SPELL_AURA_DUMMY;
        info->Effects[EFFECT_2].TriggerSpell = 0;
    }
    if (info->Id == Renewal || info->Id == Protection || info->Id == Oblivion)
    {
        // These copy an already modified Epoch heal. The periodic heal cannot critically hit again.
        info->AttributesEx2 |= SPELL_ATTR2_CANT_CRIT;
        info->AttributesEx3 |= SPELL_ATTR3_IGNORE_CASTER_MODIFIERS;
        info->AttributesEx4 |= SPELL_ATTR4_IGNORE_DAMAGE_TAKEN_MODIFIERS;
        info->AttributesEx6 |= SPELL_ATTR6_IGNORE_HEALTH_MODIFIERS;
        info->Effects[EFFECT_0].BonusMultiplier = 0.0f;
    }
}

class chronomancer_time_contracts : public GlobalScript
{
public:
    chronomancer_time_contracts() : GlobalScript("chronomancer_time_contracts",
        {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* info) override
    {
        ApplyTimeContracts(info);
    }
};
}

void AddSC_AscensionChronomancerTime()
{
    new chronomancer_time_casts();
    new chronomancer_time_contracts();
    RegisterSpellScript(aura_ascension_timeline_tether);
}
