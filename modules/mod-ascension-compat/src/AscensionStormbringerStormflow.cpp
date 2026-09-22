/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellInfo.h"

namespace
{
enum StormflowSpells : uint32
{
    SPELL_BLESSING_OF_LEI_SHEN = 561228,
    SPELL_BLESSING_OF_LEI_SHEN_HEAL = 561308
};

enum StormflowFamilyFlags : uint32
{
    STORMFLOW_FLAG_ONE = 65536
};

enum StormflowUnportedAuras : uint32
{
    AURA_HEAL_FOR_DAMAGE_DEALT = 354
};

bool IsStormflow(SpellInfo const* info)
{
    return info && info->SpellFamilyName == 22 && (info->SpellFamilyFlags[1] & STORMFLOW_FLAG_ONE);
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
        if (info->Id == SPELL_BLESSING_OF_LEI_SHEN)
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
}

void AddSC_AscensionStormbringerStormflow()
{
    new stormbringer_stormflow_contracts();
    new stormbringer_stormflow_blessing();
}
