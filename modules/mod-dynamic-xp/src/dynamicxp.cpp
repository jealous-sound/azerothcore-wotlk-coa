/*
Credits
Script reworked by Micrah/Milestorme and Poszer (Poszer is the Best)
Module Created by Micrah/Milestorme
Original Script from AshmaneCore https://github.com/conan513 Single Player Project

Local additions:
  * ".xp" lets a player pick their own rate (1, 3, 5, 7 or the per-band curve) or go
    back to the realm's rate. A game master sets the realm value with ".xp realm ...",
    which stays the default for every character without a personal choice.
  * an optional reminder about ".xp", told to a player when they log in and broadcast to
    the realm every Dynamic.XP.Reminder.Interval minutes.
*/

#include "Chat.h"
#include "CommandScript.h"
#include "Configuration/Config.h"
#include "Player.h"
#include "ScriptMgr.h"

#include <atomic>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>

using namespace Acore::ChatCommands;

namespace
{
    /// Keep the per-band curve the module shipped with. A flat preset (1, 3, 5, 7)
    /// replaces it; 1 therefore leaves experience exactly as the realm granted it.
    constexpr uint32 PRESET_BAND_CURVE = 0;
    constexpr char PRESET_CONFIG_KEY[] = "Dynamic.XP.Preset";
    constexpr char PRESET_CONFIG_FILE[] = "modules/dynamicxp.conf";
    constexpr char PLAYER_CHOICE_CONFIG_KEY[] = "Dynamic.XP.Preset.PlayerChoice";

    /// A character's own choice, stored as preset + 1 so that a stored row is never a
    /// zero and "no personal choice yet" stays distinguishable from "chose the curve".
    ///
    /// The "core." prefix is required, not cosmetic: Player::_SavePlayerSettings and
    /// _LoadCharacterSettings drop every source that does not start with it while
    /// EnablePlayerSettings is 0, which is how this realm runs. A name without the
    /// prefix is held in memory, then silently lost on relog.
    constexpr char PLAYER_PRESET_SETTING[] = "core.dynamic_xp.preset";

    constexpr char REMINDER_ENABLE_KEY[] = "Dynamic.XP.Reminder.Enable";
    constexpr char REMINDER_INTERVAL_KEY[] = "Dynamic.XP.Reminder.Interval";
    constexpr char REMINDER_MESSAGE_KEY[] = "Dynamic.XP.Reminder.Message";

    constexpr char DEFAULT_REMINDER[] =
        "|cff4CFF00XP rate|r: pick your own with |cff4CFF00.xp 1|r, |cff4CFF00.xp 3|r, "
        "|cff4CFF00.xp 5|r or |cff4CFF00.xp 7|r, or |cff4CFF00.xp default|r to follow "
        "the realm. |cff4CFF00.xp|r shows what you are on.";

    /// -1 until the value has been read from the config. XP is granted on several map
    /// threads at once, so the realm value is cached in an atomic rather than re-read.
    std::atomic<int32> g_realmPresetCache{-1};

    /// Set when a player logs in; the broadcast itself happens on the world thread.

    /// Milliseconds since the last periodic reminder. World thread only.
    uint32 g_sinceReminder = 0;

    bool IsKnownPreset(uint32 preset)
    {
        return preset == PRESET_BAND_CURVE || preset == 1 || preset == 3 ||
               preset == 5 || preset == 7;
    }

    uint32 PresetFromConfig()
    {
        uint32 const preset = sConfigMgr->GetOption<uint32>(PRESET_CONFIG_KEY, 1);
        return IsKnownPreset(preset) ? preset : 1;
    }

    uint32 RealmPreset()
    {
        int32 const cached = g_realmPresetCache.load(std::memory_order_relaxed);
        if (cached >= 0)
            return uint32(cached);

        uint32 const preset = PresetFromConfig();
        g_realmPresetCache.store(int32(preset), std::memory_order_relaxed);
        return preset;
    }

    void RefreshRealmPreset()
    {
        g_realmPresetCache.store(int32(PresetFromConfig()), std::memory_order_relaxed);
    }

    bool PlayersMayChoose()
    {
        return sConfigMgr->GetOption<bool>(PLAYER_CHOICE_CONFIG_KEY, true);
    }

    /// A character's own choice. False when they have never made one.
    bool PersonalPreset(Player const *player, uint32 &preset)
    {
        if (!player)
            return false;

        PlayerSettingVector const *values = player->FindPlayerSettings(PLAYER_PRESET_SETTING);
        if (!values || values->empty())
            return false;

        uint32 const stored = (*values)[0].value;
        if (!stored)
            return false;

        preset = stored - 1;
        return IsKnownPreset(preset);
    }

