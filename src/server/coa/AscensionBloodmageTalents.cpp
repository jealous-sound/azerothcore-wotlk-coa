/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionPooledVitality.h"
#include "DBCStores.h"
#include "Log.h"
#include "Player.h"
#include "ScriptMgr.h"
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
    SPELL_GORE_TOME = 807788,
    SPELL_GORE_TOME_WINDOW = 808014,
    SPELL_ONE_MANS_CURSE = 680661,
    SPELL_ONE_MANS_CURSE_HEAL = 680662,
    SPELL_BLOOD_CONSTRUCTOR = 561196,
    SPELL_THIRST = 706613,
    SPELL_THIRST_ANIMATED_BLOOD = 300796,
    SPELL_CRIMSON_EXPEDITION = 523727,
    SPELL_SANGUINE_SCION = 807292,
    SPELL_BLOOD_RUNS_COLD = 560257,
    SPELL_CURSED_GROUND = 561195
};

constexpr uint32 SanguineRuptureMinimumTargets = 5;

constexpr uint32 BloodboltClassMask1 = 0x00020000;

enum AscensionRawCombatSelector : int32
{
    RAW_MASKED_CRIT = 20000,
    RAW_MASKED_CRIT_DAMAGE = 20001,
    RAW_CREATURE_DAMAGE = 20014
};

constexpr uint32 AnimatedBloodSummons[] = {325301, 335301, 315301};

constexpr uint32 CursedForms[] = {562572, 562720, 680692, 800157, 801076, 524865};
constexpr uint32 MortalAbilityForms[] = {680692, 801076};

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

bool HasCursedFormRestrictingMortalAbilities(Player const* player)
{
    for (uint32 form : CursedForms)
    {
        if (std::find(std::begin(MortalAbilityForms), std::end(MortalAbilityForms), form) !=
            std::end(MortalAbilityForms))
            continue;
        if (player->HasAura(form, player->GetGUID()))
            return true;
    }
    return false;
}

void ClearVisibleWeapon(Player* player, uint8 slot)
{
    player->SetUInt32Value(PLAYER_VISIBLE_ITEM_1_ENTRYID + slot * 2, 0);
    player->SetUInt32Value(PLAYER_VISIBLE_ITEM_1_ENCHANTMENT + slot * 2, 0);
}

void UpdateCursedFormWeapons(Player* player, bool hidden)
{
    for (uint8 slot : CursedFormWeaponSlots)
        if (hidden)
            ClearVisibleWeapon(player, slot);
        else
            player->SetVisibleItemSlot(slot, player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot));
}

void SyncCursedFormRequirement(Player* player)
{
    bool active = HasCursedForm(player);

    for (uint32 marker : {uint32(SPELL_CURSED_FORM_REQUIREMENT), uint32(SPELL_CURSED_FORM_REQUIREMENT_2)})
    {
        if (!active)
            player->RemoveAurasDueToSpell(marker, player->GetGUID());
        else if (player->IsInWorld() && player->IsAlive() && !player->HasAura(marker, player->GetGUID()))
            player->CastSpell(player, marker, true);
    }

    if (!HasCursedFormRestrictingMortalAbilities(player))
        player->RemoveAurasDueToSpell(AscensionBloodmage::CursedForm, player->GetGUID());
    else if (player->IsInWorld() && player->IsAlive() &&
        !player->HasAura(AscensionBloodmage::CursedForm, player->GetGUID()))
        player->CastSpell(player, AscensionBloodmage::CursedForm, true);
}

