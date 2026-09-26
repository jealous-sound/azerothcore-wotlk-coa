/*
 * mod-coa-prestige - Chromie's Prestige Mode for Conquest of Azeroth.
 *
 * A character at the Prestige level speaks to Chromie (creature 178081) and lets her wind back
 * time: the character returns to level 1 in its racial starting zone, the active specialization
 * loses its talents and abilities, zone quests, exotic pets and temporary buffs and debuffs are
 * reset, and items, gold, currencies and reputation are kept. The specialization stays locked
 * until the character is back at the Prestige level, and while Prestige Mode is active the
 * Prestige Challenge aura multiplies the experience the character earns. Every activation pays
 * the Prestige rewards and raises the character's Prestige level.
 *
 * The client carries the whole interface. AscensionUI/Gossips/PrestigeMode.lua replaces Chromie's
 * gossip window with the Prestige window and drives it through three gossip options it finds by
 * their exact text; GetPrestigeLevel() reads the character and account Prestige levels from the
 * SMSG_UPDATE_OBJECT_ADDON field table; C_Player:IsPrestiged() is HasAura(9930831).
 */

#include "AscensionSpecialization.h"
#include "CoAPrestigeRules.h"
#include "Chat.h"
#include "Config.h"
#include "CreatureScript.h"
#include "DatabaseEnv.h"
#include "Duration.h"
#include "Item.h"
#include "Log.h"
#include "Mail.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Pet.h"
#include "Player.h"
#include "PlayerScript.h"
#include "PlayerSettings.h"
#include "QueryCallback.h"
#include "QuestDef.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "WorldPacket.h"
#include "WorldScript.h"
#include "WorldSession.h"

#include <atomic>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

namespace
{
    using namespace CoAPrestige;

    constexpr char DefaultRewards[] = "1287330:1 375250:100000 90004:100";
    constexpr uint8 FirstLevel = 1;

    enum ChromieAction : uint32
    {
        ACTION_ACTIVATE = 1,
        ACTION_REWARDS = 2,
        ACTION_EXPERIENCE_ITEMS = 3
    };

    std::atomic<bool> g_enabled{ true };
    std::atomic<uint32> g_requiredLevel{ 60 };
    std::atomic<uint32> g_experienceBonusPercent{ 150 };

    std::mutex g_rewardsLock;
    std::vector<RewardItem> g_rewards;

    std::mutex g_accountLock;
    std::unordered_map<ObjectGuid::LowType, uint32> g_otherCharactersPrestige;

    std::vector<RewardItem> Rewards()
    {
        std::lock_guard<std::mutex> guard(g_rewardsLock);
        return g_rewards;
    }

    void LoadConfig()
    {
        g_enabled = sConfigMgr->GetOption<bool>("CoAPrestige.Enable", true);
        g_requiredLevel = std::max<uint32>(2, sConfigMgr->GetOption<uint32>("CoAPrestige.RequiredLevel", 60));
        g_experienceBonusPercent = sConfigMgr->GetOption<uint32>("CoAPrestige.ExperienceBonusPercent", 150);

        std::string const text = sConfigMgr->GetOption<std::string>("CoAPrestige.Rewards", DefaultRewards);
        std::optional<std::vector<RewardItem>> rewards = ParseRewards(text);
        if (!rewards)
        {
            LOG_ERROR("module.coa_prestige", "CoAPrestige.Rewards \"{}\" is not a list of item:count pairs; "
                "Prestige Mode pays no rewards.", text);
            rewards.emplace();
        }

        std::lock_guard<std::mutex> guard(g_rewardsLock);
        g_rewards = std::move(*rewards);
    }

    std::vector<uint32> SettingValues(Player const* player, char const* source)
    {
        std::vector<uint32> values;
        if (PlayerSettingVector const* settings = player->FindPlayerSettings(source))
            for (PlayerSetting const& setting : *settings)
                values.push_back(setting.value);
        return values;
    }

    void WriteSetting(Player* player, char const* source, std::vector<uint32> const& values)
    {
        std::size_t const previous = SettingValues(player, source).size();
        for (std::size_t index = 0; index < values.size(); ++index)
            player->UpdatePlayerSetting(source, uint32(index), values[index]);
        for (std::size_t index = values.size(); index < previous; ++index)
            player->UpdatePlayerSetting(source, uint32(index), 0);
    }

