/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "Creature.h"
#include "CreatureAI.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellInfo.h"

namespace
{
constexpr uint32 SPELL_THRESH = 505170;
constexpr uint32 SPELL_THRESH_TRANSFORM = 525058;
constexpr uint32 SPELL_BLOODSHATTER = 505326;
constexpr uint32 SPELL_BLOODSHATTER_TRANSFORM = 525299;
constexpr uint32 SPELL_SOULSTONE_LURE = 561376;
constexpr uint32 SPELL_SOULSTONE_LURE_AURA = 561826;
constexpr uint32 NPC_SOULSTONE_LURE = 557911;
constexpr uint32 SUMMON_PROPERTIES_STATIONARY = 64;
constexpr uint32 SPELL_DEATHBRINGER = 573040;
constexpr uint32 SPELL_HARVEST_TIME = 803995;

class reaper_spell_contracts : public GlobalScript
{
public:
    reaper_spell_contracts() : GlobalScript("reaper_spell_contracts",
        {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* info) override
    {
        if (!info || info->SpellFamilyName != 36)
            return;

        if (info->Id == SPELL_THRESH)
        {
            info->CasterAuraSpell = SPELL_THRESH_TRANSFORM;
            info->ExcludeCasterAuraSpell = SPELL_BLOODSHATTER_TRANSFORM;
        }
        else if (info->Id == SPELL_BLOODSHATTER)
            info->CasterAuraSpell = SPELL_BLOODSHATTER_TRANSFORM;
        else if (info->Id == SPELL_SOULSTONE_LURE && info->Effects[EFFECT_0].Effect == SPELL_EFFECT_SUMMON &&
            info->Effects[EFFECT_0].MiscValue == NPC_SOULSTONE_LURE)
            info->Effects[EFFECT_0].MiscValueB = SUMMON_PROPERTIES_STATIONARY;
        else if (info->Id == SPELL_DEATHBRINGER &&
            info->Effects[EFFECT_0].ApplyAuraName == SPELL_AURA_ADD_PCT_MODIFIER &&
            info->Effects[EFFECT_0].MiscValue == SPELLMOD_DURATION &&
            info->Effects[EFFECT_0].SpellClassMask == flag96(16777216, 0, 0))
            info->Effects[EFFECT_0].SpellClassMask = flag96(16777216, 536875008, 67108864);
        else if (info->Id == SPELL_HARVEST_TIME &&
            info->Effects[EFFECT_1].ApplyAuraName == SPELL_AURA_ADD_FLAT_MODIFIER &&
            info->Effects[EFFECT_1].MiscValue == SPELLMOD_CHANCE_OF_SUCCESS &&
            !info->Effects[EFFECT_1].SpellClassMask)
            info->Effects[EFFECT_1].ApplyAuraName = SPELL_AURA_DUMMY;
    }
};

class npc_ascension_reaper_soulstone_lure : public CreatureScript
{
public:
    npc_ascension_reaper_soulstone_lure() : CreatureScript("npc_ascension_reaper_soulstone_lure") { }

    struct LureAI : public CreatureAI
    {
        explicit LureAI(Creature* creature) : CreatureAI(creature) { }

        void IsSummonedBy(WorldObject* summoner) override
        {
            Player* player = summoner ? summoner->ToPlayer() : nullptr;
            if (!player || player->getClass() != CLASS_REAPER)
                return;

            me->SetOwnerGUID(player->GetGUID());
            me->SetCreatorGUID(player->GetGUID());
            me->SetFaction(player->GetFaction());
            me->SetReactState(REACT_PASSIVE);
            me->SetCombatMovement(false);
            me->CastSpell(me, SPELL_SOULSTONE_LURE_AURA, true);
        }

        void AttackStart(Unit*) override { }
        void UpdateAI(uint32) override { }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new LureAI(creature);
    }
};

}

void AddSC_AscensionReaperSpellContracts()
{
    new reaper_spell_contracts();
    new npc_ascension_reaper_soulstone_lure();
}
