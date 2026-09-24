/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionReaperTalentProcs.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuras.h"
#include "SpellInfo.h"
#include "SpellScript.h"
#include "Unit.h"
#include <algorithm>

namespace
{
using AscensionReaperTalentProcs::Rules;

class spell_ascension_reaper_talent_proc : public AuraScript
{
    PrepareAuraScript(spell_ascension_reaper_talent_proc);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        auto rule = std::find_if(Rules.begin(), Rules.end(),
            [this](AscensionReaperTalentProcs::Rule const& entry)
            {
                return entry.Talent == GetId();
            });
        if (rule == Rules.end())
            return false;

        if (!rule->Spells[0])
            return true;

        SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
        if (!spellInfo)
            return false;

        return std::find(rule->Spells.begin(), rule->Spells.end(), spellInfo->Id) !=
            rule->Spells.end();
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_ascension_reaper_talent_proc::CheckProc);
    }
};

class aura_ascension_reaper_soulrot : public AuraScript
{
    PrepareAuraScript(aura_ascension_reaper_soulrot);

    void HandleDispel(DispelInfo* dispelInfo)
    {
        if (!dispelInfo)
            return;

        Unit* caster = GetCaster();
        Unit* target = GetUnitOwner();
        Unit* dispeller = dispelInfo->GetDispeller();
        if (!caster || !target || !dispeller || !caster->IsInWorld() || !dispeller->IsInWorld())
            return;

        if (target->IsPvP() && dispeller->IsPlayer())
            dispeller->ToPlayer()->UpdatePvP(true);

        caster->CastSpell(dispeller, 805089, true);
    }

    void Register() override
    {
        AfterDispel += AuraDispelFn(aura_ascension_reaper_soulrot::HandleDispel);
    }
};
}

void AddSC_AscensionReaperTalentProcs()
{
    RegisterSpellScript(spell_ascension_reaper_talent_proc);
    RegisterSpellScript(aura_ascension_reaper_soulrot);
}
