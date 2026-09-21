/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellInfo.h"

namespace
{
enum BoulderSpells : uint32
{
    BoulderDash = 500692,
    BoulderDamage = 500693,
    BoulderImmunity = 680473,
    BoulderPacify = 681302
};

class primalist_boulder_metadata : public GlobalScript
{
public:
    primalist_boulder_metadata() : GlobalScript("primalist_boulder_metadata",
        {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* info) override
    {
        if (info->SpellFamilyName != 37)
            return;
        if (info->Id == BoulderImmunity)
        {
            // Native immunity-set 1733 covers movement impairment, loss of control and knockback.
            info->Effects[EFFECT_2].MiscValue = 1733;
            // Disorientation and slows are covered by the set; retain room for disarm and silence.
            info->Effects[EFFECT_0].MiscValue = MECHANIC_DISARM;
            info->Effects[EFFECT_1].ApplyAuraName = SPELL_AURA_MECHANIC_IMMUNITY;
            info->Effects[EFFECT_1].MiscValue = MECHANIC_SILENCE;
            info->Effects[EFFECT_1].BasePoints = 0;
            info->Effects[EFFECT_1].DieSides = 0;
        }
        else if (info->Id == BoulderPacify)
        {
            // The first helper already covers Sap; this native aura also stops autoattacks.
            info->Effects[EFFECT_2].ApplyAuraName = SPELL_AURA_MOD_PACIFY;
            info->Effects[EFFECT_2].MiscValue = 0;
        }
        if (info->Id == BoulderImmunity || info->Id == BoulderPacify || info->Id == BoulderDamage)
            // These short helpers are owned by the parent aura and must not survive it across logout.
            info->AttributesCu |= SPELL_ATTR0_CU_AURA_CANNOT_BE_SAVED;
    }
};

class primalist_boulder_cast_lock : public AllSpellScript
{
public:
    primalist_boulder_cast_lock() : AllSpellScript("primalist_boulder_cast_lock",
        {ALLSPELLHOOK_ON_SPELL_CHECK_CAST}) { }

    void OnSpellCheckCast(Spell* spell, bool /*strict*/, SpellCastResult& result) override
    {
        Unit* caster = spell->GetCaster();
        if (caster->IsPlayer() && caster->getClass() == CLASS_WILDWALKER && !spell->IsTriggered() &&
            caster->HasAura(BoulderDash, caster->GetGUID()))
            // The copied allow-only-ability aura exempts next-melee-swing spells and arrives on the first tick.
            result = SPELL_FAILED_SPELL_IN_PROGRESS;
    }
};
}

void AddSC_AscensionPrimalistBoulder()
{
    new primalist_boulder_metadata();
    new primalist_boulder_cast_lock();
}
