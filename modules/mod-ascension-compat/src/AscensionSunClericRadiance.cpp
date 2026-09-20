/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionSunClericRadiance.h"
#include "AscensionSunCleric.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include <algorithm>
namespace
{
using namespace AscensionSunCleric;

// #2442 Champion's Arrival (704905): "Increases the duration of Champion of the Sun and Chains
// of Light by 5 sec." Spell.dbc gives it SPELL_AURA_ADD_FLAT_MODIFIER / SPELLMOD_DURATION
// (EffectBasePoints_1 4999, DieSides 1 -> real +5000ms) with EffectSpellClassMask
// (0x4000, 0x800, 0x100000) on family 33. Word 1 (0x4000) matches the Chains of Light debuff
// (806697, SpellFamilyFlags_1 0x4000) exactly, so SpellInfo::IsAffectedBySpellMod already lets
// that half through natively -- no code needed there. Words 2 and 3 (0x800, 0x100000) do not
// match Champion of the Sun (800612, SpellFamilyFlags_1 0x20 only, its only nonzero word);
// scanning every family-33 record for those two bits instead finds "Twilight" (707612, 712445)
// and "Holy Form" (805301) / "Vow of the Valkyr" (807751) -- none of them Champion of the Sun.
// So the shipped record can never lengthen 800612's own self-buff (DurationIndex 63, 25000 ms:
// "Become a champion of An'she ... "). 800612 is cast on self (EffectImplicitTargetA_1 self), so
// extend it directly on its own apply.
constexpr uint32 CHAMPIONS_ARRIVAL = 704905;
constexpr int32 CHAMPIONS_ARRIVAL_EXTRA_MS = 5000;

class aura_ascension_champion_of_the_sun_arrival : public AuraScript
{
    PrepareAuraScript(aura_ascension_champion_of_the_sun_arrival);

    void ExtendForArrival(AuraEffect const* /*effect*/, AuraEffectHandleModes /*mode*/)
    {
        Player* player = Owner(GetCaster());
        Aura* aura = GetAura();
        if (!player || !aura || !player->HasAura(CHAMPIONS_ARRIVAL))
            return;
        int32 duration = aura->GetDuration() + CHAMPIONS_ARRIVAL_EXTRA_MS;
        aura->SetDuration(duration);
        aura->SetMaxDuration(std::max(aura->GetMaxDuration(), duration));
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(aura_ascension_champion_of_the_sun_arrival::ExtendForArrival,
            EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
    }
};
} // namespace
void ApplyAscensionSunClericRadianceContracts(SpellInfo* info)
{
    if (!info)
        return;

    // #2446 Harmonious Bells (704917): tooltip promises "Your Mercy now affects 4 additional
    // allies near the primary target." Effect 0 is SPELL_AURA_ADD_FLAT_MODIFIER /
    // SPELLMOD_JUMP_TARGETS (17) with EffectSpellClassMask (0x20, 0, 0), correctly matching
    // Mercy's (504848) own SpellFamilyFlags_1 0x20 -- Spell::SelectImplicitChainTargets adds this
    // modifier's raw value straight onto Mercy's own ChainTarget before searching secondary
    // targets, so the talent's own contribution to the extra-target count equals the modifier's
    // real value regardless of Mercy's baseline. Shipped EffectBasePoints_1 is 4; with
    // EffectDieSides_1 1, SpellEffectInfo::CalcValue resolves that to a real value of 5
    // (basePoints + 1) -- one more than the quoted "4 additional". The class's own convention for
    // this exact wording is a 1:1 match (Radiant Cascade's native ChainTarget 4 is worded "jumps
    // to up to 4 additional allies"; Everglow's own +2 modifier on Radiant Cascade matches its
    // "2 additional allies" exactly), so this is a shipped off-by-one, not a different design.
    // Patch it down by one so CalcValue returns exactly 4.
    if (info->Id == 704917)
        info->Effects[EFFECT_0].BasePoints -= 1;
}
void AddSC_AscensionSunClericRadiance()
{
    RegisterSpellScript(aura_ascension_champion_of_the_sun_arrival);
}
