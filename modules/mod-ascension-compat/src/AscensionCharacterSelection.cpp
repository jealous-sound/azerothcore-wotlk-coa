/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "AscensionCharacterSelection.h"
#include "Config.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "Player.h"
#include "QueryResult.h"
#include "StringFormat.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

namespace
{
    // Extensions.dll 2026-07 build: opcodes confirmed from the client senders
    // (push 0x72E in RequestActivate @0x10191540, push 0x72F in RequestDeactivate
    // @0x10191650, push 0x772 in SetCharacterSelectionSortOrder) and the client's
    // opcode name table (SMSG_CHARACTER_LIST_INFO @0x075E, ..._SORT_ORDER @0x076F,
    // ..._MAIL @0x0770, ..._GAME_MODE @0x0771, CMSG_..._SET_SORT_ORDER @0x0772).
    // The character-selection session has no Player object, so requests are
    // validated against the account id.
    constexpr uint16 CMSG_ASCENSION_CHARACTER_ACTIVATE = 0x072E;
    constexpr uint16 CMSG_ASCENSION_CHARACTER_DEACTIVATE = 0x072F;
    constexpr uint16 CMSG_ASCENSION_CHARACTER_SORT_ORDER = 0x0772;
    constexpr uint16 SMSG_ASCENSION_CHARACTER_LIST_INFO = 0x075E;
    constexpr uint16 SMSG_ASCENSION_CHARACTER_ACTIVATE_RESULT = 0x075F;
    constexpr uint16 SMSG_ASCENSION_CHARACTER_DEACTIVATE_RESULT = 0x0760;
    constexpr uint16 SMSG_ASCENSION_CHARACTER_SORT_ORDER = 0x076F;
    constexpr uint16 SMSG_ASCENSION_CHARACTER_SELECTION_MAIL = 0x0770;
    constexpr uint16 SMSG_ASCENSION_CHARACTER_SELECTION_GAME_MODE = 0x0771;

    // Realm-info cluster that the live server sends around the character list
    // (verified against the 2026-09 live capture, wire order):
    //   SMSG 0x09D0 (4 zero bytes) — flushes the client's pending selection
    //                                state; registered unconditionally by
    //                                Extensions.dll.
    //   SMSG 0x09BC (57 bytes)     — realm parameters. Extensions.dll's handler
    //                                (0x102FC6C0) parses this payload and copies
    //                                the eight feature-gate bytes into
    //                                DLL+0xBDB178..0xBDB17F, which is what lets
    //                                the Character Advancement UI render trees at
    //                                all; the realm name and rates land in the
    //                                realm object at DLL+0xBDB138. Without this
    //                                packet the custom feature set stays
    //                                uninitialized.
    //   SMSG 0x06E5 (9 zero bytes) — the live server also sends its
    //                                chat-infraction notice here with the
    //                                "no pending infractions" defaults.
    constexpr uint16 SMSG_ASCENSION_SELECTION_RESET = 0x09D0;
    constexpr uint16 SMSG_ASCENSION_REALM_INFO = 0x09BC;
    constexpr uint16 SMSG_ASCENSION_CHAT_INFRACTION_UPDATE = 0x06E5;

    // Enum.CharacterSelect (SharedXML\Enum.lua, mirrored by the client): team ids
    // are the classic faction-group ids and the ruleset decides which banner the
    // character list draws next to the name.
    constexpr uint32 CHARACTER_SELECTION_FACTION_ALLIANCE = 469;
    constexpr uint32 CHARACTER_SELECTION_FACTION_HORDE = 67;
    constexpr uint8 CHARACTER_SELECTION_RULESET_NONE = 0;
    constexpr uint8 CHARACTER_SELECTION_RULESET_HIGH_RISK = 1;

    // Rulesets are persisted as auras on the character (AscensionRulesets.cpp).
    constexpr uint32 SPELL_ASCENSION_HIGH_RISK = 1004019;

