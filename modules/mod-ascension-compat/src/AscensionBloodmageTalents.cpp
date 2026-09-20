/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionPooledVitality.h"
#include "DBCStores.h"
#include "Log.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellScript.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "TemporarySummon.h"
#include <algorithm>
#include <utility>
#include <limits>
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
    SPELL_BLOODMOON_POWER = 801961,
    SPELL_ETERNAL_CURSE = 800157,
    SPELL_ETERNAL_CURSE_ARMOR = 804320,
    SPELL_BLOOD_SHIELD = 504296,
    SPELL_COAGULATION_DISPEL = 504102,
    SPELL_DARK_MARK = 705731,
    SPELL_DARK_MARK_AURA = 707375,
    SPELL_TERRORIZER = 806210,
    SPELL_ENDURING = 300585,
    SPELL_ADRENALINE_BOOST = 680675,
    SPELL_BLOOD_PLAGUE = 575335,
    SPELL_APPETITE_FOR_BLOOD = 560479,
    SPELL_DARK_SIGIL = 560535,
    SPELL_BLOODLORDS_CURSE = 707449,
    SPELL_BLOOD_MOON = 707623,
    SPELL_BLOOD_MOON_HEAL = 572786,
    SPELL_CURSED_BLOOD = 707435,
    SPELL_CURSED_BLOOD_RUPTURE = 707708,
    SPELL_BLOODSURGE = 553267,
    SPELL_BLOODCHASER = 523721,
    SPELL_BLOOD_BOND_REWARD = 505325,
    SPELL_CURSED_GROUND = 561195,
    SPELL_CURSED_GROUND_BLEED = 561303,
    SPELL_GORE_TOME = 807788,
    SPELL_GORE_TOME_WINDOW = 808014,
    SPELL_ONE_MANS_CURSE = 680661,
    SPELL_ONE_MANS_CURSE_HEAL = 680662,
    SPELL_BLOOD_CONSTRUCTOR = 561196,
    SPELL_THIRST = 706613,
    SPELL_THIRST_ANIMATED_BLOOD = 300796,
    SPELL_CRIMSON_EXPEDITION = 523727,
    SPELL_SANGUINE_SCION = 807292,
    SPELL_BLOOD_RUNS_COLD = 560257
};

// SpellFamilyFlags word 1 bit 17, the eight Bloodbolt records (578304, 578305, 804685, 806928-806932)
// and no other family-26 row.
constexpr uint32 BloodboltClassMask1 = 0x00020000;

// The private selectors the client ships on aura 112. SpellAuraDefines.h states that raw 20000-series
// records are not implicitly enabled; they are named here only to recognize the shipped shapes.
enum AscensionRawCombatSelector : int32
{
    RAW_MASKED_CRIT = 20000,
    RAW_MASKED_CRIT_DAMAGE = 20001,
    RAW_CREATURE_DAMAGE = 20014
};

// Every creature Animated Blood can leave behind: worms, parasites and the rank 3 amalgam.
constexpr uint32 AnimatedBloodSummons[] = {325301, 335301, 315301};

// Every shape the Bloodmage's Cursed Form can take: Blood Curse and the spells that replace it.
// Final Embrace (524865) belongs here too: Spell.dbc describes it as "a dramatically improved Cursed
// Form" and the Crimson Maw it advertises carries CasterAuraSpell 524861, one of the markers below.
constexpr uint32 CursedForms[] = {562572, 562720, 680692, 800157, 801076, 524865};

bool IsCursedForm(uint32 id)
{
    return std::find(std::begin(CursedForms), std::end(CursedForms), id) != std::end(CursedForms);
}

constexpr uint8 CursedFormWeaponSlots[] = {EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_OFFHAND, EQUIPMENT_SLOT_RANGED};

bool HasCursedForm(Player const* player, Aura const* ignored = nullptr)
{
    for (uint32 form : CursedForms)
        if (Aura const* aura = player->GetAura(form, player->GetGUID()); aura && aura != ignored)
            return true;
    return false;
}

void ClearVisibleWeapon(Player* player, uint8 slot)
{
    player->SetUInt32Value(PLAYER_VISIBLE_ITEM_1_ENTRYID + slot * 2, 0);
    player->SetUInt32Value(PLAYER_VISIBLE_ITEM_1_ENCHANTMENT + slot * 2, 0);
}

// The worgen model of Cursed Form fights with claws, so the weapons are only hidden, not disarmed.
void UpdateCursedFormWeapons(Player* player, bool hidden)
{
    for (uint8 slot : CursedFormWeaponSlots)
        if (hidden)
            ClearVisibleWeapon(player, slot);
        else
            player->SetVisibleItemSlot(slot, player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot));
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
    bool active = HasCursedForm(player);

    for (uint32 marker : {uint32(SPELL_CURSED_FORM_REQUIREMENT), uint32(SPELL_CURSED_FORM_REQUIREMENT_2),
        uint32(AscensionBloodmage::CursedForm)})
    {
        if (!active)
            player->RemoveAurasDueToSpell(marker, player->GetGUID());
        else if (player->IsInWorld() && player->IsAlive() && !player->HasAura(marker, player->GetGUID()))
            player->CastSpell(player, marker, true);
    }
}

