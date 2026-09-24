/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU
 * AGPL v3 license:
 * https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "AscensionFelsworn.h"
#include "AscensionPyromancer.h"
#include "AscensionCultist.h"
#include "AscensionVenomancer.h"
#include "AscensionTinker.h"
#include "AscensionSunCleric.h"
#include "AllCreatureScript.h"
#include "AllSpellScript.h"
#include "AscensionChangelogCompat.h"
#include "AscensionCompatOpcodes.h"
#include "AscensionCharacterSelection.h"
#include "AscensionManastorm.h"
#include "AscensionClassMechanics.h"
#include "AscensionClassMechanics19To25.h"
#include "AscensionClassMechanics26To32.h"
#include "AscensionCoATalentData.h"
#include "AscensionCoAConfig.h"
#include "WorldSessionMgr.h"
#include "AscensionCoATalentState.h"
#include "AscensionRunemasterEchoes.h"
#include "AscensionCollectionModelData.h"
#include "AscensionAmmunitionData.h"
#include "AscensionPersonalBank.h"
#include "AscensionCollectibleSpellData.h"
#include "AscensionCustomClassData.h"
#include "AscensionAuraAmounts.h"
#include "AscensionClassTuning.h"
#include "AscensionBarbarian.h"
#include "AscensionBarbarianScaling.h"
#include "AscensionCustomResourceData.h"
#include "AscensionFreshCharacterCheck.h"
#include "AscensionLiveBaselineData.h"
#include "AscensionRacialAbilities.h"
#include "AscensionPrimalistEarthquake.h"
#include "AscensionPrimalistEarthshaping.h"
#include "AscensionPrimalistSpiritBeast.h"
#include "AscensionPrimalistWeapons.h"
#include "AscensionRunemasterTalents.h"
#include "AscensionRangerTalents.h"
#include "AscensionChronomancerTalents.h"
#include "AscensionReaperTalents.h"
#include "AscensionReaperSoulStrike.h"
#include "AscensionReaperDeathwind.h"
#include "AscensionReaperPainmail.h"
#include "AscensionReaperScytheRush.h"
#include "AscensionVenomancerCatalyst.h"
#include "AscensionSpecialization.h"
#include "AscensionSpellProgressionData.h"
#include "AscensionTalentReplacementData.h"
#include "AscensionTaughtAbilityData.h"
#include "AscensionPooledVitality.h"
#include "AscensionCreaturePreset.h"
#include "Bag.h"
#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "Chat.h"
#include "StringFormat.h"
#include "ClientDBC.h"
#include "CommandScript.h"
#include "ConfigValueCache.h"
#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "GameTime.h"
#include "GossipDef.h"
#include "GameTime.h"
#include "GlobalScript.h"
#include "GridTerrainData.h"
#include "GuildPackets.h"
#include "Item.h"
#include "ItemScript.h"
#include "LocalLevelScaling.h"
#include "Log.h"
#include "Map.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "QuestDef.h"
#include "Random.h"
#include "Realm.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "Spell.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Timer.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include <algorithm>
#include <array>
#include <bit>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <type_traits>
#include <deque>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#ifndef _WIN32
extern char** environ;
#endif

using namespace Acore::ChatCommands;

namespace {
constexpr uint16 CMSG_ANTICHEAT_ALERT = 0x051F;
constexpr uint16 CMSG_CUSTOM_ASCENSION_POINT_SPEND_REQUEST = 0x0523;
constexpr uint16 CMSG_EXTENSION_INITIALIZED = 0x0561;
constexpr uint16 CMSG_CREATURE_QUERY_BULK = 0x061A;
constexpr uint16 CMSG_ITEM_QUERY_BULK = 0x061B;
constexpr uint16 CMSG_APPLY_APPEARANCES = 0x0697;
constexpr uint16 SMSG_APPLY_APPEARANCES_RESULT = 0x0698;
constexpr uint16 SMSG_APPEARANCE_COLLECTION_INFO = 0x0699;
constexpr uint16 SMSG_APPEARANCE_ACTIVE_INFO = 0x069A;
constexpr uint16 SMSG_APPEARANCE_ADDED = 0x069B;
constexpr uint16 SMSG_APPEARANCE_OUTFIT_INFO = 0x069D;
constexpr uint16 SMSG_CAN_SEE_APPEARANCES_INFO = 0x06A2;
constexpr uint16 CMSG_SET_CAN_SEE_APPEARANCES = 0x06A3;
constexpr uint16 SMSG_VANITY_COLLECTION_INFO = 0x06F7;
constexpr uint16 SMSG_VANITY_COLLECTION_ADDED = 0x06F8;

constexpr uint16 SMSG_QUERY_CUSTOM_STORE_RESULT = 0x06BA;
constexpr std::size_t VANITY_STORE_RECORD_DWORDS = 16;
constexpr uint16 SMSG_CHARACTER_ADVANCEMENT_ACTIVE_SPEC = 0x0725;
constexpr uint16 SMSG_CHARACTER_ADVANCEMENT_KNOWN_ENTRIES = 0x0726;
constexpr uint16 CMSG_CHARACTER_ADVANCEMENT_KNOWN_ENTRIES = 0x0727;
constexpr uint16 CMSG_MISSILE_FIRE_POSITION = 0x09C7;

constexpr uint16 SMSG_PATCH_VANITY_COLLECTION = 0x0573;
constexpr uint16 SMSG_UPDATE_OBJECT_ADDON = 0x0578;
constexpr uint32 PLAYER_ADDON_FIELD_AVERAGE_ITEM_LEVEL = 5;

constexpr uint16 SMSG_REALM_INFO = 0x09BC;
constexpr uint8 REALM_CREATION_FLAG_CONQUEST_OF_AZEROTH = 6;
constexpr uint8 REALM_CREATION_FLAG_WARCRAFT_REBORN = 7;
constexpr uint8 REALM_INFO_ADDONS_ALLOWED = 1;

constexpr uint16 SMSG_BANK_PERMISSIONS = 0x0769;

constexpr uint32 MAX_CREATURE_QUERY_BULK_ENTRIES = 50;
constexpr uint32 MAX_ITEM_QUERY_BULK_ENTRIES = 50;
constexpr std::size_t MAX_EXTENSION_REPLIES_PER_UPDATE = 150;
static_assert(MAX_CREATURE_QUERY_BULK_ENTRIES <= MAX_EXTENSION_REPLIES_PER_UPDATE);
static_assert(MAX_ITEM_QUERY_BULK_ENTRIES <= MAX_EXTENSION_REPLIES_PER_UPDATE);
constexpr std::size_t POINT_SPEND_REQUEST_SIZE = sizeof(uint8) + sizeof(uint32);
constexpr uint8 VANITY_CURRENCY_DONATION_POINTS = 2;

constexpr std::array<uint16, 4> QUEUED_EXTENSION_OPCODES = {
    CMSG_APPLY_APPEARANCES, CMSG_SET_CAN_SEE_APPEARANCES,
    CMSG_EXTENSION_INITIALIZED, CMSG_CUSTOM_ASCENSION_POINT_SPEND_REQUEST};

struct ExtensionOpcodeIdentity {
  uint16 Opcode;
  char const *Name;
};

constexpr ExtensionOpcodeIdentity EXTENSION_OPCODES[] = {
    {CMSG_ANTICHEAT_ALERT, "CMSG_ANTICHEAT_ALERT"},
    {CMSG_CUSTOM_ASCENSION_POINT_SPEND_REQUEST, "CMSG_CUSTOM_ASCENSION_POINT_SPEND_REQUEST"},
    {0x053B, "CMSG_ASCENSIONGM_TICKET_LIST_REQUEST"},
    {CMSG_EXTENSION_INITIALIZED, "CMSG_EXTENSION_INITIALIZED"},
    {0x05A1, "CMSG_CHALLENGE_QUERY_FAILURE"},
    {CMSG_ITEM_QUERY_BULK, "CMSG_ITEM_QUERY_BULK"},
    {0x0667, "CMSG_SET_LEVEL_SCALING"},
    {CMSG_APPLY_APPEARANCES, "CMSG_APPLY_APPEARANCES"},
    {SMSG_APPLY_APPEARANCES_RESULT, "SMSG_APPLY_APPEARANCES_RESULT"},
    {SMSG_APPEARANCE_COLLECTION_INFO, "SMSG_APPEARANCE_COLLECTION_INFO"},
    {SMSG_APPEARANCE_ACTIVE_INFO, "SMSG_APPEARANCE_ACTIVE_INFO"},
    {SMSG_APPEARANCE_ADDED, "SMSG_APPEARANCE_ADDED"},
    {SMSG_APPEARANCE_OUTFIT_INFO, "SMSG_APPEARANCE_OUTFIT_INFO"},
    {SMSG_CAN_SEE_APPEARANCES_INFO, "SMSG_CAN_SEE_APPEARANCES_INFO"},
    {CMSG_SET_CAN_SEE_APPEARANCES, "CMSG_SET_CAN_SEE_APPEARANCES"},
    {0x06B9, "CMSG_QUERY_CUSTOM_STORE"},
    {SMSG_VANITY_COLLECTION_INFO, "SMSG_VANITY_COLLECTION_INFO"},
    {SMSG_VANITY_COLLECTION_ADDED, "SMSG_VANITY_COLLECTION_ADDED"},
    {SMSG_QUERY_CUSTOM_STORE_RESULT, "SMSG_QUERY_CUSTOM_STORE_RESULT"},
    {0x06FD, "CMSG_QUERY_INSTANCE_BINDS"},
    {SMSG_CHARACTER_ADVANCEMENT_ACTIVE_SPEC, "SMSG_CHARACTER_ADVANCEMENT_ACTIVE_SPEC"},
    {SMSG_CHARACTER_ADVANCEMENT_KNOWN_ENTRIES, "SMSG_CHARACTER_ADVANCEMENT_KNOWN_ENTRIES"},
    {CMSG_CHARACTER_ADVANCEMENT_KNOWN_ENTRIES, "CMSG_CHARACTER_ADVANCEMENT_KNOWN_ENTRIES"},
    {0x0741, "CMSG_GOSSIP_CLOSE"},
    {0x0745, "CMSG_PLAYER_POLL_LIST_REQUEST"},
    {SMSG_BANK_PERMISSIONS, "SMSG_BANK_PERMISSIONS"},
    {0x0777, "CMSG_STATISTIC_QUERY"},
    {CMSG_MISSILE_FIRE_POSITION, "CMSG_MISSILE_FIRE_POSITION"},
};

[[nodiscard]] char const *ExtensionOpcodeName(uint16 opcode) {
  for (ExtensionOpcodeIdentity const &entry : EXTENSION_OPCODES)
    if (entry.Opcode == opcode)
      return entry.Name;
  return nullptr;
}

[[nodiscard]] std::string DescribePacketPayload(WorldPacket const &packet,
                                                std::size_t limit = 64) {
  std::size_t const size = packet.size();
  if (!size)
    return "";

  std::size_t count = std::min(size, limit);
  uint8 const *bytes = nullptr;
  try {
    bytes = const_cast<WorldPacket &>(packet).contents();
  } catch (...) {
    return "<unreadable>";
  }
  if (!bytes || !count)
    return "";

  static constexpr char HEX_DIGITS[] = "0123456789ABCDEF";
  std::string description;
  description.reserve(count * 3);
  for (std::size_t i = 0; i < count; ++i) {
    description += HEX_DIGITS[(bytes[i] >> 4) & 0x0F];
    description += HEX_DIGITS[bytes[i] & 0x0F];
    description += ' ';
  }
  if (packet.size() > limit)
    description += "...";
  return description;
}

[[nodiscard]] std::vector<uint32> ReadBulkQueryEntries(WorldPacket const& packet, uint32 maxEntries)
{
    if (packet.size() < sizeof(uint32))
        return {};

    uint32 const count = packet.read<uint32>(0);
    if (!count || count > maxEntries ||
        packet.size() != sizeof(uint32) + std::size_t(count) * sizeof(uint32))
        return {};

    std::vector<uint32> entries;
    entries.reserve(count);
    for (uint32 index = 0; index < count; ++index)
        entries.push_back(packet.read<uint32>(sizeof(uint32) + std::size_t(index) * sizeof(uint32)));
    return entries;
}

[[nodiscard]] std::size_t ExpectedReplies(WorldPacket const& packet)
{
    uint16 const opcode = packet.GetOpcode();
    return opcode == CMSG_ITEM_QUERY_BULK || opcode == CMSG_CREATURE_QUERY_BULK ? packet.read<uint32>(0) : 1;
}

constexpr uint32 SPELL_PYROMANCER_HEAT = 807389;
constexpr uint32 SPELL_PYROMANCER_EMBER = 807533;
constexpr uint32 SPELL_PRIMALIST_EARTHSHAPING = 680441;
constexpr uint32 SPELL_STORMBRINGER_STATIC = 803102;
constexpr uint32 SPELL_STORMBRINGER_CHARGED_CONDUIT = 803790;
constexpr uint32 SPELL_STORMBRINGER_WRATH_OF_ALAKIR = 300834;
constexpr uint32 SPELL_STORMBRINGER_UNSHACKLE_EXTENSION = 300835;
constexpr uint32 SPELL_BLOODMAGE_THIRST_PASSIVE = 92112;
constexpr uint32 SPELL_BLOODMAGE_THIRST = 706613;
constexpr uint32 BLOODMAGE_FLESHWEAVER_SPEC = 25;
constexpr uint32 SPELL_REAPER_REAPED_SOUL = 500363;
constexpr uint32 SPELL_REAPER_SOUL_INFUSION = 803031;
constexpr uint32 SPELL_REAPER_SOUL_INFUSION_REMOVER = 561290;
constexpr uint32 SPELL_REAPER_SOUL_FRAGMENT = 805077;
constexpr uint32 SPELL_REAPER_GENERATE_SOUL = 805078;
constexpr uint32 SPELL_REAPER_SCYTHE_RUSH = 500359;
constexpr uint32 SPELL_REAPER_SCYTHE_RUSH_MARKER = 500377;
constexpr uint32 SPELL_REAPER_HARVEST_TIME = 803995;
constexpr uint32 SPELL_REAPER_TIER_FROSTBITTEN_4P = 2990002;
constexpr uint32 SPELL_REAPER_HARVESTED_MIGHT = 2990003;
constexpr char ASCENSION_LOCAL_RESOURCE_PREFIX[] = "ASC_LOCAL_RESOURCE";
constexpr char ASCENSION_ACTIVE_SPEC_SETTING[] = "core.ascension_active_spec";
constexpr char ASCENSION_TALENT_BUILD_SETTING_PREFIX[] = "core.ascension_build.";

enum CompanionLoot : uint32
{
    APPEARANCE_CATEGORY_COMPANION_LOOT = 38,
    APPEARANCE_CATEGORY_COMPANION_SKINNING = 61,
    APPEARANCE_LOOT_TRANSFIGURATOR = 47520,
    APPEARANCE_SKIN_PEELER = 639807,
    SPELL_LOOT_TRANSFIGURATOR = 84419,
    SPELL_SKIN_PEELER = 92864
};

constexpr uint8 PYROMANCER_HEAT_PER_EMBER = 100;
constexpr uint8 REAPER_SOUL_FRAGMENT_COST = 3;

constexpr std::array<uint32, 12> REAPER_ALL_SOUL_CONSUMERS =
{{
    500483,
    500484,
    500576,
    500631,
    572341, 572342, 573316, 573317, 573318, 573319, 573321, 573322
}};

constexpr std::array<std::pair<uint32, uint32>, 1> REAPER_ONE_SOUL_CONSUMERS =
{{
    {500361, 500361}
}};

constexpr std::size_t APPEARANCE_CATEGORY_COUNT = 69;
constexpr uint32 APPEARANCE_CATEGORY_AMMUNITION = 32;
constexpr std::size_t MAX_APPEARANCE_SNAPSHOT_ENTRIES = 65536;
constexpr std::size_t APPEARANCE_ADDS_PER_BATCH = 16;
constexpr uint32 APPEARANCE_ADD_BATCH_INTERVAL_MS = 100;
constexpr uint32 APPEARANCE_ADD_INITIAL_DELAY_MS = 500;
constexpr uint32 APPEARANCE_LOGIN_RESYNC_DELAY_MS = 3000;
constexpr std::size_t MAX_QUEUED_EXTENSION_PACKETS = 64;
constexpr uint32 VANITY_CATEGORY_MOUNTS = 0x04000000;
constexpr uint32 VANITY_CATEGORY_COMPANIONS = 0x08000000;
constexpr uint32 ITEM_WONDROUS_WISDOMBALL = 101169;
constexpr uint32 ITEM_FIX_O_TRON_5000 = 97330;
constexpr std::size_t COMPANION_SPELLS_PER_BATCH = 4;
constexpr uint32 COMPANION_SPELL_BATCH_INTERVAL_MS = 200;

enum AscensionRidingSpells : uint32
{
    SPELL_RIDING_APPRENTICE = 33388,
    SPELL_RIDING_JOURNEYMAN = 33391,
    SPELL_RIDING_EXPERT = 34090,
    SPELL_RIDING_ARTISAN = 34091,
    SPELL_COLD_WEATHER_FLYING = 54197
};

enum class AscensionCompatConfig {
  ENABLED,
  LOG_CONSUMED_PACKETS,
  FIRST_EXTENSION_OPCODE,
  LAST_EXTENSION_OPCODE,
  AUTO_COLLECT_APPEARANCES,
  UNLOCK_LOCAL_APPEARANCE_CATALOG,
  APPEARANCE_CATALOG_PER_CATEGORY,
  UNLOCK_ALL_VANITY,
  REALM_TYPE,
  CLASS_MODEL,
  ALLOW_LEARNED_SPELL_DELIVERY,
  LEARN_OWNED_COMPANIONS,
  MAX_RIDING_FROM_START,
  LEVEL_SCALING,
  QUEST_LEVEL_SCALING,
  AUTO_PROGRESSION,

  NUM_CONFIGS,
};

class AscensionCompatConfigData
    : public ConfigValueCache<AscensionCompatConfig> {
public:
  AscensionCompatConfigData()
      : ConfigValueCache(AscensionCompatConfig::NUM_CONFIGS) {}

  void BuildConfigCache() override {
    SetConfigValue<bool>(AscensionCompatConfig::ENABLED,
                         "CoA.Enable", true);
    SetConfigValue<bool>(AscensionCompatConfig::LOG_CONSUMED_PACKETS,
                         "CoA.LogConsumedPackets", true);
    SetConfigValue<uint32>(AscensionCompatConfig::FIRST_EXTENSION_OPCODE,
                           "CoA.FirstExtensionOpcode", 0x051F);
    SetConfigValue<uint32>(AscensionCompatConfig::LAST_EXTENSION_OPCODE,
                           "CoA.LastExtensionOpcode", 0x09D3);
    SetConfigValue<bool>(AscensionCompatConfig::AUTO_COLLECT_APPEARANCES,
                         "CoA.AutoCollectAppearances", true);
    SetConfigValue<bool>(
        AscensionCompatConfig::UNLOCK_LOCAL_APPEARANCE_CATALOG,
        "CoA.UnlockLocalAppearanceCatalog", true);
    SetConfigValue<uint32>(
        AscensionCompatConfig::APPEARANCE_CATALOG_PER_CATEGORY,
        "CoA.AppearanceCatalogPerCategory", 500);
    SetConfigValue<bool>(AscensionCompatConfig::UNLOCK_ALL_VANITY,
                         "CoA.UnlockAllVanity", true);
    SetConfigValue<std::string>(AscensionCompatConfig::REALM_TYPE,
                                "CoA.RealmType", "live");
    SetConfigValue<std::string>(AscensionCompatConfig::CLASS_MODEL,
                                "CoA.ClassModel", "coa");
    SetConfigValue<bool>(AscensionCompatConfig::ALLOW_LEARNED_SPELL_DELIVERY,
                         "CoA.AllowLearnedSpellDelivery", true);
    SetConfigValue<bool>(AscensionCompatConfig::LEARN_OWNED_COMPANIONS,
                         "CoA.LearnOwnedCompanions", true);
    SetConfigValue<bool>(AscensionCompatConfig::MAX_RIDING_FROM_START,
                         "CoA.MaxRidingFromStart", true);
    SetConfigValue<bool>(AscensionCompatConfig::LEVEL_SCALING,
                         "CoA.LevelScaling", true);
    SetConfigValue<bool>(AscensionCompatConfig::QUEST_LEVEL_SCALING,
                         "CoA.QuestLevelScaling", true);
    SetConfigValue<bool>(AscensionCompatConfig::AUTO_PROGRESSION,
                         "CoA.AutoProgression", false);
  }
};

AscensionCompatConfigData ascensionCompatConfig;

struct AppearanceInfo {
  uint32 SourceItem = 0;
  uint32 PrimaryCategory = 0;
  uint32 SecondaryCategory = 0;
  uint32 TertiaryCategory = 0;
  uint32 EnchantId = 0;
  uint32 CosmeticSpell = 0;
};

struct VanityInfo {
  uint32 LearnedSpell = 0;
  uint32 Flags = 0;
  uint32 CategoryMask = 0;
  std::array<uint32, VANITY_STORE_RECORD_DWORDS> StoreRecord{};
};

struct PlayerCollectionState {
  uint32 AccountId = 0;
  std::unordered_set<uint32> CollectedAppearances;
  std::unordered_set<uint32> OwnedVanityItems;
  std::array<uint32, APPEARANCE_CATEGORY_COUNT> ActiveAppearances{};
  std::vector<uint32> PendingAppearanceAdds;
  std::size_t NextPendingAppearanceAdd = 0;
  uint32 AppearanceAddTimer = 0;
  uint32 LoginResyncTimer = 0;
  std::vector<uint32> PendingCompanionSpells;
  std::size_t NextCompanionSpell = 0;
  uint32 CompanionSpellTimer = 0;
  uint32 CompanionLootTimer = 0;
  uint32 CompanionSkinningTimer = 0;
  uint32 CosmeticTimer = 0;
  std::unordered_set<uint32> AppliedCosmeticSpells;
  bool CanSeeItemAppearances = true;
  bool CanSeeSpellAppearances = true;
};

uint8 AppearanceCategoryForEquipmentSlot(uint8 slot) {
  switch (slot) {
  case EQUIPMENT_SLOT_HEAD:
    return 1;
  case EQUIPMENT_SLOT_SHOULDERS:
    return 2;
  case EQUIPMENT_SLOT_BACK:
    return 3;
  case EQUIPMENT_SLOT_CHEST:
    return 4;
  case EQUIPMENT_SLOT_TABARD:
    return 5;
  case EQUIPMENT_SLOT_BODY:
    return 6;
  case EQUIPMENT_SLOT_WRISTS:
    return 7;
  case EQUIPMENT_SLOT_HANDS:
    return 8;
  case EQUIPMENT_SLOT_WAIST:
    return 9;
  case EQUIPMENT_SLOT_LEGS:
    return 10;
  case EQUIPMENT_SLOT_FEET:
    return 11;
  case EQUIPMENT_SLOT_RANGED:
    return 12;
  case EQUIPMENT_SLOT_MAINHAND:
    return 13;
  case EQUIPMENT_SLOT_OFFHAND:
    return 14;
  default:
    return 0;
  }
}

uint8 WeaponEffectCategoryForEquipmentSlot(uint8 slot) {
  switch (slot) {
  case EQUIPMENT_SLOT_MAINHAND:
    return 15;
  case EQUIPMENT_SLOT_OFFHAND:
    return 16;
  default:
    return 0;
  }
}

bool IsAscensionCustomClass(Player const *player) {
  uint8 playerClass = player->getClass();
  return playerClass >= CLASS_BARBARIAN && playerClass <= CLASS_SPIRIT_MAGE;
}

enum LegacyQuestSpells : uint32
{
    QuestStoneskinTotem = 8073,
    QuestPathOfDefense = 8121,
    LegacyDefensiveStance = 1100071,
    LegacyTaunt = 1100355,
    LegacySunderArmor = 1107386,
    LegacyStoneskinTotem = 1108071
};

struct LegacyQuestReward
{
    uint32 Wrapper;
    std::array<uint32, MAX_SPELL_EFFECTS> Spells;
};

constexpr std::array<LegacyQuestReward, 2> LegacyQuestRewards = {{
    {QuestStoneskinTotem, {LegacyStoneskinTotem, 0, 0}},
    {QuestPathOfDefense, {LegacyDefensiveStance, LegacySunderArmor, LegacyTaunt}}
}};

LegacyQuestReward const* GetLegacyQuestReward(uint32 wrapper)
{
    for (LegacyQuestReward const& reward : LegacyQuestRewards)
        if (reward.Wrapper == wrapper)
            return &reward;
    return nullptr;
}

void RemoveLegacyQuestSpells(Player* player)
{
    if (!IsAscensionCustomClass(player))
        return;

    for (uint32 questId : player->getRewardedQuests())
        if (Quest const* quest = sObjectMgr->GetQuestTemplate(questId))
            if (LegacyQuestReward const* reward = GetLegacyQuestReward(quest->GetRewSpellCast()))
                for (uint32 spell : reward->Spells)
                    if (spell)
                        player->removeSpell(spell, SPEC_MASK_ALL, false);
}

AscensionCompatData::StarterKit const *GetStarterKit(uint8 playerClass) {
  auto itr = std::find_if(
      AscensionCompatData::StarterKits.begin(),
      AscensionCompatData::StarterKits.end(),
      [playerClass](AscensionCompatData::StarterKit const &kit) {
        return kit.ClassId == playerClass;
      });
  return itr == AscensionCompatData::StarterKits.end() ? nullptr : &*itr;
}

std::vector<uint32> GetAscensionRacialSpells(Player const* player)
{
    std::vector<uint32> spells;
    for (auto const& skill : AscensionRacialAbilities::Skills)
        if (skill.RaceId == player->getRace())
            for (SkillLineAbilityEntry const* ability : GetSkillLineAbilitiesBySkillLine(skill.SkillId))
                if (AscensionRacialAbilities::CanLearn(*ability, player->getRace(), player->getClass()))
                    spells.push_back(ability->Spell);

    std::sort(spells.begin(), spells.end());
    spells.erase(std::unique(spells.begin(), spells.end()), spells.end());
    return spells;
}

struct FelswornRiftGrant
{
    uint32 SpellId;
    uint8 RequiredLevel;
};

constexpr std::array<FelswornRiftGrant, 3> FelswornHordeCapitalRifts =
{{
    {535598, 26},
    {535599, 30},
    {535600, 36}
}};

constexpr std::array<uint32, 6> FelswornCapitalRifts = {535595, 535596, 535597, 535598, 535599, 535600};

bool CanGrantAscensionRacialSpell(Player const* player, uint32 spellId)
{
    bool racial = false;
    auto const bounds = sSpellMgr->GetSkillLineAbilityMapBounds(spellId);
    if (std::find(FelswornCapitalRifts.begin(), FelswornCapitalRifts.end(), spellId) != FelswornCapitalRifts.end())
        for (auto itr = bounds.first; itr != bounds.second; ++itr)
            if (itr->second->RaceMask && !(itr->second->RaceMask & player->getRaceMask()))
                return false;
    for (auto itr = bounds.first; itr != bounds.second; ++itr)
        if (AscensionRacialAbilities::GetRace(itr->second->SkillLine))
        {
            racial = true;
            if (AscensionRacialAbilities::CanLearn(*itr->second, player->getRace(), player->getClass()))
                return true;
        }
    return !racial;
}

static void LearnRestoredSpell(Player* player, uint32 spellId)
{
    player->learnSpell(spellId, false);
    player->MarkSpellForSave(spellId);
}

class AscensionClassService {
public:
  static AscensionClassService &Instance() {
    static AscensionClassService instance;
    return instance;
  }

  uint32 SynchronizeProgression(Player *player, bool explicitRequest = false) {
    if (!IsAscensionCustomClass(player))
      return 0;

    uint32 const activeSpec = GetActiveSpecialization(player);
    AscensionClassTuning::Synchronize(player, activeSpec, true);
    auto const racialSpells = GetAscensionRacialSpells(player);
    auto selectedTalentOwns = [player, activeSpec](uint32 spellId)
    {
      uint32 const root = sSpellMgr->GetFirstSpellInChain(spellId);
      return std::any_of(AscensionCompatData::CoATalentEntries.begin(),
          AscensionCompatData::CoATalentEntries.end(), [player, activeSpec, spellId, root](auto const& entry)
          {
            if (entry.ClassId != player->getClass() || (activeSpec && entry.SpecId && entry.SpecId != activeSpec) ||
                entry.RequiredLevel > player->GetLevel() || (!entry.AECost && !entry.TECost))
              return false;
            return std::any_of(entry.SpellIds.begin(), entry.SpellIds.end(),
                [spellId, root](uint32 id) { return id && (id == spellId || id == root); });
          });
    };
    auto currentGrantAllows = [player, activeSpec, &selectedTalentOwns, &racialSpells](uint32 spellId)
    {
      if (!CanGrantAscensionRacialSpell(player, spellId))
        return false;
      bool const observed = std::any_of(AscensionLiveBaseline::Spells.begin(), AscensionLiveBaseline::Spells.end(),
          [player, spellId](auto const& entry)
          { return entry.ClassId == player->getClass() && entry.SpellId == spellId && (!entry.RaceId || entry.RaceId == player->getRace()); });
      bool const proficiency = std::any_of(AscensionLiveBaseline::Proficiencies.begin(), AscensionLiveBaseline::Proficiencies.end(),
          [player, spellId](auto const& entry) { return entry.ClassId == player->getClass() && entry.SpellId == spellId; });
      bool const unresolved = std::any_of(AscensionCompatData::UnresolvedTrainerSpells.begin(), AscensionCompatData::UnresolvedTrainerSpells.end(),
          [player, spellId](auto const& entry) { return entry.ClassId == player->getClass() && entry.SpellId == spellId; });
      bool const automatic = player->GetLevel() > 1 && std::any_of(AscensionCompatData::CoATalentEntries.begin(),
          AscensionCompatData::CoATalentEntries.end(), [player, activeSpec, spellId](auto const& entry)
          {
            return CanGrantAutomaticEntry(player, entry, activeSpec) &&
                   std::find(entry.SpellIds.begin(), entry.SpellIds.end(), spellId) != entry.SpellIds.end();
          });
      return std::binary_search(racialSpells.begin(), racialSpells.end(), spellId) ||
          observed || proficiency || unresolved || automatic || selectedTalentOwns(spellId) ||
          std::any_of(AscensionCompatData::ClassSpells.begin(),
          AscensionCompatData::ClassSpells.end(), [player, spellId](auto const& entry)
          {
            return entry.ClassId == player->getClass() && entry.SpellId == spellId &&
                   entry.RequiredLevel <= player->GetLevel();
          });
    };
    uint32 removed = 0;
    auto reconcile = [player, &currentGrantAllows, &removed](auto const& entries)
    {
      for (auto const& entry : entries)
        if (entry.ClassId == player->getClass() && player->HasSpell(entry.SpellId) &&
            !currentGrantAllows(entry.SpellId))
        {
          player->removeSpell(entry.SpellId, SPEC_MASK_ALL, false);
          ++removed;
        }
    };
    reconcile(AscensionCompatData::LegacyGeneratedClassSpells);
    reconcile(AscensionCompatData::ClassSpells);
    if (removed)
      LOG_INFO("coa", "Reconciled {} proven class grants for {} against live level {}",
          removed, player->GetName(), uint32(player->GetLevel()));
    uint32 learned = 0;
    bool const botCannotBuyBooksOfAscension = player->GetSession() && player->GetSession()->IsBot();
    bool const automaticProgression =
        explicitRequest || ascensionCompatConfig.GetConfigValue<bool>(
                               AscensionCompatConfig::AUTO_PROGRESSION) ||
        botCannotBuyBooksOfAscension;
    for (uint32 spellId : racialSpells)
        if (automaticProgression && !player->HasSpell(spellId) && sSpellMgr->GetSpellInfo(spellId))
        {
            LearnRestoredSpell(player, spellId);
            ++learned;
        }
    for (auto const& entry : AscensionLiveBaseline::Spells)
      if (automaticProgression && entry.ClassId == player->getClass() &&
          (!entry.RaceId || entry.RaceId == player->getRace()) &&
          CanGrantAscensionRacialSpell(player, entry.SpellId) &&
          !player->HasSpell(entry.SpellId) && sSpellMgr->GetSpellInfo(entry.SpellId))
      {
        LearnRestoredSpell(player, entry.SpellId);
        ++learned;
      }
    for (AscensionCompatData::ClassSpell const &progressionSpell :
         AscensionCompatData::ClassSpells) {
      if (!automaticProgression ||
          progressionSpell.ClassId != player->getClass() ||
          progressionSpell.RequiredLevel > player->GetLevel() ||
          !CanGrantAscensionRacialSpell(player, progressionSpell.SpellId) ||
          player->HasSpell(progressionSpell.SpellId))
        continue;

      if (!sSpellMgr->GetSpellInfo(progressionSpell.SpellId))
      {
        LOG_ERROR("coa",
                  "Cannot teach missing Ascension class spell {} to {}",
                  progressionSpell.SpellId, player->GetName());
        continue;
      }

      LearnRestoredSpell(player, progressionSpell.SpellId);
      ++learned;
    }
    if (player->getClass() == CLASS_DEMON_HUNTER)
      for (FelswornRiftGrant const& rift : FelswornHordeCapitalRifts)
        if (automaticProgression && rift.RequiredLevel <= player->GetLevel() &&
            CanGrantAscensionRacialSpell(player, rift.SpellId) &&
            !player->HasSpell(rift.SpellId) && sSpellMgr->GetSpellInfo(rift.SpellId))
        {
          player->learnSpell(rift.SpellId, false);
          ++learned;
        }

    ReconcileRunemasterFists(player, activeSpec);
    learned += SynchronizeAutomaticTalents(player, GetActiveSpecialization(player));
    for (AscensionProgression::Rank const& rank : AscensionProgression::Ranks)
    {
        if (!automaticProgression || rank.ClassId != player->getClass() ||
            rank.RequiredLevel > player->GetLevel() ||
            !player->HasSpell(rank.FirstSpellId) || player->HasSpell(rank.SpellId))
            continue;

        if (sSpellMgr->GetSpellInfo(rank.SpellId))
        {
            LearnRestoredSpell(player, rank.SpellId);
            ++learned;
        }
    }

    learned += SynchronizeTaughtAbilities(player);
    learned += SynchronizeTalentReplacements(player);
    RemoveAscensionPrimalistWeapons(player);
    SynchronizeAscensionRunemasterEchoes(player, GetActiveSpecialization(player));

    if (learned)
    {
      ChatHandler(player->GetSession())
          .PSendSysMessage("Restored {} Ascension class abilities.", learned);
      LOG_INFO("coa",
               "Restored {} progression spells for {} (class {}, level {})",
               learned, player->GetName(), uint32(player->getClass()),
               uint32(player->GetLevel()));
    }

    return learned;
  }