    // Result codes are matched verbatim by the client string tables
    // (Extensions.dll VA 0x10B38B38 for activate, 0x10B38A1C for deactivate).
    constexpr char ACTIVATE_CHARACTER_OK[] = "ACTIVATE_CHARACTER_OK";
    constexpr char ACTIVATE_CHARACTER_NOT_FOUND[] = "ACTIVATE_CHARACTER_NOT_FOUND";
    constexpr char ACTIVATE_CHARACTER_NOT_OWNED[] = "ACTIVATE_CHARACTER_NOT_OWNED";
    constexpr char ACTIVATE_CHARACTER_ALREADY_ACTIVE[] = "ACTIVATE_CHARACTER_ALREADY_ACTIVE";
    constexpr char ACTIVATE_CHARACTER_MAX_ACTIVE[] = "ACTIVATE_CHARACTER_MAX_ACTIVE";
    constexpr char ACTIVATE_CHARACTER_ONLINE[] = "ACTIVATE_CHARACTER_ONLINE";
    constexpr char ACTIVATE_CHARACTER_FAILED[] = "ACTIVATE_CHARACTER_FAILED";

    constexpr char DEACTIVATE_CHARACTER_OK[] = "DEACTIVATE_CHARACTER_OK";
    constexpr char DEACTIVATE_CHARACTER_NOT_FOUND[] = "DEACTIVATE_CHARACTER_NOT_FOUND";
    constexpr char DEACTIVATE_CHARACTER_NOT_OWNED[] = "DEACTIVATE_CHARACTER_NOT_OWNED";
    constexpr char DEACTIVATE_CHARACTER_ALREADY_INACTIVE[] = "DEACTIVATE_CHARACTER_ALREADY_INACTIVE";
    constexpr char DEACTIVATE_CHARACTER_ONLINE[] = "DEACTIVATE_CHARACTER_ONLINE";
    constexpr char DEACTIVATE_CHARACTER_FAILED[] = "DEACTIVATE_CHARACTER_FAILED";

    // The client clamps the reported character-list maximum to 0x80 entries.
    constexpr uint32 CHARACTER_LIST_MAXIMUM = 128;
    constexpr std::size_t SORT_ORDER_PAYLOAD_MAXIMUM = 1023;

    bool CharacterSelectionEnabled()
    {
        return sConfigMgr->GetOption<bool>("AscensionCompat.CharacterSelectionEnable", true);
    }

    uint32 CharacterSelectionMaxActive()
    {
        uint32 const maximum = sConfigMgr->GetOption<uint32>(
            "AscensionCompat.CharacterSelectionMaxActive", 10);
        return std::clamp(maximum, uint32(1), CHARACTER_LIST_MAXIMUM);
    }

    // Challenge modes as the client renders them (Enum.GameMode in
    // SharedXML\Enum.lua): Random 0x01, Ironman 0x02, Survivalist 0x04,
    // Draft 0x08, Resolute 0x20, WildCard 0x40, Felforged 0x80, Nightmare
    // 0x100, FreepickRarities 0x400, BuildDraft 0x800, Crusader 0x1000.
    // This fork does not persist per-character challenge modes yet, so the
    // client is told "no modes" and simply leaves the mode line empty.
    uint32 CharacterSelectionActiveGameModes()
    {
        return 0;
    }

    uint32 CharacterSelectionEnabledGameModes()
    {
        return 0;
    }

    // The client compares this against Enum.CharacterSelect.Faction
    // (Alliance = 469, Horde = 67, Other = 0) to pick the banner icon.
    uint32 CharacterSelectionTeamId(uint8 race)
    {
        switch (Player::TeamIdForRace(race))
        {
            case TEAM_ALLIANCE:
                return CHARACTER_SELECTION_FACTION_ALLIANCE;
            case TEAM_HORDE:
                return CHARACTER_SELECTION_FACTION_HORDE;
            default:
                return 0;
        }
    }

