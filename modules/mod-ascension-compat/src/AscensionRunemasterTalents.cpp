/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionRunemasterTalents.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuras.h"
#include "SpellInfo.h"

namespace
{
bool IsEarthTattoo(uint32 id)
{
    return id == 801094 || (id >= 803754 && id <= 803758);
}

bool StonePetroglyphActive(Player* player)
{
    if (!player->IsAlive() || !player->HasAura(707157))
        return false;
    if (player->HasAura(801094, player->GetGUID()))
        return true;
    for (uint32 id = 803754; id <= 803758; ++id)
        if (player->HasAura(id, player->GetGUID()))
            return true;
    return false;
}

void SyncStonePetroglyph(Player* player)
{
    if (!StonePetroglyphActive(player))
        player->RemoveAurasDueToSpell(712310, player->GetGUID());
    else if (!player->HasAura(712310, player->GetGUID()))
        player->CastSpell(player, 712310, true);
}

class runemaster_talent_events : public UnitScript
{
public:
    runemaster_talent_events() : UnitScript("runemaster_talent_events", true,
        {UNITHOOK_ON_AURA_APPLY, UNITHOOK_ON_AURA_REMOVE}) { }

    void OnAuraApply(Unit* unit, Aura* aura) override
    {
        Player* player = unit ? unit->ToPlayer() : nullptr;
        if (!player || player->getClass() != CLASS_SPIRIT_MAGE || !aura)
            return;
        uint32 id = aura->GetId();
        if (id == 707157 || id == 712310 || IsEarthTattoo(id))
            SyncStonePetroglyph(player);
    }

    void OnAuraRemove(Unit* unit, AuraApplication* application, AuraRemoveMode mode) override
    {
        Player* player = unit ? unit->ToPlayer() : nullptr;
        if (!player || player->getClass() != CLASS_SPIRIT_MAGE || !application)
            return;
        Aura* aura = application->GetBase();
        uint32 id = aura->GetId();
        if (id == 707157 || IsEarthTattoo(id))
            SyncStonePetroglyph(player);
        if (id == 500288 && aura->GetCasterGUID() == player->GetGUID() && mode != AURA_REMOVE_BY_DEATH &&
            player->IsAlive() && player->IsInWorld() && player->HasAura(520054))
            player->CastSpell(player, 520768, true);
    }
};
}

void ApplyAscensionRunemasterTalentContracts(SpellInfo* info)
{
    if (info->Id != 712310 || info->SpellFamilyName != 38)
        return;
    // The native periodic heal and effect-98 immunity already exist. Complete
    // knockback immunity for the separate destination-based effect as well.
    auto& effect = info->Effects[EFFECT_1];
    effect.Effect = SPELL_EFFECT_APPLY_AURA;
    effect.ApplyAuraName = SPELL_AURA_EFFECT_IMMUNITY;
    effect.MiscValue = SPELL_EFFECT_KNOCK_BACK_DEST;
    effect.BasePoints = 0;
    effect.DieSides = 0;
}

void AddSC_AscensionRunemasterTalents()
{
    new runemaster_talent_events();
}