    bool AffectsTaughtAbilities(uint32 spellId) const
    {
        return std::any_of(AscensionCompatData::TaughtAbilities.begin(),
            AscensionCompatData::TaughtAbilities.end(),
            [spellId](auto const& entry) { return entry.ParentSpellId == spellId; });
    }

    uint32 SynchronizeTaughtAbilities(Player* player, bool beforeMap = false)
    {
        if (!IsAscensionCustomClass(player))
            return 0;

        uint32 const specializationId = GetActiveSpecialization(player);
        uint32 learned = 0;
        for (auto const& entry : AscensionCompatData::TaughtAbilities)
        {
            if (entry.ClassId != player->getClass())
                continue;

            bool const allowed = specializationId == entry.SpecId &&
                player->GetLevel() >= entry.RequiredLevel && player->HasSpell(entry.ParentSpellId);
            if (!allowed)
            {
                player->removeSpell(entry.SpellId, SPEC_MASK_ALL, true);
                continue;
            }

            auto const& spells = player->GetSpellMap();
            if (spells.find(entry.SpellId) != spells.end() || !sSpellMgr->GetSpellInfo(entry.SpellId))
                continue;

            player->learnSpell(entry.SpellId, true);
            if (player->HasSpell(entry.SpellId))
                ++learned;
        }
        if (!beforeMap && player->getClass() == CLASS_SON_OF_ARUGAL)
        {
            bool const dualWield = player->HasSpell(674);
            if (player->CanDualWield() != dualWield)
            {
                player->SetCanDualWield(dualWield);
                if (!dualWield)
                    player->AutoUnequipOffhandIfNeed();
            }
        }
        return learned;
    }

    bool AffectsTalentReplacements(uint32 spellId) const
    {
        uint32 const root = sSpellMgr->GetFirstSpellInChain(spellId);
        return std::any_of(AscensionCompatData::TalentReplacements.begin(),
            AscensionCompatData::TalentReplacements.end(), [spellId, root](auto const& entry)
            {
                return entry.ParentSpellId == spellId || entry.OriginalSpellId == root;
            });
    }

    uint32 SynchronizeTalentReplacements(Player* player)
    {
        if (!IsAscensionCustomClass(player))
            return 0;

        uint32 const specializationId = GetActiveSpecialization(player);
        std::set<uint32> candidates;
        std::map<uint32, uint32> replacements;
        for (auto const& entry : AscensionCompatData::TalentReplacements)
        {
            if (entry.ClassId != player->getClass())
                continue;

            uint32 replacement = 0;
            bool const allowed = specializationId == entry.SpecId && player->HasSpell(entry.ParentSpellId);
            for (auto const& rank : entry.Ranks)
            {
                if (!rank.SpellId)
                    continue;
                candidates.insert(rank.SpellId);
                if (allowed && rank.RequiredLevel <= player->GetLevel() && sSpellMgr->GetSpellInfo(rank.SpellId))
                    replacement = rank.SpellId;
            }

            for (auto const& [id, spell] : player->GetSpellMap())
            {
                if (sSpellMgr->GetFirstSpellInChain(id) != entry.OriginalSpellId)
                    continue;
                replacements.try_emplace(id, 0);
                if (replacement && player->HasActiveSpell(id))
                    replacements[id] = replacement;
            }
        }

        std::set<uint32> desired;
        for (auto const& [id, replacement] : replacements)
        {
            if (replacement)
                desired.insert(replacement);
            if (player->GetTemporarySpellReplacement(id) != replacement)
                player->SetTemporarySpellReplacement(id, 0);
        }

        for (uint32 id : candidates)
        {
            if (desired.count(id))
                continue;
            bool const neededByHigherRank = std::any_of(desired.begin(), desired.end(), [id](uint32 rank)
            {
                return sSpellMgr->GetFirstSpellInChain(id) == sSpellMgr->GetFirstSpellInChain(rank) &&
                    sSpellMgr->GetSpellRank(id) < sSpellMgr->GetSpellRank(rank);
            });
            if (!neededByHigherRank)
                player->removeSpell(id, SPEC_MASK_ALL, true);
        }

        uint32 learned = 0;
        for (uint32 id : desired)
        {
            if (player->GetSpellMap().find(id) == player->GetSpellMap().end())
            {
                player->learnSpell(id, true);
                if (player->HasSpell(id))
                    ++learned;
            }
        }
        for (auto const& [id, replacement] : replacements)
            player->SetTemporarySpellReplacement(id, replacement);
        return learned;
    }

  bool AffectsProficiencies(uint32 spellId) const {
    bool const isProficiency = std::any_of(
        AscensionCompatData::ProficiencyDefinitions.begin(),
        AscensionCompatData::ProficiencyDefinitions.end(),
        [spellId](AscensionCompatData::ProficiencyDefinition const &entry) {
          return entry.SpellId == spellId;
        });
    if (isProficiency)
      return true;

    return std::any_of(
        AscensionCompatData::TalentProficiencies.begin(),
        AscensionCompatData::TalentProficiencies.end(),
        [spellId](AscensionCompatData::TalentProficiency const &entry) {
          return entry.TalentSpellId == spellId;
        });
  }

  void SynchronizeProficiencies(Player *player) {
    if (!IsAscensionCustomClass(player))
      return;

    uint32 const guid = player->GetGUID().GetCounter();
    {
      std::lock_guard<std::mutex> lock(_stateLock);
      if (!_proficiencySynchronizations.insert(guid).second)
        return;
    }

    auto isAllowed = [player](uint32 proficiencySpellId) {
      bool const isObserved = std::any_of(
          AscensionLiveBaseline::Proficiencies.begin(), AscensionLiveBaseline::Proficiencies.end(),
          [player, proficiencySpellId](AscensionLiveBaseline::Proficiency const& entry)
          {
              return entry.ClassId == player->getClass() && entry.SpellId == proficiencySpellId;
          });
      if (isObserved)
        return true;

      return std::any_of(
          AscensionCompatData::TalentProficiencies.begin(),
          AscensionCompatData::TalentProficiencies.end(),
          [player, proficiencySpellId](
              AscensionCompatData::TalentProficiency const &entry) {
            return entry.ClassId == player->getClass() &&
                   entry.ProficiencySpellId == proficiencySpellId &&
                   player->HasSpell(entry.TalentSpellId);
          });
    };

    uint32 learned = 0;
    uint32 removed = 0;
    for (AscensionCompatData::ProficiencyDefinition const &definition :
         AscensionCompatData::ProficiencyDefinitions) {
      bool const allowed = isAllowed(definition.SpellId);
      if (allowed)
      {
        if (!player->HasSpell(definition.SpellId))
        {
          if (sSpellMgr->GetSpellInfo(definition.SpellId))
          {
            LearnRestoredSpell(player, definition.SpellId);
            ++learned;
          }
          else
          {
            LOG_ERROR("coa",
                      "Cannot teach missing proficiency spell {} to {}",
                      definition.SpellId, player->GetName());
          }
        }

        uint16 const maximum = definition.ScalesWithLevel
                                   ? player->GetMaxSkillValueForLevel()
                                   : 1;
        uint16 const step = player->HasSkill(definition.SkillId)
                                ? player->GetSkillStep(definition.SkillId)
                                : 0;
        player->SetSkill(definition.SkillId, step, maximum, maximum);
        continue;
      }

      if (player->HasSpell(definition.SpellId))
      {
        player->removeSpell(definition.SpellId, SPEC_MASK_ALL, false);
        ++removed;
      }
      if (player->HasSkill(definition.SkillId))
        player->SetSkill(definition.SkillId, 0, 0, 0);
    }

    for (uint16 skill : std::array<uint16, 2>{SKILL_DEFENSE, SKILL_UNARMED})
      if (player->HasSkill(skill))
      {
        uint16 const maximum = player->GetMaxSkillValueForLevel();
        player->SetSkill(skill, player->GetSkillStep(skill), maximum, maximum);
        if (skill == SKILL_DEFENSE)
          player->UpdateDefenseBonusesMod();
      }

    {
      std::lock_guard<std::mutex> lock(_stateLock);
      _proficiencySynchronizations.erase(guid);
    }
    if (learned || removed)
    {
      LOG_INFO("coa",
               "Synchronized proficiencies for {} (class {}, level {}): "
               "learned {}, removed {}",
               player->GetName(), uint32(player->getClass()),
               uint32(player->GetLevel()), learned, removed);
    }
  }

  bool InitializeLiveBaseline(Player* player)
  {
    if (!IsAscensionCustomClass(player) || player->IsInWorld())
      return false;

    for (uint32 spellId : GetAscensionRacialSpells(player))
    {
      if (!sSpellMgr->GetSpellInfo(spellId))
        return false;
      if (!player->HasSpell(spellId))
        player->addSpell(spellId, SPEC_MASK_ALL, true);
    }
    for (auto const& entry : AscensionLiveBaseline::Spells)
    {
      if (entry.ClassId != player->getClass() || (entry.RaceId && entry.RaceId != player->getRace()))
        continue;
      if (!CanGrantAscensionRacialSpell(player, entry.SpellId))
      {
        if (player->HasSpell(entry.SpellId))
          player->removeSpell(entry.SpellId, SPEC_MASK_ALL, false);
        continue;
      }
      if (!sSpellMgr->GetSpellInfo(entry.SpellId))
        return false;
      player->addSpell(entry.SpellId, SPEC_MASK_ALL, true);
    }
    for (auto const& entry : AscensionLiveBaseline::Proficiencies)
    {
      if (entry.ClassId != player->getClass())
        continue;
      if (!sSpellMgr->GetSpellInfo(entry.SpellId))
        return false;
      player->addSpell(entry.SpellId, SPEC_MASK_ALL, true);
    }
    for (auto const& entry : AscensionLiveBaseline::Skills)
    {
      if (entry.ClassId != player->getClass())
        continue;
      if (!sSkillLineStore.LookupEntry(entry.SkillId))
        return false;
      bool const weapon = std::any_of(AscensionCompatData::ProficiencyDefinitions.begin(),
          AscensionCompatData::ProficiencyDefinitions.end(), [&entry](auto const& definition)
          { return definition.SkillId == entry.SkillId && definition.ScalesWithLevel; });
      if (entry.SkillId == SKILL_UNARMED)
        continue;
      uint16 const maximum = weapon ? player->GetMaxSkillValueForLevel() : entry.Maximum;
      uint16 const value = weapon ? maximum : entry.Rank;
      player->SetSkill(entry.SkillId, 0, value, maximum);
    }
    return true;
  }

  bool InitializeLiveStarterKit(Player* player)
  {
    if (!IsAscensionCustomClass(player) || player->IsInWorld())
      return false;

    constexpr uint32 liveStarterRevision = 20260903;
    char const* const liveSetting = "core.ascension_starter_live";
    if (player->GetPlayerSetting(liveSetting, 0).value == liveStarterRevision)
      return true;

    for (uint8 slot = EQUIPMENT_SLOT_START; slot < INVENTORY_SLOT_ITEM_END; ++slot)
      if (player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
      {
        LOG_ERROR("coa", "Refused non-empty live starter initialization for class {}", uint32(player->getClass()));
        return false;
      }

    uint32 entries = 0;
    std::unordered_set<uint16> positions;
    for (AscensionCompatData::LiveStarterItem const& entry : AscensionCompatData::LiveStarterItems)
    {
      if (entry.ClassId != player->getClass())
        continue;

      bool const equipped = entry.Slot < EQUIPMENT_SLOT_END;
      uint16 const position = uint16(entry.Bag) << 8 | entry.Slot;
      ItemTemplate const* item = sObjectMgr->GetItemTemplate(entry.ItemId);
      if (entry.Bag != INVENTORY_SLOT_BAG_0 ||
          (!equipped && (entry.Slot < INVENTORY_SLOT_ITEM_START || entry.Slot >= INVENTORY_SLOT_ITEM_END)) ||
          !entry.Count || !item || entry.Count > item->GetMaxStackSize() ||
          (equipped && entry.Count != 1) || !positions.insert(position).second)
      {
        LOG_ERROR("coa", "Invalid live starter class {} item {} slot {} count {}", uint32(entry.ClassId), entry.ItemId, uint32(entry.Slot), entry.Count);
        return false;
      }

      if (equipped)
      {
        uint16 destination = 0;
        InventoryResult const result = player->CanEquipNewItem(entry.Slot, destination, entry.ItemId, false);
        if (result != EQUIP_ERR_OK || destination != position)
        {
          LOG_ERROR("coa", "Cannot equip live starter class {} item {} in slot {}: {}", uint32(entry.ClassId), entry.ItemId, uint32(entry.Slot), uint32(result));
          return false;
        }
      }
      else
      {
        ItemPosCountVec destinations;
        InventoryResult const result = player->CanStoreNewItem(entry.Bag, entry.Slot, destinations, entry.ItemId, entry.Count);
        if (result != EQUIP_ERR_OK || destinations.size() != 1 ||
            destinations.front().pos != position || destinations.front().count != entry.Count)
        {
          LOG_ERROR("coa", "Cannot store live starter class {} item {} in slot {}: {}", uint32(entry.ClassId), entry.ItemId, uint32(entry.Slot), uint32(result));
          return false;
        }
      }
      ++entries;
    }
    if (!entries)
      return false;

    for (AscensionCompatData::LiveStarterItem const& entry : AscensionCompatData::LiveStarterItems)
    {
      if (entry.ClassId != player->getClass())
        continue;

      uint16 const position = uint16(entry.Bag) << 8 | entry.Slot;
      Item* created = nullptr;
      if (entry.Slot < EQUIPMENT_SLOT_END)
      {
        uint16 destination = 0;
        if (player->CanEquipNewItem(entry.Slot, destination, entry.ItemId, false) == EQUIP_ERR_OK && destination == position)
          created = player->EquipNewItem(destination, entry.ItemId, false);
      }
      else
      {
        ItemPosCountVec destinations;
        if (player->CanStoreNewItem(entry.Bag, entry.Slot, destinations, entry.ItemId, entry.Count) == EQUIP_ERR_OK &&
            destinations.size() == 1 && destinations.front().pos == position && destinations.front().count == entry.Count)
          created = player->StoreNewItem(destinations, entry.ItemId, false);
      }
      if (!created || created->GetEntry() != entry.ItemId || created->GetCount() != entry.Count ||
          player->GetItemByPos(entry.Bag, entry.Slot) != created)
      {
        LOG_ERROR("coa", "Failed exact live starter placement for class {} item {} slot {}", uint32(entry.ClassId), entry.ItemId, uint32(entry.Slot));
        return false;
      }
    }

    player->UpdatePlayerSetting(liveSetting, 0, liveStarterRevision);
    player->UpdatePlayerSetting("core.ascension_starter", 0, 1);
    return true;
  }

  bool RepairStarterKit(Player *player, bool force) {
    if (!IsAscensionCustomClass(player))
      return false;

    AscensionCompatData::StarterKit const *kit =
        GetStarterKit(player->getClass());
    if (!kit)
      return false;

    constexpr uint32 starterRevision = 1;
    char const* const setting = "core.ascension_starter";
    if (!force && player->GetPlayerSetting(setting, 0).value >= starterRevision)
      return false;

    uint32 restored = 0;
    bool complete = true;
    for (uint8 index = 0; index < kit->ItemCount; ++index) {
      uint32 itemId = kit->Items[index];
      if (player->HasItemCount(itemId, 1, true))
        continue;

      ItemTemplate const* item = sObjectMgr->GetItemTemplate(itemId);
      if (!item)
      {
        complete = false;
        LOG_ERROR("coa", "Missing starter item template {}", itemId);
        continue;
      }

      uint8 slot = EQUIPMENT_SLOT_END;
      switch (item->InventoryType)
      {
        case INVTYPE_HEAD: slot = EQUIPMENT_SLOT_HEAD; break;
        case INVTYPE_SHOULDERS: slot = EQUIPMENT_SLOT_SHOULDERS; break;
        case INVTYPE_BODY: slot = EQUIPMENT_SLOT_BODY; break;
        case INVTYPE_CHEST:
        case INVTYPE_ROBE: slot = EQUIPMENT_SLOT_CHEST; break;
        case INVTYPE_WAIST: slot = EQUIPMENT_SLOT_WAIST; break;
        case INVTYPE_LEGS: slot = EQUIPMENT_SLOT_LEGS; break;
        case INVTYPE_FEET: slot = EQUIPMENT_SLOT_FEET; break;
        case INVTYPE_WRISTS: slot = EQUIPMENT_SLOT_WRISTS; break;
        case INVTYPE_HANDS: slot = EQUIPMENT_SLOT_HANDS; break;
        case INVTYPE_CLOAK: slot = EQUIPMENT_SLOT_BACK; break;
        case INVTYPE_WEAPON:
        case INVTYPE_2HWEAPON:
        case INVTYPE_WEAPONMAINHAND: slot = EQUIPMENT_SLOT_MAINHAND; break;
        case INVTYPE_SHIELD:
        case INVTYPE_WEAPONOFFHAND:
        case INVTYPE_HOLDABLE: slot = EQUIPMENT_SLOT_OFFHAND; break;
        case INVTYPE_RANGED:
        case INVTYPE_RANGEDRIGHT:
        case INVTYPE_THROWN:
        case INVTYPE_RELIC: slot = EQUIPMENT_SLOT_RANGED; break;
        default: break;
      }
      if (slot == EQUIPMENT_SLOT_END)
      {
        complete = false;
        LOG_ERROR("coa", "Unsupported starter item inventory type for {}", itemId);
        continue;
      }
      if (player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot) ||
          (slot == EQUIPMENT_SLOT_OFFHAND && player->IsTwoHandUsed()) ||
          (item->InventoryType == INVTYPE_2HWEAPON &&
           player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND)))
        continue;

      if (player->StoreNewItemInBestSlots(itemId, 1))
        ++restored;
      else
        complete = false;
    }

    if (!player->HasItemCount(6948, 1, true))
    {
      if (player->StoreNewItemInBestSlots(6948, 1))
        ++restored;
      else
        complete = false;
    }
    if (complete)
      player->UpdatePlayerSetting(setting, 0, starterRevision);

    if (restored)
    {
      ChatHandler(player->GetSession())
          .PSendSysMessage("Restored {} custom-class starter items.",
                           restored);
      LOG_INFO("coa",
               "Restored {} starter items for {} (class {})", restored,
               player->GetName(), uint32(player->getClass()));
    }

    return restored != 0;
  }

  void OnPlayerLogin(Player *player) {
    if (!IsAscensionCustomClass(player))
      return;

    uint32 const specializationId = player->GetPlayerSetting(ASCENSION_ACTIVE_SPEC_SETTING, 0).value;
    if (specializationId)
    {
        std::lock_guard<std::mutex> lock(_stateLock);
        _activeSpecializations[player->GetGUID().GetCounter()] = specializationId;
    }

    SynchronizeProgression(player);
    SynchronizeProficiencies(player);
    RepairStarterKit(player, false);
    QueueCharacterAdvancementState(player);
    SendCharacterAdvancementBridge(player);
    SendLocalTalentState(player);

    CharacterDatabasePreparedStatement* actionsStmt =
        CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_ACTIONS_SPEC);
    actionsStmt->SetData(0, player->GetGUID().GetRawValue());
    actionsStmt->SetData(1, player->GetActiveSpec());