    // Realm parameters (SMSG 0x09BC), byte-for-byte the live layout: u32, u32,
    // five floats, u32 (realm rates and two auction values, matching the
    // AscensionCoAConfigData payloads), the eight feature-gate bytes the client
    // copies into DLL+0xBDB178, the empty first string, the realm name (client
    // caches it at realm-object +0x4C), one byte and the trailing u32. The 0x09D0
    // reset precedes the cluster and 0x06E5 carries the live "no pending chat
    // infraction" defaults (empty name, zeroed fields).
    void SendAscensionRealmInfo(WorldSession* session)
    {
        WorldPacket reset(SMSG_ASCENSION_SELECTION_RESET, 4);
        reset << uint32(0);
        session->SendPacket(&reset);

        WorldPacket realm(SMSG_ASCENSION_REALM_INFO, 64);
        realm << uint32(20);        // live 0x14; client stores at realm-object +0x04
        realm << uint32(0);         // ruleset 0 = classic (level cap 60)
        realm << float(1.26f);      // RATE_XP_QUEST
        realm << float(1.0f);
        realm << float(1.0f);
        realm << uint32(0);         // realm-object +0x18 = (value != 0)
        realm << float(1.0f);
        realm << float(0.0833f);    // RATE_AUCTION_DEPOSIT_VANITY
        realm << uint32(200000);    // CONFIG_MAX_AUCTION_DEPOSIT_VANITY
        uint8 const featureGates[8] = { 1, 0, 0, 0, 0, 0, 1, 0 };
        realm.append(featureGates, 8);
        realm << std::string();     // first string is empty on live as well
        // Live sends the realm name here; this core never calls
        // World::SetRealmName, so the option provides it (empty = empty field).
        realm << sConfigMgr->GetOption<std::string>(
            "AscensionCompat.RealmName", sWorld->GetRealmName());
        realm << uint8(1);          // realm-object +0x48
        realm << uint32(40);        // realm-object +0x64 (value as on live)
        session->SendPacket(&realm);

        WorldPacket infraction(SMSG_ASCENSION_CHAT_INFRACTION_UPDATE, 9);
        infraction << uint32(0) << std::string() << uint32(0);
        session->SendPacket(&infraction);
    }

    // Per-character extras, mirroring the live server: one SMSG 0x0771 (game
    // mode) and one SMSG 0x0770 (mail) per character, sent before SMSG 0x075E.
    //
    // The leading u32 is not the guid but the character's 1-based position in the
    // character list: CharacterSelect.lua resolves each row through
    // GetCharIDFromIndex() (translationTable, built from 1..GetNumCharacters())
    // and passes that value to GetCharacterSelectionGameModeData(),
    // Extensions.dll hashes it straight into its per-character table. Inactive
    // characters are absent from that list (CHAR_SEL_ENUM filters them), so only
    // active characters get an entry and they are numbered consecutively.
    //
    // isMercenary: the client's own name for the byte after teamId
    // (CharacterSelect.lua reads it from GetCharacterSelectionGameModeData, and
    // Extensions.dll keeps it at struct +0x18 and hands it back as a boolean).
    // Its UI never uses it: that read is the only mercenary reference in the
    // shipped interface, neither client binary nor the server data has a
    // mercenary system, and the live capture has 0 for every character. Whether
    // it means own-faction PvP is unconfirmed, so 0 is sent.
    void SendCharacterSelectionPerCharacter(WorldSession* session, uint32 listIndex, uint8 race,
        bool highRisk, bool hasMail)
    {
        WorldPacket gameMode(SMSG_ASCENSION_CHARACTER_SELECTION_GAME_MODE, 18);
        gameMode << listIndex
                 << CharacterSelectionActiveGameModes()      // activeGameModes
                 << CharacterSelectionEnabledGameModes()     // enabledGameModes
                 << CharacterSelectionTeamId(race)           // teamId (banner)
                 << uint8(0)                                 // isMercenary
                 << uint8(highRisk ? CHARACTER_SELECTION_RULESET_HIGH_RISK
                                   : CHARACTER_SELECTION_RULESET_NONE);
        session->SendPacket(&gameMode);

        WorldPacket mail(SMSG_ASCENSION_CHARACTER_SELECTION_MAIL, 6);
        mail << listIndex
             << uint8(hasMail ? 1 : 0)                       // hasMail
             << uint8(0);                                    // hasStoreMail (not tracked by this fork)
        session->SendPacket(&mail);
    }

    void SendResult(WorldSession* session, uint16 opcode, char const* result)
    {
        WorldPacket packet(opcode, 64);
        packet << result;
        session->SendPacket(&packet);
    }

