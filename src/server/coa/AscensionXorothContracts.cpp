/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionXoroth.h"
#include "AscensionXorothData.h"
#include "DBCStores.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include <algorithm>
namespace AscensionXoroth
{
namespace
{
enum FleshHook : uint32
{
    SPELL_FLESH_HOOK_PULL = 800605,
    SPELL_RANGE_THIRTY_YARDS = 4
};
}

void ApplyContracts(SpellInfo* info)
{
    if (!info || info->SpellFamilyName != 23)
        return;
    uint32 id = info->Id;
    if (id == SPELL_WARPATH_PROTECTION && info->Effects[EFFECT_0].ApplyAuraName == SPELL_AURA_MOD_MINIMUM_SPEED)
        info->DurationEntry = sSpellDurationStore.LookupEntry(27);
    if (id == SPELL_FLESH_HOOK_PULL)
    {
        info->DmgClass = SPELL_DAMAGE_CLASS_NONE;
        info->RangeEntry = sSpellRangeStore.LookupEntry(SPELL_RANGE_THIRTY_YARDS);
    }
    if (id == 520440 || id == 520441)
        for (auto& effect : info->Effects)
            effect.Effect = 0;
    auto dummy = [info](uint8 i) {
        info->Effects[i].ApplyAuraName = SPELL_AURA_DUMMY;
        info->Effects[i].TriggerSpell = 0;
    };
    for (auto const& list :
         {std::pair(XorothEvents, std::size(XorothEvents)), std::pair(XorothCastDrivers, std::size(XorothCastDrivers))})
        for (size_t n = 0; n < list.second; ++n)
            if (list.first[n] == id)
            {
                for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                    if (info->Effects[i].ApplyAuraName == 42 || info->Effects[i].ApplyAuraName == 354)
                        dummy(i);
                info->ProcFlags = 0;
                info->ProcCharges = 0;
            }
    for (uint32 sid : XorothCopies)
        if (sid == id)
        {
            info->AttributesEx2 |= SPELL_ATTR2_CANT_CRIT;
            info->AttributesEx3 |= SPELL_ATTR3_IGNORE_CASTER_MODIFIERS;
            info->AttributesEx4 |= SPELL_ATTR4_IGNORE_DAMAGE_TAKEN_MODIFIERS;
            info->AttributesCu |= SPELL_ATTR0_CU_IGNORE_ARMOR;
            info->ProcFlags = 0;
            info->Effects[0].TargetA =
                SpellImplicitTargetInfo(id == 681206 ? TARGET_UNIT_CASTER : TARGET_UNIT_TARGET_ENEMY);
            info->Effects[0].TargetB = SpellImplicitTargetInfo();
            if (id == 680204)
                info->Effects[0].ValueMultiplier = 1;
        }
    for (uint32 sid : {500906, 704195, 704217, 707630})
        if (id == sid)
            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                if (info->Effects[i].Effect)
                    dummy(i);
    for (uint32 sid : {680197, 680203, 681184, 520021, 802617, 802618, 524913, 560630})
        if (id == sid)
        {
            info->ProcFlags = 0;
            info->ProcCharges = 0;
        }
    if (id == 680197)
    {
        info->Effects[0].ApplyAuraName = SPELL_AURA_ADD_PCT_MODIFIER;
        info->Effects[0].BasePoints = -101;
        info->Effects[0].SpellClassMask = flag96(0, 768, 0);
        info->Effects[1].Effect = 0;
    }
    if (id == 802618)
        info->Effects[0].SpellClassMask = flag96(0, 768, 0);
    if (id == 802617)
        info->Effects[0].SpellClassMask = flag96(0, 1, 0);
    if (id == 524913)
    {
        dummy(0);
        dummy(1);
    }
    if (id == 92100)
        info->Effects[1].MiscValue = 127;
    if (id == 804284)
        dummy(2);
    if (id == 573035)
        dummy(0);
    if (id == 573075)
    {
        info->DurationEntry = sSpellDurationStore.LookupEntry(21);
        info->Effects[1].Effect = 0;
    }
    if (id == 302546 || id == 302573 || id == 302574 || id == 302592)
    {
        auto& e = info->Effects[0];
        e.Effect = SPELL_EFFECT_APPLY_AURA;
        e.ApplyAuraName = id == 302546   ? SPELL_AURA_MOD_BLOCK_PERCENT
                          : id == 302573 ? SPELL_AURA_MOD_PERCENT_STAT
                          : id == 302574 ? SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN
                                         : SPELL_AURA_DUMMY;
        e.MiscValue = id == 302573 ? STAT_STRENGTH : id == 302574 ? 127 : 0;
        e.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
        e.TargetB = SpellImplicitTargetInfo();
        info->Effects[1].Effect = 0;
        info->Effects[2].Effect = 0;
    }
    if (id == 706935)
        dummy(0);
    if (id == 800999)
        dummy(2);
    if (id == 805680)
        info->Effects[1].Effect = 0;
    if (id == 805679)
        info->Effects[0].TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
    if (id == 302555)
    {
        info->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
        info->Effects[0].TargetA = SpellImplicitTargetInfo(TARGET_UNIT_TARGET_ALLY);
    }
    if (id == 804879)
    {
        info->Effects[0].ApplyAuraName = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
        info->Effects[0].MiscValue = 127;
    }
    if (id == 805678 || id == 807898)
    {
        info->Effects[1].ApplyAuraName = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
        info->Effects[1].BasePoints = -std::abs(info->Effects[1].BasePoints + 1) - 1;
    }
    if (id == 803671 || Named(info, 707693) || id == 800443 || id == 562029 || id == 520662)
        for (auto& e : info->Effects)
            if (e.Effect)
            {
                e.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_TARGET_ALLY);
                e.TargetB = SpellImplicitTargetInfo();
            }
    if (id == 807671 || id == 707515 || id == 704988 || id == 560828 || id == 300375)
        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            if (info->Effects[i].Effect)
                dummy(i);
    if (id == 500003)
        info->Effects[0].MiscValue = ASCENSION_CREATURE_GLOBAL_CRIT, info->Effects[0].MiscValueB = 4;
    if (id == 805697 || id == 807899)
        info->Effects[0].MiscValue = ASCENSION_STATE_GLOBAL_CRIT_DAMAGE;
    if (id == 300374 || id == 301357 || id == 807669)
        dummy(0);
    if (id == 704987)
        dummy(1);
    if (id == 704185)
    {
        dummy(0);
        dummy(1);
    }
    if (id == 804787)
    {
        info->StackAmount = 5;
        dummy(1);
        dummy(2);
    }
    if (id == 805677)
        info->CasterAuraSpell = 0;
    if (id == 805965)
    {
        auto& e = info->Effects[0];
        e.Effect = SPELL_EFFECT_APPLY_AURA;
        e.ApplyAuraName = SPELL_AURA_ADD_FLAT_MODIFIER;
        e.MiscValue = SPELLMOD_COST;
        e.SpellClassMask = flag96(32768, 0, 0);
        e.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
        info->DurationEntry = sSpellDurationStore.LookupEntry(21);
    }
    if (id == 800702)
        dummy(0);
    if (id == 801064)
        info->DurationEntry = sSpellDurationStore.LookupEntry(1);
    if (id == 802342)
        info->Effects[1].Effect = 0;
    if (id == 803334)
        dummy(2);
    if (id == 801053 || id == 802344 || id == 804786)
    {
        auto& e = info->Effects[0];
        e.Effect = SPELL_EFFECT_APPLY_AURA;
        e.ApplyAuraName = SPELL_AURA_PERIODIC_DUMMY;
        e.Amplitude = 1000;
        e.TriggerSpell = 0;
        e.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
        e.TargetB = SpellImplicitTargetInfo();
    }
    if (id == 804786)
    {
        info->AttributesCu &= ~SPELL_ATTR0_CU_NEGATIVE_EFF0;
    }
    if (id == 801055 || id == 560817 || id == 802855 || id == 802856 || id == 802857 || id == 801052)
        for (auto& e : info->Effects)
            if (e.Effect)
            {
                e.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_TARGET_ENEMY);
                e.TargetB = SpellImplicitTargetInfo();
            }
    if (id == 802855)
        info->Effects[2].Effect = 0;
    if (id == 802602)
        info->Effects[2].Effect = 0;
    if (id == 520857)
        info->Effects[0].TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CONE_ENEMY_104);
    if (id == 520292)
    {
        info->ManaCost = 200;
        info->RecoveryTime = 15000;
        info->CategoryRecoveryTime = 0;
    }
    if (id == 804886 || id == 804011)
        for (auto& e : info->Effects)
            if (e.Effect == SPELL_EFFECT_SCHOOL_DAMAGE)
            {
                e.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_SRC_AREA_ENEMY);
                e.TargetB = SpellImplicitTargetInfo();
                e.RadiusEntry = sSpellRadiusStore.LookupEntry(13);
            }
    if (id == 806219)
        info->Effects[0].TargetA = SpellImplicitTargetInfo(TARGET_UNIT_TARGET_ENEMY);
    if (id == 801042)
    {
        info->AttributesEx2 |= SPELL_ATTR2_ALLOW_DEAD_TARGET;
        info->Effects[0].Effect = SPELL_EFFECT_DUMMY;
        info->Effects[0].TargetA = SpellImplicitTargetInfo(TARGET_UNIT_TARGET_ENEMY);
        for (uint8 i = 1; i < MAX_SPELL_EFFECTS; ++i)
            if (info->Effects[i].Effect)
                info->Effects[i].TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
    }
    if (id == 804703)
    {
        info->Effects[2].ApplyAuraName = SPELL_AURA_PERIODIC_DUMMY;
        info->Effects[2].TriggerSpell = 0;
    }
    if (id == 804775)
    {
        info->Effects[0].ApplyAuraName = SPELL_AURA_MOUNTED;
        info->Effects[0].MiscValue = 914505;
    }
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (info->Effects[i].ApplyAuraName == SPELL_AURA_ADD_PCT_MODIFIER && info->Effects[i].MiscValue == 32)
            dummy(i);
    for (uint32 sid : {801071, 802347, 804801})
        if (id == sid)
            for (auto& effect : info->Effects)
                if (effect.Effect)
                {
                    effect.Effect = SPELL_EFFECT_APPLY_AURA;
                    effect.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_TARGET_ENEMY);
                    effect.TargetB = SpellImplicitTargetInfo();
                }
    if (id == 680204)
        info->Effects[1].Effect = 0;
    if (id == 712294)
    {
        info->Effects[1].Effect = 0;
        info->Effects[2].Effect = 0;
    }
    if (id == 704953)
    {
        // Chop Shop: each stack of Demon's Blood (500906) adds 1% crit chance to Sever/Brimstone
        // Bludgeon (family bit 0,2097152 - shared by Sever 500904 and its Brimstone Bludgeon 520005
        // replacement) and Meatsaw (family bit 0,8388608). Base amount starts at 0 and is kept in sync
        // with the current stack count by Refresh() in AscensionXoroth.cpp.
        SpellEffectInfo& e = info->Effects[EFFECT_0];
        e.Effect = SPELL_EFFECT_APPLY_AURA;
        e.ApplyAuraName = SPELL_AURA_ADD_FLAT_MODIFIER;
        e.BasePoints = 0;
        e.DieSides = 0;
        e.MiscValue = SPELLMOD_CRITICAL_CHANCE;
        e.SpellClassMask = flag96(0, 2097152 | 8388608, 0);
        e.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
        e.TargetB = SpellImplicitTargetInfo();
        info->Effects[EFFECT_1].Effect = 0;
        info->Effects[EFFECT_2].Effect = 0;
    }
    if (id == 704999)
    {
        // Combusting Blade: flat +5% Intellect, plus a private SPELLMOD_COST flat modifier that
        // reduces Infernal Strike's Rage cost by 5. The classmask reuses the same combined Infernal
        // Strike/Shieldgore family bits already patched onto Fiend (680197) above, so the discount
        // survives the Shieldgore replacement exactly like that existing damage bonus does.
        SpellEffectInfo& intellect = info->Effects[EFFECT_0];
        intellect.Effect = SPELL_EFFECT_APPLY_AURA;
        intellect.ApplyAuraName = SPELL_AURA_MOD_PERCENT_STAT;
        intellect.BasePoints = 5;
        intellect.DieSides = 0;
        intellect.MiscValue = STAT_INTELLECT;
        intellect.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
        intellect.TargetB = SpellImplicitTargetInfo();
        SpellEffectInfo& cost = info->Effects[EFFECT_1];
        cost.Effect = SPELL_EFFECT_APPLY_AURA;
        cost.ApplyAuraName = SPELL_AURA_ADD_FLAT_MODIFIER;
        cost.BasePoints = -5;
        cost.DieSides = 0;
        cost.MiscValue = SPELLMOD_COST;
        cost.SpellClassMask = flag96(0, 768, 0);
        cost.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
        cost.TargetB = SpellImplicitTargetInfo();
        info->Effects[EFFECT_2].Effect = 0;
    }
    if (id == 707388)
    {
        // Consuming Blade: a private SPELLMOD_CRITICAL_CHANCE flat modifier scoped to Gore's family
        // classmask (0,64,0), plus a flat expertise bonus via the native SPELL_AURA_MOD_EXPERTISE.
        SpellEffectInfo& crit = info->Effects[EFFECT_0];
        crit.Effect = SPELL_EFFECT_APPLY_AURA;
        crit.ApplyAuraName = SPELL_AURA_ADD_FLAT_MODIFIER;
        crit.BasePoints = 25;
        crit.DieSides = 0;
        crit.MiscValue = SPELLMOD_CRITICAL_CHANCE;
        crit.SpellClassMask = flag96(0, 64, 0);
        crit.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
        crit.TargetB = SpellImplicitTargetInfo();
        SpellEffectInfo& expertise = info->Effects[EFFECT_1];
        expertise.Effect = SPELL_EFFECT_APPLY_AURA;
        expertise.ApplyAuraName = SPELL_AURA_MOD_EXPERTISE;
        expertise.BasePoints = 5;
        expertise.DieSides = 0;
        expertise.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
        expertise.TargetB = SpellImplicitTargetInfo();
        info->Effects[EFFECT_2].Effect = 0;
    }
    if (id == 804947)
    {
        // Screamin' Demon: a private SPELLMOD_COOLDOWN flat modifier of -15 sec scoped to Unleash
        // Pestilence's own family classmask, following the same full-effect-rewrite pattern used for
        // Battle Cleric (AscensionSunClericBattleCleric.cpp) - explicit DieSides 0 avoids the
        // BasePoints+1 rounding that a die-sides-1 slot would otherwise apply.
        SpellEffectInfo& e = info->Effects[EFFECT_0];
        e.Effect = SPELL_EFFECT_APPLY_AURA;
        e.ApplyAuraName = SPELL_AURA_ADD_FLAT_MODIFIER;
        e.BasePoints = -15000;
        e.DieSides = 0;
        e.MiscValue = SPELLMOD_COOLDOWN;
        e.SpellClassMask = flag96(0, 65536, 0);
        e.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
        e.TargetB = SpellImplicitTargetInfo();
        info->Effects[EFFECT_1].Effect = 0;
        info->Effects[EFFECT_2].Effect = 0;
    }
    if (id == 706501)
    {
        // Absolutism: partial armor ignore on Meatsaw via the custom SPELL_AURA_ASCENSION_MOD_IGNORE_ARMOR_PCT
        // (Unit::CalcArmorReducedDamage reads this as a positive percentage to shave off the target's
        // armor before mitigation), scoped to Meatsaw's family classmask. Same aura type already used
        // by AscensionWitchHunterCompletion.cpp (804194) and AscensionSunClericRadiance.cpp (Vindicator).
        SpellEffectInfo& e = info->Effects[EFFECT_0];
        e.Effect = SPELL_EFFECT_APPLY_AURA;
        e.ApplyAuraName = SPELL_AURA_ASCENSION_MOD_IGNORE_ARMOR_PCT;
        e.BasePoints = 10;
        e.DieSides = 0;
        e.SpellClassMask = flag96(0, 8388608, 0);
        e.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
        e.TargetB = SpellImplicitTargetInfo();
        info->Effects[EFFECT_1].Effect = 0;
        info->Effects[EFFECT_2].Effect = 0;
    }
    if (id == 705020)
    {
        // War Pig: flat +4% Physical damage via the native SPELL_AURA_MOD_DAMAGE_PERCENT_DONE
        // (MiscValue = SPELL_SCHOOL_MASK_NORMAL), already folded into the melee/ranged/spell
        // damage-done paths by UpdateDamagePctDoneMods.
        SpellEffectInfo& e = info->Effects[EFFECT_0];
        e.Effect = SPELL_EFFECT_APPLY_AURA;
        e.ApplyAuraName = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
        e.BasePoints = 4;
        e.DieSides = 0;
        e.MiscValue = SPELL_SCHOOL_MASK_NORMAL;
        e.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
        e.TargetB = SpellImplicitTargetInfo();
        info->Effects[EFFECT_1].Effect = 0;
        info->Effects[EFFECT_2].Effect = 0;
    }
    if (id == 680723)
    {
        // Brute Strength: native SPELL_AURA_MOD_CRIT_DAMAGE_BONUS (+10% Physical crit damage), plus
        // a private SPELLMOD_RANGE flat modifier (+5 yds) scoped to a combined classmask spanning
        // Flesh Hook (500020, dword0 bit3 and dword2 bit0), Chains of Malice (803185, dword0 bit22)
        // and Chainwhip (800081, dword0 bit19).
        SpellEffectInfo& crit = info->Effects[EFFECT_0];
        crit.Effect = SPELL_EFFECT_APPLY_AURA;
        crit.ApplyAuraName = SPELL_AURA_MOD_CRIT_DAMAGE_BONUS;
        crit.BasePoints = 10;
        crit.DieSides = 0;
        crit.MiscValue = SPELL_SCHOOL_MASK_NORMAL;
        crit.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
        crit.TargetB = SpellImplicitTargetInfo();
        SpellEffectInfo& range = info->Effects[EFFECT_1];
        range.Effect = SPELL_EFFECT_APPLY_AURA;
        range.ApplyAuraName = SPELL_AURA_ADD_FLAT_MODIFIER;
        range.BasePoints = 5;
        range.DieSides = 0;
        range.MiscValue = SPELLMOD_RANGE;
        range.SpellClassMask = flag96(4718592, 0, 1);
        range.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
        range.TargetB = SpellImplicitTargetInfo();
    }
    if (id == 804354)
    {
        // Black Skull Shield: native SPELL_AURA_ADD_PCT_MODIFIER (SPELLMOD_DAMAGE) +25% Shieldgore
        // damage, plus a private SPELLMOD_COST flat modifier of -5 Rage, both scoped to Shieldgore's
        // own family classmask (804353, dword1 bit8) - the same SPELLMOD_COST pattern used by
        // Combusting Blade (#942).
        SpellEffectInfo& dmg = info->Effects[EFFECT_0];
        dmg.Effect = SPELL_EFFECT_APPLY_AURA;
        dmg.ApplyAuraName = SPELL_AURA_ADD_PCT_MODIFIER;
        dmg.BasePoints = 25;
        dmg.DieSides = 0;
        dmg.MiscValue = SPELLMOD_DAMAGE;
        dmg.SpellClassMask = flag96(0, 256, 0);
        dmg.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
        dmg.TargetB = SpellImplicitTargetInfo();
        SpellEffectInfo& cost = info->Effects[EFFECT_1];
        cost.Effect = SPELL_EFFECT_APPLY_AURA;
        cost.ApplyAuraName = SPELL_AURA_ADD_FLAT_MODIFIER;
        cost.BasePoints = -5;
        cost.DieSides = 0;
        cost.MiscValue = SPELLMOD_COST;
        cost.SpellClassMask = flag96(0, 256, 0);
        cost.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
        cost.TargetB = SpellImplicitTargetInfo();
        info->Effects[EFFECT_2].Effect = 0;
    }
    if (id == 704959)
    {
        // Cinderblade: native SPELL_AURA_MOD_RATING_FROM_STAT converts 20% of Intellect into melee/
        // ranged/spell critical strike rating (Player::UpdateRating reads MiscValue as a CombatRating
        // bitmask - CR_CRIT_MELEE|CR_CRIT_RANGED|CR_CRIT_SPELL = 1792 - and MiscValueB as the source
        // Stats index), plus native SPELL_AURA_MOD_SPELL_HIT_CHANCE +6%.
        SpellEffectInfo& crit = info->Effects[EFFECT_0];
        crit.Effect = SPELL_EFFECT_APPLY_AURA;
        crit.ApplyAuraName = SPELL_AURA_MOD_RATING_FROM_STAT;
        crit.BasePoints = 20;
        crit.DieSides = 0;
        crit.MiscValue = (1 << CR_CRIT_MELEE) | (1 << CR_CRIT_RANGED) | (1 << CR_CRIT_SPELL);
        crit.MiscValueB = STAT_INTELLECT;
        crit.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
        crit.TargetB = SpellImplicitTargetInfo();
        SpellEffectInfo& hit = info->Effects[EFFECT_1];
        hit.Effect = SPELL_EFFECT_APPLY_AURA;
        hit.ApplyAuraName = SPELL_AURA_MOD_SPELL_HIT_CHANCE;
        hit.BasePoints = 6;
        hit.DieSides = 0;
        hit.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
        hit.TargetB = SpellImplicitTargetInfo();
    }
    if (id == 707232)
        // Speed Demon: neutralize its own raw SPELLMOD_EFFECT3 effect (which targeted Suffuse's
        // then-inert EFFECT_2 slot and would otherwise still fire through the native spellmod
        // pipeline whenever Suffuse is cast). The talent's actual bonus is read directly by value
        // below via Amount(). Re-tag EFFECT_0 as SPELL_AURA_DUMMY rather than zeroing
        // info->Effects[EFFECT_0].Effect outright: zeroing the effect type leaves the spell with no
        // valid unit-owned aura effect at all (Aura::BuildEffectMaskForOwner returns 0), so the
        // passive can never actually become an active aura via set_aura's fixture path or the
        // native learn-time CastSpell (Player::addSpell) alike - the very player->HasAura(707232)
        // gate this talent depends on could then never be true (same bug class as Murderous Might,
        // 520290, above).
        info->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
    if (id == 801063)
    {
        // Suffuse: claim the previously inert EFFECT_2 slot as a native SPELL_AURA_MOD_INCREASE_SPEED
        // effect on the caster. Its amount is zero here and is scaled to Speed Demon's own tooltip
        // value (707232) only while the player has that talent, in
        // aura_ascension_xoroth_lifecycle::Calculate (AscensionXorothAuras.cpp) - Suffuse's own
        // aura lifetime already gates how long the speed bonus lasts.
        SpellEffectInfo& e = info->Effects[EFFECT_2];
        e.Effect = SPELL_EFFECT_APPLY_AURA;
        e.ApplyAuraName = SPELL_AURA_MOD_INCREASE_SPEED;
        e.BasePoints = 0;
        e.DieSides = 0;
        e.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
        e.TargetB = SpellImplicitTargetInfo();
    }
    if (id == 704954)
        // Boundless Fury: its own baked SPELLMOD_ALL_EFFECTS classmask (dword0 bit26, matching
        // Unleash Death 801055 - Pestilence of Death's unleash spell) is duplicated identically
        // on EFFECT_0 and EFFECT_1 and is never read by AscensionXoroth::Unleash()'s custom
        // State().unleash damage multiplier. Null both; the real 30% value is read directly via
        // Amount() and applied in Unleash() below.
        for (auto& e : info->Effects)
            e.Effect = 0;
    if (id == 704956)
        // Heart of Xoroth: its own baked SPELLMOD_MAX_AURA_STACKS effect (misc 31, +5 stacks via
        // BasePoints 4 + the DieSides 1 rounding) carries a scrambled classmask
        // ([536870912,0,16]) that overlaps none of Demon's Blood's (500906) actual family flags
        // (dword1 bit4 / 16). Correct it so the native spellmod - applied via
        // AscensionXoroth::Gain()'s ApplySpellMod(500906, SPELLMOD_MAX_AURA_STACKS, ...), the
        // same pattern as Necromancer's Capacity() (AscensionNecromancer.cpp) - actually matches.
        info->Effects[EFFECT_0].SpellClassMask = flag96(0, 16, 0);
    if (id == 520290)
        // Murderous Might: its own baked SPELLMOD_DAMAGE classmask (dword1 bit0, dword2 bits
        // 2/25) only overlaps 3 of the 11 Demonfire-spending root spells in
        // AscensionXoroth::Spender() - Skulltaker (800168), Melt (803334) and Warbringer
        // (802581) - and Hellstorm (802342) carries no family flags at all, so no classmask can
        // ever cover it. Null the native SPELLMOD_DAMAGE behavior and read its value directly via
        // Amount() in Factor() below, covering every spender uniformly. Re-tag EFFECT_0 as
        // SPELL_AURA_DUMMY rather than zeroing info->Effects[EFFECT_0].Effect outright: zeroing the
        // effect type leaves the spell with no valid unit-owned aura effect at all
        // (Aura::BuildEffectMaskForOwner returns 0), so the passive can never actually become an
        // active aura via the native learn-time CastSpell (Player::addSpell) - the very
        // player->HasAura(520290) gate this fix (and its own scenario fixture) depends on could
        // then never be true. Keeping EFFECT_0 as a harmless dummy aura lets it apply normally
        // while carrying no native spellmod behavior of its own.
        info->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
    if (id == 680900)
        // Infernal Pummeling: its own baked effect is a stray SPELLMOD_EFFECT2 flat modifier
        // scoped to Melt's own classmask (would add +10 to Melt's disabled EFFECT_2 crit-chance
        // slot instead of a damage-taken bonus). Neutralize it; the real 10% value is read directly
        // via Amount() and applied to Melt's own inert MOD_DAMAGE_FROM_CASTER slot (id == 803334,
        // EFFECT_1 in ModifySpellEffectBaseValue below), gated on this talent. Re-tag as
        // SPELL_AURA_DUMMY rather than zeroing the effect type outright - the latter leaves no
        // valid unit-owned aura effect at all (Aura::BuildEffectMaskForOwner returns 0), so
        // set_aura's fixture path (and native learn-time CastSpell) can never make the passive an
        // active aura, and player->HasAura(680900) could then never be true (same bug class as
        // Murderous Might, 520290, above; confirmed empirically: set_aura threw "Could not apply
        // fixture aura" before this fix).
        info->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
    if (id == 300386)
        info->Effects[EFFECT_0].Effect = 0;
    if (id == 300387)
        // Brimstone's Blood: same "zero effect" bug class as Murderous Might (520290) above - the
        // player->HasAura(300387) gate in aura_ascension_xoroth_event::Check (AscensionXorothEvents.cpp,
        // case 804345) could never be true if this effect is zeroed outright. Re-tag as SPELL_AURA_DUMMY.
        info->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
    if (id == 300390)
    {
        SpellEffectInfo& e = info->Effects[EFFECT_0];
        e.Effect = SPELL_EFFECT_APPLY_AURA;
        e.ApplyAuraName = SPELL_AURA_ADD_FLAT_MODIFIER;
        e.BasePoints = -100;
        e.DieSides = 0;
        e.MiscValue = SPELLMOD_COST;
        e.SpellClassMask = flag96(0, 8389376, 4);
        e.TargetA = SpellImplicitTargetInfo(TARGET_UNIT_CASTER);
        e.TargetB = SpellImplicitTargetInfo();
        info->Effects[EFFECT_1].Effect = 0;
        info->Effects[EFFECT_2].Effect = 0;
    }
    if (id == 704980)
        info->Effects[EFFECT_1].Effect = 0;
    if (id == 705000)
        info->Effects[EFFECT_0].Effect = 0;
    info->_InitializeExplicitTargetMask();
}
}
namespace
{
using namespace AscensionXoroth;
class xoroth_scaling : public UnitScript
{
  public:
    xoroth_scaling()
        : UnitScript("xoroth_scaling", true,
                     {UNITHOOK_MODIFY_SPELL_EFFECT_BASE_VALUE, UNITHOOK_MODIFY_SPELL_DAMAGE_TAKEN,
                      UNITHOOK_MODIFY_PERIODIC_DAMAGE_AURAS_TICK, UNITHOOK_ON_PERIODIC_DAMAGE_RESULT,
                      UNITHOOK_ON_DAMAGE, UNITHOOK_MODIFY_MELEE_DAMAGE})
    {
    }
    void ModifySpellEffectBaseValue(Unit const* caster, SpellInfo const* info, uint8 index, float& value) override
    {
        Player* player = Owner(caster);
        if (!player && caster)
            player = Owner(caster->GetOwner());
        if (!player || !info)
            return;
        if (info->Id == 630930 && index == EFFECT_0 && caster->GetEntry() == 510100)
        {
            double level = caster->GetLevel();
            value *= float(0.0267291844060354 + 0.0048541098014737 * level + 0.0001859597762293 * level * level);
        }
        for (auto const& row : XorothCoefficients)
            if (row.spell == info->Id && row.effect == index)
            {
                float sp = float(std::max(0, player->SpellBaseDamageBonusDone(SpellSchoolMask(row.school))));
                float bonus = row.sp * sp + row.ap * player->GetTotalAttackPowerValue(BASE_ATTACK) +
                              row.strength * player->GetStat(STAT_STRENGTH) +
                              row.stamina * player->GetStat(STAT_STAMINA);
                if (Named(info, 805671) && player->HasAura(704988))
                    bonus *= 1.25f;
                value += bonus;
            }
        if (Named(info, 801016) && index == 1 && player->HasAura(92100))
            value *= 2;
        if (info->Id == 804353 && index == 1)
            value += player->GetShieldBlockValue();
        if (info->Id == 801064 && index == 0)
            value += player->GetUInt32Value(PLAYER_FIELD_COMBAT_RATING_1 + CR_DEFENSE_SKILL) * .20f;
        if (info->Id == 805680 && index == 0 && State(player).curseShield)
            value += player->GetStat(STAT_STAMINA) * 2;
        if (info->Id == 805680 && index == 0 && player->HasAura(560828) && player->GetHealthPct() < 35)
            value *= 2;
        if (info->Id == 573075 && index == 0)
            value = player->GetUInt32Value(PLAYER_FIELD_COMBAT_RATING_1 + CR_BLOCK) * .5f;
        if (info->Id == 573066 && index == 0)
            value += State(player).imps.size() * Amount(805916, 2);
        if (info->Id == 804788 && index == 0)
            value *= 1 + .2f * Count(player, 804787);
        if (info->Id == 800999 && index == 1)
            value = player->HasAura(302581) ? float(Amount(302581)) : 0;
        if (info->Id == 803334 && index == 1)
            value = player->HasAura(680900) ? float(Amount(680900)) : 0;
        if (info->Id == 801055 || info->Id == 560817 || info->Id == 802855)
            value *= State(player).unleash;
        value = std::clamp(value, float(INT32_MIN / 2), float(INT32_MAX / 2));
    }
    float Factor(Unit* target, Unit* caster, SpellInfo const* info)
    {
        Player* player = Owner(caster);
        bool pet = false;
        if (!player && caster)
            player = Owner(caster->GetOwner()), pet = true;
        float defense = 1;
        if (target && (target->GetEntry() == 50301 || target->GetEntry() == 50375))
            if (Player* master = Owner(target->GetOwner()))
                if (Aura* grit = master->GetAuraOfRankedSpell(805678))
                    if (!target->HasAura(grit->GetId(), master->GetGUID()))
                        defense *= 1 - Amount(grit->GetId(), 1) / 100.0f;
        if (!player || !target || !info || Derived(info))
            return defense;
        float factor = defense;
        if (pet && caster->GetEntry() == 50301 && player->HasAura(804879) &&
            !caster->HasAura(804879, player->GetGUID()))
            factor *= 2.5f;
        if ((info->SchoolMask & SPELL_SCHOOL_MASK_FIRE) && target->GetHealthPct() > 80)
        {
            if (!pet)
            {
                if (Aura* a = player->GetAuraOfRankedSpell(300374))
                    factor *= 1 + Amount(a->GetId()) / 100.0f;
            }
            else if (caster->GetEntry() == 50301 && player->HasAura(807669))
                factor *= 1 + Amount(807669) / 100.0f;
        }
        if (!pet && Named(info, 805671))
        {
            if (player->HasAura(707515) && player->HasAura(805696))
                factor *= 1 + Amount(707515, 1) / 100.0f;
            if (player->HasAura(807671) && target->HasAura(804801, player->GetGUID()))
                factor *= 1 + (Amount(807671) + 10 * Count(player, 500906)) / 100.0f;
        }
        if (!pet && Sever(info) && player->HasAura(300375) && target->HasAuraState(AuraStateType(30), info, player))
            factor *= 1 + Amount(300375) / 100.0f;
        if (!pet && Spender(info) && player->HasAura(520290))
            factor *= 1 + Amount(520290) / 100.0f;
        if (info->Id == 806219 && target->GetCreatureType() == CREATURE_TYPE_HUMANOID)
            factor *= 1 + Amount(704987, 1) / 100.0f;
        return factor;
    }
    void ModifyMeleeDamage(Unit* target, Unit* caster, uint32& damage) override
    {
        damage = uint32(damage * Factor(target, caster, nullptr));
    }
    void ModifySpellDamageTaken(Unit* target, Unit* caster, int32& damage, SpellInfo const* info) override
    {
        damage = int32(damage * Factor(target, caster, info));
    }
    void ModifyPeriodicDamageAurasTick(Unit* target, Unit* caster, uint32& damage, SpellInfo const* info) override
    {
        damage = uint32(damage * Factor(target, caster, info));
    }
    void OnPeriodicDamageResult(Unit* target, Unit* caster, uint32 damage, SpellInfo const* info) override
    {
        Player* player = Owner(caster);
        if (player && target && player->HasAura(802619) && Named(info, 806965))
            if (Aura* aura = target->GetAura(info->Id, player->GetGUID()))
                aura->SetScriptValue(802619, aura->GetScriptValue(802619) + damage);
    }
    void OnDamage(Unit* caster, Unit* target, uint32& damage) override
    {
        if (!damage)
            return;
        for (Player* player : {Owner(caster), Owner(target)})
            if (player)
            {
                State(player).timers.RescheduleEvent(804703, 3s);
                if (player == target && player->HasAura(804703) && Count(player, 804787) && !player->HasAura(804785) &&
                    !player->HasAura(805746) && !roll_chance_i(player->HasAura(704964) ? Amount(704964) : 0))
                    Cast(player, player, 805746);
            }
    }
};
}
void AddSC_AscensionXorothContracts()
{
    new xoroth_scaling();
}
