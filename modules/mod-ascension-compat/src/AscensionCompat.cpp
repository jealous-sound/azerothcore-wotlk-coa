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
#include "AscensionManastorm.h"
#include "AscensionClassMechanics.h"
#include "AscensionClassMechanics19To25.h"
#include "AscensionClassMechanics26To32.h"
#include "AscensionCoATalentData.h"
#include "AscensionRunemasterEchoes.h"
#include "AscensionCollectionModelData.h"
#include "AscensionAmmunitionData.h"
#include "AscensionCollectibleSpellData.h"
#include "AscensionCustomClassData.h"
#include "AscensionAuraAmounts.h"
#include "AscensionBarbarian.h"
#include "AscensionBarbarianScaling.h"
#include "AscensionCustomResourceData.h"
#include "AscensionFreshCharacterCheck.h"
#include "AscensionLiveBaselineData.h"
#include "AscensionPrimalistEarthshaping.h"
#include "AscensionPrimalistSpiritBeast.h"
#include "AscensionReaperSoulStrike.h"
#include "AscensionReaperDeathwind.h"
#include "AscensionReaperPainmail.h"
#include "AscensionVenomancerCatalyst.h"
#include "AscensionSpellProgressionData.h"
#include "AscensionTaughtAbilityData.h"
#include "Bag.h"
#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "Chat.h"
#include "CommandScript.h"
#include "ConfigValueCache.h"
#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "GossipDef.h"
#include "GlobalScript.h"
#include "Item.h"
#include "ItemScript.h"
#include "LocalLevelScaling.h"
#include "Log.h"
#include "Map.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "Random.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "Spell.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include <algorithm>
#include <array>
#include <cstring>
#include <type_traits>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace Acore::ChatCommands;

namespace {
constexpr uint16 CMSG_VANITY_DELIVERY = 0x0523;
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
constexpr uint16 SMSG_CHARACTER_ADVANCEMENT_AUTHENTICATION = 0x0725;
constexpr uint16 CMSG_MISSILE_FIRE_POSITION = 0x09C7;

constexpr uint32 SPELL_PYROMANCER_HEAT = 807389;
constexpr uint32 SPELL_PYROMANCER_EMBER = 807533;
constexpr uint32 SPELL_PRIMALIST_EARTHSHAPING = 680441;
constexpr uint32 SPELL_REAPER_REAPED_SOUL = 500363;
constexpr uint32 SPELL_REAPER_SOUL_INFUSION = 803031;
constexpr uint32 SPELL_REAPER_SOUL_FRAGMENT = 805077;
constexpr uint32 SPELL_REAPER_GENERATE_SOUL = 805078;
constexpr char ASCENSION_LOCAL_RESOURCE_PREFIX[] = "ASC_LOCAL_RESOURCE";
constexpr char ASCENSION_ACTIVE_SPEC_SETTING[] = "core.ascension_active_spec";

constexpr uint8 PYROMANCER_HEAT_PER_EMBER = 100;
constexpr uint8 REAPER_SOUL_FRAGMENT_COST = 3;

constexpr std::array<uint32, 4> REAPER_ALL_SOUL_CONSUMERS =
{{
    500483, // Tormented Souls
    500484, // Spectral Scythe
    500576, // Spectral Scythe (Soul Infusion variant)
    500631  // Reliquary of the Lost
}};

constexpr std::array<std::pair<uint32, uint32>, 1> REAPER_ONE_SOUL_CONSUMERS =
{{
    // Lament's datamined rank IDs are absent from the live local Spell.dbc.
    {500361, 500361} // Sanguine Orb
}};

constexpr uint8 VANITY_DELIVERY_ACTION = 2;
constexpr std::size_t APPEARANCE_CATEGORY_COUNT = 69;
constexpr uint32 APPEARANCE_CATEGORY_AMMUNITION = 32;
// The copied 3.3.5 client supports the 23-bit extended world-packet header.
// Bound this snapshot to 512 KiB of entries (1 MiB in its native vector), not
// the former, incorrect 64 KiB transport assumption. The full local catalog fits.
constexpr std::size_t MAX_APPEARANCE_SNAPSHOT_ENTRIES = 65536;
constexpr std::size_t APPEARANCE_ADDS_PER_BATCH = 16;
constexpr uint32 APPEARANCE_ADD_BATCH_INTERVAL_MS = 100;
constexpr uint32 APPEARANCE_ADD_INITIAL_DELAY_MS = 500;
constexpr uint32 APPEARANCE_LOGIN_RESYNC_DELAY_MS = 3000;
constexpr std::size_t MAX_QUEUED_EXTENSION_PACKETS = 64;
constexpr uint32 VANITY_CATEGORY_MOUNTS = 0x04000000;
constexpr uint32 VANITY_CATEGORY_COMPANIONS = 0x08000000;
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
  DBC_DIRECTORY,
  AUTO_COLLECT_APPEARANCES,
  UNLOCK_LOCAL_APPEARANCE_CATALOG,
  APPEARANCE_CATALOG_PER_CATEGORY,
  UNLOCK_ALL_VANITY,
  ALLOW_LEARNED_SPELL_DELIVERY,
  LEARN_OWNED_COMPANIONS,
  MAX_RIDING_FROM_START,
  LEVEL_SCALING,
  QUEST_LEVEL_SCALING,

  NUM_CONFIGS,
};

class AscensionCompatConfigData
    : public ConfigValueCache<AscensionCompatConfig> {
public:
  AscensionCompatConfigData()
      : ConfigValueCache(AscensionCompatConfig::NUM_CONFIGS) {}

  void BuildConfigCache() override {
    SetConfigValue<bool>(AscensionCompatConfig::ENABLED,
                         "AscensionCompat.Enable", true);
    SetConfigValue<bool>(AscensionCompatConfig::LOG_CONSUMED_PACKETS,
                         "AscensionCompat.LogConsumedPackets", true);
    SetConfigValue<uint32>(AscensionCompatConfig::FIRST_EXTENSION_OPCODE,
                           "AscensionCompat.FirstExtensionOpcode", 0x051F);
    SetConfigValue<uint32>(AscensionCompatConfig::LAST_EXTENSION_OPCODE,
                           "AscensionCompat.LastExtensionOpcode", 0x09D3);
    SetConfigValue<std::string>(AscensionCompatConfig::DBC_DIRECTORY,
                                "AscensionCompat.DbcDirectory",
                                "./data/dbc/Ascension");
    SetConfigValue<bool>(AscensionCompatConfig::AUTO_COLLECT_APPEARANCES,
                         "AscensionCompat.AutoCollectAppearances", true);
    SetConfigValue<bool>(
        AscensionCompatConfig::UNLOCK_LOCAL_APPEARANCE_CATALOG,
        "AscensionCompat.UnlockLocalAppearanceCatalog", true);
    SetConfigValue<uint32>(
        AscensionCompatConfig::APPEARANCE_CATALOG_PER_CATEGORY,
        "AscensionCompat.AppearanceCatalogPerCategory", 500);
    SetConfigValue<bool>(AscensionCompatConfig::UNLOCK_ALL_VANITY,
                         "AscensionCompat.UnlockAllVanity", true);
    SetConfigValue<bool>(AscensionCompatConfig::ALLOW_LEARNED_SPELL_DELIVERY,
                         "AscensionCompat.AllowLearnedSpellDelivery", true);
    SetConfigValue<bool>(AscensionCompatConfig::LEARN_OWNED_COMPANIONS,
                         "AscensionCompat.LearnOwnedCompanions", true);
    SetConfigValue<bool>(AscensionCompatConfig::MAX_RIDING_FROM_START,
                         "AscensionCompat.MaxRidingFromStart", true);
    SetConfigValue<bool>(AscensionCompatConfig::LEVEL_SCALING,
                         "AscensionCompat.LevelScaling", true);
    SetConfigValue<bool>(AscensionCompatConfig::QUEST_LEVEL_SCALING,
                         "AscensionCompat.QuestLevelScaling", true);
  }
};

AscensionCompatConfigData ascensionCompatConfig;

struct AppearanceInfo {
  uint32 SourceItem = 0;
  uint32 PrimaryCategory = 0;
  uint32 SecondaryCategory = 0;
  uint32 TertiaryCategory = 0;
  uint32 EnchantId = 0;
};

struct VanityInfo {
  uint32 LearnedSpell = 0;
  uint32 Flags = 0;
  uint32 CategoryMask = 0;
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
  bool CanSeeItemAppearances = true;
  bool CanSeeSpellAppearances = true;
};

