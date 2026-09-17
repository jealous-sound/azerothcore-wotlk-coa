/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

// Teleport tokens (the Stone of Retreat collection and similar items) teach their
// teleport spell only when the item is used, and the matching Ascension client does
// not let a player pick those spells up from its spellbook or collection panel.
//
// This module makes a token usable where a player expects it, and keeps the teleport
// it unlocks on a free action button:
//
//  * using a token casts the teleport that token teaches (see
//    TeleportActionBar.CastFromItem), so a stone on an action bar still teleports,
//  * every teleport token spell a character knows is placed on a free action button,
//    which the client shows, moves and removes like any other button,
//  * a stone whose hub belongs to the other faction is refused
//    (TeleportActionBar.EnforceFaction), from the spellbook, an action bar or the
//    item itself; shared hubs stay usable by both sides.
//
// The module never creates or grants items; the stones live in the client's
// collection UI and on the action bar only.

#include "Chat.h"
#include "CommandScript.h"
#include "Config.h"
#include "DataMap.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "RBAC.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "StoneFactionData.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace Acore::ChatCommands;

namespace
{
constexpr char const* LogCategory = "module.teleport_actionbar";

// Items apply their taught spell through this trigger (ITEM_SPELLTRIGGER_LEARN_SPELL).
constexpr uint32 LearnSpellTrigger = 6;

// Player::CastItemUseSpell routes every item whose first spell is this generic learn
// spell through this spell id, with the item as the cast item. The item's second
// spell is the spell it teaches.
constexpr uint32 LearnSpellTemplate = 55884;

// Learned spells emit client events, so carried tokens are taught in small batches
// instead of one bulk grant that can freeze the legacy client UI.
constexpr std::size_t LearnBatchSize = 4;
constexpr uint32 LearnBatchIntervalMs = 250;

// The client repeats a refused cast, so a refusal is only announced this often.
constexpr uint32 ForeignNoticeIntervalMs = 3000;

// Per-player storage key for the spells still waiting to be taught.
constexpr char PendingLearnKey[] = "teleport.actionbar.pending";

struct Settings
{
    bool Enabled = true;
    bool OnLogin = false;
    bool OnLearnSpell = false;
    bool OnSpecChange = false;
    bool LearnCarriedTokens = false;
    bool LearnFromBank = false;
    bool CastFromItem = true;
    bool EnforceFaction = true;
    uint8 FirstButton = 0;
    uint8 LastButton = MAX_ACTION_BUTTONS - 1;
    uint32 MaxButtons = 24;
};

struct PendingLearn : DataMap::Base
{
    std::vector<uint32> Spells;
    std::size_t Next = 0;
    uint32 Timer = 0;
};

Settings settings;

// spell taught by a token item -> that item, and the reverse per item.
std::unordered_map<uint32, uint32> tokenSpellItem;
std::unordered_map<uint32, uint32> tokenItemSpell;

// teleport spell -> the side allowed to use it (a StoneFaction::Side value).
std::unordered_map<uint32, uint8> tokenSpellSide;

// Last time each player was told a stone is not theirs.
std::unordered_map<ObjectGuid, uint32> lastForeignNotice;

bool tokenTeleportsLoaded = false;

void LoadSettings()
{
    settings.Enabled = sConfigMgr->GetOption<bool>("TeleportActionBar.Enable", true);
    settings.OnLogin = sConfigMgr->GetOption<bool>("TeleportActionBar.OnLogin", false);
    settings.OnLearnSpell = sConfigMgr->GetOption<bool>("TeleportActionBar.OnLearnSpell", false);
    settings.OnSpecChange = sConfigMgr->GetOption<bool>("TeleportActionBar.OnSpecChange", false);
    settings.LearnCarriedTokens = sConfigMgr->GetOption<bool>("TeleportActionBar.LearnCarriedTokens", false);
    settings.LearnFromBank = sConfigMgr->GetOption<bool>("TeleportActionBar.LearnCarriedTokensFromBank", false);
    settings.CastFromItem = sConfigMgr->GetOption<bool>("TeleportActionBar.CastFromItem", true);
    settings.EnforceFaction = sConfigMgr->GetOption<bool>("TeleportActionBar.EnforceFaction", true);
    settings.MaxButtons = sConfigMgr->GetOption<uint32>("TeleportActionBar.MaxButtons", 24);

    uint32 first = sConfigMgr->GetOption<uint32>("TeleportActionBar.FirstButton", 0);
    uint32 last = sConfigMgr->GetOption<uint32>("TeleportActionBar.LastButton", MAX_ACTION_BUTTONS - 1);
    last = std::min<uint32>(last, MAX_ACTION_BUTTONS - 1);
    if (first > last)
        std::swap(first, last);
    settings.FirstButton = uint8(first);
    settings.LastButton = uint8(last);
}

// The item name filter is a startup configuration value (never player input) and it
// only ever appears inside a read-only query, but quote characters are still dropped
// so a stray value cannot break the statement.
std::string SanitizeLikePattern(std::string const& pattern)
{
    std::string safe;
    safe.reserve(pattern.size());
    for (char character : pattern)
    {
        if (character == '\'' || character == '"' || character == '\\' || character == ';' || character == '`')
            continue;
        safe.push_back(character);
    }
    return safe;
}

void LoadTokenTeleports()
{
    tokenTeleportsLoaded = false;
    tokenSpellItem.clear();
    tokenItemSpell.clear();
    tokenSpellSide.clear();

    std::string const pattern = SanitizeLikePattern(
        sConfigMgr->GetOption<std::string>("TeleportActionBar.ItemNameFilter", "Stone of Retreat%"));
    std::string const patternClause = pattern.empty() ? std::string() : " AND `name` LIKE '" + pattern + "'";

    std::string query = "SELECT DISTINCT `item`, `spell` FROM (";
    for (uint32 slot = 1; slot <= MAX_ITEM_PROTO_SPELLS; ++slot)
    {
        std::string const number = std::to_string(slot);
        if (slot > 1)
            query += " UNION ";
        query += "SELECT `entry` AS `item`, `spellid_" + number + "` AS `spell` FROM `item_template` WHERE `spelltrigger_" + number + "` = " +
            std::to_string(LearnSpellTrigger) + " AND `spellid_" + number + "` <> 0" + patternClause;
    }
    query += ") AS `token` JOIN `spell_target_position` AS `destination` ON `destination`.`ID` = `token`.`spell`";

    // std::string_view instance, so the raw SQL is never run through the {}-style
    // argument formatter.
    QueryResult result = WorldDatabase.Query(std::string_view(query));
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 const itemId = fields[0].Get<uint32>();
            uint32 const spellId = fields[1].Get<uint32>();
            if (!sSpellMgr->GetSpellInfo(spellId))
                continue;
            tokenItemSpell[itemId] = spellId;
            tokenSpellItem.insert({spellId, itemId});
        } while (result->NextRow());
    }

    // Attach each token's side from the generated table (StoneFactionData.h). A token
    // the table does not know about stays usable by both rather than becoming dead.
    uint32 alliance = 0;
    uint32 horde = 0;
    uint32 shared = 0;
    uint32 unlisted = 0;
    for (auto const& entry : tokenSpellItem)
    {
        uint8 side = StoneFaction::Both;
        bool listed = false;
        for (uint32 i = 0; i < StoneFaction::TeleportFactionCount; ++i)
        {
            if (StoneFaction::TeleportFactions[i].Spell == entry.first)
            {
                side = StoneFaction::TeleportFactions[i].Side;
                listed = true;
                break;
            }
        }

        if (!listed)
            ++unlisted;
        else if (side == StoneFaction::Alliance)
            ++alliance;
        else if (side == StoneFaction::Horde)
            ++horde;
        else
            ++shared;

        tokenSpellSide[entry.first] = side;
    }

    tokenTeleportsLoaded = true;

    if (tokenSpellItem.empty())
    {
        LOG_WARN(LogCategory, "No item-taught teleport spells matched the item filter '{}'; the module stays idle",
            pattern.empty() ? std::string("<none>") : pattern);
        return;
    }

    LOG_INFO(LogCategory, "Loaded {} teleport token spell(s) from {} item(s) (item filter: '{}')",
        tokenSpellItem.size(), tokenItemSpell.size(), pattern.empty() ? std::string("<none>") : pattern);
    LOG_INFO(LogCategory, "Token sides: {} Alliance-only, {} Horde-only, {} shared{}", alliance, horde, shared,
        unlisted ? " (" + std::to_string(unlisted) + " token(s) are not in the faction table and stay usable by both)" : std::string());
}

