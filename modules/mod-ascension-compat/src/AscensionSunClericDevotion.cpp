/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionSunCleric.h"
#include "AscensionSunClericData.h"
#include "AscensionSunClericDevotion.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellInfo.h"
#include <algorithm>
namespace
{
using namespace AscensionSunCleric;
// Grace (#1910): "Increases the spell damage bonus of Dawnsear by 20% and Sunflare by 10%."
constexpr uint32 SUN_CLERIC_GRACE = 504070;
class sun_cleric_devotion_scaling : public UnitScript
{
public:
    sun_cleric_devotion_scaling() : UnitScript("sun_cleric_devotion_scaling", true,
        {UNITHOOK_MODIFY_SPELL_EFFECT_BASE_VALUE}) { }
    // Grace 504070 carries SPELL_AURA_ADD_FLAT_MODIFIER (107) with EffectMiscValue SPELLMOD_BONUS_MULTIPLIER
    // (24), effect 0 matching Dawnsear via class mask word1 bit 0x2 (+20, i.e. +20%) and effect 1 matching
    // Sunflare via word1 bit 0x100 (+10, i.e. +10%). That native modifier is only read inside
    // Unit::SpellDamageBonusDone's `if ((coeff || unleashedFrostGlyph) && DoneAdvertisedBenefit)` block. The
    // live spell_bonus_data row shared by every Dawnsear/Sunflare rank (500146, 502366-502371 and
    // 800231, 502386-502397) sets direct_bonus to 0, which unconditionally overwrites that function's local
    // `coeff` to 0 just before this check (`coeff = bonus->direct_damage;`), so the condition is false and
    // ApplySpellMod(SPELLMOD_BONUS_MULTIPLIER, ...) never runs: Grace's own aura can never fire on this core.
    // These spells' real spell-power scaling instead runs through this same ModifySpellEffectBaseValue hook,
    // reading SunClericCoefficients (see sun_cleric_scaling in AscensionSunClericContracts.cpp), so Grace's
    // percentage is applied to that same SP-derived term here instead, independently of the dead native path.
    void ModifySpellEffectBaseValue(Unit const* caster, SpellInfo const* info, uint8 index, float& value) override
    {
        Player* player = Owner(caster);
        if (!player || !info || info->SpellFamilyName != 33 || Derived(info) || !player->HasAura(SUN_CLERIC_GRACE))
            return;
        bool const dawnsear = info->SpellFamilyFlags.HasFlag(0, 0x2, 0);
        bool const sunflare = !dawnsear && info->SpellFamilyFlags.HasFlag(0, 0x100, 0);
        if (!dawnsear && !sunflare)
            return;
        for (auto const& row : SunClericCoefficients)
            if (row.spell == info->Id && row.effect == index && row.sp != 0.0f)
            {
                float const bonusPercent = Amount(SUN_CLERIC_GRACE, dawnsear ? 0 : 1) / 100.0f;
                value += row.sp * std::max(0, player->SpellBaseDamageBonusDone(info->GetSchoolMask())) * bonusPercent;
            }
    }
};
}
void AddSC_AscensionSunClericDevotion()
{
    new sun_cleric_devotion_scaling();
}
