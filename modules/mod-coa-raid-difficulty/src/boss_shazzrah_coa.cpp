/*
 * Shazzrah's Blink (2105611) folds vanilla Gate of Shazzrah's dummy+teleport
 * pair (23138 dummy, 23139 teleport) into a single spell: EFFECT_0 is a
 * dummy (SPELL_EFFECT_DUMMY, target "current spell target") and EFFECT_1 is
 * SPELL_EFFECT_TELEPORT_UNITS (caster to a point behind that target) - the
 * engine already performs the teleport by itself. What is missing is the
 * threat wipe stock Gate of Shazzrah does onto whoever it teleports to
 * (boss_shazzrah.cpp's spell_shazzrah_gate_dummy::HandleScript, cited there
 * as sourced from wowwiki); this restores just that piece.
 */

#include "Creature.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellScriptLoader.h"
#include "Unit.h"

namespace
{
    class spell_shazzrah_blink_coa : public SpellScript
    {
        PrepareSpellScript(spell_shazzrah_blink_coa);

        void HandleThreatWipe(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();

            if (caster && target)
            {
                if (Creature* creatureCaster = caster->ToCreature())
                {
                    creatureCaster->GetThreatMgr().ResetAllThreat();
                    creatureCaster->GetThreatMgr().AddThreat(target, 1);
                    creatureCaster->AI()->AttackStart(target);
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