// The player's own side, in StoneFaction terms.
uint8 PlayerSide(Player const* player)
{
    return player->GetTeamId() == TEAM_ALLIANCE ? StoneFaction::Alliance : StoneFaction::Horde;
}

char const* SideName(uint8 side)
{
    return side == StoneFaction::Alliance ? "Alliance" : "Horde";
}

char const* TokenHub(uint32 spellId)
{
    for (uint32 i = 0; i < StoneFaction::TeleportFactionCount; ++i)
        if (StoneFaction::TeleportFactions[i].Spell == spellId)
            return StoneFaction::TeleportFactions[i].Hub;
    return "(unknown hub)";
}

// True when the spell is a teleport token that belongs to the other faction. Shared
// hubs and spells that are not tokens at all are never restricted.
bool IsForeignToken(uint32 spellId, Player const* player)
{
    auto it = tokenSpellSide.find(spellId);
    if (it == tokenSpellSide.end() || it->second == StoneFaction::Both)
        return false;
    return it->second != PlayerSide(player);
}

void NotifyForeignToken(Player* player, uint32 spellId)
{
    auto it = tokenSpellSide.find(spellId);
    if (it == tokenSpellSide.end())
        return;

    uint32 const now = getMSTime();
    uint32& last = lastForeignNotice[player->GetGUID()];
    if (last && now - last < ForeignNoticeIntervalMs)
        return;
    last = now;

    ChatHandler handler(player->GetSession());
    handler.PSendSysMessage("Stone of Retreat: {} is a {} stone - your {} character cannot use it.",
        TokenHub(spellId), SideName(it->second), SideName(PlayerSide(player)));
}

