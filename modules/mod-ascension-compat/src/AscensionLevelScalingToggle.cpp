/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

// Creature and quest level scaling are two process-wide switches in LocalLevelScaling, set once from
// mod_ascension_compat.conf when the world loads. Turning either of them off meant editing the file
// and restarting the world, which is a heavy way to answer "is this fight hard because of scaling?".
//
// This is a gossip NPC that flips both at runtime and remembers the choice, so a realm can toggle
// scaling the way it toggles anything else: by talking to someone.

#include "Creature.h"
#include "LocalLevelScaling.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "WorldState.h"

namespace
{
enum LevelScalingToggle : uint32
{
    // Persisted in `worldstates`. Zero means "never set", so the configured value still wins on a
    // fresh realm; 1 and 2 are the two answers, which keeps "off" distinguishable from "unset".
    WorldStateCreatureScaling = 2100001,
    WorldStateQuestScaling = 2100002,

    StateUnset = 0,
    StateOff = 1,
    StateOn = 2,

    // npc_text row added alongside the creature template.
    GossipTextScaling = 990010,

    ActionToggleScaling = GOSSIP_ACTION_INFO_DEF + 1
};

bool ResolveStored(uint32 worldStateId, bool configured)
{
    switch (sWorldState->getWorldState(worldStateId))
    {
        case StateOn:
            return true;
        case StateOff:
            return false;
        default:
            return configured;
    }
}

void Store(uint32 worldStateId, bool enabled)
{
    sWorldState->setWorldState(worldStateId, uint64(enabled ? StateOn : StateOff));
}

class npc_ascension_level_scaling_toggle : public CreatureScript
{
public:
    npc_ascension_level_scaling_toggle() : CreatureScript("npc_ascension_level_scaling_toggle") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        ClearGossipMenuFor(player);

        // One switch, not two. Creature and quest scaling are halves of the same thing - a world
        // that rises to meet the player - and a realm that wants one without the other has never
        // come up. Read the creature flag for the label; Toggle keeps both in step.
        bool const scaling = LocalLevelScaling::CreatureEnabled.load(std::memory_order_relaxed);

        AddGossipItemFor(player, GOSSIP_ICON_CHAT,
            scaling ? "Level scaling is ON - turn it off." : "Level scaling is OFF - turn it on.",
            GOSSIP_SENDER_MAIN, ActionToggleScaling);

        SendGossipMenuFor(player, GossipTextScaling, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
    {
        if (sender != GOSSIP_SENDER_MAIN)
        {
            CloseGossipMenuFor(player);
            return true;
        }

        if (action != ActionToggleScaling)
        {
            CloseGossipMenuFor(player);
            return true;
        }

        bool const enabled = !LocalLevelScaling::CreatureEnabled.load(std::memory_order_relaxed);
        LocalLevelScaling::CreatureEnabled.store(enabled, std::memory_order_relaxed);
        LocalLevelScaling::QuestEnabled.store(enabled, std::memory_order_relaxed);
        Store(WorldStateCreatureScaling, enabled);
        Store(WorldStateQuestScaling, enabled);

        // Creatures already in the world keep the level they spawned with either way; the switch
        // applies from the next spawn. Reopen the menu so the new state is visible immediately.
        return OnGossipHello(player, creature);
    }
};

// The configured values are written by AscensionCompatWorldScript::OnBeforeConfigLoad, which runs
// before the world states are loaded from the database. Re-apply the stored choice on top of them
// once both are available.
class AscensionLevelScalingToggleWorldScript : public WorldScript
{
public:
    AscensionLevelScalingToggleWorldScript()
        : WorldScript("AscensionLevelScalingToggleWorldScript", {WORLDHOOK_ON_STARTUP}) { }

    void OnStartup() override
    {
        LocalLevelScaling::CreatureEnabled.store(
            ResolveStored(WorldStateCreatureScaling,
                LocalLevelScaling::CreatureEnabled.load(std::memory_order_relaxed)),
            std::memory_order_relaxed);
        LocalLevelScaling::QuestEnabled.store(
            ResolveStored(WorldStateQuestScaling,
                LocalLevelScaling::QuestEnabled.load(std::memory_order_relaxed)),
            std::memory_order_relaxed);
    }
};
}

void AddSC_AscensionLevelScalingToggle()
{
    new npc_ascension_level_scaling_toggle();
    new AscensionLevelScalingToggleWorldScript();
}
