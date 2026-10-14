/*
 * White-Glove Mischief (quest 9920010): Little Mickey's Flask (item 559857) casts
 * Open Flask (spell 365022), a SEND_EVENT spell whose event id 3839 is already used
 * by an unrelated stock event script, so the effect is replaced here.
 *
 * The flask only opens inside the Petitioner's Chamber of Stormwind Keep while the
 * quest is in progress. Opening it credits the chamber objective and turns the
 * Flask into the Empty Flask the quest asks for.
 */

#include "CreatureScript.h"
#include "GameObject.h"
#include "GameObjectScript.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "SpellScript.h"
#include "SpellScriptLoader.h"
#include <map>
#include <set>
#include <unordered_map>

namespace
{
    constexpr uint32 QUEST_WHITE_GLOVE_MISCHIEF = 9920010;
    constexpr uint32 NPC_PETITIONERS_CHAMBER_CREDIT = 9920003;
    constexpr uint32 ITEM_FLASK = 559857;
    constexpr uint32 ITEM_EMPTY_FLASK = 559858;

    constexpr uint32 CHAMBER_MAP = 0;
    constexpr float CHAMBER_X = -8314.038f;
    constexpr float CHAMBER_Y = 292.43518f;
    constexpr float CHAMBER_Z = 126.93212f;
    constexpr float CHAMBER_RADIUS = 25.0f;
}

class spell_coa_open_little_mickeys_flask : public SpellScript
{
    PrepareSpellScript(spell_coa_open_little_mickeys_flask);

    SpellCastResult CheckCast()
    {
        Player* player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
        if (!player || player->GetQuestStatus(QUEST_WHITE_GLOVE_MISCHIEF) != QUEST_STATUS_INCOMPLETE)
            return SPELL_FAILED_DONT_REPORT;

        if (player->GetMapId() != CHAMBER_MAP ||
            player->GetDistance(CHAMBER_X, CHAMBER_Y, CHAMBER_Z) > CHAMBER_RADIUS)
            return SPELL_FAILED_INCORRECT_AREA;

        return SPELL_CAST_OK;
    }

    void HandleOpen(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);

        Player* player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
        if (!player)
            return;

        player->KilledMonsterCredit(NPC_PETITIONERS_CHAMBER_CREDIT);
        player->DestroyItemCount(ITEM_FLASK, 1, true);
        GetSpell()->m_CastItem = nullptr;
        GetSpell()->m_castItemGUID.Clear();
        player->AddItem(ITEM_EMPTY_FLASK, 1);
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_coa_open_little_mickeys_flask::CheckCast);
        OnEffectHit += SpellEffectFn(spell_coa_open_little_mickeys_flask::HandleOpen, EFFECT_0,
            SPELL_EFFECT_SEND_EVENT);
    }
};

/*
 * Voices at the North Gate (quest 175198): five citizens at Stormwind's north gate
 * show their own gossip text. While the quest is in progress each offers one option
 * that closes the window, counts once towards "Concerned citizens informed" and,
 * for the citizens whose reply was captured, says it in a chat bubble.
 */
namespace
{
    constexpr uint32 QUEST_VOICES_AT_THE_NORTH_GATE = 175198;
    constexpr uint32 NPC_CONCERNED_CITIZEN_INFORMED = 9920020;
    constexpr uint32 GOSSIP_ACTION_INFORM = GOSSIP_ACTION_INFO_DEF + 1;

    struct NorthGateCitizen
    {
        uint32 npcTextId;
        bool hasReply;
    };

    std::map<uint32, NorthGateCitizen> const NorthGateCitizens =
    {
        { 164981, { 9920032, true  } },
        { 164982, { 9920034, false } },
        { 164983, { 9920031, false } },
        { 164984, { 9920033, false } },
        { 164985, { 9920030, true  } },
    };

    std::unordered_map<ObjectGuid::LowType, std::set<uint32>> InformedCitizens;
}