// Reviewed conversions of the private Ascension selectors these five talents ship on aura 112.
// SpellAuraDefines.h states that raw 20000-series records are not implicitly enabled, so each record is
// checked against its shipped shape and then rewritten in place. No amount, class mask, aura state or
// creature-type mask is redeclared here: every number stays the one Spell.dbc carries.
void ApplyBloodmageConditionalContracts(SpellInfo* info)
{
    // Terrorizer and Enduring carry the native aura-303 contract mirrored: the 20007 selector sits in
    // MiscValue and the aura state in MiscValueB, while Unit.cpp:8831 and Unit.cpp:10741 read the
    // opposite arrangement. Appetite for Blood does the same against the creature-type bonus.
    auto scoped = [info](uint8 index, int32 raw, AuraType aura)
    {
        SpellEffectInfo& effect = info->Effects[index];
        if (effect.Effect != SPELL_EFFECT_APPLY_AURA ||
            effect.ApplyAuraName != SPELL_AURA_OVERRIDE_CLASS_SCRIPTS || effect.MiscValue != raw ||
            effect.MiscValueB <= 0 || !effect.SpellClassMask)
        {
            LOG_ERROR("module.ascension_compat", "Skipped unexpected Bloodmage scoped damage record {}",
                info->Id);
            return;
        }
        effect.ApplyAuraName = aura;
        std::swap(effect.MiscValue, effect.MiscValueB);
    };

    // Adrenaline Boost and Blood Plague ship the raw forms of the reviewed selectors that
    // Unit::GetAscensionConditionalCombatModifier switches on; only the selector changes.
    auto conditional = [info](uint8 index, int32 raw, int32 selector)
    {
        SpellEffectInfo& effect = info->Effects[index];
        if (effect.Effect != SPELL_EFFECT_APPLY_AURA ||
            effect.ApplyAuraName != SPELL_AURA_OVERRIDE_CLASS_SCRIPTS || effect.MiscValue != raw ||
            effect.MiscValueB <= 0 || !effect.SpellClassMask)
        {
            LOG_ERROR("module.ascension_compat", "Skipped unexpected Bloodmage conditional record {}",
                info->Id);
            return;
        }
        effect.MiscValue = selector;
    };

    switch (info->Id)
    {
        case SPELL_TERRORIZER:
        case SPELL_ENDURING:
            // "on enemies below 35% health" / "while below 35% health": MiscValueB is aura state 13.
            scoped(EFFECT_0, ASCENSION_CLASSMASK_AURASTATE_DAMAGE, SPELL_AURA_MOD_DAMAGE_DONE_VERSUS_AURASTATE);
            break;
        case SPELL_APPETITE_FOR_BLOOD:
            // "against Humanoids and Beasts": MiscValueB is the creature-type mask 65.
            scoped(EFFECT_0, RAW_CREATURE_DAMAGE, SPELL_AURA_MOD_DAMAGE_DONE_VERSUS);
            break;
        case SPELL_ADRENALINE_BOOST:
            // "on enemies above 75% or below 35% health": two masked crit-chance effects, states 23 and 13.
            conditional(EFFECT_0, RAW_MASKED_CRIT, ASCENSION_STATE_MASKED_CRIT);
            conditional(EFFECT_2, RAW_MASKED_CRIT, ASCENSION_STATE_MASKED_CRIT);
            break;
        case SPELL_BLOOD_PLAGUE:
            // "critical strike chance and critical damage ... against Diseased targets", condition 31.
            conditional(EFFECT_0, RAW_MASKED_CRIT, ASCENSION_STATE_MASKED_CRIT);
            conditional(EFFECT_1, RAW_MASKED_CRIT_DAMAGE, ASCENSION_STATE_MASKED_CRIT_DAMAGE);
            break;
        default:
            break;
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
        if (IsCursedForm(aura->GetId()))
            UpdateCursedFormWeapons(player, true);
        // "Armor contribution from items" is a hidden passive (804320) that nothing ever applied.
        if (aura->GetId() == SPELL_ETERNAL_CURSE)
            player->CastSpell(player, SPELL_ETERNAL_CURSE_ARMOR, true);
        // Dark Mark's whole payload lives in 707375, the area aura that carries the radius and the -3%
        // hit chance; no Spell.dbc row ever applies it, so the talent's marker applies it directly.
        if (aura->GetId() == SPELL_DARK_MARK)
            player->CastSpell(player, SPELL_DARK_MARK_AURA, true);
        // Gore Tome (807788): "For the first 5 sec after activating a Cursed Form, your Bloodfang Bite is
        // now guaranteed to critically strike." The talent's own effect cannot say that - it is aura 108
        // SPELLMOD_VALUE_MULTIPLIER on Bloodbolt's class mask - while a second "Gore Tome" record, 808014,
        // carries exactly the promised contract: aura 107, MiscValue 7 (SPELLMOD_CRITICAL_CHANCE), +100,
        // EffectSpellClassMask (0, 8388608, 0) = Bloodfang Bite, DurationIndex 28 = 5000 ms. No Spell.dbc
        // row triggers 808014 and no acquisition route reaches it, so the window is opened here; its own
        // five-second duration ends it.
        if (IsCursedForm(aura->GetId()) && aura->GetCasterGUID() == player->GetGUID() &&
            player->HasAura(SPELL_GORE_TOME, player->GetGUID()))
            player->CastSpell(player, SPELL_GORE_TOME_WINDOW, true);
        // One Man's Curse (680661): "Removes the health cost of Accursed Form and entering it now instantly
        // restores $680662s1% of your maximum health." The cost half is native (aura 108 SPELLMOD_COST
        // -100% over Accursed Form's class mask, applied after CalcPowerCost's POWER_HEALTH branch). The
        // restore half lives in 680662, a SPELL_EFFECT_HEAL_PCT of 15% that nothing ever cast.
        if (aura->GetId() == SPELL_ACCURSED_FORM && aura->GetCasterGUID() == player->GetGUID() &&
            player->HasAura(SPELL_ONE_MANS_CURSE, player->GetGUID()))
            player->CastSpell(player, SPELL_ONE_MANS_CURSE_HEAL, true);
        // Bloodsurge (553267): "At the end of the duration, or whenever you trigger Accursed Form, gain
        // Bloodchaser instead." Its effect 1 is SPELL_AURA_PROC_TRIGGER_SPELL on Bloodchaser 523721, but
        // Spell.dbc gives 553267 ProcFlags 0 and no `spell_proc` row can express either trigger anyway:
        // an aura reaching the end of its duration is not a proc event, and "whenever you trigger Accursed
        // Form" is an aura application, not a damage or cast event. Both halves are handled here instead.
        // Effects 0 (aura 79, Shadow damage +30%) and 2 (aura 216, spell haste +30%) are native and
        // untouched. "Instead" is why the buff is removed rather than left running alongside Bloodchaser.
        if (aura->GetId() == SPELL_ACCURSED_FORM && aura->GetCasterGUID() == player->GetGUID() &&
            player->HasAura(SPELL_BLOODSURGE, player->GetGUID()))
        {
            player->RemoveAurasDueToSpell(SPELL_BLOODSURGE, player->GetGUID());
            player->CastSpell(player, SPELL_BLOODCHASER, true);
        }
    }

    void OnAuraRemove(Unit* unit, AuraApplication* application, AuraRemoveMode mode) override
    {
        Player* player = unit ? unit->ToPlayer() : nullptr;
        if (!player || player->getClass() != CLASS_SON_OF_ARUGAL || !application)
            return;
        Aura* aura = application->GetBase();
        if (IsCursedForm(aura->GetId()))
            SyncCursedFormRequirement(player);
        if (IsCursedForm(aura->GetId()) && !HasCursedForm(player, aura))
            UpdateCursedFormWeapons(player, false);
        if (aura->GetId() == SPELL_ETERNAL_CURSE)
            player->RemoveAurasDueToSpell(SPELL_ETERNAL_CURSE_ARMOR);
        if (aura->GetId() == SPELL_DARK_MARK)
            player->RemoveAurasDueToSpell(SPELL_DARK_MARK_AURA, player->GetGUID());
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
        // The other half of Bloodsurge's transition clause: the buff running out on its own. Only a real
        // expiry counts - a dispel, a cancel or the Accursed Form replacement above all report a different
        // AuraRemoveMode, and the replacement casts Bloodchaser itself.
        if (aura->GetId() == SPELL_BLOODSURGE && aura->GetCasterGUID() == player->GetGUID() &&
            mode == AURA_REMOVE_BY_EXPIRE)
            player->CastSpell(player, SPELL_BLOODCHASER, true);
    }
};

