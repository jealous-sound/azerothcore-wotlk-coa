/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionScalingBaseData.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellInfo.h"
#include "Unit.h"

#include <algorithm>

namespace
{
// CoA scales most of its content by level through SpellDescriptionVariables row 182 rather than
// through Spell.dbc's own per-level fields. A spell's formula text multiplies one effect's authored
// base points by it, always as "${$<spellId>m<N>*$<scalingbp>+ ...}", and the generated list records
// which effect of which spell that is.
//
// The curve is a level normaliser, not a stat coefficient: 0.0318 at level 1, 0.0939 at 10, 0.1982
// at 20, 0.5184 at 40, and 0.9874 at CoA's level-60 cap, which it crosses at about 60.46. A server
// that ignores it therefore pays the full level-60 amount at every level, which is invisible at the
// cap and enormous while levelling - Templar's Scourgebane helper 801832 carries 261 base points, so
// a level 13 character took the whole 261 where the client's own tooltip promises 261 * 0.1213 = 32
// plus its attack power term.
//
// SpellInfo::CalcValue calls this hook after base points, dice and RealPointsPerLevel and before
// combo points, effect modifiers and the creature-level multiplier, which is exactly where the
// authored base is still the authored base.
double ScalingBase(uint8 level)
{
    return 0.0267291844060354 + 0.0048541098014737 * level + 0.0001859597762293 * level * level;
}

// $PL is the player's level. A summon or totem carries its own, so resolve the owner and leave the
// value alone when no player is behind the cast rather than scaling by a creature's level.
Player const* LevelOwner(Unit const* caster)
{
    if (!caster)
        return nullptr;
    if (Player const* player = caster->ToPlayer())
        return player;
    if (Unit const* owner = caster->GetOwner())
        return owner->ToPlayer();
    return nullptr;
}

class ascension_scaling_base : public UnitScript
{
public:
    ascension_scaling_base() : UnitScript("ascension_scaling_base",
        true, {UNITHOOK_MODIFY_SPELL_EFFECT_BASE_VALUE}) { }

    void ModifySpellEffectBaseValue(Unit const* caster, SpellInfo const* info, uint8 index,
        float& value) override
    {
        if (!info || index >= MAX_SPELL_EFFECTS || value == 0.0f)
            return;

        auto const entry = std::lower_bound(AscensionCompatData::ScalingBaseSpells.begin(),
            AscensionCompatData::ScalingBaseSpells.end(), info->Id,
            [](AscensionCompatData::ScalingBaseEntry const& row, uint32 id) { return row.Spell < id; });
        if (entry == AscensionCompatData::ScalingBaseSpells.end() || entry->Spell != info->Id ||
            !(entry->EffectMask & (1 << index)))
            return;

        Player const* player = LevelOwner(caster);
        if (!player)
            return;

        value = float(double(value) * ScalingBase(player->GetLevel()));
    }
};
}

void AddAscensionScalingBaseScripts()
{
    new ascension_scaling_base();
}
