/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionPooledVitality.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "Random.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <list>
#include <unordered_map>
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
    SPELL_ANEURYSM = 806099,
    SPELL_ANEURYSM_HEAL = 520860, // Aneurysm's own 5% maximum-health SPELL_EFFECT_HEAL_PCT helper
    SPELL_KILLER_INSTINCT = 704690,
    SPELL_NIGHT_FEAST = 563736,
    SPELL_THIRST_FOR_BLOOD = 570023,
    SPELL_SATED = 570024,
    SPELL_RAVENOUS = 570025,
    SPELL_DARK_ESSENCE = 680732,
    SPELL_DARK_ESSENCE_HEAL = 681036,
    SPELL_BLOOD_RITUALS = 706623,
    SPELL_CURSED_FORM_REQUIREMENT = 525031,
    SPELL_CURSED_FORM_REQUIREMENT_2 = 524861,
    SPELL_VAMPIRIC_FANG_SHARE = 572373, // the aura-354 record that names the leeched share
    SPELL_VAMPIRIC_FANG_HEAL = 572374   // its TriggerSpell, a plain SPELL_EFFECT_HEAL on the caster
};

// Every Vampiric Fang rank: the base strike plus its learned ranks.
constexpr uint32 VampiricFangRanks[] = {804726, 504093, 504094, 504095, 504096, 504097, 553271, 553272};

bool IsVampiricFang(uint32 id)
{
    return std::find(std::begin(VampiricFangRanks), std::end(VampiricFangRanks), id) !=
        std::end(VampiricFangRanks);
}

