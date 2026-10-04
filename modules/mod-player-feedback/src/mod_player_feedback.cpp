/*
 * Accepts feature requests and bug reports from the HXC Player Feedback addon.
 * Reports are written to the normal worldserver log with a stable search tag.
 */

#include "Chat.h"
#include "Log.h"
#include "Player.h"
#include "PlayerScript.h"
#include "ScriptMgr.h"

#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_map>

namespace
{
char const* const kPrefix = "HXR\t";
size_t constexpr kPrefixLength = 4;
size_t constexpr kMaxReportBytes = 180;
uint32 constexpr kCooldownMs = 60000;

std::unordered_map<uint32, uint32> sLastReportMs;

std::string JsonEscape(std::string const& value)
{
    std::string escaped;
    escaped.reserve(value.size() + 8);
    for (unsigned char ch : value)
    {
        switch (ch)
        {
            case '\\': escaped += "\\\\"; break;
            case '"': escaped += "\\\""; break;
            case '\n': escaped += "\\n"; break;
            case '\r': escaped += "\\r"; break;
            case '\t': escaped += "\\t"; break;
            default:
                if (ch >= 0x20)
                    escaped += static_cast<char>(ch);
                break;
        }
    }
    return escaped;
}

void Reply(Player* player, char const* result)
{
    if (!player)
        return;
    WorldPacket data;
    ChatHandler::BuildChatPacket(data, CHAT_MSG_WHISPER, LANG_ADDON, player->GetGUID(),
                                 player->GetGUID(), std::string("HXR\t") + result,
                                 CHAT_TAG_NONE, player->GetName());
    player->GetSession()->SendPacket(&data);
}

class PlayerFeedbackScript final : public PlayerScript
{
public:
    PlayerFeedbackScript() : PlayerScript("PlayerFeedbackScript", {
        PLAYERHOOK_ON_BEFORE_SEND_CHAT_MESSAGE,
        PLAYERHOOK_CAN_PLAYER_USE_PRIVATE_CHAT,
        PLAYERHOOK_CAN_PLAYER_USE_GROUP_CHAT
    }) { }

    static bool IsFeedbackMessage(uint32 type, uint32 language, std::string const& message)
    {
        if (language != LANG_ADDON)
            return false;
        if (type != CHAT_MSG_WHISPER && type != CHAT_MSG_PARTY &&
            type != CHAT_MSG_PARTY_LEADER && type != CHAT_MSG_RAID &&
            type != CHAT_MSG_RAID_LEADER)
            return false;
        return message.compare(0, kPrefixLength, kPrefix) == 0;
    }

    bool OnPlayerCanUseChat(Player* /*player*/, uint32 type, uint32 language,
                            std::string& message, Player* /*receiver*/) override
    {
        return !IsFeedbackMessage(type, language, message);
    }

    bool OnPlayerCanUseChat(Player* /*player*/, uint32 type, uint32 language,
                            std::string& message, Group* /*group*/) override
    {
        return !IsFeedbackMessage(type, language, message);
    }

    void OnPlayerBeforeSendChatMessage(Player* player, uint32& type, uint32& language,
                                       std::string& message) override
    {
        if (!IsFeedbackMessage(type, language, message) || !player)
            return;

        std::string payload = message.substr(kPrefixLength);
        size_t const firstTab = payload.find('\t');
        size_t const secondTab = firstTab == std::string::npos
            ? std::string::npos : payload.find('\t', firstTab + 1);
        if (firstTab == std::string::npos || secondTab == std::string::npos ||
            payload.substr(0, firstTab) != "SUBMIT")
        {
            Reply(player, "INVALID");
            return;
        }

        std::string const kind = payload.substr(firstTab + 1, secondTab - firstTab - 1);
        std::string text = payload.substr(secondTab + 1);
        while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back())))
            text.pop_back();
        auto begin = std::find_if_not(text.begin(), text.end(), [](unsigned char ch) {
            return std::isspace(ch);
        });
        text.erase(text.begin(), begin);

        if ((kind != "FEATURE" && kind != "BUG") || text.empty() ||
            text.size() > kMaxReportBytes || text.find_first_of("\r\n\0", 0, 3) != std::string::npos)
        {
            Reply(player, "INVALID");
            return;
        }

        uint32 const now = getMSTime();
        uint32 const accountId = player->GetSession()->GetAccountId();
        auto last = sLastReportMs.find(accountId);
        if (last != sLastReportMs.end() && getMSTimeDiff(last->second, now) < kCooldownMs)
        {
            Reply(player, "WAIT");
            return;
        }
        sLastReportMs[accountId] = now;

        LOG_INFO("server", "[PLAYER_FEEDBACK] type={} player=\"{}\" level={} map={} zone={} area={} pos=({:.1f},{:.1f},{:.1f}) text=\"{}\"",
                 kind, JsonEscape(player->GetName()), player->GetLevel(), player->GetMapId(),
                 player->GetZoneId(), player->GetAreaId(), player->GetPositionX(),
                 player->GetPositionY(), player->GetPositionZ(), JsonEscape(text));
        Reply(player, "OK");
    }
};
}

void AddSC_player_feedback()
{
    new PlayerFeedbackScript();
}
