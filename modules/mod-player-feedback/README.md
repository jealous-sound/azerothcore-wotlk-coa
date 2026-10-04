# mod-player-feedback

Server receiver for the HXC Player Feedback addon. It accepts addon messages over a whisper to the sender and writes reports to the regular worldserver log.

Reports appear with the searchable tag `[PLAYER_FEEDBACK]` and include the report type, character, level, map, zone, area, position, and submitted text. Each account can submit one report per minute. The message limit is 180 bytes, matching the Wrath addon message limit. Input is escaped before logging so report text cannot forge extra log lines.

The client addon is in `C:/Azerothcore/friend-client-setup/Interface/AddOns/PlayerFeedback`. Copy it to clients or publish it through the existing client catalog. This module must be included in the SurvivalCraft+ (CoA) worldserver build and deployed before players can submit reports.
