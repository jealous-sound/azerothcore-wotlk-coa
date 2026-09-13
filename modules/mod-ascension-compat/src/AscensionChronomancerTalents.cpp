/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionChronomancerTalents.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellScript.h"

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
    SPELL_DIVERGENCE_SPEED = 803703
};

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
}
