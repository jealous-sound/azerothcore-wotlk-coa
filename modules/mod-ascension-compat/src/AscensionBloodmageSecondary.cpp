/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionPooledVitality.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "Random.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <list>
#include <vector>

namespace
{
enum BloodmageSecondarySpells : uint32
{
    SPELL_VEINBURST = 504260,
    SPELL_REAVE = 800490,
    SPELL_REAVE_BLEED = 802883,
    SPELL_REAVE_EXECUTE = 802884,
    SPELL_REAVE_BACK = 805612,
    SPELL_HEMOBURST = 572855,
    SPELL_HEMOTURGY = 572856,
    SPELL_CURSED_FORM = 562720,
    SPELL_ACCURSED_FORM = 562572,
    SPELL_VAMPYRS_KISS = 504275,
    SPELL_VAMPYRS_KISS_COPY = 504785,
    SPELL_BLACK_HEART = 680731,
    SPELL_NIGHT_HUNTER = 704659,
    SPELL_BLOOD_FEAST_RESTORE = 706608,
    SPELL_ROTCLAW = 804197,
    SPELL_ROTCLAW_ENERGIZE = 805352, // Ravenous Strike (Energize): 30..70 internal, i.e. 3 to 7 Rage
    SPELL_BLOOD_THIRST = 706613,
    SPELL_INSATIABLE = 706621,
    SPELL_INSATIABLE_STACK = 706663,
    SPELL_VAMPIRIC_FANG = 804726,
    SPELL_VAMPIRIC_FANG_SCALAR = 680753, // Thirst SLS: EFFECT_2 carries the damage percent per Thirst stack
    SPELL_BLOODMOON_BLAST = 500125,
    SPELL_BLOOD_RITUALS = 706623,
    SPELL_BLOOD_RITUALS_HEAL = 704119 // "Blood Rituals Heal Proc Source" (704118) triggers this
};

// Every Vampiric Fang rank: the base strike plus its learned ranks.
constexpr uint32 VampiricFangRanks[] = {804726, 504093, 504094, 504095, 504096, 504097, 553271, 553272};

bool IsVampiricFang(uint32 id)
{
    return std::find(std::begin(VampiricFangRanks), std::end(VampiricFangRanks), id) !=
        std::end(VampiricFangRanks);
}

bool RankOf(uint32 id, uint32 root)
{
    return id == root || sSpellMgr->GetFirstSpellInChain(id) == root;
}

Player* Bloodmage(Spell* spell)
{
    Player* player = spell->GetCaster()->ToPlayer();
    return player && player->getClass() == CLASS_SON_OF_ARUGAL &&
        spell->GetSpellInfo()->SpellFamilyName == 26 ? player : nullptr;
}

void CopyDamage(Player* player, Unit* target, uint32 id, uint32 damage)
{
    if (!damage)
        return;
    // Custom basepoints pass through native float arithmetic before conversion back to int32.
    uint32 maximum = uint32(std::nextafter(float(std::numeric_limits<int32>::max()), 0.0f));
    player->CastCustomSpell(id, SPELLVALUE_BASE_POINT0, int32(std::min(damage, maximum)), target, true);
}

class bloodmage_secondary_casts : public AllSpellScript
{
public:
    bloodmage_secondary_casts() : AllSpellScript("bloodmage_secondary_casts",
        {ALLSPELLHOOK_ON_CAST, ALLSPELLHOOK_ON_CALCULATED_TARGET, ALLSPELLHOOK_ON_CRIT_CHANCE,
            ALLSPELLHOOK_ON_HIT_RESULT}) { }

