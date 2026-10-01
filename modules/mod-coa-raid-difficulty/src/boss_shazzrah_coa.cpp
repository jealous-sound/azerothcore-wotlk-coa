/*
 * Shazzrah's Blink (2105611) folds vanilla Gate of Shazzrah's dummy+teleport
 * pair (23138 dummy, 23139 teleport) into a single spell: EFFECT_0 is a
 * dummy (SPELL_EFFECT_DUMMY, target "current spell target") and EFFECT_1 is
 * SPELL_EFFECT_TELEPORT_UNITS (caster to a point behind that target) - the
 * engine already performs the teleport by itself. What is missing is the
 * threat wipe stock Gate of Shazzrah does onto whoever it teleports to
 * (boss_shazzrah.cpp's spell_shazzrah_gate_dummy::HandleScript, cited there
 * as sourced from wowwiki); this restores just that piece.
 *
 * It also leaves a "Reflection of Shazzrah" (11504) behind at the spot he
 * blinked away from, casting Mirrored Arcane Explosion (2105650) once
 * (smart_scripts, pending SQL) before its own timed despawn clears it. The
 * reflection is summoned in effect 0, before the engine applies effect 1's
 * teleport, so GetCaster()'s position here is still the pre-blink spot.
 */

#include "Creature.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellScriptLoader.h"
#include "TemporarySummon.h"
#include "Unit.h"

namespace
{
    enum ShazzrahBlinkSpells
    {
        NPC_REFLECTION_OF_SHAZZRAH = 11504,
    };

    class spell_shazzrah_blink_coa : public SpellScript
    {
        PrepareSpellScript(spell_shazzrah_blink_coa);

        void HandleThreatWipe(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();

            if (caster && target)
            {
                Position const prevBlinkPos = caster->GetPosition();

                if (Creature* creatureCaster = caster->ToCreature())
                {
                    creatureCaster->GetThreatMgr().ResetAllThreat();
                    creatureCaster->GetThreatMgr().AddThreat(target, 1);
                    creatureCaster->AI()->AttackStart(target);
                    creatureCaster->SummonCreature(NPC_REFLECTION_OF_SHAZZRAH, prevBlinkPos, TEMPSUMMON_TIMED_DESPAWN, 6000);
                }
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_shazzrah_blink_coa::HandleThreatWipe, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };
}

void AddCoaShazzrahScripts()
{
    RegisterSpellScript(spell_shazzrah_blink_coa);
}
