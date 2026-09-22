/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "Creature.h"
#include "ObjectAccessor.h"
#include "PetAI.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"

namespace
{
enum ElectrifiedWatersSpells : uint32
{
    SPELL_ELECTRIFIED_WATERS = 573436,
    SPELL_ELECTRIFIED_REFUND = 573442
};

enum ElectrifiedWatersEntries : uint32
{
    NPC_ELECTRIFIED_WATER_ELEMENTAL = 310603
};

constexpr uint32 DrownRanks[] = {806407, 572760, 572761, 572762, 572763, 572764};

bool DrowningByCaster(Unit* victim, ObjectGuid caster)
{
    for (uint32 rank : DrownRanks)
        if (victim->GetAura(rank, caster))
            return true;
    return false;
}

class aura_ascension_electrified_waters : public AuraScript
{
    PrepareAuraScript(aura_ascension_electrified_waters);

    bool Validate(SpellInfo const*) override
    {
        return ValidateSpellInfo({SPELL_ELECTRIFIED_REFUND});
    }

    bool CheckProc(ProcEventInfo& event)
    {
        DamageInfo* damage = event.GetDamageInfo();
        Unit* victim = event.GetActionTarget();
        return (event.GetHitMask() & PROC_HIT_CRITICAL) && damage && damage->GetDamage() &&
            damage->GetDamageType() != DOT && victim && victim != GetTarget() &&
            DrowningByCaster(victim, GetTarget()->GetGUID());
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_electrified_waters::CheckProc);
    }
};

struct npc_ascension_electrified_water_elemental : PetAI
{
    explicit npc_ascension_electrified_water_elemental(Creature* creature) : PetAI(creature) { }

    void OnDespawn() override
    {
        if (me->GetEntry() != NPC_ELECTRIFIED_WATER_ELEMENTAL)
            return;
        Player* owner = ObjectAccessor::GetPlayer(*me, me->GetOwnerGUID());
        if (owner && owner->IsInWorld() && owner->getClass() == CLASS_STORMBRINGER &&
            owner->HasAura(SPELL_ELECTRIFIED_WATERS))
            owner->CastSpell(owner, SPELL_ELECTRIFIED_REFUND, true);
    }
};
}

void AddSC_AscensionStormbringerDrown()
{
    RegisterSpellScript(aura_ascension_electrified_waters);
    RegisterCreatureAI(npc_ascension_electrified_water_elemental);
}
