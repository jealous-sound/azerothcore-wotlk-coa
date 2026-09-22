/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellInfo.h"

namespace
{
enum StormflowSpells : uint32
{
    SPELL_BLESSING_OF_LEI_SHEN = 561228,
    SPELL_BLESSING_OF_LEI_SHEN_HEAL = 561308,
    SPELL_STORM_SYNERGY = 578300,
    SPELL_CONDUCTIVE = 567559
};

enum StormflowFamilyFlags : uint32
{
    STORMFLOW_FLAG_ONE = 65536,
    ELECTROCUTE_FLAG_ZERO = 33554432,
    ELECTROCUTE_FLAG_TWO = 32
};

enum StormflowUnportedAuras : uint32
{
    AURA_HEAL_FOR_DAMAGE_DEALT = 354
};

bool IsStormflow(SpellInfo const* info)
{
    return info && info->SpellFamilyName == 22 && (info->SpellFamilyFlags[1] & STORMFLOW_FLAG_ONE);
}

bool IsElectrocute(SpellInfo const* info)
{
    return info && info->SpellFamilyName == 22 && (info->SpellFamilyFlags[0] & ELECTROCUTE_FLAG_ZERO) &&
        (info->SpellFamilyFlags[2] & ELECTROCUTE_FLAG_TWO);
}

Spell* StormflowChannel(Player* player)
{
    Spell* channel = player->GetCurrentSpell(CURRENT_CHANNELED_SPELL);
    if (!channel || channel->getState() == SPELL_STATE_FINISHED || !IsStormflow(channel->GetSpellInfo()))
        return nullptr;
    return channel;
}

class stormbringer_stormflow_contracts : public GlobalScript
{
public:
    stormbringer_stormflow_contracts() : GlobalScript("stormbringer_stormflow_contracts",
        {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* info) override
    {
        if (!info || info->SpellFamilyName != 22)
            return;
        auto dummy = [info](uint8 slot)
        {
            info->Effects[slot].ApplyAuraName = SPELL_AURA_DUMMY;
            info->Effects[slot].TriggerSpell = 0;
        };
        if (info->Id == SPELL_BLESSING_OF_LEI_SHEN || info->Id == SPELL_STORM_SYNERGY)
        {
            for (uint8 slot = 0; slot < MAX_SPELL_EFFECTS; ++slot)
                if (info->Effects[slot].ApplyAuraName == SPELL_AURA_PROC_TRIGGER_SPELL ||
                    info->Effects[slot].ApplyAuraName == AURA_HEAL_FOR_DAMAGE_DEALT)
                    dummy(slot);
            info->ProcFlags = info->ProcCharges = 0;
        }
    }
};

class stormbringer_stormflow_blessing : public UnitScript
{
public:
    stormbringer_stormflow_blessing() : UnitScript("stormbringer_stormflow_blessing", true,
        {UNITHOOK_ON_PERIODIC_DAMAGE_RESULT}) { }

    void OnPeriodicDamageResult(Unit*, Unit* caster, uint32 damage, SpellInfo const* info) override
    {
        if (!caster || !damage || !IsStormflow(info))
            return;
        Player* owner = caster->ToPlayer();
        if (!owner || owner->getClass() != CLASS_STORMBRINGER || !owner->IsAlive())
            return;
        AuraEffect const* blessing = owner->GetAuraEffect(SPELL_BLESSING_OF_LEI_SHEN, EFFECT_0);
        if (!blessing)
            return;
        owner->CastCustomSpell(SPELL_BLESSING_OF_LEI_SHEN_HEAL, SPELLVALUE_BASE_POINT0,
            int32(CalculatePct(damage, blessing->GetAmount())), owner, true);
    }
};

class stormbringer_stormflow_hits : public AllSpellScript
{
public:
    stormbringer_stormflow_hits() : AllSpellScript("stormbringer_stormflow_hits",
        {ALLSPELLHOOK_ON_HIT_RESULT}) { }

    void OnSpellHitResult(Spell* spell, Unit* target, uint8 miss, uint32 damage, uint32, bool) override
    {
        Player* player = spell->GetCaster() ? spell->GetCaster()->ToPlayer() : nullptr;
        SpellInfo const* info = spell->GetSpellInfo();
        if (!player || player->getClass() != CLASS_STORMBRINGER || info->SpellFamilyName != 22 ||
            !target || target == player || player->IsFriendlyTo(target) || miss != SPELL_MISS_NONE ||
            !damage || spell->IsTriggered())
            return;

        if (IsElectrocute(info) && player->HasAura(SPELL_STORM_SYNERGY) && StormflowChannel(player))
            player->CastSpell(player, SPELL_CONDUCTIVE, true);
    }
};
}

void AddSC_AscensionStormbringerStormflow()
{
    new stormbringer_stormflow_contracts();
    new stormbringer_stormflow_blessing();
    new stormbringer_stormflow_hits();
}