struct WdbcHeader {
  char Magic[4];
  uint32 RecordCount;
  uint32 FieldCount;
  uint32 RecordSize;
  uint32 StringBlockSize;
};

uint32 ReadRecordField(std::vector<uint8> const &record,
                       std::size_t fieldIndex) {
  uint32 value = 0;
  std::memcpy(&value, record.data() + fieldIndex * sizeof(uint32),
              sizeof(value));
  return value;
}

bool ForEachWdbcRecord(
    std::filesystem::path const &path, std::size_t minimumDwordCount,
    std::function<void(std::vector<uint8> const &)> const &visitor) {
  std::ifstream input(path, std::ios::binary);
  if (!input)
  {
    LOG_ERROR("module.ascension_compat", "Unable to open Ascension DBC {}",
              path.generic_string());
    return false;
  }

  WdbcHeader header{};
  input.read(reinterpret_cast<char *>(&header), sizeof(header));
  if (!input || std::memcmp(header.Magic, "WDBC", 4) != 0)
  {
    LOG_ERROR("module.ascension_compat", "Invalid WDBC header in {}",
              path.generic_string());
    return false;
  }

  if (header.RecordSize < minimumDwordCount * sizeof(uint32) ||
      header.RecordSize % sizeof(uint32) != 0) {
    LOG_ERROR("module.ascension_compat",
              "Unsupported record layout in {}: fields={}, recordSize={}",
              path.generic_string(), header.FieldCount, header.RecordSize);
    return false;
  }

  std::vector<uint8> record(header.RecordSize);
  for (uint32 row = 0; row < header.RecordCount; ++row) {
    input.read(reinterpret_cast<char *>(record.data()), record.size());
    if (!input)
    {
      LOG_ERROR("module.ascension_compat", "Truncated DBC {} at row {}",
                path.generic_string(), row);
      return false;
    }

    visitor(record);
  }

  return true;
}

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

AscensionCompatData::StarterKit const *GetStarterKit(uint8 playerClass) {
  auto itr = std::find_if(
      AscensionCompatData::StarterKits.begin(),
      AscensionCompatData::StarterKits.end(),
      [playerClass](AscensionCompatData::StarterKit const &kit) {
        return kit.ClassId == playerClass;
      });
  return itr == AscensionCompatData::StarterKits.end() ? nullptr : &*itr;
}

class AscensionClassService {
public:
  static AscensionClassService &Instance() {
    static AscensionClassService instance;
    return instance;
  }

