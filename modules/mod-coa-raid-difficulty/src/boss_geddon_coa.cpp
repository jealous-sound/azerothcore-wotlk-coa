/*
 * Baron Geddon's Living Bomb (2105701 dummy -> 2105702..05 periodic carrier aura, ticking
 * 2105706 self-damage) has no explosion-on-expiry anywhere in the DBC kit (diag-G2.md item 1) -
 * this adds one per the user's own CoA memory: on natural expiry (not a dispel, not the
 * carrier's death) the carrier launches into the air and everyone else within 5 yd takes fire
 * damage. Not evidenced in any corpus log; flagged designed in docs/coa/molten-core.md. Damage
 * follows the vanilla Living Bomb explosion (20476, 3200) scaled by the same 1/1.44/1.88/2.32
 * ladder used for every other per-difficulty figure in this instance - the user's own choice of
 * coefficient, not a measured value for this spell.
 */

#include "Map.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "SpellScriptLoader.h"
#include "Unit.h"

#include <algorithm>

namespace
{
    enum GeddonLivingBombSpells
    {
        SPELL_LIVING_BOMB_EXPLOSION = 2105706, // reused as the explosion's own combat-log id
    };

    constexpr float EXPLOSION_RADIUS = 5.0f;
    constexpr uint32 EXPLOSION_BASE_DAMAGE = 3200;
    constexpr float DIFFICULTY_LADDER[4] = { 1.0f, 1.44f, 1.88f, 2.32f };
    constexpr float EXPLOSION_LAUNCH_SPEED_Z = 10.0f;

    class spell_geddon_living_bomb_explosion_coa : public AuraScript
    {
        PrepareAuraScript(spell_geddon_living_bomb_explosion_coa);

        void HandleExpire(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            AuraApplication const* application = GetTargetApplication();
            if (!application || application->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                return;

            Unit* carrier = GetTarget();
            if (!carrier || !carrier->GetMap())
                return;

            Unit* source = GetCaster();
            if (!source)
                source = carrier;

            SpellInfo const* info = sSpellMgr->GetSpellInfo(SPELL_LIVING_BOMB_EXPLOSION);
            if (info)
            {
                uint8 const mode = std::min<uint8>(uint8(carrier->GetMap()->GetSpawnMode()), 3);
                uint32 const damage = uint32(EXPLOSION_BASE_DAMAGE * DIFFICULTY_LADDER[mode]);

                carrier->GetMap()->DoForAllPlayers([&](Player* player)
                {
                    if (static_cast<Unit*>(player) == carrier || !player->IsAlive() || player->IsGameMaster())
                        return;
                    if (!player->IsWithinDist(carrier, EXPLOSION_RADIUS))
                        return;

                    SpellNonMeleeDamage log(source, player, info, SPELL_SCHOOL_MASK_FIRE);
                    log.damage = std::min<uint32>(damage, player->GetHealth());
                    source->SendSpellNonMeleeDamageLog(&log);
                    Unit::DealDamage(source, player, log.damage, nullptr, SPELL_DIRECT_DAMAGE, SPELL_SCHOOL_MASK_FIRE, info, false);
                });
            }

            carrier->KnockbackFrom(carrier->GetPositionX(), carrier->GetPositionY(), 0.0f, EXPLOSION_LAUNCH_SPEED_Z);
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_geddon_living_bomb_explosion_coa::HandleExpire, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE, AURA_EFFECT_HANDLE_REAL);
        }
    };
}

void AddCoaGeddonScripts()
{
    RegisterSpellScript(spell_geddon_living_bomb_explosion_coa);
}
