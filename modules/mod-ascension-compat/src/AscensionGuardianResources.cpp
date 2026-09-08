/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionGuardianResources.h"
#include "Player.h"
#include "Spell.h"
#include "SpellAuras.h"
#include "SpellInfo.h"

namespace
{
bool IsGuardianBalladCast(uint32 id)
{
    return id == 801776 || id == 801772 || (id >= 501066 && id <= 501074) ||
        (id >= 572717 && id <= 572719) || (id >= 574340 && id <= 574341) ||
        (id >= 574363 && id <= 574364);
}

void AddGuardianGlory(Player* player)
{
    player->CastSpell(player, 802302, true);
    Aura const* stacks = player->GetAura(802302);
    if (!stacks || stacks->GetStackAmount() < 3)
        return;

    player->RemoveAurasDueToSpell(802302);
    player->CastSpell(player, 802303, true);
    if (player->HasAura(705323))
        player->CastSpell(player, 707822, true);
}
}

void ApplyAscensionGuardianResourceContracts(SpellInfo* info)
{
    if (!info || info->SpellFamilyName != 24)
        return;

    // Native spell modifiers consume charges once per affected successful cast,
    // including delayed missiles. Do not add a spell_proc row for these auras:
    // Player::RemoveSpellMods delegates charge ownership when one is present.
    if (info->Id == 802303 && info->Effects[EFFECT_0].IsAura(SPELL_AURA_ADD_PCT_MODIFIER) &&
        info->Effects[EFFECT_0].MiscValue == SPELLMOD_DAMAGE && !info->ProcFlags)
    {
        info->ProcCharges = 1;
        if (info->Effects[EFFECT_1].TriggerSpell == 707822)
            info->Effects[EFFECT_1].Effect = 0;
    }
    if (info->Id == 707822 && info->Effects[EFFECT_0].IsAura(SPELL_AURA_ADD_PCT_MODIFIER) &&
        info->Effects[EFFECT_0].MiscValue == SPELLMOD_COST && !info->ProcFlags)
        info->ProcCharges = 2;

    if ((info->Id == 572820 || info->Id == 552780 || info->Id == 583027 || info->Id == 524615) &&
        !info->ProcFlags &&
        info->Effects[EFFECT_0].IsAura(SPELL_AURA_ADD_PCT_MODIFIER))
        info->ProcCharges = 1; // Harrowing Melody / Pied Piper: the next matching cast.

    if (info->Id == 807728 && info->Effects[EFFECT_0].IsAura(SPELL_AURA_ADD_FLAT_MODIFIER) &&
        info->Effects[EFFECT_0].MiscValue == SPELLMOD_EFFECT1 &&
        info->Effects[EFFECT_0].SpellClassMask == flag96(0, 4097, 0))
    {
        // This helper is present only with Footman's Calling and Tower/Line.
        // Use the same -10% on both formations, preserving its separate threat aura.
        info->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
        info->Effects[EFFECT_0].MiscValue = SPELL_SCHOOL_MASK_ALL;
        info->Effects[EFFECT_0].SpellClassMask = flag96();
    }

    if ((info->Id == 802286 || (info->Id >= 802734 && info->Id <= 802738)) &&
        info->Effects[EFFECT_1].Effect == SPELL_EFFECT_TRIGGER_SPELL &&
        info->Effects[EFFECT_1].TriggerSpell == 504721)
        info->Effects[EFFECT_1].Effect = 0; // Champion grants this conditionally below.

    if (info->Id == 504383 && info->Effects[EFFECT_1].IsAura(SPELL_AURA_ADD_FLAT_MODIFIER) &&
        info->Effects[EFFECT_1].MiscValue == SPELLMOD_CRITICAL_CHANCE &&
        info->Effects[EFFECT_1].BasePoints == 24 && info->Effects[EFFECT_1].DieSides == 1)
        info->Effects[EFFECT_1].BasePoints = -26; // With Honor: -25 percentage points.

    if ((info->Id == 504730 || info->Id == 504883) &&
        info->Effects[EFFECT_0].IsAura(SPELL_AURA_ADD_PCT_MODIFIER) &&
        info->Effects[EFFECT_0].MiscValue == SPELLMOD_CRIT_DAMAGE_BONUS &&
        info->Effects[EFFECT_0].SpellClassMask == flag96(0, 1074790400, 0))
        info->Effects[EFFECT_0].SpellClassMask[2] |= 2048; // Centurion Strike, all six ranks.

    if (info->Id == 803417 && info->Effects[EFFECT_2].IsAura(SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN) &&
        info->Effects[EFFECT_2].MiscValue == SPELL_SCHOOL_MASK_MAGIC)
        info->Effects[EFFECT_2].MiscValue = SPELL_SCHOOL_MASK_ALL;
}

void HandleAscensionGuardianResourceCast(Spell* spell)
{
    if (!spell)
        return;
    Player* player = spell->GetCaster()->ToPlayer();
    if (!player || player->getClass() != CLASS_GUARDIAN)
        return;

    uint32 id = spell->GetSpellInfo()->Id;
    if (id == 572612)
    {
        // Sound of War is itself a triggered AoE. These are per-cast grants,
        // independent of how many enemies it hits.
        if (player->HasAura(504782))
            player->CastSpell(player, 505198, true);
        if (player->HasAura(704525))
            player->CastSpell(player, 707904, true);
        if (player->HasAura(705350))
            player->CastSpell(player, 552780, true);
        return;
    }
    if (spell->IsTriggered())
        return;
    bool ram = id == 802284 || (id >= 573204 && id <= 573211);
    bool centurion = id == 802286 || (id >= 802734 && id <= 802738);
    if (centurion && player->HasAura(300972))
        player->CastSpell(player, 504721, true);
    if ((ram && player->HasAura(92106)) || (centurion && player->HasAura(301057)))
        AddGuardianGlory(player);

    if (!IsGuardianBalladCast(id) || !player->HasAura(92107))
        return;
    player->CastSpell(player, 560514, true);
    Aura const* tempo = player->GetAura(560514);
    if (!tempo || tempo->GetStackAmount() < 3)
        return;

    player->RemoveAurasDueToSpell(560514);
    player->CastSpell(player, 572611, true);
}

void RemoveAscensionGuardianResourceTalent(Player* player, std::uint32_t id)
{
    if (!player || player->getClass() != CLASS_GUARDIAN)
        return;

    switch (id)
    {
        case 92106:
        case 301057:
            if (!player->HasAura(id == 92106 ? 301057 : 92106))
            {
                player->RemoveAurasDueToSpell(802302);
                player->RemoveAurasDueToSpell(802303);
            }
            break;
        case 705323:
            player->RemoveAurasDueToSpell(707822);
            break;
        case 704322:
            player->RemoveAurasDueToSpell(707821);
            break;
        case 92107:
            player->RemoveAurasDueToSpell(560514);
            break;
        case 705350:
            player->RemoveAurasDueToSpell(552780);
            break;
        case 582937:
            player->RemoveAurasDueToSpell(583027);
            break;
        case 804047:
            player->RemoveAurasDueToSpell(524615);
            break;
        default:
            break;
    }
}