    State LoadState(Player const* player)
    {
        return DecodeState(SettingValues(player, StateSetting));
    }

    void SaveState(Player* player, State const& state)
    {
        WriteSetting(player, StateSetting, EncodeState(state));
    }

    uint32 OtherCharactersPrestige(Player const* player)
    {
        std::lock_guard<std::mutex> guard(g_accountLock);
        auto const found = g_otherCharactersPrestige.find(player->GetGUID().GetCounter());
        return found == g_otherCharactersPrestige.end() ? 0 : found->second;
    }

    void SendPrestigeLevels(Player* player)
    {
        uint32 const level = LoadState(player).level;
        WorldPacket data(AddonFieldOpcode, 8 + 2 * 8);
        data << player->GetGUID()
             << AddonFieldPrestigeLevel << level
             << AddonFieldAccountPrestigeLevel << (level + OtherCharactersPrestige(player));
        player->SendDirectMessage(&data);
    }

    // Other characters of the account are not in memory: their Prestige levels come from the
    // setting their own saves wrote, read once at login.
    void LoadOtherCharactersPrestige(Player* player)
    {
        CharacterDatabasePreparedStatement* stmt =
            CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_CHARACTER_SETTINGS_BY_SOURCE);
        stmt->SetData(0, player->GetSession()->GetAccountId());
        stmt->SetData(1, player->GetGUID().GetCounter());
        stmt->SetData(2, std::string(StateSetting));