    // Character state shared by activate and deactivate: owning account, the
    // persisted online flag, the effective active flag (missing row = active)
    // and how many characters of that account are currently active.
    std::string BuildCharacterStateQuery(uint32 charGuid)
    {
        return Acore::StringFormat(
            "SELECT `c`.`account`, `c`.`online`, COALESCE(`s`.`active` <> 0, 1), "
            "(SELECT COUNT(*) FROM `characters` AS `c2` "
            "LEFT JOIN `character_ascension_state` AS `s2` ON `s2`.`guid` = `c2`.`guid` "
            "WHERE `c2`.`account` = `c`.`account` AND (`s2`.`active` IS NULL OR `s2`.`active` <> 0)) "
            "FROM `characters` AS `c` "
            "LEFT JOIN `character_ascension_state` AS `s` ON `s`.`guid` = `c`.`guid` "
            "WHERE `c`.`guid` = {}", charGuid);
    }

    void HandleActivateRequest(WorldSession* session, WorldPacket const& packet)
    {
        if (packet.size() < sizeof(uint32))
        {
            SendResult(session, SMSG_ASCENSION_CHARACTER_ACTIVATE_RESULT, ACTIVATE_CHARACTER_FAILED);
            return;
        }

        uint32 const charGuid = packet.read<uint32>(0);
        uint32 const accountId = session->GetAccountId();

        session->GetQueryProcessor().AddCallback(
            CharacterDatabase.AsyncQuery(BuildCharacterStateQuery(charGuid)).WithCallback(
                [session, accountId, charGuid](QueryResult result)
                {
                    if (!result || result->GetRowCount() == 0)
                    {
                        SendResult(session, SMSG_ASCENSION_CHARACTER_ACTIVATE_RESULT, ACTIVATE_CHARACTER_NOT_FOUND);
                        return;
                    }

                    Field* fields = result->Fetch();

                    if (fields[0].Get<uint32>() != accountId)
                    {
                        SendResult(session, SMSG_ASCENSION_CHARACTER_ACTIVATE_RESULT, ACTIVATE_CHARACTER_NOT_OWNED);
                        return;
                    }

                    if (fields[1].Get<uint8>() != 0)
                    {
                        SendResult(session, SMSG_ASCENSION_CHARACTER_ACTIVATE_RESULT, ACTIVATE_CHARACTER_ONLINE);
                        return;
                    }

                    if (fields[2].Get<uint32>() != 0)
                    {
                        SendResult(session, SMSG_ASCENSION_CHARACTER_ACTIVATE_RESULT, ACTIVATE_CHARACTER_ALREADY_ACTIVE);
                        return;
                    }

                    uint64 const activeCount = fields[3].Get<uint64>();
                    if (activeCount >= CharacterSelectionMaxActive())
                    {
                        SendResult(session, SMSG_ASCENSION_CHARACTER_ACTIVATE_RESULT, ACTIVATE_CHARACTER_MAX_ACTIVE);
                        return;
                    }

                    CharacterDatabase.Execute(
                        "INSERT INTO `character_ascension_state` (`guid`, `active`) VALUES ({}, 1) "
                        "ON DUPLICATE KEY UPDATE `active` = 1", charGuid);

                    SendResult(session, SMSG_ASCENSION_CHARACTER_ACTIVATE_RESULT, ACTIVATE_CHARACTER_OK);

                    // The auth-side realm count follows active characters (the
                    // realm list shows it), so it has to learn about the change.
                    sWorld->UpdateRealmCharCount(accountId);

                    LOG_INFO("module.ascension_compat",
                        "Activated character {} for account {} ({}/{} active characters)",
                        charGuid, accountId, activeCount + 1, CharacterSelectionMaxActive());
                }));
    }

