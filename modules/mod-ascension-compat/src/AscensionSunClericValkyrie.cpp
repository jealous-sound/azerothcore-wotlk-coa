/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "ScriptMgr.h"
#include "SpellInfo.h"

namespace
{
enum ValkyrieSpells : uint32
{
    SPELL_ARBITER_OF_GRACE = 301313,
    SPELL_GLORIOUS_EXECUTION_MANA_PASSIVE = 807451,
    SPELL_ARBITER_OF_LIGHT = 302914,
    SPELL_GAVEL_OF_LIGHT_PASSIVE = 707521,
};

class sun_cleric_valkyrie_metadata : public GlobalScript
{
public:
    sun_cleric_valkyrie_metadata() : GlobalScript("sun_cleric_valkyrie_metadata",
        {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* info) override
    {
        if (info->SpellFamilyName != 33)
            return;
        // Arbiter of Grace (#1818): the tooltip promises Glorious Execution's Mana Passive
        // (807451, matched natively: SpellInfo::IsAffectedBySpellMod's generic family-33 path
        // ANDs this aura's EffectSpellClassMask word 1 = 0x400 against 807451's own
        // SpellFamilyFlags word 1 = 0x400, a real overlap) an additional 15% mana restore.
        // Effect 0's stored BasePoints (-1) resolves to 0 with DieSides 1
        // (SpellEffectInfo::CalcBaseValue only applies the "-1" convention when DieSides != 0;
        // every correctly authored sibling talent in this batch stores value-1, e.g. Holy
        // Fervor's 19 for "20%"), so the SPELLMOD_EFFECT1 (op 3) flat modifier this aura builds
        // always adds zero regardless of routing. Corrected to 14 so the modifier resolves to
        // +15, matching "an additional 15% mana equal to the damage dealt" exactly. Read by the
        // already-existing AscensionSunClericAbilities.cpp OnSpellHitResult handler via
        // Amount(807451) whenever Glorious Execution's main-hand hit (800691) or off-hand hit
        // (802566) lands with damage.
        if (info->Id == SPELL_ARBITER_OF_GRACE)
            info->Effects[EFFECT_0].BasePoints = 14;
        // Arbiter of Light (#1834): the same defect, targeting the Gavel of Light passive
        // (707521, matched natively: EffectSpellClassMask word 0 = 0x200000 against 707521's
        // own SpellFamilyFlags word 0 = 0x200000, the only family-33 spell carrying that bit).
        // EffectBasePoints_1 was -1 (resolves to 0); corrected to 99 so the modifier resolves to
        // +100, matching "your Gavel of Light now restores an additional 100% health equal to
        // the damage dealt". Read by the already-existing AscensionSunClericAbilities.cpp
        // OnSpellHitResult handler via Amount(707521) whenever Gavel of Light rank 1 (800611)
        // lands with damage.
        if (info->Id == SPELL_ARBITER_OF_LIGHT)
            info->Effects[EFFECT_0].BasePoints = 99;
    }
};
}

void AddSC_AscensionSunClericValkyrie()
{
    new sun_cleric_valkyrie_metadata();
}
