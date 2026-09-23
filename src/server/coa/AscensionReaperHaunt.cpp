


#include "Creature.h"
#include "Random.h"
#include "CreatureAI.h"
#include "MotionMaster.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"
#include <cmath>

namespace
{
constexpr uint32 NPC_REAPER_HAUNT_VISAGE = 990020;

constexpr uint32 SPELL_CLONE_CASTER = 45204;

constexpr float FleeDistance = 40.0f;

class npc_ascension_reaper_haunt : public CreatureScript
{
public:
    npc_ascension_reaper_haunt() : CreatureScript("npc_ascension_reaper_haunt") { }

    struct npc_ascension_reaper_hauntAI : public CreatureAI
    {
        npc_ascension_reaper_hauntAI(Creature* creature) : CreatureAI(creature) { }

        void IsSummonedBy(WorldObject* summoner) override
        {
            Player* player = summoner ? summoner->ToPlayer() : nullptr;
            if (!player)
                return;

            me->SetDisplayId(player->GetDisplayId());
            player->CastSpell(me, SPELL_CLONE_CASTER, true);
            me->SetFaction(player->GetFaction());
            me->SetLevel(player->GetLevel());
            me->SetMaxHealth(std::max(5u, uint32(player->GetLevel()) * 10));
            me->SetHealth(me->GetMaxHealth());

            me->SetOwnerGUID(player->GetGUID());
            me->SetCreatorGUID(player->GetGUID());
            me->SetReactState(REACT_PASSIVE);
            me->SetCombatMovement(false);

            float const angle = frand(0.0f, 2.0f * float(M_PI));
            float x = player->GetPositionX() + FleeDistance * std::cos(angle);
            float y = player->GetPositionY() + FleeDistance * std::sin(angle);
            float z = player->GetPositionZ();
            me->UpdateAllowedPositionZ(x, y, z);
            me->GetMotionMaster()->MovePoint(0, x, y, z);
        }

        void AttackStart(Unit*) override { }
        void EnterEvadeMode(EvadeReason) override { }
        void UpdateAI(uint32) override { }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ascension_reaper_hauntAI(creature);
    }
};
}

void AddSC_AscensionReaperHaunt()
{
    new npc_ascension_reaper_haunt();
}
