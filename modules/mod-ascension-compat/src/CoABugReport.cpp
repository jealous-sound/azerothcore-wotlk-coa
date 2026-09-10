/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license:
 * https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "CoABugReportService.h"
#include "Chat.h"
#include "Config.h"
#include "GameTime.h"
#include "GitRevision.h"
#include "Log.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "WorldSession.h"
#include <memory>
#include <sstream>

namespace
{
    std::unique_ptr<CoABugReport::Service> ReportService;

    class CoABugReportWorld final : public WorldScript
    {
    public:
        CoABugReportWorld() : WorldScript("CoABugReportWorld", { WORLDHOOK_ON_AFTER_CONFIG_LOAD }) { }

        void OnAfterConfigLoad(bool reload) override
        {
            if (reload || !sConfigMgr->GetOption<bool>("CoABugReport.Enable", false))
                return;
            std::filesystem::path const path(sConfigMgr->GetOption<std::string>("CoABugReport.SpoolDirectory", ""));
            std::error_code error;
            if (!path.is_absolute() || !std::filesystem::is_directory(path, error) || error)
            {
                LOG_ERROR("server.loading",
                    "CoA bug reports disabled: configure an existing absolute spool directory.");
                return;
            }
            ReportService = std::make_unique<CoABugReport::Service>(path,
                sConfigMgr->GetOption<uint32>("CoABugReport.CooldownSeconds", 120));
            LOG_INFO("server.loading", "CoA bug-report queue enabled. GitHub delivery requires the separate relay.");
        }
    };

    class CoABugReportPlayer final : public PlayerScript
    {
    public:
        CoABugReportPlayer() : PlayerScript("CoABugReportPlayer", { PLAYERHOOK_CAN_PLAYER_USE_PRIVATE_CHAT }) { }

        bool OnPlayerCanUseChat(Player* player, uint32 type, uint32 language, std::string& message,
            Player* receiver) override
        {
            if (type != CHAT_MSG_WHISPER || language != LANG_ADDON || !message.starts_with(CoABugReport::Prefix))
                return true;
            // Consume this protocol even when disabled or misaddressed; never forward report text to another player.
            if (receiver != player || message.size() > 255)
                return false;
            std::string response = "H|0";
            if (ReportService)
            {
                std::ostringstream context;
                context << "\n\n### Server context\nClass ID: " << uint32(player->getClass())
                    << "\nLevel: " << uint32(player->GetLevel()) << "\nMap ID: " << player->GetMapId()
                    << "\nPosition: " << player->GetPositionX() << ", " << player->GetPositionY()
                    << ", " << player->GetPositionZ() << "\nCore revision: " << GitRevision::GetHash() << '\n';
                response = ReportService->Handle(player->GetSession()->GetAccountId(),
                    std::string_view(message).substr(CoABugReport::Prefix.size()), context.str(),
                    GameTime::GetGameTime().count());
            }
            if (!response.empty())
            {
                WorldPacket packet;
                ChatHandler::BuildChatPacket(packet, CHAT_MSG_WHISPER, LANG_ADDON, player->GetGUID(), player->GetGUID(),
                    std::string(CoABugReport::Prefix) + response, 0, player->GetName(), player->GetName(), 0, false);
                player->SendDirectMessage(&packet);
            }
            return false;
        }
    };
}

void AddCoABugReportScripts()
{
    new CoABugReportWorld();
    new CoABugReportPlayer();
}
