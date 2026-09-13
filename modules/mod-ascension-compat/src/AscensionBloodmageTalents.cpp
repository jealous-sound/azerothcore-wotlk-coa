/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuras.h"

namespace
{
enum BloodmageTalentSpells : uint32
{
    SPELL_LIQUIFY = 806310,
    SPELL_VAMPIRIC_POOLS = 504088,
    SPELL_VAMPIRIC_POOLS_LEECH = 806311
};

class bloodmage_talent_events : public UnitScript
{
public:
    bloodmage_talent_events() : UnitScript("bloodmage_talent_events", true, {UNITHOOK_ON_AURA_REMOVE}) { }

    void OnAuraRemove(Unit* unit, AuraApplication* application, AuraRemoveMode mode) override
    {
        Player* player = unit ? unit->ToPlayer() : nullptr;
        if (!player || player->getClass() != CLASS_SON_OF_ARUGAL || !application || !player->IsAlive() ||
            !player->IsInWorld() || mode == AURA_REMOVE_BY_DEATH)
            return;
        Aura* aura = application->GetBase();
        if (aura->GetId() == SPELL_LIQUIFY && aura->GetCasterGUID() == player->GetGUID() &&
            player->HasAura(SPELL_VAMPIRIC_POOLS))
            player->CastSpell(player, SPELL_VAMPIRIC_POOLS_LEECH, true);
    }
};
}

void AddSC_AscensionBloodmageTalents()
{
    new bloodmage_talent_events();
}
