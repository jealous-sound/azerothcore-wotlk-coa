/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionClassMechanics26To32.h"
#include "Player.h"
#include "Random.h"
#include "Spell.h"
#include "SpellAuras.h"

namespace
{
constexpr std::uint32_t SPELL_SUN_CLERIC_SUNS_HOPE = 560276;
constexpr std::uint32_t SPELL_SUN_CLERIC_SOLAR_FLARE = 807175;
constexpr std::uint32_t SPELL_SUN_CLERIC_GAIN_SOLAR_POWER = 804098;
constexpr std::uint32_t SPELL_SUN_CLERIC_SUNS_HOPE_DEBUFF = 706733;
constexpr std::uint32_t SPELL_SUN_CLERIC_SOLAR_NOVA = 680621;
constexpr std::uint32_t SPELL_SUN_CLERIC_SOLAR_NOVA_RANK_2 = 681432;
constexpr std::uint32_t SPELL_SUN_CLERIC_SOLAR_NOVA_RANK_3 = 681433;
constexpr std::uint32_t SPELL_SUN_CLERIC_SOLAR_NOVA_COOLDOWN = 524858;
constexpr std::uint32_t SPELL_TINKER_COGMASTER = 504525;
constexpr std::uint32_t SPELL_TINKER_COGMASTER_SCRAP = 706535;
constexpr std::uint8_t TINKER_COGMASTER_CHANCE = 50;
constexpr std::uint32_t SPELL_REAPER_DIRGE_HIT = 801311;
constexpr std::uint32_t SPELL_REAPER_SOUL_GENERATOR = 520056;
constexpr std::int16_t REAPER_SOUL_GENERATOR_RUNIC_POWER = 100;
constexpr std::uint32_t SPELL_PRIMALIST_CAVE_IN = 500615;
constexpr std::uint32_t SPELL_PRIMALIST_EARTHSHAPING = 680441;

bool IsSuccessfulOffensiveHit(Player const* player, Unit const* target,
    std::uint8_t missInfo)
{
    // IsValidAttackTarget rejects some already-dead units. Killing blows are
    // still damage events and must generate the same resources as other hits.
    return player && target && player != target &&
        missInfo == SPELL_MISS_NONE && !player->IsFriendlyTo(target);
}

bool IsRangedWeaponAttack(SpellInfo const* spellInfo)
{
    // Match Spell's attack-type construction without an invented public
    // GetAttackType API. This includes Auto Shot and wand auto-repeat casts.
    return (spellInfo->DmgClass == SPELL_DAMAGE_CLASS_RANGED &&
        spellInfo->IsRangedWeaponSpell()) ||
        spellInfo->HasAttribute(SPELL_ATTR2_AUTO_REPEAT);
}

bool IsSolarNova(std::uint32_t spellId)
{
    return spellId == SPELL_SUN_CLERIC_SOLAR_NOVA ||
        spellId == SPELL_SUN_CLERIC_SOLAR_NOVA_RANK_2 ||
        spellId == SPELL_SUN_CLERIC_SOLAR_NOVA_RANK_3;
}
}

void HandleAscensionClassMechanics26To32Hit(Spell* spell, Player* player,
    Unit* target, std::uint8_t missInfo, std::uint32_t damage, bool critical)
{
    if (!spell || !IsSuccessfulOffensiveHit(player, target, missInfo))
        return;

    std::uint32_t spellId = spell->GetSpellInfo()->Id;
    switch (player->getClass())
    {
        case CLASS_SUN_CLERIC:
        {
            if (damage && spellId == SPELL_SUN_CLERIC_SOLAR_FLARE &&
                player->HasAura(SPELL_SUN_CLERIC_SUNS_HOPE))
            {
                // Solar Flare is normally a triggered child. Both authored
                // Sun's Hope proc auras have ProcFlags=0, so supply the event,
                // not another resource amount in the central gain table.
                player->CastSpell(player,
                    SPELL_SUN_CLERIC_GAIN_SOLAR_POWER, true);
                player->CastSpell(target,
                    SPELL_SUN_CLERIC_SUNS_HOPE_DEBUFF, true);
            }

            if (IsSolarNova(spellId))
            {
                // "For each target struck": include a landed fully absorbed
                // hit. Do not deduplicate the cast. The native helper reduces
                // each named cooldown by 1000 ms.
                player->CastSpell(player,
                    SPELL_SUN_CLERIC_SOLAR_NOVA_COOLDOWN, true);
            }
            break;
        }
        case CLASS_TINKER:
        {
            if (damage && critical && player->HasAura(SPELL_TINKER_COGMASTER) &&
                IsRangedWeaponAttack(spell->GetSpellInfo()) &&
                roll_chance_i(TINKER_COGMASTER_CHANCE))
            {
                // The authored helper has effect 175, delta 3, aura 801816.
                // Never also add a +3 row to ResourceGainRules.
                player->CastSpell(player, SPELL_TINKER_COGMASTER_SCRAP, true);
            }
            break;
        }
        case CLASS_REAPER:
        {
            if (damage && spellId == SPELL_REAPER_DIRGE_HIT && critical &&
                player->HasAura(SPELL_REAPER_SOUL_GENERATOR))
            {
                // Dirge's parent launches two effect-142 children. Only a
                // child critical strike satisfies the talent. Its own DBC
                // fragment grant remains intact and is not duplicated here.
                player->ModifyPower(POWER_RUNIC_POWER,
                    REAPER_SOUL_GENERATOR_RUNIC_POWER);
            }
            break;
        }
        default:
            break;
    }
}

void HandleAscensionClassMechanics26To32SuccessfulInterrupt(Spell* spell,
    Player* player)
{
    if (!spell || !player || player->getClass() != CLASS_WILDWALKER ||
        spell->GetSpellInfo()->Id != SPELL_PRIMALIST_CAVE_IN)
        return;

    if (Aura* earthshaping = player->GetAura(SPELL_PRIMALIST_EARTHSHAPING))
    {
        // Earthshaping explicitly forbids duration refresh on stack gain.
        // ModStackAmount refreshes native timers; restore the remaining time.
        std::int32_t remaining = earthshaping->GetDuration();
        earthshaping->ModStackAmount(1);
        earthshaping->SetDuration(remaining);
    }
    else
    {
        player->CastSpell(player, SPELL_PRIMALIST_EARTHSHAPING, true);
    }
}