        ObjectGuid const guid = player->GetGUID();
        player->GetSession()->GetQueryProcessor().AddCallback(CharacterDatabase.AsyncQuery(stmt).WithPreparedCallback(
            [guid](PreparedQueryResult result)
            {
                Player* player = ObjectAccessor::FindConnectedPlayer(guid);
                if (!player)
                    return;

                uint32 total = 0;
                if (result)
                {
                    do
                    {
                        PlayerSettingVector const values =
                            PlayerSettingsStore::ParseSettingsData(result->Fetch()[0].Get<std::string>());
                        if (!values.empty())
                            total += values[0].value;
                    } while (result->NextRow());
                }

                {
                    std::lock_guard<std::mutex> lock(g_accountLock);
                    g_otherCharactersPrestige[guid.GetCounter()] = total;
                }
                if (player->IsInWorld())
                    SendPrestigeLevels(player);
            }));
    }

    void RememberActionBar(Player* player)
    {
        std::vector<BarSlot> bar;
        for (uint8 button = 0; button < MAX_ACTION_BUTTONS; ++button)
            if (ActionButton const* action = player->GetActionButton(button))
                if (action->GetType() == ACTION_BUTTON_SPELL && action->GetAction())
                    bar.push_back({ button, action->GetAction() });
        WriteSetting(player, BarSetting, EncodeBar(bar));
    }

    void ClearForgottenActionButtons(Player* player)
    {
        for (uint8 button = 0; button < MAX_ACTION_BUTTONS; ++button)
            if (ActionButton const* action = player->GetActionButton(button))
                if (action->GetType() == ACTION_BUTTON_SPELL && !player->HasSpell(action->GetAction()))
                    player->removeActionButton(button);
    }

    void DismissPets(Player* player)
    {
        auto isExotic = [](uint32 creatureId)
        {
            CreatureTemplate const* creature = sObjectMgr->GetCreatureTemplate(creatureId);
            return creature && creature->IsExotic();
        };
        auto isExoticHunterPet = [&](PetStable::PetInfo const& info)
        {
            return info.Type == HUNTER_PET && isExotic(info.CreatureId);
        };

        if (Pet* pet = player->GetPet())
            player->RemovePet(pet, pet->getPetType() == HUNTER_PET && isExotic(pet->GetEntry())
                ? PET_SAVE_AS_DELETED : PET_SAVE_NOT_IN_SLOT);

        if (PetStable* stable = player->GetPetStable())
        {
            if (stable->CurrentPet && isExoticHunterPet(*stable->CurrentPet))
            {
                Pet::DeleteFromDB(stable->CurrentPet->PetNumber);
                stable->CurrentPet.reset();
                player->SetTemporaryUnsummonedPetNumber(0);
            }

            for (Optional<PetStable::PetInfo>& stabled : stable->StabledPets)
                if (stabled && isExoticHunterPet(*stabled))
                {
                    Pet::DeleteFromDB(stabled->PetNumber);
                    stabled.reset();
                }

            std::erase_if(stable->UnslottedPets, [&](PetStable::PetInfo const& info)
            {
                if (!isExoticHunterPet(info))
                    return false;
                Pet::DeleteFromDB(info.PetNumber);
                return true;
            });
        }

        // A pet put away by mounting is not GetPet(), and the dismount below would bring it back.
        if (player->GetTemporaryUnsummonedPetNumber())
        {
            player->RemovePet(nullptr, PET_SAVE_NOT_IN_SLOT);
            player->SetTemporaryUnsummonedPetNumber(0);
        }

        player->RemoveAllControlled();
        player->UnsummonAllTotems();
    }

    void ResetQuests(Player* player, uint32 maxLevel)
    {
        for (uint16 slot = 0; slot < MAX_QUEST_LOG_SIZE; ++slot)
        {
            uint32 const questId = player->GetQuestSlotQuestId(slot);
            if (!questId)
                continue;

            player->TakeQuestSourceItem(questId, false);
            player->AbandonQuest(questId);
            player->RemoveTimedQuest(questId);
            player->SetQuestSlot(slot, 0);
            player->RemoveActiveQuest(questId, false);
        }

        std::vector<uint32> replayable;
        for (uint32 questId : player->getRewardedQuests())
            if (Quest const* quest = sObjectMgr->GetQuestTemplate(questId))
            {
                QuestTraits const traits{ quest->GetZoneOrSort(), quest->GetQuestLevel(), quest->GetType(),
                    quest->IsRepeatable() || quest->IsDailyOrWeekly() || quest->IsMonthly() || quest->IsSeasonal() ||
                    quest->IsDFQuest() };
                if (IsReplayableQuest(traits, maxLevel))
                    replayable.push_back(questId);
            }

        for (uint32 questId : replayable)
            player->RemoveRewardedQuest(questId, false);
    }

    // Upgraded ranks above the new level, which CoA's level pass never takes back. First ranks are
    // left to that pass: it knows which ones a racial, baseline or proficiency grant still allows.
    // The returned chains are the ones whose remaining lower rank has to be made usable again.
    std::unordered_set<uint32> ForgetRanksAbove(Player* player, uint8 level)
    {
        std::unordered_set<uint32> chains;
        for (AscensionClassAbility const& ability : GetAscensionClassAbilities(player->getClass()))
            if (ability.SpellId != ability.FirstSpellId && ability.RequiredLevel > level &&
                player->HasSpell(ability.SpellId))
            {
                chains.insert(sSpellMgr->GetFirstSpellInChain(ability.SpellId));
                player->removeSpell(ability.SpellId, SPEC_MASK_ALL, false);
            }
        return chains;
    }

    // Removing a rank leaves the rank below it known but inactive, so it cannot be cast until the
    // next login. Learning the highest rank left again makes it the active one.
    void ReactivateRemainingRanks(Player* player, std::unordered_set<uint32> const& chains)
    {
        for (uint32 const firstRank : chains)
        {
            uint32 highest = 0;
            for (uint32 rank = firstRank; rank; rank = sSpellMgr->GetNextSpellInChain(rank))
                if (player->HasSpell(rank))
                    highest = rank;

            if (!highest || player->HasActiveSpell(highest))
                continue;

            player->removeSpell(highest, SPEC_MASK_ALL, false);
            player->learnSpell(highest, false);
        }
    }

    void StripTemporaryAuras(Player* player)
    {
        player->RemoveAurasByType(SPELL_AURA_MOUNTED);
        player->RemoveOwnedAuras([](Aura const* aura)
        {
            return !aura->IsPassive() && !aura->IsPermanent() && aura->GetId() != PrestigedAura;
        });
    }

    // The item overload of CanUseItem also checks the weapon or armor proficiency, which a
    // forgotten talent may have granted.
    void UnequipUnusableItems(Player* player, std::vector<Item*>& mailed, CharacterDatabaseTransaction trans)
    {
        for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
        {
            Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
            if (!item || player->CanUseItem(item, false) == EQUIP_ERR_OK)
                continue;

            ItemPosCountVec destination;
            if (player->CanStoreItem(NULL_BAG, NULL_SLOT, destination, item, false) == EQUIP_ERR_OK)
            {
                player->RemoveItem(INVENTORY_SLOT_BAG_0, slot, true);
                player->StoreItem(destination, item, true);
                continue;
            }

            player->MoveItemFromInventory(INVENTORY_SLOT_BAG_0, slot, true);
            item->DeleteFromInventoryDB(trans);
            item->SaveToDB(trans);
            mailed.push_back(item);
        }

        player->AutoUnequipOffhandIfNeed();
    }

    void GrantRewards(Player* player, std::vector<RewardItem> const& rewards, std::vector<Item*>& mailed,
                      CharacterDatabaseTransaction trans)
    {
        for (RewardItem const& reward : rewards)
        {
            if (!sObjectMgr->GetItemTemplate(reward.item))
            {
                LOG_ERROR("module.coa_prestige", "Prestige reward item {} does not exist.", reward.item);
                continue;
            }

            uint32 missing = 0;
            ItemPosCountVec destination;
            if (player->CanStoreNewItem(NULL_BAG, NULL_SLOT, destination, reward.item, reward.count, &missing) !=
                    EQUIP_ERR_OK && !missing)
            {
                missing = reward.count;
                destination.clear();
            }

            if (uint32 const stored = reward.count - missing; stored && !destination.empty())
                if (Item* item = player->StoreNewItem(destination, reward.item, true,
                        Item::GenerateItemRandomPropertyId(reward.item)))
                    player->SendNewItem(item, stored, true, false);

            uint32 const stack = std::max<uint32>(1, sObjectMgr->GetItemTemplate(reward.item)->GetMaxStackSize());
            for (uint32 remaining = missing; remaining;)
            {
                uint32 const count = std::min(remaining, stack);
                remaining -= count;
                if (Item* item = Item::CreateItem(reward.item, count, player))
                {
                    item->SaveToDB(trans);
                    mailed.push_back(item);
                }
            }
        }
    }

    void MailItems(Player* player, std::vector<Item*> const& items, CharacterDatabaseTransaction trans)
    {
        for (std::size_t first = 0; first < items.size(); first += MAX_MAIL_ITEMS)
        {
            MailDraft draft("Prestige Mode", "Chromie could not fit these into your bags.");
            for (std::size_t index = first; index < std::min(items.size(), first + MAX_MAIL_ITEMS); ++index)
                draft.AddItem(items[index]);
            draft.SendMailTo(trans, MailReceiver(player), MailSender(MAIL_CREATURE, ChromieEntry),
                MAIL_CHECK_MASK_COPIED);
        }
    }

    bool Activate(Player* player)
    {
        State state = LoadState(player);
        uint32 const requiredLevel = g_requiredLevel;
        uint32 const specialization = GetAscensionActiveSpecialization(player);

        ActivationFacts facts;
        facts.enabled = g_enabled;
        facts.customClass = IsAscensionCustomClassId(player->getClass());
        facts.level = player->GetLevel();
        facts.requiredLevel = requiredLevel;
        facts.active = state.active;
        facts.specialization = specialization;
        facts.alive = player->IsAlive();
        facts.inCombat = player->IsInCombat();
        facts.travelling = player->IsInFlight() || player->GetVehicle() || player->GetTransport();
        facts.inInstance = player->GetMap()->Instanceable();

        ChatHandler chat(player->GetSession());
        if (Refusal const refusal = CheckActivation(facts); refusal != Refusal::None)
        {
            chat.SendSysMessage(RefusalMessage(refusal, requiredLevel));
            return false;
        }

        PlayerInfo const* start = sObjectMgr->GetPlayerInfo(player->getRace(true), player->getClass());
        if (!start)
        {
            LOG_ERROR("module.coa_prestige", "No starting location for race {} class {}: {} cannot prestige.",
                uint32(player->getRace(true)), uint32(player->getClass()), player->GetName());
            chat.SendSysMessage("Chromie cannot find where your journey began.");
            return false;
        }

        RememberActionBar(player);
        DismissPets(player);
        ResetQuests(player, requiredLevel);

        uint32 const talents = ForgetAscensionClassTalents(player);
        std::unordered_set<uint32> const chains = ForgetRanksAbove(player, FirstLevel);
        player->GiveLevel(FirstLevel);
        player->SetUInt32Value(PLAYER_XP, 0);
        player->SetRestBonus(player->GetRestBonus());
        ClearForgottenActionButtons(player);
        ReactivateRemainingRanks(player, chains);
        player->SendInitialActionButtons();
        StripTemporaryAuras(player);

        CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
        std::vector<Item*> mailed;
        UnequipUnusableItems(player, mailed, trans);

        state.level += 1;
        state.active = true;
        state.specialization = specialization;
        SaveState(player, state);

        player->AddAura(PrestigedAura, player);
        player->KilledMonsterCredit(PrestigeKillCredit);
        GrantRewards(player, Rewards(), mailed, trans);
        player->SetHomebind(WorldLocation(start->mapId, start->positionX, start->positionY, start->positionZ,
            start->orientation), start->areaId);

        MailItems(player, mailed, trans);
        player->SaveToDB(trans, false, false);
        CharacterDatabase.CommitTransaction(trans);

        SendPrestigeLevels(player);
        chat.PSendSysMessage("Chromie winds back time. Prestige {}: reach level {} again to unlock your "
            "specialization.", state.level, requiredLevel);
        if (!mailed.empty())
            chat.SendSysMessage("Items that did not fit in your bags were sent to your mailbox.");

        LOG_INFO("module.coa_prestige", "{} ({}) activated Prestige {} in specialization {}: {} talent spell(s) "
            "and {} ability chain(s) forgotten.", player->GetName(), player->GetGUID().ToString(), state.level,
            specialization, talents, chains.size());

        if (!player->TeleportTo(start->mapId, start->positionX, start->positionY, start->positionZ,
                start->orientation))
            LOG_ERROR("module.coa_prestige", "{} prestiged but could not be sent to the starting zone.",
                player->GetName());
        return true;
    }

    void CompleteIfDue(Player* player)
    {
        State state = LoadState(player);
        if (!state.active || player->GetLevel() < g_requiredLevel)
            return;

        state.active = false;
        SaveState(player, state);
        player->RemoveAurasDueToSpell(PrestigedAura);
        SendPrestigeLevels(player);
        ChatHandler(player->GetSession()).PSendSysMessage(
            "Prestige {} complete: your specialization is unlocked.", state.level);
    }

    std::string SpecializationSwitchRefusal(Player* player, uint32 /*active*/, uint32 requested)
    {
        if (!g_enabled || IsSpecializationSwitchAllowed(LoadState(player), requested))
            return {};
        return SpecializationLockedMessage(g_requiredLevel);
    }
}

