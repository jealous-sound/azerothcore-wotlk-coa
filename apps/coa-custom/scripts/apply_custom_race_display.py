# Core patch: races whose model cannot be dressed as a player (Naga) or that have no player model at all
# (Murloc) wear an NPC look in game. The look comes from acore_world.custom_race_display and is chosen by the
# skin colour picked at character creation (skin index modulo the number of looks of that race/gender).
import sys, pathlib

core = pathlib.Path(sys.argv[1] if len(sys.argv) > 1 else r'C:\CoA-Build\core')
header = core / 'src/server/game/Entities/Unit/AscensionIncarnation.h'
player = core / 'src/server/game/Entities/Player/Player.cpp'
coa = core / 'src/server/coa/AscensionCompat.cpp'


def patch(path, old, new, key):
    text = path.read_text(encoding='utf-8')
    if key in text:
        return
    assert text.count(old) == 1, (path.name, old[:60])
    path.write_text(text.replace(old, new), encoding='utf-8', newline='')


patch(header, '''#endif''', '''/// NPC look a player of a custom race wears in game (custom_race_display, chosen by skin colour), or 0.
uint32 GetAscensionCustomRaceDisplay(Player const* player);

#endif''', key='GetAscensionCustomRaceDisplay')

patch(player, '''        default:
            LOG_ERROR("entities.player", "Invalid gender {} for player", gender);
            return;
    }
}
''', '''        default:
            LOG_ERROR("entities.player", "Invalid gender {} for player", gender);
            return;
    }

    // Custom races without a dressable player model wear an NPC look picked by skin colour.
    if (uint32 customDisplay = GetAscensionCustomRaceDisplay(this))
    {
        SetDisplayId(customDisplay);
        SetNativeDisplayId(customDisplay);
    }
}
''', key='GetAscensionCustomRaceDisplay(this)')
patch(player, '#include "Player.h"\n', '#include "Player.h"\n#include "AscensionIncarnation.h"\n', key='#include "AscensionIncarnation.h"')

patch(coa, '''void AddAscensionCompatScripts() {
''', '''// Custom race looks ---------------------------------------------------------------------------
static std::unordered_map<uint32, std::vector<uint32>> CustomRaceDisplays; // (race << 8 | gender) -> looks

static void LoadCustomRaceDisplays()
{
    CustomRaceDisplays.clear();
    QueryResult result = WorldDatabase.Query("SELECT race, gender, displayId FROM custom_race_display ORDER BY race, gender, idx");
    if (!result)
        return;

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        CustomRaceDisplays[(fields[0].Get<uint32>() << 8) | fields[1].Get<uint32>()].push_back(fields[2].Get<uint32>());
        ++count;
    } while (result->NextRow());
    LOG_INFO("server.loading", ">> Loaded {} custom race looks", count);
}

uint32 GetAscensionCustomRaceDisplay(Player const* player)
{
    if (!player)
        return 0;

    auto const itr = CustomRaceDisplays.find((uint32(player->getRace(true)) << 8) | player->getGender());
    if (itr == CustomRaceDisplays.end() || itr->second.empty())
        return 0;

    return itr->second[player->GetByteValue(PLAYER_BYTES, 0) % itr->second.size()];
}

class AscensionCustomRaceDisplayWorldScript : public WorldScript
{
public:
    AscensionCustomRaceDisplayWorldScript() : WorldScript("AscensionCustomRaceDisplayWorldScript", { WORLDHOOK_ON_STARTUP }) { }

    void OnStartup() override { LoadCustomRaceDisplays(); }
};

void AddAscensionCompatScripts() {
''', key='LoadCustomRaceDisplays()')
patch(coa, '''  new AscensionIncarnationCreatureScript();
}
''', '''  new AscensionIncarnationCreatureScript();
  new AscensionCustomRaceDisplayWorldScript();
}
''', key='new AscensionCustomRaceDisplayWorldScript')
print('custom race display patch applied')
