/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "Chat.h"
#include "DBCStores.h"
#include "GameTime.h"
#include "Item.h"
#include "Map.h"
#include "MapMgr.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "StringFormat.h"
#include <mutex>
#include <unordered_map>
#include <vector>

namespace
{
struct ZoneScrollEntry
{
    uint32 ItemEntry;
    uint32 SpellId;
    char const* Name;
};

constexpr ZoneScrollEntry kZoneScrolls[] = {
    { 696661, 993961, "Golganneth" },
    { 696662, 993943, "Norgannon" },
    { 696663, 993955, "Khaz'goroth" },
    { 696664, 993959, "Aggramar" },
    { 696665, 993957, "Eonar" },
    { 1179240, 91770, "Steadfast" },
    { 1179261, 91796, "Featherfall" },
    { 1179266, 91803, "Ghost Runner" },
    { 1179269, 91814, "Crafting Speed" },
};

uint32 SpellForItem(uint32 itemEntry)
{
    for (ZoneScrollEntry const& entry : kZoneScrolls)
        if (entry.ItemEntry == itemEntry)
            return entry.SpellId;
    return 0;
}

constexpr char const* ANNOUNCE_TAG_COLOR = "ffa54a";

std::string ItemIcon(ItemTemplate const* proto)
{
    ItemDisplayInfoEntry const* display = sItemDisplayInfoStore.LookupEntry(proto->DisplayInfoID);
    if (!display || !display->inventoryIcon || !*display->inventoryIcon)
        return "";
    return Acore::StringFormat("|TInterface\\Icons\\{}:20:20|t ", display->inventoryIcon);
}

std::string ZoneBlessingAnnouncement(Player const* player, ItemTemplate const* proto, SpellInfo const* spellInfo)
{
    std::string icon = ItemIcon(proto);
    return Acore::StringFormat(
        "{}|cff{}[Keeper's Scroll]|r {}{} used their |c{:08x}|Hitem:{}:0:0:0:0:0:0:0:0:0|h[{}]|h|r "
        "to buff the zone with |cff71d5ff|Hspell:{}|h[{}]|h|r!",
        icon, ANNOUNCE_TAG_COLOR, icon, player->GetName(), ItemQualityColors[proto->Quality], proto->ItemId,
        proto->Name1, spellInfo->Id, spellInfo->SpellName[LOCALE_enUS]);
}

std::mutex g_zoneScrollLock;
std::unordered_map<uint32, std::unordered_map<uint32, time_t>> g_zoneScrollBuffs;

void ApplyZoneScrollAura(Player* player, uint32 spellId, int32 remainingMs)
{
    if (remainingMs <= 0)
        return;

    if (!player->HasAura(spellId))
        player->CastSpell(player, spellId, true);

    if (Aura* aura = player->GetAura(spellId))
    {
        aura->SetMaxDuration(remainingMs);
        aura->SetDuration(remainingMs);
    }
}

constexpr uint32 SPELL_KEEPERS_SCROLL_GHOST_RUNNER = 91803;
constexpr uint32 SPELL_GHOST_RUNNER_SPEED = 92417;

void SyncGhostRunner(Player* player)
{
    bool const wanted = player->HasPlayerFlag(PLAYER_FLAGS_GHOST) && player->HasAura(SPELL_KEEPERS_SCROLL_GHOST_RUNNER);
    if (wanted && !player->HasAura(SPELL_GHOST_RUNNER_SPEED))
        player->CastSpell(player, SPELL_GHOST_RUNNER_SPEED, true);
    else if (!wanted && player->HasAura(SPELL_GHOST_RUNNER_SPEED))
        player->RemoveAurasDueToSpell(SPELL_GHOST_RUNNER_SPEED);
}

bool IsActiveInZone(uint32 zoneId, uint32 spellId)
{
    std::lock_guard<std::mutex> lock(g_zoneScrollLock);
    auto zone = g_zoneScrollBuffs.find(zoneId);
    if (zone == g_zoneScrollBuffs.end())
        return false;
    auto spell = zone->second.find(spellId);
    return spell != zone->second.end() && spell->second > GameTime::GetGameTime().count();
}

bool TryBlessZone(uint32 zoneId, uint32 spellId, int32 durationMs)
{
    time_t now = GameTime::GetGameTime().count();
    std::lock_guard<std::mutex> lock(g_zoneScrollLock);
    time_t& slot = g_zoneScrollBuffs[zoneId][spellId];
    if (slot > now)
        return false;
    slot = now + durationMs / 1000;
    return true;
}

class ascension_keepers_scroll_zone_buff_spell : public AllSpellScript
{
public:
    ascension_keepers_scroll_zone_buff_spell()
        : AllSpellScript("ascension_keepers_scroll_zone_buff_spell", {ALLSPELLHOOK_ON_BEFORE_EFFECTS}) { }