    void HandleDeactivateRequest(WorldSession* session, WorldPacket const& packet)
    {
        if (packet.size() < sizeof(uint32))
        {
            SendResult(session, SMSG_ASCENSION_CHARACTER_DEACTIVATE_RESULT, DEACTIVATE_CHARACTER_FAILED);
            return;
        }

        uint32 const charGuid = packet.read<uint32>(0);
        uint32 const accountId = session->GetAccountId();

        session->GetQueryProcessor().AddCallback(
            CharacterDatabase.AsyncQuery(BuildCharacterStateQuery(charGuid)).WithCallback(
                [session, accountId, charGuid](QueryResult result)
                {
                    if (!result || result->GetRowCount() == 0)
                    {
                        SendResult(session, SMSG_ASCENSION_CHARACTER_DEACTIVATE_RESULT, DEACTIVATE_CHARACTER_NOT_FOUND);
                        return;
                    }

                    Field* fields = result->Fetch();

                    if (fields[0].Get<uint32>() != accountId)
                    {
                        SendResult(session, SMSG_ASCENSION_CHARACTER_DEACTIVATE_RESULT, DEACTIVATE_CHARACTER_NOT_OWNED);
                        return;
                    }

                    if (fields[1].Get<uint8>() != 0)
                    {
                        SendResult(session, SMSG_ASCENSION_CHARACTER_DEACTIVATE_RESULT, DEACTIVATE_CHARACTER_ONLINE);
                        return;
                    }

                    if (fields[2].Get<uint32>() == 0)
                    {
                        SendResult(session, SMSG_ASCENSION_CHARACTER_DEACTIVATE_RESULT, DEACTIVATE_CHARACTER_ALREADY_INACTIVE);
                        return;
                    }

                    CharacterDatabase.Execute(
                        "INSERT INTO `character_ascension_state` (`guid`, `active`) VALUES ({}, 0) "
                        "ON DUPLICATE KEY UPDATE `active` = 0", charGuid);

                    SendResult(session, SMSG_ASCENSION_CHARACTER_DEACTIVATE_RESULT, DEACTIVATE_CHARACTER_OK);

                    // Parking a character frees a realm slot, and the realm list
                    // count has to follow.
                    sWorld->UpdateRealmCharCount(accountId);

                    LOG_INFO("module.ascension_compat",
                        "Deactivated character {} for account {}", charGuid, accountId);
                }));
    }

    // CMSG 0x0772 carries an opaque, space-separated sort table ("1 2 3 ...").
    // The client composes it from its own list positions, so the server only
    // stores and later echoes the payload through SMSG 0x076F.
    void HandleSortOrderRequest(WorldSession* session, WorldPacket const& packet)
    {
        // operator>> advances the read cursor, so read from a copy of the const packet.
        WorldPacket readable = packet;
        std::string payload;
        readable >> payload;

        // The client always sends a full table (the reset button re-sends
        // "1 2 3 ..."), so an empty payload is malformed and must not clobber
        // a stored order.
        if (payload.empty())
        {
            LOG_DEBUG("module.ascension_compat",
                "Ignored empty character-selection sort order from account {}",
                session->GetAccountId());
            return;
        }

        if (payload.size() > SORT_ORDER_PAYLOAD_MAXIMUM)
        {
            LOG_ERROR("module.ascension_compat",
                "Account {} sent an oversized character-selection sort order ({} bytes); ignored",
                session->GetAccountId(), payload.size());
            return;
        }

        std::string escaped = payload;
        CharacterDatabase.EscapeString(escaped);

        CharacterDatabase.Execute(
            "INSERT INTO `account_ascension_settings` (`account_id`, `sort_order`) VALUES ({}, '{}') "
            "ON DUPLICATE KEY UPDATE `sort_order` = VALUES(`sort_order`)",
            session->GetAccountId(), escaped);

        LOG_DEBUG("module.ascension_compat",
            "Stored character-selection sort order for account {} ({} bytes)",
            session->GetAccountId(), payload.size());
    }

