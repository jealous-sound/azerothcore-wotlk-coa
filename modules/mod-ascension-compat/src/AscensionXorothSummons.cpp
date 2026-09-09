/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionXoroth.h"
#include "Map.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "Pet.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "TemporarySummon.h"
#include "ThreatManager.h"
#include <algorithm>
namespace AscensionXoroth
{
void Summon(Player* player, uint32 entry, Position const& position, uint32 duration)
{
    if (!player || !player->IsAlive() || !player->IsInWorld())
        return;
    if (TempSummon* unit =
            player->GetMap()->SummonCreature(entry, position, nullptr, duration + (entry == 51323 ? 500 : 0), player))
        unit->SetTempSummonType(TEMPSUMMON_TIMED_DESPAWN);
}
} // namespace AscensionXoroth
namespace
{
using namespace AscensionXoroth;
void Scale(Creature* unit, Player* player)
{
    bool imp = unit->GetEntry() == 50301;
    float health = player->GetMaxHealth() * (imp ? .25f : .6f);
    if (imp && player->HasAura(707666))
        health *= 1.3f;
    if (imp && player->HasAura(804879))
        health *= 1.5f;
    if (Aura* grit = player->GetAuraOfRankedSpell(805678))
        health *= 1 + Amount(grit->GetId()) / 100.0f;
    uint32 maximum = std::max(1u, uint32(health));
    float percent = unit->GetHealthPct() / 100.0f;
    unit->SetLevel(player->GetLevel());
    unit->SetMaxHealth(maximum);
    unit->SetHealth(std::max(1u, uint32(maximum * percent)));
    unit->SetArmor(player->GetArmor());
    float damage = player->GetLevel() * 1.5f + player->GetTotalAttackPowerValue(BASE_ATTACK) * .10f;
    unit->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, damage * .8f);
    unit->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, damage * 1.2f);
    unit->UpdateDamagePhysical(BASE_ATTACK);
}
struct npc_ascension_xoroth_summon : public ScriptedAI
{
    explicit npc_ascension_xoroth_summon(Creature* creature) : ScriptedAI(creature) {}
    ObjectGuid owner;
    EventMap events;
    uint32 remaining = 0;
    void IsSummonedBy(WorldObject* summoner) override
    {
        Player* player = Owner(summoner ? summoner->ToUnit() : nullptr);
        if (!player)
        {
            me->DespawnOrUnsummon();
            return;
        }
        owner = player->GetGUID();
        me->SetOwnerGUID(owner);
        me->SetFaction(player->GetFaction());
        if (me->GetEntry() == 51323 || me->GetEntry() == 50268)
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetUnitFlag(UNIT_FLAG_NON_ATTACKABLE);
            me->GetMotionMaster()->MoveIdle();
            if (me->GetEntry() == 50268)
                me->SetNpcFlag(UNIT_NPC_FLAG_GOSSIP);
            else
                remaining = uint32(sSpellMgr->GetSpellInfo(706756)->GetDuration());
        }
        else
        {
            Scale(me, player);
            me->SetFullHealth();
            me->SetReactState(REACT_DEFENSIVE);
            if (me->GetEntry() == 50301)
            {
                State(player).imps.push_back(me->GetGUID());
                me->GetThreatMgr().RegisterRedirectThreat(706571, owner, 100);
                Cast(player, me, 800443);
            }
            else
                Cast(me, me, 302580);
        }
        events.ScheduleEvent(1, 1s);
        Refresh(player);
    }
    void sGossipHello(Player* player) override
    {
        Player* summoner = ObjectAccessor::GetPlayer(*me, owner);
        if (me->GetEntry() == 50268 && summoner && player->IsAlive() && !player->IsInCombat() &&
            player->IsWithinDistInMap(me, 5) && (player == summoner || summoner->IsInRaidWith(player)))
            Cast(player, player, 804775);
    }
    void UpdateAI(uint32 diff) override
    {
        Player* player = ObjectAccessor::GetPlayer(*me, owner);
        if (!player || !player->IsAlive() || !me->IsWithinDistInMap(player, 100))
        {
            me->DespawnOrUnsummon();
            return;
        }
        if (me->GetEntry() == 51323 && remaining)
        {
            if (diff >= remaining)
            {
                remaining = 0;
                for (Unit* target : Nearby(me, 10))
                    if (player->IsValidAttackTarget(target) &&
                        !target->IsImmunedToSpell(sSpellMgr->GetSpellInfo(803185)))
                        target->GetMotionMaster()->MoveJump(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(),
                                                            20, 10);
            }
            else
                remaining -= diff;
        }
        events.Update(diff);
        if (events.ExecuteEvent())
        {
            if (me->GetEntry() == 50301 || me->GetEntry() == 50375)
                Scale(me, player);
            if (me->GetEntry() == 50301 && me->GetVictim())
                Cast(me, me->GetVictim(), 800444);
            events.ScheduleEvent(1, 2s);
        }
        if (me->GetEntry() == 51323 || me->GetEntry() == 50268)
            return;
        if (Unit* target = me->GetVictim(); target && !player->IsValidAttackTarget(target))
            me->AttackStop();
        if (!me->GetVictim())
        {
            Unit* target = player->GetVictim();
            if (target && player->IsValidAttackTarget(target))
                AttackStart(target);
            else if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() != FOLLOW_MOTION_TYPE)
                me->GetMotionMaster()->MoveFollow(player, 2, 0);
        }
        if (UpdateVictim() && me->GetEntry() == 50375)
            DoMeleeAttackIfReady();
    }
};
} // namespace
void AddSC_AscensionXorothSummons()
{
    RegisterCreatureAI(npc_ascension_xoroth_summon);
}
