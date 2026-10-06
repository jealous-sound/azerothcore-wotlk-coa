/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "CreatureAI.h"
#include "GameObject.h"
#include "Map.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "TemporarySummon.h"
#include <list>

namespace
{
enum VultureCultMoonwell : uint32
{
    QUEST_TRANSSUBSTANTIATING_THE_FLESH = 1660015,
    NPC_ABERRANT_FLESH_REMNANT = 161783,
    SAY_REMNANT_RISES = 0
};

enum VultureCultNewborns : uint32
{
    QUEST_A_TRAIL_OF_PETALS = 1660017,
    NPC_NEWBORN_SAVED = 161846,
    NPC_BUTTERFLY = 161847,
    SPELL_RESCUING_A_NEWBORN = 256715
};

constexpr uint32 KalimdorMapId = 1;
constexpr float MoonwellReach = 8.0f;
constexpr float RemnantSearchReach = 20.0f;
constexpr uint32 RemnantIdleLifetimeMs = 30000;
constexpr uint32 ButterflyLifetimeMs = 600000;
Position const EyrieMoonwell = { 10834.42f, 545.13f, 1396.39f, 0.0f };
Position const RemnantRise = { 10834.8f, 544.1f, 1396.25f, 3.14f };

Creature* FindOwnRemnant(Player* player)
{
    std::list<Creature*> remnants;
    player->GetCreatureListWithEntryInGrid(remnants, NPC_ABERRANT_FLESH_REMNANT, RemnantSearchReach);
    for (Creature* remnant : remnants)
        if (remnant->IsAlive() && remnant->GetSummonerGUID() == player->GetGUID())
            return remnant;

    return nullptr;
}

class spell_coa_channeling_blessing_of_the_moon : public SpellScript
{
    PrepareSpellScript(spell_coa_channeling_blessing_of_the_moon);

    SpellCastResult CheckAtEyrieMoonwell()
    {
        Player* player = GetCaster()->ToPlayer();
        if (!player || player->GetQuestStatus(QUEST_TRANSSUBSTANTIATING_THE_FLESH) != QUEST_STATUS_INCOMPLETE)
            return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

        if (player->GetMapId() != KalimdorMapId || !player->IsWithinDist3d(&EyrieMoonwell, MoonwellReach))
            return SPELL_FAILED_NOT_HERE;

        return SPELL_CAST_OK;
    }

    void RaiseAberrantFleshRemnant(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);

        Player* player = GetCaster()->ToPlayer();
        if (!player)
            return;

        if (Creature* remnant = FindOwnRemnant(player))
        {
            if (!remnant->IsInCombat())
                remnant->AI()->AttackStart(player);
            return;
        }

        if (TempSummon* remnant = player->SummonCreature(NPC_ABERRANT_FLESH_REMNANT, RemnantRise,
                                                         TEMPSUMMON_TIMED_DESPAWN_OOC_ALIVE, RemnantIdleLifetimeMs, 0,
                                                         nullptr, true))
        {
            remnant->SetReactState(REACT_DEFENSIVE);
            remnant->AI()->Talk(SAY_REMNANT_RISES, player);
            remnant->AI()->AttackStart(player);
        }
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_coa_channeling_blessing_of_the_moon::CheckAtEyrieMoonwell);
        OnEffectHit += SpellEffectFn(spell_coa_channeling_blessing_of_the_moon::RaiseAberrantFleshRemnant, EFFECT_0,
                                     SPELL_EFFECT_SEND_EVENT);
    }
};

class go_coa_baby_night_elf : public GameObjectScript
{
public:
    go_coa_baby_night_elf() : GameObjectScript("go_coa_baby_night_elf") { }

    bool OnGossipHello(Player* player, GameObject* baby) override
    {
        if (player->GetQuestStatus(QUEST_A_TRAIL_OF_PETALS) == QUEST_STATUS_INCOMPLETE)
            player->CastSpell(baby, SPELL_RESCUING_A_NEWBORN, false);

        return true;
    }
};

class spell_coa_rescuing_a_newborn : public SpellScript
{
    PrepareSpellScript(spell_coa_rescuing_a_newborn);

    void TransformNewborn(SpellEffIndex)
    {
        Player* player = GetCaster()->ToPlayer();
        GameObject* baby = GetHitGObj();
        if (!player || !baby || !baby->isSpawned() ||
            player->GetQuestStatus(QUEST_A_TRAIL_OF_PETALS) != QUEST_STATUS_INCOMPLETE)
            return;

        player->KilledMonsterCredit(NPC_NEWBORN_SAVED);
        player->SummonCreature(NPC_BUTTERFLY, baby->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, ButterflyLifetimeMs, 0,
                               nullptr, true);
        baby->DespawnOrUnsummon();
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_coa_rescuing_a_newborn::TransformNewborn, EFFECT_0,
                                           SPELL_EFFECT_DUMMY);
    }
};
}

void AddSC_AscensionVultureCultQuests()
{
    RegisterSpellScript(spell_coa_channeling_blessing_of_the_moon);
    RegisterSpellScript(spell_coa_rescuing_a_newborn);
    new go_coa_baby_night_elf();
}