    void StorePersonalPreset(Player *player, uint32 preset)
    {
        if (player)
            player->UpdatePlayerSetting(PLAYER_PRESET_SETTING, 0, preset + 1);
    }

    void ClearPersonalPreset(Player *player)
    {
        if (player)
            player->UpdatePlayerSetting(PLAYER_PRESET_SETTING, 0, 0);
    }

    /// The rate that applies to this character: their own choice, else the realm's.
    uint32 EffectivePreset(Player const *player)
    {
        uint32 preset = 0;
        if (PlayersMayChoose() && PersonalPreset(player, preset))
            return preset;
        return RealmPreset();
    }

    std::string DescribePreset(uint32 preset)
    {
        if (preset == PRESET_BAND_CURVE)
            return "the per-band curve";
        return "x" + std::to_string(preset);
    }

    /// Applies one preset to a granted amount. The band curve needs the player's level.
    void ApplyPreset(Player *player, uint32 &amount, uint32 preset)
    {
        if (preset > 1)
        {
            amount = static_cast<uint32>(std::round(amount * preset));
            return;
        }

        // x1 leaves the granted amount alone.
        if (preset != PRESET_BAND_CURVE ||
            !sConfigMgr->GetOption<bool>("Dynamic.XP.Rate", true))
            return;

        if (player->GetLevel() <= 9)
            amount =  static_cast<uint32>(round(amount * sConfigMgr->GetOption<float>("Dynamic.XP.Rate.1-9", 1)));
        else if (player->GetLevel() <= 19)
            amount =  static_cast<uint32>(round(amount * sConfigMgr->GetOption<float>("Dynamic.XP.Rate.10-19", 2)));
        else if (player->GetLevel() <= 29)
            amount =  static_cast<uint32>(round(amount * sConfigMgr->GetOption<float>("Dynamic.XP.Rate.20-29", 3)));
        else if (player->GetLevel() <= 39)
            amount =  static_cast<uint32>(round(amount * sConfigMgr->GetOption<float>("Dynamic.XP.Rate.30-39", 4)));
        else if (player->GetLevel() <= 49)
            amount =  static_cast<uint32>(round(amount * sConfigMgr->GetOption<float>("Dynamic.XP.Rate.40-49", 5)));
        else if (player->GetLevel() <= 59)
            amount =  static_cast<uint32>(round(amount * sConfigMgr->GetOption<float>("Dynamic.XP.Rate.50-59", 6)));
        else if (player->GetLevel() <= 69)
            amount =  static_cast<uint32>(round(amount * sConfigMgr->GetOption<float>("Dynamic.XP.Rate.60-69", 7)));
        else if (player->GetLevel() <= 79)
            amount =  static_cast<uint32>(round(amount * sConfigMgr->GetOption<float>("Dynamic.XP.Rate.70-79", 8)));
    }

    /// Writes the realm value into this module's config so a restart keeps it. Only the
    /// "Dynamic.XP.Preset" line is rewritten: the comments and the band rates stay as
    /// the operator wrote them. The line is appended when the file does not have it.
    /// Returns whether the file now holds the value; \p path always names the file.
    bool PersistRealmPreset(uint32 preset, std::string &path)
    {
        path = sConfigMgr->GetConfigPath() + PRESET_CONFIG_FILE;

        std::vector<std::string> lines;
        bool replaced = false;
        std::ifstream input(path);
        if (input)
        {
            std::string line;
            while (std::getline(input, line))
            {
                size_t const first = line.find_first_not_of(" \t");
                if (!replaced && first != std::string::npos &&
                    line.compare(first, sizeof(PRESET_CONFIG_KEY) - 1, PRESET_CONFIG_KEY) == 0)
                {
                    lines.push_back(std::string(PRESET_CONFIG_KEY) + " = " +
                                    std::to_string(preset));
                    replaced = true;
                    continue;
                }
                lines.push_back(line);
            }
            input.close();
        }

        std::ofstream output(path, replaced ? std::ios::trunc : std::ios::app);
        if (!output)
            return false;

        if (replaced)
        {
            for (std::string const &entry : lines)
                output << entry << '\n';
        }
        else
        {
            output << '\n' << PRESET_CONFIG_KEY << " = " << preset << '\n';
        }

        return output.good();
    }

    bool ReminderEnabled()
    {
        return sConfigMgr->GetOption<bool>(REMINDER_ENABLE_KEY, true);
    }