// Passive shapeshift auras survive Unit::RemoveAllAurasOnDeath, so a Bloodmage who died in a Cursed Form
// (Eternal Curse) stayed shapeshifted as a ghost, and dropping the form then killed the ghost again.
class bloodmage_cursed_form_death : public PlayerScript
{
public:
    bloodmage_cursed_form_death() : PlayerScript("bloodmage_cursed_form_death", {PLAYERHOOK_ON_PLAYER_JUST_DIED}) { }

    void OnPlayerJustDied(Player* player) override
    {
        if (!player || player->getClass() != CLASS_SON_OF_ARUGAL)
            return;
        for (uint32 form : CursedForms)
            player->RemoveAurasDueToSpell(form);
        SyncCursedFormRequirement(player);
    }
};

// Equipping or swapping a weapon rewrites the visible item fields, which would show it again.
class bloodmage_cursed_form_weapons : public PlayerScript
{
public:
    bloodmage_cursed_form_weapons() : PlayerScript("bloodmage_cursed_form_weapons",
        {PLAYERHOOK_ON_AFTER_SET_VISIBLE_ITEM_SLOT}) { }

    void OnPlayerAfterSetVisibleItemSlot(Player* player, uint8 slot, Item* /*item*/) override
    {
        if (!player || player->getClass() != CLASS_SON_OF_ARUGAL)
            return;
        if (std::find(std::begin(CursedFormWeaponSlots), std::end(CursedFormWeaponSlots), slot) ==
            std::end(CursedFormWeaponSlots) || !HasCursedForm(player))
            return;
        ClearVisibleWeapon(player, slot);
    }
};

