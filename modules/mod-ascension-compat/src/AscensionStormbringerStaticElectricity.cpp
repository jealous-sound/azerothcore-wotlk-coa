/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "Creature.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"

namespace
{
enum StaticElectricitySpells : uint32
{
    SPELL_STATIC_ELECTRICITY = 524954,
    SPELL_ELECTRIFIED = 573442
};

struct npc_ascension_stormbringer_electrified_water : ScriptedAI
{
    explicit npc_ascension_stormbringer_electrified_water(Creature* creature) : ScriptedAI(creature) { }

    bool dissipated = false;

    void AttackStart(Unit*) override { }
    void MoveInLineOfSight(Unit*) override { }
    void EnterEvadeMode(EvadeReason) override { }

    void IsSummonedBy(WorldObject* summoner) override
    {
        Player* owner = summoner ? summoner->ToPlayer() : nullptr;
        if (!owner || owner->getClass() != CLASS_STORMBRINGER || !owner->HasAura(SPELL_STATIC_ELECTRICITY))
        {
            me->DespawnOrUnsummon();
            return;
        }
        me->SetOwnerGUID(owner->GetGUID());
        me->SetFaction(owner->GetFaction());
        me->SetLevel(owner->GetLevel());
        me->SetReactState(REACT_PASSIVE);
        me->GetMotionMaster()->Clear();
        me->GetMotionMaster()->MoveIdle();
    }

    void OnDespawn() override
    {
        if (dissipated)
            return;

        dissipated = true;
        Player* owner = ObjectAccessor::GetPlayer(*me, me->GetOwnerGUID());
        if (owner && owner->IsInWorld() && owner->IsAlive())
            owner->CastSpell(owner, SPELL_ELECTRIFIED, true);
    }
};
}

void AddSC_AscensionStormbringerStaticElectricity()
{
    RegisterCreatureAI(npc_ascension_stormbringer_electrified_water);
}