// Cursed Form abilities are exactly the records the kit gates on one of its two "Cursed Form" markers.
bool IsCursedFormAbility(SpellInfo const* info)
{
    return info->CasterAuraSpell == SPELL_CURSED_FORM_REQUIREMENT ||
        info->CasterAuraSpell == SPELL_CURSED_FORM_REQUIREMENT_2;
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
        {ALLSPELLHOOK_ON_CAST, ALLSPELLHOOK_ON_PREPARE, ALLSPELLHOOK_ON_CALCULATED_TARGET,
            ALLSPELLHOOK_ON_CRIT_CHANCE, ALLSPELLHOOK_ON_HIT_RESULT,
            ALLSPELLHOOK_ON_SUCCESSFUL_INTERRUPT}) { }

    // Aneurysm's school lockout is native (effect 68 plus its own 4 s duration), but its second clause -
    // "Successfully interrupting an enemy heals you for $520860s1% of your maximum health" - had no caster:
    // 520860 exists as a SPELL_EFFECT_HEAL_PCT of 5% on the caster and nothing ever cast it.
    // Spell::EffectInterruptCast raises this hook only when an interrupt actually landed.
    void OnSpellSuccessfulInterrupt(Spell* spell, Unit*) override
    {
        Player* player = Bloodmage(spell);
        if (!player || spell->GetSpellInfo()->Id != SPELL_ANEURYSM ||
            spell->GetScriptValue(SPELL_ANEURYSM_HEAL))
            return;
        spell->SetScriptValue(SPELL_ANEURYSM_HEAL, 1);
        player->CastSpell(player, SPELL_ANEURYSM_HEAL, true);
    }

    void OnSpellPrepare(Spell* spell, Unit*, SpellInfo const* info) override
    {
        Player* player = Bloodmage(spell);
        if (!player || spell->IsTriggered())
            return;
        // Night Feast reads the Rage the caster holds when the cast starts: Spell::_cast pays the
        // (increased) cost before the launch phase calculates damage, so the amount is kept here.
        if (AuraEffect const* feast = player->GetAuraEffect(SPELL_NIGHT_FEAST, EFFECT_0))
            if (feast->IsAffectedOnSpell(info))
                spell->SetScriptValue(SPELL_NIGHT_FEAST, player->GetPower(POWER_RAGE));
    }

    void OnSpellCast(Spell* spell, Unit*, SpellInfo const* info, bool) override
    {
        Player* player = Bloodmage(spell);
        if (!player || spell->IsTriggered())
            return;
        // Dark Essence: "Casting Cursed Form abilities or Bloodbolt will now heal allies affected by
        // Blood Rituals". Every number lives in 681036, which only the unobtainable 806945 ever cast.
        if (player->HasAura(SPELL_DARK_ESSENCE) && (IsCursedFormAbility(info) ||
            AscensionBloodmage::GetEmpowerment(info->Id) == AscensionBloodmage::Bloodbolt))
            player->CastSpell(player, SPELL_DARK_ESSENCE_HEAL, true);
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
        Player* player = Bloodmage(spell);
        if (!player)
            return;
        if (target && RankOf(spell->GetSpellInfo()->Id, SPELL_HEMOBURST) &&
            target->HasAuraState(AURA_STATE_BLEEDING))
            chance = 100;
        // Killer Instinct: "Each point of Rage now increases the critical strike chance of Veinburst and
        // Reave by $/1000;S1%, up to a maximum of 50%." Its own effect 0 holds the per-point amount in
        // thousandths, the cap in MiscValueB and the two abilities in its class mask. Rage is stored ten
        // times the displayed value, so amount/1000 per displayed point is amount/10000 per stored unit.
        if (AuraEffect const* instinct = player->GetAuraEffect(SPELL_KILLER_INSTINCT, EFFECT_0))
            if (instinct->IsAffectedOnSpell(spell->GetSpellInfo()))
                chance += std::min(float(player->GetPower(POWER_RAGE)) * instinct->GetAmount() / 10000.0f,
                    float(instinct->GetMiscValueB()));
    }

    void OnSpellCalculatedTarget(Spell* spell, Unit* target, TargetInfo& hit) override
    {
        Player* player = Bloodmage(spell);
        if (!player || !target || hit.damage <= 0 || hit.missCondition != SPELL_MISS_NONE)
            return;
        uint32 id = spell->GetSpellInfo()->Id;
        // Night Feast: "Each point of Rage now increases the damage of Bloodbolt by 1%". Effect 0 is a
        // private override-class-script marker carrying that percentage and Bloodbolt's class mask;
        // effect 1's SPELLMOD_COST half already works natively and is left alone.
        if (uint64 rage = spell->GetScriptValue(SPELL_NIGHT_FEAST))
            if (AuraEffect const* feast = player->GetAuraEffect(SPELL_NIGHT_FEAST, EFFECT_0))
            {
                float multiplier = 1.0f + float(rage) * feast->GetAmount() / 1000.0f;
                hit.damage = uint32(hit.damage * multiplier);
                hit.damageBeforeTakenMods = uint32(hit.damageBeforeTakenMods * multiplier);
            }
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
        // Vampiric Fang promises "Steal ${$m1+0+$SP*0.34} health from an enemy", but none of its eight
        // ranks (804726, 504093-504097, 553271, 553272) has a leech or heal effect: each is a plain
        // SPELL_EFFECT_SCHOOL_DAMAGE plus an aura 271. The stolen share lives in a separate "Vampiric
        // Fang" record, 572373, whose single effect is aura type 354 with amount 100 (BasePoints 99 +
        // DieSides 1) naming TriggerSpell 572374, a SPELL_EFFECT_HEAL on the caster with BasePoints 0.
        // AuraEffectHandler[354] is nullptr, no Spell.dbc row applies 572373 and no acquisition route
        // reaches it, so the heal never happened. Dominion of Blood (680716) is aura 107 with MiscValue 3
        // (SPELLMOD_EFFECT1) and +50 over EffectSpellClassMask (0, 1073741824, 0), which is 572373's own
        // SpellFamilyFlags - "Your Vampiric Fang now heals you for an additional 50% of the damage dealt".
        // SpellEffectInfo::CalcValue runs Unit::ApplyEffectModifiers, so the share reads 100 without the
        // talent and 150 with it; no number is declared here.
        if (RankOf(id, SPELL_VAMPIRIC_FANG))
            if (SpellInfo const* share = sSpellMgr->GetSpellInfo(SPELL_VAMPIRIC_FANG_SHARE))
                if (int32 percent = share->Effects[EFFECT_0].CalcValue(player); percent > 0)
                {
                    uint64 heal = uint64(damage) * uint64(percent) / 100;
                    CopyDamage(player, player, SPELL_VAMPIRIC_FANG_HEAL,
                        uint32(std::min<uint64>(heal, std::numeric_limits<uint32>::max())));
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
        // Thirst for Blood's two markers ship TriggerSpell 570025/570024 and Amplitude 500 on aura 4,
        // which never ticks and forwards nothing. Aura 226 is the periodic dummy the record's own half
        // second re-evaluation needs; the tier thresholds and amounts stay in its own text and payloads.
        if (info->Id == SPELL_THIRST_FOR_BLOOD &&
            info->Effects[EFFECT_0].ApplyAuraName == SPELL_AURA_DUMMY && info->Effects[EFFECT_0].Amplitude)
            info->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_PERIODIC_DUMMY;
    }
};