// Blood Constructor (561196): "Increases the duration of Animated Blood by $/1000;s1 sec per stack of
// Thirst." Its one effect is aura 107 with MiscValue 3 (SPELLMOD_EFFECT1) and BasePoints 999 + DieSides 1
// = +1000, over EffectSpellClassMask (0, 64, 0), which selects Sated 532125 and "Thirst Val/Kal Cast/Anim
// Blood" 300796. 300796 is the intended target: its own effect 0 is aura 107 with MiscValue 1
// (SPELLMOD_DURATION) and amount 0, over EffectSpellClassMask (1, 0, 0) = the three Animated Blood records
// (573299, 573356, 573357) - the per-stack duration the tooltip names, which this talent raises from 0 to
// 1000 ms. Nothing applies 300796: no Spell.dbc row references it in any field and no acquisition route
// reaches it, so the inner modifier was never installed and Animated Blood never lasted any longer.
// The amount is read off the record here, so every number stays Spell.dbc's own, and 300796 is left
// unapplied, which keeps Dissipation's SPELLMOD_DURATION -60000 over the same word-1 bit 6 - which would
// clamp the helper's own 20 s duration to 0 - out of the picture.
// Spell::EffectSummonType applies SPELLMOD_DURATION to the summon lifetime, so this hook is the same
// quantity the modifier would have reached, taken at the moment the amalgam is created.
class bloodmage_blood_constructor : public PlayerScript
{
public:
    bloodmage_blood_constructor() : PlayerScript("bloodmage_blood_constructor",
        {PLAYERHOOK_ON_BEFORE_TEMP_SUMMON_INIT_STATS}) { }

    void OnPlayerBeforeTempSummonInitStats(Player* player, TempSummon* summon, uint32& duration) override
    {
        if (!player || player->getClass() != CLASS_SON_OF_ARUGAL || !summon || !duration)
            return;
        if (std::find(std::begin(AnimatedBloodSummons), std::end(AnimatedBloodSummons), summon->GetEntry())
            == std::end(AnimatedBloodSummons))
            return;
        if (!player->HasAura(SPELL_BLOOD_CONSTRUCTOR, player->GetGUID()))
            return;
        Aura const* thirst = player->GetAura(SPELL_THIRST, player->GetGUID());
        SpellInfo const* helper = sSpellMgr->GetSpellInfo(SPELL_THIRST_ANIMATED_BLOOD);
        if (!thirst || !helper)
            return;
        int32 const perStack = helper->Effects[EFFECT_0].CalcValue(player);
        if (perStack > 0)
            duration += uint32(perStack) * thirst->GetStackAmount();
    }
};

// Spell.dbc gives Dark Sigil, Blood Moon and Cursed Blood an aura of type 354 that names a TriggerSpell.
// AuraEffectHandler[354] is nullptr, AuraEffect::HandleProc has no case for it and isTriggerAura[354] is
// never set, so the core can neither apply nor proc those effects on its own: each one needs a `spell_proc`
// row plus an OnEffectProc script, the way Volcanic Blast and Hammer of Life already do.
bool IsBloodmageDamageProc(Unit* player, Unit* caster, ProcEventInfo& event)
{
    Unit* victim = event.GetActionTarget();
    DamageInfo const* damage = event.GetDamageInfo();
    return player->IsPlayer() && player->getClass() == CLASS_SON_OF_ARUGAL && player->IsAlive() &&
        caster == player && event.GetActor() == player && victim && victim != player &&
        !player->IsFriendlyTo(victim) && damage && damage->GetDamage();
}

