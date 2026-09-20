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

// #2452 Vindicator (704938 rank 1, 707773 rank 2): "Your Glorious Execution now ignores 10%/20%
// of the target's armor while Vow of the Valkyr is active." Both ranks are contract-patched in
// ApplyAscensionSunClericRadianceContracts() below onto the dedicated Ascension aura
// Unit::CalcArmorReducedDamage already reads for this exact purpose
// (SPELL_AURA_ASCENSION_MOD_IGNORE_ARMOR_PCT, 338), filtered onto Glorious Execution (800626) by
// AuraEffect::IsAffectedOnSpell. That native aura is unconditional once applied, so the "while
// Vow of the Valkyr is active" clause is enforced here by zeroing/restoring its cached amount as
// Vow of the Valkyr (807749) is applied or removed -- the same pattern
// AscensionSunCleric::Refresh() already uses for other conditional talents (see e.g. its
// SetAmount(player, 300364, 0, extraHaste) toggle), reused via the public SetAmount()/Amount()
// helpers rather than a parallel mechanism.
constexpr uint32 VINDICATOR_RANK_1 = 704938;
constexpr uint32 VINDICATOR_RANK_2 = 707773;
constexpr uint32 VOW_OF_THE_VALKYR = 807749;

void SyncVindicator(Player* player)
{
    if (!player)
        return;
    bool active = player->HasAura(VOW_OF_THE_VALKYR);
    for (uint32 id : {VINDICATOR_RANK_1, VINDICATOR_RANK_2})
        if (player->HasAura(id))
            SetAmount(player, id, EFFECT_0, active ? Amount(id, EFFECT_0) : 0);
}

class aura_ascension_vindicator_vow_gate : public AuraScript
{
    PrepareAuraScript(aura_ascension_vindicator_vow_gate);

    void Sync(AuraEffect const* /*effect*/, AuraEffectHandleModes /*mode*/)
    {
        SyncVindicator(Owner(GetCaster()));
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(aura_ascension_vindicator_vow_gate::Sync,
            EFFECT_ALL, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        AfterEffectRemove += AuraEffectRemoveFn(aura_ascension_vindicator_vow_gate::Sync,
            EFFECT_ALL, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
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

    // #2452 Vindicator: see the comment above aura_ascension_vindicator_vow_gate. Shipped as
    // SPELL_AURA_ADD_FLAT_MODIFIER / SPELLMOD_EFFECT2 (12) with EffectSpellClassMask
    // (0, 0, 0x100000). Scanning every family-33 record for that bit finds only "Holy Form"
    // (805301) and "Vow of the Valkyr" (807751) -- never Glorious Execution (800626,
    // SpellFamilyFlags (0, 0x1000, 4)) -- so the shipped mod can never match its own named
    // target, and even a corrected classMask would still be an unconditional SpellMod with no
    // way to gate on another aura's presence. Retarget both ranks onto
    // SPELL_AURA_ASCENSION_MOD_IGNORE_ARMOR_PCT (338); its native consumer
    // Unit::CalcArmorReducedDamage does armor = AddPct(armor, -ignoreArmorPct), so the stored
    // amount must be positive, the opposite sign from the old flat modifier (shipped raw
    // BasePoints -11 / -21 resolve, via CalcValue's "+1" for DieSides 1, to a real -10 / -20).
    if (info->Id == VINDICATOR_RANK_1 || info->Id == VINDICATOR_RANK_2)
    {
        SpellEffectInfo& effect = info->Effects[EFFECT_0];
        int32 const raw = effect.BasePoints; // -11 (rank 1) / -21 (rank 2) -> real -10 / -20
        effect.ApplyAuraName = SPELL_AURA_ASCENSION_MOD_IGNORE_ARMOR_PCT;
        effect.BasePoints = -raw - 2; // +9 / +19 -> CalcValue() now returns +10 / +20
        effect.SpellClassMask = flag96(0, 4096, 4); // Glorious Execution's (800626) own family flags
    }
}
void AddSC_AscensionSunClericRadiance()
{
    RegisterSpellScript(aura_ascension_champion_of_the_sun_arrival);
    RegisterSpellScript(aura_ascension_vindicator_vow_gate);
}