class npc_north_gate_citizen : public CreatureScript
{
public:
    npc_north_gate_citizen() : CreatureScript("npc_north_gate_citizen") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        auto citizen = NorthGateCitizens.find(creature->GetEntry());
        if (citizen == NorthGateCitizens.end())
            return false;

        ClearGossipMenuFor(player);
        std::set<uint32>& informed = InformedCitizens[player->GetGUID().GetCounter()];
        if (!player->GetReqKillOrCastCurrentCount(QUEST_VOICES_AT_THE_NORTH_GATE, NPC_CONCERNED_CITIZEN_INFORMED))
            informed.clear();

        if (player->GetQuestStatus(QUEST_VOICES_AT_THE_NORTH_GATE) == QUEST_STATUS_INCOMPLETE &&
            !informed.count(creature->GetEntry()))
            AddGossipItemFor(player, GOSSIP_ICON_CHAT,
                "The House of Nobles is taking action to resolve the situation.",
                GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFORM);

        SendGossipMenuFor(player, citizen->second.npcTextId, creature);
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        CloseGossipMenuFor(player);
        auto citizen = NorthGateCitizens.find(creature->GetEntry());
        if (action != GOSSIP_ACTION_INFORM || citizen == NorthGateCitizens.end() ||
            player->GetQuestStatus(QUEST_VOICES_AT_THE_NORTH_GATE) != QUEST_STATUS_INCOMPLETE)
            return true;

        if (!InformedCitizens[player->GetGUID().GetCounter()].insert(creature->GetEntry()).second)
            return true;

        player->KilledMonsterCredit(NPC_CONCERNED_CITIZEN_INFORMED);
        if (citizen->second.hasReply && creature->AI())
            creature->AI()->Talk(0, player);

        return true;
    }
};

/*
 * From Grain to Flour (quest 9920021): using the Dunshire Mill millstone (object 2300641)
 * makes the player cast Grinding the Wheat (spell 365012, 12 s, dummy effect on the
 * millstone). Finishing the cast yields Very Fine and Smooth Flour, and Miller Bartren
 * cheers the player on.
 */
namespace
{
    constexpr uint32 QUEST_FROM_GRAIN_TO_FLOUR = 9920021;
    constexpr uint32 SPELL_GRINDING_THE_WHEAT = 365012;
    constexpr uint32 ITEM_FINE_FLOUR = 559862;
    constexpr uint32 NPC_MILLER_BARTREN = 164035;
    constexpr uint8 SAY_MILLER_FLOUR_DONE = 1;
}

class go_dunshire_mill : public GameObjectScript
{
public:
    go_dunshire_mill() : GameObjectScript("go_dunshire_mill") { }

    bool OnGossipHello(Player* player, GameObject* go) override
    {
        if (player->GetQuestStatus(QUEST_FROM_GRAIN_TO_FLOUR) == QUEST_STATUS_INCOMPLETE &&
            !player->HasItemCount(ITEM_FINE_FLOUR, 1))
            player->CastSpell(go, SPELL_GRINDING_THE_WHEAT, false);

        return true;
    }
};

class spell_coa_grinding_the_wheat : public SpellScript
{
    PrepareSpellScript(spell_coa_grinding_the_wheat);

    void HandleGrind(SpellEffIndex /*effIndex*/)
    {
        Player* player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
        if (!player || player->GetQuestStatus(QUEST_FROM_GRAIN_TO_FLOUR) != QUEST_STATUS_INCOMPLETE)
            return;

        player->AddItem(ITEM_FINE_FLOUR, 1);
        if (Creature* miller = player->FindNearestCreature(NPC_MILLER_BARTREN, 40.0f))
            if (miller->AI())
                miller->AI()->Talk(SAY_MILLER_FLOUR_DONE, player);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_coa_grinding_the_wheat::HandleGrind, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

void AddSC_spell_coa_stormwind_quests()
{
    RegisterSpellScript(spell_coa_open_little_mickeys_flask);
    new npc_north_gate_citizen();
    new go_dunshire_mill();
    RegisterSpellScript(spell_coa_grinding_the_wheat);
}
