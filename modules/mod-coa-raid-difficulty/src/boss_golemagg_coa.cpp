/*
 * Golemagg the Incinerator: Massive Stomp (2105817) ground-fire follow-up.
 *
 * Massive Stomp's own DBC kit is a tank-only dummy+stun with no damage and no area
 * component (diag-G2.md item 6). Per the user's own memory of live CoA play, it drops the
 * same ground-fire puddle Magmadar's heads already cast under a random player
 * (boss_magmadar_coa.cpp, 2105366 dummy -> persistent-area 2105367-70, Damage Info'd per
 * difficulty) - reused here rather than inventing a new spell family, cast from Golemagg
 * himself the same way a Magmadar head does (not the hit player, which is what originally
 * broke the puddle's own target search - see boss_magmadar_coa.cpp). Designed from player
 * memory; no corpus log confirms it.
 */

#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellScriptLoader.h"
#include "../../../src/server/scripts/EasternKingdoms/BlackrockMountain/MoltenCore/molten_core.h"

namespace
{
    enum GolemaggCoaSpells
    {
        SPELL_MASSIVE_STOMP        = 2105817,
        SPELL_GROUND_FIRE_DUMMY    = 2105366, // shared with Magmadar's heads (boss_magmadar_coa.cpp)
    };

    class spell_golemagg_massive_stomp_ground_fire_coa : public SpellScript
    {
        PrepareSpellScript(spell_golemagg_massive_stomp_ground_fire_coa);

        void HandleGroundFire(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Unit* target = caster->SelectNearbyTarget(nullptr, 100.0f))
                caster->CastSpell(target, SPELL_GROUND_FIRE_DUMMY, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_golemagg_massive_stomp_ground_fire_coa::HandleGroundFire, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };
}

void AddCoaGolemaggScripts()
{
    RegisterSpellScript(spell_golemagg_massive_stomp_ground_fire_coa);
}