    // Wire layout of one SMSG 0x075E entry (Extensions.dll parser RVA 190EE0):
    // u32 guid, u8 active, u8 online, u8 level, u8 race, u8 class, u8 gender,
    // u32 zoneId, cstring name. The header holds four counts, read into the
    // list object at +4, +8, +0xC and +0x10 in packet order and returned by
    // C_CharacterList.GetCounts() as max, total, active and inactive; the
    // inactive one is what HasInactiveCharacters() tests. The client fires
    // CHARACTER_LIST_UPDATED when the packet arrives.
    void HandleCharacterListQuery(WorldSession* session, uint32 maxActive, QueryResult result)
    {
        // Per-character data for SMSG 0x0770 / 0x0771, sent after the sort order
        // and before the list itself (the order the live server uses).
        struct CharacterSelectionExtra
        {
            uint32 index;
            uint8 race;
            bool highRisk;
            bool hasMail;
        };

        std::vector<CharacterSelectionExtra> extras;
        extras.reserve(CHARACTER_LIST_MAXIMUM);

        uint32 total = 0;
        uint32 activeCount = 0;
        uint32 listIndex = 0;
        std::string sortOrder;

        // Four counts, in the order CharacterSelect.lua receives them from
        // C_CharacterList.GetCounts(): max, total, active, inactive. The max is
        // clamped to 128 by Extensions.dll, and the client's create gate is
        // "active < max" (CharacterSelect_CanCreateCharacter), so this header is
        // what decides whether a character can be created at all: with the max at
        // the active limit, creating needs a free active slot. DEFAULT_MAX_
        // CHARACTERS_PER_REALM = 10 in the client Lua is only the fallback it uses
        // when the header reports a max of 0.
        WorldPacket info(SMSG_ASCENSION_CHARACTER_LIST_INFO, 256);
        info << maxActive << uint32(0) << uint32(0) << uint32(0);

        // Query layout: 0 guid, 1 name, 2 online, 3 level, 4 race, 5 class,
        // 6 gender, 7 zone, 8 active, 9 stored sort order, 10 high-risk aura,
        // 11 unread mail.
        if (result && result->GetRowCount() != 0)
        {
            do
            {
                Field* fields = result->Fetch();

                uint8 const active = fields[8].Get<uint32>() != 0 ? 1 : 0;
                ++total;
                activeCount += active;

                if (total == 1)
                    sortOrder = fields[9].Get<std::string>();

                uint32 const guid = fields[0].Get<uint32>();
                uint8 const race = fields[4].Get<uint8>();

                // Extras are keyed by list position, and CHAR_SEL_ENUM drops
                // inactive characters from that list, so they must not consume
                // one (their rows never ask for game-mode or mail data).
                if (active != 0)
                {
                    extras.push_back({ ++listIndex, race,
                        fields[10].Get<uint64>() != 0, fields[11].Get<uint64>() != 0 });
                }

                info << guid                                                       // guid
                     << active                                                     // active
                     << uint8(fields[2].Get<uint8>() != 0 ? 1 : 0)                 // online
                     << fields[3].Get<uint8>()                                    // level
                     << race                                                      // race
                     << fields[5].Get<uint8>()                                    // class
                     << fields[6].Get<uint8>()                                    // gender
                     << fields[7].Get<uint32>()                                   // zone
                     << fields[1].Get<std::string>();                             // name (cstring)
            } while (result->NextRow());
        }

        // Second count is the whole list, third the active count, fourth the
        // parked count. Lua reads the third one for the create gate and the
        // fourth one for HasInactiveCharacters(), so both have to stay accurate.
        info.put<uint32>(4, total);
        info.put<uint32>(8, activeCount);
        info.put<uint32>(12, total - activeCount);

        // CharacterSelect.lua only enables reordering when its sort order is
        // set (canSort = true after GetCharacterSelectionSortOrder() returns a
        // table), and it can only send one once reordering is possible. So the
        // packet is always answered: the stored order, or the identity table
        // the client itself produces when the order is reset ("1 2 3 ...",
        // padded to the display maximum it reports in the list header).
        if (sortOrder.empty())
        {
            for (uint32 i = 1; i <= maxActive; ++i)
            {
                if (i > 1)
                    sortOrder += ' ';
                sortOrder += std::to_string(i);
            }
        }

        WorldPacket order(SMSG_ASCENSION_CHARACTER_SORT_ORDER, sortOrder.size() + 8);
        order << sortOrder;
        session->SendPacket(&order);

        for (CharacterSelectionExtra const& extra : extras)
        {
            SendCharacterSelectionPerCharacter(session, extra.index, extra.race,
                extra.highRisk, extra.hasMail);
        }

        session->SendPacket(&info);

        // NOTE: no list-flags packet anymore. The live capture contains no
        // SMSG 0x0767, the client's opcode table assigns 0x0768 to something
        // else entirely, and C_CharacterList.HasInactiveCharacters() reads the
        // inactive count out of the list header above rather than from a flag.

        LOG_DEBUG("module.ascension_compat",
            "Sent Ascension character list for account {}: max={}, total={}, active={}, inactive={}, extras={}, sort order={} bytes",
            session->GetAccountId(), maxActive, total, activeCount, total - activeCount, extras.size(), sortOrder.size());
    }
}