    void OnSpellCast(Spell* spell, Unit*, SpellInfo const* info, bool) override
    {
        Player* player = Bloodmage(spell);
        if (!player || spell->IsTriggered())
            return;

        if (player->HasAura(SPELL_BLOOD_RITUALS) &&
            sSpellMgr->GetFirstSpellInChain(info->Id) == SPELL_BLOODMOON_BLAST)
            // Blood Rituals' heal is a Proc Trigger Spell dummy (704118, "Blood Rituals Heal
            // Proc Source") with zero proc flags, so it never fires natively; the cast event
            // is supplied here instead.
            player->CastSpell(player, SPELL_BLOOD_RITUALS_HEAL, true);

        if (!player->HasAura(SPELL_NIGHT_HUNTER))
            return;
        if (RankOf(info->Id, SPELL_VEINBURST))
        {
            if (roll_chance_i(40))
                player->RemoveSpellCooldown(info->Id, true);
        }
        else if (AscensionBloodmage::GetEmpowerment(info->Id) == AscensionBloodmage::Bloodbolt &&
            info->PowerType == POWER_RAGE && spell->GetPowerCost() > 0 && roll_chance_i(40))
            player->ModifyPower(POWER_RAGE, spell->GetPowerCost() / 2);
    }

    void OnSpellCritChance(Spell* spell, Unit* target, float& chance) override
    {
        if (Bloodmage(spell) && target && RankOf(spell->GetSpellInfo()->Id, SPELL_HEMOBURST) &&
            target->HasAuraState(AURA_STATE_BLEEDING))
            chance = 100;
    }

    void OnSpellCalculatedTarget(Spell* spell, Unit* target, TargetInfo& hit) override
    {
        Player* player = Bloodmage(spell);
        if (!player || !target || hit.damage <= 0 || hit.missCondition != SPELL_MISS_NONE)
            return;
        uint32 id = spell->GetSpellInfo()->Id;
        if (RankOf(id, SPELL_VEINBURST) && target->HasAuraState(AURA_STATE_BLEEDING))
        {
            hit.damage = uint32(hit.damage * 1.25f);
            hit.damageBeforeTakenMods = uint32(hit.damageBeforeTakenMods * 1.25f);
        }
        if (!spell->IsTriggered() && RankOf(id, SPELL_REAVE))
        {
            uint32 conditions = target->HasAuraState(AURA_STATE_BLEEDING) ? 1 : 0;
            if (target->HealthBelowPct(35))
                conditions |= 2;
            if (!target->HasInArc(float(M_PI), player))
                conditions |= 4;
            // Reave has one victim. Snapshot its conditions before the initial hit changes health.
            spell->SetScriptValue(SPELL_REAVE, conditions);
        }
        if (IsVampiricFang(id))
        {
            // Vampiric Fang promises bonus damage per Thirst stack. The scalar
            // lives on Thirst SLS (680753) EFFECT_2; fixed values read straight
            // off BasePoints, rolled ones add the engine's +1.
            int32 pctPerStack = 0;
            if (SpellInfo const* scalar = sSpellMgr->GetSpellInfo(SPELL_VAMPIRIC_FANG_SCALAR))
                pctPerStack = scalar->Effects[EFFECT_2].BasePoints +
                    (scalar->Effects[EFFECT_2].DieSides ? 1 : 0);
            uint32 stacks = 0;
            if (Aura const* thirst = player->GetAura(SPELL_BLOOD_THIRST))
                stacks = thirst->GetStackAmount();
            if (stacks && pctPerStack > 0)
            {
                // Custom basepoints pass through native float arithmetic before returning to int32.
                uint32 maximum =
                    uint32(std::nextafter(float(std::numeric_limits<int32>::max()), 0.0f));
                double mult = 1.0 + double(stacks) * double(pctPerStack) / 100.0;
                hit.damage = uint32(std::min(double(hit.damage) * mult, double(maximum)));
                hit.damageBeforeTakenMods =
                    uint32(std::min(double(hit.damageBeforeTakenMods) * mult, double(maximum)));
            }
        }
    }

