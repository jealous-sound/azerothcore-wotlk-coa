/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "Chat.h"
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
};

constexpr ZoneScrollEntry kZoneScrolls[] = {
    { 696661, 993961 },  // Keeper's Scroll: Golganneth
    { 696662, 993943 },  // Keeper's Scroll: Norgannon
    { 696663, 993955 },  // Keeper's Scroll: Khaz'goroth
    { 696664, 993959 },  // Keeper's Scroll: Aggramar
    { 696665, 993957 },  // Keeper's Scroll: Eonar
    { 1179240, 91770 },  // Keeper's Scroll: Steadfast
    { 1179261, 91796 },  // Keeper's Scroll: Featherfall
    { 1179266, 91803 },  // Keeper's Scroll: Ghost Runner
    { 1179269, 91814 },  // Keeper's Scroll: Crafting Speed
};

uint32 SpellForItem(uint32 itemEntry)
{
    for (ZoneScrollEntry const& entry : kZoneScrolls)
        if (entry.ItemEntry == itemEntry)
            return entry.SpellId;
    return 0;
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

class ascension_keepers_scroll_zone_buff_player : public PlayerScript
{
public:
    ascension_keepers_scroll_zone_buff_player()
        : PlayerScript("ascension_keepers_scroll_zone_buff_player",
            {PLAYERHOOK_ON_UPDATE_ZONE, PLAYERHOOK_CAN_CAST_ITEM_USE_SPELL}) { }

    bool OnPlayerCanCastItemUseSpell(Player* player, Item* item, SpellCastTargets const& /*targets*/,
        uint8 castCount, uint32 /*glyphIndex*/) override
    {
        uint32 spellId = SpellForItem(item->GetEntry());
        if (!spellId)
            return true;

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
        int32 durationMs = spellInfo ? spellInfo->GetMaxDuration() : 0;
        if (durationMs <= 0)
            return true;

        uint32 zoneId = player->GetZoneId();
        time_t now = GameTime::GetGameTime().count();

        {
            std::lock_guard<std::mutex> lock(g_zoneScrollLock);
            time_t& slot = g_zoneScrollBuffs[zoneId][spellId];
            if (slot > now)
            {
                Spell::SendCastResult(player, spellInfo, castCount, SPELL_FAILED_AURA_BOUNCED);
                ChatHandler(player->GetSession()).PSendSysMessage(
                    "{} is already active in this zone.", spellInfo->SpellName[LOCALE_enUS]);
                return false;
            }
            slot = now + durationMs / 1000;
        }

        if (Map* map = player->GetMap())
        {
            for (MapReference const& ref : map->GetPlayers())
            {
                Player* target = ref.GetSource();
                if (target != player && target->IsInWorld() && target->GetZoneId() == zoneId)
                    ApplyZoneScrollAura(target, spellId, durationMs);
            }

            map->SendZoneText(zoneId, Acore::StringFormat(
                "{} has blessed this zone with {}!", player->GetName(), spellInfo->SpellName[LOCALE_enUS]).c_str());
        }

        return true;
    }

    void OnPlayerUpdateZone(Player* player, uint32 newZone, uint32 /*newArea*/) override
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
        std::vector<std::pair<uint32, uint32>> expired; // {zoneId, spellId}

        {
            std::lock_guard<std::mutex> lock(g_zoneScrollLock);
            for (auto& [zoneId, spells] : g_zoneScrollBuffs)
            {
                for (auto it = spells.begin(); it != spells.end();)
                {
                    if (it->second <= now)
                    {
                        expired.emplace_back(zoneId, it->first);
                        it = spells.erase(it);
                    }
                    else
                        ++it;
                }
            }
        }

        for (auto const& [zoneId, spellId] : expired)
        {
            sMapMgr->DoForAllMaps([zoneId, spellId](Map* map)
            {
                for (MapReference const& ref : map->GetPlayers())
                {
                    Player* player = ref.GetSource();
                    if (player->IsInWorld() && player->GetZoneId() == zoneId && player->HasAura(spellId))
                        player->RemoveAurasDueToSpell(spellId);
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
    new ascension_keepers_scroll_zone_buff_player();
    new ascension_keepers_scroll_zone_buff_world();
}