class npc_coa_prestige_chromie : public CreatureScript
{
public:
    npc_coa_prestige_chromie() : CreatureScript("npc_coa_prestige_chromie") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (!g_enabled)
            return false;

        ClearGossipMenuFor(player);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, std::string(OptionActivate), GOSSIP_SENDER_MAIN, ACTION_ACTIVATE);
        AddGossipItemFor(player, GOSSIP_ICON_VENDOR, std::string(OptionRewards), GOSSIP_SENDER_MAIN, ACTION_REWARDS);
        AddGossipItemFor(player, GOSSIP_ICON_VENDOR, std::string(OptionExperienceItems), GOSSIP_SENDER_MAIN,
            ACTION_EXPERIENCE_ITEMS);
        SendPrestigeLevels(player);
        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature);
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        ClearGossipMenuFor(player);
        switch (action)
        {
            case ACTION_ACTIVATE:
                CloseGossipMenuFor(player);
                Activate(player);
                break;
            case ACTION_REWARDS:
                player->GetSession()->SendListInventory(creature->GetGUID());
                break;
            case ACTION_EXPERIENCE_ITEMS:
                player->GetSession()->SendListInventory(creature->GetGUID(), ExperienceVendorEntry);
                break;
            default:
                CloseGossipMenuFor(player);
                break;
        }
        return true;
    }
};