    std::string ReminderText()
    {
        std::string text = sConfigMgr->GetOption<std::string>(REMINDER_MESSAGE_KEY, "");
        return text.empty() ? std::string(DEFAULT_REMINDER) : text;
    }

    /// One announcement, to every player who is online and has not turned automatic
    /// announcements off (the same switch the realm's autobroadcast honours).
    void BroadcastReminder()
    {
        if (!ReminderEnabled())
            return;

        std::string const text = ReminderText();
        ChatHandler(nullptr).DoForAllValidSessions([&text](Player *player)
        {
            ChatHandler(player->GetSession()).SendWorldTextOptional(
                text, ANNOUNCER_FLAG_DISABLE_AUTOBROADCAST);
        });

        LOG_INFO("module.dynamic_xp", "Announced the .xp reminder to the realm.");
    }
}

class spp_dynamic_xp_rate : public PlayerScript
{
public:
    spp_dynamic_xp_rate() : PlayerScript("spp_dynamic_xp_rate", {
        PLAYERHOOK_ON_LOGIN,
        PLAYERHOOK_ON_GIVE_EXP
    }) { };

    void OnPlayerLogin(Player* player) override
    {
        // Told to the player who just arrived, not broadcast to the realm: a realm with bots logs
        // characters in and out constantly, and each login announced to everybody filled the chat.
        if (ReminderEnabled() && !player->GetSession()->IsBot())
            ChatHandler(player->GetSession()).SendSysMessage(ReminderText().c_str());

        if (!sConfigMgr->GetOption<bool>("Dynamic.XP.Rate.Announce", false))
            return;

        ChatHandler(player->GetSession()).PSendSysMessage(
            "This server is running a |cff4CFF00{}|r XP rate. Type |cff4CFF00.xp|r to "
            "change your own.", DescribePreset(EffectivePreset(player)));
    }

    void OnPlayerGiveXP(Player* player, uint32& amount, Unit* /*victim*/, uint8 /*xpSource*/) override
    {
        ApplyPreset(player, amount, EffectivePreset(player));
    }
};

/// Configs are loaded before this runs, at startup and on ".reload config", so an edit
/// of dynamicxp.conf by hand takes effect without a restart.
class spp_dynamic_xp_config : public WorldScript
{
public:
    spp_dynamic_xp_config() : WorldScript("spp_dynamic_xp_config") { }

    void OnBeforeConfigLoad(bool /*reload*/) override
    {
        RefreshRealmPreset();
    }

    void OnUpdate(uint32 diff) override
    {
        uint32 const minutes = sConfigMgr->GetOption<uint32>(REMINDER_INTERVAL_KEY, 40);
        if (!minutes)
            return;

        g_sinceReminder += diff;
        if (g_sinceReminder < minutes * 60 * 1000)
            return;

        g_sinceReminder = 0;
        BroadcastReminder();
    }
};

class dynamic_xp_commandscript : public CommandScript
{
public:
    dynamic_xp_commandscript() : CommandScript("dynamic_xp_commandscript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable const realmTable =
        {
            { "1",       HandleRealmOne,     SEC_GAMEMASTER, Console::Yes },
            { "3",       HandleRealmThree,   SEC_GAMEMASTER, Console::Yes },
            { "5",       HandleRealmFive,    SEC_GAMEMASTER, Console::Yes },
            { "7",       HandleRealmSeven,   SEC_GAMEMASTER, Console::Yes },
            { "dynamic", HandleRealmCurve,   SEC_GAMEMASTER, Console::Yes },
            { "",        HandleShowRealm,    SEC_GAMEMASTER, Console::Yes },
        };

        static ChatCommandTable const xpCommandTable =
        {
            { "1",       HandleMineOne,      SEC_PLAYER, Console::No },
            { "3",       HandleMineThree,    SEC_PLAYER, Console::No },
            { "5",       HandleMineFive,     SEC_PLAYER, Console::No },
            { "7",       HandleMineSeven,    SEC_PLAYER, Console::No },
            { "dynamic", HandleMineCurve,    SEC_PLAYER, Console::No },
            { "default", HandleMineDefault,  SEC_PLAYER, Console::No },
            { "realm",   realmTable },
            { "",        HandleShowMine,     SEC_PLAYER, Console::No },
        };

        static ChatCommandTable const commandTable =
        {
            { "xp", xpCommandTable },
        };
        return commandTable;
    }

private:
    // --- a player's own rate -------------------------------------------------