void ApplyBloodmageConditionalContracts(SpellInfo* info)
{
    auto scoped = [info](uint8 index, int32 raw, AuraType aura)
    {
        SpellEffectInfo& effect = info->Effects[index];
        if (effect.Effect != SPELL_EFFECT_APPLY_AURA ||
            effect.ApplyAuraName != SPELL_AURA_OVERRIDE_CLASS_SCRIPTS || effect.MiscValue != raw ||
            effect.MiscValueB <= 0 || !effect.SpellClassMask)
        {
            LOG_ERROR("coa", "Skipped unexpected Bloodmage scoped damage record {}",
                info->Id);
            return;
        }
        effect.ApplyAuraName = aura;
        std::swap(effect.MiscValue, effect.MiscValueB);
    };

    auto conditional = [info](uint8 index, int32 raw, int32 selector)
    {
        SpellEffectInfo& effect = info->Effects[index];
        if (effect.Effect != SPELL_EFFECT_APPLY_AURA ||
            effect.ApplyAuraName != SPELL_AURA_OVERRIDE_CLASS_SCRIPTS || effect.MiscValue != raw ||
            effect.MiscValueB <= 0 || !effect.SpellClassMask)
        {
            LOG_ERROR("coa", "Skipped unexpected Bloodmage conditional record {}",
                info->Id);
            return;
        }
        effect.MiscValue = selector;
    };

    switch (info->Id)
    {
        case SPELL_TERRORIZER:
        case SPELL_ENDURING:
            scoped(EFFECT_0, ASCENSION_CLASSMASK_AURASTATE_DAMAGE, SPELL_AURA_MOD_DAMAGE_DONE_VERSUS_AURASTATE);
            break;
        case SPELL_APPETITE_FOR_BLOOD:
            scoped(EFFECT_0, RAW_CREATURE_DAMAGE, SPELL_AURA_MOD_DAMAGE_DONE_VERSUS);
            break;
        case SPELL_ADRENALINE_BOOST:
            conditional(EFFECT_0, RAW_MASKED_CRIT, ASCENSION_STATE_MASKED_CRIT);
            conditional(EFFECT_2, RAW_MASKED_CRIT, ASCENSION_STATE_MASKED_CRIT);
            break;
        case SPELL_BLOOD_PLAGUE:
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
        if (count)
            caster->CastCustomSpell(SPELL_BLOOD_TEAR_SPAWN, SPELLVALUE_BASE_POINT0, count, caster, true);
    }

    void ReplacePreviousBrood()
    {
        if (Unit* caster = GetCaster())
            for (uint32 entry : AnimatedBloodSummons)
                caster->RemoveAllMinionsByEntry(entry);
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_ascension_animated_blood::ReplacePreviousBrood);
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
        if (aura->GetId() == SPELL_ETERNAL_CURSE)
            player->CastSpell(player, SPELL_ETERNAL_CURSE_ARMOR, true);
        if (aura->GetId() == SPELL_DARK_MARK)
            player->CastSpell(player, SPELL_DARK_MARK_AURA, true);
        if (IsCursedForm(aura->GetId()) && aura->GetCasterGUID() == player->GetGUID() &&
            player->HasAura(SPELL_GORE_TOME, player->GetGUID()))
            player->CastSpell(player, SPELL_GORE_TOME_WINDOW, true);
        if (aura->GetId() == SPELL_ACCURSED_FORM && aura->GetCasterGUID() == player->GetGUID() &&
            player->HasAura(SPELL_ONE_MANS_CURSE, player->GetGUID()))
            player->CastSpell(player, SPELL_ONE_MANS_CURSE_HEAL, true);
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
        if (aura->GetId() == SPELL_BLOODSURGE && aura->GetCasterGUID() == player->GetGUID() &&
            mode == AURA_REMOVE_BY_EXPIRE)
            player->CastSpell(player, SPELL_BLOODCHASER, true);
    }
};

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

class bloodmage_cursed_form_weapons : public PlayerScript
{
public:
    bloodmage_cursed_form_weapons() : PlayerScript("bloodmage_cursed_form_weapons",
        {PLAYERHOOK_ON_AFTER_SET_VISIBLE_ITEM_SLOT}) { }

    void OnPlayerAfterSetVisibleItemSlot(Player* player, uint8 slot, Item*) override
    {
        if (!player || player->getClass() != CLASS_SON_OF_ARUGAL)
            return;
        if (std::find(std::begin(CursedFormWeaponSlots), std::end(CursedFormWeaponSlots), slot) ==
            std::end(CursedFormWeaponSlots) || !HasCursedForm(player))
            return;
        ClearVisibleWeapon(player, slot);
    }
};

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