bool FindFreeButton(Player* player, uint8& slot)
{
    for (uint16 candidate = settings.FirstButton; candidate <= settings.LastButton; ++candidate)
    {
        if (player->GetActionButton(uint8(candidate)))
            continue;
        slot = uint8(candidate);
        return true;
    }
    return false;
}

// Puts every known teleport token spell on a free action button. Buttons the player
// already arranged are never touched: only empty slots inside the configured range
// are used, and a spell that already sits on any button in that range is skipped.
uint32 AssignTokenButtons(Player* player, char const* reason)
{
    if (!settings.Enabled || !tokenTeleportsLoaded || !player || !player->IsInWorld())
        return 0;

    std::unordered_set<uint32> assigned;
    uint32 free = 0;
    for (uint16 slot = settings.FirstButton; slot <= settings.LastButton; ++slot)
    {
        ActionButton const* button = player->GetActionButton(uint8(slot));
        if (!button)
        {
            ++free;
            continue;
        }
        if (button->GetType() == ACTION_BUTTON_SPELL)
            assigned.insert(button->GetAction());
    }

    uint32 const budget = settings.MaxButtons ? std::min(settings.MaxButtons, free) : free;
    if (!budget)
        return 0;

    std::vector<uint32> pending;
    pending.reserve(tokenSpellItem.size());
    for (auto const& entry : tokenSpellItem)
        if (!assigned.contains(entry.first) && player->HasSpell(entry.first))
            pending.push_back(entry.first);

    // Stable order so repeated runs place the same spells on the same buttons.
    std::sort(pending.begin(), pending.end());

    uint32 placed = 0;
    for (uint32 spellId : pending)
    {
        if (placed >= budget)
            break;

        uint8 slot = 0;
        if (!FindFreeButton(player, slot))
            break;
        if (!player->addActionButton(slot, spellId, ACTION_BUTTON_SPELL))
            continue;

        ++placed;
        LOG_DEBUG(LogCategory, "Placed teleport spell {} on action button {} for {} ({})",
            spellId, slot, player->GetName(), reason);
    }

    if (!placed)
        return 0;

    player->SendActionButtons(1);
    LOG_INFO(LogCategory, "Placed {} teleport token spell(s) on the action bar of {} ({})",
        placed, player->GetName(), reason);
    return placed;
}

