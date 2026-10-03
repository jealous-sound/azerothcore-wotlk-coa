/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionTamingData.h"
#include "AscensionWildcard.h"
#include "Creature.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "Map.h"
#include "ObjectMgr.h"
#include "Pet.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellScriptLoader.h"

namespace AscensionTaming
{
namespace
{
bool IsKind(uint32 creatureEntry, uint32 kind)
{
    CreatureTemplate const* creature = sObjectMgr->GetCreatureTemplate(creatureEntry);
    return creature && creature->type == kind;
}

void MoveToSlot(Player* player, uint32 petNumber, PetSaveMode slot)
{
    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);
    stmt->SetData(0, uint8(slot));
    stmt->SetData(1, player->GetGUID().GetCounter());
    stmt->SetData(2, petNumber);
    CharacterDatabase.Execute(stmt);
}

std::optional<std::size_t> FreeStableSlot(PetStable const& stable)
{
    for (std::size_t slot = 0; slot < stable.StabledPets.size(); ++slot)
        if (!stable.StabledPets[slot])
            return slot;
    return std::nullopt;
}

void Park(Player* player, PetStable& stable, PetStable::PetInfo pet)
{
    if (std::optional<std::size_t> const slot = FreeStableSlot(stable))
    {
        MoveToSlot(player, pet.PetNumber, PetSaveMode(PET_SAVE_FIRST_STABLE_SLOT + *slot));
        stable.StabledPets[*slot] = std::move(pet);
        return;
    }
    MoveToSlot(player, pet.PetNumber, PET_SAVE_NOT_IN_SLOT);
    stable.UnslottedPets.push_back(std::move(pet));
}

std::optional<PetStable::PetInfo> TakeStored(PetStable& stable, uint32 kind)
{
    for (std::optional<PetStable::PetInfo>& stabled : stable.StabledPets)
        if (stabled && IsKind(stabled->CreatureId, kind))
        {
            std::optional<PetStable::PetInfo> taken = std::move(stabled);
            stabled.reset();
            return taken;
        }
    for (auto pet = stable.UnslottedPets.begin(); pet != stable.UnslottedPets.end(); ++pet)
        if (IsKind(pet->CreatureId, kind))
        {
            PetStable::PetInfo taken = std::move(*pet);
            stable.UnslottedPets.erase(pet);
            return taken;
        }
    return std::nullopt;
}

bool Prepare(Player* player, FamilyCall const& call)
{
    PetStable& stable = player->GetOrInitPetStable();
    while (!stable.UnslottedPets.empty() && FreeStableSlot(stable))
    {
        PetStable::PetInfo pet = std::move(stable.UnslottedPets.back());
        stable.UnslottedPets.pop_back();
        Park(player, stable, std::move(pet));
    }

    if (stable.CurrentPet && IsKind(stable.CurrentPet->CreatureId, call.Kind))
        return true;

    std::optional<PetStable::PetInfo> wanted = TakeStored(stable, call.Kind);
    if (stable.CurrentPet)
    {
        PetStable::PetInfo current = std::move(*stable.CurrentPet);
        stable.CurrentPet.reset();
        Park(player, stable, std::move(current));
    }
    if (!wanted)
        return false;

    MoveToSlot(player, wanted->PetNumber, PET_SAVE_AS_CURRENT);
    stable.CurrentPet = std::move(wanted);
    return true;
}

bool GrantStarter(Player* player, FamilyCall const& call)
{
    if (!call.Starter || player->GetPetGUID())
        return false;
    Pet* pet = player->CreateTamedPetFrom(call.Starter, call.SpellId);
    if (!pet)
    {
        PetStable const* stable = player->GetPetStable();
        LOG_ERROR("coa", "Starter pet {} for {} of {} could not be created: current {} unslotted {}", call.Starter,
            call.SpellId, player->GetName(), stable && stable->CurrentPet ? stable->CurrentPet->CreatureId : 0,
            stable ? stable->UnslottedPets.size() : 0);
        return false;
    }

    uint8 const level = player->GetLevel();
    pet->SetUInt32Value(UNIT_FIELD_LEVEL, level > 1 ? level - 1 : level);
    pet->GetMap()->AddToMap(pet->ToCreature(), true);
    pet->SetUInt32Value(UNIT_FIELD_LEVEL, level);
    player->SetMinion(pet, true);
    pet->InitTalentForLevel();
    pet->SavePetToDB(PET_SAVE_AS_CURRENT);
    player->PetSpellInitialize();
    LOG_INFO("coa", "Granted {} the starter pet {} for spell {}", player->GetName(), call.Starter, call.SpellId);
    return true;
}
}

class spell_ascension_family_call : public SpellScript
{
    PrepareSpellScript(spell_ascension_family_call);

    SpellCastResult CheckCast()
    {
        Player* player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
        FamilyCall const* call = FindFamilyCall(GetSpellInfo()->Id);
        if (!player || !call || !AscensionWildcard::IsClasslessHero(player))
            return SPELL_CAST_OK;

        if (Pet* out = player->GetPet())
        {
            if (IsKind(out->GetEntry(), call->Kind))
                return SPELL_CAST_OK;
            player->RemovePet(out, PET_SAVE_AS_CURRENT);
        }

        if (Prepare(player, *call))
            return SPELL_CAST_OK;
        if (GrantStarter(player, *call))
            return SPELL_FAILED_DONT_REPORT;
        return SPELL_CAST_OK;
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_ascension_family_call::CheckCast);
    }
};

class AscensionTamingPlayer final : public PlayerScript
{
public:
    AscensionTamingPlayer() : PlayerScript("AscensionTamingPlayer", { PLAYERHOOK_ON_FORGOT_SPELL }) { }

    void OnPlayerForgotSpell(Player* player, uint32 spellId) override
    {
        FamilyCall const* call = FindFamilyCall(spellId);
        if (!call || !AscensionWildcard::IsClasslessHero(player))
            return;
        if (Pet* out = player->GetPet(); out && IsKind(out->GetEntry(), call->Kind))
            player->RemovePet(out, PET_SAVE_AS_CURRENT);
    }
};
}

void AddAscensionTamingScripts()
{
    RegisterSpellScript(AscensionTaming::spell_ascension_family_call);
    new AscensionTaming::AscensionTamingPlayer();
}
