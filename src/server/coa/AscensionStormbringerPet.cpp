/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "Pet.h"
#include "Player.h"
#include "Random.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"

namespace
{
enum AirElementalSpells : uint32
{
    SPELL_SUMMON_AIR_ELEMENTAL = 804019,
    SPELL_AIR_ELEMENTAL_PASSIVE = 806010,
    SPELL_INVIGORATION_PROC = 806020,
    SPELL_GENERATE_INVIGORATION = 500348,
    SPELL_INVIGORATION = 680918,
    SPELL_AURAT_SCRIPTS = 712431,
    SPELL_AURAT_SCRIPTS_PROC = 712488,
    SPELL_AURAT_GALE = 500019,
    SPELL_FLURRY_READY = 807465,
    SPELL_FLURRY_DOT = 807555,
    SPELL_FLURRY_DEBUFF = 807464
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

    bool Validate(SpellInfo const*) override
    {
        return ValidateSpellInfo({SPELL_GENERATE_INVIGORATION, SPELL_AURAT_SCRIPTS_PROC, SPELL_AURAT_GALE,
            SPELL_FLURRY_READY, SPELL_FLURRY_DOT});
    }

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

    void Invigorate(AuraEffect const*, ProcEventInfo& event)
    {
        PreventDefaultAction();
        GetTarget()->CastSpell(GetTarget(), SPELL_GENERATE_INVIGORATION, true);
        Player* owner = AirElementalOwner(GetTarget());
        if (owner && GetTarget()->HasAura(SPELL_FLURRY_READY, GetTarget()->GetGUID()))
        {
            GetTarget()->RemoveAurasDueToSpell(SPELL_FLURRY_READY, GetTarget()->GetGUID());
            int32 amount = sSpellMgr->GetSpellInfo(SPELL_FLURRY_DOT)->Effects[EFFECT_0].CalcValue(owner) +
                int32(owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_NATURE) * 0.2f);
            GetTarget()->CastCustomSpell(SPELL_FLURRY_DOT, SPELLVALUE_BASE_POINT0, amount,
                event.GetActionTarget(), true);
        }
        if (owner && owner->HasActiveSpell(SPELL_AURAT_SCRIPTS) &&
            roll_chance_i(sSpellMgr->GetSpellInfo(SPELL_AURAT_SCRIPTS_PROC)->ProcChance))
        {
            owner->CastSpell(owner, SPELL_AURAT_GALE, true);
            if (Aura* empowerment = owner->GetAura(SPELL_AURAT_GALE, owner->GetGUID()))
                empowerment->SetCharges(1);
        }
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

class stormbringer_pet_contracts : public GlobalScript
{
public:
    stormbringer_pet_contracts() : GlobalScript("stormbringer_pet_contracts",
        {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* info) override
    {
        if (info && info->Id == SPELL_FLURRY_DEBUFF && info->SpellFamilyName == 22 &&
            info->Effects[EFFECT_1].ApplyAuraName == SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN)
            info->Effects[EFFECT_1].BasePoints = info->Effects[EFFECT_1].CalcBaseValue(2);
        if (info && info->Id == SPELL_FLURRY_DOT && info->SpellFamilyName == 22)
        {
            info->Effects[EFFECT_0].BonusMultiplier = 0.0f;
        }
    }
};
}

void AddSC_AscensionStormbringerPet()
{
    new stormbringer_pet_lifecycle();
    new stormbringer_pet_contracts();
    RegisterSpellScript(aura_ascension_air_invigoration);
    RegisterSpellScript(spell_ascension_air_invigoration_duration);
}
