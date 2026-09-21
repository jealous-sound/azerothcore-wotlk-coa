/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "ScriptMgr.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Unit.h"

namespace
{
// Bloodmage proc passives whose tooltip states a condition that no `spell_proc` column can express.
// The `spell_proc` row supplies the widest event set the tooltip allows; these CheckProc handlers
// narrow it to the exact wording. Everything else about the proc (family mask, chance, phase) stays
// in data.

// The Bloodmage's Spell.dbc family, as the neighbouring module files spell it.
constexpr uint32 BLOODMAGE_SPELL_FAMILY = 26;

// Ravenous Strike ranks 500123/501671-501679 all carry SpellFamilyFlags (0, 1, 0), and Bloodfang Bite
// ranks 501695-501697/503613-503615/572549-572551/800156 all carry (0, 8388608, 0).
constexpr uint32 GRIM_OMEN_STRIKE_MASK1 = 1 | 8388608;

// The Howl set in Spell.dbc family 26: Shadow Howl 806177 and Wicked Howl 804207 (0, 0, 128),
// Night Hunter's Howl 500124/501680-501686 (0, 0, 2048), Blood Howl 800782 (0, 0, 131072) and
// Monstrous Howl 804811 (0, 4, 131072).
constexpr uint32 GRIM_OMEN_HOWL_MASK1 = 4;
constexpr uint32 GRIM_OMEN_HOWL_MASK2 = 128 | 2048 | 131072;

// Infection (704624): "Auto attacking a bleeding target will now infect the wound". A `spell_proc` row
// can require the auto attack but has no column for "bleeding", so the target is checked here.
// MECHANIC_BLEED is 15; Unit::HasAuraWithMechanic inspects both the spell's Mechanic and its effects'.
class aura_ascension_bloodmage_infection : public AuraScript
{
    PrepareAuraScript(aura_ascension_bloodmage_infection);

    bool CheckProc(ProcEventInfo& event)
    {
        Unit* victim = event.GetActionTarget();
        return victim && victim->HasAuraWithMechanic(1ULL << MECHANIC_BLEED);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_bloodmage_infection::CheckProc);
    }
};

// Grim Omen (800154): "Critical strikes with Ravenous Strike or Bloodfang Bite, or using Howl spells,
// now triggers Call of the Darkwing." That is two different proc conditions - one gated on a critical
// hit, one not - and a single `spell_proc` entry carries only one HitMask/SpellPhaseMask pair. The row
// admits both families in both phases; this handler applies the tooltip's per-family rule.
class aura_ascension_bloodmage_grim_omen : public AuraScript
{
    PrepareAuraScript(aura_ascension_bloodmage_grim_omen);

    bool CheckProc(ProcEventInfo& event)
    {
        SpellInfo const* spellInfo = event.GetSpellInfo();
        if (!spellInfo || spellInfo->SpellFamilyName != BLOODMAGE_SPELL_FAMILY)
            return false;

        // Monstrous Howl carries a first-word flag too, so the Howl set is tested first.
        if (spellInfo->SpellFamilyFlags.HasFlag(0, GRIM_OMEN_HOWL_MASK1, GRIM_OMEN_HOWL_MASK2))
            return (event.GetSpellPhaseMask() & PROC_SPELL_PHASE_CAST) != 0;

        if (spellInfo->SpellFamilyFlags.HasFlag(0, GRIM_OMEN_STRIKE_MASK1, 0))
            return (event.GetSpellPhaseMask() & PROC_SPELL_PHASE_HIT) != 0 &&
                (event.GetHitMask() & PROC_HIT_CRITICAL) != 0;

        return false;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_bloodmage_grim_omen::CheckProc);
    }
};

// Malediction (704120): "increasing the damage they take from your next 5 direct damage spells by 20%".
// The companion `spell_proc` row supplies the five charges and the family mask; what no `spell_proc`
// column expresses is "your". Effect 0's own aura type is already caster-restricted - both in
// Unit::SpellDamageBonusTaken, which compares the aura effect's caster with the attacker before applying
// the bonus, and in AuraEffect::CheckEffectProc, which rejects SPELL_AURA_MOD_DAMAGE_FROM_CASTER for a
// foreign actor - but that per-effect rejection only clears effect 0 from the proc mask. Effect 1 is a
// plain SPELL_AURA_PERIODIC_DAMAGE whose CheckEffectProc has no caster test, so the mask stays non-zero,
// Aura::PrepareProcToTrigger still spends a charge, and a second Bloodmage attacking the same victim
// would drain the first one's five charges. This handler rejects the event outright instead.
class aura_ascension_bloodmage_malediction : public AuraScript
{
    PrepareAuraScript(aura_ascension_bloodmage_malediction);

    bool CheckProc(ProcEventInfo& event)
    {
        Unit* actor = event.GetActor();
        return actor && actor->GetGUID() == GetCasterGUID();
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_bloodmage_malediction::CheckProc);
    }
};
}

void AddSC_AscensionBloodmageProcs()
{
    RegisterSpellScript(aura_ascension_bloodmage_infection);
    RegisterSpellScript(aura_ascension_bloodmage_grim_omen);
    RegisterSpellScript(aura_ascension_bloodmage_malediction);
}
