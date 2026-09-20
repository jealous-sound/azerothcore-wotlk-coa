/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "ScriptMgr.h"
#include "SpellInfo.h"

namespace
{
constexpr uint32 FaeDust = 804664;

class primalist_fae_presence_metadata : public GlobalScript
{
public:
    primalist_fae_presence_metadata() : GlobalScript("primalist_fae_presence_metadata",
        {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* info) override
    {
        if (info->Id == FaeDust && info->SpellFamilyName == 37 &&
            info->Effects[EFFECT_0].IsAura(SPELL_AURA_MOD_STEALTH_DETECT) &&
            info->Effects[EFFECT_0].MiscValue == 10)
            // This client index exceeds the native two-entry stealth array.
            // Keep the native Fae Presence amount/reagent modifiers, applying detection to ordinary stealth.
            info->Effects[EFFECT_0].MiscValue = STEALTH_GENERAL;
    }
};
}

void AddSC_AscensionPrimalistFaePresence()
{
    new primalist_fae_presence_metadata();
}
