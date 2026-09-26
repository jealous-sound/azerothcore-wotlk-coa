/*
 * mod-coa-prestige: the rules of Prestige Mode that need no game server, so the unit tests can
 * hold them to the contract. Everything that touches a Player lives in CoAPrestige.cpp.
 */
#ifndef COA_PRESTIGE_RULES_H
#define COA_PRESTIGE_RULES_H

#include <algorithm>
#include <charconv>
#include <cstdint>
#include <limits>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace CoAPrestige
{
    // The client's PrestigeModeUI (AscensionUI/Gossips/PrestigeMode.lua) redirects the gossip of
    // creature 178081 to its own window and finds its buttons by these exact option texts.
    constexpr uint32_t ChromieEntry = 178081;
    constexpr std::string_view OptionActivate = "I would like to prestige!";
    constexpr std::string_view OptionRewards = "I would like to purchase Prestige items!";
    constexpr std::string_view OptionExperienceItems = "GOSSIP_ACTION_EXPERIENCE_ITEMS";

    // Vendor list behind the Experience Items button; never spawned.
    constexpr uint32_t ExperienceVendorEntry = 990782;

    // C_Player:IsPrestiged() is HasAura(9930831), and mod-coa-challenges gates its Prestige
    // trials on the same aura.
    constexpr uint32_t PrestigedAura = 9930831;

    // The "Prestige 1..10" achievements count kill credit of this creature.
    constexpr uint32_t PrestigeKillCredit = 888101;

    // GetPrestigeLevel() in the client's Extensions.dll returns slots 2 and 3 of the per-GUID
    // table that SMSG_UPDATE_OBJECT_ADDON (0x0578) fills: u64 guid, then {u32 index, u32 value}.
    constexpr uint16_t AddonFieldOpcode = 0x0578;
    constexpr uint32_t AddonFieldPrestigeLevel = 2;
    constexpr uint32_t AddonFieldAccountPrestigeLevel = 3;

    constexpr char StateSetting[] = "core.coa_prestige";
    constexpr char BarSetting[] = "core.coa_prestige_bar";

    struct State
    {
        uint32_t level = 0;
        bool active = false;
        uint32_t specialization = 0;

        bool operator==(State const&) const = default;
    };

    inline State DecodeState(std::vector<uint32_t> const& values)
    {
        State state;
        if (values.size() > 0)
            state.level = values[0];
        if (values.size() > 1)
            state.active = values[1] != 0;
        if (values.size() > 2)
            state.specialization = values[2];
        return state;
    }

    inline std::vector<uint32_t> EncodeState(State const& state)
    {
        return { state.level, state.active ? 1u : 0u, state.specialization };
    }

    enum class Refusal
    {
        None,
        Disabled,
        NotCustomClass,
        BelowLevel,
        InProgress,
        NoSpecialization,
        Dead,
        InCombat,
        Travelling,
        NotInOpenWorld
    };

    struct ActivationFacts
    {
        bool enabled = true;
        bool customClass = true;
        uint32_t level = 0;
        uint32_t requiredLevel = 60;
        bool active = false;
        uint32_t specialization = 0;
        bool alive = true;
        bool inCombat = false;
        bool travelling = false;
        bool inInstance = false;
    };

    inline Refusal CheckActivation(ActivationFacts const& facts)
    {
        if (!facts.enabled)
            return Refusal::Disabled;
        if (!facts.customClass)
            return Refusal::NotCustomClass;
        if (facts.level < facts.requiredLevel)
            return Refusal::BelowLevel;
        if (facts.active)
            return Refusal::InProgress;
        if (!facts.specialization)
            return Refusal::NoSpecialization;
        if (!facts.alive)
            return Refusal::Dead;
        if (facts.inCombat)
            return Refusal::InCombat;
        if (facts.travelling)
            return Refusal::Travelling;
        if (facts.inInstance)
            return Refusal::NotInOpenWorld;
        return Refusal::None;
    }

    inline std::string RefusalMessage(Refusal refusal, uint32_t requiredLevel)
    {
        switch (refusal)
        {
            case Refusal::None:
                return {};
            case Refusal::Disabled:
                return "Chromie cannot wind back time right now.";
            case Refusal::NotCustomClass:
                return "Prestige Mode is only available to Conquest of Azeroth classes.";
            case Refusal::BelowLevel:
                return "You must be level " + std::to_string(requiredLevel) +
                    " or higher to participate in Prestige Mode";
            case Refusal::InProgress:
                return "You are already in Prestige Mode.";
            case Refusal::NoSpecialization:
                return "Choose a specialization before you activate Prestige Mode.";
            case Refusal::Dead:
                return "You cannot activate Prestige Mode while dead.";
            case Refusal::InCombat:
                return "You cannot activate Prestige Mode while in combat.";
            case Refusal::Travelling:
                return "You cannot activate Prestige Mode while travelling.";
            case Refusal::NotInOpenWorld:
                return "Prestige Mode can only be activated in the open world.";
        }
        return {};
    }

    // Once Prestige Mode is active the specialization stays locked until the character is back at
    // the required level; choosing the locked specialization again is not a switch.
    inline bool IsSpecializationSwitchAllowed(State const& state, uint32_t requestedSpecialization)
    {
        return !state.active || !state.specialization || requestedSpecialization == state.specialization;
    }

    inline std::string SpecializationLockedMessage(uint32_t requiredLevel)
    {
        return "Your active specialization is prestige locked until you reach level " +
            std::to_string(requiredLevel) + ".";
    }

    // Quest types (quest_template.QuestInfoID) that are ordinary open-world content.
    constexpr uint32_t QuestTypeNormal = 0;
    constexpr uint32_t QuestTypeGroup = 1;
    constexpr uint32_t QuestTypeLife = 21;
    constexpr uint32_t QuestTypeEscort = 84;

    struct QuestTraits
    {
        int32_t zoneOrSort = 0;
        int32_t level = 0;
        uint32_t type = QuestTypeNormal;
        bool repeatable = false;
    };

    // A reset hands the zone quests of the levelling path back to the character. Class and
    // profession quests (negative sort), dungeon, raid, PvP and event quests, anything above the
    // Prestige level and every daily, weekly, monthly, seasonal or repeatable quest stay done.
    inline bool IsReplayableQuest(QuestTraits const& quest, uint32_t maxLevel)
    {
        bool const ordinary = quest.type == QuestTypeNormal || quest.type == QuestTypeGroup ||
            quest.type == QuestTypeLife || quest.type == QuestTypeEscort;
        return ordinary && !quest.repeatable && quest.zoneOrSort > 0 && quest.level >= 1 &&
            uint32_t(quest.level) <= maxLevel;
    }

    struct RewardItem
    {
        uint32_t item = 0;
        uint32_t count = 0;

        bool operator==(RewardItem const&) const = default;
    };

    // "item:count item:count ...", separated by spaces or commas.
    inline std::optional<std::vector<RewardItem>> ParseRewards(std::string_view text)
    {
        std::vector<RewardItem> rewards;
        auto parseNumber = [](std::string_view digits, uint32_t& value)
        {
            auto const [end, error] = std::from_chars(digits.data(), digits.data() + digits.size(), value);
            return error == std::errc() && end == digits.data() + digits.size() && value != 0;
        };

        std::size_t position = 0;
        while (position < text.size())
        {
            std::size_t const start = text.find_first_not_of(" ,\t", position);
            if (start == std::string_view::npos)
                break;
            std::size_t const end = std::min(text.find_first_of(" ,\t", start), text.size());
            std::string_view const token = text.substr(start, end - start);
            position = end;

            std::size_t const colon = token.find(':');
            RewardItem reward;
            if (colon == std::string_view::npos || !parseNumber(token.substr(0, colon), reward.item) ||
                !parseNumber(token.substr(colon + 1), reward.count))
                return std::nullopt;
            rewards.push_back(reward);
        }
        return rewards;
    }

    inline uint32_t ApplyExperienceBonus(uint32_t amount, uint32_t bonusPercent)
    {
        uint64_t const boosted = uint64_t(amount) * (100u + uint64_t(bonusPercent)) / 100u;
        return uint32_t(std::min<uint64_t>(boosted, std::numeric_limits<uint32_t>::max()));
    }

    // The spells on the action bar when time was wound back, so each can return to its button
    // when the character learns it again.
    struct BarSlot
    {
        uint8_t button = 0;
        uint32_t spell = 0;

        bool operator==(BarSlot const&) const = default;
    };

    inline std::vector<uint32_t> EncodeBar(std::vector<BarSlot> const& bar)
    {
        std::vector<uint32_t> values = { uint32_t(bar.size()) };
        for (BarSlot const& slot : bar)
        {
            values.push_back(slot.button);
            values.push_back(slot.spell);
        }
        return values;
    }

    inline std::vector<BarSlot> DecodeBar(std::vector<uint32_t> const& values)
    {
        std::vector<BarSlot> bar;
        if (values.empty())
            return bar;

        std::size_t const count = std::min<std::size_t>(values[0], (values.size() - 1) / 2);
        for (std::size_t index = 0; index < count; ++index)
            if (uint32_t const spell = values[2 * index + 2])
                bar.push_back({ uint8_t(values[2 * index + 1]), spell });
        return bar;
    }

    // Buttons whose remembered spell shares a rank chain with the one just learned. They are taken
    // out of the memory, so a later rank of the same spell does not claim them again.
    template <typename FirstRankOf>
    std::vector<uint8_t> ClaimButtons(std::vector<BarSlot>& bar, uint32_t learnedSpell, FirstRankOf firstRankOf)
    {
        std::vector<uint8_t> buttons;
        uint32_t const learnedChain = firstRankOf(learnedSpell);
        auto const claimed = std::remove_if(bar.begin(), bar.end(), [&](BarSlot const& slot)
        {
            if (firstRankOf(slot.spell) != learnedChain)
                return false;
            buttons.push_back(slot.button);
            return true;
        });
        bar.erase(claimed, bar.end());
        return buttons;
    }
}

#endif
