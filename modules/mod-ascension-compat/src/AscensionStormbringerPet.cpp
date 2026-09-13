/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "Pet.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellScript.h"

namespace
{
enum AirElementalSpells : uint32
{
    SPELL_SUMMON_AIR_ELEMENTAL = 804019,
    SPELL_AIR_ELEMENTAL_PASSIVE = 806010,
    SPELL_INVIGORATION_PROC = 806020,
    SPELL_GENERATE_INVIGORATION = 500348,
    SPELL_INVIGORATION = 680918
};

enum AirElementalEntries : uint32
{
    NPC_AIR_ELEMENTAL = 500941
};

Player* AirElementalOwner(Unit* unit)
{
    Pet* pet = unit ? unit->ToPet() : nullptr;
    if (!pet || pet->GetEntry() != NPC_AIR_ELEMENTAL)
        return nullptr;
    Player* owner = pet->GetOwner();
    return owner && owner->getClass() == CLASS_STORMBRINGER && owner->GetPet() == pet &&
        owner->HasActiveSpell(SPELL_SUMMON_AIR_ELEMENTAL) ? owner : nullptr;
}

class stormbringer_pet_lifecycle : public PlayerScript
{
public:
    stormbringer_pet_lifecycle() : PlayerScript("stormbringer_pet_lifecycle",
        {PLAYERHOOK_ON_BEFORE_GUARDIAN_INIT_STATS_FOR_LEVEL, PLAYERHOOK_ON_UPDATE}) { }

    void OnPlayerBeforeGuardianInitStatsForLevel(Player* player, Guardian* guardian, CreatureTemplate const*,
        PetType& type) override
    {
        if (player->getClass() == CLASS_STORMBRINGER && guardian && guardian->GetEntry() == NPC_AIR_ELEMENTAL)
            type = SUMMON_PET;
    }

    void OnPlayerUpdate(Player* player, uint32) override
    {
        if (player->getClass() != CLASS_STORMBRINGER)
            return;
        Pet* pet = player->GetPet();
        if (!pet || pet->GetEntry() != NPC_AIR_ELEMENTAL || pet->isBeingLoaded())
            return;
        if (!player->HasActiveSpell(SPELL_SUMMON_AIR_ELEMENTAL))
        {
            player->RemovePet(pet, PET_SAVE_NOT_IN_SLOT);
            return;
        }
        // Pet loading is asynchronous. Apply the captured owner-to-pet passive only
        // after native spell/aura loading, including restoration after a resummon.
        if (player->IsAlive() && pet->IsAlive() && player->IsInWorld() && pet->IsInWorld() &&
            player->GetMap() == pet->GetMap() && player->InSamePhase(pet) &&
            (!pet->HasAura(SPELL_AIR_ELEMENTAL_PASSIVE, player->GetGUID()) ||
                !pet->HasAura(SPELL_INVIGORATION_PROC, pet->GetGUID())))
            player->CastSpell(pet, SPELL_AIR_ELEMENTAL_PASSIVE, true);
    }
};

class aura_ascension_air_invigoration : public AuraScript
{
    PrepareAuraScript(aura_ascension_air_invigoration);

    bool Validate(SpellInfo const*) override { return ValidateSpellInfo({SPELL_GENERATE_INVIGORATION}); }

    bool CheckProc(ProcEventInfo& event)
    {
        Unit* pet = GetTarget();
        Player* owner = AirElementalOwner(pet);
        Unit* victim = event.GetActionTarget();
        return owner && owner->IsAlive() && owner->IsInWorld() && pet->IsAlive() && pet->IsInWorld() &&
            owner->GetMap() == pet->GetMap() && owner->InSamePhase(pet) && event.GetActor() == pet &&
            victim && victim != pet && !pet->IsFriendlyTo(victim) &&
            event.GetDamageInfo() && event.GetDamageInfo()->GetDamage();
    }

    void Invigorate(AuraEffect const*, ProcEventInfo&)
    {
        PreventDefaultAction();
        GetTarget()->CastSpell(GetTarget(), SPELL_GENERATE_INVIGORATION, true);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_air_invigoration::CheckProc);
        OnEffectProc += AuraEffectProcFn(aura_ascension_air_invigoration::Invigorate,
            EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class spell_ascension_air_invigoration_duration : public SpellScript
{
    PrepareSpellScript(spell_ascension_air_invigoration_duration);
    int32 remaining = 0;

    bool Load() override { return AirElementalOwner(GetCaster()) != nullptr; }

    void SnapshotDuration()
    {
        if (Aura* aura = GetCaster()->GetAura(SPELL_INVIGORATION, GetCaster()->GetGUID()))
            remaining = aura->GetDuration();
    }

    void RestoreDuration()
    {
        // ModStackAmount refreshes native timers, including at the ten-stack cap.
        // Preserve this aura's original expiry for every source of additional stacks.
        if (remaining > 0)
            if (Aura* aura = GetCaster()->GetAura(SPELL_INVIGORATION, GetCaster()->GetGUID()))
                aura->SetDuration(remaining);
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_ascension_air_invigoration_duration::SnapshotDuration);
        AfterCast += SpellCastFn(spell_ascension_air_invigoration_duration::RestoreDuration);
    }
};
}

void AddSC_AscensionStormbringerPet()
{
    new stormbringer_pet_lifecycle();
    RegisterSpellScript(aura_ascension_air_invigoration);
    RegisterSpellScript(spell_ascension_air_invigoration_duration);
}