  uint32 SynchronizeProgression(Player *player) {
    if (!IsAscensionCustomClass(player))
      return 0;

    // Explicitly authorized 2026-09-03: reconcile only generator-owned class
    // grants. Do not scan arbitrary quest, collection or purchased spells for
    // absence from a level-one snapshot. Valid selected talents are independent.
    uint32 const activeSpec = GetActiveSpecialization(player);
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
    auto currentGrantAllows = [player, activeSpec, &selectedTalentOwns](uint32 spellId)
    {
      // Older local talent choices were persisted only as learned spell IDs.
      // A colliding paid talent therefore remains protected until the client
      // supplies its selection, rather than treating catalog absence as proof.
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
      return observed || proficiency || unresolved || automatic || selectedTalentOwns(spellId) || std::any_of(AscensionCompatData::ClassSpells.begin(),
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
      LOG_INFO("module.ascension_compat", "Reconciled {} proven class grants for {} against live level {}",
          removed, player->GetName(), uint32(player->GetLevel()));
    uint32 learned = 0;
    for (auto const& entry : AscensionLiveBaseline::Spells)
      if (entry.ClassId == player->getClass() && (!entry.RaceId || entry.RaceId == player->getRace()) &&
          !player->HasSpell(entry.SpellId) && sSpellMgr->GetSpellInfo(entry.SpellId))
      {
        player->learnSpell(entry.SpellId, false);
        ++learned;
      }
    for (AscensionCompatData::ClassSpell const &progressionSpell :
         AscensionCompatData::ClassSpells) {
      if (progressionSpell.ClassId != player->getClass() ||
          progressionSpell.RequiredLevel > player->GetLevel() ||
          player->HasSpell(progressionSpell.SpellId))
        continue;

      if (!sSpellMgr->GetSpellInfo(progressionSpell.SpellId))
      {
        LOG_ERROR("module.ascension_compat",
                  "Cannot teach missing Ascension class spell {} to {}",
                  progressionSpell.SpellId, player->GetName());
        continue;
      }

      player->learnSpell(progressionSpell.SpellId, false);
      ++learned;
    }

    ReconcileRunemasterFists(player, activeSpec);
    learned += SynchronizeAutomaticTalents(player, GetActiveSpecialization(player));
    // Rank upgrades are conditional on already owning the root. They cannot
    // spend talent points, pick an unselected ability, or leak an old spec.
    for (AscensionProgression::Rank const& rank : AscensionProgression::Ranks)
    {
        if (rank.ClassId != player->getClass() || rank.RequiredLevel > player->GetLevel() ||
            !player->HasSpell(rank.FirstSpellId) || player->HasSpell(rank.SpellId))
            continue;

        if (sSpellMgr->GetSpellInfo(rank.SpellId))
        {
            player->learnSpell(rank.SpellId, false);
            ++learned;
        }
    }

    learned += SynchronizeTaughtAbilities(player);
    SynchronizeAscensionRunemasterEchoes(player, GetActiveSpecialization(player));

    if (learned)
    {
      ChatHandler(player->GetSession())
          .PSendSysMessage("Restored {} Ascension class abilities.", learned);
      LOG_INFO("module.ascension_compat",
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

    uint32 SynchronizeTaughtAbilities(Player* player)
    {
        if (!IsAscensionCustomClass(player))
            return 0;

        uint32 const specializationId = GetActiveSpecialization(player);
        uint32 learned = 0;
        for (auto const& entry : AscensionCompatData::TaughtAbilities)
        {
            if (entry.ClassId != player->getClass())
                continue;

            // Wait for CAD's confirmed specialization after login. A persisted
            // parent alone must not teach an ability from the previous spec.
            bool const allowed = specializationId == entry.SpecId &&
                player->GetLevel() >= entry.RequiredLevel && player->HasSpell(entry.ParentSpellId);
            if (!allowed)
            {
                player->removeSpell(entry.SpellId, SPEC_MASK_ALL, true);
                continue;
            }

            // Preserve independent permanent ownership, other native specs and
            // pending deletion records. Native _addSpell would resurrect a
            // tombstone as CHANGED even when requested as temporary.
            auto const& spells = player->GetSpellMap();
            if (spells.find(entry.SpellId) != spells.end() || !sSpellMgr->GetSpellInfo(entry.SpellId))
                continue;

            player->learnSpell(entry.SpellId, true);
            if (player->HasSpell(entry.SpellId))
                ++learned;
        }
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
    if (!_proficiencySynchronizations.insert(guid).second)
      return;

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
            player->learnSpell(definition.SpellId, false);
            ++learned;
          }
          else
          {
            LOG_ERROR("module.ascension_compat",
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

      // Bounded to the known proficiency catalog, not arbitrary learned spells.
      if (player->HasSpell(definition.SpellId))
      {
        player->removeSpell(definition.SpellId, SPEC_MASK_ALL, false);
        ++removed;
      }
      if (player->HasSkill(definition.SkillId))
        player->SetSkill(definition.SkillId, 0, 0, 0);
    }

    _proficiencySynchronizations.erase(guid);
    if (learned || removed)
    {
      LOG_INFO("module.ascension_compat",
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

    // Called only inside Player::Create, never while loading an existing player.
    // Hidden proficiency spells are separate from the visible spellbook roots.
    for (auto const& entry : AscensionLiveBaseline::Spells)
    {
      if (entry.ClassId != player->getClass() || (entry.RaceId && entry.RaceId != player->getRace()))
        continue;
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
      // Weapon display ranks need a separate compatibility review: preserve
      // native level-scaled combat skill for now, and record this deviation.
      bool const weapon = std::any_of(AscensionCompatData::ProficiencyDefinitions.begin(),
          AscensionCompatData::ProficiencyDefinitions.end(), [&entry](auto const& definition)
          { return definition.SkillId == entry.SkillId && definition.ScalesWithLevel; });
      // Unlike our explicitly maximized weapon proficiencies, Unarmed keeps
      // native current/cap (normally 1/5 here). A max of 1 prevents future growth.
      if (entry.SkillId == SKILL_UNARMED)
        continue;
      uint16 const maximum = weapon ? player->GetMaxSkillValueForLevel() : entry.Maximum;
      uint16 const value = weapon ? maximum : entry.Rank;
      player->SetSkill(entry.SkillId, 0, value, maximum);
    }
    return true;
  }

  // Creation-only entry point. The caller must abort Player::Create on false
  // and skip both legacy starter placement and the later bag auto-equip pass.
  // Login/repair paths deliberately never call this function.
  bool InitializeLiveStarterKit(Player* player)
  {
    if (!IsAscensionCustomClass(player) || player->IsInWorld())
      return false;

    constexpr uint32 liveStarterRevision = 20260903;
    char const* const liveSetting = "core.ascension_starter_live";
    if (player->GetPlayerSetting(liveSetting, 0).value == liveStarterRevision)
      return true;

    // Do not replace, relocate, delete, or top up any pre-existing inventory.
    // An empty, newly constructed Player is the only supported input.
    for (uint8 slot = EQUIPMENT_SLOT_START; slot < INVENTORY_SLOT_ITEM_END; ++slot)
      if (player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
      {
        LOG_ERROR("module.ascension_compat", "Refused non-empty live starter initialization for class {}", uint32(player->getClass()));
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
        LOG_ERROR("module.ascension_compat", "Invalid live starter class {} item {} slot {} count {}", uint32(entry.ClassId), entry.ItemId, uint32(entry.Slot), entry.Count);
        return false;
      }

      if (equipped)
      {
        uint16 destination = 0;
        InventoryResult const result = player->CanEquipNewItem(entry.Slot, destination, entry.ItemId, false);
        if (result != EQUIP_ERR_OK || destination != position)
        {
          LOG_ERROR("module.ascension_compat", "Cannot equip live starter class {} item {} in slot {}: {}", uint32(entry.ClassId), entry.ItemId, uint32(entry.Slot), uint32(result));
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
          LOG_ERROR("module.ascension_compat", "Cannot store live starter class {} item {} in slot {}: {}", uint32(entry.ClassId), entry.ItemId, uint32(entry.Slot), uint32(result));
          return false;
        }
      }
      ++entries;
    }
    if (!entries)
      return false;

    // The generated order is equipment first (main hand before off hand), then
    // backpack positions. Equal item IDs in different slots remain distinct.
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
        LOG_ERROR("module.ascension_compat", "Failed exact live starter placement for class {} item {} slot {}", uint32(entry.ClassId), entry.ItemId, uint32(entry.Slot));
        return false;
      }
    }

    // Saved with the initial character transaction, not after the create
    // callback. This also prevents the legacy login repair from adding old gear.
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

    // A hearthstone or one surviving starter item does not prove the kit is
    // complete. Repair each character once; never replace gear already worn.
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
        LOG_ERROR("module.ascension_compat", "Missing starter item template {}", itemId);
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
        LOG_ERROR("module.ascension_compat", "Unsupported starter item inventory type for {}", itemId);
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
      LOG_INFO("module.ascension_compat",
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
        _activeSpecializations[player->GetGUID().GetCounter()] = specializationId;

    SynchronizeProgression(player);
    SynchronizeProficiencies(player);
    RepairStarterKit(player, false);
    SendCharacterAdvancementAuthentication(player);
  }

  void SendCharacterAdvancementAuthentication(Player *player) {
    WorldPacket packet(SMSG_CHARACTER_ADVANCEMENT_AUTHENTICATION,
                       sizeof(uint32) * 2);
    packet << uint32(0) << uint32(0);
    player->GetSession()->SendPacket(&packet);

    LOG_INFO("module.ascension_compat",
             "Initialized Character Advancement for {} (class {}, level {})",
             player->GetName(), uint32(player->getClass()),
             uint32(player->GetLevel()));
  }

  uint32 GetActiveSpecialization(Player const *player) const {
    auto itr = _activeSpecializations.find(player->GetGUID().GetCounter());
    return itr == _activeSpecializations.end() ? 0 : itr->second;
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
      _activeSpecializations[player->GetGUID().GetCounter()] =
          specializationId;
      player->UpdatePlayerSetting(ASCENSION_ACTIVE_SPEC_SETTING, 0, specializationId);

      uint32 granted = SynchronizeProgression(player);
      LOG_INFO("module.ascension_compat",
               "Synchronized {} (class {}) with local specialization {} and "
               "granted {} missing automatic spells",
               player->GetName(), uint32(player->getClass()), specializationId,
               granted);
      return true;
    }

    std::unordered_set<uint32> visitedSpellIds;
    uint32 removed = 0;
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

    _activeSpecializations[player->GetGUID().GetCounter()] =
        specializationId;
    player->UpdatePlayerSetting(ASCENSION_ACTIVE_SPEC_SETTING, 0, specializationId);

    uint32 granted = SynchronizeProgression(player);
    ChatHandler(player->GetSession())
        .PSendSysMessage(
            "Activated specialization {}. Refunded all CoA talent points, "
            "removed {} old talent spell(s), and granted {} automatic "
            "ability/passive spell(s).",
            specializationId, removed, granted);
    LOG_INFO("module.ascension_compat",
             "Switched {} (class {}) to local specialization {}: removed "
             "{} CoA spells and granted {} automatic spells",
             player->GetName(), uint32(player->getClass()), specializationId,
             removed, granted);
    return true;
  }

  void OnPlayerLogout(Player *player) {
    _activeSpecializations.erase(player->GetGUID().GetCounter());
    _proficiencySynchronizations.erase(player->GetGUID().GetCounter());
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
        // An unconfirmed custom specialization cannot disprove a saved identity.
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

        // Preserve native HasSpell eligibility, including an inactive Zenith or
        // a temporary prerequisite. Acquisition still uses normal progression.
        if (!CanGrantAutomaticEntry(player, *fists, specializationId) && player->HasSpell(92153))
            player->removeSpell(92153, player->GetActiveSpecMask(), false);
    }

private:
    static uint32 SynchronizeAutomaticTalents(Player* player, uint32 specializationId)
    {
        // At level one the observed spellbook, not empty implicit/CAD responses,
        // defines the baseline. Higher-level dependency-gated talents remain native.
        if (player->GetLevel() == 1)
            return 0;
        uint32 learned = 0;
        bool changed = true;
        // Resolve dependencies even when their entry IDs sort after their children.
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

                player->learnSpell(spellId, false);
                ++learned;
                changed = true;
            }
        }
        return learned;
    }

  std::unordered_map<uint32, uint32> _activeSpecializations;
  std::unordered_set<uint32> _proficiencySynchronizations;
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
                    LOG_ERROR("module.ascension_compat",
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

        for (AscensionCompatData::ResourceGainRule const& rule :
             AscensionCompatData::ResourceGainRules)
        {
            validateResourceSpell(rule.ResourceSpellId);
            validateResourceSpell(rule.RequiredAuraSpellId);
            validateResourceSpell(rule.ForbiddenAuraSpellId);
            if (rule.ChancePercent > 100)
            {
                LOG_ERROR("module.ascension_compat",
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
                    LOG_ERROR("module.ascension_compat",
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
                LOG_ERROR("module.ascension_compat",
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
                    LOG_ERROR("module.ascension_compat",
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
                LOG_ERROR("module.ascension_compat",
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
                    LOG_ERROR("module.ascension_compat",
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
                LOG_ERROR("module.ascension_compat",
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
                    LOG_ERROR("module.ascension_compat",
                        "Ascension Reaper one-soul consumer spell {} is missing from the server DBC",
                        spellId);
                    ++missingAbilitySpells;
                }
            }
        }

        LOG_INFO("module.ascension_compat",
            "Validated {} custom resource auras and spell helpers; {} resource spells and {} mapped abilities are missing",
            checkedResourceSpells.size(), missingResourceSpells,
            missingAbilitySpells);
    }

    void OnPlayerLogin(Player* player) const
    {
        _lastClientResourceStates.erase(player->GetGUID().GetCounter());
        if (IsAscensionCustomClass(player))
        {
            SynchronizeThresholdResources(player);
            SendClientState(player, true);
        }
    }

    void OnPlayerUpdate(Player* player) const
    {
        if (IsAscensionCustomClass(player))
        {
            SynchronizeThresholdResources(player);
            SendClientState(player, false);
        }
    }

    void OnPlayerLogout(Player* player) const
    {
        if (player)
            _lastClientResourceStates.erase(player->GetGUID().GetCounter());
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

        // Soul Fragment schedules this helper after every gained fragment.
        // Ascension's private dummy handler only lets the helper continue at
        // three stacks; without this gate every fragment becomes a full soul.
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
            // The copied Ranger records carry the correct Advantage contract,
            // but the proprietary realm also enforced it before the normal
            // cast pipeline. Keep a compatibility-side guard so every rank and
            // talent consumer follows the same requirement.
            result = SPELL_FAILED_CASTER_AURASTATE;
            return;
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

            if (rule.PreserveCostAuraSpellId &&
                player->HasAura(rule.PreserveCostAuraSpellId) &&
                rule.PreserveCostChancePercent &&
                roll_chance_i(rule.PreserveCostChancePercent))
                break;

            if (rule.Consumption ==
                AscensionCompatData::ResourceConsumption::Fixed)
            {
                ModifyAuraStacks(player, rule.ResourceSpellId, -rule.Amount);
            }
            else if (rule.Consumption ==
                     AscensionCompatData::ResourceConsumption::All)
            {
                player->RemoveAurasDueToSpell(rule.ResourceSpellId);
            }
            break;
        }

        ConsumeReaperSouls(player, spellInfo);
        SynchronizeThresholdResources(player);
        SendClientState(player, false);
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
        // This hook runs after damage. Keep killing blows and neutral/yellow
        // enemies eligible without accepting friendly or self targets.
        bool hostile = target != player && !player->IsFriendlyTo(target);
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
                    qualifies = successful && hostile && damage;
                    firstOnly = true;
                    break;
                case AscensionCompatData::ResourceGainEvent::EachSuccessfulDamagingHit:
                    qualifies = successful && hostile && damage;
                    break;
                case AscensionCompatData::ResourceGainEvent::FirstCriticalDamagingHit:
                    qualifies = successful && hostile && damage && critical;
                    firstOnly = true;
                    break;
                case AscensionCompatData::ResourceGainEvent::EachCriticalDamagingHit:
                    qualifies = successful && hostile && damage && critical;
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
                    qualifies = successful && hostile && damage;
                    firstOnly = true;
                    break;
                case AscensionCompatData::ResourceGainEvent::EachSuccessfulDamagingHit:
                    qualifies = successful && hostile && damage;
                    break;
                case AscensionCompatData::ResourceGainEvent::FirstCriticalDamagingHit:
                    qualifies = successful && hostile && damage && critical;
                    firstOnly = true;
                    break;
                case AscensionCompatData::ResourceGainEvent::EachCriticalDamagingHit:
                    qualifies = successful && hostile && damage && critical;
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

        // A packed comparison key keeps the per-tick synchronization silent
        // unless the authoritative server-side resource state changed.
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
        // Use the GUID overload explicitly.  The WorldObject overload turns
        // messages sent by a GM account into SMSG_GM_MESSAGECHAT, which does
        // not reach Lua as CHAT_MSG_ADDON on this client.
        ChatHandler::BuildChatPacket(packet, CHAT_MSG_WHISPER, LANG_ADDON,
            player->GetGUID(), player->GetGUID(), message, 0,
            player->GetName(), player->GetName(), 0, false);
        player->GetSession()->SendPacket(&packet);

        LOG_INFO("module.ascension_compat",
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

    static void ConsumeReaperSouls(Player* player,
        SpellInfo const* spellInfo)
    {
        if (player->getClass() != CLASS_REAPER)
            return;

        uint32 spellId = spellInfo->Id;
        if (std::find(REAPER_ALL_SOUL_CONSUMERS.begin(),
                REAPER_ALL_SOUL_CONSUMERS.end(), spellId) !=
            REAPER_ALL_SOUL_CONSUMERS.end())
        {
            player->RemoveAurasDueToSpell(SPELL_REAPER_REAPED_SOUL);
            player->RemoveAurasDueToSpell(SPELL_REAPER_SOUL_INFUSION);
            return;
        }

        for (std::pair<uint32, uint32> const& range :
             REAPER_ONE_SOUL_CONSUMERS)
        {
            if (spellId >= range.first && spellId <= range.second)
            {
                ModifyAuraStacks(player, SPELL_REAPER_REAPED_SOUL, -1);
                return;
            }
        }
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
        }
    }

    mutable std::unordered_map<uint32, uint64> _lastClientResourceStates;
};

class AscensionCollectionService {
public:
  static AscensionCollectionService &Instance() {
    static AscensionCollectionService instance;
    return instance;
  }

  bool LoadClientData(std::filesystem::path const &dbcDirectory) {
    _appearances.clear();
    _itemAppearances.clear();
    _itemSetItems.clear();
    _vanityItems.clear();
    _allAppearanceIds.clear();
    _allVanityItemIds.clear();

    bool appearancesLoaded =
        ForEachWdbcRecord(dbcDirectory / "Appearances.dbc", 8,
                          [this](std::vector<uint8> const &record) {
                            uint32 appearanceId = ReadRecordField(record, 0);
                            if (!appearanceId)
                              return;

                            uint32 displayId = ReadRecordField(record, 3);
                            _appearances[appearanceId] = AppearanceInfo{
                                displayId, ReadRecordField(record, 5),
                                ReadRecordField(record, 6),
                                ReadRecordField(record, 7), displayId};
                            _allAppearanceIds.push_back(appearanceId);
                          });

    bool itemAppearancesLoaded =
        ForEachWdbcRecord(dbcDirectory / "ItemAppearances.dbc", 3,
                          [this](std::vector<uint8> const &record) {
                            uint32 itemId = ReadRecordField(record, 1);
                            uint32 appearanceId = ReadRecordField(record, 2);
                            if (itemId && appearanceId)
                              _itemAppearances[itemId] = appearanceId;
                          });

    bool itemSetsLoaded =
        ForEachWdbcRecord(dbcDirectory.parent_path() / "ItemSet.dbc", 35,
                          [this](std::vector<uint8> const &record) {
                            uint32 itemSetId = ReadRecordField(record, 0);
                            if (!itemSetId)
                              return;

                            std::vector<uint32> &items =
                                _itemSetItems[itemSetId];
                            for (std::size_t field = 18; field <= 34; ++field) {
                              uint32 itemId = ReadRecordField(record, field);
                              if (itemId)
                                items.push_back(itemId);
                            }
                          });

    bool vanityLoaded =
        ForEachWdbcRecord(dbcDirectory / "VanityCollection.dbc", 77,
                          [this](std::vector<uint8> const &record) {
                            uint32 itemId = ReadRecordField(record, 1);
                            if (!itemId)
                              return;

                            _vanityItems[itemId] =
                                // f44 is an empty locale column. The physical
                                // record has 77 DWORDs; f76 is LearnedSpell.
                                VanityInfo{ReadRecordField(record, 76),
                                           ReadRecordField(record, 12),
                                           ReadRecordField(record, 2)};
                            _allVanityItemIds.push_back(itemId);
                          });

    std::sort(_allAppearanceIds.begin(), _allAppearanceIds.end());
    _allAppearanceIds.erase(
        std::unique(_allAppearanceIds.begin(), _allAppearanceIds.end()),
        _allAppearanceIds.end());

    LOG_INFO("module.ascension_compat",
             "Loaded Ascension collection data: {} appearances, {} item "
             "mappings, {} item sets, {} vanity entries",
             _appearances.size(), _itemAppearances.size(),
             _itemSetItems.size(), _vanityItems.size());

    if (!itemSetsLoaded)
      LOG_WARN("module.ascension_compat",
               "Ascension item-set expansion is unavailable; individual "
               "appearance categories remain usable");

    _clientDataLoaded =
        appearancesLoaded && itemAppearancesLoaded && vanityLoaded;
    return _clientDataLoaded;
  }

  void QueueClientPacket(uint32 accountId, WorldPacket const &packet) {
    std::lock_guard lock(_packetMutex);
    std::deque<WorldPacket> &queue = _pendingPackets[accountId];
    if (queue.size() >= MAX_QUEUED_EXTENSION_PACKETS)
    {
      LOG_WARN("module.ascension_compat",
               "Dropping Ascension extension packet 0x{:04X} for account {} "
               "because its queue is full",
               packet.GetOpcode(), accountId);
      return;
    }

    queue.emplace_back(packet);
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
    SendVanityCollection(player, *state);
    RefreshVisibleItems(player);
    InitializeRiding(player);
    QueueOwnedCompanionSpells(player, *state);

    LOG_INFO("module.ascension_compat",
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

    std::lock_guard lock(_packetMutex);
    _pendingPackets.erase(player->GetSession()->GetAccountId());
  }

  void OnPlayerUpdate(Player *player, uint32 diff) {
    std::deque<WorldPacket> packets;
    uint32 accountId = player->GetSession()->GetAccountId();
    {
      std::lock_guard lock(_packetMutex);
      auto itr = _pendingPackets.find(accountId);
      if (itr != _pendingPackets.end())
      {
        packets = std::move(itr->second);
        _pendingPackets.erase(itr);
      }
    }

    for (WorldPacket &packet : packets)
      HandleClientPacket(player, packet);

    ProcessPendingAppearanceAdds(player, diff);
    ProcessPendingCompanionSpells(player, diff);
  }

    void InitializeRiding(Player* player) const
    {
        if (!ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::MAX_RIDING_FROM_START))
            return;

        // Permanent riding ranks, not profession/weapon skills or talent grants.
        // Learning the ranks also activates the client's riding spellbook entries.
        for (uint32 spellId : {SPELL_RIDING_APPRENTICE, SPELL_RIDING_JOURNEYMAN,
            SPELL_RIDING_EXPERT, SPELL_RIDING_ARTISAN, SPELL_COLD_WEATHER_FLYING})
            if (sSpellMgr->GetSpellInfo(spellId) && !player->HasSpell(spellId))
                player->learnSpell(spellId, false);

        player->SetSkill(SKILL_RIDING, 4, 300, 300);
    }

    // This hook runs after the normal spellbook snapshot but before AddToMap.
    // Replace that snapshot once if necessary; learnSpell does not emit a
    // separate learned-spell packet while the player is outside the world.
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
            player->learnSpell(spellId, false);
            if (player->HasSpell(spellId))
                ++learned;
        }

        if (learned)
        {
            player->SendInitialSpells();
            LOG_INFO("module.ascension_compat", "Prepared {} account mount/companion spells for {} before entering the world",
                learned, player->GetName());
        }
    }

    std::vector<uint32> GetMissingOwnedCompanionSpells(Player* player, PlayerCollectionState const& state) const
    {
        std::vector<uint32> spells;
        if (!ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::LEARN_OWNED_COMPANIONS))
            return spells;

        bool const unlockAll = ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::UNLOCK_ALL_VANITY);
        for (auto const& [itemId, vanity] : _vanityItems)
        {
            if (!(vanity.CategoryMask & (VANITY_CATEGORY_MOUNTS | VANITY_CATEGORY_COMPANIONS)) ||
                (!unlockAll && !state.OwnedVanityItems.contains(itemId)) ||
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
        // Retain bounded late synchronization for any grant not prepared at login.
        state.PendingCompanionSpells = GetMissingOwnedCompanionSpells(player, state);
        state.CompanionSpellTimer = 5000;
        if (!state.PendingCompanionSpells.empty())
            LOG_INFO("module.ascension_compat", "Queued {} owned mount/companion spells for {} (4 per 200 ms)",
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

        // Never catch up by draining the whole list after a slow server tick.
        // Each learned spell emits client events; a bulk grant can freeze its UI.
        state->CompanionSpellTimer = COMPANION_SPELL_BATCH_INTERVAL_MS;
        std::size_t const end = std::min(state->NextCompanionSpell + COMPANION_SPELLS_PER_BATCH,
            state->PendingCompanionSpells.size());
        while (state->NextCompanionSpell < end)
        {
            uint32 const spellId = state->PendingCompanionSpells[state->NextCompanionSpell++];
            if (!player->HasSpell(spellId))
                player->learnSpell(spellId, false);
        }

        if (state->NextCompanionSpell == state->PendingCompanionSpells.size())
        {
            LOG_INFO("module.ascension_compat", "Completed owned mount/companion spell synchronization for {}", player->GetName());
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
      if ((categoryId <= 14 || categoryId == APPEARANCE_CATEGORY_AMMUNITION) &&
          appearance.PrimaryCategory != categoryId &&
          appearance.SecondaryCategory != categoryId &&
          appearance.TertiaryCategory != categoryId) {
        SendApplyResult(player, "APPLY_APPEARANCES_INVALID_CATEGORY");
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
    RefreshVisibleItems(player);
    SendActiveAppearances(player, *state);
    SendApplyResult(player, "APPLY_APPEARANCES_OK");

    LOG_INFO("module.ascension_compat",
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
    if (!unlockAll && !state->OwnedVanityItems.contains(itemId))
    {
      ChatHandler(player->GetSession())
          .SendSysMessage("That vanity item is not unlocked on this account.");
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

      player->StoreNewItem(destinations, itemId, true);
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

    LOG_INFO("module.ascension_compat",
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
      LOG_INFO("module.ascension_compat", "Sent complete wardrobe snapshot for {}: {} appearances, no per-item login notifications",
          player->GetName(), appearances.size());
      return;
    }

    // A future catalog exceeding our explicit native allocation bound still
    // uses the throttled fallback. Do not split 0x0699 across packets: each
    // native snapshot replaces the previous collection instead of appending.
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

    LOG_INFO("module.ascension_compat",
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

    LOG_INFO("module.ascension_compat",
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
      case CMSG_VANITY_DELIVERY:
        HandleVanityDelivery(player, packet);
        break;
      default:
        break;
      }
    } catch (ByteBufferException const &) {
      LOG_WARN("module.ascension_compat",
               "Malformed Ascension extension packet opcode=0x{:04X} from {}",
               packet.GetOpcode(), player->GetName());
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
      if ((categoryId <= 14 || categoryId == APPEARANCE_CATEGORY_AMMUNITION) &&
          appearance.PrimaryCategory != categoryId &&
          appearance.SecondaryCategory != categoryId &&
          appearance.TertiaryCategory != categoryId) {
        SendApplyResult(player, "APPLY_APPEARANCES_INVALID_CATEGORY");
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

  void HandleVanityDelivery(Player *player, WorldPacket &packet) {
    uint8 action = 0;
    uint32 itemId = 0;
    packet >> action >> itemId;
    if (action != VANITY_DELIVERY_ACTION)
      return;

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

  void SendOutfitCollection(Player *player)
  {
    // The 0x069D handler clears/rebuilds the client's saved-outfit map and,
    // importantly, finalizes the filtered appearance cache by firing
    // VIEWABLE_APPEARANCES_RESET.  The local server does not persist named
    // outfits yet, but it must still send an empty snapshot after collection
    // and active-appearance data or the Wardrobe remains on its pre-login
    // empty page despite showing correct collected/total counts.
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
      vanityItems = _allVanityItemIds;
    else
      vanityItems.assign(state.OwnedVanityItems.begin(),
                         state.OwnedVanityItems.end());

    std::sort(vanityItems.begin(), vanityItems.end());
    WorldPacket packet(SMSG_VANITY_COLLECTION_INFO,
                       sizeof(uint32) + vanityItems.size() * sizeof(uint32));
    packet << static_cast<uint32>(vanityItems.size());
    for (uint32 itemId : vanityItems)
      packet << itemId;

    player->GetSession()->SendPacket(&packet);
  }

  void SendApplyResult(Player *player, char const *result) {
    WorldPacket packet(SMSG_APPLY_APPEARANCES_RESULT, std::strlen(result) + 1);
    packet << result;
    player->GetSession()->SendPacket(&packet);
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

  std::mutex _stateMutex;
  std::unordered_map<uint32, std::shared_ptr<PlayerCollectionState>>
      _playerStates;
};

class AscensionCompatServerScript : public ServerScript {
public:
  AscensionCompatServerScript()
      : ServerScript("AscensionCompatServerScript",
                     {SERVERHOOK_CAN_PACKET_RECEIVE_EARLY, SERVERHOOK_CAN_PACKET_RECEIVE})
  {
  }

    [[nodiscard]] bool CanPacketReceive(WorldSession* session, WorldPacket const& packet) override
    {
        if (!session || !session->GetPlayer() || packet.GetOpcode() != CMSG_CREATURE_QUERY ||
            packet.size() < sizeof(uint32) ||
            !ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED))
            return true;

        uint32 const entry = packet.read<uint32>(0);
        // Existing world creatures retain the normal authoritative query handler.
        // Only missing, evidence-backed collection preview entries get a reply.
        if (sObjectMgr->GetCreatureTemplate(entry))
            return true;

        auto const& models = AscensionCollectionModels::Entries;
        auto const itr = std::lower_bound(models.begin(), models.end(), entry,
            [](AscensionCollectionModels::Entry const& model, uint32 value)
            {
                return model.CreatureId < value;
            });
        if (itr == models.end() || itr->CreatureId != entry)
            return true;

        // Normal WotLK creature-query wire layout, mirrored from QueryHandler.
        // This provides preview metadata only; it does not spawn a creature or
        // invent combat stats/loot for an Ascension NPC absent from the world DB.
        WorldPacket response(SMSG_CREATURE_QUERY_RESPONSE, 100);
        response << entry << std::string(itr->Name);
        for (uint8 i = 0; i < 5; ++i)
            response << uint8(0); // name2/3/4, title, icon
        response << uint32(0) << uint32(CREATURE_TYPE_CRITTER) << uint32(0);
        response << uint32(0) << uint32(0) << uint32(0); // rank, kill credits
        response << itr->DisplayId << uint32(0) << uint32(0) << uint32(0);
        response << float(1.0f) << float(1.0f) << uint8(0);
        for (uint8 i = 0; i < 6; ++i)
            response << uint32(0); // quest items
        response << uint32(0); // movementId
        session->SendPacket(&response);
        LOG_DEBUG("module.ascension_compat", "Answered local creature preview query: entry {}, display {}, payload {}",
            entry, itr->DisplayId, packet.size());
        return false;
    }

  [[nodiscard]] bool CanPacketReceiveEarly(WorldSession *session,
                                           WorldPacket const &packet) override {
    if (!ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED))
      return true;

    uint32 opcode = packet.GetOpcode();
    uint32 firstOpcode = ascensionCompatConfig.GetConfigValue<uint32>(
        AscensionCompatConfig::FIRST_EXTENSION_OPCODE);
    uint32 lastOpcode = ascensionCompatConfig.GetConfigValue<uint32>(
        AscensionCompatConfig::LAST_EXTENSION_OPCODE);

    if (opcode < firstOpcode || opcode > lastOpcode)
      return true;

    if (QueueAscensionManastormPacket(session, packet))
      return false;

    if (opcode == CMSG_APPLY_APPEARANCES ||
        opcode == CMSG_SET_CAN_SEE_APPEARANCES ||
        opcode == CMSG_VANITY_DELIVERY) {
      AscensionCollectionService::Instance().QueueClientPacket(
          session->GetAccountId(), packet);
    }

    // Ascension sends this after the regular CMSG_CAST_SPELL packet to carry
    // client projectile rendering coordinates. AzerothCore has already handled
    // the actual cast, so no server-side action is required for local play.
    if (opcode == CMSG_MISSILE_FIRE_POSITION)
    {
      if (ascensionCompatConfig.GetConfigValue<bool>(
              AscensionCompatConfig::LOG_CONSUMED_PACKETS))
      {
        LOG_INFO("module.ascension_compat",
                 "Consumed Ascension missile-position packet payload={} bytes",
                 packet.size());
      }

      return false;
    }

    if (ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::LOG_CONSUMED_PACKETS)) {
      LOG_INFO("module.ascension_compat",
               "Consumed Ascension extension packet opcode=0x{:04X} ({}) "
               "payload={} bytes",
               opcode, opcode, packet.size());
    }

    return false;
  }
};

class AscensionCompatCommandScript : public CommandScript {
public:
  AscensionCompatCommandScript()
      : CommandScript("AscensionCompatCommandScript") {}

  ChatCommandTable GetCommands() const override {
    static ChatCommandTable commandTable = {
        {"localfreshcheck", HandleAscensionFreshCharacterCheck, SEC_ADMINISTRATOR, Console::Yes},
        {"localappearance", HandleLocalAppearanceCommand, SEC_PLAYER,
         Console::No},
        {"localvanity", HandleLocalVanityCommand, SEC_PLAYER, Console::No},
        {"localtalent", HandleLocalTalentCommand, SEC_PLAYER, Console::No},
        {"localspec", HandleLocalSpecCommand, SEC_PLAYER, Console::No},
        {"localresource", HandleLocalResourceCommand, SEC_PLAYER,
         Console::No},
        {"localcharges", HandleLocalChargesCommand, SEC_PLAYER, Console::No},
        {"localclassrepair", HandleLocalClassRepairCommand, SEC_PLAYER,
         Console::No}};
    return commandTable;
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

  static bool HandleLocalTalentCommand(ChatHandler *handler, uint32 entryId,
                                       uint32 rank) {
    Player *player = handler->GetPlayer();
    if (!player)
      return false;

    auto itr = std::lower_bound(
        AscensionCompatData::CoATalentEntries.begin(),
        AscensionCompatData::CoATalentEntries.end(), entryId,
        [](AscensionCompatData::CoATalentEntry const &entry, uint32 id) {
          return entry.EntryId < id;
        });
    if (itr == AscensionCompatData::CoATalentEntries.end() ||
        itr->EntryId != entryId) {
      handler->PSendSysMessage(
          "Talent entry {} is not in the local CoA catalog.", entryId);
      return true;
    }

    if (itr->ClassId != player->getClass())
    {
      handler->PSendSysMessage(
          "Talent entry {} does not belong to your custom class.", entryId);
      return true;
    }

    uint32 activeSpecialization =
        AscensionClassService::Instance().GetActiveSpecialization(player);
    if (rank > 0 && itr->SpecId != 0 && !activeSpecialization)
    {
      AscensionClassService::Instance().SwitchSpecialization(player,
                                                              itr->SpecId);
      activeSpecialization =
          AscensionClassService::Instance().GetActiveSpecialization(player);
    }

    if (rank > 0 && itr->SpecId != 0 &&
        itr->SpecId != activeSpecialization)
    {
      handler->PSendSysMessage(
          "Talent entry {} belongs to specialization {}, but your active "
          "local specialization is {}.",
          entryId, uint32(itr->SpecId), activeSpecialization);
      return true;
    }

    if (rank > itr->SpellCount)
    {
      handler->PSendSysMessage("Talent entry {} only has {} rank(s).", entryId,
                               uint32(itr->SpellCount));
      return true;
    }

    uint32 const freeChoiceGroup = AscensionClassService::GetSelectableFreeGroup(entryId);
    bool const automaticallyGranted = itr->AECost == 0 && itr->TECost == 0 && !freeChoiceGroup;
    if (automaticallyGranted && rank != 0 && rank != itr->SpellCount)
    {
      handler->PSendSysMessage(
          "Progression entry {} must use its full automatic rank.", entryId);
      return true;
    }

    if (rank > 0 && player->GetLevel() < itr->RequiredLevel)
    {
      handler->PSendSysMessage("Talent entry {} requires level {}.", entryId,
                               uint32(itr->RequiredLevel));
      return true;
    }

    if (automaticallyGranted)
    {
        // A UI synchronization request cannot bypass automatic prerequisites.
        // Automatic ranks are immutable; paid talent choices remain below.
        if (rank > 0 && !AscensionClassService::CanGrantAutomaticEntry(player, *itr, activeSpecialization))
            handler->PSendSysMessage("Progression entry {} requires its prerequisite ability.", entryId);
        AscensionClassService::Instance().SynchronizeProgression(player);
        return true;
    }

    uint32 selectedSpellId = rank > 0 ? itr->SpellIds[rank - 1] : 0;
    if (rank > 0 && (!selectedSpellId || !sSpellMgr->GetSpellInfo(selectedSpellId)))
    {
        handler->PSendSysMessage(
            "Talent entry {} rank {} references a missing server spell.", entryId, rank);
        return true;
    }

    if (rank > 0 && freeChoiceGroup)
    {
        // An explicit player selection resolves a group. Login must not choose
        // between alternatives previously double-granted by the old free rule.
        for (auto const& other : AscensionCompatData::CoATalentEntries)
            if (other.ClassId == player->getClass() && other.SpecId == itr->SpecId && other.EntryId != entryId &&
                AscensionClassService::GetSelectableFreeGroup(other.EntryId) == freeChoiceGroup)
                for (uint32 spellId : other.SpellIds)
                    if (spellId && player->HasSpell(spellId))
                        player->removeSpell(spellId, SPEC_MASK_ALL, false);
    }

    for (uint32 spellId : itr->SpellIds) {
      if (spellId && player->HasSpell(spellId))
        player->removeSpell(spellId, SPEC_MASK_ALL, false);
    }

    if (rank > 0)
        player->learnSpell(selectedSpellId, false);

    AscensionClassService::Instance().SynchronizeProgression(player);

    LOG_INFO("module.ascension_compat",
             "Set local CoA talent entry {} to rank {} for {} (class {})",
             entryId, rank, player->GetName(), uint32(player->getClass()));
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
};

class AscensionCompatPlayerScript : public PlayerScript {
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
             PLAYERHOOK_ON_SEND_INITIAL_PACKETS_BEFORE_ADD_TO_MAP}) {}

    void OnPlayerGetAmmoDisplay(Player* player, SpellInfo const* spellInfo,
        uint32& displayId, uint32& inventoryType) override
    {
        // Only ranged auto-attacks. Authored missiles on abilities, thrown weapons and wands stay native.
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

    void OnPlayerAfterUpdateAttackPowerAndDamage(Player* player, float& /*level*/, float& /*baseAttackPower*/,
        float& modifier, float& /*multiplier*/, bool ranged) override
    {
        if (ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED))
            HandleAscensionBarbarianAttackPower(player, modifier, ranged);
    }

    void OnPlayerSendInitialPacketsBeforeAddToMap(Player* player, WorldPacket& /*data*/) override
    {
        if (ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED))
            AscensionCollectionService::Instance().PrepareOwnedCompanionsBeforeMap(player);
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

  void OnPlayerLogin(Player *player) override {
    if (ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED)) {
      AscensionClassService::Instance().OnPlayerLogin(player);
      SynchronizeAscensionClassMechanics(player);
      AscensionResourceService::Instance().OnPlayerLogin(player);
      AscensionCollectionService::Instance().OnPlayerLogin(player);
    }
  }

  void OnPlayerLevelChanged(Player *player, uint8 /*oldLevel*/) override {
    if (ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED))
    {
      AscensionClassService::Instance().SynchronizeProgression(player);
      AscensionClassService::Instance().SynchronizeProficiencies(player);

      // Quest templates are shared globally, so scaling is serialized per
      // player. Refresh accepted quest query data when the player's effective
      // quest level changes; this keeps the quest log in sync without mutating
      // the canonical template for anyone else.
      if (LocalLevelScaling::QuestEnabled.load(std::memory_order_relaxed))
      {
        for (auto const& [questId, status] : player->getQuestStatusMap())
        {
          if (status.Status != QUEST_STATUS_INCOMPLETE &&
              status.Status != QUEST_STATUS_COMPLETE &&
              status.Status != QUEST_STATUS_FAILED)
            continue;

          if (Quest const* quest = sObjectMgr->GetQuestTemplate(questId))
            if (quest->GetQuestLevel() <= 0 || quest->GetQuestLevel() < player->GetLevel())
              player->PlayerTalkClass->SendQuestQueryResponse(quest);
        }
      }
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

    if (ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED) &&
        AscensionClassService::Instance().AffectsProficiencies(spellId))
      AscensionClassService::Instance().SynchronizeProficiencies(player);
  }

  void OnPlayerForgotSpell(Player *player, uint32 spellId) override {
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

    if (ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED) &&
        AscensionClassService::Instance().AffectsProficiencies(spellId))
      AscensionClassService::Instance().SynchronizeProficiencies(player);
  }

    void OnPlayerAfterSpecSlotChanged(Player* player, uint8 /*newSlot*/) override
    {
        if (ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED))
            AscensionClassService::ReconcileRunemasterFists(player,
                AscensionClassService::Instance().GetActiveSpecialization(player));
    }

  void OnPlayerLogout(Player *player) override {
    AscensionClassService::Instance().OnPlayerLogout(player);
    AscensionResourceService::Instance().OnPlayerLogout(player);
    AscensionCollectionService::Instance().OnPlayerLogout(player);
  }

  void OnPlayerUpdate(Player *player, uint32 diff) override {
    if (ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED)) {
      AscensionResourceService::Instance().OnPlayerUpdate(player);
      AscensionCollectionService::Instance().OnPlayerUpdate(player, diff);
    }
  }

  void OnPlayerAfterSetVisibleItemSlot(Player *player, uint8 slot,
                                       Item *item) override {
    AscensionCollectionService::Instance().OnVisibleItemSet(player, slot, item);
  }

  void OnPlayerEquip(Player *player, Item *item, uint8 /*bag*/, uint8 /*slot*/,
                     bool /*update*/) override {
    AscensionCollectionService::Instance().OnItemObtained(player, item);
  }

  void OnPlayerStoreNewItem(Player *player, Item *item,
                            uint32 /*count*/) override {
    AscensionCollectionService::Instance().OnItemObtained(player, item);
  }

  void OnPlayerCreateItem(Player *player, Item *item,
                           uint32 /*count*/) override {
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

    void OnSpellCheckCast(Spell* spell, bool /*strict*/,
        SpellCastResult& result) override
    {
        if (ascensionCompatConfig.GetConfigValue<bool>(
                AscensionCompatConfig::ENABLED))
            AscensionResourceService::Instance().CheckCast(spell, result);
    }

    [[nodiscard]] bool CanPrepare(Spell* spell,
        SpellCastTargets const* /*targets*/,
        AuraEffect const* /*triggeredByAura*/) override
    {
        if (!ascensionCompatConfig.GetConfigValue<bool>(
                AscensionCompatConfig::ENABLED))
            return true;

        return CanPrepareAscensionClassMechanics19To25(spell) &&
            AscensionResourceService::Instance().CanPrepare(spell);
    }

    void OnSpellCast(Spell* spell, Unit* /*caster*/,
        SpellInfo const* /*spellInfo*/, bool /*skipCheck*/) override
    {
        if (ascensionCompatConfig.GetConfigValue<bool>(
                AscensionCompatConfig::ENABLED))
        {
            AscensionResourceService::Instance().OnSpellCast(spell);
            HandleAscensionClassMechanicsCast(spell);
        }
    }

    void OnSpellBeforeEffects(Spell* spell, Unit* /*caster*/,
        SpellInfo const* /*spellInfo*/) override
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

    void OnSpellSuccessfulInterrupt(Spell* spell, Unit* /*target*/) override
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
            {UNITHOOK_ON_DAMAGE, UNITHOOK_ON_BLOCK,
             UNITHOOK_ON_PERIODIC_DAMAGE_RESULT,
             UNITHOOK_ON_AURA_APPLY, UNITHOOK_ON_AURA_REMOVE,
             UNITHOOK_ON_SEND_AURA_UPDATE}) {}

    void OnSendAuraUpdate(Unit* target, Player* receiver,
        AuraApplication const* application, bool remove) override
    {
        if (ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED))
            SendAscensionAuraAmounts(target, receiver, application, remove);
    }

  void OnDamage(Unit* /*attacker*/, Unit* victim, uint32& damage) override {
    if (!ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED) ||
        !victim || !victim->IsPlayer())
      return;

    HandleAscensionClassMechanicsDamageTaken(victim->ToPlayer(), damage);
  }

  void OnBlock(Unit *victim, Unit * /*attacker*/) override {
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
            ApplyAscensionClassMechanics(spellInfo);
            ApplyAscensionPrimalistEarthshapingContracts(spellInfo);
            ApplyAscensionPrimalistSpiritBeastContract(spellInfo);
            ApplyAscensionVenomancerCatalystContract(spellInfo);
            ApplyAscensionReaperDeathwindContracts(spellInfo);
        }
    }
};

class AscensionCompatLevelScalingScript : public AllCreatureScript
{
public:
  AscensionCompatLevelScalingScript()
      : AllCreatureScript("AscensionCompatLevelScalingScript") {}

  void OnBeforeCreatureSelectLevel(CreatureTemplate const* /*creatureTemplate*/,
                                   Creature* creature, uint8& level) override
  {
    if (!CanScale(creature))
      return;

    uint64 guid = creature->GetGUID().GetRawValue();
    uint8 original = level;
    {
      std::lock_guard<std::mutex> guard(_lock);
      auto [itr, inserted] = _states.try_emplace(guid, State{level, 1000});
      original = itr->second.Original;
      if (inserted)
        itr->second.Original = level;
    }

    level = DesiredLevel(creature, original);
  }

  void OnAllCreatureUpdate(Creature* creature, uint32 diff) override
  {
    if (!CanScale(creature) || creature->IsInCombat() || !creature->IsAlive() ||
        creature->GetHealth() != creature->GetMaxHealth())
      return;

    uint64 guid = creature->GetGUID().GetRawValue();
    uint8 original;
    {
      std::lock_guard<std::mutex> guard(_lock);
      State& state = _states.try_emplace(guid, State{creature->GetLevel(), 1000}).first->second;
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

    // SelectLevel reuses stock health, mana, attack-power and damage curves. It
    // runs only while full and out of combat, so an active fight never heals or
    // changes level underneath the player.
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
    std::lock_guard<std::mutex> guard(_lock);
    _states.erase(creature->GetGUID().GetRawValue());
  }

private:
  struct State
  {
    uint8 Original;
    uint32 Timer;
  };

  static bool CanScale(Creature const* creature)
  {
    return LocalLevelScaling::CreatureEnabled.load(std::memory_order_relaxed) && creature &&
        !creature->GetMap()->IsScriptedPrivateInstance() &&
        !creature->IsPet() && !creature->IsTotem() && !creature->IsTrigger() && !creature->IsCritter() &&
        creature->GetCreatureType() != CREATURE_TYPE_NON_COMBAT_PET && !creature->GetCharmerOrOwner();
  }

  static uint8 DesiredLevel(Creature const* creature, uint8 original)
  {
    Map* map = creature->GetMap();
    if (!map)
      return original;

    uint8 desired = original;
    float range = creature->GetSightRange();
    for (auto const& reference : map->GetPlayers())
    {
      Player* player = reference.GetSource();
      if (!player || !player->IsAlive() || player->IsGameMaster() ||
          !creature->InSamePhase(player) || !creature->IsWithinDistInMap(player, range) ||
          !player->IsValidAttackTarget(creature))
        continue;
      desired = std::max(desired, LocalLevelScaling::ScaleCreatureLevel(original, player->GetLevel(),
          LocalLevelScaling::CreatureOffset.load(std::memory_order_relaxed)));
    }
    return desired;
  }

  std::mutex _lock;
  std::unordered_map<uint64, State> _states;
};

class AscensionCompatWorldScript : public WorldScript {
public:
  AscensionCompatWorldScript()
      : WorldScript("AscensionCompatWorldScript",
                    {WORLDHOOK_ON_BEFORE_CONFIG_LOAD, WORLDHOOK_ON_STARTUP}) {}

  void OnBeforeConfigLoad(bool reload) override {
    ascensionCompatConfig.Initialize(reload);
    bool enabled = ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED);
    LocalLevelScaling::CreatureEnabled.store(enabled && ascensionCompatConfig.GetConfigValue<bool>(
        AscensionCompatConfig::LEVEL_SCALING), std::memory_order_relaxed);
    LocalLevelScaling::QuestEnabled.store(enabled && ascensionCompatConfig.GetConfigValue<bool>(
        AscensionCompatConfig::QUEST_LEVEL_SCALING), std::memory_order_relaxed);
  }

  void OnStartup() override {
    if (!ascensionCompatConfig.GetConfigValue<bool>(
            AscensionCompatConfig::ENABLED))
      return;

    uint32 firstOpcode = ascensionCompatConfig.GetConfigValue<uint32>(
        AscensionCompatConfig::FIRST_EXTENSION_OPCODE);
    uint32 lastOpcode = ascensionCompatConfig.GetConfigValue<uint32>(
        AscensionCompatConfig::LAST_EXTENSION_OPCODE);
    std::filesystem::path dbcDirectory(
        std::string(ascensionCompatConfig.GetConfigValue(
            AscensionCompatConfig::DBC_DIRECTORY)));

    bool dataLoaded =
        AscensionCollectionService::Instance().LoadClientData(dbcDirectory);
    AscensionResourceService::Instance().ValidateDefinitions();
    LOG_INFO("module.ascension_compat",
             "Ascension compatibility enabled; consuming extension opcodes "
             "0x{:04X}-0x{:04X}; collection data {}",
             firstOpcode, lastOpcode, dataLoaded ? "ready" : "unavailable");
  }
};

// Tradesman's Scroll.
//
// Ascension's scroll opens a gossip listing fifteen professions and sets the
// chosen one to its maximum skill. Captured from the live realm, its text reads:
//
//   "Select a Profession from the list below to receive Max Skill Level in it.
//    You will still need to train the recipes from items and trainers."
//
// and the item itself says "Maxes out a Profession of your choice. You will need
// to learn the profession as well." Both halves matter: the scroll raises the
// skill VALUE, it does not grant the skill and it does not teach any recipe.
// A player who has not learned the profession gets told so rather than silently
// gaining nothing.
//
// The option list is exactly the fifteen the live scroll offered, in the order
// it offered them -- note that it includes Lockpicking, which the Book of
// Artisans does not train, and excludes Jewelcrafting, Inscription and
// Bushcraft, which it does.
//
// This is an ItemScript rather than a creature: the scroll has no companion NPC
// (unlike the Books, which summon one), and ItemScript exposes both OnUse and
// OnGossipSelect, so the whole interaction lives on the item.

struct ScrollProfession
{
    uint32 skillId;
    char const* name;
};

// Order and membership taken from the captured gossip, not from a profession
// enum -- the scroll's list is its own thing.
constexpr ScrollProfession kProfessions[] = {
    { 171, "Alchemy" },        { 164, "Blacksmithing" }, { 333, "Enchanting" },
    { 202, "Engineering" },    { 165, "Leatherworking" }, { 197, "Tailoring" },
    { 182, "Herbalism" },      { 186, "Mining" },        { 393, "Skinning" },
    { 185, "Cooking" },        { 129, "First Aid" },     { 356, "Fishing" },
    { 633, "Lockpicking" },    { 732, "Woodcutting" },   { 757, "Woodworking" },
};

constexpr uint32 kGossipTextId = 1;      // generic; the options carry the meaning
constexpr uint32 kSenderScroll = 0xA5C0; // distinctive, so stray gossip cannot match

class AscensionTradesmanScroll : public ItemScript
{
public:
    AscensionTradesmanScroll() : ItemScript("ascension_tradesman_scroll") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
    {
        if (!player || !item)
            return false;

        ClearGossipMenuFor(player);
        for (uint32 i = 0; i < std::extent<decltype(kProfessions)>::value; ++i)
        {
            ScrollProfession const& prof = kProfessions[i];
            // Show what the player will actually get. A profession they have
            // not learned is still listed -- the live scroll listed all fifteen
            // regardless -- but the label says so up front.
            std::string label = prof.name;
            if (!player->HasSkill(prof.skillId))
                label += " (not learned)";
            else if (player->GetSkillValue(prof.skillId) >= player->GetPureMaxSkillValue(prof.skillId))
                label += " (already maxed)";

            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, label, kSenderScroll, i);
        }

        SendGossipMenuFor(player, kGossipTextId, item->GetGUID());
        // true suppresses the item's own on-use spell: the gossip is the effect.
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

        // The scroll raises a skill; it never grants one. Learning the
        // profession is a separate step, exactly as the item text says.
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

        // Raise to the cap the player's current rank allows -- an Apprentice is
        // maxed at 75, not 450. Advancing further still means learning the next
        // rank from a trainer, which is what "Max Skill Level" meant on live.
        player->SetSkill(prof.skillId, player->GetSkillStep(prof.skillId), cap, cap);
        ChatHandler(player->GetSession()).PSendSysMessage(
            "{} raised to {}.", prof.name, cap);

        LOG_DEBUG("module.ascension_compat",
                  "Tradesman's Scroll: player {} set {} to {}",
                  player->GetName(), prof.name, cap);

        // Consume one scroll, matching a single-use consumable.
        player->DestroyItemCount(item->GetEntry(), 1, true);
    }
};

// Ascension mount buttons frequently cast a wrapper, not the riding aura.
// Resolve only validated catalog wrappers, using the same zone/riding rules
// as AzerothCore's spell_gen_mount and the matching client spell variants.
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
        // Validate runs on the registration instance; bind this cast instance too.
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
        if ((area && (area->flags & AREA_FLAG_NO_FLY_ZONE)) || (battlefield && !battlefield->CanFlyIn()))
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

} // namespace

bool IsAscensionPrimalistTameEligible(Player const* player)
{
    return player && ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED) &&
        player->getClass() == CLASS_WILDWALKER && player->GetLevel() >= 10 && player->HasSpell(92148) &&
        AscensionClassService::Instance().GetActiveSpecialization(player) == 59;
}

void AddAscensionCompatScripts() {
  RegisterSpellScript(spell_ascension_local_mount);
  new AscensionTradesmanScroll();
  new AscensionCompatServerScript();
  new AscensionCompatCommandScript();
  new AscensionCompatPlayerScript();
  new AscensionCompatAllSpellScript();
  new AscensionCompatUnitScript();
  new AscensionCompatChangelogScript();
  new AscensionCompatLevelScalingScript();
  new AscensionCompatWorldScript();
}