bool IsAscensionCharacterSelectionOpcode(uint16 opcode)
{
    return opcode == CMSG_ASCENSION_CHARACTER_ACTIVATE ||
           opcode == CMSG_ASCENSION_CHARACTER_DEACTIVATE ||
           opcode == CMSG_ASCENSION_CHARACTER_SORT_ORDER;
}

bool HandleAscensionCharacterSelectionPacket(WorldSession* session, WorldPacket const& packet)
{
    if (!session || !CharacterSelectionEnabled())
        return false;

    switch (uint16(packet.GetOpcode()))
    {
        case CMSG_ASCENSION_CHARACTER_ACTIVATE:
            HandleActivateRequest(session, packet);
            return true;
        case CMSG_ASCENSION_CHARACTER_DEACTIVATE:
            HandleDeactivateRequest(session, packet);
            return true;
        case CMSG_ASCENSION_CHARACTER_SORT_ORDER:
            HandleSortOrderRequest(session, packet);
            return true;
        default:
            return false;
    }
}

void SendAscensionCharacterListInfo(WorldSession* session)
{
    if (!session || !CharacterSelectionEnabled())
        return;

    // The realm cluster precedes the list on live; Extensions.dll uses it to
    // initialize the realm object and to latch the feature gates the Character
    // Advancement UI depends on.
    SendAscensionRealmInfo(session);

    uint32 const accountId = session->GetAccountId();
    uint32 const maxActive = CharacterSelectionMaxActive();

    // Mirrors the core enum filter: inactive characters are excluded from
    // SMSG_CHAR_ENUM but still listed here with active = 0. Active characters
    // come first, each group in enum order (COALESCE(c.order, c.guid)): the
    // client takes the entries after the active count to be the inactive ones,
    // and it resolves a row by its position among the active characters.
    std::string const query = Acore::StringFormat(
        "SELECT `c`.`guid`, `c`.`name`, `c`.`online`, `c`.`level`, `c`.`race`, `c`.`class`, `c`.`gender`, `c`.`zone`, "
        "COALESCE(`s`.`active` <> 0, 1), "
        "COALESCE((SELECT `a`.`sort_order` FROM `account_ascension_settings` AS `a` "
        "WHERE `a`.`account_id` = {}), ''), "
        "EXISTS(SELECT 1 FROM `character_aura` AS `ha` WHERE `ha`.`guid` = `c`.`guid` AND `ha`.`spell` = {}), "
        "EXISTS(SELECT 1 FROM `mail` AS `m` WHERE `m`.`receiver` = `c`.`guid` "
        "AND (`m`.`checked` & 1) = 0 AND `m`.`deliver_time` <= UNIX_TIMESTAMP()) "
        "FROM `characters` AS `c` "
        "LEFT JOIN `character_ascension_state` AS `s` ON `s`.`guid` = `c`.`guid` "
        "WHERE `c`.`account` = {} AND `c`.`deleteInfos_Name` IS NULL "
        "ORDER BY COALESCE(`s`.`active` <> 0, 1) DESC, COALESCE(`c`.`order`, `c`.`guid`)",
        accountId, SPELL_ASCENSION_HIGH_RISK, accountId);

    session->GetQueryProcessor().AddCallback(
        CharacterDatabase.AsyncQuery(query).WithCallback(
            [session, maxActive](QueryResult result)
            {
                HandleCharacterListQuery(session, maxActive, result);
            }));
}