    WorldSession* session = player->GetSession();
    session->GetQueryProcessor().AddCallback(CharacterDatabase.AsyncQuery(actionsStmt)
        .WithPreparedCallback([session](PreparedQueryResult result)
        {
            if (Player* owner = session->GetPlayer())
                owner->LoadActions(result);
        }));
  }

  void PrepareTaughtAbilitiesBeforeMap(Player* player)
  {
    if (!IsAscensionCustomClass(player) || player->IsInWorld() ||
        !player->GetSession()->PlayerLoading())
      return;

    uint32 const specializationId = player->GetPlayerSetting(ASCENSION_ACTIVE_SPEC_SETTING, 0).value;
    if (specializationId)
    {
        std::lock_guard<std::mutex> lock(_stateLock);
        _activeSpecializations[player->GetGUID().GetCounter()] = specializationId;
    }

    if (uint32 const learned = SynchronizeTaughtAbilities(player, true))
    {
        player->SendInitialSpells();
        LOG_INFO("coa",
                 "Prepared {} taught abilities for {} before entering the world",
                 learned, player->GetName());
    }
  }

  static AscensionCompatData::CoATalentEntry const* FindTalentEntry(uint32 entryId)
  {
    auto const& entries = AscensionCompatData::CoATalentEntries;
    auto itr = std::lower_bound(entries.begin(), entries.end(), entryId,
        [](AscensionCompatData::CoATalentEntry const& entry, uint32 id) { return entry.EntryId < id; });
    return itr != entries.end() && itr->EntryId == entryId ? &*itr : nullptr;
  }

  static AscensionCoATalentState::HasSpell SpellbookOf(Player const* player)
  {
    return [player](uint32 spellId) { return player->HasSpell(spellId); };
  }

  static std::vector<AscensionCoATalentState::KnownEntry> KnownTalentEntries(Player const* player)
  {
    return AscensionCoATalentState::KnownEntries(player->getClass(), SpellbookOf(player));
  }

  void QueueCharacterAdvancementState(Player* player)
  {
    std::lock_guard<std::mutex> lock(_stateLock);
    _advancementPending.insert(player->GetGUID().GetCounter());
    _advancementSent.erase(player->GetGUID().GetCounter());
  }

  void OnPlayerActiveMover(Player* player)
  {
    {
      std::lock_guard<std::mutex> lock(_stateLock);
      if (!_advancementPending.erase(player->GetGUID().GetCounter()))
        return;
      _advancementSent.insert(player->GetGUID().GetCounter());
    }
    SendCharacterAdvancementState(player);
  }

  void SendCharacterAdvancementState(Player* player)
  {
    WorldPacket packet(SMSG_CHARACTER_ADVANCEMENT_ACTIVE_SPEC, sizeof(uint32) * 2);
    packet << uint32(0) << uint32(1);
    player->GetSession()->SendPacket(&packet);

    uint32 const sent = SendKnownTalentEntries(player);
    LOG_INFO("coa",
             "Initialized Character Advancement for {} (class {}, level {}) with {} known entries",
             player->GetName(), uint32(player->getClass()), uint32(player->GetLevel()), sent);
  }

  void SendCharacterAdvancementKnownEntries(Player* player)
  {
    SendCharacterAdvancementBridge(player);
    SendLocalTalentState(player);
    {
      std::lock_guard<std::mutex> lock(_stateLock);
      if (!_advancementSent.count(player->GetGUID().GetCounter()))
        return;
    }
    SendKnownTalentEntries(player);
  }

  void SendCharacterAdvancementBridge(Player* player)
  {
    if (!player->GetSession())
      return;

    uint32 const specializationId = GetActiveSpecialization(player);
    std::vector<std::string> ranks;
    for (AscensionCoATalentState::KnownEntry const& known : KnownTalentEntries(player))
      ranks.push_back(std::to_string(known.EntryId) + "," + std::to_string(known.Rank));

    constexpr std::size_t maxPayload = 180;
    std::vector<std::string> chunks;
    std::string current;
    for (std::string const& rank : ranks)
    {
      if (!current.empty() && current.size() + rank.size() + 1 > maxPayload)
      {
        chunks.push_back(current);
        current.clear();
      }
      if (!current.empty())
        current += ';';
      current += rank;
    }
    if (!current.empty() || chunks.empty())
      chunks.push_back(current);

    for (std::size_t index = 0; index < chunks.size(); ++index)
    {
      std::string const message = "ASC_LOCAL_CAD\t1:" + std::to_string(specializationId) + ":" +
          std::to_string(index + 1) + ":" + std::to_string(chunks.size()) + ":" + chunks[index];
      WorldPacket packet;
      ChatHandler::BuildChatPacket(packet, CHAT_MSG_WHISPER, LANG_ADDON, player->GetGUID(), player->GetGUID(),
          message, 0, player->GetName(), player->GetName(), 0, false);
      player->GetSession()->SendPacket(&packet);
    }

    LOG_DEBUG("coa",
              "Sent Character Advancement bridge to {}: specialization {}, {} entries in {} message(s)",
              player->GetName(), specializationId, uint32(ranks.size()), uint32(chunks.size()));
  }

  void SendLocalTalentState(Player* player)
  {
    if (!player->GetSession())
      return;

    auto send = [player](char const* prefix, std::string const& body)
    {
      std::string message = prefix;
      message += '\t';
      message += body;
      WorldPacket packet;
      ChatHandler::BuildChatPacket(packet, CHAT_MSG_WHISPER, LANG_ADDON, player->GetGUID(), player->GetGUID(),
          message, 0, player->GetName(), player->GetName(), 0, false);
      player->GetSession()->SendPacket(&packet);
    };

    std::string ranks;
    for (AscensionCoATalentState::KnownEntry const& known : KnownTalentEntries(player))
    {
      AscensionCompatData::CoATalentEntry const* entry = FindTalentEntry(known.EntryId);
      if (!entry || (!entry->AECost && !entry->TECost && !GetSelectableFreeGroup(entry->EntryId)))
        continue;
      if (!ranks.empty())
        ranks += ' ';
      ranks += std::to_string(known.EntryId) + ":" + std::to_string(known.Rank);
    }

    send("ASC_LOCAL_SPEC", std::to_string(GetActiveSpecialization(player)));
    send("ASC_LOCAL_RECORDS", "1 1");
    send("ASC_LOCAL_TALENTS", ranks);
  }

  uint32 SendKnownTalentEntries(Player* player)
  {
    std::vector<AscensionCoATalentState::KnownEntry> const known = KnownTalentEntries(player);
    std::vector<uint8> const body = AscensionCoATalentState::KnownEntriesPayload(known);
    WorldPacket packet(SMSG_CHARACTER_ADVANCEMENT_KNOWN_ENTRIES, body.size());
    packet.append(body.data(), body.size());
    player->GetSession()->SendPacket(&packet);
    return uint32(known.size());
  }

  bool SetTalentRank(Player* player, AscensionCompatData::CoATalentEntry const& entry, uint32 rank,
                     std::string& error, bool checkBudget = true)
  {
    uint32 const entryId = entry.EntryId;
    if (entry.ClassId != player->getClass())
    {
      error = Acore::StringFormat("Talent entry {} does not belong to your custom class.", entryId);
      return false;
    }

    uint32 activeSpecialization = GetActiveSpecialization(player);
    if (rank > 0 && entry.SpecId != 0 && !activeSpecialization)
    {
      SwitchSpecialization(player, entry.SpecId);
      activeSpecialization = GetActiveSpecialization(player);
    }

    if (rank > 0 && entry.SpecId != 0 && entry.SpecId != activeSpecialization)
    {
      error = Acore::StringFormat(
          "Talent entry {} belongs to specialization {}, but your active local specialization is {}.",
          entryId, uint32(entry.SpecId), activeSpecialization);
      return false;
    }

    if (rank > entry.SpellCount)
    {
      error = Acore::StringFormat("Talent entry {} only has {} rank(s).", entryId, uint32(entry.SpellCount));
      return false;
    }

    uint32 const freeChoiceGroup = GetSelectableFreeGroup(entryId);
    bool const automaticallyGranted = entry.AECost == 0 && entry.TECost == 0 && !freeChoiceGroup;
    if (automaticallyGranted && rank != 0 && rank != entry.SpellCount)
    {
      error = Acore::StringFormat("Progression entry {} must use its full automatic rank.", entryId);
      return false;
    }

    if (rank > 0 && player->GetLevel() < entry.RequiredLevel)
    {
      error = Acore::StringFormat("Talent entry {} requires level {}.", entryId, uint32(entry.RequiredLevel));
      return false;
    }

    if (automaticallyGranted)
    {
      bool const grantable = rank == 0 || CanGrantAutomaticEntry(player, entry, activeSpecialization);
      SynchronizeProgression(player);
      if (!grantable)
      {
        error = Acore::StringFormat("Progression entry {} requires its prerequisite ability.", entryId);
        return false;
      }
      return true;
    }

    uint32 const selectedSpellId = rank > 0 ? entry.SpellIds[rank - 1] : 0;
    if (rank > 0 && (!selectedSpellId || !sSpellMgr->GetSpellInfo(selectedSpellId)))
    {
      error = Acore::StringFormat("Talent entry {} rank {} references a missing server spell.", entryId, rank);
      return false;
    }

    uint32 const currentRank = AscensionCoATalentState::KnownRank(entry, SpellbookOf(player));
    if (checkBudget && rank > currentRank && (entry.AECost || entry.TECost))
    {
      uint32 classBudget = 0;
      uint32 specializationBudget = 0;
      if (!TalentBudget(player, classBudget, specializationBudget, error))
        return false;

      bool const classTree = entry.SpecId == 0;
      AscensionCoATalentState::SpentPoints const spent =
          AscensionCoATalentState::Spent(KnownTalentEntries(player));
      uint32 const used = classTree ? spent.AE : spent.TE;
      uint32 const budget = classTree ? classBudget : specializationBudget;
      uint32 const cost = (rank - currentRank) * uint32(classTree ? entry.AECost : entry.TECost);
      if (used + cost > budget)
      {
        error = Acore::StringFormat(
            "Talent entry {} rank {} needs {} {} point(s), but {} of the {} available at level {} are spent.",
            entryId, rank, cost, classTree ? "class" : "specialization", used, budget,
            uint32(player->GetLevel()));
        return false;
      }
    }

    if (rank > 0 && freeChoiceGroup)
    {
      for (auto const& other : AscensionCompatData::CoATalentEntries)
        if (other.ClassId == player->getClass() && other.SpecId == entry.SpecId && other.EntryId != entryId &&
            GetSelectableFreeGroup(other.EntryId) == freeChoiceGroup)
          for (uint32 spellId : other.SpellIds)
            if (spellId && player->HasSpell(spellId))
              player->removeSpell(spellId, SPEC_MASK_ALL, false);
    }

    for (uint32 spellId : entry.SpellIds)
      if (spellId && player->HasSpell(spellId))
        player->removeSpell(spellId, SPEC_MASK_ALL, false);

    if (rank > 0)
      player->learnSpell(selectedSpellId, false);

    SynchronizeProgression(player);

    LOG_INFO("coa", "Set local CoA talent entry {} to rank {} for {} (class {})", entryId, rank,
             player->GetName(), uint32(player->getClass()));
    return true;
  }

  uint32 ResetPaidTalents(Player* player)
  {
    uint32 removed = 0;
    for (AscensionCompatData::CoATalentEntry const& entry : AscensionCompatData::CoATalentEntries)
    {
      if (entry.ClassId != player->getClass() || (!entry.AECost && !entry.TECost))
        continue;
      for (uint32 spellId : entry.SpellIds)
        if (spellId && player->HasSpell(spellId))
        {
          player->removeSpell(spellId, SPEC_MASK_ALL, false);
          ++removed;
        }
    }

    SynchronizeProgression(player);
    LOG_INFO("coa", "Reset {} paid CoA talent rank(s) for {} (class {})", removed,
             player->GetName(), uint32(player->getClass()));
    return removed;
  }

  bool TalentBudget(Player const* player, uint32& classBudget, uint32& specializationBudget, std::string& error)
  {
    if (AscensionCompatData::GetCoATalentBudget(player->getClass(), player->GetLevel(), classBudget,
                                                specializationBudget))
      return true;

    LOG_ERROR("coa", "No CoA talent budget row for class {} at level {} ({})",
              uint32(player->getClass()), uint32(player->GetLevel()), player->GetName());
    error = Acore::StringFormat("No talent budget is known for class {} at level {}; no rank can be raised.",
                                uint32(player->getClass()), uint32(player->GetLevel()));
    return false;
  }

  static uint32 PersistentRank(Player const* player, AscensionCompatData::CoATalentEntry const& entry)
  {
    uint32 rank = 0;
    for (uint32 index = 0; index < entry.SpellCount; ++index)
    {
      uint32 const spellId = entry.SpellIds[index];
      bool const granted = spellId && std::any_of(AscensionCompatData::ClassSpells.begin(),
          AscensionCompatData::ClassSpells.end(), [player, spellId](AscensionCompatData::ClassSpell const& spell)
          {
            return spell.ClassId == player->getClass() && spell.SpellId == spellId &&
                   spell.RequiredLevel <= player->GetLevel();
          });
      if (granted)
        rank = index + 1;
    }
    return rank;
  }

  bool ApplyKnownEntriesUpload(Player* player, std::vector<AscensionCoATalentState::KnownEntry> const& upload,
                               std::string& error)
  {
    uint32 activeSpecialization = GetActiveSpecialization(player);
    uint32 uploadedSpecialization = 0;
    std::unordered_map<uint32, uint32> wanted;
    for (AscensionCoATalentState::KnownEntry const& item : upload)
    {
      AscensionCompatData::CoATalentEntry const* entry = FindTalentEntry(item.EntryId);
      if (!entry || entry->ClassId != player->getClass())
      {
        error = Acore::StringFormat("Talent entry {} does not belong to your custom class.", item.EntryId);
        return false;
      }
      if (!entry->AECost && !entry->TECost && !GetSelectableFreeGroup(entry->EntryId))
        continue;
      if (item.Rank > entry->SpellCount)
      {
        error = Acore::StringFormat("Talent entry {} only has {} rank(s).", entry->EntryId,
                                    uint32(entry->SpellCount));
        return false;
      }
      if (item.Rank > 0)
      {
        if (player->GetLevel() < entry->RequiredLevel)
        {
          error = Acore::StringFormat("Talent entry {} requires level {}.", entry->EntryId,
                                      uint32(entry->RequiredLevel));
          return false;
        }
        if (!sSpellMgr->GetSpellInfo(entry->SpellIds[item.Rank - 1]))
        {
          error = Acore::StringFormat("Talent entry {} rank {} references a missing server spell.",
                                      entry->EntryId, item.Rank);
          return false;
        }
        if (entry->SpecId && activeSpecialization && entry->SpecId != activeSpecialization)
        {
          error = Acore::StringFormat(
              "Talent entry {} belongs to specialization {}, but your active local specialization is {}.",
              entry->EntryId, uint32(entry->SpecId), activeSpecialization);
          return false;
        }
        if (entry->SpecId && !activeSpecialization)
        {
          if (uploadedSpecialization && uploadedSpecialization != entry->SpecId)
          {
            error = Acore::StringFormat("The uploaded build mixes specializations {} and {}.",
                                        uploadedSpecialization, uint32(entry->SpecId));
            return false;
          }
          uploadedSpecialization = entry->SpecId;
        }
      }
      wanted[entry->EntryId] = item.Rank;
    }

    std::vector<AscensionCoATalentState::KnownEntry> priced;
    for (AscensionCompatData::CoATalentEntry const& entry : AscensionCompatData::CoATalentEntries)
    {
      if (entry.ClassId != player->getClass() || (!entry.AECost && !entry.TECost))
        continue;
      auto itr = wanted.find(entry.EntryId);
      uint32 const rank = std::max(itr == wanted.end() ? 0 : itr->second, PersistentRank(player, entry));
      if (rank)
        priced.push_back({ entry.EntryId, rank });
    }

    uint32 classBudget = 0;
    uint32 specializationBudget = 0;
    if (!TalentBudget(player, classBudget, specializationBudget, error))
      return false;
    AscensionCoATalentState::SpentPoints const spent = AscensionCoATalentState::Spent(priced);
    if (spent.AE > classBudget || spent.TE > specializationBudget)
    {
      error = Acore::StringFormat(
          "That build spends {} class and {} specialization point(s); level {} has {} and {}.", spent.AE,
          spent.TE, uint32(player->GetLevel()), classBudget, specializationBudget);
      return false;
    }

    if (uploadedSpecialization && !SwitchSpecialization(player, uploadedSpecialization))
    {
      error = Acore::StringFormat("Specialization {} is not valid for your custom class.",
                                  uploadedSpecialization);
      return false;
    }

    std::vector<std::pair<AscensionCompatData::CoATalentEntry const*, uint32>> changes;
    for (AscensionCompatData::CoATalentEntry const& entry : AscensionCompatData::CoATalentEntries)
    {
      if (entry.ClassId != player->getClass() ||
          (!entry.AECost && !entry.TECost && !GetSelectableFreeGroup(entry.EntryId)))
        continue;
      auto itr = wanted.find(entry.EntryId);
      uint32 const rank = itr == wanted.end() ? 0 : itr->second;
      if (rank != AscensionCoATalentState::KnownRank(entry, SpellbookOf(player)))
        changes.emplace_back(&entry, rank);
    }
    std::stable_sort(changes.begin(), changes.end(),
                     [](auto const& left, auto const& right) { return (left.second == 0) > (right.second == 0); });
    for (auto const& [entry, rank] : changes)
      if (!SetTalentRank(player, *entry, rank, error, false))
      {
        LOG_ERROR("coa",
                  "Known-entries upload for {} failed after validation at entry {} rank {}: {}", player->GetName(),
                  entry->EntryId, rank, error);
        return false;
      }
    return true;
  }

  void QueueKnownEntriesUpload(uint32 accountId, WorldPacket const& packet)
  {
    std::lock_guard<std::mutex> lock(_stateLock);
    std::deque<std::vector<uint8>>& queue = _pendingUploads[accountId];
    if (queue.size() >= MAX_QUEUED_KNOWN_ENTRIES_UPLOADS)
    {
      LOG_WARN("coa", "Dropping known-entries upload for account {}: its queue is full",
               accountId);
      return;
    }

    std::vector<uint8> body;
    if (packet.size())
      body.assign(packet.contents(), packet.contents() + packet.size());
    queue.push_back(std::move(body));
  }

  void ProcessKnownEntriesUploads(Player* player)
  {
    std::deque<std::vector<uint8>> uploads;
    {
      std::lock_guard<std::mutex> lock(_stateLock);
      auto itr = _pendingUploads.find(player->GetSession()->GetAccountId());
      if (itr == _pendingUploads.end())
        return;
      uploads = std::move(itr->second);
      _pendingUploads.erase(itr);
    }

    if (!IsAscensionCustomClass(player))
      return;
    for (std::vector<uint8> const& body : uploads)
      HandleKnownEntriesUpload(player, body);
  }

  void HandleKnownEntriesUpload(Player* player, std::vector<uint8> const& body)
  {
    std::vector<AscensionCoATalentState::KnownEntry> upload;
    if (!AscensionCoATalentState::ParseKnownEntriesUpload(body.data(), body.size(), upload))
    {
      LOG_WARN("coa", "Malformed Ascension known-entries upload from {} payload={} bytes",
               player->GetName(), body.size());
    }
    else
    {
      std::string error;
      if (!ApplyKnownEntriesUpload(player, upload, error))
      {
        ChatHandler(player->GetSession()).SendSysMessage(error);
        LOG_INFO("coa", "Refused known-entries upload of {} record(s) from {}: {}",
                 upload.size(), player->GetName(), error);
      }
    }
    SendCharacterAdvancementKnownEntries(player);
  }

  uint32 GetActiveSpecialization(Player const *player) const {
    std::lock_guard<std::mutex> lock(_stateLock);
    auto itr = _activeSpecializations.find(player->GetGUID().GetCounter());
    return itr == _activeSpecializations.end() ? 0 : itr->second;
  }

  static std::string BuildSetting(uint32 specializationId)
  {
    return std::string(ASCENSION_TALENT_BUILD_SETTING_PREFIX) + std::to_string(specializationId);
  }

  static std::vector<uint32> LivePicks(Player const* player, uint32 specializationId)
  {
    std::vector<uint32> picks;
    for (AscensionCompatData::CoATalentEntry const& entry : AscensionCompatData::CoATalentEntries)
    {
      if (entry.ClassId != player->getClass() || entry.SpecId != specializationId ||
          (!entry.AECost && !entry.TECost && !GetSelectableFreeGroup(entry.EntryId)))
        continue;
      if (uint32 const rank = AscensionCoATalentState::KnownRank(entry, SpellbookOf(player)))
        picks.push_back(entry.EntryId * 10 + rank);
    }
    return picks;
  }

  static void StoreBuild(Player* player, uint32 specializationId, std::vector<uint32> const& picks)
  {
    std::string const setting = BuildSetting(specializationId);
    std::size_t previous = 0;
    if (PlayerSettingVector const* values = player->FindPlayerSettings(setting))
      previous = values->size();

    player->UpdatePlayerSetting(setting, 0, uint32(picks.size()));
    for (std::size_t index = 0; index < picks.size(); ++index)
      player->UpdatePlayerSetting(setting, uint32(index) + 1, picks[index]);
    for (std::size_t index = picks.size() + 1; index < previous; ++index)
      player->UpdatePlayerSetting(setting, uint32(index), 0);
  }

  static std::vector<uint32> StoredBuild(Player const* player, uint32 specializationId)
  {
    std::vector<uint32> picks;
    PlayerSettingVector const* values = player->FindPlayerSettings(BuildSetting(specializationId));
    if (!values || values->empty())
      return picks;

    std::size_t const count = std::min<std::size_t>((*values)[0].value, values->size() - 1);
    for (std::size_t index = 1; index <= count; ++index)
      if (uint32 const pick = (*values)[index].value)
        picks.push_back(pick);
    return picks;
  }

  static std::string BarSetting(uint32 specializationId)
  {
    return "core.ascension_bar." + std::to_string(specializationId);
  }

  static std::vector<std::pair<uint8, uint32>> StoredBar(Player const* player, uint32 specializationId)
  {
    std::vector<std::pair<uint8, uint32>> bar;
    PlayerSettingVector const* values = player->FindPlayerSettings(BarSetting(specializationId));
    if (!values || values->empty())
      return bar;

    std::size_t const count = std::min<std::size_t>((*values)[0].value, (values->size() - 1) / 2);
    for (std::size_t index = 0; index < count; ++index)
      if (uint32 const button = (*values)[2 * index + 1].value; button < MAX_ACTION_BUTTONS)
        if (uint32 const spell = (*values)[2 * index + 2].value)
          bar.emplace_back(uint8(button), spell);
    return bar;
  }

  static void StoreBar(Player* player, uint32 specializationId,
                       std::vector<std::pair<uint8, uint32>> const& bar)
  {
    std::string const setting = BarSetting(specializationId);
    std::size_t previous = 0;
    if (PlayerSettingVector const* values = player->FindPlayerSettings(setting))
      previous = values->size();

    player->UpdatePlayerSetting(setting, 0, uint32(bar.size()));
    for (std::size_t index = 0; index < bar.size(); ++index)
    {
      player->UpdatePlayerSetting(setting, uint32(2 * index + 1), bar[index].first);
      player->UpdatePlayerSetting(setting, uint32(2 * index + 2), bar[index].second);
    }
    for (std::size_t index = 2 * bar.size() + 1; index < previous; ++index)
      player->UpdatePlayerSetting(setting, uint32(index), 0);
  }

  static void RememberBarButtons(Player* player, uint32 specializationId,
                                 std::unordered_set<uint32> const& spells)
  {
    std::vector<std::pair<uint8, uint32>> bar;
    for (uint8 button = 0; button < MAX_ACTION_BUTTONS; ++button)
    {
      ActionButton const* action = player->GetActionButton(button);
      if (!action || action->GetType() != ACTION_BUTTON_SPELL || !spells.contains(action->GetAction()))
        continue;

      bar.emplace_back(button, action->GetAction());
      player->removeActionButton(button);
    }
    StoreBar(player, specializationId, bar);
  }

  static void RestoreBarButtons(Player* player, uint32 specializationId, uint32 previousSpecialization)
  {
    uint32 const source = player->FindPlayerSettings(BarSetting(specializationId))
        ? specializationId : previousSpecialization;
    for (auto const& [button, spell] : StoredBar(player, source))
      if (player->HasSpell(spell) && !player->GetActionButton(button))
        player->addActionButton(button, spell, ACTION_BUTTON_SPELL);

    player->SendInitialActionButtons();
  }

  void StoreBuilds(Player* player, uint32 specializationId)
  {
    StoreBuild(player, 0, LivePicks(player, 0));
    StoreBuild(player, specializationId, LivePicks(player, specializationId));
  }

  uint32 RestoreBuilds(Player* player, uint32 specializationId)
  {
    uint32 restored = 0;
    for (uint32 const tree : { uint32(0), specializationId })
      for (uint32 const pick : StoredBuild(player, tree))
      {
        AscensionCompatData::CoATalentEntry const* entry = FindTalentEntry(pick / 10);
        uint32 const rank = pick % 10;
        if (!entry || entry->ClassId != player->getClass() || entry->SpecId != tree || !rank ||
            rank > entry->SpellCount)
          continue;
        if (AscensionCoATalentState::KnownRank(*entry, SpellbookOf(player)) >= rank)
          continue;

        std::string error;
        if (SetTalentRank(player, *entry, rank, error))
          ++restored;
        else
          LOG_INFO("coa", "Stored talent entry {} rank {} not restored for {}: {}",
                   entry->EntryId, rank, player->GetName(), error);
      }
    return restored;
  }

  bool SwitchSpecialization(Player *player, uint32 specializationId) {
    if (!IsAscensionCustomClass(player) || !specializationId)
      return false;

    bool validSpecialization = std::any_of(
        AscensionCompatData::CoATalentEntries.begin(),
        AscensionCompatData::CoATalentEntries.end(),
        [player, specializationId](
            AscensionCompatData::CoATalentEntry const &entry) {
          return entry.ClassId == player->getClass() &&
                 entry.SpecId == specializationId;
        });
    if (!validSpecialization)
      return false;

    uint32 const previousSpecialization = GetActiveSpecialization(player);

    if (!previousSpecialization || previousSpecialization == specializationId)
    {
      {
        std::lock_guard<std::mutex> lock(_stateLock);
        _activeSpecializations[player->GetGUID().GetCounter()] = specializationId;
      }
      player->UpdatePlayerSetting(ASCENSION_ACTIVE_SPEC_SETTING, 0, specializationId);

      uint32 const restored = previousSpecialization ? 0 : RestoreBuilds(player, specializationId);
      uint32 granted = SynchronizeProgression(player);
      LOG_INFO("coa",
               "Synchronized {} (class {}) with local specialization {}, restored {} stored rank(s) and "
               "granted {} missing automatic spells",
               player->GetName(), uint32(player->getClass()), specializationId, restored, granted);
      return true;
    }

    StoreBuilds(player, previousSpecialization);

    if (Pet* pet = player->GetPet())
      player->RemovePet(pet, PET_SAVE_NOT_IN_SLOT);

    std::unordered_set<uint32> visitedSpellIds;
    uint32 removed = 0;
    {
      std::unordered_set<uint32> talentSpells;
      for (AscensionCompatData::CoATalentEntry const& entry : AscensionCompatData::CoATalentEntries)
        if (entry.ClassId == player->getClass())
          for (uint32 spellId : entry.SpellIds)
            if (spellId && player->HasSpell(spellId))
              talentSpells.insert(spellId);
      player->SendActionButtons(2);
      RememberBarButtons(player, previousSpecialization, talentSpells);
    }
    for (AscensionCompatData::CoATalentEntry const &entry :
         AscensionCompatData::CoATalentEntries) {
      if (entry.ClassId != player->getClass())
        continue;

      for (uint32 spellId : entry.SpellIds) {
        if (!spellId || !visitedSpellIds.insert(spellId).second ||
            !player->HasSpell(spellId))
          continue;

        player->removeSpell(spellId, SPEC_MASK_ALL, false);
        ++removed;
      }
    }

    if (player->getClass() == CLASS_SON_OF_ARUGAL && previousSpecialization == BLOODMAGE_FLESHWEAVER_SPEC)
      player->RemoveAurasDueToSpell(AscensionBloodmage::PooledVitality);

    {
      std::lock_guard<std::mutex> lock(_stateLock);
      _activeSpecializations[player->GetGUID().GetCounter()] = specializationId;
    }
    player->UpdatePlayerSetting(ASCENSION_ACTIVE_SPEC_SETTING, 0, specializationId);

    uint32 const restored = RestoreBuilds(player, specializationId);
    uint32 granted = SynchronizeProgression(player);
    RestoreBarButtons(player, specializationId, previousSpecialization);
    ChatHandler(player->GetSession())
        .PSendSysMessage(
            "Activated specialization {}. Stored the build of specialization {}, removed {} old talent "
            "spell(s), restored {} stored rank(s) and granted {} automatic ability/passive spell(s).",
            specializationId, previousSpecialization, removed, restored, granted);
    LOG_INFO("coa",
             "Switched {} (class {}) to local specialization {}: removed {} CoA spells, restored {} "
             "stored ranks and granted {} automatic spells",
             player->GetName(), uint32(player->getClass()), specializationId, removed, restored, granted);
    return true;
  }

    void UpdateClassTuning(Player* player, uint32 diff)
    {
        if (!IsAscensionCustomClass(player))
            return;

        {
            std::lock_guard<std::mutex> lock(_stateLock);
            uint32& remaining = _tuningUpdates[player->GetGUID()];
            if (diff < remaining)
            {
                remaining -= diff;
                return;
            }

            remaining = 1000;
        }
        AscensionClassTuning::Synchronize(player, GetActiveSpecialization(player), false);
    }

  void OnPlayerLogout(Player *player) {
    std::lock_guard<std::mutex> lock(_stateLock);
    _tuningUpdates.erase(player->GetGUID());
    _activeSpecializations.erase(player->GetGUID().GetCounter());
    _proficiencySynchronizations.erase(player->GetGUID().GetCounter());
    _advancementPending.erase(player->GetGUID().GetCounter());
    _advancementSent.erase(player->GetGUID().GetCounter());
    _pendingUploads.erase(player->GetSession()->GetAccountId());
  }

    static uint32 GetSelectableFreeGroup(uint32 entryId)
    {
        for (auto const& entry : AscensionCompatData::CoASelectableFreeEntries)
            if (entry.EntryId == entryId)
                return entry.GroupId;
        return 0;
    }

    static bool CanGrantAutomaticEntry(Player const* player,
        AscensionCompatData::CoATalentEntry const& entry, uint32 specializationId)
    {
        if (entry.ClassId != player->getClass() ||
            (entry.SpecId != 0 && entry.SpecId != specializationId) ||
            entry.AECost != 0 || entry.TECost != 0 ||
            entry.RequiredLevel > player->GetLevel() || !entry.SpellCount || GetSelectableFreeGroup(entry.EntryId))
            return false;

        auto const& dependencies = AscensionCompatData::CoAAutomaticDependencies;
        auto dependency = std::lower_bound(dependencies.begin(), dependencies.end(), entry.EntryId,
            [](AscensionCompatData::CoAAutomaticDependency const& value, uint32 id)
            {
                return value.EntryId < id;
            });
        if (dependency == dependencies.end() || dependency->EntryId != entry.EntryId)
            return true;

        for (uint32 requiredId : dependency->RequiredEntryIds)
        {
            if (!requiredId)
                continue;

            auto const& entries = AscensionCompatData::CoATalentEntries;
            auto required = std::lower_bound(entries.begin(), entries.end(), requiredId,
                [](AscensionCompatData::CoATalentEntry const& value, uint32 id)
                {
                    return value.EntryId < id;
                });
            if (required == entries.end() || required->EntryId != requiredId ||
                required->ClassId != player->getClass() ||
                !std::any_of(required->SpellIds.begin(), required->SpellIds.end(),
                    [player](uint32 spellId) { return spellId && player->HasSpell(spellId); }))
                return false;
        }
        return true;
    }

    static void ReconcileRunemasterFists(Player* player, uint32 specializationId)
    {
        if (!player || player->getClass() != CLASS_SPIRIT_MAGE || !specializationId)
            return;

        auto const& entries = AscensionCompatData::CoATalentEntries;
        auto findEntry = [&entries](uint32 entryId)
        {
            return std::lower_bound(entries.begin(), entries.end(), entryId,
                [](AscensionCompatData::CoATalentEntry const& entry, uint32 id)
                {
                    return entry.EntryId < id;
                });
        };
        auto const fists = findEntry(4062);
        auto const zenith = findEntry(29521);
        if (fists == entries.end() || fists->EntryId != 4062 ||
            fists->ClassId != CLASS_SPIRIT_MAGE || fists->SpecId != 61 ||
            fists->SpellCount != 1 || fists->AECost || fists->TECost || fists->RequiredLevel != 10 ||
            fists->SpellIds != std::array<uint32, 3>{92153, 0, 0} ||
            zenith == entries.end() || zenith->EntryId != 29521 ||
            zenith->ClassId != CLASS_SPIRIT_MAGE || zenith->SpecId ||
            zenith->SpellCount != 1 || zenith->AECost != 1 || zenith->TECost || zenith->RequiredLevel ||
            zenith->SpellIds != std::array<uint32, 3>{712325, 0, 0})
            return;

        auto const& dependencies = AscensionCompatData::CoAAutomaticDependencies;
        auto const dependency = std::lower_bound(dependencies.begin(), dependencies.end(), uint32(4062),
            [](AscensionCompatData::CoAAutomaticDependency const& entry, uint32 id)
            {
                return entry.EntryId < id;
            });
        if (dependency == dependencies.end() || dependency->EntryId != 4062 ||
            dependency->RequiredEntryIds != std::array<uint32, 2>{29521, 0})
            return;

        if (!CanGrantAutomaticEntry(player, *fists, specializationId) && player->HasSpell(92153))
            player->removeSpell(92153, player->GetActiveSpecMask(), false);
    }

private:
    static uint32 SynchronizeAutomaticTalents(Player* player, uint32 specializationId)
    {
        if (player->GetLevel() == 1)
            return 0;
        uint32 learned = 0;
        bool changed = true;
        for (std::size_t pass = 0; changed && pass < AscensionCompatData::CoATalentEntries.size(); ++pass)
        {
            changed = false;
            for (auto const& entry : AscensionCompatData::CoATalentEntries)
            {
                if (!CanGrantAutomaticEntry(player, entry, specializationId))
                    continue;

                uint32 spellId = entry.SpellIds[entry.SpellCount - 1];
                if (!spellId || player->HasSpell(spellId) || !sSpellMgr->GetSpellInfo(spellId))
                    continue;

                LearnRestoredSpell(player, spellId);
                ++learned;
                changed = true;
            }
        }
        return learned;
    }

  mutable std::mutex _stateLock;
  std::unordered_map<ObjectGuid, uint32> _tuningUpdates;
  std::unordered_map<uint32, uint32> _activeSpecializations;
  std::unordered_set<uint32> _proficiencySynchronizations;
  std::unordered_set<uint32> _advancementPending;
  std::unordered_set<uint32> _advancementSent;
  static constexpr std::size_t MAX_QUEUED_KNOWN_ENTRIES_UPLOADS = 8;
  std::unordered_map<uint32, std::deque<std::vector<uint8>>> _pendingUploads;
};

class AscensionResourceService
{
public:
    static AscensionResourceService& Instance()
    {
        static AscensionResourceService instance;
        return instance;
    }

    void ValidateDefinitions() const
    {
        std::unordered_set<uint32> checkedResourceSpells;
        uint32 missingResourceSpells = 0;
        uint32 missingAbilitySpells = 0;

        auto validateResourceSpell =
            [&checkedResourceSpells, &missingResourceSpells](uint32 spellId)
            {
                if (!spellId || !checkedResourceSpells.insert(spellId).second)
                    return;

                if (!sSpellMgr->GetSpellInfo(spellId))
                {
                    LOG_ERROR("coa",
                        "Ascension resource spell {} is missing from the server DBC",
                        spellId);
                    ++missingResourceSpells;
                }
            };

        for (AscensionCompatData::ResourceDisplay const& display :
             AscensionCompatData::ResourceDisplays)
            validateResourceSpell(display.SpellId);

        for (AscensionCompatData::ResourceThresholdRule const& rule :
             AscensionCompatData::ResourceThresholdRules)
        {
            validateResourceSpell(rule.ResourceSpellId);
            validateResourceSpell(rule.ThresholdSpellId);
        }

        validateResourceSpell(SPELL_REAPER_GENERATE_SOUL);
        validateResourceSpell(SPELL_BLOODMAGE_THIRST_PASSIVE);

        for (AscensionCompatData::ResourceGainRule const& rule :
             AscensionCompatData::ResourceGainRules)
        {
            validateResourceSpell(rule.ResourceSpellId);
            validateResourceSpell(rule.RequiredAuraSpellId);
            validateResourceSpell(rule.ForbiddenAuraSpellId);
            if (rule.ChancePercent > 100)
            {
                LOG_ERROR("coa",
                    "Ascension resource generator {}-{} has invalid chance {}",
                    rule.FirstSpellId, rule.LastSpellId,
                    uint32(rule.ChancePercent));
                ++missingResourceSpells;
            }

            if (!rule.FirstSpellId && !rule.LastSpellId)
                continue;

            for (uint32 spellId = rule.FirstSpellId;
                 spellId <= rule.LastSpellId; ++spellId)
            {
                if (!sSpellMgr->GetSpellInfo(spellId))
                {
                    LOG_ERROR("coa",
                        "Ascension resource generator spell {} is missing from the server DBC",
                        spellId);
                    ++missingAbilitySpells;
                }
            }
        }

        for (AscensionCompatData::NativePowerGainRule const& rule :
             AscensionCompatData::NativePowerGainRules)
        {
            validateResourceSpell(rule.RequiredAuraSpellId);
            validateResourceSpell(rule.ForbiddenAuraSpellId);
            if (rule.PowerType >= MAX_POWERS)
            {
                LOG_ERROR("coa",
                    "Ascension native resource generator has invalid power type {}",
                    uint32(rule.PowerType));
                ++missingResourceSpells;
            }

            if (!rule.FirstSpellId && !rule.LastSpellId)
                continue;

            for (uint32 spellId = rule.FirstSpellId;
                 spellId <= rule.LastSpellId; ++spellId)
            {
                if (!sSpellMgr->GetSpellInfo(spellId))
                {
                    LOG_ERROR("coa",
                        "Ascension native resource generator spell {} is missing from the server DBC",
                        spellId);
                    ++missingAbilitySpells;
                }
            }
        }

        for (AscensionCompatData::ResourceCostRule const& rule :
             AscensionCompatData::ResourceCostRules)
        {
            validateResourceSpell(rule.ResourceSpellId);
            validateResourceSpell(rule.PreserveCostAuraSpellId);
            if (rule.PreserveCostChancePercent > 100)
            {
                LOG_ERROR("coa",
                    "Ascension resource spender {}-{} has invalid preserve-cost chance {}",
                    rule.FirstSpellId, rule.LastSpellId,
                    uint32(rule.PreserveCostChancePercent));
                ++missingResourceSpells;
            }
            for (uint32 spellId = rule.FirstSpellId;
                 spellId <= rule.LastSpellId; ++spellId)
            {
                if (!sSpellMgr->GetSpellInfo(spellId))
                {
                    LOG_ERROR("coa",
                        "Ascension resource spender spell {} is missing from the server DBC",
                        spellId);
                    ++missingAbilitySpells;
                }
            }
        }

        for (uint32 spellId : REAPER_ALL_SOUL_CONSUMERS)
        {
            if (!sSpellMgr->GetSpellInfo(spellId))
            {
                LOG_ERROR("coa",
                    "Ascension Reaper all-soul consumer spell {} is missing from the server DBC",
                    spellId);
                ++missingAbilitySpells;
            }
        }

        for (std::pair<uint32, uint32> const& range :
             REAPER_ONE_SOUL_CONSUMERS)
        {
            for (uint32 spellId = range.first; spellId <= range.second;
                 ++spellId)
            {
                if (!sSpellMgr->GetSpellInfo(spellId))
                {
                    LOG_ERROR("coa",
                        "Ascension Reaper one-soul consumer spell {} is missing from the server DBC",
                        spellId);
                    ++missingAbilitySpells;
                }
            }
        }

        LOG_INFO("coa",
            "Validated {} custom resource auras and spell helpers; {} resource spells and {} mapped abilities are missing",
            checkedResourceSpells.size(), missingResourceSpells,
            missingAbilitySpells);
    }

    void OnPlayerLogin(Player* player) const
    {
        _lastClientResourceStates.erase(player->GetGUID().GetCounter());
        _staticDecayTimers.erase(player->GetGUID());
        if (IsAscensionCustomClass(player))
        {
            SynchronizeThresholdResources(player);
            SendClientState(player, true);
        }
    }

    void OnPlayerUpdate(Player* player, uint32 diff) const
    {
        if (IsAscensionCustomClass(player))
        {
            DecayStatic(player, diff);
            SynchronizeThresholdResources(player);
            SendClientState(player, false);
        }
    }

    void OnPlayerLogout(Player* player) const
    {
        if (player)
        {
            _lastClientResourceStates.erase(player->GetGUID().GetCounter());
            _staticDecayTimers.erase(player->GetGUID());
        }
    }

    [[nodiscard]] bool CanPrepare(Spell* spell) const
    {
        if (!spell)
            return true;

        Player* player = spell->GetCaster()->ToPlayer();
        if (!player)
            return true;

        if (spell->GetSpellInfo()->Id != SPELL_REAPER_GENERATE_SOUL ||
            player->getClass() != CLASS_REAPER)
            return true;

        return GetAuraStacks(player, SPELL_REAPER_SOUL_FRAGMENT) >=
               REAPER_SOUL_FRAGMENT_COST;
    }

    void CheckCast(Spell* spell, SpellCastResult& result) const
    {
        if (!spell || spell->IsTriggered() || result != SPELL_CAST_OK)
            return;

        Player* player = spell->GetCaster()->ToPlayer();
        if (!player)
            return;

        SpellInfo const* spellInfo = spell->GetSpellInfo();
        uint32 spellId = spellInfo->Id;
        if (player->getClass() == CLASS_RANGER &&
            spellInfo->SpellFamilyName == uint32(CLASS_RANGER) + 6 &&
            spellInfo->CasterAuraSpell == 804329 && !player->HasAura(804329))
        {
            result = SPELL_FAILED_CASTER_AURASTATE;
            return;
        }

        if (player->getClass() == CLASS_REAPER && spellId == SPELL_REAPER_SCYTHE_RUSH)
        {
            Unit* target = spell->m_targets.GetUnitTarget();
            if (target && target->HasAura(SPELL_REAPER_SCYTHE_RUSH_MARKER,
                    player->GetGUID()))
            {
                result = SPELL_FAILED_TARGET_AURASTATE;
                return;
            }
        }

        for (AscensionCompatData::ResourceCostRule const& rule :
             AscensionCompatData::ResourceCostRules)
        {
            if (!Matches(player, spellId, rule.ClassId, rule.FirstSpellId,
                    rule.LastSpellId))
                continue;

            if (GetAuraStacks(player, rule.ResourceSpellId) < rule.Amount)
                result = SPELL_FAILED_NO_POWER;
            return;
        }
    }

    void OnSpellCast(Spell* spell) const
    {
        if (!spell || spell->IsTriggered())
            return;

        Player* player = spell->GetCaster()->ToPlayer();
        if (!player || !IsAscensionCustomClass(player))
            return;

        SpellInfo const* spellInfo = spell->GetSpellInfo();
        uint32 spellId = spellInfo->Id;

        if (player->getClass() == CLASS_SON_OF_ARUGAL && spellInfo->SpellFamilyName == 26 &&
            spellInfo->PowerType == POWER_HEALTH && spell->GetPowerCost() > 0 &&
            player->HasAura(SPELL_BLOODMAGE_THIRST_PASSIVE))
            ModifyAuraStacks(player, SPELL_BLOODMAGE_THIRST, 1);

        for (AscensionCompatData::ResourceGainRule const& rule :
             AscensionCompatData::ResourceGainRules)
        {
            if (!MatchesGainRule(player, spellId, rule) ||
                rule.Event != AscensionCompatData::ResourceGainEvent::Cast ||
                (rule.RequiredAuraSpellId &&
                    !player->HasAura(rule.RequiredAuraSpellId)) ||
                (rule.ForbiddenAuraSpellId &&
                    player->HasAura(rule.ForbiddenAuraSpellId)))
                continue;

            ApplyGainRule(player, rule);
        }

        for (AscensionCompatData::NativePowerGainRule const& rule :
             AscensionCompatData::NativePowerGainRules)
        {
            if (!MatchesNativePowerRule(player, spellId, rule) ||
                rule.Event != AscensionCompatData::ResourceGainEvent::Cast ||
                (rule.RequiredAuraSpellId &&
                    !player->HasAura(rule.RequiredAuraSpellId)) ||
                (rule.ForbiddenAuraSpellId &&
                    player->HasAura(rule.ForbiddenAuraSpellId)))
                continue;

            player->ModifyPower(static_cast<Powers>(rule.PowerType),
                rule.InternalAmount);
        }

        for (AscensionCompatData::ResourceCostRule const& rule :
             AscensionCompatData::ResourceCostRules)
        {
            if (!Matches(player, spellId, rule.ClassId, rule.FirstSpellId,
                    rule.LastSpellId))
                continue;

            if (rule.ClassId == CLASS_STORMBRINGER && rule.ResourceSpellId == SPELL_STORMBRINGER_STATIC &&
                player->HasAura(SPELL_STORMBRINGER_CHARGED_CONDUIT))
                break;

            if (rule.PreserveCostAuraSpellId &&
                player->HasAura(rule.PreserveCostAuraSpellId) &&
                rule.PreserveCostChancePercent &&
                roll_chance_i(rule.PreserveCostChancePercent))
                break;

            if (rule.Consumption ==
                AscensionCompatData::ResourceConsumption::Fixed)
            {
                ModifyAuraStacks(player, rule.ResourceSpellId, -rule.Amount);
                ExtendUnshackleOnStaticDepleted(player, rule);
            }
            else if (rule.Consumption ==
                     AscensionCompatData::ResourceConsumption::All)
            {
                player->RemoveAurasDueToSpell(rule.ResourceSpellId);
                ExtendUnshackleOnStaticDepleted(player, rule);
            }
            break;
        }

        ConsumeReaperSouls(player, spell);
        SynchronizeThresholdResources(player);
        SendClientState(player, false);
    }

    static bool SpellDealsDamage(SpellInfo const* spellInfo)
    {
        return spellInfo &&
            (spellInfo->HasEffect(SPELL_EFFECT_SCHOOL_DAMAGE) ||
                spellInfo->HasEffect(SPELL_EFFECT_WEAPON_DAMAGE) ||
                spellInfo->HasEffect(SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL) ||
                spellInfo->HasEffect(SPELL_EFFECT_WEAPON_PERCENT_DAMAGE) ||
                spellInfo->HasEffect(SPELL_EFFECT_NORMALIZED_WEAPON_DMG));
    }

