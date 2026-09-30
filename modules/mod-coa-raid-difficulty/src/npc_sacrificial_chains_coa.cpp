/*
 * Sacrificial Chains (92030), Majordomo Executus's periodic hostage add.
 *
 * Summoner (re-queried this session, mc-dataset.json fights by pull id): 92030 appears in 48
 * of the corpus's 49 Majordomo Executus fights (Heroic 6, Mythic 8, Ascended 34 pulls; the one
 * exception is a fight the dataset labelled "Flamewaker Elite" instead, same encounter). It
 * never appears on a Normal pull, but the corpus holds zero Normal Majordomo pulls at all
 * (0/0) - this is Majordomo's own Heroic/Mythic/Ascended-only sample, not the "seen only on
 * Mythic/Ascended with Normal/Heroic pulls available" shape the restoration treats as a real
 * difficulty-gated mechanic. Wired for every difficulty; the Majordomo hook
 * (boss_majordomo_executus.cpp) does not gate it, and health is scaled per difficulty/player
 * count by coa_boss_flex like the rest of the room (rev_20260930_97).
 *
 * Timing (same re-query, by pull id): each instance's own lifetime before dying to raid damage
 * clusters at 10-24s (median ~15s) across every difficulty and player count sampled; the boss
 * summons a new one on a median ~47-50s cycle starting a median 29s after the pull (both
 * measured, see boss_majordomo_executus.cpp).
 *
 * Kit (refs.md: export + Spell.dbc; Berserk matches Majordomo's own kit, refs.md's own read):
 *   - 2108020 Sacrifice - "Sacrifices the target to the Fire Lord, reducing its damage taken
 *     from outside sources ..., stunning and roasting it." Cast on itself at spawn: the
 *     burn/stun window the raid has to kill it in.
 *   - 2108023 Sacrifice (rank 2) - "Restores the target health to full, then sacrifices it to
 *     the Firelord." Reads as the punish for missing that window: heal to full and restart the
 *     burn. [designed] 20s loop timer - the corpus never times this cast directly (92030 is
 *     only ever the log's target/health row, "casts: 0" throughout), chosen because every
 *     measured kill in every difficulty lands under 24s and the large majority under 20s,
 *     consistent with a raid routinely beating this loop once and rarely needing a second try.
 *   - 2100213 Berserk - seen once across the entire 35-pull Ascended sample (Auras applied:
 *     Berserk(1)). [designed] as the second loop's opener, matching that rarity: failing the
 *     burn window twice in a row is the edge case, not the norm.
 *
 * Type 10 in the export ("non-combat/object-like") is real DBC data, unlike this row's
 * placeholder level/health/faction (hp-pools.md) - implemented passive, no melee: the raid
 * burns it down, it does not fight back.
 */

#include "CreatureScript.h"
#include "ScriptedCreature.h"
#include "../../../src/server/scripts/EasternKingdoms/BlackrockMountain/MoltenCore/molten_core.h"

namespace
{
    enum SacrificialChainsSpells
    {
        SPELL_SACRIFICE       = 2108020,
        SPELL_SACRIFICE_RENEW = 2108023,
        SPELL_BERSERK         = 2100213,
    };

    enum SacrificialChainsEvents
    {
        EVENT_SACRIFICE_RENEW = 1,
    };

    constexpr uint8 BERSERK_AFTER_LOOPS = 2;

    struct npc_sacrificial_chains_coa : public ScriptedAI
    {
        npc_sacrificial_chains_coa(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            _loops = 0;
            me->SetReactState(REACT_PASSIVE);
            DoCastSelf(SPELL_SACRIFICE, true);
            events.ScheduleEvent(EVENT_SACRIFICE_RENEW, 20s);
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            while (uint32 const eventId = events.ExecuteEvent())
            {
                if (eventId == EVENT_SACRIFICE_RENEW)
                {
                    DoCastSelf(SPELL_SACRIFICE_RENEW, true);
                    me->SetFullHealth();

                    if (++_loops == BERSERK_AFTER_LOOPS)
                    {
                        DoCastSelf(SPELL_BERSERK, true);
                    }

                    events.ScheduleEvent(EVENT_SACRIFICE_RENEW, 20s);
                }
            }
        }

    private:
        EventMap events;
        uint8 _loops = 0;
    };
}

void AddCoaSacrificialChainsScripts()
{
    RegisterMoltenCoreCreatureAI(npc_sacrificial_chains_coa);
}
