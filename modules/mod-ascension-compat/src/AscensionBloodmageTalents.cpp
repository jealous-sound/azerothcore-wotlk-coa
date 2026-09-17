/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionPooledVitality.h"
#include "DBCStores.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuras.h"
#include "SpellScript.h"
#include "SpellInfo.h"
#include <algorithm>
#include <vector>

namespace
{
enum BloodmageTalentSpells : uint32
{
    SPELL_LIQUIFY = 806310,
    SPELL_VAMPIRIC_POOLS = 504088,
    SPELL_VAMPIRIC_POOLS_LEECH = 806311,
    SPELL_DARKCASTING = 712383,
    SPELL_BLOOD_TEAR_SPAWN = 712417,
    SPELL_ACCURSED_FORM = 562572,
    SPELL_SANGUINE_SCRIPTURE = 804851,
    SPELL_SANGUINE_SCRIPTURE_BUFF = 504264,
    SPELL_CURSED_FORM_REQUIREMENT = 525031,
    SPELL_CURSED_FORM_REQUIREMENT_2 = 524861,
    SPELL_BLOODMOON_POWER = 801961
};

// Every creature Animated Blood can leave behind: worms, parasites and the rank 3 amalgam.
constexpr uint32 AnimatedBloodSummons[] = {325301, 335301, 315301};

// Every shape the Bloodmage's Cursed Form can take: Blood Curse and the spells that replace it.
constexpr uint32 CursedForms[] = {562572, 562720, 680692, 800157, 801076};

bool IsCursedForm(uint32 id)
{
    return std::find(std::begin(CursedForms), std::end(CursedForms), id) != std::end(CursedForms);
}

// Cursed Form abilities gate their cast on a CasterAuraSpell marker that nothing in Spell.dbc ever
// grants, so they could never be cast. Two distinct marker spells are both named "Cursed Form" in
// Spell.dbc and are split across the kit's abilities (e.g. Ravenous Strike/Lunge/Claw Sweep/Bloodfang
// Bite use 525031, while Rotclaw/Ironhide/Reave/Bloodsurge/Apotheosis and others use 524861), so both
// need to be mirrored onto the real form state, the same way Palm Sigil's marker follows
// Runeshroud/Waveforged. AscensionBloodmage::CursedForm (802877) is a third, separate marker: it is
// the ExcludeCasterAuraSpell Sanguine Mend and the pooled-vitality empowerment check both rely on to
// block casting while shapeshifted, but nothing else ever grants it either, so it needs the same sync.
void SyncCursedFormRequirement(Player* player)
{
    bool active = false;
    for (uint32 form : CursedForms)
        if (player->HasAura(form, player->GetGUID()))
        {
            active = true;
            break;
        }

    for (uint32 marker : {uint32(SPELL_CURSED_FORM_REQUIREMENT), uint32(SPELL_CURSED_FORM_REQUIREMENT_2),
        uint32(AscensionBloodmage::CursedForm)})
    {
        if (!active)
            player->RemoveAurasDueToSpell(marker, player->GetGUID());
        else if (player->IsInWorld() && player->IsAlive() && !player->HasAura(marker, player->GetGUID()))
            player->CastSpell(player, marker, true);
    }
}

class spell_ascension_animated_blood : public SpellScript
{
    PrepareSpellScript(spell_ascension_animated_blood);

    bool Validate(SpellInfo const*) override
    {
        return ValidateSpellInfo({SPELL_DARKCASTING, SPELL_BLOOD_TEAR_SPAWN});
    }

    void HandleExtraWorms(SpellEffIndex index)
    {
        if (GetSpellInfo()->Effects[index].TriggerSpell != SPELL_BLOOD_TEAR_SPAWN)
            return;
        PreventHitDefaultEffect(index);
        Unit* caster = GetCaster();
        Aura* darkcasting = caster->GetAura(SPELL_DARKCASTING, caster->GetGUID());
        if (!darkcasting)
            return;
        uint8 const count = darkcasting->GetStackAmount();
        darkcasting->Remove();
        // The helper's zero summon count otherwise falls back to one worm on every ordinary cast.
        // Darkcasting supplies the extra worms; the parent supplies its rank/empowerment count.
        if (count)
            caster->CastCustomSpell(SPELL_BLOOD_TEAR_SPAWN, SPELLVALUE_BASE_POINT0, count, caster, true);
    }

