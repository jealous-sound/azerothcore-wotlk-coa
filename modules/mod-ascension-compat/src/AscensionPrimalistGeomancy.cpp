/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellMgr.h"
#include "SpellScript.h"

#include <algorithm>

namespace
{
enum GeomancySpells : uint32
{
    Rockslide = 560154,
    RockslideHelper = 560155,
    StoneshardFirst = 680448
};

constexpr TriggerCastFlags RepeatCastFlags = TriggerCastFlags(TRIGGERED_FULL_MASK & ~TRIGGERED_DISALLOW_PROC_EVENTS);

class aura_ascension_rockslide : public AuraScript
{
    PrepareAuraScript(aura_ascension_rockslide);

    bool Validate(SpellInfo const*) override { return ValidateSpellInfo({RockslideHelper, StoneshardFirst}); }

    bool Check(ProcEventInfo& event)
    {
        Unit* owner = GetTarget();
        Spell const* source = event.GetProcSpell();
        Unit* victim = source ? source->m_targets.GetUnitTarget() : nullptr;
        return owner->IsPlayer() && owner->getClass() == CLASS_WILDWALKER && owner->IsAlive() &&
            event.GetActor() == owner && source && source->GetCaster() == owner &&
            source->GetSpellInfo()->SpellFamilyName == 37 &&
            sSpellMgr->GetFirstSpellInChain(source->GetSpellInfo()->Id) == StoneshardFirst &&
            victim && victim->IsAlive() && owner->IsValidAttackTarget(victim);
    }

    void Repeat(AuraEffect const*, ProcEventInfo& event)
    {
        PreventDefaultAction();
        Unit* owner = GetTarget();
        Spell const* source = event.GetProcSpell();
        ObjectGuid ownerGuid = owner->GetGUID();
        ObjectGuid victimGuid = source->m_targets.GetUnitTarget()->GetGUID();
        uint32 spell = source->GetSpellInfo()->Id;
        int32 delay = sSpellMgr->GetSpellInfo(RockslideHelper)->Effects[EFFECT_0].CalcValue(owner);
        // The copied helper fixes the repeat to rank one. Keep its delay but repeat the actual cast rank.
        owner->m_Events.AddEventAtOffset([ownerGuid, victimGuid, spell]()
        {
            Player* player = ObjectAccessor::FindPlayer(ownerGuid);
            if (!player || !player->IsAlive() || !player->HasAura(Rockslide, ownerGuid))
                return;
            Unit* victim = ObjectAccessor::GetUnit(*player, victimGuid);
            if (victim && victim->IsAlive() && player->IsValidAttackTarget(victim))
                // The tooltip explicitly permits another Rockslide roll from this delayed repeat.
                player->CastSpell(victim, spell, RepeatCastFlags);
        }, Milliseconds(std::max(1, delay)));
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_ascension_rockslide::Check);
        OnEffectProc += AuraEffectProcFn(aura_ascension_rockslide::Repeat, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};
}

void AddSC_AscensionPrimalistGeomancy()
{
    RegisterSpellScript(aura_ascension_rockslide);
}