// Each of the three trigger spells ships with BasePoints 0, so the share of the proccing damage that the
// aura-354 effect's own amount names has to be handed over as the trigger's base point.
int32 BloodmageProcShare(AuraEffect const* effect, ProcEventInfo& event)
{
    uint64 share = uint64(event.GetDamageInfo()->GetDamage()) * std::clamp(effect->GetAmount(), 0, 100) / 100;
    return int32(std::min<uint64>(share, std::numeric_limits<int32>::max()));
}

// Dark Sigil (560535): "you now heal for $s2% of all critical damage dealt." Effect 1 is the aura 354 that
// carries that 15% ($s2, BasePoints 14 + DieSides 1) and names Bloodlord's Curse 707449, a plain
// SPELL_EFFECT_HEAL on the caster. Effect 0, the raid critical-strike aura, works natively and is kept out
// of the proc by the row's DisableEffectsMask.
class aura_ascension_bloodmage_dark_sigil : public AuraScript
{
    PrepareAuraScript(aura_ascension_bloodmage_dark_sigil);

    bool Check(ProcEventInfo& event)
    {
        return IsBloodmageDamageProc(GetTarget(), GetCaster(), event) && (event.GetHitMask() & PROC_HIT_CRITICAL);
    }

    void Proc(AuraEffect const* effect, ProcEventInfo& event)
    {
        PreventDefaultAction();
        if (int32 heal = BloodmageProcShare(effect, event))
            GetTarget()->CastCustomSpell(SPELL_BLOODLORDS_CURSE, SPELLVALUE_BASE_POINT0, heal, GetTarget(),
                TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_bloodmage_dark_sigil::Check);
        OnEffectProc += AuraEffectProcFn(aura_ascension_bloodmage_dark_sigil::Proc, EFFECT_1, AuraType(354));
    }
};

// Blood Moon (707623): "While below 75% health, you now heal for $s1% of all damage you deal." $s1 is the
// aura 354's own 3% (BasePoints 2 + DieSides 1) and the trigger is Blood Moon 572786, a self heal. The 75%
// threshold exists only in that description text - no Spell.dbc field of 707623 carries it, CasterAuraState
// included - so it is stated here as the record's own literal.
constexpr float BloodMoonHealthThreshold = 75.0f;

class aura_ascension_bloodmage_blood_moon : public AuraScript
{
    PrepareAuraScript(aura_ascension_bloodmage_blood_moon);

    bool Check(ProcEventInfo& event)
    {
        return IsBloodmageDamageProc(GetTarget(), GetCaster(), event) &&
            GetTarget()->GetHealthPct() < BloodMoonHealthThreshold;
    }

    void Proc(AuraEffect const* effect, ProcEventInfo& event)
    {
        PreventDefaultAction();
        if (int32 heal = BloodmageProcShare(effect, event))
            GetTarget()->CastCustomSpell(SPELL_BLOOD_MOON_HEAL, SPELLVALUE_BASE_POINT0, heal, GetTarget(),
                TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_bloodmage_blood_moon::Check);
        OnEffectProc += AuraEffectProcFn(aura_ascension_bloodmage_blood_moon::Proc, EFFECT_0, AuraType(354));
    }
};

// Cursed Blood (707435): Taldaram's Torment's damage "has a 20% chance to internally rupture, causing it to
// deal its damage an additional time to all enemies near the target." The aura 354's amount is 100
// (BasePoints 99 + DieSides 1), i.e. the whole tick, and its trigger Cursed Blood 707708 is a
// SPELL_EFFECT_SCHOOL_DAMAGE on the target plus everything within its own five-yard destination area. The
// 20% is the record's ProcChance and belongs in the `spell_proc` row, not here.
class aura_ascension_bloodmage_cursed_blood : public AuraScript
{
    PrepareAuraScript(aura_ascension_bloodmage_cursed_blood);

    bool Check(ProcEventInfo& event)
    {
        return IsBloodmageDamageProc(GetTarget(), GetCaster(), event);
    }

    void Proc(AuraEffect const* effect, ProcEventInfo& event)
    {
        PreventDefaultAction();
        if (int32 damage = BloodmageProcShare(effect, event))
            GetTarget()->CastCustomSpell(SPELL_CURSED_BLOOD_RUPTURE, SPELLVALUE_BASE_POINT0, damage,
                event.GetActionTarget(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_bloodmage_cursed_blood::Check);
        OnEffectProc += AuraEffectProcFn(aura_ascension_bloodmage_cursed_blood::Proc, EFFECT_0, AuraType(354));
    }
};

// Crimson Feast's cooldown reduction is native (its 5 s tick casts 804603, a
// SPELL_EFFECT_ASCENSION_MODIFY_COOLDOWN of -5000 ms on Bloodsurge), but the passive has Stances 0 and an
// infinite duration, so nothing expressed "every 5 second you remain in a Cursed Form" and Bloodsurge
// recovered at twice the normal rate permanently, out of combat and out of form.
class aura_ascension_bloodmage_crimson_feast : public AuraScript
{
    PrepareAuraScript(aura_ascension_bloodmage_crimson_feast);