    void OnSpellHitResult(Spell* spell, Unit* target, uint8 missInfo,
        uint32 damage, bool critical) const
    {
        if (!spell || spell->IsTriggered() || !target)
            return;

        Player* player = spell->GetCaster()->ToPlayer();
        if (!player || !IsAscensionCustomClass(player))
            return;

        bool successful = missInfo == SPELL_MISS_NONE;
        bool hostile = target != player && !player->IsFriendlyTo(target);
        bool damaging = damage > 0 || SpellDealsDamage(spell->GetSpellInfo());
        uint32 spellId = spell->GetSpellInfo()->Id;
        std::array<int8, 9> firstEventState = {};
        bool changed = false;

        for (AscensionCompatData::ResourceGainRule const& rule :
             AscensionCompatData::ResourceGainRules)
        {
            if (!MatchesGainRule(player, spellId, rule) ||
                rule.Event == AscensionCompatData::ResourceGainEvent::Cast ||
                rule.Event ==
                    AscensionCompatData::ResourceGainEvent::PeriodicDamageTick ||
                rule.Event == AscensionCompatData::ResourceGainEvent::Block ||
                (rule.RequiredAuraSpellId &&
                    !player->HasAura(rule.RequiredAuraSpellId)) ||
                (rule.ForbiddenAuraSpellId &&
                    player->HasAura(rule.ForbiddenAuraSpellId)))
                continue;

            bool qualifies = false;
            bool firstOnly = false;
            switch (rule.Event)
            {
                case AscensionCompatData::ResourceGainEvent::FirstSuccessfulHostileTarget:
                    qualifies = successful && hostile;
                    firstOnly = true;
                    break;
                case AscensionCompatData::ResourceGainEvent::EachSuccessfulHostileTarget:
                    qualifies = successful && hostile;
                    break;
                case AscensionCompatData::ResourceGainEvent::FirstSuccessfulDamagingHit:
                    qualifies = successful && hostile && damaging;
                    firstOnly = true;
                    break;
                case AscensionCompatData::ResourceGainEvent::EachSuccessfulDamagingHit:
                    qualifies = successful && hostile && damaging;
                    break;
                case AscensionCompatData::ResourceGainEvent::FirstCriticalDamagingHit:
                    qualifies = successful && hostile && damaging && critical;
                    firstOnly = true;
                    break;
                case AscensionCompatData::ResourceGainEvent::EachCriticalDamagingHit:
                    qualifies = successful && hostile && damaging && critical;
                    break;
                default:
                    break;
            }

            if (!qualifies)
                continue;

            if (firstOnly)
            {
                uint8 eventIndex = static_cast<uint8>(rule.Event);
                if (!firstEventState[eventIndex])
                {
                    firstEventState[eventIndex] =
                        spell->TryMarkScriptEventHandled(eventIndex) ? 1 : -1;
                }
                if (firstEventState[eventIndex] < 0)
                    continue;
            }

            changed = ApplyGainRule(player, rule) || changed;
        }

        for (AscensionCompatData::NativePowerGainRule const& rule :
             AscensionCompatData::NativePowerGainRules)
        {
            if (!MatchesNativePowerRule(player, spellId, rule) ||
                rule.Event == AscensionCompatData::ResourceGainEvent::Cast ||
                rule.Event ==
                    AscensionCompatData::ResourceGainEvent::PeriodicDamageTick ||
                rule.Event == AscensionCompatData::ResourceGainEvent::Block ||
                (rule.RequiredAuraSpellId &&
                    !player->HasAura(rule.RequiredAuraSpellId)) ||
                (rule.ForbiddenAuraSpellId &&
                    player->HasAura(rule.ForbiddenAuraSpellId)))
                continue;

            bool qualifies = false;
            bool firstOnly = false;
            switch (rule.Event)
            {
                case AscensionCompatData::ResourceGainEvent::FirstSuccessfulHostileTarget:
                    qualifies = successful && hostile;
                    firstOnly = true;
                    break;
                case AscensionCompatData::ResourceGainEvent::EachSuccessfulHostileTarget:
                    qualifies = successful && hostile;
                    break;
                case AscensionCompatData::ResourceGainEvent::FirstSuccessfulDamagingHit:
                    qualifies = successful && hostile && damaging;
                    firstOnly = true;
                    break;
                case AscensionCompatData::ResourceGainEvent::EachSuccessfulDamagingHit:
                    qualifies = successful && hostile && damaging;
                    break;
                case AscensionCompatData::ResourceGainEvent::FirstCriticalDamagingHit:
                    qualifies = successful && hostile && damaging && critical;
                    firstOnly = true;
                    break;
                case AscensionCompatData::ResourceGainEvent::EachCriticalDamagingHit:
                    qualifies = successful && hostile && damaging && critical;
                    break;
                default:
                    break;
            }

            if (!qualifies)
                continue;

            if (firstOnly)
            {
                uint8 eventIndex = static_cast<uint8>(rule.Event);
                if (!firstEventState[eventIndex])
                {
                    firstEventState[eventIndex] =
                        spell->TryMarkScriptEventHandled(eventIndex) ? 1 : -1;
                }
                if (firstEventState[eventIndex] < 0)
                    continue;
            }

            player->ModifyPower(static_cast<Powers>(rule.PowerType),
                rule.InternalAmount);
            changed = true;
        }

        if (changed)
        {
            SynchronizeThresholdResources(player);
            SendClientState(player, false);
        }
    }

    void OnPeriodicDamageTick(Unit* target, Unit* attacker, uint32 damage,
        SpellInfo const* spellInfo) const
    {
        if (!target || !attacker || !damage || !spellInfo)
            return;

        Player* player = attacker->ToPlayer();
        if (!player || !IsAscensionCustomClass(player) || target == player ||
            player->IsFriendlyTo(target))
            return;

        bool changed = false;
        for (AscensionCompatData::ResourceGainRule const& rule :
             AscensionCompatData::ResourceGainRules)
        {
            if (rule.Event !=
                    AscensionCompatData::ResourceGainEvent::PeriodicDamageTick ||
                !MatchesGainRule(player, spellInfo->Id, rule) ||
                (rule.RequiredAuraSpellId &&
                    !player->HasAura(rule.RequiredAuraSpellId)) ||
                (rule.ForbiddenAuraSpellId &&
                    player->HasAura(rule.ForbiddenAuraSpellId)))
                continue;

            changed = ApplyGainRule(player, rule) || changed;
        }

        for (AscensionCompatData::NativePowerGainRule const& rule :
             AscensionCompatData::NativePowerGainRules)
        {
            if (rule.Event !=
                    AscensionCompatData::ResourceGainEvent::PeriodicDamageTick ||
                !MatchesNativePowerRule(player, spellInfo->Id, rule) ||
                (rule.RequiredAuraSpellId &&
                    !player->HasAura(rule.RequiredAuraSpellId)) ||
                (rule.ForbiddenAuraSpellId &&
                    player->HasAura(rule.ForbiddenAuraSpellId)))
                continue;

            player->ModifyPower(static_cast<Powers>(rule.PowerType),
                rule.InternalAmount);
            changed = true;
        }

        if (changed)
        {
            SynchronizeThresholdResources(player);
            SendClientState(player, false);
        }
    }

    void OnBlock(Player* player) const
    {
        if (!player || !IsAscensionCustomClass(player))
            return;

        bool changed = false;
        for (AscensionCompatData::ResourceGainRule const& rule :
             AscensionCompatData::ResourceGainRules)
        {
            if (rule.ClassId != player->getClass() ||
                rule.Event != AscensionCompatData::ResourceGainEvent::Block ||
                (rule.RequiredAuraSpellId &&
                    !player->HasAura(rule.RequiredAuraSpellId)) ||
                (rule.ForbiddenAuraSpellId &&
                    player->HasAura(rule.ForbiddenAuraSpellId)))
                continue;

            changed = ApplyGainRule(player, rule) || changed;
        }

        if (changed)
        {
            SynchronizeThresholdResources(player);
            SendClientState(player, false);
        }
    }

    void SendStatus(ChatHandler* handler) const
    {
        Player* player = handler ? handler->GetPlayer() : nullptr;
        if (!player)
            return;

        handler->PSendSysMessage("Ascension resource status for class {}:",
            uint32(player->getClass()));

        if (player->getClass() == CLASS_REAPER)
        {
            handler->PSendSysMessage("Runic Power: {}/{}",
                player->GetPower(POWER_RUNIC_POWER) / 10,
                player->GetMaxPower(POWER_RUNIC_POWER) / 10);
        }

        uint32 displayed = 0;
        for (AscensionCompatData::ResourceDisplay const& resource :
             AscensionCompatData::ResourceDisplays)
        {
            if (resource.ClassId != player->getClass())
                continue;

            uint32 maximum = resource.DisplayMaximum;
            if (!maximum)
            {
                if (SpellInfo const* spellInfo =
                        sSpellMgr->GetSpellInfo(resource.SpellId))
                    maximum = spellInfo->StackAmount;
            }

            handler->PSendSysMessage("{} ({}): {}/{}", resource.Name,
                resource.SpellId, uint32(GetAuraStacks(player, resource.SpellId)),
                maximum);
            ++displayed;
        }

        for (AscensionCompatData::ResourceThresholdRule const& threshold :
             AscensionCompatData::ResourceThresholdRules)
        {
            if (threshold.ClassId != player->getClass())
                continue;

            handler->PSendSysMessage(
                "Threshold {} ({} {}): {}",
                threshold.ThresholdSpellId, threshold.Amount,
                threshold.ResourceSpellId,
                player->HasAura(threshold.ThresholdSpellId) ? "active" :
                                                               "inactive");
        }

        if (!displayed && player->getClass() != CLASS_REAPER)
            handler->SendSysMessage(
                "This class has no separate Ascension resource widget.");
    }

private:
    static bool ApplyGainRule(Player* player,
        AscensionCompatData::ResourceGainRule const& rule)
    {
        if (!rule.ChancePercent ||
            (rule.ChancePercent < 100 && !roll_chance_i(rule.ChancePercent)))
            return false;

        if (rule.Mutation ==
            AscensionCompatData::ResourceMutation::AuraStacks)
        {
            ModifyAuraStacks(player, rule.ResourceSpellId, rule.Amount);
            return true;
        }

        for (int16 count = 0; count < rule.Amount; ++count)
            player->CastSpell(player, rule.ResourceSpellId, true);
        return true;
    }

    void SendClientState(Player* player, bool force) const
    {
        if (!player || player->getClass() != CLASS_REAPER ||
            !player->GetSession())
            return;

        uint8 souls = GetAuraStacks(player, SPELL_REAPER_REAPED_SOUL);
        uint8 fragments = GetAuraStacks(player, SPELL_REAPER_SOUL_FRAGMENT);
        bool infused = player->HasAura(SPELL_REAPER_SOUL_INFUSION);
        uint32 runicPower = std::max<int32>(
            0, player->GetPower(POWER_RUNIC_POWER));
        uint32 maximumRunicPower = std::max<int32>(
            0, player->GetMaxPower(POWER_RUNIC_POWER));

        uint64 packedState = uint64(souls) |
            (uint64(fragments) << 8) |
            (uint64(infused ? 1 : 0) << 16) |
            (uint64(runicPower) << 17) |
            (uint64(maximumRunicPower) << 37);
        uint32 guid = player->GetGUID().GetCounter();
        auto previous = _lastClientResourceStates.find(guid);
        if (!force && previous != _lastClientResourceStates.end() &&
            previous->second == packedState)
            return;

        _lastClientResourceStates[guid] = packedState;

        std::string message = ASCENSION_LOCAL_RESOURCE_PREFIX;
        message += "\tR:" + std::to_string(uint32(souls));
        message += ":" + std::to_string(uint32(fragments));
        message += ":" + std::to_string(infused ? 1 : 0);
        message += ":" + std::to_string(runicPower);
        message += ":" + std::to_string(maximumRunicPower);

        WorldPacket packet;
        ChatHandler::BuildChatPacket(packet, CHAT_MSG_WHISPER, LANG_ADDON,
            player->GetGUID(), player->GetGUID(), message, 0,
            player->GetName(), player->GetName(), 0, false);
        player->GetSession()->SendPacket(&packet);

        LOG_INFO("coa",
            "Sent Reaper resource state to {}: souls={}, fragments={}, infused={}, runic={}/{}",
            player->GetName(), uint32(souls), uint32(fragments), infused,
            runicPower, maximumRunicPower);
    }

    static bool Matches(Player const* player, uint32 spellId, uint8 classId,
        uint32 firstSpellId, uint32 lastSpellId)
    {
        return player->getClass() == classId && spellId >= firstSpellId &&
               spellId <= lastSpellId;
    }

    static bool MatchesGainRule(Player const* player, uint32 spellId,
        AscensionCompatData::ResourceGainRule const& rule)
    {
        return player->getClass() == rule.ClassId &&
            ((!rule.FirstSpellId && !rule.LastSpellId) ||
                (spellId >= rule.FirstSpellId &&
                    spellId <= rule.LastSpellId));
    }

    static bool MatchesNativePowerRule(Player const* player, uint32 spellId,
        AscensionCompatData::NativePowerGainRule const& rule)
    {
        return player->getClass() == rule.ClassId &&
            ((!rule.FirstSpellId && !rule.LastSpellId) ||
                (spellId >= rule.FirstSpellId &&
                    spellId <= rule.LastSpellId));
    }

    static uint8 GetAuraStacks(Unit const* unit, uint32 spellId)
    {
        if (Aura const* aura = unit->GetAura(spellId))
            return aura->GetStackAmount();
        return 0;
    }

    static void ModifyAuraStacks(Player* player, uint32 spellId, int32 amount)
    {
        if (!amount)
            return;

        if (HandleAscensionReaperResource(player, spellId, amount))
            return;

        if (AscensionPyromancer::Resource(player, spellId, amount))
            return;

        if (AscensionCultist::Resource(player, spellId, amount))
            return;

        if (AscensionVenomancer::Resource(player, spellId, amount))
            return;

        if (AscensionTinker::Resource(player, spellId, amount))
            return;

        if (AscensionSunCleric::Resource(player, spellId, amount))
            return;

        if (spellId == SPELL_PRIMALIST_EARTHSHAPING && amount > 0 &&
            HandleAscensionPrimalistEarthshapingGain(player))
            return;

        if (spellId == 800058 && amount > 0)
            AscensionFelsworn::Generated(player, uint32(amount));

        if (Aura* aura = player->GetAura(spellId))
        {
            bool preserveDuration = amount > 0 &&
                spellId == SPELL_PRIMALIST_EARTHSHAPING;
            int32 remaining = aura->GetDuration();
            aura->ModStackAmount(amount);
            if (preserveDuration)
                aura->SetDuration(remaining);
            return;
        }

        if (amount < 0)
            return;

        if (Aura* aura = player->AddAura(spellId, player))
            if (amount > 1)
                aura->ModStackAmount(amount - 1);
    }

    static void GrantHarvestedMight(Player* player)
    {
        if (!player->HasAura(SPELL_REAPER_TIER_FROSTBITTEN_4P))
            return;

        if (Aura* aura = player->GetAura(SPELL_REAPER_HARVESTED_MIGHT))
        {
            aura->ModStackAmount(1);
            aura->RefreshDuration();
            return;
        }

        player->AddAura(SPELL_REAPER_HARVESTED_MIGHT, player);
    }

    static bool HarvestTimePreserves(Player const* player, SpellInfo const* spellInfo)
    {
        return spellInfo->CasterAuraSpell == SPELL_REAPER_SOUL_INFUSION &&
            player->HasAura(SPELL_REAPER_HARVEST_TIME);
    }

    static bool WasAvoidedByEveryTarget(Player const* player, Spell* spell)
    {
        bool external = false;
        for (TargetInfo const& hit : *spell->GetUniqueTargetInfo())
        {
            if (hit.targetGUID == player->GetGUID())
                continue;

            if (hit.missCondition != SPELL_MISS_MISS && hit.missCondition != SPELL_MISS_DODGE &&
                hit.missCondition != SPELL_MISS_PARRY)
                return false;

            external = true;
        }
        return external;
    }

    static void ExtendUnshackleOnStaticDepleted(Player* player,
        AscensionCompatData::ResourceCostRule const& rule)
    {
        if (rule.ClassId != CLASS_STORMBRINGER ||
            rule.ResourceSpellId != SPELL_STORMBRINGER_STATIC ||
            !player->HasSpell(SPELL_STORMBRINGER_WRATH_OF_ALAKIR))
            return;

        player->CastSpell(player, SPELL_STORMBRINGER_UNSHACKLE_EXTENSION, true);
    }

    static void ConsumeReaperSouls(Player* player, Spell* spell)
    {
        if (ConsumeReaperSoulsImpl(player, spell))
            GrantHarvestedMight(player);
    }

    static bool ConsumeReaperSoulsImpl(Player* player, Spell* spell)
    {
        if (player->getClass() != CLASS_REAPER)
            return false;

        SpellInfo const* spellInfo = spell->GetSpellInfo();

        uint32 spellId = spellInfo->Id;

        if (HarvestTimePreserves(player, spellInfo))
            return false;

        if (std::find(REAPER_ALL_SOUL_CONSUMERS.begin(),
                REAPER_ALL_SOUL_CONSUMERS.end(), spellId) !=
            REAPER_ALL_SOUL_CONSUMERS.end())
        {
            player->RemoveAurasDueToSpell(SPELL_REAPER_REAPED_SOUL);
            player->RemoveAurasDueToSpell(SPELL_REAPER_SOUL_INFUSION);
            return true;
        }

        if (spellInfo->CasterAuraSpell == SPELL_REAPER_SOUL_INFUSION &&
            player->HasAura(SPELL_REAPER_SOUL_INFUSION) &&
            !WasAvoidedByEveryTarget(player, spell))
        {
            player->CastSpell(player, SPELL_REAPER_SOUL_INFUSION_REMOVER, true);
            ApplyAscensionReaperSoulInfusionSpent(player);
            return true;
        }

        for (std::pair<uint32, uint32> const& range :
             REAPER_ONE_SOUL_CONSUMERS)
        {
            if (spellId >= range.first && spellId <= range.second)
            {
                ModifyAuraStacks(player, SPELL_REAPER_REAPED_SOUL, -1);
                return true;
            }
        }

        return false;
    }

    void DecayStatic(Player* player, uint32 diff) const
    {
        ObjectGuid const guid = player->GetGUID();
        uint8 const stacks = GetAuraStacks(player, SPELL_STORMBRINGER_STATIC);
        if (player->getClass() != CLASS_STORMBRINGER || !player->IsAlive() || !stacks || player->IsInCombat())
        {
            _staticDecayTimers.erase(guid);
            return;
        }

        constexpr uint32 graceMs = 5000;
        constexpr uint32 intervalMs = 1000;
        uint32& timer = _staticDecayTimers[guid];
        uint64 const elapsed = uint64(timer) + diff;
        if (elapsed < graceMs + intervalMs)
        {
            timer = uint32(elapsed);
            return;
        }

        uint32 const loss = uint32(std::min<uint64>(stacks, (elapsed - graceMs) / intervalMs));
        timer = graceMs + uint32((elapsed - graceMs) % intervalMs);
        ModifyAuraStacks(player, SPELL_STORMBRINGER_STATIC, -int32(loss));
        if (loss == stacks)
            _staticDecayTimers.erase(guid);
    }

    static void SynchronizeThresholdResources(Player* player)
    {
        for (AscensionCompatData::ResourceThresholdRule const& rule :
             AscensionCompatData::ResourceThresholdRules)
        {
            if (rule.ClassId != player->getClass())
                continue;

            bool meetsThreshold =
                GetAuraStacks(player, rule.ResourceSpellId) >= rule.Amount;
            if (meetsThreshold && !player->HasAura(rule.ThresholdSpellId))
            {
                player->CastSpell(player, rule.ThresholdSpellId, true);
            }
            else if (!meetsThreshold &&
                     player->HasAura(rule.ThresholdSpellId))
            {
                player->RemoveAurasDueToSpell(rule.ThresholdSpellId);
            }
        }

        if (player->getClass() == CLASS_PYROMANCER)
        {
            while (GetAuraStacks(player, SPELL_PYROMANCER_HEAT) >=
                   PYROMANCER_HEAT_PER_EMBER)
            {
                ModifyAuraStacks(player, SPELL_PYROMANCER_HEAT,
                    -PYROMANCER_HEAT_PER_EMBER);
                ModifyAuraStacks(player, SPELL_PYROMANCER_EMBER, 1);
            }
        }

        if (player->getClass() == CLASS_REAPER &&
            GetAuraStacks(player, SPELL_REAPER_SOUL_FRAGMENT) >=
                REAPER_SOUL_FRAGMENT_COST)
        {
            while (GetAuraStacks(player, SPELL_REAPER_SOUL_FRAGMENT) >=
                   REAPER_SOUL_FRAGMENT_COST)
            {
                ModifyAuraStacks(player, SPELL_REAPER_SOUL_FRAGMENT,
                    -REAPER_SOUL_FRAGMENT_COST);
                ModifyAuraStacks(player, SPELL_REAPER_REAPED_SOUL, 1);
            }
        }

        if (player->getClass() == CLASS_REAPER &&
            GetAuraStacks(player, SPELL_REAPER_REAPED_SOUL) >= 3 &&
            !player->HasAura(SPELL_REAPER_SOUL_INFUSION))
        {
            player->CastSpell(player, SPELL_REAPER_SOUL_INFUSION, true);
            ApplyAscensionReaperSoulInfusionGained(player);
        }
    }

    mutable std::unordered_map<uint32, uint64> _lastClientResourceStates;
    mutable std::unordered_map<ObjectGuid, uint32> _staticDecayTimers;
};

bool SendCollectionCreatureQueryResponse(WorldSession* session, uint32 creatureId);

class AscensionCollectionService {
public:
    static bool IsCosmeticCategory(uint32 category)
    {
        return category >= 56 && category <= 58;
    }

    static uint32 ResolveCosmeticSpell(uint32 appearance, uint32 display, uint32 alternate)
    {
        if (appearance == 2992 || appearance == 51444 || appearance == 52428)
            return 0;
        if (appearance == 2714)
            display = 985235;
        if (appearance == 42965)
            display = 935566;
        if (!sSpellMgr->GetSpellInfo(display))
            display = alternate;

        std::unordered_set<uint32> visited;
        while (display && visited.insert(display).second && visited.size() <= 8)
        {
            SpellInfo const* spell = sSpellMgr->GetSpellInfo(display);
            if (!spell || spell->Effects[EFFECT_1].Effect || spell->Effects[EFFECT_2].Effect)
                return 0;
            SpellEffectInfo const& effect = spell->Effects[EFFECT_0];
            if (effect.Effect == SPELL_EFFECT_TRIGGER_SPELL)
            {
                display = effect.TriggerSpell;
                continue;
            }
            if (effect.IsAura() && (effect.ApplyAuraName == SPELL_AURA_DUMMY ||
                effect.ApplyAuraName == SPELL_AURA_MOD_SCALE ||
                (display == 1985213 && effect.ApplyAuraName == SPELL_AURA_PROC_TRIGGER_SPELL)))
                return display;
            return 0;
        }
        return 0;
    }

  static AscensionCollectionService &Instance() {
    static AscensionCollectionService instance;
    return instance;
  }

  bool LoadClientData() {
    _appearances.clear();
    _itemAppearances.clear();
    _itemSetItems.clear();
    _vanityItems.clear();
    _allAppearanceIds.clear();
    _allVanityItemIds.clear();

    ClientDBC appearances;
    bool appearancesLoaded =
        appearances.Load(GetClientDBCPath("Appearances.dbc"), 9);
    for (uint32 row = 0; row < appearances.GetRecordCount(); ++row) {
      ClientDBC::Record record = appearances.GetRecord(row);
      uint32 appearanceId = record.GetUInt32(0);
      if (!appearanceId)
        continue;

      uint32 displayId = record.GetUInt32(3);
      _appearances[appearanceId] =
          AppearanceInfo{displayId, record.GetUInt32(5), record.GetUInt32(6),
                         record.GetUInt32(7), displayId};
      AppearanceInfo& appearance = _appearances[appearanceId];
      if (IsCosmeticCategory(appearance.PrimaryCategory))
        appearance.CosmeticSpell = ResolveCosmeticSpell(appearanceId,
            displayId, record.GetUInt32(8));
      _allAppearanceIds.push_back(appearanceId);
    }

    ClientDBC itemAppearances;
    bool itemAppearancesLoaded =
        itemAppearances.Load(GetClientDBCPath("ItemAppearances.dbc"), 3);
    for (uint32 row = 0; row < itemAppearances.GetRecordCount(); ++row) {
      ClientDBC::Record record = itemAppearances.GetRecord(row);
      uint32 itemId = record.GetUInt32(1);
      uint32 appearanceId = record.GetUInt32(2);
      if (itemId && appearanceId)
        _itemAppearances[itemId] = appearanceId;
    }

    ClientDBC itemSets;
    bool itemSetsLoaded = itemSets.Load(GetClientDBCPath("ItemSet.dbc"), 35);
    for (uint32 row = 0; row < itemSets.GetRecordCount(); ++row) {
      ClientDBC::Record record = itemSets.GetRecord(row);
      uint32 itemSetId = record.GetUInt32(0);
      if (!itemSetId)
        continue;

      std::vector<uint32> &items = _itemSetItems[itemSetId];
      for (uint32 field = 18; field <= 34; ++field) {
        uint32 itemId = record.GetUInt32(field);
        if (itemId)
          items.push_back(itemId);
      }
    }

    ClientDBC vanity;
    bool vanityLoaded =
        vanity.Load(GetClientDBCPath("VanityCollection.dbc"), 77);
    for (uint32 row = 0; row < vanity.GetRecordCount(); ++row) {
      ClientDBC::Record record = vanity.GetRecord(row);
      uint32 itemId = record.GetUInt32(1);
      if (!itemId)
        continue;

      VanityInfo info{
          record.GetUInt32(76), record.GetUInt32(12), record.GetUInt32(2)};

      for (uint32 field = 0; field < VANITY_STORE_RECORD_DWORDS; ++field)
        info.StoreRecord[field] = record.GetUInt32(field);

      _vanityItems[itemId] = info;
      _allVanityItemIds.push_back(itemId);
    }

    std::sort(_allAppearanceIds.begin(), _allAppearanceIds.end());
    _allAppearanceIds.erase(
        std::unique(_allAppearanceIds.begin(), _allAppearanceIds.end()),
        _allAppearanceIds.end());

    LOG_INFO("coa",
             "Loaded Ascension collection data: {} appearances, {} item "
             "mappings, {} item sets, {} vanity entries",
             _appearances.size(), _itemAppearances.size(),
             _itemSetItems.size(), _vanityItems.size());

    if (!itemSetsLoaded)
      LOG_WARN("coa",
               "Ascension item-set expansion is unavailable; individual "
               "appearance categories remain usable");

    _clientDataLoaded =
        appearancesLoaded && itemAppearancesLoaded && vanityLoaded;
    return _clientDataLoaded;
  }

  void QueueClientPacket(uint32 accountId, WorldPacket const &packet) {
    std::lock_guard lock(_packetMutex);
    std::deque<WorldPacket> &queue = _pendingPackets[accountId];
    auto const isWorldEntryNotice = [](WorldPacket const& queued)
    {
      return queued.GetOpcode() == CMSG_EXTENSION_INITIALIZED;
    };
    if (isWorldEntryNotice(packet) && std::any_of(queue.begin(), queue.end(), isWorldEntryNotice))
      return;

    if (queue.size() >= MAX_QUEUED_EXTENSION_PACKETS)
    {
      RejectClientPacket(accountId, packet, "its queue is full");
      return;
    }

    queue.emplace_back(packet);
  }

  void RejectClientPacket(uint32 accountId, WorldPacket const& packet, std::string_view reason)
  {
    std::lock_guard lock(_rejectedPacketMutex);
    uint32 const rejected = ++_rejectedPackets[accountId];
    if (std::has_single_bit(rejected))
      LOG_WARN("coa", "Rejected Ascension extension packet 0x{:04X} ({} bytes) from account {}: {}; {} rejected so far",
          packet.GetOpcode(), packet.size(), accountId, reason, rejected);
  }

  void OnPlayerLogin(Player *player) {
    if (!_clientDataLoaded)
    {
      ChatHandler(player->GetSession())
          .SendSysMessage("Ascension collection data is unavailable; transmog "
                          "and vanity are disabled.");
      return;
    }

    std::shared_ptr<PlayerCollectionState> state =
        std::make_shared<PlayerCollectionState>();
    state->AccountId = player->GetSession()->GetAccountId();
    LoadPlayerState(player, *state);

    UnlockLocalAppearanceCatalog(player, *state);

    {
      std::lock_guard lock(_stateMutex);
      _playerStates[player->GetGUID().GetCounter()] = state;
    }

    if (ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::AUTO_COLLECT_APPEARANCES))
      ScanPlayerInventory(player, *state);

    BeginAppearanceCollectionSync(player, *state);
    state->LoginResyncTimer = APPEARANCE_LOGIN_RESYNC_DELAY_MS;
    SendActiveAppearances(player, *state);
    SendOutfitCollection(player);
    SendAppearanceVisibility(player, *state);
    SendRealmInfo(player);
    SendVanityCollection(player, *state);
    SendOwnedVanityStoreRecords(player, *state);
    RefreshVisibleItems(player);
    for (auto const& [id, appearance] : _appearances)
        if (appearance.CosmeticSpell)
            player->RemoveAurasDueToSpell(appearance.CosmeticSpell, player->GetGUID());
    RefreshCosmetics(player, *state);
    InitializeRiding(player);
    QueueOwnedCompanionSpells(player, *state);