class coa_prestige_player : public PlayerScript
{
public:
    coa_prestige_player() : PlayerScript("coa_prestige_player", { PLAYERHOOK_ON_LOGIN, PLAYERHOOK_ON_LOGOUT,
        PLAYERHOOK_ON_LEVEL_CHANGED, PLAYERHOOK_ON_GIVE_EXP, PLAYERHOOK_ON_LEARN_SPELL, PLAYERHOOK_ON_MAP_CHANGED,
        PLAYERHOOK_ON_SEND_INITIAL_PACKETS_BEFORE_ADD_TO_MAP }) { }

    // A client that reconnects to a character still in the world gets its initial packets again but
    // no login or map hook, and starts with an empty field table. The levels follow once the
    // character is back on the map.
    void OnPlayerSendInitialPacketsBeforeAddToMap(Player* player, WorldPacket& /*data*/) override
    {
        if (g_enabled)
            player->m_Events.AddEventAtOffset([guid = player->GetGUID()]
            {
                if (Player* player = ObjectAccessor::FindPlayer(guid))
                    SendPrestigeLevels(player);
            }, 1s);
    }

    void OnPlayerLogin(Player* player) override
    {
        if (!g_enabled)
            return;

        LoadOtherCharactersPrestige(player);
        CompleteIfDue(player);
        if (LoadState(player).active && !player->HasAura(PrestigedAura))
            player->AddAura(PrestigedAura, player);
        SendPrestigeLevels(player);
    }