    void Tick(AuraEffect const*)
    {
        Player* player = GetTarget() ? GetTarget()->ToPlayer() : nullptr;
        if (!player || player->getClass() != CLASS_SON_OF_ARUGAL || !HasCursedForm(player))
            PreventDefaultAction();
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(aura_ascension_bloodmage_crimson_feast::Tick,
            EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

// Coagulation is scoped to Blood Shield ("Your Blood Shield now also dispels ..."), but the passive is
// permanent and its tick was unconditional, so bleeds came off every 5 seconds with or without the shield.
class aura_ascension_bloodmage_coagulation : public AuraScript
{
    PrepareAuraScript(aura_ascension_bloodmage_coagulation);

    void Tick(AuraEffect const*)
    {
        Unit* target = GetTarget();
        if (!target || !target->HasAura(SPELL_BLOOD_SHIELD, target->GetGUID()))
            PreventDefaultAction();
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(aura_ascension_bloodmage_coagulation::Tick,
            EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

// Forbidden Power's third clause ("you now gain Spell Penetration equal to your Armor Penetration Rating")
// ships as a SPELL_AURA_MOD_TARGET_RESISTANCE helper with base points of zero, refreshed every 3 s so that
// the amount can track the rating. Nothing computed it. Spell penetration is negative on this aura:
// Unit::CalcAbsorbResist adds the modifier to the victim's resistance.
class aura_ascension_bloodmage_forbidden_power : public AuraScript
{
    PrepareAuraScript(aura_ascension_bloodmage_forbidden_power);

    void Calculate(AuraEffect const*, int32& amount, bool&)
    {
        amount = 0;
        Player* player = GetUnitOwner() ? GetUnitOwner()->ToPlayer() : nullptr;
        if (!player || player->getClass() != CLASS_SON_OF_ARUGAL)
            return;
        uint32 rating = player->GetUInt32Value(
            static_cast<uint16>(PLAYER_FIELD_COMBAT_RATING_1) + static_cast<uint16>(CR_ARMOR_PENETRATION));
        amount = -int32(std::min<uint32>(rating, uint32(std::numeric_limits<int32>::max())));
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(aura_ascension_bloodmage_forbidden_power::Calculate,
            EFFECT_0, SPELL_AURA_MOD_TARGET_RESISTANCE);
    }
};

// Essence Harvester (806421): "Being struck by an enemy while below 35% health instantly resets the
// cooldown of your Vampiric Fang and grants you Wretched. Can only occur once per minute."
// Its single effect is SPELL_AURA_PROC_TRIGGER_SPELL on Wretched 504284, which is itself complete: aura 23
// SPELL_AURA_PERIODIC_TRIGGER_SPELL on 504283 every 1000 ms plus SPELL_EFFECT_ASCENSION_RESET_COOLDOWN
// (195) with MiscValue 804726 = Vampiric Fang and MiscValueB 1, so the cooldown reset needs no code.
// The taken-damage event and the once-per-minute limit are the `spell_proc` row's ProcFlags and Cooldown.
// The health gate has no `spell_proc` column at all, which is the only reason this script exists.
constexpr float EssenceHarvesterHealthThreshold = 35.0f;

class aura_ascension_bloodmage_essence_harvester : public AuraScript
{
    PrepareAuraScript(aura_ascension_bloodmage_essence_harvester);

    bool Check(ProcEventInfo& event)
    {
        Unit* target = GetTarget();
        Unit* attacker = event.GetActor();
        return target && target->IsPlayer() && target->IsAlive() && attacker && attacker != target &&
            !target->IsFriendlyTo(attacker) && target->GetHealthPct() < EssenceHarvesterHealthThreshold;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_bloodmage_essence_harvester::Check);
    }
};

// Blood Bond (504627): "causing direct damage taken by the ally to generate $/10;505325s1 Rage and
// ${$505325m2+$505325ppl2}% base health". Effect 0 is SPELL_AURA_PROC_TRIGGER_SPELL on Blood Bond 505325
// (SPELL_EFFECT_ENERGIZE of 9/10 Rage plus SPELL_EFFECT_ASCENSION_RESTORE_BASE_HEALTH_PCT of 2%, both
// TargetA 1 = caster), but effect 0's TargetA is 57, so the aura sits on the bonded ally. The default proc
// path uses aurApp->GetTarget() as the trigger caster, which would hand the Rage and the health to the
// ally instead of the Bloodmage the tooltip names; that redirection is why this script replaces it.
// Effect 1 (aura 23 on 505169, the out-of-combat +30% run speed) is native and untouched.
// The tooltip's third clause, "redirect $s3% of their threat generated to you", has no effect slot at all -
// 504627 has two effects - so it is not implemented here and is recorded as a separate finding.
class aura_ascension_bloodmage_blood_bond : public AuraScript
{
    PrepareAuraScript(aura_ascension_bloodmage_blood_bond);

    bool Validate(SpellInfo const*) override { return ValidateSpellInfo({SPELL_BLOOD_BOND_REWARD}); }

    bool Check(ProcEventInfo& event)
    {
        Unit* owner = GetCaster();
        DamageInfo const* damage = event.GetDamageInfo();
        return owner && owner->IsPlayer() && owner->IsAlive() && owner->IsInWorld() && GetTarget() &&
            GetTarget() != owner && damage && damage->GetDamage();
    }

    void Proc(AuraEffect const*, ProcEventInfo&)
    {
        PreventDefaultAction();
        if (Unit* owner = GetCaster())
            owner->CastSpell(owner, SPELL_BLOOD_BOND_REWARD, true);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_bloodmage_blood_bond::Check);
        OnEffectProc += AuraEffectProcFn(aura_ascension_bloodmage_blood_bond::Proc, EFFECT_0,
            SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

// Cursed Ground (561195): "Increases the cast time of Sanguine Rupture by $/1000;S1 sec, but now when you
// strike at least 5 enemies it causes them to bleed out, dealing an additional $561303o1 Shadow damage
// over $561303d." Effect 0 (aura 107 SPELLMOD_CASTING_TIME, BasePoints 1999, EffectSpellClassMask
// (0, 0, 2147483648)) is native and already works. Effect 1 is SPELL_AURA_PROC_TRIGGER_SPELL on Cursed
// Ground 561205, whose own single effect is an aura 4 dummy with no handler and no `spell_script_names`
// row - it carries nothing but the client tooltip, which names the real bleed, Cursed Ground 561303
// (aura 3 SPELL_AURA_PERIODIC_DAMAGE, BasePoints 86, RealPointsPerLevel 1.02, Amplitude 500, Mechanic 15,
// TargetA 6 = enemy). "At least 5 enemies in one cast" is not expressible in any `spell_proc` column, so
// the count is taken here. Spell::HandleLaunchPhase runs DoAllEffectOnLaunchTarget - and therefore this
// hook - for every target before the first target is hit, so the total is complete by hit time, the same
// ordering AscensionBloodmageSecondary.cpp already relies on for Reave.
// The ranks are selected by SpellFamilyFlags word 2 bit 31, exactly the class mask Cursed Ground's own
// native effect uses; the two odd Sanguine Rupture records 800775 and 802496 carry (0, 2097152, 0) and are
// outside that mask, so they are outside this clause too, just as they are outside the cast-time half.
constexpr uint64 CursedGroundTargetThreshold = 5;

Player* CursedGroundCaster(Spell* spell)
{
    Unit* caster = spell ? spell->GetCaster() : nullptr;
    Player* player = caster ? caster->ToPlayer() : nullptr;
    if (!player || player->getClass() != CLASS_SON_OF_ARUGAL || spell->IsTriggered())
        return nullptr;
    SpellInfo const* info = spell->GetSpellInfo();
    if (!info || info->SpellFamilyName != 26 || !(info->SpellFamilyFlags[2] & 0x80000000u))
        return nullptr;
    return player->HasAura(SPELL_CURSED_GROUND, player->GetGUID()) ? player : nullptr;
}

class bloodmage_cursed_ground : public AllSpellScript
{
public:
    bloodmage_cursed_ground() : AllSpellScript("bloodmage_cursed_ground",
        {ALLSPELLHOOK_ON_CALCULATED_TARGET, ALLSPELLHOOK_ON_HIT_RESULT}) { }

    void OnSpellCalculatedTarget(Spell* spell, Unit* target, TargetInfo& hit) override
    {
        Player* player = CursedGroundCaster(spell);
        if (!player || !target || target == player || hit.missCondition != SPELL_MISS_NONE ||
            player->IsFriendlyTo(target))
            return;
        // One entry per unit in the cast's target list, so a plain counter is already distinct.
        spell->SetScriptValue(SPELL_CURSED_GROUND, spell->GetScriptValue(SPELL_CURSED_GROUND) + 1);
    }

    void OnSpellHitResult(Spell* spell, Unit* target, uint8 miss, uint32, uint32, bool) override
    {
        Player* player = CursedGroundCaster(spell);
        if (!player || !target || miss != SPELL_MISS_NONE || target == player ||
            player->IsFriendlyTo(target) ||
            spell->GetScriptValue(SPELL_CURSED_GROUND) < CursedGroundTargetThreshold)
            return;
        // Sanguine Rupture's own effects are SPELL_EFFECT_TRIGGER_SPELL_WITH_VALUE (142) on 800775 and
        // SPELL_EFFECT_TRIGGER_SPELL (64) on 561304, so the parent cast reports zero damage per target;
        // the landed hit, not a damage amount, is what "strike" means here.
        player->CastSpell(target, SPELL_CURSED_GROUND_BLEED, true);
    }
};

class bloodmage_talent_contracts : public GlobalScript
{
public:
    bloodmage_talent_contracts() : GlobalScript("bloodmage_talent_contracts",
        {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* info) override
    {
        if (!info || info->SpellFamilyName != 26)
            return;

        ApplyBloodmageConditionalContracts(info);

        // Crimson Expedition (523727): "Increases the critical strike chance of your Bloodbolt, Blood
        // Shards, and Aortic Assault by $s1%." Its one effect is aura 107 with MiscValue 7
        // (SPELLMOD_CRITICAL_CHANCE) and +10 over EffectSpellClassMask (0, 4194306, 0), i.e. word 1 bit 22
        // (Aortic Assault 806502) and word 1 bit 1 (Blood Shard 504115). Bloodbolt is word 1 bit 17 and is
        // absent from the mask, so the first spell the tooltip names received nothing.
        // Sanguine Scion (807292): "Increases the critical strike chance of Sanguine Mend and Bloodbolt by
        // $s1%", same operation and amount over (524288, 8192, 0) = Sanguine Mend plus Bloodmoon Blast -
        // the same omission. Only the missing bit is added; no amount, operation or other bit changes.
        if ((info->Id == SPELL_CRIMSON_EXPEDITION || info->Id == SPELL_SANGUINE_SCION) &&
            info->Effects[EFFECT_0].ApplyAuraName == SPELL_AURA_ADD_FLAT_MODIFIER &&
            info->Effects[EFFECT_0].MiscValue == SPELLMOD_CRITICAL_CHANCE)
            info->Effects[EFFECT_0].SpellClassMask[1] |= BloodboltClassMask1;

        // Blood Runs Cold (560257): "Your Blood Pact now also increases your healing received by $s1%."
        // Its one effect is aura 107 with +20 over EffectSpellClassMask (0, 0, 65536) = Blood Pact
        // (801955, 801964), but MiscValue 3 is SPELLMOD_EFFECT1, which Unit::ApplyEffectModifiers maps to
        // effect index 0 - Blood Pact's aura 185 MOD_ATTACKER_RANGED_HIT_CHANCE of -50, which the talent
        // silently weakened to -30 while the healing clause stayed empty. The placeholder the tooltip
        // fills is index 1: aura 118 SPELL_AURA_MOD_HEALING_PCT, MiscValue 127 (all schools), amount 0,
        // and Blood Pact's own text carries the matching conditional "$?s560257[, and increasing your
        // healing received by $560257s1%][]". SPELLMOD_EFFECT2 is the operation that reaches index 1.
        if (info->Id == SPELL_BLOOD_RUNS_COLD &&
            info->Effects[EFFECT_0].ApplyAuraName == SPELL_AURA_ADD_FLAT_MODIFIER &&
            info->Effects[EFFECT_0].MiscValue == SPELLMOD_EFFECT1)
            info->Effects[EFFECT_0].MiscValue = SPELLMOD_EFFECT2;

        // Coagulation promises "1 bleed effect" a tick. Spell::EffectDispelMechanic stops after the first
        // match only when the effect's raw BasePoints are exactly 1, and Spell.dbc ships 0, so the whole
        // dispel list came off at once. SPELL_EFFECT_DISPEL_MECHANIC reads no amount, so the field is free.
        if (info->Id == SPELL_COAGULATION_DISPEL &&
            info->Effects[EFFECT_0].Effect == SPELL_EFFECT_DISPEL_MECHANIC &&
            info->Effects[EFFECT_0].MiscValue == int32(MECHANIC_BLEED))
            info->Effects[EFFECT_0].BasePoints = 1;

        if (info->Id != SPELL_VAMPIRIC_POOLS_LEECH ||
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
    new bloodmage_cursed_form_death();
    new bloodmage_cursed_form_weapons();
    new bloodmage_blood_constructor();
    new bloodmage_talent_contracts();
    RegisterSpellScript(spell_ascension_animated_blood);
    RegisterSpellScript(aura_ascension_bloodmage_crimson_feast);
    RegisterSpellScript(aura_ascension_bloodmage_coagulation);
    RegisterSpellScript(aura_ascension_bloodmage_forbidden_power);
    RegisterSpellScript(aura_ascension_bloodmage_dark_sigil);
    RegisterSpellScript(aura_ascension_bloodmage_blood_moon);
    RegisterSpellScript(aura_ascension_bloodmage_cursed_blood);
    RegisterSpellScript(aura_ascension_bloodmage_essence_harvester);
    RegisterSpellScript(aura_ascension_bloodmage_blood_bond);
    new bloodmage_cursed_ground();
}
