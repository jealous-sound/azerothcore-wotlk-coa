/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionRunemasterTalents.h"
#include "ObjectAccessor.h"
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

void SyncRuneshroudOrWaveforged(Player* player)
{
    bool active = player->HasAura(500288, player->GetGUID()) || player->HasAura(705565, player->GetGUID());
    if (!active)
        player->RemoveAurasDueToSpell(808089, player->GetGUID());
    else if (!player->HasAura(808089, player->GetGUID()))
        player->CastSpell(player, 808089, true);
}

constexpr uint32 SPELL_PERMAFROST_RUNE = 804060;
constexpr uint32 SPELL_PERMAFROST_MARKER = 807114;
constexpr uint32 SPELL_RUNESHROUD = 500288;
constexpr int32 PERMAFROST_PLAYER_DURATION = 8000;

void ApplyPermafrostAura(Unit* unit, Aura* aura)
{
    uint32 id = aura->GetId();
    if (id != SPELL_PERMAFROST_RUNE && id != SPELL_PERMAFROST_MARKER)
        return;
    if (unit->IsPlayer() && aura->GetMaxDuration() > PERMAFROST_PLAYER_DURATION)
    {
        aura->SetMaxDuration(PERMAFROST_PLAYER_DURATION);
        aura->SetDuration(PERMAFROST_PLAYER_DURATION);
    }
    if (id != SPELL_PERMAFROST_RUNE)
        return;
    Player* caster = ObjectAccessor::FindPlayer(aura->GetCasterGUID());
    if (!caster || caster->getClass() != CLASS_SPIRIT_MAGE || !caster->HasAura(SPELL_RUNESHROUD, caster->GetGUID()))
        return;
    uint32 remaining = caster->GetSpellCooldownDelay(SPELL_PERMAFROST_RUNE);
    if (remaining)
        caster->ModifySpellCooldown(SPELL_PERMAFROST_RUNE, -int32(remaining * 4 / 5));
}

class runemaster_talent_events : public UnitScript
{
public:
    runemaster_talent_events() : UnitScript("runemaster_talent_events", true,
        {UNITHOOK_ON_AURA_APPLY, UNITHOOK_ON_AURA_REMOVE}) { }

    void OnAuraApply(Unit* unit, Aura* aura) override
    {
        if (unit && aura)
            ApplyPermafrostAura(unit, aura);
        Player* player = unit ? unit->ToPlayer() : nullptr;
        if (!player || player->getClass() != CLASS_SPIRIT_MAGE || !aura)
            return;
        uint32 id = aura->GetId();
        if (id == 707157 || id == 712310 || IsEarthTattoo(id))
            SyncStonePetroglyph(player);
        if (id == 500288 || id == 705565)
            SyncRuneshroudOrWaveforged(player);
    }

    void OnAuraRemove(Unit* unit, AuraApplication* application, AuraRemoveMode mode) override
    {
        if (unit && application && application->GetBase()->GetId() == SPELL_PERMAFROST_RUNE)
            unit->RemoveAurasDueToSpell(SPELL_PERMAFROST_MARKER, application->GetBase()->GetCasterGUID());
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
        if (id == 500288 || id == 705565)
            SyncRuneshroudOrWaveforged(player);
    }
};
}

void ApplyAscensionRunemasterTalentContracts(SpellInfo* info)
{
    if (info->Id == SPELL_PERMAFROST_RUNE)
    {
        info->AuraInterruptFlags |= AURA_INTERRUPT_FLAG_TAKE_DAMAGE;
        return;
    }
    if (info->Id != 712310 || info->SpellFamilyName != 38)
        return;
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