    void OnSpellHitResult(Spell* spell, Unit* target, uint8 miss, uint32 damage, uint32, bool) override
    {
        Player* player = Bloodmage(spell);
        if (!player || spell->IsTriggered() || !target || miss != SPELL_MISS_NONE)
            return;
        uint32 id = spell->GetSpellInfo()->Id;
        if (target == player && (id == SPELL_CURSED_FORM || id == SPELL_ACCURSED_FORM))
            // Native duration modifiers have already extended this application.
            player->RemoveAurasDueToSpell(SPELL_HEMOTURGY);
        if (target == player || player->IsFriendlyTo(target))
            return;
        if (RankOf(id, SPELL_HEMOBURST) && !spell->GetScriptValue(SPELL_HEMOTURGY))
        {
            spell->SetScriptValue(SPELL_HEMOTURGY, 1);
            player->CastSpell(player, SPELL_HEMOTURGY, true);
        }
        // Rotclaw's description promises the income ("dealing ... Shadow damage, generating Rage and
        // infecting their wounds"), but none of its three effects is an energize and no companion
        // "Rotclaw (Energize)" record exists, unlike Ravenous Strike, Bloodmoon Blast, Sanguine Rupture
        // and Lunge, which all carry effect 142 pointing at their own Energize spell. Unit::DealDamage
        // only pays Rage for weapon damage, so the ability granted none. The amount is in no source -
        // not Spell.dbc, not spell_proc/spell_linked_spell, not the 2026-09-13 exiles-db export - so the
        // Rage is paid with Ravenous Strike's own Energize record rather than a new number, and once per
        // cast: Rotclaw hits up to 25 enemies.
        if (RankOf(id, SPELL_ROTCLAW) && !spell->GetScriptValue(SPELL_ROTCLAW_ENERGIZE))
        {
            spell->SetScriptValue(SPELL_ROTCLAW_ENERGIZE, 1);
            player->CastSpell(player, SPELL_ROTCLAW_ENERGIZE, true);
        }
        // Vampiric Fang expends Thirst: steal health equal to the damage dealt,
        // then clear Thirst and Insatiable. Runs once per cast on the first
        // successful hostile hit.
        if (IsVampiricFang(id) && !spell->GetScriptValue(SPELL_VAMPIRIC_FANG))
        {
            spell->SetScriptValue(SPELL_VAMPIRIC_FANG, 1);
            if (damage)
                Unit::DealHeal(player, player, damage);
            player->RemoveAurasDueToSpell(SPELL_BLOOD_THIRST);
            player->RemoveAurasDueToSpell(SPELL_INSATIABLE);
            player->RemoveAurasDueToSpell(SPELL_INSATIABLE_STACK);
        }
        if (!damage)
            return;
        if (RankOf(id, SPELL_REAVE) && !spell->GetScriptValue(SPELL_REAVE_BLEED))
        {
            spell->SetScriptValue(SPELL_REAVE_BLEED, 1);
            uint32 conditions = uint32(spell->GetScriptValue(SPELL_REAVE));
            if (conditions & 1)
                CopyDamage(player, target, SPELL_REAVE_BLEED, damage);
            if (conditions & 2)
                CopyDamage(player, target, SPELL_REAVE_EXECUTE, damage);
            if (conditions & 4)
                CopyDamage(player, target, SPELL_REAVE_BACK, damage);
        }
    }
};

class bloodmage_kiss_periodic : public UnitScript
{
public:
    bloodmage_kiss_periodic() : UnitScript("bloodmage_kiss_periodic", true,
        {UNITHOOK_ON_PERIODIC_DAMAGE_RESULT}) { }