    LOG_INFO("coa",
             "Synchronized Ascension collections for {}: {} appearances, {} "
             "saved vanity items",
             player->GetName(), state->CollectedAppearances.size(),
             state->OwnedVanityItems.size());
  }

  void OnPlayerLogout(Player *player) {
    {
      std::lock_guard lock(_stateMutex);
      _playerStates.erase(player->GetGUID().GetCounter());
    }

    {
      std::lock_guard lock(_rejectedPacketMutex);
      _rejectedPackets.erase(player->GetSession()->GetAccountId());
    }

    std::lock_guard lock(_packetMutex);
    _pendingPackets.erase(player->GetSession()->GetAccountId());
  }

  [[nodiscard]] std::vector<WorldPacket> TakeClientPackets(uint32 accountId)
  {
    std::vector<WorldPacket> packets;
    std::lock_guard lock(_packetMutex);
    auto itr = _pendingPackets.find(accountId);
    if (itr == _pendingPackets.end())
      return packets;

    std::deque<WorldPacket> &queue = itr->second;
    std::size_t budget = MAX_EXTENSION_REPLIES_PER_UPDATE;
    while (!queue.empty())
    {
      std::size_t const replies = ExpectedReplies(queue.front());
      if (replies > budget)
        break;

      budget -= replies;
      packets.push_back(std::move(queue.front()));
      queue.pop_front();
    }

    if (queue.empty())
      _pendingPackets.erase(itr);
    return packets;
  }

  void OnPlayerUpdate(Player *player, uint32 diff) {
    for (WorldPacket &packet : TakeClientPackets(player->GetSession()->GetAccountId()))
      HandleClientPacket(player, packet);

    ProcessPendingAppearanceAdds(player, diff);
    ProcessPendingCompanionSpells(player, diff);
    ProcessCompanionLoot(player, diff);
    ProcessCompanionLoot(player, diff, true);
    if (auto state = GetState(player))
    {
        if (state->CosmeticTimer <= diff)
        {
            state->CosmeticTimer = 1000;
            RefreshCosmetics(player, *state);
        }
        else
            state->CosmeticTimer -= diff;
    }
  }

    void ProcessCompanionLoot(Player* player, uint32 diff, bool skin = false)
    {
        constexpr uint32 CREATURE_LOOTBOT_3000 = 44022;
        uint32 const category = skin ? APPEARANCE_CATEGORY_COMPANION_SKINNING : APPEARANCE_CATEGORY_COMPANION_LOOT;
        uint32 const appearance = skin ? APPEARANCE_SKIN_PEELER : APPEARANCE_LOOT_TRANSFIGURATOR;
        auto state = GetState(player);
        if (!state)
            return;

        bool const hasAppearance = state->ActiveAppearances[category] == appearance &&
            state->CollectedAppearances.contains(appearance);

        bool isLootbot = false;
        if (!skin && !hasAppearance)
        {
            Creature* c = player->GetMap()->GetCreature(player->GetCritterGUID());
            isLootbot = c && c->IsAlive() && c->GetOwnerGUID() == player->GetGUID() &&
                        c->GetEntry() == CREATURE_LOOTBOT_3000;
        }

        if (!hasAppearance && !isLootbot)
            return;

        uint32& timer = skin ? state->CompanionSkinningTimer : state->CompanionLootTimer;
        if (timer > diff)
        {
            timer -= diff;
            return;
        }
        SpellInfo const* spell = sSpellMgr->GetSpellInfo(skin ? SPELL_SKIN_PEELER : SPELL_LOOT_TRANSFIGURATOR);
        if (!spell || !spell->Effects[EFFECT_0].Amplitude)
            return;
        timer = spell->Effects[EFFECT_0].Amplitude;
        Creature* companion = player->GetMap()->GetCreature(player->GetCritterGUID());
        if (!companion || !companion->IsAlive() || companion->GetOwnerGUID() != player->GetGUID())
            return;
        float const radius = spell->Effects[EFFECT_0].CalcRadius(player);
        if (radius <= 0.0f)
            return;
        std::list<Creature*> corpses;
        companion->GetDeadCreatureListInGrid(corpses, radius, true);
        for (Creature* creature : corpses)
            player->LootCreatureWithCompanion(creature, radius, skin);
    }

    void InitializeRiding(Player* player) const
    {
        if (!ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::MAX_RIDING_FROM_START))
            return;

        for (uint32 spellId : {SPELL_RIDING_APPRENTICE, SPELL_RIDING_JOURNEYMAN,
            SPELL_RIDING_EXPERT, SPELL_RIDING_ARTISAN, SPELL_COLD_WEATHER_FLYING})
            if (sSpellMgr->GetSpellInfo(spellId) && !player->HasSpell(spellId))
                LearnRestoredSpell(player, spellId);

        player->SetSkill(SKILL_RIDING, 4, 300, 300);
    }

    void PrepareOwnedCompanionsBeforeMap(Player* player)
    {
        if (!_clientDataLoaded || player->IsInWorld() || !player->GetSession()->PlayerLoading() ||
            !ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::LEARN_OWNED_COMPANIONS))
            return;

        PlayerCollectionState state;
        state.AccountId = player->GetSession()->GetAccountId();
        LoadPlayerState(player, state);
        std::vector<uint32> const spells = GetMissingOwnedCompanionSpells(player, state);
        std::size_t learned = 0;
        for (uint32 spellId : spells)
        {
            LearnRestoredSpell(player, spellId);
            if (!player->HasSpell(spellId))
                continue;

            player->MarkSpellForSave(spellId);
            ++learned;
        }

        if (learned)
        {
            player->SendInitialSpells();
            LOG_INFO("coa", "Prepared {} account mount/companion spells for {} before entering the world",
                learned, player->GetName());
        }
    }

    static constexpr std::array<uint32, 4> BankVanityItems = { 110000, 134985, 509892, 1180097 };

    [[nodiscard]] static bool IsBankVanityItem(uint32 itemId)
    {
        return std::find(BankVanityItems.begin(), BankVanityItems.end(), itemId) != BankVanityItems.end();
    }

    [[nodiscard]] bool OwnsBankVanityItem(Player* player, PlayerCollectionState const& state, uint32 itemId) const
    {
        if (state.OwnedVanityItems.contains(itemId) || player->HasItemCount(itemId))
            return true;

        ItemTemplate const* proto = sObjectMgr->GetItemTemplate(itemId);
        if (!proto)
            return false;

        for (uint8 slot = 0; slot < MAX_ITEM_PROTO_SPELLS; ++slot)
        {
            uint32 const spellId = uint32(std::max<int32>(proto->Spells[slot].SpellId, 0));
            if (spellId && player->HasSpell(spellId))
                return true;
        }

        return false;
    }

    std::vector<uint32> GetMissingBankSpells(Player* player, PlayerCollectionState const& state) const
    {
        std::vector<uint32> spells;

        for (uint32 itemId : BankVanityItems)
        {
            if (!OwnsBankVanityItem(player, state, itemId))
                continue;

            ItemTemplate const* proto = sObjectMgr->GetItemTemplate(itemId);
            if (!proto)
                continue;

            for (uint8 slot = 0; slot < MAX_ITEM_PROTO_SPELLS; ++slot)
            {
                uint32 const spellId = uint32(std::max<int32>(proto->Spells[slot].SpellId, 0));
                if (!spellId || player->HasSpell(spellId) || !sSpellMgr->GetSpellInfo(spellId))
                    continue;

                spells.push_back(spellId);
            }
        }

        std::sort(spells.begin(), spells.end());
        spells.erase(std::unique(spells.begin(), spells.end()), spells.end());
        return spells;
    }

    void LearnOwnedBankSpells(Player* player, PlayerCollectionState const& state, bool beforeMap) const
    {
        std::size_t learned = 0;
        for (uint32 spellId : GetMissingBankSpells(player, state))
        {
            LearnRestoredSpell(player, spellId);
            if (player->HasSpell(spellId))
                ++learned;
        }

        if (!learned)
            return;

        if (beforeMap)
            player->SendInitialSpells();

        LOG_INFO("coa", "Learned {} owned bank spell(s) for {}",
                 learned, player->GetName());
    }

    void PrepareOwnedBankSpellsBeforeMap(Player* player)
    {
        if (!_clientDataLoaded || player->IsInWorld() || !player->GetSession()->PlayerLoading())
            return;

        PlayerCollectionState state;
        state.AccountId = player->GetSession()->GetAccountId();
        LoadPlayerState(player, state);

        LearnOwnedBankSpells(player, state, true);
    }

    std::vector<uint32> GetMissingOwnedCompanionSpells(Player* player, PlayerCollectionState const& state) const
    {
        std::vector<uint32> spells;
        if (!ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::LEARN_OWNED_COMPANIONS))
            return spells;

        bool const unlockAll = ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::UNLOCK_ALL_VANITY);
        for (auto const& [itemId, vanity] : _vanityItems)
        {
            bool const utilityCompanion = itemId == ITEM_WONDROUS_WISDOMBALL || itemId == ITEM_FIX_O_TRON_5000;
            if (!(vanity.CategoryMask & (VANITY_CATEGORY_MOUNTS | VANITY_CATEGORY_COMPANIONS)) && !utilityCompanion)
                continue;
            if ((!unlockAll && !state.OwnedVanityItems.contains(itemId)) ||
                std::binary_search(AscensionCollectibles::SigilSpells.begin(),
                    AscensionCollectibles::SigilSpells.end(), vanity.LearnedSpell) ||
                !vanity.LearnedSpell || player->HasSpell(vanity.LearnedSpell) ||
                !sSpellMgr->GetSpellInfo(vanity.LearnedSpell))
                continue;

            spells.push_back(vanity.LearnedSpell);
        }

        std::sort(spells.begin(), spells.end());
        spells.erase(std::unique(spells.begin(), spells.end()), spells.end());
        return spells;
    }

    void QueueOwnedCompanionSpells(Player* player, PlayerCollectionState& state) const
    {
        state.PendingCompanionSpells = GetMissingOwnedCompanionSpells(player, state);
        state.CompanionSpellTimer = 5000;
        if (!state.PendingCompanionSpells.empty())
            LOG_INFO("coa", "Queued {} owned mount/companion spells for {} (4 per 200 ms)",
                state.PendingCompanionSpells.size(), player->GetName());
    }

    void ProcessPendingCompanionSpells(Player* player, uint32 diff)
    {
        auto state = GetState(player);
        if (!state || state->PendingCompanionSpells.empty())
            return;

        if (state->CompanionSpellTimer > diff)
        {
            state->CompanionSpellTimer -= diff;
            return;
        }

        state->CompanionSpellTimer = COMPANION_SPELL_BATCH_INTERVAL_MS;
        std::size_t const end = std::min(state->NextCompanionSpell + COMPANION_SPELLS_PER_BATCH,
            state->PendingCompanionSpells.size());
        while (state->NextCompanionSpell < end)
        {
            uint32 const spellId = state->PendingCompanionSpells[state->NextCompanionSpell++];
            if (!player->HasSpell(spellId))
                LearnRestoredSpell(player, spellId);
        }

        if (state->NextCompanionSpell == state->PendingCompanionSpells.size())
        {
            LOG_INFO("coa", "Completed owned mount/companion spell synchronization for {}", player->GetName());
            state->PendingCompanionSpells.clear();
            state->NextCompanionSpell = 0;
        }
    }

  void OnItemObtained(Player *player, Item *item) {
    if (!item)
      return;

    std::shared_ptr<PlayerCollectionState> state = GetState(player);
    if (!state)
      return;

    CollectItem(player, *state, item->GetEntry(), true);
  }

  void OnVisibleItemSet(Player *player, uint8 slot, Item *item) {
    if (!item)
      return;

    std::shared_ptr<PlayerCollectionState> state = GetState(player);
    if (!state)
      return;

    uint8 itemCategoryId = AppearanceCategoryForEquipmentSlot(slot);
    if (itemCategoryId && state->CanSeeItemAppearances)
    {
      uint32 appearanceId = state->ActiveAppearances[itemCategoryId];
      auto appearanceItr = _appearances.find(appearanceId);
      if (appearanceItr != _appearances.end() &&
          appearanceItr->second.SourceItem) {
        player->SetUInt32Value(PLAYER_VISIBLE_ITEM_1_ENTRYID + slot * 2,
                               appearanceItr->second.SourceItem);
      }
    }

    uint8 effectCategoryId = WeaponEffectCategoryForEquipmentSlot(slot);
    if (!effectCategoryId || !state->CanSeeSpellAppearances)
      return;

    uint32 effectAppearanceId = state->ActiveAppearances[effectCategoryId];
    auto effectItr = _appearances.find(effectAppearanceId);
    if (effectItr == _appearances.end() || !effectItr->second.EnchantId)
      return;

    player->SetUInt16Value(
        PLAYER_VISIBLE_ITEM_1_ENCHANTMENT + slot * 2, 0,
        static_cast<uint16>(effectItr->second.EnchantId));
  }

    uint32 GetAmmunitionDisplay(Player* player)
    {
        auto state = GetState(player);
        if (!state || !state->CanSeeSpellAppearances)
            return 0;

        uint32 const appearanceId = state->ActiveAppearances[APPEARANCE_CATEGORY_AMMUNITION];
        if (!state->CollectedAppearances.contains(appearanceId))
            return 0;

        auto const entry = std::lower_bound(AscensionAmmunition::Entries.begin(), AscensionAmmunition::Entries.end(),
            appearanceId, [](AscensionAmmunition::Entry const& row, uint32 id) { return row.AppearanceId < id; });
        return entry != AscensionAmmunition::Entries.end() && entry->AppearanceId == appearanceId ?
            entry->ItemDisplayId : 0;
    }

  void ApplyLocalAppearance(Player *player, uint32 categoryId,
                            uint32 appearanceId) {
    std::shared_ptr<PlayerCollectionState> state = GetState(player);
    if (!state)
      return;

    if (!categoryId || categoryId >= state->ActiveAppearances.size())
    {
      SendApplyResult(player, "APPLY_APPEARANCES_INVALID_CATEGORY");
      return;
    }

    if (appearanceId)
    {
      if (!state->CollectedAppearances.contains(appearanceId))
      {
        SendApplyResult(player, "APPLY_APPEARANCES_NOT_COLLECTED");
        return;
      }

      auto appearanceItr = _appearances.find(appearanceId);
      if (appearanceItr == _appearances.end())
      {
        SendApplyResult(player, "APPLY_APPEARANCES_INVALID_SELECTION");
        return;
      }

      AppearanceInfo const &appearance = appearanceItr->second;
      if ((categoryId <= 14 || categoryId == APPEARANCE_CATEGORY_AMMUNITION ||
          IsCosmeticCategory(categoryId)) &&
          appearance.PrimaryCategory != categoryId &&
          appearance.SecondaryCategory != categoryId &&
          appearance.TertiaryCategory != categoryId) {
        SendApplyResult(player, "APPLY_APPEARANCES_INVALID_CATEGORY");
        return;
      }

      if (IsCosmeticCategory(categoryId) && !appearance.CosmeticSpell)
      {
        SendApplyResult(player, "APPLY_APPEARANCES_INVALID_SELECTION");
        return;
      }

      if (categoryId == 55 &&
          !ExpandItemSetAppearance(state->ActiveAppearances, appearanceId)) {
        SendApplyResult(player, "APPLY_APPEARANCES_INVALID_SELECTION");
        return;
      }
    }

    state->ActiveAppearances[categoryId] = appearanceId;
    SaveActiveAppearances(player, *state);
    RefreshCosmetics(player, *state);
    RefreshVisibleItems(player);
    SendActiveAppearances(player, *state);
    SendApplyResult(player, "APPLY_APPEARANCES_OK");

    LOG_INFO("coa",
             "Applied local appearance {} to category {} for {}",
             appearanceId, categoryId, player->GetName());
  }

  void DeliverLocalVanityItem(Player *player, uint32 itemId) {
    std::shared_ptr<PlayerCollectionState> state = GetState(player);
    if (!state)
      return;

    auto vanityItr = _vanityItems.find(itemId);
    if (vanityItr == _vanityItems.end())
    {
      ChatHandler(player->GetSession())
          .PSendSysMessage(
              "Vanity item {} is not present in this client build.", itemId);
      return;
    }

    bool unlockAll = ascensionCompatConfig.GetConfigValue<bool>(
        AscensionCompatConfig::UNLOCK_ALL_VANITY);

    bool const entitled =
        IsBankVanityItem(itemId)
            ? OwnsBankVanityItem(player, *state, itemId)
            : (unlockAll || state->OwnedVanityItems.contains(itemId));
    if (!entitled)
    {
      ChatHandler(player->GetSession())
          .SendSysMessage(IsBankVanityItem(itemId)
              ? "That bank is not unlocked on this account."
              : "That vanity item is not unlocked on this account.");
      return;
    }

    if (std::binary_search(AscensionCollectibles::SigilVanityItems.begin(),
        AscensionCollectibles::SigilVanityItems.end(), itemId))
    {
        ChatHandler(player->GetSession()).SendSysMessage("Sigil companions are excluded from local grants.");
        return;
    }

    if (sObjectMgr->GetItemTemplate(itemId))
    {
      ItemPosCountVec destinations;
      InventoryResult result =
          player->CanStoreNewItem(NULL_BAG, NULL_SLOT, destinations, itemId, 1);
      if (result != EQUIP_ERR_OK)
      {
        player->SendEquipError(result, nullptr, nullptr, itemId);
        return;
      }

      if (Item* delivered = player->StoreNewItem(destinations, itemId, true))
        player->SendNewItem(delivered, 1, true, false);

      if (IsBankVanityItem(itemId))
        LearnOwnedBankSpells(player, *state, false);

      return;
    }

    uint32 learnedSpell = vanityItr->second.LearnedSpell;
    if (learnedSpell &&
        ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ALLOW_LEARNED_SPELL_DELIVERY) &&
        sSpellMgr->GetSpellInfo(learnedSpell)) {
      player->learnSpell(learnedSpell);
      ChatHandler(player->GetSession())
          .PSendSysMessage("Learned vanity spell {} because item {} has no "
                           "local server template.",
                           learnedSpell, itemId);
      return;
    }

    ChatHandler(player->GetSession())
        .PSendSysMessage("Vanity item {} exists in the Ascension client but "
                         "has no AzerothCore item template yet.",
                         itemId);
  }

private:
  void UnlockLocalAppearanceCatalog(Player *player,
                                    PlayerCollectionState &state) {
    if (!ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::UNLOCK_LOCAL_APPEARANCE_CATALOG))
      return;

    std::size_t before = state.CollectedAppearances.size();
    for (uint32 appearanceId : _allAppearanceIds)
      state.CollectedAppearances.insert(appearanceId);

    LOG_INFO("coa",
             "Unlocked {} local wardrobe appearances for {} ({} total)",
             state.CollectedAppearances.size() - before, player->GetName(),
             state.CollectedAppearances.size());
  }

  void BeginAppearanceCollectionSync(Player *player,
                                     PlayerCollectionState &state) {
    std::vector<uint32> appearances(state.CollectedAppearances.begin(),
                                    state.CollectedAppearances.end());
    std::sort(appearances.begin(), appearances.end());

    if (appearances.size() <= MAX_APPEARANCE_SNAPSHOT_ENTRIES)
    {
      SendAppearanceCollection(player, appearances);
      LOG_INFO("coa", "Sent complete wardrobe snapshot for {}: {} appearances, no per-item login notifications",
          player->GetName(), appearances.size());
      return;
    }

    uint32 perCategory = ascensionCompatConfig.GetConfigValue<uint32>(
        AscensionCompatConfig::APPEARANCE_CATALOG_PER_CATEGORY);
    std::array<uint32, APPEARANCE_CATEGORY_COUNT> categoryCounts{};
    std::vector<uint32> snapshot;
    snapshot.reserve(MAX_APPEARANCE_SNAPSHOT_ENTRIES);
    std::unordered_set<uint32> snapshotIds;
    snapshotIds.reserve(MAX_APPEARANCE_SNAPSHOT_ENTRIES);

    if (perCategory)
    {
      for (uint32 appearanceId : appearances) {
        if (snapshot.size() >= MAX_APPEARANCE_SNAPSHOT_ENTRIES)
          break;

        auto itr = _appearances.find(appearanceId);
        if (itr == _appearances.end())
          continue;

        AppearanceInfo const &appearance = itr->second;
        uint32 categoryId = appearance.PrimaryCategory;
        if (!categoryId || categoryId > 14 ||
            categoryCounts[categoryId] >= perCategory ||
            !appearance.SourceItem ||
            !sObjectMgr->GetItemTemplate(appearance.SourceItem))
          continue;

        snapshot.push_back(appearanceId);
        snapshotIds.insert(appearanceId);
        ++categoryCounts[categoryId];
      }
    }

    for (uint32 appearanceId : appearances) {
      if (snapshot.size() >= MAX_APPEARANCE_SNAPSHOT_ENTRIES)
        break;

      if (snapshotIds.insert(appearanceId).second)
        snapshot.push_back(appearanceId);
    }

    state.PendingAppearanceAdds.clear();
    state.PendingAppearanceAdds.reserve(appearances.size() - snapshot.size());
    for (uint32 appearanceId : appearances) {
      if (!snapshotIds.contains(appearanceId))
        state.PendingAppearanceAdds.push_back(appearanceId);
    }

    state.NextPendingAppearanceAdd = 0;
    state.AppearanceAddTimer = APPEARANCE_ADD_INITIAL_DELAY_MS;
    SendAppearanceCollection(player, snapshot);

    LOG_INFO("coa",
             "Started full wardrobe sync for {}: {} snapshot entries and {} "
             "streamed entries",
             player->GetName(), snapshot.size(),
             state.PendingAppearanceAdds.size());
  }

  void ProcessPendingAppearanceAdds(Player *player, uint32 diff) {
    std::shared_ptr<PlayerCollectionState> state = GetState(player);
    if (!state)
      return;

    if (state->LoginResyncTimer)
    {
      if (state->LoginResyncTimer > diff)
      {
        state->LoginResyncTimer -= diff;
        return;
      }

      state->LoginResyncTimer = 0;
      SendActiveAppearances(player, *state);
      SendOutfitCollection(player);
      SendAppearanceVisibility(player, *state);
      SendVanityCollection(player, *state);
    }

    if (state->PendingAppearanceAdds.empty())
      return;

    if (state->AppearanceAddTimer > diff)
    {
      state->AppearanceAddTimer -= diff;
      return;
    }

    state->AppearanceAddTimer = APPEARANCE_ADD_BATCH_INTERVAL_MS;
    std::size_t end = std::min(
        state->NextPendingAppearanceAdd + APPEARANCE_ADDS_PER_BATCH,
        state->PendingAppearanceAdds.size());
    for (; state->NextPendingAppearanceAdd < end;
         ++state->NextPendingAppearanceAdd) {
      uint32 appearanceId =
          state->PendingAppearanceAdds[state->NextPendingAppearanceAdd];
      auto itr = _appearances.find(appearanceId);
      SendAppearanceAdded(
          player, appearanceId,
          itr != _appearances.end() ? itr->second.SourceItem : 0);
    }

    if (state->NextPendingAppearanceAdd < state->PendingAppearanceAdds.size())
      return;

    std::size_t total = state->CollectedAppearances.size();
    std::vector<uint32>().swap(state->PendingAppearanceAdds);
    state->NextPendingAppearanceAdd = 0;
    state->AppearanceAddTimer = 0;
    SendOutfitCollection(player);
    ChatHandler(player->GetSession())
        .PSendSysMessage("Unlocked all {} local wardrobe appearances.", total);

    LOG_INFO("coa",
             "Completed full wardrobe sync for {}: {} appearances",
             player->GetName(), total);
  }

  std::shared_ptr<PlayerCollectionState> GetState(Player const *player) {
    std::lock_guard lock(_stateMutex);
    auto itr = _playerStates.find(player->GetGUID().GetCounter());
    return itr != _playerStates.end() ? itr->second : nullptr;
  }

  void LoadPlayerState(Player *player, PlayerCollectionState &state) {
    uint32 accountId = state.AccountId;
    uint32 characterGuid = player->GetGUID().GetCounter();

    if (QueryResult result = CharacterDatabase.Query(
            "SELECT `appearance_id` FROM `account_appearance_collection` WHERE "
            "`account_id` = {}",
            accountId)) {
      do {
        state.CollectedAppearances.insert(result->Fetch()[0].Get<uint32>());
      } while (result->NextRow());
    }

    if (QueryResult result = CharacterDatabase.Query(
            "SELECT `category_id`, `appearance_id` FROM `character_appearance` "
            "WHERE `guid` = {}",
            characterGuid)) {
      do {
        Field *fields = result->Fetch();
        uint32 categoryId = fields[0].Get<uint32>();
        if (categoryId < APPEARANCE_CATEGORY_COUNT)
          state.ActiveAppearances[categoryId] = fields[1].Get<uint32>();
      } while (result->NextRow());
    }

    if (QueryResult result = CharacterDatabase.Query(
            "SELECT `can_see_item`, `can_see_spell` FROM "
            "`character_appearance_settings` WHERE `guid` = {}",
            characterGuid)) {
      Field *fields = result->Fetch();
      state.CanSeeItemAppearances = fields[0].Get<uint8>() != 0;
      state.CanSeeSpellAppearances = fields[1].Get<uint8>() != 0;
    }

    if (QueryResult result = CharacterDatabase.Query(
            "SELECT `item_id` FROM `account_vanity_collection` WHERE "
            "`account_id` = {}",
            accountId)) {
      do {
        state.OwnedVanityItems.insert(result->Fetch()[0].Get<uint32>());
      } while (result->NextRow());
    }
  }

  void ScanPlayerInventory(Player *player, PlayerCollectionState &state) {
    for (uint8 slot = EQUIPMENT_SLOT_START; slot < INVENTORY_SLOT_ITEM_END;
         ++slot) {
      if (Item *item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
        CollectItem(player, state, item->GetEntry(), false);
    }

    for (uint8 bagSlot = INVENTORY_SLOT_BAG_START;
         bagSlot < INVENTORY_SLOT_BAG_END; ++bagSlot) {
      Bag *bag = player->GetBagByPos(bagSlot);
      if (!bag)
        continue;

      for (uint32 slot = 0; slot < bag->GetBagSize(); ++slot) {
        if (Item *item = bag->GetItemByPos(slot))
          CollectItem(player, state, item->GetEntry(), false);
      }
    }
  }

  void CollectItem(Player *player, PlayerCollectionState &state, uint32 itemId,
                   bool notifyClient) {
    auto mappingItr = _itemAppearances.find(itemId);
    if (mappingItr != _itemAppearances.end())
    {
      uint32 appearanceId = mappingItr->second;
      if (_appearances.contains(appearanceId) &&
          state.CollectedAppearances.insert(appearanceId).second) {
        CharacterDatabase.Execute(
            "INSERT IGNORE INTO `account_appearance_collection` (`account_id`, "
            "`appearance_id`, `source_item`) "
            "VALUES ({}, {}, {})",
            state.AccountId, appearanceId, itemId);

        if (notifyClient)
          SendAppearanceAdded(player, appearanceId, itemId);
      }
    }

    if (!ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::UNLOCK_ALL_VANITY) &&
        _vanityItems.contains(itemId) &&
        state.OwnedVanityItems.insert(itemId).second) {
      CharacterDatabase.Execute(
          "INSERT IGNORE INTO `account_vanity_collection` (`account_id`, "
          "`item_id`) VALUES ({}, {})",
          state.AccountId, itemId);

      if (notifyClient)
      {
        WorldPacket packet(SMSG_VANITY_COLLECTION_ADDED, sizeof(uint32));
        packet << itemId;
        player->GetSession()->SendPacket(&packet);
      }
    }

    if (IsBankVanityItem(itemId) && state.OwnedVanityItems.insert(itemId).second)
    {
      CharacterDatabase.Execute(
          "INSERT IGNORE INTO `account_vanity_collection` (`account_id`, "
          "`item_id`) VALUES ({}, {})",
          state.AccountId, itemId);

      if (notifyClient)
      {
        WorldPacket packet(SMSG_VANITY_COLLECTION_ADDED, sizeof(uint32));
        packet << itemId;
        player->GetSession()->SendPacket(&packet);
      }

      LearnOwnedBankSpells(player, state, !player->IsInWorld());
      SendOwnedVanityStoreRecords(player, state);
      LOG_INFO("coa", "Account {} acquired the bank item {} through {}",
               state.AccountId, itemId, player->GetName());
    }
  }

  void HandleClientPacket(Player *player, WorldPacket &packet) {
    packet.rpos(0);
    try {
      switch (packet.GetOpcode()) {
      case CMSG_APPLY_APPEARANCES:
        HandleApplyAppearances(player, packet);
        break;
      case CMSG_SET_CAN_SEE_APPEARANCES:
        HandleSetAppearanceVisibility(player, packet);
        break;
      case CMSG_EXTENSION_INITIALIZED:
        player->SendAllSpellChargeStates();
        SendAscensionRunemasterEchoesOwnership(player);
        LOG_DEBUG("coa", "Resent spell charge state to {} after client world entry",
                  player->GetName());
        break;
      case CMSG_ITEM_QUERY_BULK:
        for (uint32 entry : ReadBulkQueryEntries(packet, MAX_ITEM_QUERY_BULK_ENTRIES))
          player->GetSession()->SendItemQuerySingleResponse(entry);
        break;
      case CMSG_CREATURE_QUERY_BULK:
        for (uint32 entry : ReadBulkQueryEntries(packet, MAX_CREATURE_QUERY_BULK_ENTRIES))
          SendCollectionCreatureQueryResponse(player->GetSession(), entry);
        break;
      case CMSG_CUSTOM_ASCENSION_POINT_SPEND_REQUEST:
        HandlePointSpendRequest(player, packet);
        break;
      default:
        break;
      }
    } catch (ByteBufferException const &) {
      RejectClientPacket(player->GetSession()->GetAccountId(), packet, "malformed payload");
    }
  }

  void HandleApplyAppearances(Player *player, WorldPacket &packet) {
    std::shared_ptr<PlayerCollectionState> state = GetState(player);
    if (!state)
      return;

    uint32 count = 0;
    packet >> count;
    if (!count || count > 256)
    {
      SendApplyResult(player, "APPLY_APPEARANCES_INVALID_SELECTION");
      return;
    }

    std::array<uint32, APPEARANCE_CATEGORY_COUNT> requested{};
    for (uint32 index = 0; index < count; ++index) {
      uint32 appearanceId = 0;
      packet >> appearanceId;
      if (index < requested.size())
        requested[index] = appearanceId;
    }

    for (uint32 categoryId = 1; categoryId < requested.size(); ++categoryId) {
      uint32 appearanceId = requested[categoryId];
      if (!appearanceId)
        continue;

      if (!state->CollectedAppearances.contains(appearanceId))
      {
        SendApplyResult(player, "APPLY_APPEARANCES_NOT_COLLECTED");
        return;
      }

      auto appearanceItr = _appearances.find(appearanceId);
      if (appearanceItr == _appearances.end())
      {
        SendApplyResult(player, "APPLY_APPEARANCES_INVALID_SELECTION");
        return;
      }

      AppearanceInfo const &appearance = appearanceItr->second;
      if ((categoryId <= 14 || categoryId == APPEARANCE_CATEGORY_AMMUNITION ||
          IsCosmeticCategory(categoryId)) &&
          appearance.PrimaryCategory != categoryId &&
          appearance.SecondaryCategory != categoryId &&
          appearance.TertiaryCategory != categoryId) {
        SendApplyResult(player, "APPLY_APPEARANCES_INVALID_CATEGORY");
        return;
      }
      if (IsCosmeticCategory(categoryId) && !appearance.CosmeticSpell)
      {
        SendApplyResult(player, "APPLY_APPEARANCES_INVALID_SELECTION");
        return;
      }
    }

    if (requested[55] && !ExpandItemSetAppearance(requested, requested[55]))
    {
      SendApplyResult(player, "APPLY_APPEARANCES_INVALID_SELECTION");
      return;
    }

    state->ActiveAppearances = requested;
    SaveActiveAppearances(player, *state);
    RefreshCosmetics(player, *state);
    RefreshVisibleItems(player);
    SendApplyResult(player, "APPLY_APPEARANCES_OK");
  }

  void HandleSetAppearanceVisibility(Player *player, WorldPacket &packet) {
    std::shared_ptr<PlayerCollectionState> state = GetState(player);
    if (!state)
      return;

    uint8 canSeeItem = 0;
    uint8 canSeeSpell = 0;
    packet >> canSeeItem >> canSeeSpell;
    state->CanSeeItemAppearances = canSeeItem != 0;
    state->CanSeeSpellAppearances = canSeeSpell != 0;

    CharacterDatabase.Execute("REPLACE INTO `character_appearance_settings` "
                              "(`guid`, `can_see_item`, `can_see_spell`) "
                              "VALUES ({}, {}, {})",
                              player->GetGUID().GetCounter(),
                              canSeeItem ? 1 : 0, canSeeSpell ? 1 : 0);

    RefreshVisibleItems(player);
    SendAppearanceVisibility(player, *state);
  }

    void HandlePointSpendRequest(Player* player, WorldPacket& packet)
    {
        if (packet.size() != POINT_SPEND_REQUEST_SIZE)
        {
            RejectClientPacket(player->GetSession()->GetAccountId(), packet, "malformed point spend request");
            return;
        }

        uint8 currency = 0;
        uint32 itemId = 0;
        packet >> currency >> itemId;
        if (currency != VANITY_CURRENCY_DONATION_POINTS)
        {
            LOG_DEBUG("coa", "Ignored Ascension point spend request in vanity currency {} for {} from {}",
                currency, itemId, player->GetName());
            return;
        }

        DeliverLocalVanityItem(player, itemId);
    }

  void SaveActiveAppearances(Player *player,
                             PlayerCollectionState const &state) {
    uint32 characterGuid = player->GetGUID().GetCounter();
    CharacterDatabaseTransaction transaction =
        CharacterDatabase.BeginTransaction();
    transaction->Append("DELETE FROM `character_appearance` WHERE `guid` = {}",
                        characterGuid);

    for (uint32 categoryId = 1; categoryId < state.ActiveAppearances.size();
         ++categoryId) {
      uint32 appearanceId = state.ActiveAppearances[categoryId];
      if (!appearanceId)
        continue;

      transaction->Append("INSERT INTO `character_appearance` (`guid`, "
                          "`category_id`, `appearance_id`) VALUES ({}, {}, {})",
                          characterGuid, categoryId, appearanceId);
    }

    CharacterDatabase.CommitTransaction(transaction);
  }

  void SendAppearanceCollection(Player *player,
                                std::vector<uint32> const &appearances) {
    WorldPacket packet(SMSG_APPEARANCE_COLLECTION_INFO,
                       sizeof(uint32) +
                           appearances.size() * sizeof(uint32) * 2);
    packet << static_cast<uint32>(appearances.size());
    for (uint32 appearanceId : appearances) {
      packet << appearanceId;
      auto itr = _appearances.find(appearanceId);
      packet << (itr != _appearances.end() ? itr->second.SourceItem : 0);
    }

    player->GetSession()->SendPacket(&packet);
  }

  void SendActiveAppearances(Player *player,
                             PlayerCollectionState const &state) {
    WorldPacket packet(SMSG_APPEARANCE_ACTIVE_INFO,
                       sizeof(uint32) +
                           state.ActiveAppearances.size() * sizeof(uint32));
    packet << static_cast<uint32>(state.ActiveAppearances.size());
    for (uint32 appearanceId : state.ActiveAppearances)
      packet << appearanceId;

    player->GetSession()->SendPacket(&packet);
  }

  void SendAppearanceAdded(Player *player, uint32 appearanceId,
                           uint32 sourceItem) {
    WorldPacket packet(SMSG_APPEARANCE_ADDED, sizeof(uint32) * 2);
    packet << appearanceId << sourceItem;
    player->GetSession()->SendPacket(&packet);
  }

