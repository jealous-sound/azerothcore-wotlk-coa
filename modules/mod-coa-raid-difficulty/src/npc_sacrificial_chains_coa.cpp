/*
 * Sacrificial Chains (92030), Majordomo Executus's periodic hostage add.
 *
 * Correction (this session, raw WoWCombatLog re-query - mc-dataset.json's own aggregation drops
 * aura target names, so this reads the four logs directly:
 * local/logs/ascension/logs-rar/2026-08-25-14.41.47, -19.31.05, -23.06.25, -27-18.49.53 in the
 * coa-combatlog-parser checkout). The previous implementation had 92030 self-cast Sacrifice and
 * loop a heal-to-full/re-sacrifice on itself. That is not what happens:
 *
 *   - 2108020 Sacrifice's own Spell.dbc row targets TARGET_UNIT_TARGET_ENEMY, not the caster.
 *     Every logged SPELL_AURA_APPLIED for 2108020 has "Sacrificial Chains" as the *source* and a
 *     player as the *dest* (e.g. "..., 0xF13001677E00317C, Sacrificial Chains, ..., 0x...C569,
 *     Paredinha, ..., 2108020, Sacrifice, ..., DEBUFF"). The chain casts Sacrifice on players, not
 *     itself - this is the "chains a player" mechanic the restoration was missing.
 *   - Each spawn casts 2108023 Sacrifice rank 2 (a SPELL_EFFECT_HEAL_MAX_HEALTH, "restores the
 *     target health to full, then sacrifices it") on the same target a few hundred ms *before*
 *     2108020, once, at spawn - not a repeating loop. No repeat cast of 2108023 or 2108020 was
 *     ever observed for the same chain instance in any of the four logs.
 *   - One spawn (one creature GUID) chains more than one player at once: every apply for a given
 *     GUID lands within ~0.5s of the others, e.g. GUID …E003357 (Ascended, 12p,
 *     2026-08-25-14.41.47) applies to both "Scorchwalker" and "Geonox" 0.5s apart. Counting
 *     distinct targets per GUID across all four logs: Heroic (13p, 6 spawns) maxes at 2;
 *     Ascended (12p and 15p, 18 spawns) maxes at 2; Mythic (17p, 8 spawns) maxes at 3 (two of the
 *     eight spawns). No Normal Majordomo pull exists in the corpus. Implemented as a per-mode cap
 *     (Normal/Heroic/Ascended 2, Mythic 3 - Ascended's cap matches Heroic's own measured value,
 *     not the nominal difficulty order; Normal mirrors Heroic, untested) on a random distinct
 *     selection of nearby raid members, clamped to however many are actually available.
 *   - Killing the chain frees its chained player(s): in every clean (non-wipe) sample, e.g. GUID
 *     …E00317C, the chain's own UNIT_DIED/PARTY_KILL and the SPELL_AURA_REMOVED of 2108020 on its
 *     target(s) share the exact same log timestamp (17:43:57.250 for all three lines). 2108020's
 *     own SpellDuration (index 5) is 300000ms - far longer than the 10-24s the chain actually
 *     lived - so this is not the debuff's own timer expiring; the removal is tied to the chain's
 *     death. JustDied here explicitly clears Sacrifice from every player it chained.
 *   - Berserk (2100213) is real but rare (1/56 Sacrifice applications, one Ascended pull) and, in
 *     that one case, cast at spawn alongside the Sacrifice applies (not after some elapsed
 *     "loops"; there is no loop to count). Kept as a small chance at spawn, matching the measured
 *     rarity, self-only (TARGET_UNIT_CASTER on all three effects per Spell.dbc) and with no other
 *     behavioural effect on this passive, non-melee add.
 *   - Not implemented: a "roast" that kills an unfreed chain's captives. 2108020 has no
 *     EffectTriggerSpell and no SPELL_PERIODIC_DAMAGE tied to spell id 2108020 appears anywhere
 *     in the four logs. The one Berserk sample's two chained players (Scorchwalker, Froez) did
 *     both die ~38-48s after being chained without the chain itself dying first - but nothing in
 *     Spell.dbc or the log attributes that death to 2108020 specifically (2108020's own
 *     SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN effect is -101, i.e. ~immune to ordinary damage from
 *     "outside sources"), so a timed kill on unfreed captives would be invented, not evidenced.
 *     Flagged, not built.
 *
 * Timing (same re-query, by pull id): each instance's own lifetime before dying to raid damage
 * clusters at 10-24s (median ~15s) across every difficulty and player count sampled; the boss
 * summons a new one on a median ~47-50s cycle starting a median 29s after the pull (both
 * measured, see boss_majordomo_executus.cpp - unchanged by this correction).
 *
 * Type 10 in the export ("non-combat/object-like") is real DBC data, unlike this row's
 * placeholder level/health/faction (hp-pools.md) - implemented passive, no melee: the raid
 * burns it down, it does not fight back.
 */

#include "Containers.h"
#include "CreatureScript.h"
#include "DBCEnums.h"
#include "ObjectAccessor.h"
#include "ScriptedCreature.h"
#include "zg_coa_common.h"
#include "../../../src/server/scripts/EasternKingdoms/BlackrockMountain/MoltenCore/molten_core.h"

namespace
{
    enum SacrificialChainsSpells
    {
        SPELL_SACRIFICE       = 2108020,
        SPELL_SACRIFICE_RENEW = 2108023,
        SPELL_BERSERK         = 2100213,
    };

    // Measured max distinct targets chained per spawn (raw WoWCombatLog re-query, see header):
    // Normal untested, mirrors Heroic; Ascended matches Heroic's own measured cap, not Mythic's.
    constexpr uint8 CHAIN_TARGETS_BY_MODE[MAX_RAID_DIFFICULTY] = { 2, 2, 3, 2 };

    // 1/56 Sacrifice applications carried Berserk in the corpus (one Ascended pull).
    constexpr float BERSERK_CHANCE_PCT = 1.8f;

    struct npc_sacrificial_chains_coa : public ScriptedAI
    {
        npc_sacrificial_chains_coa(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            me->SetReactState(REACT_PASSIVE);
            _chained.clear();

            uint8 const mode = std::min<uint8>(uint8(me->GetMap()->GetSpawnMode()), MAX_RAID_DIFFICULTY - 1);
            uint8 const cap = CHAIN_TARGETS_BY_MODE[mode];

            std::vector<Player*> targets = coa_zg::PlayersWithin(me, 40.0f);
            Acore::Containers::RandomResize(targets, cap);

            for (Player* target : targets)
            {
                DoCast(target, SPELL_SACRIFICE_RENEW, true);
                DoCast(target, SPELL_SACRIFICE, true);
                _chained.push_back(target->GetGUID());
            }

            if (roll_chance_f(BERSERK_CHANCE_PCT))
                DoCastSelf(SPELL_BERSERK, true);
        }

        void JustDied(Unit* /*killer*/) override
        {
            for (ObjectGuid const& guid : _chained)
                if (Unit* target = ObjectAccessor::GetUnit(*me, guid))
                    target->RemoveAurasDueToSpell(SPELL_SACRIFICE);

            _chained.clear();
        }

        void UpdateAI(uint32 /*diff*/) override { }

    private:
        GuidVector _chained;
    };
}

void AddCoaSacrificialChainsScripts()
{
    RegisterMoltenCoreCreatureAI(npc_sacrificial_chains_coa);
}