std::vector<uint32> CollectCarriedTokenSpells(Player* player)
{
    std::vector<uint32> spells;
    spells.reserve(tokenItemSpell.size());
    for (auto const& entry : tokenItemSpell)
    {
        uint32 const spellId = entry.second;
        if (player->HasSpell(spellId) || !player->GetItemCount(entry.first, settings.LearnFromBank))
            continue;
        // A stone of the other faction is not taught at all: the client would list it
        // and the cast would be refused anyway.
        if (settings.EnforceFaction && IsForeignToken(spellId, player))
            continue;
        spells.push_back(spellId);
    }

    std::sort(spells.begin(), spells.end());
    spells.erase(std::unique(spells.begin(), spells.end()), spells.end());
    return spells;
}

void LearnTokenSpellsNow(Player* player, std::vector<uint32> const& spells)
{
    if (!player || spells.empty())
        return;

    for (uint32 spellId : spells)
        player->learnSpell(spellId);
}

void QueueCarriedTokenSpells(Player* player)
{
    if (!player)
        return;

    std::vector<uint32> const spells = CollectCarriedTokenSpells(player);
    if (spells.empty())
        return;

    PendingLearn* state = player->CustomData.GetDefault<PendingLearn>(PendingLearnKey);
    state->Spells = spells;
    state->Next = 0;
    state->Timer = 0;
}

void DrainPendingTokenSpells(Player* player, uint32 diff)
{
    PendingLearn* state = player->CustomData.GetDefault<PendingLearn>(PendingLearnKey);
    if (state->Next >= state->Spells.size())
        return;

    state->Timer += diff;
    if (state->Timer < LearnBatchIntervalMs)
        return;
    state->Timer = 0;

    uint32 taught = 0;
    while (state->Next < state->Spells.size() && taught < LearnBatchSize)
    {
        player->learnSpell(state->Spells[state->Next]);
        ++state->Next;
        ++taught;
    }

    if (state->Next >= state->Spells.size())
        LOG_INFO(LogCategory, "Taught {} carried token spell(s) to {}", state->Spells.size(), player->GetName());
}

// Every spell in the world passes through this script, so the token rules apply to any
// spell id the database defines - no spell_script_names row per stone. Two jobs:
//   * refuse a token whose hub belongs to the other faction, whether it is cast from
//     the spellbook, from an action bar button, or by using the stone item itself;
//   * keep using a token item casting the teleport it unlocked.
class TeleportActionBarAllSpellScript : public AllSpellScript
{
public:
    TeleportActionBarAllSpellScript() : AllSpellScript("TeleportActionBarAllSpellScript",
        {ALLSPELLHOOK_ON_SPELL_CHECK_CAST, ALLSPELLHOOK_ON_CAST}) { }