bool IsBloodmageDamageProc(Unit* player, Unit* caster, ProcEventInfo& event)
{
    Unit* victim = event.GetActionTarget();
    DamageInfo const* damage = event.GetDamageInfo();
    return player->IsPlayer() && player->getClass() == CLASS_SON_OF_ARUGAL && player->IsAlive() &&
        caster == player && event.GetActor() == player && victim && victim != player &&
        !player->IsFriendlyTo(victim) && damage && damage->GetDamage();
}

int32 BloodmageProcShare(AuraEffect const* effect, ProcEventInfo& event)
{
    uint64 share = uint64(event.GetDamageInfo()->GetDamage()) * std::clamp(effect->GetAmount(), 0, 100) / 100;
    return int32(std::min<uint64>(share, std::numeric_limits<int32>::max()));
}

class spell_ascension_bloodmage_sanguine_rupture : public SpellScript
{
    PrepareSpellScript(spell_ascension_bloodmage_sanguine_rupture);

    void HandleBleed(SpellEffIndex effIndex)
    {
        bool shouldBleed = GetCaster()->HasAura(SPELL_CURSED_GROUND);
        if (shouldBleed)
        {
            uint32 successfulTargets = 0;
            for (auto const& target : *GetSpell()->GetUniqueTargetInfo())
                if ((target.effectMask & (1 << EFFECT_1)) && target.missCondition == SPELL_MISS_NONE)
                    ++successfulTargets;

            shouldBleed = successfulTargets >= SanguineRuptureMinimumTargets;
        }

        if (!shouldBleed)
            PreventHitDefaultEffect(effIndex);
    }

    void Register() override
    {
        OnEffectLaunchTarget += SpellEffectFn(spell_ascension_bloodmage_sanguine_rupture::HandleBleed,
            EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
    }
};

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

        if ((info->Id == SPELL_CRIMSON_EXPEDITION || info->Id == SPELL_SANGUINE_SCION) &&
            info->Effects[EFFECT_0].ApplyAuraName == SPELL_AURA_ADD_FLAT_MODIFIER &&
            info->Effects[EFFECT_0].MiscValue == SPELLMOD_CRITICAL_CHANCE)
            info->Effects[EFFECT_0].SpellClassMask[1] |= BloodboltClassMask1;

        if (info->Id == SPELL_BLOOD_RUNS_COLD &&
            info->Effects[EFFECT_0].ApplyAuraName == SPELL_AURA_ADD_FLAT_MODIFIER &&
            info->Effects[EFFECT_0].MiscValue == SPELLMOD_EFFECT1)
            info->Effects[EFFECT_0].MiscValue = SPELLMOD_EFFECT2;

        if (info->Id == SPELL_COAGULATION_DISPEL &&
            info->Effects[EFFECT_0].Effect == SPELL_EFFECT_DISPEL_MECHANIC &&
            info->Effects[EFFECT_0].MiscValue == int32(MECHANIC_BLEED))
            info->Effects[EFFECT_0].BasePoints = 1;

        if (info->Id != SPELL_VAMPIRIC_POOLS_LEECH ||
            info->Effects[EFFECT_0].Effect != SPELL_EFFECT_HEALTH_LEECH)
            return;

        info->DurationEntry = sSpellDurationStore.LookupEntry(32);
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
    RegisterSpellScript(spell_ascension_bloodmage_sanguine_rupture);
    RegisterSpellScript(spell_ascension_animated_blood);
    RegisterSpellScript(aura_ascension_bloodmage_crimson_feast);
    RegisterSpellScript(aura_ascension_bloodmage_coagulation);
    RegisterSpellScript(aura_ascension_bloodmage_forbidden_power);
    RegisterSpellScript(aura_ascension_bloodmage_dark_sigil);
    RegisterSpellScript(aura_ascension_bloodmage_blood_moon);
    RegisterSpellScript(aura_ascension_bloodmage_cursed_blood);
    RegisterSpellScript(aura_ascension_bloodmage_essence_harvester);
    RegisterSpellScript(aura_ascension_bloodmage_blood_bond);
}
