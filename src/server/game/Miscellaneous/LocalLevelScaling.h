/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information.
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU GPL.
 */

#ifndef AC_LOCAL_LEVEL_SCALING_H
#define AC_LOCAL_LEVEL_SCALING_H

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <mutex>
#include <optional>
#include <unordered_set>

class Creature;
class Player;

namespace LocalLevelScaling
{
inline std::atomic<bool> CreatureEnabled{false};
inline std::atomic<bool> QuestEnabled{false};
inline std::atomic<std::uint8_t> CreatureOffset{3};

// De combien de niveaux, au maximum, une creature peut etre remontee au-dessus
// du sien. Sans ce plafond, un joueur de niveau 30 traversant une zone de
// depart hissait chaque creature a 27 : les bots de bas niveau qui les
// engageaient se faisaient tuer par des creatures qui n'etaient pas les leurs.
//
// Une creature n'a qu'un seul niveau, diffuse a tous les clients : elle ne peut
// pas valoir 27 pour un joueur et 2 pour un bot. Borner la remontee est donc le
// seul compromis possible entre « le contenu reste pertinent » et « le monde
// reste jouable pour ceux qui y sont deja ».
//
// 0 desactive le plafond et rend le comportement d'origine.
//
// This ceiling bounds a level that is shared: the one creature every client is
// told about. A module that gives each character their own version of a creature
// raises CreatureScalingOwnedPerViewer below, and those levels are bounded by
// nothing, because one character's version of a creature is theirs alone.
inline std::atomic<std::uint8_t> CreatureMaxLift{5};

/// Set by the module that owns a character's scaling choice, when that ownership
/// is per viewer rather than realm wide.
///
/// It is how the two models stay out of each other's way: the realm-wide path
/// lifts the creature object itself, which every client is then told about, so a
/// character who never asked for scaling would see a raised world anyway. While
/// this is set, that path stands aside (its `CanScaleCreature` refuses) and the
/// per-viewer path answers instead - kept live rather than decided once at load,
/// so a `.reload config` moves between the two models without a restart.
inline std::atomic<bool> CreatureScalingOwnedPerViewer{false};

/// How much of the level-scaled reward a quest keeps when it is lifted from its own level to the
/// player's, at the extreme end of the range: the rest is lost in proportion to how much of that
/// range the quest spans, so a quest at the player's own level always keeps all of it. Both default
/// to 100, which is no discount at all, and both are set from a module's config.
///
/// Money carries a discount by default: the reward class (the tier) says nothing about level - the
/// median tier is the same in every level band - so paid at full strength a rich quest from a
/// starting zone would pay what a rich level-appropriate one pays while being trivial to complete.
/// A discount keeps the old zones worth playing without letting the easy content become the
/// profitable content, and because the discount only ever scales *down* from the price the class
/// pays at the player's level, old content can never out-pay level-appropriate content.
inline std::atomic<std::uint32_t> QuestMoneyKeepSharePercent{100};

/// Experience is left whole by default. The promise scaling makes is that scaled content always
/// awards experience, and levelling through the old zones is exactly what the system is for - a
/// discount here would put the dead zones back. Lower it to bound how much of a character's
/// progress can come from content far below them.
inline std::atomic<std::uint32_t> QuestXpKeepSharePercent{100};

/// The realm switches above are the default. A character can have a choice of their own, and
/// whichever module owns that choice installs a resolver here; the realm switches keep the final
/// say, because the resolver is only asked while they are on. With no resolver installed, or when
/// it has no opinion about the character, the realm-wide behaviour stands - which is what a
/// resolver returning true means.
using QuestScalingResolver = bool (*)(Player const*);
inline std::atomic<QuestScalingResolver> QuestScalingOwner{nullptr};

/// The character's own answer, with no realm switch in front of it: whether this character asked
/// for open-world scaling. The realm switches ask *whether* a character's choice is consulted at
/// all; this asks *what* it is, and is what the per-viewer combat paths resolve.
inline bool ScalingChoiceEnabled(Player const* player)
{
    QuestScalingResolver const owner = QuestScalingOwner.load(std::memory_order_relaxed);
    return owner && owner(player);
}

inline bool QuestScalingEnabled(Player const* player)
{
    if (!QuestEnabled.load(std::memory_order_relaxed))
        return false;

    QuestScalingResolver const owner = QuestScalingOwner.load(std::memory_order_relaxed);
    return !owner || owner(player);
}

/// One character's version of one creature, asked for by the few places in the core that compute a
/// fight and cannot see the viewer's object fields: the armor a blow lands against, today. Returns
/// the armor the viewer's version of the creature wears, or nullopt when the creature is already their
/// version of it.
///
/// The implementation stays with whichever module owns the character's choice, so the level, the
/// pool, the armor and the damage all come out of one place and cannot disagree.
using CreatureViewArmorResolver = std::optional<std::uint32_t> (*)(Player const*, Creature const*);
inline std::atomic<CreatureViewArmorResolver> CreatureViewArmorOwner{nullptr};

inline std::optional<std::uint32_t> ViewArmorFor(Player const* viewer, Creature const* creature)
{
    CreatureViewArmorResolver const owner = CreatureViewArmorOwner.load(std::memory_order_relaxed);
    return owner ? owner(viewer, creature) : std::nullopt;
}

/// The level one character's version of one creature stands at, or zero when that character sees
/// the authored creature.
///
/// This is the lever every level-derived term in a fight hangs off. `Unit::getLevelForTarget` is
/// the function the core asks "how high is this unit, *relative to me*" - spell hit and resistance
/// tables, weapon and defence skill (`GetMaxSkillValueForLevel`, `GetUnitMeleeSkill`, and therefore
/// `GetWeaponSkillValue` / `GetDefenseSkillValue` for a creature), the glancing and crushing tables,
/// stealth detection, aggro radius, and kill experience all read it. It is already virtual and
/// already overridden to give world bosses a target-relative level, which is exactly the shape a
/// per-viewer level needs: with a view installed, one character rolls against the level they are
/// fighting while everybody else rolls against the authored creature, from one definition.
///
/// Zero means "no view", so an unset owner, a character who never chose scaling, or a creature that
/// already is their version of it all keep the stock behaviour.
using CreatureViewLevelResolver = std::uint8_t (*)(Player const*, Creature const*);
inline std::atomic<CreatureViewLevelResolver> CreatureViewLevelOwner{nullptr};

inline std::uint8_t ViewLevelFor(Player const* viewer, Creature const* creature)
{
    CreatureViewLevelResolver const owner = CreatureViewLevelOwner.load(std::memory_order_relaxed);
    return owner ? owner(viewer, creature) : 0;
}

/// The max health one character's version of one creature has, or zero when that character sees the
/// authored creature. For effects worded as a share of "the creature's health", which the character
/// reads off the health bar they are shown.
using CreatureViewMaxHealthResolver = std::uint32_t (*)(Player const*, Creature const*);
inline std::atomic<CreatureViewMaxHealthResolver> CreatureViewMaxHealthOwner{nullptr};

inline std::uint32_t ViewMaxHealthFor(Player const* viewer, Creature const* creature)
{
    CreatureViewMaxHealthResolver const owner = CreatureViewMaxHealthOwner.load(std::memory_order_relaxed);
    return owner ? owner(viewer, creature) : 0;
}

inline std::uint8_t ScaleCreatureLevel(std::uint8_t originalLevel, std::uint8_t playerLevel,
    std::uint8_t offset = 3)
{
    std::uint8_t floor = playerLevel > offset ? playerLevel - offset : 1;

    // Le plafond s'applique AVANT le max : une creature deja plus haute que
    // originalLevel + lift garde son niveau, on ne la rabaisse jamais.
    std::uint8_t lift = CreatureMaxLift.load(std::memory_order_relaxed);
    if (lift)
    {
        std::uint32_t capped = static_cast<std::uint32_t>(originalLevel) + lift;
        if (floor > capped)
            floor = static_cast<std::uint8_t>(std::min<std::uint32_t>(capped, 255));
    }

    return std::max(originalLevel, floor);
}

/// The same rule as ScaleCreatureLevel, without the ceiling.
///
/// This is the level a *viewer* is shown for a creature. The ceiling above exists to bound one
/// character's effect on the level everybody else is told about, and a view is told to nobody else,
/// so the reason for it is absent: the creature in front of a character comes all the way up to
/// that character's band, which is the whole point of the feature (a starting-zone creature stays
/// relevant to the character standing in front of it).
///
/// A realm that also runs the realm-wide path keeps its ceiling for that path; the two are
/// independent, and CreatureScalingOwnedPerViewer is what keeps them from both answering.
inline std::uint8_t ScaleCreatureLevelForViewer(std::uint8_t originalLevel, std::uint8_t playerLevel,
    std::uint8_t offset = 3)
{
    std::uint8_t floor = playerLevel > offset ? playerLevel - offset : 1;
    return std::max(originalLevel, floor);
}

/// The viewer's rule inside a normal five-player dungeon, which also brings a creature down.
///
/// The dungeon finder admits a group to a classic dungeon from well below its authored level
/// (Scarlet Monastery - Cathedral from 20 against creatures of 36-40), so a dungeon creature is held
/// inside the viewer's band on both sides.
inline std::uint8_t ScaleDungeonCreatureLevelForViewer(std::uint8_t originalLevel, std::uint8_t playerLevel,
    std::uint8_t offset = 3)
{
    std::uint32_t const ceiling = std::uint32_t(playerLevel) + offset;
    std::uint8_t const lifted = ScaleCreatureLevelForViewer(originalLevel, playerLevel, offset);
    return static_cast<std::uint8_t>(std::min<std::uint32_t>(lifted, ceiling));
}

inline std::uint8_t ScaleQuestLevel(std::int32_t originalLevel, std::uint8_t playerLevel)
{
    if (originalLevel <= 0)
        return playerLevel;
    std::uint8_t questLevel = static_cast<std::uint8_t>(std::min<std::int32_t>(originalLevel, UINT8_MAX));
    return std::max(questLevel, playerLevel);
}

/// The share of a scaled reward a quest keeps: `floorPercent` at the far end of the level range,
/// growing to 100% at the player's own level, in proportion to how much of that range the quest
/// spans. A quest at or above the player's level - the case where scaling changes nothing - always
/// keeps the whole reward.
inline std::uint32_t RewardKeepPercent(std::uint32_t floorPercent, std::int32_t questLevel,
    std::uint8_t effectiveLevel)
{
    if (floorPercent >= 100 || questLevel <= 0 || effectiveLevel == 0 ||
        std::uint32_t(questLevel) >= effectiveLevel)
        return 100;

    std::uint32_t const sharePercent = std::uint32_t(questLevel) * 100 / effectiveLevel;
    return floorPercent + (100 - floorPercent) * sharePercent / 100;
}

// coa-gameplay-test summons every fixture creature into this phase, then gives it the level and the
// maximum health its scenario declared. Creature scaling does not assign a level, it rebuilds the
// creature through SelectLevel(), which recomputes maximum health from the template and throws that
// declared state away. The lift cap made this visible: a fixture declared at level 80 on a level 11
// template is rescaled down to level 16, and the hit the scenario was measuring kills it.
//
// A fixture is therefore left alone, unless its scenario asked for the opposite by declaring
// "level_scaling": true on the creature - which only the scenario that tests scaling itself does.
inline constexpr std::uint32_t FixturePhaseMask = 1u << 30;

inline std::mutex ScalableFixtureLock;
inline std::unordered_set<std::uint64_t> ScalableFixtures;

inline void AllowFixtureScaling(std::uint64_t guid)
{
    std::lock_guard<std::mutex> guard(ScalableFixtureLock);
    ScalableFixtures.insert(guid);
}

inline void ForgetFixture(std::uint64_t guid)
{
    std::lock_guard<std::mutex> guard(ScalableFixtureLock);
    ScalableFixtures.erase(guid);
}

// The phase test alone answers for every creature in the live world, so the lock is never taken
// outside a harness run.
inline bool IsUnscaledFixture(std::uint32_t phaseMask, std::uint64_t guid)
{
    if (!(phaseMask & FixturePhaseMask))
        return false;

    std::lock_guard<std::mutex> guard(ScalableFixtureLock);
    return ScalableFixtures.find(guid) == ScalableFixtures.end();
}
}

#endif