    void ReplacePreviousBrood()
    {
        // Recasting replaces the previous brood instead of stacking a second one beside it.
        if (Unit* caster = GetCaster())
            for (uint32 entry : AnimatedBloodSummons)
                caster->RemoveAllMinionsByEntry(entry);
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_ascension_animated_blood::ReplacePreviousBrood);
        // This destination-only helper is triggered in LAUNCH, before target-specific effects.
        OnEffectLaunch += SpellEffectFn(spell_ascension_animated_blood::HandleExtraWorms,
            EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
    }
};

class bloodmage_talent_events : public UnitScript
{
public:
    bloodmage_talent_events() : UnitScript("bloodmage_talent_events", true,
        {UNITHOOK_ON_AURA_APPLY, UNITHOOK_ON_AURA_REMOVE}) { }

    void OnAuraApply(Unit* unit, Aura* aura) override
    {
        Player* player = unit ? unit->ToPlayer() : nullptr;
        if (!player || player->getClass() != CLASS_SON_OF_ARUGAL || !aura)
            return;
        if (IsCursedForm(aura->GetId()))
            SyncCursedFormRequirement(player);
    }

    void OnAuraRemove(Unit* unit, AuraApplication* application, AuraRemoveMode mode) override
    {
        Player* player = unit ? unit->ToPlayer() : nullptr;
        if (!player || player->getClass() != CLASS_SON_OF_ARUGAL || !application)
            return;
        Aura* aura = application->GetBase();
        if (IsCursedForm(aura->GetId()))
            SyncCursedFormRequirement(player);
        if (!player->IsAlive() || !player->IsInWorld() || mode == AURA_REMOVE_BY_DEATH)
            return;
        if (aura->GetId() == SPELL_LIQUIFY && aura->GetCasterGUID() == player->GetGUID() &&
            player->HasAura(SPELL_VAMPIRIC_POOLS))
            player->CastSpell(player, SPELL_VAMPIRIC_POOLS_LEECH, true);
        if (aura->GetId() == SPELL_LIQUIFY && aura->GetCasterGUID() == player->GetGUID() &&
            player->HasAura(SPELL_BLOODMOON_POWER))
        {
            // Bloodmoon Power: Liquify cleanses all negative dispellable effects when it ends.
            std::vector<uint32> remove;
            for (auto const& pair : player->GetAppliedAuras())
                if (!pair.second->IsPositive() && pair.second->GetBase()->GetSpellInfo()->Dispel != DISPEL_NONE)
                    remove.push_back(pair.second->GetBase()->GetId());
            for (uint32 id : remove)
                player->RemoveAurasDueToSpell(id);
        }
        if (aura->GetId() == SPELL_ACCURSED_FORM && aura->GetCasterGUID() == player->GetGUID() &&
            player->HasAura(SPELL_SANGUINE_SCRIPTURE))
            player->CastSpell(player, SPELL_SANGUINE_SCRIPTURE_BUFF, true);
    }
};

class bloodmage_talent_contracts : public GlobalScript
{
public:
    bloodmage_talent_contracts() : GlobalScript("bloodmage_talent_contracts",
        {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* info) override
    {
        if (!info || info->Id != SPELL_VAMPIRIC_POOLS_LEECH || info->SpellFamilyName != 26 ||
            info->Effects[EFFECT_0].Effect != SPELL_EFFECT_HEALTH_LEECH)
            return;

        // Vampiric Pools leeches and fears the same nearby targets when Liquify ends.
        // Keep the existing leech amount/coefficient and native damage-break proc data.
        info->DurationEntry = sSpellDurationStore.LookupEntry(32); // Six seconds.
        info->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE_EFF1;
        auto& fear = info->Effects[EFFECT_1];
        fear.Effect = SPELL_EFFECT_APPLY_AURA;
        fear.ApplyAuraName = SPELL_AURA_MOD_FEAR;
        fear.Mechanic = MECHANIC_FEAR;
        fear.TargetA = info->Effects[EFFECT_0].TargetA;
        fear.TargetB = info->Effects[EFFECT_0].TargetB;
        fear.RadiusEntry = info->Effects[EFFECT_0].RadiusEntry;
    }
};
}

void AddSC_AscensionBloodmageTalents()
{
    new bloodmage_talent_events();
    new bloodmage_talent_contracts();
    RegisterSpellScript(spell_ascension_animated_blood);
}