    void OnSpellCheckCast(Spell* spell, bool /*strict*/, SpellCastResult& result) override
    {
        if (result != SPELL_CAST_OK || !settings.Enabled || !settings.EnforceFaction || !tokenTeleportsLoaded)
            return;

        Unit* caster = spell->GetCaster();
        if (!caster || !caster->IsPlayer())
            return;

        SpellInfo const* spellInfo = spell->GetSpellInfo();
        if (!spellInfo)
            return;

        uint32 checked = spellInfo->Id;
        if (spellInfo->Id == LearnSpellTemplate)
        {
            // A token item teaches its teleport through the generic learn spell, so the
            // item decides which stone is being asked for.
            if (Item* item = spell->m_CastItem)
            {
                auto it = tokenItemSpell.find(item->GetEntry());
                if (it != tokenItemSpell.end())
                    checked = it->second;
            }
        }

        Player* player = caster->ToPlayer();
        if (!IsForeignToken(checked, player))
            return;

        result = SPELL_FAILED_NOT_HERE;
        NotifyForeignToken(player, checked);
    }

    // Using a token casts the teleport that token unlocked, so the item keeps working
    // from an action bar item button after it taught its spell. Player::CastItemUseSpell
    // routes every item whose first spell is the generic learn spell through this spell
    // id, with the item as the cast item.
    void OnSpellCast(Spell* spell, Unit* caster, SpellInfo const* /*spellInfo*/, bool /*skipCheck*/) override
    {
        if (!settings.Enabled || !settings.CastFromItem || !tokenTeleportsLoaded)
            return;
        if (!caster || !caster->IsPlayer())
            return;

        Item* item = spell->m_CastItem;
        if (!item)
            return;

        auto it = tokenItemSpell.find(item->GetEntry());
        if (it == tokenItemSpell.end())
            return;

        Player* player = caster->ToPlayer();
        uint32 const teleportSpell = it->second;

        // The item's own learn effect runs before this hook, so the teleport is known
        // by now unless the template points at a spell the server does not have.
        if (!player->HasSpell(teleportSpell) || !sSpellMgr->GetSpellInfo(teleportSpell))
            return;

        LOG_DEBUG(LogCategory, "Token item {} used by {}: casting teleport {}", item->GetEntry(),
            player->GetName(), teleportSpell);
        player->CastSpell(player, teleportSpell, true);
    }
};

class TeleportActionBarWorldScript : public WorldScript
{
public:
    TeleportActionBarWorldScript() : WorldScript("TeleportActionBarWorldScript",
        {WORLDHOOK_ON_STARTUP, WORLDHOOK_ON_AFTER_CONFIG_LOAD}) { }

    void OnStartup() override
    {
        LoadSettings();
        LoadTokenTeleports();
    }

    void OnAfterConfigLoad(bool reload) override
    {
        LoadSettings();
        if (reload)
            LoadTokenTeleports();
    }
};

class TeleportActionBarPlayerScript : public PlayerScript
{
public:
    TeleportActionBarPlayerScript() : PlayerScript("TeleportActionBarPlayerScript",
        {PLAYERHOOK_ON_LOGIN, PLAYERHOOK_ON_LEARN_SPELL, PLAYERHOOK_ON_AFTER_SPEC_SLOT_CHANGED,
         PLAYERHOOK_ON_UPDATE, PLAYERHOOK_ON_LOGOUT}) { }

    void OnPlayerLogin(Player* player) override
    {
        if (!settings.Enabled)
            return;

        if (settings.LearnCarriedTokens)
            QueueCarriedTokenSpells(player);
        if (settings.OnLogin)
            AssignTokenButtons(player, "login");
    }

    void OnPlayerLearnSpell(Player* player, uint32 spellId) override
    {
        if (!settings.Enabled || !settings.OnLearnSpell || !tokenSpellItem.contains(spellId))
            return;
        AssignTokenButtons(player, "teleport token learned");
    }

    void OnPlayerAfterSpecSlotChanged(Player* player, uint8 /*newSlot*/) override
    {
        if (!settings.Enabled || !settings.OnSpecChange)
            return;
        AssignTokenButtons(player, "spec change");
    }