    void OnPeriodicDamageResult(Unit* target, Unit*, uint32 damage, SpellInfo const*) override
    {
        if (!target || !target->IsAlive() || !damage)
            return;
        // The active curse copies all periodic damage to its victim, including allied casters' ticks.
        // Collect owner GUIDs before casting, since a copy can kill the target and remove its auras.
        std::vector<ObjectGuid> owners;
        for (auto const& pair : target->GetAppliedAuras())
        {
            Aura* aura = pair.second->GetBase();
            if (RankOf(aura->GetId(), SPELL_VAMPYRS_KISS) &&
                std::find(owners.begin(), owners.end(), aura->GetCasterGUID()) == owners.end())
                owners.push_back(aura->GetCasterGUID());
        }
        for (ObjectGuid const& guid : owners)
            if (Unit* unit = ObjectAccessor::GetUnit(*target, guid))
                if (Player* player = unit->ToPlayer(); player && player->getClass() == CLASS_SON_OF_ARUGAL &&
                    player->IsAlive() && player->IsInWorld() && player->InSamePhase(target) && target->IsAlive() &&
                    player->IsValidAttackTarget(target))
                {
                    CopyDamage(player, target, SPELL_VAMPYRS_KISS_COPY, damage / 4);
                    // Black Heart: Vampyr's Kiss also regenerates 20% of maximum Rage when it copies damage.
                    if (player->HasAura(SPELL_BLACK_HEART))
                        player->ModifyPower(POWER_RAGE, int32(player->GetMaxPower(POWER_RAGE)) / 5);
                }
    }
};

class bloodmage_secondary_contracts : public GlobalScript
{
public:
    bloodmage_secondary_contracts() : GlobalScript("bloodmage_secondary_contracts",
        {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* info) override
    {
        if (!info || info->SpellFamilyName != 26)
            return;
        if (info->Id == SPELL_REAVE_BLEED || info->Id == SPELL_REAVE_EXECUTE || info->Id == SPELL_REAVE_BACK ||
            info->Id == SPELL_VAMPYRS_KISS_COPY)
        {
            info->AttributesEx2 |= SPELL_ATTR2_CANT_CRIT;
            info->AttributesEx3 |= SPELL_ATTR3_IGNORE_CASTER_MODIFIERS;
            info->AttributesEx4 |= SPELL_ATTR4_IGNORE_DAMAGE_TAKEN_MODIFIERS;
            info->AscensionInheritsResolvedAmount = true;
            info->Effects[EFFECT_0].BonusMultiplier = 0.0f;
        }
    }
};

class spell_ascension_blood_feast_corpses : public SpellScript
{
    PrepareSpellScript(spell_ascension_blood_feast_corpses);

    void Select(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();
        // Filter before the native three-target cap so living units cannot take corpse slots.
        targets.remove_if([caster](WorldObject* object)
        {
            Unit* target = object ? object->ToUnit() : nullptr;
            return !target || target->IsAlive() || target == caster || caster->IsFriendlyTo(target);
        });
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ascension_blood_feast_corpses::Select,
            EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
    }
};

class spell_ascension_blood_feast_drain : public SpellScript
{
    PrepareSpellScript(spell_ascension_blood_feast_drain);

    bool Validate(SpellInfo const*) override { return ValidateSpellInfo({SPELL_BLOOD_FEAST_RESTORE}); }

    void Drain(SpellEffIndex)
    {
        Player* player = GetCaster()->ToPlayer();
        Unit* corpse = GetHitUnit();
        if (player && player->getClass() == CLASS_SON_OF_ARUGAL && player->IsAlive() && !player->IsInCombat() &&
            corpse && !corpse->IsAlive() && corpse != player && !player->IsFriendlyTo(corpse))
            player->CastSpell(player, SPELL_BLOOD_FEAST_RESTORE, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_ascension_blood_feast_drain::Drain, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};
}

void AddSC_AscensionBloodmageSecondary()
{
    new bloodmage_secondary_casts();
    new bloodmage_kiss_periodic();
    new bloodmage_secondary_contracts();
    RegisterSpellScript(spell_ascension_blood_feast_corpses);
    RegisterSpellScript(spell_ascension_blood_feast_drain);
}