public:
  void SendRealmInfo(Player *player) {
    if (player && player->GetSession())
      SendRealmInfo(player->GetSession(), player->GetName());
  }

  void SendRealmInfo(WorldSession *session, std::string const &who = "char-select") {
    if (!session)
      return;

    std::string const art = ascensionCompatConfig.GetConfigValue<std::string>(
        AscensionCompatConfig::REALM_TYPE);

    uint8 flags[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    if (art == "seasonal")         flags[1] = 1;
    else if (art == "league")      flags[2] = 1;
    else if (art == "ptr")         flags[3] = 1;
    else if (art == "development") flags[4] = 1;
    else                           flags[0] = 1;

    std::string const model = ascensionCompatConfig.GetConfigValue<std::string>(
        AscensionCompatConfig::CLASS_MODEL);
    if (model == "coa")
      flags[REALM_CREATION_FLAG_CONQUEST_OF_AZEROTH] = 1;
    else if (model == "wcr")
      flags[REALM_CREATION_FLAG_WARCRAFT_REBORN] = 1;

    WorldPacket p(SMSG_REALM_INFO, 64);
    p << static_cast<uint32>(realm.Id.Realm);
    p << static_cast<uint32>(EXPANSION_WRATH_OF_THE_LICH_KING);
    p << 0.0f << 0.0f << 0.0f;
    p << static_cast<uint32>(0);
    p << 0.0f << 0.0f;
    p << static_cast<uint32>(0);
    for (uint8 f : flags)
      p << f;
    p << sWorld->GetRealmName();
    p << "";
    p << REALM_INFO_ADDONS_ALLOWED;

    session->SendPacket(&p);

    LOG_INFO("coa",
             "Realm info sent to {}: type {}, class model {}, realm {}.",
             who, art, model, realm.Id.Realm);
  }

private:
  void SendOutfitCollection(Player *player)
  {
    WorldPacket packet(SMSG_APPEARANCE_OUTFIT_INFO, sizeof(uint32));
    packet << static_cast<uint32>(0);
    player->GetSession()->SendPacket(&packet);
  }

  void SendAppearanceVisibility(Player *player,
                                PlayerCollectionState const &state) {
    WorldPacket packet(SMSG_CAN_SEE_APPEARANCES_INFO, 2);
    packet << static_cast<uint8>(state.CanSeeItemAppearances ? 1 : 0);
    packet << static_cast<uint8>(state.CanSeeSpellAppearances ? 1 : 0);
    player->GetSession()->SendPacket(&packet);
  }

  void SendVanityCollection(Player *player,
                            PlayerCollectionState const &state) {
    bool unlockAll = ascensionCompatConfig.GetConfigValue<bool>(
        AscensionCompatConfig::UNLOCK_ALL_VANITY);
    std::vector<uint32> vanityItems;
    if (unlockAll)
    {
      vanityItems = _allVanityItemIds;

      vanityItems.erase(
          std::remove_if(vanityItems.begin(), vanityItems.end(),
                         [](uint32 itemId) { return IsBankVanityItem(itemId); }),
          vanityItems.end());

      for (uint32 itemId : state.OwnedVanityItems)
        if (IsBankVanityItem(itemId))
          vanityItems.push_back(itemId);
    }
    else
      vanityItems.assign(state.OwnedVanityItems.begin(),
                         state.OwnedVanityItems.end());

    std::sort(vanityItems.begin(), vanityItems.end());
    vanityItems.erase(std::unique(vanityItems.begin(), vanityItems.end()), vanityItems.end());
    WorldPacket packet(SMSG_VANITY_COLLECTION_INFO,
                       sizeof(uint32) + vanityItems.size() * sizeof(uint32));
    packet << static_cast<uint32>(vanityItems.size());
    for (uint32 itemId : vanityItems)
      packet << itemId;

    player->GetSession()->SendPacket(&packet);
  }

  void SendOwnedVanityStoreRecords(Player *player,
                                   PlayerCollectionState const &state) {
    bool const unlockAll = ascensionCompatConfig.GetConfigValue<bool>(
        AscensionCompatConfig::UNLOCK_ALL_VANITY);

    std::vector<uint32> itemIds;
    if (unlockAll)
      itemIds = _allVanityItemIds;
    else
      for (uint32 itemId : state.OwnedVanityItems)
        if (_vanityItems.contains(itemId))
          itemIds.push_back(itemId);

    std::sort(itemIds.begin(), itemIds.end());
    itemIds.erase(std::unique(itemIds.begin(), itemIds.end()), itemIds.end());

    WorldPacket packet(SMSG_QUERY_CUSTOM_STORE_RESULT,
                       64 + itemIds.size() * VANITY_STORE_RECORD_DWORDS * sizeof(uint32));
    packet << "QUERY_CUSTOM_STORE_OK";
    packet << static_cast<uint32>(itemIds.size());
    for (uint32 itemId : itemIds)
      for (uint32 field : _vanityItems.at(itemId).StoreRecord)
        packet << field;

    player->GetSession()->SendPacket(&packet);

    if (!itemIds.empty())
    {
      std::string owned;
      for (uint32 itemId : itemIds) {
        if (!owned.empty())
          owned += ' ';
        owned += std::to_string(itemId);
      }

      LOG_INFO("coa",
               "Sent {} vanity store record(s) to {} for owned item(s): {}",
               itemIds.size(), player->GetName(), owned);
    }
  }

  void SendApplyResult(Player *player, char const *result) {
    WorldPacket packet(SMSG_APPLY_APPEARANCES_RESULT, std::strlen(result) + 1);
    packet << result;
    player->GetSession()->SendPacket(&packet);
  }

    void RefreshCosmetics(Player* player, PlayerCollectionState& state)
    {
        std::unordered_set<uint32> desired;
        for (uint32 category = 56; category <= 58; ++category)
        {
            uint32 id = state.ActiveAppearances[category];
            auto itr = _appearances.find(id);
            if (state.CollectedAppearances.contains(id) && itr != _appearances.end() &&
                itr->second.CosmeticSpell)
                desired.insert(itr->second.CosmeticSpell);
        }
        for (uint32 spell : state.AppliedCosmeticSpells)
            if (!desired.contains(spell))
                player->RemoveAurasDueToSpell(spell, player->GetGUID());
        state.AppliedCosmeticSpells = std::move(desired);
        if (!player->IsAlive())
            return;
        for (uint32 spell : state.AppliedCosmeticSpells)
        {
            if (!player->HasAura(spell, player->GetGUID()))
                if (Aura* aura = player->AddAura(spell, player))
                {
                    aura->SetMaxDuration(-1);
                    aura->SetDuration(-1);
                }
        }
    }

  void RefreshVisibleItems(Player *player) {
    for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
      player->SetVisibleItemSlot(
          slot, player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot));
  }

  bool ExpandItemSetAppearance(
      std::array<uint32, APPEARANCE_CATEGORY_COUNT> &activeAppearances,
      uint32 setAppearanceId) const {
    auto setAppearanceItr = _appearances.find(setAppearanceId);
    if (setAppearanceItr == _appearances.end())
      return false;

    uint32 itemSetId = setAppearanceItr->second.SourceItem;
    auto itemSetItr = _itemSetItems.find(itemSetId);
    if (itemSetItr == _itemSetItems.end())
      return false;

    bool expanded = false;
    for (uint32 itemId : itemSetItr->second) {
      auto itemAppearanceItr = _itemAppearances.find(itemId);
      if (itemAppearanceItr == _itemAppearances.end())
        continue;

      auto appearanceItr = _appearances.find(itemAppearanceItr->second);
      if (appearanceItr == _appearances.end())
        continue;

      AppearanceInfo const &appearance = appearanceItr->second;
      std::array<uint32, 3> const categories = {
          appearance.PrimaryCategory, appearance.SecondaryCategory,
          appearance.TertiaryCategory};
      auto categoryItr = std::find_if(
          categories.begin(), categories.end(), [](uint32 categoryId) {
            return categoryId >= 1 && categoryId <= 14;
          });
      if (categoryItr == categories.end())
        continue;

      activeAppearances[*categoryItr] = itemAppearanceItr->second;
      expanded = true;
    }

    return expanded;
  }

  bool _clientDataLoaded = false;
  std::unordered_map<uint32, AppearanceInfo> _appearances;
  std::unordered_map<uint32, uint32> _itemAppearances;
  std::unordered_map<uint32, std::vector<uint32>> _itemSetItems;
  std::unordered_map<uint32, VanityInfo> _vanityItems;
  std::vector<uint32> _allAppearanceIds;
  std::vector<uint32> _allVanityItemIds;

  std::mutex _packetMutex;
  std::unordered_map<uint32, std::deque<WorldPacket>> _pendingPackets;
  std::mutex _rejectedPacketMutex;
  std::unordered_map<uint32, uint32> _rejectedPackets;

  std::mutex _stateMutex;
  std::unordered_map<uint32, std::shared_ptr<PlayerCollectionState>>
      _playerStates;
};

AscensionCollectionModels::Entry const* FindCollectionModel(uint32 creatureId)
{
    auto const& models = AscensionCollectionModels::Entries;
    auto const itr = std::lower_bound(models.begin(), models.end(), creatureId,
        [](AscensionCollectionModels::Entry const& model, uint32 value)
        {
            return model.CreatureId < value;
        });
    return itr != models.end() && itr->CreatureId == creatureId ? &*itr : nullptr;
}

bool SendCollectionCreatureQueryResponse(WorldSession* session, uint32 creatureId)
{
    AscensionCollectionModels::Entry const* model = FindCollectionModel(creatureId);
    if (!session || !model)
        return false;

    WorldPacket response(SMSG_CREATURE_QUERY_RESPONSE, 100);
    response << creatureId << std::string(model->Name);
    for (uint8 i = 0; i < 5; ++i)
        response << uint8(0);
    response << uint32(0) << uint32(CREATURE_TYPE_CRITTER) << uint32(0);
    response << uint32(0) << uint32(0) << uint32(0);
    response << model->DisplayId << uint32(0) << uint32(0) << uint32(0);
    response << float(1.0f) << float(1.0f) << uint8(0);
    for (uint8 i = 0; i < 6; ++i)
        response << uint32(0);
    response << uint32(0);
    session->SendPacket(&response);
    return true;
}

enum PersonalBankKind : uint8
{
    PERSONAL_BANK_PERSONAL = 0,
    PERSONAL_BANK_REALM = 1
};

enum PersonalBankSpell : uint32
{
    SPELL_PERSONAL_BANK = 100702,
    SPELL_CELESTIAL_PERSONAL_BANK = 93416,
    SPELL_REALM_BANK = 92078
};

enum PersonalBankObject : uint32
{
    BANK_OBJECT_PERSONAL_ALLIANCE = 475001,
    BANK_OBJECT_PERSONAL_HORDE = 475002,
    BANK_OBJECT_CELESTIAL = 80782,
    BANK_OBJECT_REALM_ALLIANCE = 80159,
    BANK_OBJECT_REALM_HORDE = 80160
};

constexpr uint32 BANK_VAULT_DURATION = 10 * 60;

[[nodiscard]] uint32 BankObjectEntry(uint32 spellId, TeamId team)
{
    bool const alliance = team == TEAM_ALLIANCE;
    if (spellId == SPELL_REALM_BANK)
        return alliance ? BANK_OBJECT_REALM_ALLIANCE : BANK_OBJECT_REALM_HORDE;
    if (spellId == SPELL_CELESTIAL_PERSONAL_BANK)
        return BANK_OBJECT_CELESTIAL;
    return alliance ? BANK_OBJECT_PERSONAL_ALLIANCE : BANK_OBJECT_PERSONAL_HORDE;
}

struct PersonalBankVault
{
    ObjectGuid Owner;
    uint8 Kind = PERSONAL_BANK_PERSONAL;
};

std::unordered_map<ObjectGuid::LowType, PersonalBankVault> personalBankVaults;

void SendBankPermissions(Player* player, uint8 kind)
{
    AscensionPersonalBank::SendKindHint(player, uint8(kind));
}

[[nodiscard]] float AverageEquippedItemLevel(Player* player, uint8 emptiedSlot)
{
    float sum = 0.0f;
    uint32 count = 0;
    for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
    {
        if (slot == EQUIPMENT_SLOT_TABARD || slot == EQUIPMENT_SLOT_RANGED || slot == EQUIPMENT_SLOT_OFFHAND ||
            slot == EQUIPMENT_SLOT_BODY)
            continue;

        ++count;
        if (Item* item = slot == emptiedSlot ? nullptr : player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
            sum += item->GetTemplate()->Quality == ITEM_QUALITY_HEIRLOOM ? player->GetLevel()
                                                                        : item->GetTemplate()->ItemLevel;
    }
    return sum / count;
}

void SendAverageItemLevel(Player* player, uint8 emptiedSlot = EQUIPMENT_SLOT_END)
{
    WorldPacket data(SMSG_UPDATE_OBJECT_ADDON, 16);
    data << player->GetGUID() << PLAYER_ADDON_FIELD_AVERAGE_ITEM_LEVEL
         << AverageEquippedItemLevel(player, emptiedSlot);
    player->SendMessageToSet(&data, true);
}

[[nodiscard]] bool OwnsPlacedBank(Player* player, uint8 kind)
{
    static std::array<PersonalBankSpell, 2> const personalSpells =
        { SPELL_PERSONAL_BANK, SPELL_CELESTIAL_PERSONAL_BANK };
    static std::array<uint32, 3> const personalItems = { 110000, 134985, 509892 };

    if (kind == PERSONAL_BANK_REALM)
        return player->HasSpell(SPELL_REALM_BANK) || player->HasItemCount(1180097);

    for (PersonalBankSpell spell : personalSpells)
        if (player->HasSpell(spell))
            return true;

    for (uint32 item : personalItems)
        if (player->HasItemCount(item))
            return true;

    return false;
}

bool HandlePersonalBankActivate(Player* player, WorldPacket const& packet)
{
    ObjectGuid banker;
    bool fullUpdate = false;
    try
    {
        WorldPacket copy(packet);
        WorldPackets::Guild::GuildBankActivate activate(std::move(copy));
        activate.Read();
        banker = activate.Banker;
        fullUpdate = activate.FullUpdate;
    }
    catch (...)
    {
        return false;
    }

    auto itr = personalBankVaults.find(banker.GetCounter());
    if (itr == personalBankVaults.end())
        return false;

    if (!OwnsPlacedBank(player, itr->second.Kind))
    {
        ChatHandler(player->GetSession())
            .PSendSysMessage("You do not own a {} bank.",
                             itr->second.Kind == PERSONAL_BANK_REALM ? "Realm" : "Personal");
        LOG_INFO("coa",
                 "{} touched a {} bank placed by {} (vault {}) without owning one",
                 player->GetName(),
                 itr->second.Kind == PERSONAL_BANK_REALM ? "realm" : "personal",
                 itr->second.Owner.ToString(), banker.ToString());
        return true;
    }

    SendBankPermissions(player, itr->second.Kind);
    AscensionPersonalBank::Opened(player, itr->second.Kind, banker);

    LOG_INFO("coa",
             "Personal bank opened for {} (kind {}, vault {}, full update {})",
             player->GetName(), uint32(itr->second.Kind), banker.ToString(),
             fullUpdate);
    return true;
}

static float GroundHeightBeneath(Map* map, float x, float y, float feetZ)
{
    constexpr float PROBE_ABOVE_FEET = 0.3f;
    constexpr float STEP = 0.5f;

    float const height = map->GetHeight(x, y, feetZ + PROBE_ABOVE_FEET, true, STEP);
    if (height > INVALID_HEIGHT && height <= feetZ + STEP && height >= feetZ - STEP)
        return height;

    return feetZ;
}

class spell_ascension_personal_bank : public SpellScript
{
    PrepareSpellScript(spell_ascension_personal_bank);

    void SummonBankVault()
    {
        Player* player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
        if (!player || !player->IsInWorld())
            return;

        uint8 kind = PERSONAL_BANK_PERSONAL;
        if (GetSpellInfo()->Id == SPELL_REALM_BANK)
            kind = PERSONAL_BANK_REALM;

        uint32 const entry = BankObjectEntry(GetSpellInfo()->Id, player->GetTeamId());

        float x, y, z;
        player->GetClosePoint(x, y, z, player->GetCombatReach(), 2.0f);

        z = GroundHeightBeneath(player->GetMap(), x, y, player->GetPositionZ());

        GameObject* vault = player->GetMap()->SummonGameObject(
            entry, x, y, z, player->GetOrientation(), 0.0f, 0.0f, 0.0f,
            0.0f, BANK_VAULT_DURATION, true);
        if (vault)
            vault->SetPhaseMask(player->GetPhaseMask(), true);

        if (!vault)
        {
            LOG_ERROR("coa",
                      "Could not summon bank object {} for {} (spell {})",
                      entry, player->GetName(), GetSpellInfo()->Id);
            return;
        }

        personalBankVaults[vault->GetGUID().GetCounter()] = {player->GetGUID(), kind};

        player->AddSpellCooldown(GetSpellInfo()->Id, 0, BANK_VAULT_DURATION * IN_MILLISECONDS, true);

        LOG_INFO("coa",
                 "Summoned bank object {} (kind {}, entry {}, spell {}) for {} at "
                 "{:.2f} {:.2f} {:.2f} (caster feet {:.2f})",
                 vault->GetGUID().ToString(), uint32(kind), entry,
                 GetSpellInfo()->Id, player->GetName(), x, y, z,
                 player->GetPositionZ());
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_ascension_personal_bank::SummonBankVault);
    }
};

bool HandleAscensionGuideTrainerBuy(Player* player, ObjectGuid trainerGuid, uint32 spellId);

class AscensionCompatServerScript : public ServerScript {
public:
  AscensionCompatServerScript()
      : ServerScript("AscensionCompatServerScript",
                     {SERVERHOOK_CAN_PACKET_RECEIVE_EARLY, SERVERHOOK_CAN_PACKET_RECEIVE})
  {
  }

    [[nodiscard]] bool CanPacketReceive(WorldSession* session, WorldPacket const& packet) override
    {
        if (session && session->GetPlayer())
        {
            Player* player = session->GetPlayer();

            if (packet.GetOpcode() == CMSG_TRAINER_BUY_SPELL &&
                packet.size() >= sizeof(uint64) + sizeof(int32) &&
                ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED))
            {
                uint64 rawGuid;
                int32 spellId;
                std::memcpy(&rawGuid, packet.contents(), sizeof(rawGuid));
                std::memcpy(&spellId, packet.contents() + sizeof(rawGuid), sizeof(spellId));
                if (HandleAscensionGuideTrainerBuy(player, ObjectGuid(rawGuid), uint32(spellId)))
                    return false;
            }

            if (packet.GetOpcode() == CMSG_GUILD_BANKER_ACTIVATE)
            {
                if (HandlePersonalBankActivate(player, packet))
                    return false;
            }
            else if (AscensionPersonalBank::IsOpen(player) &&
                     AscensionPersonalBank::HandlePacket(player, packet))
                return false;
        }

        if (!session || !session->GetPlayer() ||
            !ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED))
            return true;

        if (packet.GetOpcode() == CMSG_SET_ACTIVE_MOVER)
            AscensionClassService::Instance().OnPlayerActiveMover(session->GetPlayer());

        if (packet.GetOpcode() == CMSG_GET_MIRRORIMAGE_DATA && packet.size() >= sizeof(uint64))
        {
            ObjectGuid guid = packet.read<ObjectGuid>(0);
            CreatureDisplayPreset const* preset = nullptr;

            if (guid.IsCreatureOrVehicle())
            {
                uint32 displayId = 0;
                if (Creature const* creature = session->GetPlayer()->GetMap()->GetCreature(guid))
                    displayId = creature->GetDisplayId();

                preset = sAscensionPresets->GetPreset(guid.GetEntry(), displayId);
            }

            if (!preset)
            {
                preset = sAscensionPresets->GetActivePresetOverride(guid);
            }

            if (preset)
            {
                WorldPacket response(SMSG_MIRRORIMAGE_DATA, 68);
                response << guid;
                response << uint32(preset->display_id);
                response << uint8(preset->race);
                response << uint8(preset->gender);
                response << uint8(preset->class_id);
                response << uint8(preset->skin);
                response << uint8(preset->face);
                response << uint8(preset->hair);
                response << uint8(preset->haircolor);
                response << uint8(preset->facialhair);
                response << uint32(preset->guild_id);
                for (uint32 item : preset->items)
                    response << uint32(item);

                session->SendPacket(&response);
                return false;
            }
            return true;
        }

        if (packet.GetOpcode() != CMSG_CREATURE_QUERY || packet.size() < sizeof(uint32))
            return true;

        uint32 const entry = packet.read<uint32>(0);
        if (sObjectMgr->GetCreatureTemplate(entry))
            return true;

        AscensionCollectionModels::Entry const* model = FindCollectionModel(entry);
        if (!model)
            return true;

        SendCollectionCreatureQueryResponse(session, entry);
        LOG_DEBUG("coa", "Answered local creature preview query: entry {}, display {}, payload {}",
            entry, model->DisplayId, packet.size());
        return false;
    }

  [[nodiscard]] bool CanPacketReceiveEarly(WorldSession *session,
                                           WorldPacket const &packet) override {
    if (!ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED))
      return true;

    uint32 opcode = packet.GetOpcode();

    if (opcode == CMSG_CHARACTER_ADVANCEMENT_KNOWN_ENTRIES)
    {
      if (session)
        AscensionClassService::Instance().QueueKnownEntriesUpload(session->GetAccountId(), packet);
      return false;
    }

    if (IsAscensionCharacterSelectionOpcode(static_cast<uint16>(opcode)))
    {
      if (HandleAscensionCharacterSelectionPacket(session, packet))
        return false;
    }
    else if (opcode == CMSG_CHAR_ENUM)
    {
      SendAscensionCharacterListInfo(session);
      AscensionCollectionService::Instance().SendRealmInfo(session);
    }

    uint32 firstOpcode = ascensionCompatConfig.GetConfigValue<uint32>(
        AscensionCompatConfig::FIRST_EXTENSION_OPCODE);
    uint32 lastOpcode = ascensionCompatConfig.GetConfigValue<uint32>(
        AscensionCompatConfig::LAST_EXTENSION_OPCODE);

    if (opcode < firstOpcode || opcode > lastOpcode)
      return true;

    if (opcode == CMSG_ANTICHEAT_ALERT)
      return true;

    if (opcode == CMSG_RESET_DUNGEONS)
      return true;

    if (opcode == CMSG_CREATURE_QUERY_BULK || opcode == CMSG_ITEM_QUERY_BULK)
    {
        AscensionCollectionService& service = AscensionCollectionService::Instance();
        uint32 const maxEntries = opcode == CMSG_ITEM_QUERY_BULK ? MAX_ITEM_QUERY_BULK_ENTRIES :
            MAX_CREATURE_QUERY_BULK_ENTRIES;
        if (ReadBulkQueryEntries(packet, maxEntries).empty())
            service.RejectClientPacket(session->GetAccountId(), packet, "malformed bulk query");
        else
            service.QueueClientPacket(session->GetAccountId(), packet);
        return false;
    }

    static constexpr std::array<uint32, 13> kChallengeCmsgs = {
        CMSG_COA_START_CHALLENGE,
        CMSG_COA_STOP_CHALLENGE,
        CMSG_COA_QUERY_FAILURES,
        CMSG_COA_SYNC_RESPONSE,
        CMSG_COA_QUERY_COMPLETIONS,
        CMSG_COA_SAVE_TRIAL,
        CMSG_COA_DELETE_TRIAL,
        CMSG_COA_QUERY_TRIALS,
        CMSG_COA_ACTIVATE_TRIAL,
        CMSG_COA_DEACTIVATE_TRIAL,
        CMSG_COA_RATE_TRIAL,
        CMSG_COA_QUERY_TRIAL_COMPLETIONS,
        CMSG_COA_TOGGLE_GAME_MODE,
    };
    if (std::find(kChallengeCmsgs.begin(), kChallengeCmsgs.end(), opcode) !=
        kChallengeCmsgs.end())
      return true;

    if (QueueAscensionManastormPacket(session, packet))
      return false;

    if (std::find(QUEUED_EXTENSION_OPCODES.begin(), QUEUED_EXTENSION_OPCODES.end(), opcode) !=
        QUEUED_EXTENSION_OPCODES.end())
        AscensionCollectionService::Instance().QueueClientPacket(session->GetAccountId(), packet);

    if (opcode == CMSG_MISSILE_FIRE_POSITION)
    {
      if (ascensionCompatConfig.GetConfigValue<bool>(
              AscensionCompatConfig::LOG_CONSUMED_PACKETS))
      {
        LOG_INFO("coa",
                 "Consumed Ascension missile-position packet payload={} bytes [{}]",
                 packet.size(), DescribePacketPayload(packet));
      }

      return false;
    }

    if (AscensionCompatOpcodes::Dispatch(session, packet))
      return false;

    if (ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::LOG_CONSUMED_PACKETS)) {
      char const *name = ExtensionOpcodeName(uint16(opcode));
      LOG_INFO("coa",
               "Consumed Ascension extension packet opcode=0x{:04X} ({}) "
               "payload={} bytes [{}]",
               opcode, name ? name : "unknown", packet.size(),
               DescribePacketPayload(packet));
    }

    return false;
  }
};

class AscensionCompatCommandScript : public CommandScript {
public:
  AscensionCompatCommandScript()
      : CommandScript("AscensionCompatCommandScript") {}

  ChatCommandTable GetCommands() const override {
    static ChatCommandTable spellChargesCommandTable = {
        {"reset", HandleSpellChargesResetCommand, SEC_PLAYER, Console::No},
        {"resync", HandleSpellChargesResyncCommand, SEC_PLAYER, Console::No}};

    static ChatCommandTable localTalentCommandTable = {
        {"reset", HandleLocalTalentResetCommand, SEC_PLAYER, Console::No},
        {"sync", HandleLocalTalentSyncCommand, SEC_PLAYER, Console::No},
        {"", HandleLocalTalentCommand, SEC_PLAYER, Console::No}};

    static ChatCommandTable commandTable = {
        {"localfreshcheck", HandleAscensionFreshCharacterCheck, SEC_ADMINISTRATOR, Console::Yes},
        {"localreloadpresets", HandleLocalReloadPresetsCommand, SEC_ADMINISTRATOR, Console::Yes},
        {"morphpreset", HandleMorphPresetCommand, SEC_ADMINISTRATOR, Console::No},
        {"demorphpreset", HandleDemorphPresetCommand, SEC_ADMINISTRATOR, Console::No},
        {"localreloadoutfits", HandleLocalReloadPresetsCommand, SEC_ADMINISTRATOR, Console::Yes},
        {"morphoutfit", HandleMorphPresetCommand, SEC_ADMINISTRATOR, Console::No},
        {"localappearance", HandleLocalAppearanceCommand, SEC_PLAYER,
         Console::No},
        {"localvanity", HandleLocalVanityCommand, SEC_PLAYER, Console::No},
        {"localtalent", localTalentCommandTable},
        {"localspecstate", HandleLocalTalentSyncCommand, SEC_PLAYER, Console::No},
        {"localspec", HandleLocalSpecCommand, SEC_PLAYER, Console::No},
        {"localresource", HandleLocalResourceCommand, SEC_PLAYER,
         Console::No},
        {"localcharges", HandleLocalChargesCommand, SEC_PLAYER, Console::No},
        {"localtime", HandleLocalTimeCommand, SEC_GAMEMASTER, Console::No},
        {"spellcharges", spellChargesCommandTable},
        {"localclassrepair", HandleLocalClassRepairCommand, SEC_PLAYER,
         Console::No},
        {"extprobe", HandleExtensionProbeCommand, SEC_ADMINISTRATOR,
         Console::No},
        {"bankground", HandleBankGroundCommand, SEC_ADMINISTRATOR, Console::No}};
    return commandTable;
  }

  static uint32 HexNibble(char c) {
    if (c >= '0' && c <= '9')
      return uint32(c - '0');
    if (c >= 'a' && c <= 'f')
      return uint32(c - 'a' + 10);
    if (c >= 'A' && c <= 'F')
      return uint32(c - 'A' + 10);
    return 0xFFFFFFFFu;
  }

  static bool ParseExtensionOpcode(std::string const &text, uint32 &opcode) {
    std::string body = text;
    if (body.rfind("0x", 0) == 0 || body.rfind("0X", 0) == 0)
      body = body.substr(2);

    bool allHex = !body.empty();
    for (char c : body)
      if (HexNibble(c) == 0xFFFFFFFFu)
      {
        allHex = false;
        break;
      }
    if (allHex)
    {
      opcode = 0;
      for (char c : body)
        opcode = (opcode << 4) | HexNibble(c);
      return true;
    }

    for (ExtensionOpcodeIdentity const &entry : EXTENSION_OPCODES)
      if (text == entry.Name)
      {
        opcode = entry.Opcode;
        return true;
      }
    return false;
  }

  static bool ParseHexBytes(std::string const &text, std::vector<uint8> &out) {
    std::string digits;
    for (char c : text) {
      if (c == ' ' || c == ',' || c == '\t')
        continue;
      if (HexNibble(c) == 0xFFFFFFFFu)
        return false;
      digits += c;
    }
    if (digits.empty() || digits.size() % 2 != 0)
      return false;

    for (std::size_t i = 0; i < digits.size(); i += 2)
      out.push_back(uint8((HexNibble(digits[i]) << 4) | HexNibble(digits[i + 1])));
    return true;
  }

  static bool HandleExtensionProbeCommand(ChatHandler *handler,
                                          std::string opcodeText,
                                          Optional<std::string> payloadText) {
    Player *player = handler->GetPlayer();
    if (!player)
      return false;

    uint32 opcode = 0;
    if (!ParseExtensionOpcode(opcodeText, opcode) || opcode > 0xFFFF)
    {
      handler->PSendSysMessage(
          "Unknown opcode '{}'. Give a hex id such as 0x0769, or one of the "
          "names this module knows.",
          std::string(opcodeText));
      return true;
    }

    std::vector<uint8> payload;
    if (payloadText && !ParseHexBytes(*payloadText, payload))
    {
      handler->PSendSysMessage(
          "Payload must be whole hex bytes, for example \"01 00\".");
      return true;
    }

    WorldPacket packet{static_cast<uint16>(opcode)};
    for (uint8 byte : payload)
      packet << byte;

    player->GetSession()->SendPacket(&packet);

    char const *name = ExtensionOpcodeName(uint16(opcode));
    std::string label = name ? std::string(" (") + name + ")" : std::string();
    handler->PSendSysMessage("Sent 0x{:04X}{} with {} payload bytes.",
                             opcode, label, uint32(packet.size()));
    LOG_INFO("coa",
             "Sent extension packet opcode=0x{:04X} ({}) payload={} bytes [{}] "
             "to {}",
             opcode, name ? name : "unknown", packet.size(),
             DescribePacketPayload(packet), player->GetName());
    return true;
  }

  static bool HandleBankGroundCommand(ChatHandler *handler) {
    Player *player = handler->GetPlayer();
    if (!player)
      return false;

    float x, y, z;
    player->GetClosePoint(x, y, z, player->GetCombatReach(), 2.0f);
    float const feetZ = player->GetPositionZ();
    Map *map = player->GetMap();

    handler->PSendSysMessage("feet {:.2f} on map {} at {:.2f} {:.2f}", feetZ,
                             player->GetMapId(), player->GetPositionX(),
                             player->GetPositionY());
    handler->PSendSysMessage("spot {:.2f} {:.2f} (2 yards {:.2f} rad ahead)", x,
                             y, player->GetOrientation());
    for (float start : {feetZ + 0.3f, feetZ + 2.5f, feetZ + 10.0f}) {
      handler->PSendSysMessage(
          "  probe from {:.2f}: terrain {:.2f} | terrain+vmap {:.2f}", start,
          map->GetHeight(x, y, start, false, 3.0f),
          map->GetHeight(x, y, start, true, 3.0f));
    }
    handler->PSendSysMessage("  summon would ground at {:.2f} (riser {:.2f})",
                             GroundHeightBeneath(map, x, y, feetZ),
                             GroundHeightBeneath(map, x, y, feetZ) - feetZ);
    LOG_INFO("coa",
             "Bank ground probe for {}: feet {:.2f}, spot {:.2f} {:.2f}, "
             "terrain {:.2f}, terrain+vmap {:.2f}, chosen {:.2f}",
             player->GetName(), feetZ, x, y,
             map->GetHeight(x, y, feetZ + 0.3f, false, 3.0f),
             map->GetHeight(x, y, feetZ + 0.3f, true, 3.0f),
             GroundHeightBeneath(map, x, y, feetZ));
    return true;
  }

  static bool HandleLocalAppearanceCommand(ChatHandler *handler,
                                           uint32 categoryId,
                                           uint32 appearanceId) {
    Player *player = handler->GetPlayer();
    if (!player)
      return false;

    AscensionCollectionService::Instance().ApplyLocalAppearance(
        player, categoryId, appearanceId);
    return true;
  }

  static bool HandleLocalVanityCommand(ChatHandler *handler, uint32 itemId) {
    Player *player = handler->GetPlayer();
    if (!player)
      return false;

    AscensionCollectionService::Instance().DeliverLocalVanityItem(player,
                                                                   itemId);
    return true;
  }

  static constexpr float REAL_TIME_GAME_SPEED = 0.01666667f;

  static time_t SameDayAt(time_t time, uint8 hour, uint8 minute) {
    std::tm local = Acore::Time::TimeBreakdown(time);
    local.tm_hour = hour;
    local.tm_min = minute;
    local.tm_sec = 0;
    local.tm_isdst = -1;
    return std::mktime(&local);
  }

  static bool HandleLocalTimeCommand(ChatHandler *handler, Optional<uint8> hour,
                                     Optional<uint8> minute) {
    Player *player = handler->GetPlayer();
    if (!player)
      return false;

    if ((hour && *hour > 23) || (minute && *minute > 59))
    {
      handler->SendSysMessage("Give an hour from 0 to 23 and an optional minute from 0 to 59, or no "
                              "arguments to follow the server clock again.");
      return true;
    }

    time_t const now = GameTime::GetGameTime().count();
    time_t const shown = hour ? SameDayAt(now, *hour, minute.value_or(0)) : now;
    WorldPacket data(SMSG_LOGIN_SETTIMESPEED, 4 + 4 + 4);
    data.AppendPackedTime(shown);
    data << REAL_TIME_GAME_SPEED;
    data << uint32(0);
    player->SendDirectMessage(&data);

    std::tm const clock = Acore::Time::TimeBreakdown(shown);
    handler->PSendSysMessage("Your client's clock now reads {:02}:{:02}.", clock.tm_hour, clock.tm_min);
    return true;
  }

  static bool HandleLocalTalentCommand(ChatHandler *handler, uint32 entryId,
                                       uint32 rank) {
    Player *player = handler->GetPlayer();
    if (!player)
      return false;

    AscensionCompatData::CoATalentEntry const* entry = AscensionClassService::FindTalentEntry(entryId);
    if (!entry)
    {
      handler->PSendSysMessage("Talent entry {} is not in the local CoA catalog.", entryId);
      return true;
    }

    std::string error;
    if (!AscensionClassService::Instance().SetTalentRank(player, *entry, rank, error))
    {
      handler->SendSysMessage(error);
      return true;
    }

    AscensionClassService::Instance().SendCharacterAdvancementKnownEntries(player);
    return true;
  }

  static bool HandleLocalTalentSyncCommand(ChatHandler* handler)
  {
    Player* player = handler->GetPlayer();
    if (!player)
      return false;

    if (IsAscensionCustomClass(player))
      AscensionClassService::Instance().SendCharacterAdvancementKnownEntries(player);
    return true;
  }

  static bool HandleLocalTalentResetCommand(ChatHandler* handler)
  {
    Player* player = handler->GetPlayer();
    if (!player)
      return false;

    if (!IsAscensionCustomClass(player))
    {
      handler->SendSysMessage("Only a custom class has local CoA talents to reset.");
      return true;
    }

    uint32 const removed = AscensionClassService::Instance().ResetPaidTalents(player);
    handler->PSendSysMessage("Reset {} CoA talent rank(s); every class and specialization point is available again.",
                             removed);
    AscensionClassService::Instance().SendCharacterAdvancementKnownEntries(player);
    return true;
  }

  static bool HandleLocalSpecCommand(ChatHandler *handler,
                                     uint32 specializationId) {
    Player *player = handler->GetPlayer();
    if (!player)
      return false;

    if (!AscensionClassService::Instance().SwitchSpecialization(
            player, specializationId))
      handler->PSendSysMessage(
          "Specialization {} is not valid for your custom class.",
          specializationId);
    else
      AscensionClassService::Instance().SendCharacterAdvancementKnownEntries(player);
    return true;
  }

  static bool HandleLocalClassRepairCommand(ChatHandler *handler) {
    Player *player = handler->GetPlayer();
    if (!player)
      return false;

    AscensionClassService::Instance().SynchronizeProgression(player);
    AscensionClassService::Instance().SynchronizeProficiencies(player);
    bool repaired =
        AscensionClassService::Instance().RepairStarterKit(player, true);
    if (!repaired)
      handler->SendSysMessage("Your custom-class starter kit is complete.");
    return true;
  }

