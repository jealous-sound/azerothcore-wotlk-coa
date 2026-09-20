/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "DBCStores.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"

namespace
{
enum
{
    SPELL_RESET_TITHE = 800981, // periodic 300767: cuts Tithe's hour-long cooldown by 3501 sec
    TITHE_COST = 1 * SILVER
};

// Tithe 804781 only carries an empty aura: the tooltip promises that paying 1 silver at a holy site teaches a
// party aura, and the site decides which one. The pairs below are the community mapping of the eleven Eastern
// Kingdoms and Kalimdor sites the tooltip lists; faction pairs teach the same aura. The Outland sites teach Order,
// Alacrity and Ingenuity, but nobody has mapped them and the level cap keeps Outland closed.
struct TitheSite
{
    uint32 area;
    TeamId team;
    uint32 reward;
    bool exact = false; // the area itself, not its subzones
};
constexpr TitheSite sites[] = {
    {10138, TEAM_ALLIANCE, 804816}, // Northshire Valley (Northshire Abbey): Heroism
    {10140, TEAM_HORDE, 804816},    // Deathknell: Heroism
    {10225, TEAM_ALLIANCE, 804820}, // Cathedral Square (Cathedral of Light): Courage
    {10231, TEAM_HORDE, 804820},    // Valley of Spirits: Courage
    {1637, TEAM_HORDE, 804820, true}, // the valley floor, which only reports Orgrimmar
    {1661, TEAM_ALLIANCE, 804819},  // The Temple Gardens: Mysticism
    {1657, TEAM_ALLIANCE, 804819, true}, // inside the Temple of the Moon, which only reports Darnassus
    {2197, TEAM_HORDE, 804819},     // The Pools of Vision: Mysticism
    {2268, TEAM_NEUTRAL, 804782},   // Light's Hope Chapel: Crusading
    {796, TEAM_NEUTRAL, 804782},    // Scarlet Monastery: Crusading
    {228, TEAM_NEUTRAL, 804815},    // The Sepulcher: Liberty
    {2405, TEAM_NEUTRAL, 804817},   // Ethel Rethor: Elements
    {280, TEAM_NEUTRAL, 804784},    // Strahnbrad: Might
};

bool Inside(uint32 area, TitheSite const& site)
{
    if (site.exact)
        return area == site.area;
    for (uint8 depth = 0; area && depth < 4; ++depth)
    {
        if (area == site.area)
            return true;
        AreaTableEntry const* entry = sAreaTableStore.LookupEntry(area);
        area = entry ? entry->zone : 0;
    }
    return false;
}

class spell_ascension_templar_tithe : public SpellScript
{
    PrepareSpellScript(spell_ascension_templar_tithe);
    void Pay()
    {
        Player* player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
        if (!player)
            return;
        uint32 const area = player->GetAreaId();
        for (TitheSite const& site : sites)
            if ((site.team == TEAM_NEUTRAL || site.team == player->GetTeamId()) && Inside(area, site) &&
                !player->HasSpell(site.reward) && player->HasEnoughMoney(uint32(TITHE_COST)))
            {
                player->ModifyMoney(-int32(TITHE_COST));
                player->learnSpell(site.reward);
                return;
            }
        // Nothing to learn here: Ascension's own failure spell hands most of the cooldown back.
        player->CastSpell(player, SPELL_RESET_TITHE, true);
    }
    void Register() override
    {
        AfterCast += SpellCastFn(spell_ascension_templar_tithe::Pay);
    }
};
} // namespace
void AddSC_AscensionTemplarTithe()
{
    RegisterSpellScript(spell_ascension_templar_tithe);
}
