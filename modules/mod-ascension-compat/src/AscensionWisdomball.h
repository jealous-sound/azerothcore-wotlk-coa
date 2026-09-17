/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#ifndef MOD_ASCENSION_WISDOMBALL_H
#define MOD_ASCENSION_WISDOMBALL_H

#include "Define.h"
#include <vector>

class Creature;
class Player;
class Quest;
class WorldPacket;

/// The Wondrous Wisdomball: the companion that, while it follows you, lists the quests of the
/// dungeon you are standing in - including the ones a chain or another prerequisite would
/// normally hide - and takes them back at the ball itself. It also lists the dungeon quests the
/// character is already carrying wherever the ball is summoned, so a finished one can be handed
/// in without going back. The ball is summoned by the spell the item teaches and owns no quest
/// relations, so its quest packets are answered here.
namespace AscensionWisdomball
{
    /// Creature entry of the ball; the summon spell's effect points at it.
    constexpr uint32 CreatureEntry = 79025;
    /// The summon the item teaches. A character that knows it can have a ball out.
    constexpr uint32 SummonSpell = 83050;
    /// The npc_text row the frame's greeting is drawn from (module SQL).
    constexpr uint32 GreetingTextId = 790250;

    [[nodiscard]] bool IsWisdomball(Creature const* creature);

    /// The ball this character currently has out, or null.
    [[nodiscard]] Creature* ActiveBall(Player* player);

    /// The nearest ball this character can work, whether they summoned it or are standing by
    /// somebody else's - one summon serves everyone within reach of it.
    [[nodiscard]] Creature* UsableBall(Player* player);

    /// Quests of this character's dungeon the character has not taken, which the frame lists
    /// first with the client's "!" beside each name.
    [[nodiscard]] std::vector<uint32> OfferedQuests(Player* player);
    /// Quests that belong to some dungeon and this character is carrying, in progress or ready.
    /// The frame draws these with the "?", the way a quest giver draws what is in your log.
    [[nodiscard]] std::vector<uint32> CarriedQuests(Player* player);
    /// Of those, the ones that are finished and can be handed in at the ball.
    [[nodiscard]] std::vector<uint32> TurnInQuests(Player* player);

    /// The mark the client draws over the ball: "!" when there is something to take, "?" when
    /// something is ready to hand in, nothing when it has neither for this character.
    [[nodiscard]] uint8 DialogStatus(Player* player, Creature* ball);

    /// Takes the quest with the prerequisites deliberately skipped - chains, exclusive groups,
    /// level and reputation - which is the whole point of the ball.
    bool Accept(Player* player, Creature* ball, Quest const* quest);

    /// Answers the quest packets the core refuses for the ball, which owns no quest relations
    /// of its own. Returns true when the packet belonged to a ball and has been handled here.
    bool HandlePacket(Player* player, WorldPacket const& packet);

    /// Forgets the per-character state (the mark last sent). Called when a character leaves.
    void Forget(Player* player);
}

#endif // MOD_ASCENSION_WISDOMBALL_H