    void OnSpellBeforeEffects(Spell* spell, Unit* caster, SpellInfo const* spellInfo) override
    {
        Player* player = caster ? caster->ToPlayer() : nullptr;
        Item* item = spell->m_CastItem;
        if (!player || !item || SpellForItem(item->GetEntry()) != spellInfo->Id)
            return;

        int32 durationMs = spellInfo->GetMaxDuration();
        uint32 zoneId = player->GetZoneId();
        if (durationMs <= 0 || !TryBlessZone(zoneId, spellInfo->Id, durationMs))
            return;

        Map* map = player->GetMap();
        if (!map)
            return;

        for (MapReference const& ref : map->GetPlayers())
        {
            Player* target = ref.GetSource();
            if (target != player && target->IsInWorld() && target->GetZoneId() == zoneId)
            {
                ApplyZoneScrollAura(target, spellInfo->Id, durationMs);
                SyncGhostRunner(target);
            }
        }

        map->SendZoneText(zoneId, ZoneBlessingAnnouncement(player, item->GetTemplate(), spellInfo).c_str());
    }
};

class ascension_keepers_scroll_zone_buff_player : public PlayerScript
{
public:
    ascension_keepers_scroll_zone_buff_player()
        : PlayerScript("ascension_keepers_scroll_zone_buff_player",
            {PLAYERHOOK_ON_UPDATE_ZONE, PLAYERHOOK_CAN_CAST_ITEM_USE_SPELL, PLAYERHOOK_ON_PLAYER_RELEASED_GHOST,
             PLAYERHOOK_ON_PLAYER_RESURRECT}) { }

    void OnPlayerReleasedGhost(Player* player) override
    {
        SyncGhostRunner(player);
    }

    void OnPlayerResurrect(Player* player, float, bool&) override
    {
        SyncGhostRunner(player);
    }

    bool OnPlayerCanCastItemUseSpell(Player* player, Item* item, SpellCastTargets const&,
        uint8 castCount, uint32) override
    {
        uint32 spellId = SpellForItem(item->GetEntry());
        if (!spellId)
            return true;

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
        if (!spellInfo || !IsActiveInZone(player->GetZoneId(), spellId))
            return true;

        Spell::SendCastResult(player, spellInfo, castCount, SPELL_FAILED_AURA_BOUNCED);
        ChatHandler(player->GetSession()).PSendSysMessage(
            "{} is already active in this zone.", spellInfo->SpellName[LOCALE_enUS]);
        return false;
    }

    void OnPlayerUpdateZone(Player* player, uint32 newZone, uint32) override
    {
        time_t now = GameTime::GetGameTime().count();
        std::unordered_map<uint32, time_t> active;
        {
            std::lock_guard<std::mutex> lock(g_zoneScrollLock);
            auto it = g_zoneScrollBuffs.find(newZone);
            if (it != g_zoneScrollBuffs.end())
                active = it->second;
        }

        for (ZoneScrollEntry const& entry : kZoneScrolls)
        {
            auto it = active.find(entry.SpellId);
            if (it != active.end() && it->second > now)
                ApplyZoneScrollAura(player, entry.SpellId, int32((it->second - now) * 1000));
            else if (player->HasAura(entry.SpellId))
                player->RemoveAurasDueToSpell(entry.SpellId);
        }

        SyncGhostRunner(player);
    }
};

class ascension_keepers_scroll_zone_buff_world : public WorldScript
{
public:
    ascension_keepers_scroll_zone_buff_world()
        : WorldScript("ascension_keepers_scroll_zone_buff_world", {WORLDHOOK_ON_UPDATE}) { }

    void OnUpdate(uint32 diff) override
    {
        _timer += diff;
        if (_timer < EXPIRE_CHECK_INTERVAL_MS)
            return;
        _timer = 0;

        time_t now = GameTime::GetGameTime().count();
        std::vector<std::pair<uint32, uint32>> expiredZoneSpells;

        {
            std::lock_guard<std::mutex> lock(g_zoneScrollLock);
            for (auto& [zoneId, spells] : g_zoneScrollBuffs)
            {
                for (auto it = spells.begin(); it != spells.end();)
                {
                    if (it->second <= now)
                    {
                        expiredZoneSpells.emplace_back(zoneId, it->first);
                        it = spells.erase(it);
                    }
                    else
                        ++it;
                }
            }
        }

        for (auto const& [zoneId, spellId] : expiredZoneSpells)
        {
            sMapMgr->DoForAllMaps([zoneId, spellId](Map* map)
            {
                for (MapReference const& ref : map->GetPlayers())
                {
                    Player* player = ref.GetSource();
                    if (player->IsInWorld() && player->GetZoneId() == zoneId && player->HasAura(spellId))
                    {
                        player->RemoveAurasDueToSpell(spellId);
                        SyncGhostRunner(player);
                    }
                }
            });
        }
    }

private:
    uint32 _timer = 0;
    static constexpr uint32 EXPIRE_CHECK_INTERVAL_MS = 10000;
};
}

void AddSC_AscensionKeepersScrollZoneBuff()
{
    new ascension_keepers_scroll_zone_buff_spell();
    new ascension_keepers_scroll_zone_buff_player();
    new ascension_keepers_scroll_zone_buff_world();
}