// Thirst for Blood: "Between 1-5 stacks of Thirst: Spell haste increased by $570024s1%. Between 6-10
// stacks of Thirst: The bonus damage dealt by spell critical strikes is increased by $570025s1%."
// Both payloads exist and are well formed; nothing ever applied them. 570025 already drops 570024
// through its own SPELL_EFFECT_REMOVE_AURA, so the two tiers stay mutually exclusive by the record's
// own design.
class aura_ascension_bloodmage_thirst_for_blood : public AuraScript
{
    PrepareAuraScript(aura_ascension_bloodmage_thirst_for_blood);

    bool Validate(SpellInfo const*) override { return ValidateSpellInfo({SPELL_SATED, SPELL_RAVENOUS}); }

    void Evaluate(AuraEffect const* /*effect*/)
    {
        Player* player = GetTarget() ? GetTarget()->ToPlayer() : nullptr;
        if (!player || player->getClass() != CLASS_SON_OF_ARUGAL || !player->IsAlive())
            return;
        Aura const* thirst = player->GetAura(SPELL_BLOOD_THIRST);
        uint32 stacks = thirst ? thirst->GetStackAmount() : 0;
        uint32 wanted = !stacks ? 0u : stacks <= 5 ? uint32(SPELL_SATED) : uint32(SPELL_RAVENOUS);
        for (uint32 id : {uint32(SPELL_SATED), uint32(SPELL_RAVENOUS)})
            if (id != wanted)
                player->RemoveAurasDueToSpell(id, player->GetGUID());
        if (!wanted)
            return;
        // Both payloads carry a finite duration; the periodic re-evaluation keeps the active tier alive.
        if (Aura* tier = player->GetAura(wanted, player->GetGUID()))
            tier->RefreshDuration();
        else
            player->CastSpell(player, wanted, true);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(aura_ascension_bloodmage_thirst_for_blood::Evaluate,
            EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// Dark Essence's helper heals every nearby ally; the talent promises only "allies affected by Blood
// Rituals", so the area selection is narrowed to the units that carry 706623.
class spell_ascension_dark_essence : public SpellScript
{
    PrepareSpellScript(spell_ascension_dark_essence);

    void Select(std::list<WorldObject*>& targets)
    {
        targets.remove_if([](WorldObject* object)
        {
            Unit* target = object ? object->ToUnit() : nullptr;
            return !target || !target->HasAura(SPELL_BLOOD_RITUALS);
        });
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ascension_dark_essence::Select,
            EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
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

// Blood Craving's Rage tick is SPELL_EFFECT_ENERGIZE_PCT, and Spell::EffectEnergizePct pays a percentage of
// *maximum* power, while the record's Description and AuraDescription both promise a percentage of *missing*
// Rage. No native effect expresses that and the handler ignores MiscValueB, so the same percentage is paid
// here against the missing amount instead. The percentage itself stays the record's own effect value.
class spell_ascension_bloodmage_blood_craving : public SpellScript
{
    PrepareSpellScript(spell_ascension_bloodmage_blood_craving);

    void MissingRage(SpellEffIndex index)
    {
        PreventHitDefaultEffect(index);
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        int32 misc = GetSpellInfo()->Effects[index].MiscValue;
        if (!caster || !target || !target->IsAlive() || misc < 0 || misc >= int32(MAX_POWERS))
            return;
        Powers power = Powers(misc);
        if (target->IsPlayer() && !target->CanReceivePowerFromSpell(power))
            return;
        uint32 maximum = target->GetMaxPower(power);
        uint32 current = target->GetPower(power);
        if (maximum <= current)
            return;
        if (uint32 gain = CalculatePct(maximum - current, GetEffectValue()))
            caster->EnergizeBySpell(target, GetSpellInfo()->Id, gain, power);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_ascension_bloodmage_blood_craving::MissingRage,
            EFFECT_1, SPELL_EFFECT_ENERGIZE_PCT);
    }
};
// Waves of Blood (681427): "Each tick of Crimson Tide now has a $681427h% chance to heal a nearby ally for
// $681427s1% of the damage dealt." The record's single effect is the private aura 354, which has no handler
// in AuraEffectHandler (SpellAuraEffects.cpp:419) and no core consumer, and Spell.dbc gives it ProcFlags 0,
// so nothing ever reached its TriggerSpell 807652. The proc flags, the Crimson Tide restriction and the 10%
// chance come from the companion spell_proc row; the heal ships with BasePoints 0 and BonusMultiplier 0, so
// the 25% share of the tick has to be forwarded as a custom base point.
constexpr uint32 WavesOfBloodHeal = 807652;

class aura_ascension_waves_of_blood : public AuraScript
{
    PrepareAuraScript(aura_ascension_waves_of_blood);

    bool Validate(SpellInfo const*) override { return ValidateSpellInfo({WavesOfBloodHeal}); }

    bool Check(ProcEventInfo& event)
    {
        Unit* player = GetTarget();
        DamageInfo const* damage = event.GetDamageInfo();
        return player->IsPlayer() && player->getClass() == CLASS_SON_OF_ARUGAL && player->IsAlive() &&
            player->IsInWorld() && event.GetActor() == player && damage && damage->GetDamage();
    }

    void Proc(AuraEffect const* effect, ProcEventInfo& event)
    {
        PreventDefaultAction();
        uint64 amount = uint64(event.GetDamageInfo()->GetDamage()) * std::clamp(effect->GetAmount(), 0, 100) / 100;
        if (!amount)
            return;
        // 807652 picks the single nearby ally itself, from its own 40 yard TARGET_UNIT_DEST_AREA_ALLY record.
        GetTarget()->CastCustomSpell(WavesOfBloodHeal, SPELLVALUE_BASE_POINT0,
            int32(std::min<uint64>(amount, std::numeric_limits<int32>::max())), GetTarget(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_waves_of_blood::Check);
        OnEffectProc += AuraEffectProcFn(aura_ascension_waves_of_blood::Proc, EFFECT_0, AuraType(354));
    }
};

// Hemal Excision (803681): "Cut into an ally's vital essence, siphoning all curse effects from them. You
// may reactivate this ability again within $803734d to place these curse effects on an enemy."
//
// The siphoning half is native: effect 0 is 38 SPELL_EFFECT_DISPEL with MiscValue 2 (DISPEL_CURSE),
// BasePoints 19 -> 20 dispels, TargetA 21 (TARGET_UNIT_TARGET_ALLY). Spell::EffectDispel keeps no record
// of what it removed, so the siphoned set is captured here, before and after the native effect runs.
//
// The re-activation half has records of its own that nothing referenced. 803734 "Hemal Excision" is the
// window: DurationIndex 1 = 10000 ms, a single SPELL_AURA_DUMMY on the caster, AuraDescription "You may
// reactivate Hemal Excision to place all siphoned curses on an enemy". 803733 "Excision" is the
// re-activation itself: "Apply all siphoned curses onto an enemy", CastingTimeIndex 1 (instant),
// RecoveryTime 0, RangeIndex 4 (30 yds), ManaCostPercentage 5, effect 0 = 3 SPELL_EFFECT_DUMMY on
// TargetA 6 (TARGET_UNIT_TARGET_ENEMY) and effect 1 = 164 SPELL_EFFECT_REMOVE_AURA with TriggerSpell
// 803734 on the caster. So the shipped data models the re-activation as a second, cooldown-free spell
// rather than a second cast of 803681, and Hemal Excision's own 90 s RecoveryTime is never bypassed,
// reset or shortened: it starts on the first cast and keeps running through the window.
//
// The window swaps the button through the fork's own temporary-replacement machinery, the same way
// AscensionRangerFalconstrike swaps Quick Shot for Falconstrike, so "reactivate this ability" reaches
// 803733 from the client's point of view.
constexpr uint32 SPELL_HEMAL_EXCISION = 803681;
constexpr uint32 SPELL_EXCISION = 803733;
constexpr uint32 SPELL_HEMAL_EXCISION_WINDOW = 803734;

struct SiphonedCurse
{
    uint32 spellId;
    ObjectGuid caster;
    int32 duration;
    uint8 stacks;
};

// Siphoned curses live only for the 10 s window: 803734's removal clears the owner's entry, whether the
// window was spent on an enemy or simply expired.
std::unordered_map<ObjectGuid, std::vector<SiphonedCurse>> siphonedCurses;

class spell_ascension_bloodmage_hemal_excision : public SpellScript
{
    PrepareSpellScript(spell_ascension_bloodmage_hemal_excision);

    bool Validate(SpellInfo const*) override { return ValidateSpellInfo({SPELL_HEMAL_EXCISION_WINDOW}); }

    // Spell::EffectDispel picks its victims from Unit::GetDispellableAuraList and rolls
    // Aura::CalcDispelChance for each, so the only reliable record of what it actually took is the
    // difference between the dispellable set before the effect and the auras still there afterwards.
    void Capture(SpellMissInfo missInfo)
    {
        _candidates.clear();
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (missInfo != SPELL_MISS_NONE || !caster || !target)
            return;
        DispelChargesList dispellable;
        target->GetDispellableAuraList(caster, SpellInfo::GetDispelMask(DISPEL_CURSE), dispellable,
            GetSpellInfo());
        for (auto const& pair : dispellable)
            if (Aura const* aura = pair.first)
                _candidates.push_back({aura->GetId(), aura->GetCasterGUID(), aura->GetDuration(),
                    aura->GetStackAmount()});
    }

    void Siphon()
    {
        Player* player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
        Unit* target = GetHitUnit();
        if (!player || !target || _candidates.empty())
            return;
        std::vector<SiphonedCurse> taken;
        for (SiphonedCurse const& curse : _candidates)
            if (!target->GetAura(curse.spellId, curse.caster))
                taken.push_back(curse);
        if (taken.empty())
            return;
        siphonedCurses[player->GetGUID()] = taken;
        player->CastSpell(player, SPELL_HEMAL_EXCISION_WINDOW, true);
    }

    void Register() override
    {
        BeforeHit += BeforeSpellHitFn(spell_ascension_bloodmage_hemal_excision::Capture);
        AfterHit += SpellHitFn(spell_ascension_bloodmage_hemal_excision::Siphon);
    }

    std::vector<SiphonedCurse> _candidates;
};

class aura_ascension_bloodmage_hemal_excision : public AuraScript
{
    PrepareAuraScript(aura_ascension_bloodmage_hemal_excision);

    bool Validate(SpellInfo const*) override { return ValidateSpellInfo({SPELL_EXCISION}); }

    void Offer(AuraEffect const*, AuraEffectHandleModes)
    {
        Player* player = GetTarget() ? GetTarget()->ToPlayer() : nullptr;
        if (!player)
            return;
        if (player->GetSpellMap().find(SPELL_EXCISION) == player->GetSpellMap().end())
            player->learnSpell(SPELL_EXCISION, true);
        player->SetTemporarySpellReplacement(SPELL_HEMAL_EXCISION, SPELL_EXCISION);
    }

    void Withdraw(AuraEffect const*, AuraEffectHandleModes)
    {
        Player* player = GetTarget() ? GetTarget()->ToPlayer() : nullptr;
        if (!player)
            return;
        player->SetTemporarySpellReplacement(SPELL_HEMAL_EXCISION, 0);
        if (player->GetSpellMap().find(SPELL_EXCISION) != player->GetSpellMap().end())
            player->removeSpell(SPELL_EXCISION, SPEC_MASK_ALL, true);
        siphonedCurses.erase(player->GetGUID());
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(aura_ascension_bloodmage_hemal_excision::Offer,
            EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(aura_ascension_bloodmage_hemal_excision::Withdraw,
            EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_ascension_bloodmage_excision : public SpellScript
{
    PrepareSpellScript(spell_ascension_bloodmage_excision);

    bool Validate(SpellInfo const*) override { return ValidateSpellInfo({SPELL_HEMAL_EXCISION_WINDOW}); }

    SpellCastResult CheckWindow()
    {
        Player* player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
        if (!player || !player->HasAura(SPELL_HEMAL_EXCISION_WINDOW, player->GetGUID()))
            return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
        auto itr = siphonedCurses.find(player->GetGUID());
        return itr != siphonedCurses.end() && !itr->second.empty() ?
            SPELL_CAST_OK : SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
    }

    // The curses are re-applied by the Bloodmage, not by whoever cursed the ally: the tooltip has the
    // Bloodmage placing them. Each one keeps the remaining duration and stack count it had on the ally.
    void Place(SpellEffIndex)
    {
        Player* player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
        Unit* target = GetHitUnit();
        if (!player || !target)
            return;
        auto itr = siphonedCurses.find(player->GetGUID());
        if (itr == siphonedCurses.end())
            return;
        for (SiphonedCurse const& curse : itr->second)
            if (Aura* aura = player->AddAura(curse.spellId, target))
            {
                if (curse.stacks > 1)
                    aura->SetStackAmount(curse.stacks);
                if (curse.duration > 0)
                    aura->SetDuration(curse.duration);
            }
        siphonedCurses.erase(itr);
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_ascension_bloodmage_excision::CheckWindow);
        OnEffectHitTarget += SpellEffectFn(spell_ascension_bloodmage_excision::Place,
            EFFECT_0, SPELL_EFFECT_DUMMY);
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
    RegisterSpellScript(spell_ascension_bloodmage_blood_craving);
    RegisterSpellScript(aura_ascension_bloodmage_thirst_for_blood);
    RegisterSpellScript(spell_ascension_dark_essence);
    RegisterSpellScript(aura_ascension_waves_of_blood);
    RegisterSpellScript(spell_ascension_bloodmage_hemal_excision);
    RegisterSpellScript(aura_ascension_bloodmage_hemal_excision);
    RegisterSpellScript(spell_ascension_bloodmage_excision);
}