    static bool SetMine(ChatHandler* handler, uint32 preset)
    {
        Player* player = handler ? handler->GetPlayer() : nullptr;
        if (!player)
            return false;

        if (!PlayersMayChoose())
        {
            handler->SendSysMessage(
                "Choosing your own XP rate is turned off on this realm; ask a game master.");
            return true;
        }

        StorePersonalPreset(player, preset);
        handler->PSendSysMessage("Your XP rate is now |cff4CFF00{}|r, saved for this character.",
                                 DescribePreset(preset));
        return true;
    }

    static bool HandleMineOne(ChatHandler* handler)     { return SetMine(handler, 1); }
    static bool HandleMineThree(ChatHandler* handler)   { return SetMine(handler, 3); }
    static bool HandleMineFive(ChatHandler* handler)    { return SetMine(handler, 5); }
    static bool HandleMineSeven(ChatHandler* handler)   { return SetMine(handler, 7); }
    static bool HandleMineCurve(ChatHandler* handler)   { return SetMine(handler, PRESET_BAND_CURVE); }

    static bool HandleMineDefault(ChatHandler* handler)
    {
        Player* player = handler ? handler->GetPlayer() : nullptr;
        if (!player)
            return false;

        ClearPersonalPreset(player);
        handler->PSendSysMessage("Your XP rate now follows the realm: |cff4CFF00{}|r.",
                                 DescribePreset(RealmPreset()));
        return true;
    }

    static bool HandleShowMine(ChatHandler* handler)
    {
        Player* player = handler ? handler->GetPlayer() : nullptr;
        if (!player)
            return false;

        uint32 personal = 0;
        bool const owns = PlayersMayChoose() && PersonalPreset(player, personal);

        handler->PSendSysMessage("Your XP rate: |cff4CFF00{}|r ({}).",
                                 DescribePreset(EffectivePreset(player)),
                                 owns ? "your own choice" : "the realm rate");
        if (owns)
            handler->PSendSysMessage("The realm rate is {}. |cff4CFF00.xp default|r "
                                     "follows it again.", DescribePreset(RealmPreset()));
        if (PlayersMayChoose())
            handler->SendSysMessage(
                "Pick yours with |cff4CFF00.xp 1|r, .xp 3, .xp 5, .xp 7 or "
                "|cff4CFF00.xp dynamic|r (the per-band curve).");
        return true;
    }

    // --- the realm's rate, for game masters ----------------------------------

    static bool SetRealm(ChatHandler* handler, uint32 preset)
    {
        if (!handler)
            return false;

        std::string path;
        bool const persisted = PersistRealmPreset(preset, path);
        // Apply it for this session even when the file could not be written, so the
        // switch always does what the operator asked for.
        g_realmPresetCache.store(int32(preset), std::memory_order_relaxed);

        if (preset == 1)
            handler->PSendSysMessage(
                "The realm rate is back to |cff4CFF00x1|r: no bonus is applied.");
        else
            handler->PSendSysMessage("The realm rate is now |cff4CFF00{}|r.",
                                     DescribePreset(preset));

        if (persisted)
            handler->PSendSysMessage("Saved to {}. Characters with their own choice keep it.",
                                     path);
        else
            handler->PSendSysMessage("Could not write {}; the change lasts until the next "
                                     "restart.", path);
        return true;
    }

    static bool HandleRealmOne(ChatHandler* handler)    { return SetRealm(handler, 1); }
    static bool HandleRealmThree(ChatHandler* handler)  { return SetRealm(handler, 3); }
    static bool HandleRealmFive(ChatHandler* handler)   { return SetRealm(handler, 5); }
    static bool HandleRealmSeven(ChatHandler* handler)  { return SetRealm(handler, 7); }
    static bool HandleRealmCurve(ChatHandler* handler)  { return SetRealm(handler, PRESET_BAND_CURVE); }

    static bool HandleShowRealm(ChatHandler* handler)
    {
        if (!handler)
            return false;

        handler->PSendSysMessage("Realm XP rate: |cff4CFF00{}|r (the default for every "
                                 "character without its own choice).",
                                 DescribePreset(RealmPreset()));
        handler->SendSysMessage(
            "Change it with |cff4CFF00.xp realm 1|r, .xp realm 3, .xp realm 5, .xp realm 7, "
            ".xp realm dynamic|r.");
        return true;
    }
};

void AddSC_dynamic_xp_rate()
{
    new spp_dynamic_xp_rate();
    new spp_dynamic_xp_config();
    new dynamic_xp_commandscript();
}