  static bool HandleLocalResourceCommand(ChatHandler *handler) {
    AscensionResourceService::Instance().SendStatus(handler);
    return true;
  }

  static bool HandleLocalChargesCommand(ChatHandler* handler)
  {
    if (Player* player = handler->GetPlayer())
    {
        player->SendAllSpellChargeStates();
        SendAscensionRunemasterEchoesOwnership(player);
    }
    return true;
  }

  static bool HandleSpellChargesResetCommand(ChatHandler* handler)
  {
    Player* player = handler->GetPlayer();
    if (!player)
      return false;

    player->RestoreAllSpellCharges();
    player->SendAllSpellChargeStates();
    handler->SendSysMessage("All spell-charge pools reset to full.");
    return true;
  }

  static bool HandleSpellChargesResyncCommand(ChatHandler* handler)
  {
    Player* player = handler->GetPlayer();
    if (!player)
      return false;

    player->SendAllSpellChargeStates();
    SendAscensionRunemasterEchoesOwnership(player);
    handler->SendSysMessage("Spell-charge state resent to the client.");
    return true;
  }

  static bool HandleLocalReloadPresetsCommand(ChatHandler* handler) {
    sAscensionPresets->LoadFromDB();
    handler->PSendSysMessage("Reloaded %u creature display presets into cache.", uint32(sAscensionPresets->GetPresetCount()));
    return true;
  }

  static bool HandleMorphPresetCommand(ChatHandler* handler, uint32 entry, Optional<uint32> displayIdOpt) {
    Unit* target = handler->getSelectedUnit();
    if (!target)
      target = handler->GetPlayer();
    if (!target)
      return false;

    uint32 displayId = displayIdOpt ? *displayIdOpt : 0;
    CreatureDisplayPreset const* preset = nullptr;

    if (displayId != 0)
    {
      preset = sAscensionPresets->GetPreset(entry, displayId);
    }
    else if (Player* targetPlayer = target->ToPlayer())
    {
      preset = sAscensionPresets->GetPresetByGender(entry, targetPlayer->getGender());
    }
    else
    {
      preset = sAscensionPresets->GetPreset(entry);
    }

    if (!preset)
    {
      handler->PSendSysMessage("No creature display preset found for entry %u.", entry);
      return false;
    }

    sAscensionPresets->SetActivePresetOverride(target->GetGUID(), entry, preset->display_id);
    target->SetDisplayId(preset->display_id);
    target->SetUnitFlag2(UNIT_FLAG2_MIRROR_IMAGE);

    WorldPacket response(SMSG_MIRRORIMAGE_DATA, 68);
    response << target->GetGUID();
    response << uint32(preset->display_id);
    response << uint8(preset->race);
    response << uint8(preset->gender);
    response << uint8(preset->class_id);
    response << uint8(preset->skin);
    response << uint8(preset->face);
    response << uint8(preset->hair);
    response << uint8(preset->haircolor);
    response << uint8(preset->facialhair);
    response << uint32(preset->guild_id);
    for (uint32 item : preset->items)
      response << uint32(item);

    target->SendMessageToSet(&response, true);
    handler->PSendSysMessage("Morphed into creature display preset for entry %u (display %u, %s).",
        entry, preset->display_id, preset->gender == 1 ? "Female" : "Male");
    return true;
  }

  static bool HandleDemorphPresetCommand(ChatHandler* handler) {
    Unit* target = handler->getSelectedUnit();
    if (!target)
      target = handler->GetPlayer();
    if (!target)
      return false;

    sAscensionPresets->ClearActivePresetOverride(target->GetGUID());
    if (Player* player = target->ToPlayer())
    {
      player->RemoveUnitFlag2(UNIT_FLAG2_MIRROR_IMAGE);
      player->InitDisplayIds();
    }
    else if (Creature* creature = target->ToCreature())
    {
      if (CreatureTemplate const* cinfo = creature->GetCreatureTemplate())
      {
        if (CreatureModel const* model = ObjectMgr::ChooseDisplayId(cinfo, creature->GetCreatureData()))
        {
          creature->SetDisplayId(model->CreatureDisplayID, model->DisplayScale);
          creature->SetNativeDisplayId(model->CreatureDisplayID);
        }
      }
      if (sAscensionPresets->HasPreset(creature->GetEntry()))
      {
        creature->SetUnitFlag2(UNIT_FLAG2_MIRROR_IMAGE);
      }
      else
      {
        creature->RemoveUnitFlag2(UNIT_FLAG2_MIRROR_IMAGE);
      }
    }
    else
    {
      target->RemoveUnitFlag2(UNIT_FLAG2_MIRROR_IMAGE);
      target->DeMorph();
    }

    handler->PSendSysMessage("Demorphed creature display preset.");
    return true;
  }
};

class AscensionCompatPlayerScript : public PlayerScript {
    std::mutex _pendingEquipmentLock;
    std::unordered_map<ObjectGuid, std::vector<ObjectGuid>> _pendingEquipment;

    void EquipNewItems(Player* player)
    {
        std::vector<ObjectGuid> items;
        {
            std::lock_guard<std::mutex> lock(_pendingEquipmentLock);
            auto itr = _pendingEquipment.find(player->GetGUID());
            if (itr == _pendingEquipment.end())
                return;

            items = std::move(itr->second);
            _pendingEquipment.erase(itr);
        }

        for (ObjectGuid guid : items)
        {
            Item* item = player->GetItemByGuid(guid);
            if (!item || item->IsInTrade() || !Player::IsInventoryPos(item->GetPos()))
                continue;

            uint16 dest = 0;
            if (player->CanEquipItem(NULL_SLOT, dest, item, false) != EQUIP_ERR_OK ||
                !Player::IsEquipmentPos(dest) || player->GetItemByPos(dest))
                continue;

            Item* offhand = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
            ItemTemplate const* proto = item->GetTemplate();
            if (uint8(dest) == EQUIPMENT_SLOT_MAINHAND && proto->InventoryType == INVTYPE_2HWEAPON &&
                offhand && !player->CanTitanGrip(proto) &&
                !player->CanUseTwoHandWithShield(proto, offhand->GetTemplate()))
                continue;

            player->SwapItem(item->GetPos(), dest);
        }
    }

public:
  AscensionCompatPlayerScript()
      : PlayerScript(
            "AscensionCompatPlayerScript",
            {PLAYERHOOK_ON_LOGIN, PLAYERHOOK_ON_LOGOUT, PLAYERHOOK_ON_UPDATE,
             PLAYERHOOK_ON_AFTER_SET_VISIBLE_ITEM_SLOT, PLAYERHOOK_ON_EQUIP,
             PLAYERHOOK_ON_STORE_NEW_ITEM, PLAYERHOOK_ON_CREATE_ITEM,
             PLAYERHOOK_ON_PLAYER_IS_CLASS, PLAYERHOOK_ON_LEVEL_CHANGED,
             PLAYERHOOK_ON_LEARN_SPELL, PLAYERHOOK_ON_FORGOT_SPELL,
             PLAYERHOOK_ON_AFTER_SPEC_SLOT_CHANGED,
             PLAYERHOOK_ON_CREATE_INITIAL_ITEMS,
             PLAYERHOOK_ON_GET_AMMO_DISPLAY,
             PLAYERHOOK_ON_AFTER_UPDATE_ATTACK_POWER_AND_DAMAGE,
             PLAYERHOOK_ON_SEND_INITIAL_PACKETS_BEFORE_ADD_TO_MAP,
             PLAYERHOOK_CHECK_ITEM_IN_SLOT_AT_LOAD_INVENTORY}) {}

    void OnPlayerGetAmmoDisplay(Player* player, SpellInfo const* spellInfo,
        uint32& displayId, uint32& inventoryType) override
    {
        if (!player || !spellInfo || !spellInfo->IsAutoRepeatRangedSpell())
            return;

        Item const* weapon = player->GetWeaponForAttack(RANGED_ATTACK);
        if (!weapon)
            return;

        ItemTemplate const* item = weapon->GetTemplate();
        if (item->Class != ITEM_CLASS_WEAPON ||
            (item->SubClass != ITEM_SUBCLASS_WEAPON_BOW && item->SubClass != ITEM_SUBCLASS_WEAPON_GUN &&
             item->SubClass != ITEM_SUBCLASS_WEAPON_CROSSBOW))
            return;

        if (uint32 const appearance = AscensionCollectionService::Instance().GetAmmunitionDisplay(player))
        {
            displayId = appearance;
            inventoryType = INVTYPE_AMMO;
        }
    }

    void OnPlayerAfterUpdateAttackPowerAndDamage(Player* player, float&, float&,
        float& modifier, float&, bool ranged) override
    {
        if (ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED))
            HandleAscensionBarbarianAttackPower(player, modifier, ranged);
    }

    void OnPlayerSendInitialPacketsBeforeAddToMap(Player* player, WorldPacket&) override
    {
        if (ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED))
        {
            AscensionCollectionService::Instance().PrepareOwnedCompanionsBeforeMap(player);
            AscensionCollectionService::Instance().PrepareOwnedBankSpellsBeforeMap(player);
            AscensionClassService::Instance().PrepareTaughtAbilitiesBeforeMap(player);
        }
    }

  bool OnPlayerCreateInitialItems(Player* player, bool& handled) override
  {
    if (handled || !ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) ||
        !IsAscensionCustomClass(player))
      return true;

    handled = true;
    return AscensionClassService::Instance().InitializeLiveBaseline(player) &&
           AscensionClassService::Instance().InitializeLiveStarterKit(player);
  }

  bool OnPlayerCheckItemInSlotAtLoadInventory(Player* player, Item* item, uint8 slot,
      uint8& err, uint16& dest) override
  {
      if (!ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) ||
          slot != EQUIPMENT_SLOT_OFFHAND || player->getClass() != CLASS_SON_OF_ARUGAL)
          return true;

      uint8 result = player->CanEquipItem(slot, dest, item, false, false);
      if (result == EQUIP_ERR_OK || player->CanDualWield())
      {
          err = result;
          return false;
      }

      player->SetCanDualWield(true);
      uint16 dualWieldDest = 0;
      uint8 const dualWieldResult = player->CanEquipItem(slot, dualWieldDest, item, false, false);
      if (dualWieldResult != EQUIP_ERR_OK)
      {
          player->SetCanDualWield(false);
          err = result;
          return false;
      }

      dest = dualWieldDest;
      err = EQUIP_ERR_OK;
      return false;
  }

  static void RefreshScaledQuestQueries(Player *player) {
    if (!LocalLevelScaling::QuestEnabled.load(std::memory_order_relaxed))
      return;

    for (auto const& [questId, status] : player->getQuestStatusMap())
    {
      if (status.Status != QUEST_STATUS_INCOMPLETE &&
          status.Status != QUEST_STATUS_COMPLETE &&
          status.Status != QUEST_STATUS_FAILED)
        continue;

      if (Quest const* quest = sObjectMgr->GetQuestTemplate(questId))
        player->PlayerTalkClass->SendQuestQueryResponse(quest);
    }
  }

  void OnPlayerLogin(Player *player) override {
    if (ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED)) {
      SendAscensionCoAXpConfig(player->GetSession());
      AscensionClassService::Instance().OnPlayerLogin(player);
      RemoveLegacyQuestSpells(player);
      SynchronizeAscensionClassMechanics(player);
      AscensionResourceService::Instance().OnPlayerLogin(player);
      AscensionCollectionService::Instance().OnPlayerLogin(player);
      RefreshScaledQuestQueries(player);
      SendAverageItemLevel(player);
    }
  }

  void OnPlayerLevelChanged(Player *player, uint8) override {
    if (ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED))
    {
      SendAverageItemLevel(player);
      AscensionClassService::Instance().SynchronizeProgression(player);
      AscensionClassService::Instance().SynchronizeProficiencies(player);
      AscensionClassService::Instance().SendCharacterAdvancementKnownEntries(player);

      RefreshScaledQuestQueries(player);
    }
  }

  void OnPlayerLearnSpell(Player *player, uint32 spellId) override {
    if (ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) &&
        (spellId == 712325 || spellId == 712389 || spellId == 521211))
      SynchronizeAscensionRunemasterEchoes(player,
          AscensionClassService::Instance().GetActiveSpecialization(player));

    if (ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) &&
        AscensionClassService::Instance().AffectsTaughtAbilities(spellId))
      AscensionClassService::Instance().SynchronizeTaughtAbilities(player);

    if (ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) &&
        AscensionClassService::Instance().AffectsTalentReplacements(spellId))
      AscensionClassService::Instance().SynchronizeTalentReplacements(player);

    if (ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED) &&
        AscensionClassService::Instance().AffectsProficiencies(spellId))
      AscensionClassService::Instance().SynchronizeProficiencies(player);
  }

  void OnPlayerForgotSpell(Player *player, uint32 spellId) override {
    if (spellId == 537218)
      RemoveAscensionPrimalistWeapons(player);
    if (ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) && spellId == 712325)
      AscensionClassService::ReconcileRunemasterFists(player,
          AscensionClassService::Instance().GetActiveSpecialization(player));

    if (ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) &&
        (spellId == 712325 || spellId == 712389 || spellId == 521211))
      SynchronizeAscensionRunemasterEchoes(player,
          AscensionClassService::Instance().GetActiveSpecialization(player));

    if (ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) &&
        AscensionClassService::Instance().AffectsTaughtAbilities(spellId))
      AscensionClassService::Instance().SynchronizeTaughtAbilities(player);

    if (ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) &&
        AscensionClassService::Instance().AffectsTalentReplacements(spellId))
    {
      player->SetTemporarySpellReplacement(spellId, 0);
      AscensionClassService::Instance().SynchronizeTalentReplacements(player);
    }

    if (ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED) &&
        AscensionClassService::Instance().AffectsProficiencies(spellId))
      AscensionClassService::Instance().SynchronizeProficiencies(player);
  }

    void OnPlayerAfterSpecSlotChanged(Player* player, uint8) override
    {
        if (ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED))
        {
            AscensionClassService::ReconcileRunemasterFists(player,
                AscensionClassService::Instance().GetActiveSpecialization(player));
            AscensionClassService::Instance().SynchronizeTaughtAbilities(player);
            AscensionClassService::Instance().SynchronizeTalentReplacements(player);
            RemoveAscensionPrimalistWeapons(player);
        }
    }

  void OnPlayerLogout(Player *player) override {
    {
      std::lock_guard<std::mutex> lock(_pendingEquipmentLock);
      _pendingEquipment.erase(player->GetGUID());
    }
    AscensionClassService::Instance().OnPlayerLogout(player);
    AscensionResourceService::Instance().OnPlayerLogout(player);
    AscensionCollectionService::Instance().OnPlayerLogout(player);
  }

  void OnPlayerUpdate(Player *player, uint32 diff) override {
    if (ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED)) {
      AscensionClassService::Instance().ProcessKnownEntriesUploads(player);
      AscensionClassService::Instance().UpdateClassTuning(player, diff);
      AscensionResourceService::Instance().OnPlayerUpdate(player, diff);
      AscensionCollectionService::Instance().OnPlayerUpdate(player, diff);
      EquipNewItems(player);
      if (sAscensionPresets->GetActivePresetOverride(player->GetGUID()))
      {
        if (!player->HasUnitFlag2(UNIT_FLAG2_MIRROR_IMAGE))
          player->SetUnitFlag2(UNIT_FLAG2_MIRROR_IMAGE);
      }
    }
  }

  void OnPlayerAfterSetVisibleItemSlot(Player *player, uint8 slot,
                                       Item *item) override {
    AscensionCollectionService::Instance().OnVisibleItemSet(player, slot, item);
    if (player->IsInWorld() && ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED))
      SendAverageItemLevel(player, item ? EQUIPMENT_SLOT_END : slot);
  }

  void OnPlayerEquip(Player *player, Item *item, uint8, uint8,
                     bool) override {
    AscensionCollectionService::Instance().OnItemObtained(player, item);
  }

  void OnPlayerStoreNewItem(Player *player, Item *item,
                            uint32) override {
    AscensionCollectionService::Instance().OnItemObtained(player, item);
    if (item && player->IsInWorld() && player->getClass() >= CLASS_BARBARIAN &&
        player->getClass() <= CLASS_SPIRIT_MAGE &&
        ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED))
    {
        std::lock_guard<std::mutex> lock(_pendingEquipmentLock);
        _pendingEquipment[player->GetGUID()].push_back(item->GetGUID());
    }
  }

  void OnPlayerCreateItem(Player *player, Item *item,
                           uint32) override {
    AscensionCollectionService::Instance().OnItemObtained(player, item);
  }

  Optional<bool> OnPlayerIsClass(Player const *player, Classes playerClass,
                                 ClassContext context) override {
    if (!ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED))
      return std::nullopt;

    Classes actualClass = Classes(player->getClass());
    if (actualClass == playerClass)
      return true;

    if (actualClass < CLASS_BARBARIAN || actualClass > CLASS_SPIRIT_MAGE)
      return std::nullopt;

    if (context == CLASS_CONTEXT_PET && playerClass == CLASS_HUNTER &&
        HasAscensionPrimalistHunterPetContext(player))
      return true;

    switch (context) {
    case CLASS_CONTEXT_STATS:
    case CLASS_CONTEXT_SKILL:
    case CLASS_CONTEXT_EQUIP_RELIC:
    case CLASS_CONTEXT_EQUIP_SHIELDS:
    case CLASS_CONTEXT_EQUIP_ARMOR_CLASS:
    case CLASS_CONTEXT_WEAPON_SWAP:
      return GetLegacyClassForCustomClass(actualClass) == playerClass;
    default:
      return std::nullopt;
    }
  }
};

class AscensionCompatAllSpellScript : public AllSpellScript
{
public:
    AscensionCompatAllSpellScript()
        : AllSpellScript("AscensionCompatAllSpellScript",
              {ALLSPELLHOOK_ON_SPELL_CHECK_CAST, ALLSPELLHOOK_CAN_PREPARE,
                  ALLSPELLHOOK_ON_CAST, ALLSPELLHOOK_ON_BEFORE_EFFECTS,
                  ALLSPELLHOOK_ON_CALCULATED_TARGET,
                  ALLSPELLHOOK_ON_HIT_RESULT,
                  ALLSPELLHOOK_ON_SUCCESSFUL_INTERRUPT})
    {
    }

    void OnSpellCheckCast(Spell* spell, bool,
        SpellCastResult& result) override
    {
        if (ascensionCompatConfig.GetConfigValue<bool>(
                AscensionCompatConfig::ENABLED))
            AscensionResourceService::Instance().CheckCast(spell, result);
    }

    [[nodiscard]] bool CanPrepare(Spell* spell,
        SpellCastTargets const*,
        AuraEffect const*) override
    {
        if (!ascensionCompatConfig.GetConfigValue<bool>(
                AscensionCompatConfig::ENABLED))
            return true;

        return CanPrepareAscensionClassMechanics19To25(spell) &&
            AscensionResourceService::Instance().CanPrepare(spell);
    }

    void OnSpellCast(Spell* spell, Unit*,
        SpellInfo const*, bool) override
    {
        if (ascensionCompatConfig.GetConfigValue<bool>(
                AscensionCompatConfig::ENABLED))
        {
            AscensionResourceService::Instance().OnSpellCast(spell);
            HandleAscensionClassMechanicsCast(spell);
        }
    }

    void OnSpellBeforeEffects(Spell* spell, Unit*,
        SpellInfo const*) override
    {
        if (ascensionCompatConfig.GetConfigValue<bool>(
                AscensionCompatConfig::ENABLED))
        {
            PrepareAscensionClassMechanicsCast(spell);
            PrepareAscensionBarbarianScaling(spell);
        }
    }

    void OnSpellCalculatedTarget(Spell* spell, Unit* target,
        TargetInfo& targetInfo) override
    {
        if (!ascensionCompatConfig.GetConfigValue<bool>(
                AscensionCompatConfig::ENABLED) || !spell)
            return;

        if (Player* player = spell->GetCaster()->ToPlayer())
            HandleAscensionClassMechanicsCalculatedTarget(spell, player, target,
                targetInfo);
    }

    void OnSpellHitResult(Spell* spell, Unit* target, uint8 missInfo,
        uint32 damage, uint32 healing, bool critical) override
    {
        if (!ascensionCompatConfig.GetConfigValue<bool>(
                AscensionCompatConfig::ENABLED) || !spell)
            return;

        if (Player* player = spell->GetCaster()->ToPlayer())
        {
            AscensionResourceService::Instance().OnSpellHitResult(spell,
                target, missInfo, damage, critical);
            HandleAscensionClassMechanicsHit(spell, player, target, missInfo,
                damage, healing, critical);
            HandleAscensionReaperSoulStrikeHit(spell, player, target, missInfo);
            HandleAscensionReaperPainmailHit(spell, player, target, missInfo, damage);
        }
    }

    void OnSpellSuccessfulInterrupt(Spell* spell, Unit*) override
    {
        if (!ascensionCompatConfig.GetConfigValue<bool>(
                AscensionCompatConfig::ENABLED) || !spell)
            return;

        if (Player* player = spell->GetCaster()->ToPlayer())
            HandleAscensionClassMechanics26To32SuccessfulInterrupt(spell,
                player);
    }
};

class AscensionCompatUnitScript : public UnitScript {
public:
  AscensionCompatUnitScript()
      : UnitScript("AscensionCompatUnitScript", true,
            {UNITHOOK_ON_BLOCK,
             UNITHOOK_ON_PERIODIC_DAMAGE_RESULT,
             UNITHOOK_ON_AURA_APPLY, UNITHOOK_ON_AURA_REMOVE,
             UNITHOOK_ON_SEND_AURA_UPDATE}) {}

    void OnSendAuraUpdate(Unit* target, Player* receiver,
        AuraApplication const* application, bool remove) override
    {
        if (ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED))
            SendAscensionAuraAmounts(target, receiver, application, remove);
    }

  void OnBlock(Unit *victim, Unit *) override {
    if (!ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED) ||
        !victim || !victim->IsPlayer())
      return;

    Player* player = victim->ToPlayer();
    AscensionResourceService::Instance().OnBlock(player);
    HandleAscensionClassMechanicsBlock(player);
  }

  void OnPeriodicDamageResult(Unit* target, Unit* attacker,
      uint32 damage, SpellInfo const* spellInfo) override {
    if (!ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED))
      return;

    AscensionResourceService::Instance().OnPeriodicDamageTick(target,
        attacker, damage, spellInfo);
  }

  void OnAuraApply(Unit* unit, Aura* aura) override {
    if (!ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED) ||
        !unit || !unit->IsPlayer() || !aura)
      return;

    HandleAscensionClassMechanicsAuraApply(unit->ToPlayer(), aura->GetId());
  }

  void OnAuraRemove(Unit* unit, AuraApplication* aurApp,
                    AuraRemoveMode mode) override {
    if (!ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED) ||
        !unit || !unit->IsPlayer() || !aurApp || !aurApp->GetBase())
      return;

    HandleAscensionClassMechanicsAuraRemove(unit->ToPlayer(),
        aurApp->GetBase()->GetId(), mode == AURA_REMOVE_BY_DEATH);
  }
};

void ApplyAscensionExperienceContracts(SpellInfo* info)
{
    if (!info)
        return;

    switch (info->Id)
    {
        case 57353:
        case 71354:
        case 157353:
        case 818046:
        case 819046:
            for (SpellEffectInfo& effect : info->Effects)
                if (effect.ApplyAuraName == SPELL_AURA_MOD_XP_PCT &&
                    (effect.MiscValue == 2 || effect.MiscValue == 8))
                    effect.ApplyAuraName = SPELL_AURA_MOD_XP_QUEST_PCT;
            break;
        case 818059:
        {
            SpellEffectInfo& kills = info->Effects[EFFECT_1];
            SpellEffectInfo& quests = info->Effects[EFFECT_2];
            if (kills.Effect != SPELL_EFFECT_APPLY_AREA_AURA_PARTY ||
                kills.ApplyAuraName != SPELL_AURA_MOD_XP_PCT || kills.MiscValue != 63 || quests.Effect)
                break;
            kills.BasePoints = 49;
            kills.DieSides = 1;
            quests.Effect = kills.Effect;
            quests.ApplyAuraName = SPELL_AURA_MOD_XP_QUEST_PCT;
            quests.BasePoints = kills.BasePoints;
            quests.DieSides = kills.DieSides;
            quests.TargetA = kills.TargetA;
            quests.TargetB = kills.TargetB;
            quests.RadiusEntry = kills.RadiusEntry;
            break;
        }
        default:
            break;
    }
}

class AscensionCompatChangelogScript : public GlobalScript
{
public:
    AscensionCompatChangelogScript()
        : GlobalScript("AscensionCompatChangelogScript", {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* spellInfo) override
    {
        if (ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED))
        {
            ApplyAscensionChangelogSpellChanges(spellInfo);
            ApplyAscensionExperienceContracts(spellInfo);
            switch (spellInfo->Id)
            {
                case 19743:
                case 19782:
                    if (spellInfo->Effects[EFFECT_0].Effect == SPELL_EFFECT_APPLY_AURA &&
                        spellInfo->Effects[EFFECT_0].ApplyAuraName == SPELL_AURA_MOD_STAT &&
                        spellInfo->Effects[EFFECT_0].MiscValue == STAT_SPIRIT)
                        spellInfo->Effects[EFFECT_0].MiscValue = STAT_STAMINA;
                    break;
                case 83328: case 83329: case 83330: case 83331: case 83332:
                case 83334: case 83335: case 83336: case 103921:
                    if (spellInfo->Effects[EFFECT_0].Effect == SPELL_EFFECT_APPLY_AURA &&
                        spellInfo->Effects[EFFECT_0].ApplyAuraName == SPELL_AURA_NONE)
                        spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                    break;
                case 91616: case 91617: case 91618: case 91619:
                    if (spellInfo->Effects[EFFECT_0].ApplyAuraName == SPELL_AURA_MOUNTED &&
                        spellInfo->Effects[EFFECT_1].ApplyAuraName == SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED &&
                        spellInfo->Effects[EFFECT_2].Effect == SPELL_EFFECT_APPLY_AURA &&
                        spellInfo->Effects[EFFECT_2].ApplyAuraName == SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED &&
                        !spellInfo->HasAttribute(SPELL_ATTR4_ONLY_FLYING_AREAS))
                    {
                        spellInfo->Effects[EFFECT_2].Effect = 0;
                        spellInfo->Effects[EFFECT_2].ApplyAuraName = SPELL_AURA_NONE;
                        spellInfo->Effects[EFFECT_2].BasePoints = 0;
                    }
                    break;
                default:
                    break;
            }
            ApplyAscensionClassMechanics(spellInfo);
            ApplyAscensionPrimalistEarthquakeContract(spellInfo);
            ApplyAscensionPrimalistEarthshapingContracts(spellInfo);
            ApplyAscensionPrimalistSpiritBeastContract(spellInfo);
            ApplyAscensionPrimalistWeaponsContract(spellInfo);
            ApplyAscensionRunemasterTalentContracts(spellInfo);
            ApplyAscensionManuscriptionContracts(spellInfo);
            ApplyAscensionRunemasterTravelContracts(spellInfo);
            ApplyAscensionRangerTalentContracts(spellInfo);
            ApplyAscensionChronomancerTalentContracts(spellInfo);
            ApplyAscensionVenomancerCatalystContract(spellInfo);
            ApplyAscensionReaperDeathwindContracts(spellInfo);
            ApplyAscensionReaperScytheRushContracts(spellInfo);
        }
    }
};

namespace
{
struct LevelScalingState
{
  uint8 Original;
  uint32 Timer;
};

std::mutex g_levelScalingLock;
std::unordered_map<uint64, LevelScalingState> g_levelScalingStates;

std::unordered_map<uint64, uint8> g_levelScalingPendingEngager;

bool CanScaleCreature(Creature const* creature)
{
  if (LocalLevelScaling::CreatureScalingOwnedPerViewer.load(std::memory_order_relaxed))
    return false;

  return LocalLevelScaling::CreatureEnabled.load(std::memory_order_relaxed) && creature &&
      !creature->GetMap()->IsScriptedPrivateInstance() &&
      !creature->IsPet() && !creature->IsTotem() && !creature->IsTrigger() && !creature->IsCritter() &&
      creature->GetCreatureType() != CREATURE_TYPE_NON_COMBAT_PET && !creature->GetCharmerOrOwner() &&
      !LocalLevelScaling::IsUnscaledFixture(creature->GetPhaseMask(), creature->GetGUID().GetRawValue());
}
}

class AscensionCompatLevelScalingScript : public AllCreatureScript
{
public:
  AscensionCompatLevelScalingScript()
      : AllCreatureScript("AscensionCompatLevelScalingScript") {}

  void OnBeforeCreatureSelectLevel(CreatureTemplate const*,
                                   Creature* creature, uint8& level) override
  {
    if (!CanScaleCreature(creature))
      return;

    uint64 guid = creature->GetGUID().GetRawValue();
    uint8 original = level;
    {
      std::lock_guard<std::mutex> guard(g_levelScalingLock);
      auto [itr, inserted] = g_levelScalingStates.try_emplace(guid, LevelScalingState{level, 1000});
      original = itr->second.Original;
      if (inserted)
        itr->second.Original = level;
    }

    level = DesiredLevel(creature, original);
  }

  void OnAllCreatureUpdate(Creature* creature, uint32 diff) override
  {
    if (!CanScaleCreature(creature) || creature->IsInCombat() || !creature->IsAlive() ||
        creature->GetHealth() != creature->GetMaxHealth())
      return;

    uint64 guid = creature->GetGUID().GetRawValue();
    uint8 original;
    {
      std::lock_guard<std::mutex> guard(g_levelScalingLock);
      LevelScalingState& state =
          g_levelScalingStates.try_emplace(guid, LevelScalingState{creature->GetLevel(), 1000}).first->second;
      if (state.Timer > diff)
      {
        state.Timer -= diff;
        return;
      }
      state.Timer = 1000;
      original = state.Original;
    }

    if (DesiredLevel(creature, original) == creature->GetLevel())
      return;

    creature->SelectLevel();
    if (CreatureTemplate const* creatureTemplate = creature->GetCreatureTemplate())
    {
      CreatureBaseStats const* stats = sObjectMgr->GetCreatureBaseStats(
          creature->GetLevel(), creatureTemplate->unit_class);
      creature->SetStatFlatModifier(UNIT_MOD_ARMOR, BASE_VALUE, stats->GenerateArmor(creatureTemplate));
    }
  }

  void OnCreatureRemoveWorld(Creature* creature) override
  {
    LocalLevelScaling::ForgetFixture(creature->GetGUID().GetRawValue());
    std::lock_guard<std::mutex> guard(g_levelScalingLock);
    g_levelScalingStates.erase(creature->GetGUID().GetRawValue());
    g_levelScalingPendingEngager.erase(creature->GetGUID().GetRawValue());
  }

  static uint8 DesiredLevel(Creature const* creature, uint8 original)
  {
    Map* map = creature->GetMap();
    if (!map)
      return original;

    bool const useNearestPlayer = LocalLevelScaling::CreatureMaxLift.load(std::memory_order_relaxed) != 0;
    uint8 desired = original;
    float range = creature->GetSightRange();
    float meilleure = -1.0f;
    for (auto const& reference : map->GetPlayers())
    {
        Player* player = reference.GetSource();
        if (!player || !player->IsAlive() || player->IsGameMaster() ||
            !creature->InSamePhase(player) || !creature->IsWithinDistInMap(player, range) ||
            !player->IsValidAttackTarget(creature))
            continue;
        float distance = creature->GetExactDist(player);
        if (useNearestPlayer && meilleure >= 0.0f && distance >= meilleure)
            continue;
        meilleure = distance;
        uint8 const scaledLevel = LocalLevelScaling::ScaleCreatureLevel(original, player->GetLevel(),
            LocalLevelScaling::CreatureOffset.load(std::memory_order_relaxed));
        desired = useNearestPlayer ? scaledLevel : std::max(desired, scaledLevel);
    }

    std::lock_guard<std::mutex> guard(g_levelScalingLock);
    if (auto itr = g_levelScalingPendingEngager.find(creature->GetGUID().GetRawValue());
        itr != g_levelScalingPendingEngager.end())
    {
      uint8 const scaledLevel = LocalLevelScaling::ScaleCreatureLevel(original, itr->second,
          LocalLevelScaling::CreatureOffset.load(std::memory_order_relaxed));
      desired = useNearestPlayer ? scaledLevel : std::max(desired, scaledLevel);
      g_levelScalingPendingEngager.erase(itr);
    }
    return desired;
  }
};

class AscensionCompatLevelScalingEngageScript : public UnitScript
{
public:
    AscensionCompatLevelScalingEngageScript()
        : UnitScript("AscensionCompatLevelScalingEngageScript", true,
            {UNITHOOK_ON_UNIT_ENTER_COMBAT, UNITHOOK_ON_DAMAGE}) { }

    void OnUnitEnterCombat(Unit* unit, Unit* victim) override
    {
        ScaleForEngager(unit ? unit->ToCreature() : nullptr, victim);
    }

    void OnDamage(Unit* attacker, Unit* victim, uint32& damage) override
    {
        Creature* creature = victim ? victim->ToCreature() : nullptr;
        if (damage && attacker != victim && creature && !creature->IsEngaged())
            ScaleForEngager(creature, attacker);
    }

private:
    static void ScaleForEngager(Creature* creature, Unit* engager)
    {
        if (!CanScaleCreature(creature) || !engager || !creature->IsAlive() ||
            creature->GetHealth() != creature->GetMaxHealth())
            return;

        Player* player = engager->GetCharmerOrOwnerPlayerOrPlayerItself();
        if (!player || !player->IsAlive() || player->IsGameMaster())
            return;

        {
            std::lock_guard<std::mutex> guard(g_levelScalingLock);
            g_levelScalingPendingEngager[creature->GetGUID().GetRawValue()] = player->GetLevel();
        }

        creature->SelectLevel();
        if (CreatureTemplate const* creatureTemplate = creature->GetCreatureTemplate())
        {
            CreatureBaseStats const* stats = sObjectMgr->GetCreatureBaseStats(
                creature->GetLevel(), creatureTemplate->unit_class);
            creature->SetStatFlatModifier(UNIT_MOD_ARMOR, BASE_VALUE, stats->GenerateArmor(creatureTemplate));
        }
    }
};