    void OnPlayerUpdate(Player* player, uint32 diff) override
    {
        if (!settings.Enabled || !settings.LearnCarriedTokens)
            return;
        DrainPendingTokenSpells(player, diff);
    }

    void OnPlayerLogout(Player* player) override
    {
        player->CustomData.Erase(PendingLearnKey);
        lastForeignNotice.erase(player->GetGUID());
    }
};

class TeleportActionBarCommandScript : public CommandScript
{
public:
    TeleportActionBarCommandScript() : CommandScript("TeleportActionBarCommandScript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable const teleportBarCommandTable =
        {
            { "assign", HandleTeleportBarAssignCommand, SEC_PLAYER, Console::No },
            { "learn",  HandleTeleportBarLearnCommand,  SEC_PLAYER, Console::No },
            { "sides",  HandleTeleportBarSidesCommand,  SEC_PLAYER, Console::No },
            { "",       HandleTeleportBarAssignCommand, SEC_PLAYER, Console::No },
        };

        static ChatCommandTable const commandTable =
        {
            { "teleportbar", teleportBarCommandTable },
        };
        return commandTable;
    }

    static bool HandleTeleportBarAssignCommand(ChatHandler* handler)
    {
        Player* player = handler->GetPlayer();
        if (!player)
            return false;

        uint32 const placed = AssignTokenButtons(player, "command");
        handler->PSendSysMessage("Teleport tokens: placed {} button(s).", placed);
        return true;
    }

    static bool HandleTeleportBarLearnCommand(ChatHandler* handler)
    {
        Player* player = handler->GetPlayer();
        if (!player)
            return false;

        if (!tokenTeleportsLoaded)
        {
            handler->SendSysMessage("Teleport token data is not loaded.");
            return true;
        }

        // An explicit request, so carried tokens are taught regardless of
        // TeleportActionBar.LearnCarriedTokens and without batching.
        std::vector<uint32> const spells = CollectCarriedTokenSpells(player);
        LearnTokenSpellsNow(player, spells);
        uint32 const placed = AssignTokenButtons(player, "command learn");
        handler->PSendSysMessage("Teleport tokens: learned {} spell(s), placed {} button(s).",
            uint32(spells.size()), placed);
        return true;
    }

    // Reports how the stones this character knows are split, which is the quickest way
    // to see whether the faction rule is reading the hubs the way the player expects.
    static bool HandleTeleportBarSidesCommand(ChatHandler* handler)
    {
        Player* player = handler->GetPlayer();
        if (!player)
            return false;

        if (!tokenTeleportsLoaded)
        {
            handler->SendSysMessage("Teleport token data is not loaded.");
            return true;
        }

        uint32 mine = 0;
        uint32 shared = 0;
        uint32 foreign = 0;
        std::string foreignHubs;
        for (auto const& entry : tokenSpellItem)
        {
            uint32 const spellId = entry.first;
            if (!player->HasSpell(spellId))
                continue;

            auto side = tokenSpellSide.find(spellId);
            if (side == tokenSpellSide.end() || side->second == StoneFaction::Both)
            {
                ++shared;
                continue;
            }

            if (side->second == PlayerSide(player))
            {
                ++mine;
                continue;
            }

            ++foreign;
            if (foreign <= 5)
            {
                foreignHubs += foreignHubs.empty() ? "" : ", ";
                foreignHubs += TokenHub(spellId);
            }
        }

        if (!foreignHubs.empty())
            foreignHubs = " (" + foreignHubs + (foreign > 5 ? ", ..." : "") + ")";

        handler->PSendSysMessage("Stones you know: {} for your faction, {} shared, {} of the other faction{}.",
            mine, shared, foreign, foreignHubs);
        return true;
    }
};
} // namespace

void AddTeleportActionBarScripts()
{
    new TeleportActionBarAllSpellScript();
    new TeleportActionBarWorldScript();
    new TeleportActionBarPlayerScript();
    new TeleportActionBarCommandScript();
}