    void OnPlayerLogout(Player* player) override
    {
        std::lock_guard<std::mutex> guard(g_accountLock);
        g_otherCharactersPrestige.erase(player->GetGUID().GetCounter());
    }

    void OnPlayerLevelChanged(Player* player, uint8 /*oldLevel*/) override
    {
        if (g_enabled)
            CompleteIfDue(player);
    }

    void OnPlayerGiveXP(Player* player, uint32& amount, Unit* /*victim*/, uint8 /*xpSource*/) override
    {
        if (amount && g_enabled && player->HasAura(PrestigedAura))
            amount = ApplyExperienceBonus(amount, g_experienceBonusPercent);
    }

    void OnPlayerLearnSpell(Player* player, uint32 spellId) override
    {
        std::vector<uint32> const values = SettingValues(player, BarSetting);
        if (values.empty() || !values[0])
            return;

        std::vector<BarSlot> bar = DecodeBar(values);
        std::vector<uint8> const buttons = ClaimButtons(bar, spellId,
            [](uint32 spell) { return sSpellMgr->GetFirstSpellInChain(spell); });
        if (buttons.empty())
            return;

        bool placed = false;
        for (uint8 const button : buttons)
        {
            ActionButton const* current = player->GetActionButton(button);
            bool const free = !current ||
                (current->GetType() == ACTION_BUTTON_SPELL && !player->HasSpell(current->GetAction()));
            if (free && player->addActionButton(button, spellId, ACTION_BUTTON_SPELL))
                placed = true;
        }

        WriteSetting(player, BarSetting, EncodeBar(bar));
        if (placed)
            player->SendInitialActionButtons();
    }

    void OnPlayerMapChanged(Player* player) override
    {
        if (g_enabled)
            SendPrestigeLevels(player);
    }
};

class coa_prestige_world : public WorldScript
{
public:
    coa_prestige_world()
        : WorldScript("coa_prestige_world", { WORLDHOOK_ON_AFTER_CONFIG_LOAD, WORLDHOOK_ON_STARTUP }) { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        LoadConfig();
    }

    void OnStartup() override
    {
        if (!g_enabled)
            return;

        CreatureTemplate const* chromie = sObjectMgr->GetCreatureTemplate(ChromieEntry);
        if (!chromie || chromie->ScriptID != sObjectMgr->GetScriptId("npc_coa_prestige_chromie"))
            LOG_ERROR("module.coa_prestige", "Creature {} is missing or not bound to npc_coa_prestige_chromie: "
                "Prestige Mode cannot be activated.", ChromieEntry);
        if (!sObjectMgr->GetCreatureTemplate(ExperienceVendorEntry))
            LOG_ERROR("module.coa_prestige", "Creature {} is missing: Chromie's Experience Items list is empty.",
                ExperienceVendorEntry);
        if (!sSpellMgr->GetSpellInfo(PrestigedAura))
            LOG_ERROR("module.coa_prestige", "Spell {} is missing: prestiged characters get no experience bonus.",
                PrestigedAura);
    }
};

void AddSC_coa_prestige()
{
    new npc_coa_prestige_chromie();
    new coa_prestige_player();
    new coa_prestige_world();
    AddAscensionSpecializationSwitchGuard(SpecializationSwitchRefusal);
}