class AscensionCompatWorldScript : public WorldScript {
public:
  AscensionCompatWorldScript()
      : WorldScript("AscensionCompatWorldScript",
                    {WORLDHOOK_ON_BEFORE_CONFIG_LOAD, WORLDHOOK_ON_AFTER_CONFIG_LOAD, WORLDHOOK_ON_STARTUP,
                     WORLDHOOK_ON_LOAD_CUSTOM_DATABASE_TABLE}) {}

  void OnBeforeConfigLoad(bool reload) override {
    ascensionCompatConfig.Initialize(reload);
    bool enabled = ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED);
    LocalLevelScaling::CreatureEnabled.store(enabled && ascensionCompatConfig.GetConfigValue<bool>(
        AscensionCompatConfig::LEVEL_SCALING), std::memory_order_relaxed);
    LocalLevelScaling::QuestEnabled.store(enabled && ascensionCompatConfig.GetConfigValue<bool>(
        AscensionCompatConfig::QUEST_LEVEL_SCALING), std::memory_order_relaxed);

    uint32 lift = sConfigMgr->GetOption<uint32>("CoA.LevelScalingMaxLift", 5);
    LocalLevelScaling::CreatureMaxLift.store(
        static_cast<std::uint8_t>(std::min<uint32>(lift, 255)), std::memory_order_relaxed);
  }

  void OnAfterConfigLoad(bool reload) override {
    ReportRenamedConfiguration();
    if (!reload || !ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED))
      return;

    for (auto const& [accountId, session] : sWorldSessionMgr->GetAllSessions())
      if (session && session->GetPlayer() && session->GetPlayer()->IsInWorld())
        SendAscensionCoAXpConfig(session);
  }

  void OnLoadCustomDatabaseTable() override {
    if (!ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED))
      return;

    sAscensionPresets->LoadFromDB();
  }

  void OnStartup() override {
    ReportLegacyItemTemplateTable();
    AscensionCompatData::LoadCoATalentData();
    if (!ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED))
      return;

    uint32 firstOpcode = ascensionCompatConfig.GetConfigValue<uint32>(
        AscensionCompatConfig::FIRST_EXTENSION_OPCODE);
    uint32 lastOpcode = ascensionCompatConfig.GetConfigValue<uint32>(
        AscensionCompatConfig::LAST_EXTENSION_OPCODE);
    bool dataLoaded =
        AscensionCollectionService::Instance().LoadClientData();
    AscensionResourceService::Instance().ValidateDefinitions();
    LOG_INFO("coa",
             "Ascension compatibility enabled; consuming extension opcodes "
             "0x{:04X}-0x{:04X}; collection data {}",
             firstOpcode, lastOpcode, dataLoaded ? "ready" : "unavailable");
  }

private:
  static char** EnvironmentVariables() {
#ifdef _WIN32
    return _environ;
#else
    return environ;
#endif
  }

  static void ReportRenamedConfiguration() {
    std::filesystem::path const modules = std::filesystem::path(sConfigMgr->GetConfigPath()) / "modules";
    std::error_code error;
    if (std::filesystem::exists(modules / "mod_ascension_compat.conf", error))
      LOG_ERROR("coa", "{} is no longer read: move its settings into coa.conf as CoA.* keys, then delete it",
                (modules / "mod_ascension_compat.conf").generic_string());
    if (std::filesystem::exists(modules / "mod_ascension_compat.conf.dist", error))
      LOG_ERROR("coa", "{} is obsolete: delete it, or acore.sh copies it back to mod_ascension_compat.conf",
                (modules / "mod_ascension_compat.conf.dist").generic_string());

    constexpr std::string_view legacyPrefix = "AscensionCompat.";
    for (std::string const& key : sConfigMgr->GetKeysByString(std::string(legacyPrefix)))
      LOG_ERROR("coa", "Config key {} is no longer read: rename it to CoA.{}", key, key.substr(legacyPrefix.size()));

    constexpr std::string_view legacyEnvironmentPrefix = "AC_ASCENSION_COMPAT_";
    for (char** entry = EnvironmentVariables(); entry && *entry; ++entry) {
      std::string_view const variable(*entry);
      if (!variable.starts_with(legacyEnvironmentPrefix))
        continue;
      std::string_view const name = variable.substr(0, variable.find('='));
      LOG_ERROR("coa", "Environment variable {} is no longer read: rename it to AC_CO_A_{}", name,
                name.substr(legacyEnvironmentPrefix.size()));
    }

    constexpr std::array<std::pair<std::string_view, std::string_view>, 3> legacyLoggers = {{
        {"Logger.module.ascension_compat", "Logger.coa"},
        {"Logger.module.gameplay_test", "Logger.coa.gameplay_test"},
        {"Logger.module.highrisk", "Logger.coa.highrisk"}}};
    for (auto const& [legacy, current] : legacyLoggers)
      if (!sConfigMgr->GetKeysByString(std::string(legacy)).empty())
        LOG_ERROR("coa", "{} is no longer used: rename it to {}", legacy, current);
    if (sConfigMgr->GetKeysByString("Logger.coa").empty() && !sConfigMgr->GetKeysByString("Logger.module").empty())
      LOG_ERROR("coa", "worldserver.conf has no Logger.coa line, so CoA logs only errors: "
                "add Logger.coa=4,Console Server");
  }

  static void ReportLegacyItemTemplateTable() {
    auto tableExists = [](std::string_view table) {
      return WorldDatabase.Query("SELECT 1 FROM information_schema.TABLES WHERE TABLE_SCHEMA = DATABASE() "
                                 "AND TABLE_NAME = '{}'", table) != nullptr;
    };
    if (!tableExists("item_template_ascension_compat"))
      return;
    if (tableExists("item_template_coa"))
      LOG_ERROR("coa", "World table item_template_ascension_compat exists again next to item_template_coa, as "
                "happens when its creating migration is re-applied: run INSERT IGNORE INTO item_template_coa "
                "SELECT * FROM item_template_ascension_compat, then DROP TABLE item_template_ascension_compat");
    else
      LOG_ERROR("coa", "World table item_template_ascension_compat has not been renamed: apply the pending "
                "world update rev_20260923_00_coa_item_template_table.sql");
  }
};

struct ScrollProfession
{
    uint32 skillId;
    char const* name;
};

constexpr ScrollProfession kProfessions[] = {
    { 171, "Alchemy" },        { 164, "Blacksmithing" }, { 333, "Enchanting" },
    { 202, "Engineering" },    { 165, "Leatherworking" }, { 197, "Tailoring" },
    { 182, "Herbalism" },      { 186, "Mining" },        { 393, "Skinning" },
    { 185, "Cooking" },        { 129, "First Aid" },     { 356, "Fishing" },
    { 633, "Lockpicking" },    { 732, "Woodcutting" },   { 757, "Woodworking" },
};

constexpr uint32 kGossipTextId = 1;
constexpr uint32 kSenderScroll = 0xA5C0;

class AscensionTradesmanScroll : public ItemScript
{
public:
    AscensionTradesmanScroll() : ItemScript("ascension_tradesman_scroll") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const&) override
    {
        if (!player || !item)
            return false;

        ClearGossipMenuFor(player);
        for (uint32 i = 0; i < std::extent<decltype(kProfessions)>::value; ++i)
        {
            ScrollProfession const& prof = kProfessions[i];
            std::string label = prof.name;
            if (!player->HasSkill(prof.skillId))
                label += " (not learned)";
            else if (player->GetSkillValue(prof.skillId) >= player->GetPureMaxSkillValue(prof.skillId))
                label += " (already maxed)";

            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, label, kSenderScroll, i);
        }

        SendGossipMenuFor(player, kGossipTextId, item->GetGUID());
        return true;
    }

    void OnGossipSelect(Player* player, Item* item, uint32 sender, uint32 action) override
    {
        if (!player || !item || sender != kSenderScroll)
            return;

        CloseGossipMenuFor(player);

        if (action >= std::extent<decltype(kProfessions)>::value)
            return;

        ScrollProfession const& prof = kProfessions[action];

        if (!player->HasSkill(prof.skillId))
        {
            ChatHandler(player->GetSession()).PSendSysMessage(
                "You must learn {} before the scroll can raise it.", prof.name);
            return;
        }

        uint16 const cap = player->GetPureMaxSkillValue(prof.skillId);
        if (!cap)
            return;

        if (player->GetSkillValue(prof.skillId) >= cap)
        {
            ChatHandler(player->GetSession()).PSendSysMessage(
                "Your {} is already at its maximum of {}.", prof.name, cap);
            return;
        }

        player->SetSkill(prof.skillId, player->GetSkillStep(prof.skillId), cap, cap);
        ChatHandler(player->GetSession()).PSendSysMessage(
            "{} raised to {}.", prof.name, cap);

        LOG_DEBUG("coa",
                  "Tradesman's Scroll: player {} set {} to {}",
                  player->GetName(), prof.name, cap);

        player->DestroyItemCount(item->GetEntry(), 1, true);
    }
};

class spell_ascension_legacy_quest_reward : public SpellScript
{
    PrepareSpellScript(spell_ascension_legacy_quest_reward);

    bool Validate(SpellInfo const* info) override
    {
        LegacyQuestReward const* reward = GetLegacyQuestReward(info->Id);
        if (!reward)
            return false;

        for (uint8 index = 0; index < MAX_SPELL_EFFECTS; ++index)
            if (reward->Spells[index] && (info->Effects[index].Effect != SPELL_EFFECT_LEARN_SPELL ||
                info->Effects[index].TriggerSpell != reward->Spells[index]))
                return false;
        return true;
    }

    bool Load() override
    {
        return ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED);
    }

    void HandleLearn(SpellEffIndex effect)
    {
        LegacyQuestReward const* reward = GetLegacyQuestReward(GetSpellInfo()->Id);
        if (!reward || !reward->Spells[effect])
            return;

        if (Player* player = GetHitPlayer())
            if (IsAscensionCustomClass(player))
                PreventHitDefaultEffect(effect);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_ascension_legacy_quest_reward::HandleLearn,
            EFFECT_ALL, SPELL_EFFECT_LEARN_SPELL);
    }
};

constexpr uint32 kDarionIcecrownEntry = 37120;
constexpr uint32 kShadowmourneFinalQuest = 24549;
constexpr uint32 kItemShadowmourne = 49623;
constexpr uint32 kItemFrostmourne = 33350;
constexpr uint32 kSenderRuneblade = 0xA5C1;
constexpr uint32 kActionReforge = 1;
constexpr uint32 kRunebladeMenuId = 0xA5C1;

constexpr uint32 kSenderGuideTrainer = 0xA5C2;
constexpr uint32 kGuideMenuId = 0xA5C2;
constexpr uint32 kGuideGossipTextId = 1;
constexpr uint32 kClassTrainerBase = 9100000;
constexpr uint32 kProfessionTrainerId = 200001;
constexpr uint32 kActionRestoreAbilities = 0xA5C3;

class AscensionGuideTrainer : public AllCreatureScript
{
public:
    AscensionGuideTrainer() : AllCreatureScript("AscensionGuideTrainer") { _instance = this; }

    static AscensionGuideTrainer* Instance() { return _instance; }

    bool CanCreatureGossipHello(Player* player, Creature* creature) override
    {
        if (!IsGuideCompanion(player, creature))
            return false;

        ClearGossipMenuFor(player);
        AddGossipItemFor(player, GOSSIP_ICON_TRAINER,
            "Train my class abilities.", kSenderGuideTrainer, ClassTrainerAction(player));

        if (sObjectMgr->GetTrainerById(kProfessionTrainerId))
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Train professions.",
                kSenderGuideTrainer, kProfessionTrainerId);

        if (IsAscensionCustomClass(player))
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Restore my available class abilities.",
                kSenderGuideTrainer, kActionRestoreAbilities);

        player->PlayerTalkClass->GetGossipMenu().SetMenuId(kGuideMenuId);
        SendGossipMenuFor(player, kGuideGossipTextId, creature);
        return true;
    }

    bool CanCreatureGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
    {
        if (sender != kSenderGuideTrainer || !IsGuideCompanion(player, creature))
            return false;

        if (action == kActionRestoreAbilities)
        {
            CloseGossipMenuFor(player);
            if (!AscensionClassService::Instance().SynchronizeProgression(player))
                ChatHandler(player->GetSession()).SendSysMessage(
                    "Your available class abilities are already up to date.");
            return true;
        }

        Trainer::Trainer* trainer = sObjectMgr->GetTrainerById(action);
        if (!trainer)
        {
            CloseGossipMenuFor(player);
            return true;
        }

        _selectedTrainers[player->GetGUID().GetCounter()] = action;
        ClearGossipMenuFor(player);
        trainer->SendSpells(creature, player, player->GetSession()->GetSessionDbLocaleIndex());
        return true;
    }

    bool HandleTrainerBuy(Player* player, ObjectGuid trainerGuid, uint32 spellId)
    {
        auto itr = _selectedTrainers.find(player->GetGUID().GetCounter());
        if (itr == _selectedTrainers.end())
            return false;

        Creature* creature = player->GetMap()->GetCreature(trainerGuid);
        if (!IsGuideCompanion(player, creature))
            return false;

        Trainer::Trainer* trainer = sObjectMgr->GetTrainerById(itr->second);
        if (!trainer)
            return false;

        trainer->TeachSpell(creature, player, spellId);
        return true;
    }

private:
    static inline AscensionGuideTrainer* _instance = nullptr;

    static uint32 ClassTrainerAction(Player const* player)
    {
        return kClassTrainerBase + player->getClass();
    }

    static bool IsGuideCompanion(Player const* player, Creature const* creature)
    {
        return player && creature &&
            creature->GetGUID() == player->GetCritterGUID() &&
            creature->HasNpcFlag(UNIT_NPC_FLAG_TRAINER);
    }

    std::unordered_map<uint32, uint32> _selectedTrainers;
};


bool HandleAscensionGuideTrainerBuy(Player* player, ObjectGuid trainerGuid, uint32 spellId)
{
    AscensionGuideTrainer* guide = AscensionGuideTrainer::Instance();
    return guide && guide->HandleTrainerBuy(player, trainerGuid, spellId);
}

class AscensionRunebladeReforge : public AllCreatureScript
{
public:
    AscensionRunebladeReforge() : AllCreatureScript("AscensionRunebladeReforge") { }

    bool CanCreatureGossipHello(Player* player, Creature* creature) override
    {
        uint32 currentEntry = 0;
        Item* item = nullptr;
        if (!ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) ||
            !CanReforgeRuneblade(player, creature, currentEntry, item))
            return false;

        uint32 const darionMenuId = creature->GetGossipMenuId();
        uint32 const textId = player->GetGossipTextId(darionMenuId, creature);
        player->PrepareGossipMenu(creature, darionMenuId, true);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT,
            currentEntry == kItemShadowmourne ? "<Runeblade> Reshape my weapon: Shadowmourne -> Frostmourne."
                                              : "<Runeblade> Reshape my weapon: Frostmourne -> Shadowmourne.",
            kSenderRuneblade, kActionReforge);

        player->PlayerTalkClass->GetGossipMenu().SetMenuId(kRunebladeMenuId);
        SendGossipMenuFor(player, textId, creature);
        return true;
    }

    bool CanCreatureGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
    {
        if (sender != kSenderRuneblade || action != kActionReforge)
            return false;

        uint32 currentEntry = 0;
        Item* item = nullptr;
        if (!CanReforgeRuneblade(player, creature, currentEntry, item))
            return false;

        CloseGossipMenuFor(player);

        uint32 const newEntry = currentEntry == kItemShadowmourne ? kItemFrostmourne : kItemShadowmourne;
        if (!sObjectMgr->GetItemTemplate(newEntry))
            return true;

        uint8 const slot = item->GetSlot();
        bool const equipped = item->IsEquipped();

        if (equipped)
            player->_ApplyItemMods(item, slot, false);

        item->SetEntry(newEntry);
        item->SetState(ITEM_CHANGED, player);

        if (equipped)
        {
            player->_ApplyItemMods(item, slot, true);
            player->SetVisibleItemSlot(slot, item);
        }

        LOG_INFO("module.ascension_compat", "Reforged runeblade {} into {} for {} (item GUID {}, gems and enchantments kept)",
            currentEntry, newEntry, player->GetName(), item->GetGUID().GetCounter());
        return true;
    }

private:
    static Item* FindRunebladeInstance(Player* player, uint32& currentEntry)
    {
        for (uint8 slot : {EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_OFFHAND})
        {
            Item* equipped = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
            if (!equipped)
                continue;

            uint32 const entry = equipped->GetEntry();
            if (entry == kItemShadowmourne || entry == kItemFrostmourne)
            {
                currentEntry = entry;
                return equipped;
            }
        }

        for (uint32 entry : {kItemShadowmourne, kItemFrostmourne})
        {
            if (Item* item = player->GetItemByEntry(entry))
            {
                currentEntry = entry;
                return item;
            }
        }

        return nullptr;
    }

    static bool CanReforgeRuneblade(Player* player, Creature* creature, uint32& currentEntry, Item*& item)
    {
        if (!player || !creature || creature->GetEntry() != kDarionIcecrownEntry ||
            !player->GetQuestRewardStatus(kShadowmourneFinalQuest))
            return false;

        item = FindRunebladeInstance(player, currentEntry);
        return item != nullptr;
    }
};

class spell_ascension_jailers_bargain : public AuraScript
{
    PrepareAuraScript(spell_ascension_jailers_bargain);

    static constexpr uint8 AbsorbPercent = 30;

    bool Load() override
    {
        return ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) &&
            GetUnitOwner() && GetUnitOwner()->IsPlayer();
    }

    void CalculateAmount(AuraEffect const*, int32& amount, bool& canBeRecalculated)
    {
        if (Unit* owner = GetUnitOwner())
            amount = int32(owner->GetMaxHealth() * AbsorbPercent / 100);

        canBeRecalculated = false;
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_ascension_jailers_bargain::CalculateAmount,
            EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
    }
};

class spell_ascension_reaper_extinction : public AuraScript
{
    PrepareAuraScript(spell_ascension_reaper_extinction);

    static constexpr uint32 BaseChance = 5;
    static constexpr uint32 ChancePerSoul = 10;

    bool Load() override
    {
        return ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) &&
            GetUnitOwner() && GetUnitOwner()->IsPlayer();
    }

    bool CheckProc(ProcEventInfo&)
    {
        Unit* owner = GetUnitOwner();
        if (!owner)
            return false;

        uint32 souls = 0;
        if (Aura* reapedSouls = owner->GetAura(SPELL_REAPER_REAPED_SOUL))
            souls = reapedSouls->GetStackAmount();

        return roll_chance_i(int32(BaseChance + ChancePerSoul * souls));
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_ascension_reaper_extinction::CheckProc);
    }
};

class spell_ascension_reaper_extinction_buff : public AuraScript
{
    PrepareAuraScript(spell_ascension_reaper_extinction_buff);

    static constexpr std::array<uint32, 7> SlaughterRanks =
        {{500373, 500429, 500430, 500431, 500432, 500433, 500434}};

    bool Load() override
    {
        return ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED);
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
        return spellInfo && std::find(SlaughterRanks.begin(), SlaughterRanks.end(), spellInfo->Id) !=
            SlaughterRanks.end();
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_ascension_reaper_extinction_buff::CheckProc);
    }
};

class spell_ascension_reaper_ruin : public AuraScript
{
    PrepareAuraScript(spell_ascension_reaper_ruin);

    static constexpr std::array<uint32, 5> ShudderScythe =
        {{572382, 578261, 578262, 801322, 805708}};

    bool Load() override
    {
        return ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED);
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
        return spellInfo && std::find(ShudderScythe.begin(), ShudderScythe.end(), spellInfo->Id) !=
            ShudderScythe.end();
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_ascension_reaper_ruin::CheckProc);
    }
};

class spell_ascension_reaper_redshade : public AuraScript
{
    PrepareAuraScript(spell_ascension_reaper_redshade);

    static constexpr std::array<uint32, 10> Reap =
        {{354319, 500357, 504056, 504057, 504058, 504557, 505151, 573302, 573303, 801327}};

    bool Load() override
    {
        return ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED);
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
        return spellInfo && std::find(Reap.begin(), Reap.end(), spellInfo->Id) != Reap.end();
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_ascension_reaper_redshade::CheckProc);
    }
};

class spell_ascension_local_mount : public SpellScript
{
    PrepareSpellScript(spell_ascension_local_mount);

    AscensionCollectibles::MountWrapper const* _mount = nullptr;

    bool Validate(SpellInfo const* spellInfo) override
    {
        auto const& entries = AscensionCollectibles::MountWrappers;
        auto itr = std::lower_bound(entries.begin(), entries.end(), spellInfo->Id,
            [](AscensionCollectibles::MountWrapper const& entry, uint32 id)
            {
                return entry.SpellId < id;
            });
        if (itr == entries.end() || itr->SpellId != spellInfo->Id)
            return false;

        _mount = &*itr;
        for (uint32 spellId : {_mount->Ground60, _mount->Ground100, _mount->Flying150,
            _mount->Flying280, _mount->Flying310})
            if (spellId && !sSpellMgr->GetSpellInfo(spellId))
                return false;

        return true;
    }

    bool Load() override
    {
        return ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) &&
            GetCaster()->IsPlayer() && Validate(GetSpellInfo());
    }

    void HandleMount(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);
        Player* player = GetHitPlayer();
        if (!player || !_mount)
            return;

        uint16 riding = player->GetBaseSkillValue(SKILL_RIDING);
        if (riding < 75)
        {
            PreventHitAura();
            return;
        }
        uint32 selected = riding >= 150 ? _mount->Ground100 : _mount->Ground60;
        uint32 map = GetVirtualMapForMapAndZone(player->GetMapId(), player->GetZoneId());
        bool canFly = map == MAP_OUTLAND || (map == MAP_NORTHREND && player->HasSpell(SPELL_COLD_WEATHER_FLYING));
        AreaTableEntry const* area = sAreaTableStore.LookupEntry(player->GetAreaId());
        Battlefield* battlefield = sBattlefieldMgr->GetBattlefieldToZoneId(player->GetZoneId());
        if ((area && (area->flags & AREA_FLAG_NO_FLY_ZONE)) || (battlefield && !battlefield->CanFlyIn()) ||
            player->InBattleground())
            canFly = false;

        if (canFly && riding >= 225)
        {
            uint32 flying = riding >= 300 ? (_mount->Flying310 ? _mount->Flying310 : _mount->Flying280) :
                _mount->Flying150;
            if (!flying)
                flying = _mount->Flying150;
            SpellInfo const* spell = flying ? sSpellMgr->GetSpellInfo(flying) : nullptr;
            if (spell && spell->CheckLocation(player->GetMapId(), player->GetZoneId(),
                player->GetAreaId(), player) == SPELL_CAST_OK &&
                player->canFlyInZone(player->GetMapId(), player->GetZoneId(), spell))
                selected = flying;
        }

        if (!selected)
            return;

        uint32 petNumber = player->GetTemporaryUnsummonedPetNumber();
        player->SetTemporaryUnsummonedPetNumber(0);
        player->RemoveAurasByType(SPELL_AURA_MOUNTED, ObjectGuid::Empty, GetHitAura());
        PreventHitAura();
        player->CastSpell(player, selected, true);
        if (petNumber)
            player->SetTemporaryUnsummonedPetNumber(petNumber);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_ascension_local_mount::HandleMount, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

class spell_ascension_wildcard_mount : public SpellScript
{
    PrepareSpellScript(spell_ascension_wildcard_mount);

    bool Load() override
    {
        return ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) &&
            GetCaster()->IsPlayer();
    }

    void HandleDummy(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);
        Player* player = GetHitPlayer();
        if (!player)
            return;

        std::vector<uint32> known;
        for (AscensionCollectibles::MountWrapper const& entry : AscensionCollectibles::MountWrappers)
            if (player->HasSpell(entry.SpellId))
                known.push_back(entry.SpellId);

        if (known.empty())
            return;

        player->CastSpell(player, known[urand(0, uint32(known.size()) - 1)], true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_ascension_wildcard_mount::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

class npc_ascension_training_book : public CreatureScript
{
public:
    npc_ascension_training_book() : CreatureScript("npc_ascension_training_book") { }

    enum BookGossip : uint32
    {
        TextTraining = 900370,
        ActionRestoreAbilities = GOSSIP_ACTION_INFO_DEF + 1
    };

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        ClearGossipMenuFor(player);
        if (ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) &&
            IsAscensionCustomClass(player))
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Restore my available class abilities.",
                GOSSIP_SENDER_MAIN, ActionRestoreAbilities);
        SendGossipMenuFor(player, TextTraining, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature*, uint32 sender, uint32 action) override
    {
        ClearGossipMenuFor(player);
        CloseGossipMenuFor(player);
        if (sender != GOSSIP_SENDER_MAIN || action != ActionRestoreAbilities ||
            !ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) ||
            !IsAscensionCustomClass(player))
            return true;

        if (!AscensionClassService::Instance().SynchronizeProgression(player, true))
            ChatHandler(player->GetSession()).SendSysMessage("Your available class abilities are already up to date.");
        return true;
    }
};

class spell_ascension_experience_potion : public SpellScript
{
    PrepareSpellScript(spell_ascension_experience_potion);

    int32 _remaining = 0;

    bool Load() override
    {
        return ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) && GetCaster()->IsPlayer();
    }

    void SnapshotDuration(SpellMissInfo missInfo)
    {
        _remaining = 0;
        if (missInfo == SPELL_MISS_NONE)
            if (Unit* target = GetHitUnit())
                if (Aura* aura = target->GetAura(GetSpellInfo()->Id, GetCaster()->GetGUID()))
                    _remaining = std::max(0, aura->GetDuration());
    }

    void ExtendDuration()
    {
        if (_remaining > 0)
            if (Aura* aura = GetHitAura())
            {
                int32 const duration = int32(std::min<int64>(int64(aura->GetDuration()) + _remaining,
                    std::numeric_limits<int32>::max()));
                aura->SetMaxDuration(duration);
                aura->SetDuration(duration);
            }
    }

    void Register() override
    {
        BeforeHit += BeforeSpellHitFn(spell_ascension_experience_potion::SnapshotDuration);
        AfterHit += SpellHitFn(spell_ascension_experience_potion::ExtendDuration);
    }
};

}

bool IsAscensionPrimalistTameEligible(Player const* player)
{
    return player && ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) &&
        player->getClass() == CLASS_WILDWALKER && player->GetLevel() >= 10 && player->HasSpell(92148) &&
        AscensionClassService::Instance().GetActiveSpecialization(player) == 59;
}

bool IsAscensionPrimalistWeaponsEligible(Player const* player, bool allowUnconfirmed)
{
    return player && ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) &&
        player->getClass() == CLASS_WILDWALKER && player->GetLevel() >= 20 && player->HasSpell(537218) &&
        (AscensionClassService::Instance().GetActiveSpecialization(player) == 59 ||
            (allowUnconfirmed && !AscensionClassService::Instance().GetActiveSpecialization(player)));
}

uint32 GetAscensionActiveSpecialization(Player const* player)
{
    if (!player || !IsAscensionCustomClass(player))
        return 0;

    if (uint32 const active = AscensionClassService::Instance().GetActiveSpecialization(player))
        return active;

    return const_cast<Player*>(player)->GetPlayerSetting(ASCENSION_ACTIVE_SPEC_SETTING, 0).value;
}

bool SwitchAscensionSpecialization(Player* player, uint32 specializationId)
{
    return player && ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) &&
        AscensionClassService::Instance().SwitchSpecialization(player, specializationId);
}

static AscensionCompatData::CoATalentEntry const* FindAscensionTalentEntry(uint32 entryId)
{
    auto const& entries = AscensionCompatData::CoATalentEntries;
    auto itr = std::lower_bound(entries.begin(), entries.end(), entryId,
        [](AscensionCompatData::CoATalentEntry const& entry, uint32 id) { return entry.EntryId < id; });
    return itr != entries.end() && itr->EntryId == entryId ? &*itr : nullptr;
}

uint32 GetAscensionTalentRank(Player const* player, uint32 entryId)
{
    AscensionCompatData::CoATalentEntry const* entry = FindAscensionTalentEntry(entryId);
    if (!player || !entry)
        return 0;

    for (uint32 rank = entry->SpellCount; rank > 0; --rank)
        if (entry->SpellIds[rank - 1] && player->HasSpell(entry->SpellIds[rank - 1]))
            return rank;
    return 0;
}

bool SetAscensionTalentRank(Player* player, uint32 entryId, uint32 rank)
{
    AscensionCompatData::CoATalentEntry const* entry = FindAscensionTalentEntry(entryId);
    if (!player || !entry || !IsAscensionCustomClass(player) || entry->ClassId != player->getClass() ||
        rank > entry->SpellCount)
        return false;

    uint32 const freeChoiceGroup = AscensionClassService::GetSelectableFreeGroup(entryId);
    if (entry->AECost == 0 && entry->TECost == 0 && !freeChoiceGroup)
        return false;

    if (rank > 0 && entry->SpecId != 0 && entry->SpecId != GetAscensionActiveSpecialization(player))
        return false;

    uint32 const selectedSpellId = rank > 0 ? entry->SpellIds[rank - 1] : 0;
    if (rank > 0 && (!selectedSpellId || !sSpellMgr->GetSpellInfo(selectedSpellId)))
        return false;

    if (rank > 0 && freeChoiceGroup)
        for (auto const& other : AscensionCompatData::CoATalentEntries)
            if (other.ClassId == player->getClass() && other.SpecId == entry->SpecId && other.EntryId != entryId &&
                AscensionClassService::GetSelectableFreeGroup(other.EntryId) == freeChoiceGroup)
                for (uint32 spellId : other.SpellIds)
                    if (spellId && player->HasSpell(spellId))
                        player->removeSpell(spellId, SPEC_MASK_ALL, false);

    for (uint32 spellId : entry->SpellIds)
        if (spellId && player->HasSpell(spellId))
            player->removeSpell(spellId, SPEC_MASK_ALL, false);

    if (rank > 0)
        player->learnSpell(selectedSpellId, false);

    AscensionClassService::Instance().SynchronizeProgression(player);
    return true;
}

bool IsAscensionCustomClassId(uint8 classId)
{
    return classId >= CLASS_BARBARIAN && classId <= CLASS_SPIRIT_MAGE;
}

std::vector<AscensionClassAbility> GetAscensionClassAbilities(uint8 classId)
{
    std::vector<AscensionClassAbility> abilities;
    if (!IsAscensionCustomClassId(classId))
        return abilities;

    for (auto const& grant : AscensionCompatData::ClassSpells)
        if (grant.ClassId == classId)
            abilities.push_back({ grant.SpellId, grant.SpellId, 0, grant.RequiredLevel });

    std::unordered_map<uint32, uint16> specializationOf;
    for (auto const& entry : AscensionCompatData::CoATalentEntries)
    {
        if (entry.ClassId != classId || !entry.SpellIds[0])
            continue;

        for (uint32 spellId : entry.SpellIds)
        {
            if (!spellId)
                continue;

            abilities.push_back({ spellId, entry.SpellIds[0], entry.SpecId, entry.RequiredLevel });
            specializationOf.emplace(spellId, entry.SpecId);
        }
    }

    for (auto const& rank : AscensionProgression::Ranks)
    {
        if (rank.ClassId != classId)
            continue;

        auto const specialization = specializationOf.find(rank.FirstSpellId);
        uint16 const specId = specialization != specializationOf.end() ? specialization->second : 0;
        abilities.push_back({ rank.SpellId, rank.FirstSpellId, specId, rank.RequiredLevel });
    }

    return abilities;
}

class AscensionCompatAllCreatureScript : public AllCreatureScript {
public:
  AscensionCompatAllCreatureScript()
      : AllCreatureScript("AscensionCompatAllCreatureScript") {}

  void OnCreatureAddWorld(Creature* creature) override {
    if (!creature || !ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED))
      return;
    if (sAscensionPresets->HasPreset(creature->GetEntry()))
    {
      creature->SetUnitFlag2(UNIT_FLAG2_MIRROR_IMAGE);
    }
  }

  void OnAllCreatureUpdate(Creature* creature, uint32) override {
    if (!creature || !ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED))
      return;
    if (sAscensionPresets->HasPreset(creature->GetEntry()))
    {
      if (!creature->HasUnitFlag2(UNIT_FLAG2_MIRROR_IMAGE))
      {
        creature->SetUnitFlag2(UNIT_FLAG2_MIRROR_IMAGE);
      }
    }
  }
};

void AddAscensionCompatScripts() {
  new npc_ascension_training_book();
  RegisterSpellScript(spell_ascension_personal_bank);
  RegisterSpellScript(spell_ascension_experience_potion);
  RegisterSpellScript(spell_ascension_local_mount);
  RegisterSpellScript(spell_ascension_jailers_bargain);
  RegisterSpellScript(spell_ascension_reaper_extinction);
  RegisterSpellScript(spell_ascension_reaper_extinction_buff);
  RegisterSpellScript(spell_ascension_reaper_ruin);
  RegisterSpellScript(spell_ascension_reaper_redshade);
  new AscensionRunebladeReforge();
  new AscensionGuideTrainer();
  RegisterSpellScript(spell_ascension_wildcard_mount);
  RegisterSpellScript(spell_ascension_legacy_quest_reward);
  new AscensionTradesmanScroll();
  new AscensionCompatServerScript();
  new AscensionCompatCommandScript();
  new AscensionCompatPlayerScript();
  new AscensionCompatAllSpellScript();
  new AscensionCompatUnitScript();
  new AscensionCompatChangelogScript();
  new AscensionCompatLevelScalingScript();
  new AscensionCompatLevelScalingEngageScript();
  new AscensionCompatWorldScript();
  new AscensionCompatAllCreatureScript();
}
