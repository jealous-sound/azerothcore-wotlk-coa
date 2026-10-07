#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using uint32 = std::uint32_t;

struct PlayerSetting
{
    uint32 value = 0;
};
using PlayerSettingVector = std::vector<PlayerSetting>;

struct SpellInfo
{
};

struct SpellMgr
{
    std::set<uint32> known;
    SpellInfo info;
    SpellInfo const* GetSpellInfo(uint32 spellId) const { return known.contains(spellId) ? &info : nullptr; }
};

SpellMgr spellMgr;
SpellMgr* sSpellMgr = &spellMgr;

struct Player
{
    std::set<uint32> spells;
    std::map<std::string, PlayerSettingVector> settings;
    std::vector<uint32> learnOrder;

    PlayerSettingVector const* FindPlayerSettings(std::string const& source) const
    {
        auto found = settings.find(source);
        return found == settings.end() ? nullptr : &found->second;
    }
    void UpdatePlayerSetting(std::string const& source, uint32 index, uint32 value)
    {
        PlayerSettingVector& stored = settings[source];
        if (stored.size() <= index)
            stored.resize(index + 1);
        stored[index].value = value;
    }
    bool HasSpell(uint32 spellId) const { return spells.contains(spellId); }
    void learnSpell(uint32 spellId)
    {
        spells.insert(spellId);
        learnOrder.push_back(spellId);
    }
};

struct Tables
{
    std::unordered_map<uint32, std::vector<uint32>> RankLadders;
    std::unordered_map<uint32, uint32> RankRoots;
};

Tables Loaded;
constexpr char TRAINED_RANKS_SETTING[] = "core.wildcard.trainedranks";

// ACTUAL_RANKS

void Expect(bool condition, char const* what)
{
    if (!condition)
    {
        std::cerr << "FAIL: " << what << "\n";
        std::exit(1);
    }
}

void Forget(Player& player, uint32 spellId)
{
    if (Loaded.RankRoots.contains(spellId))
        RememberTrainedRank(&player, spellId);
    player.spells.erase(spellId);
}

int main()
{
    Loaded.RankLadders[100] = { 100, 101, 102, 103 };
    Loaded.RankLadders[200] = { 200, 0, 202 };
    for (auto const& [root, ladder] : Loaded.RankLadders)
        for (std::size_t rank = 1; rank < ladder.size(); ++rank)
            if (ladder[rank])
                Loaded.RankRoots[ladder[rank]] = root;
    spellMgr.known = { 100, 101, 102, 103, 200, 202 };

    Player player;
    player.spells = { 100, 101, 102, 200, 202 };

    Forget(player, 102);
    Forget(player, 101);
    Forget(player, 100);
    Forget(player, 202);
    Forget(player, 200);
    Expect(player.spells.empty(), "unlearning removes every rank");

    Forget(player, 102);
    PlayerSettingVector const* stored = player.FindPlayerSettings(TRAINED_RANKS_SETTING);
    Expect(stored && std::count_if(stored->begin(), stored->end(), [](PlayerSetting s) { return s.value == 102; }) == 1,
           "a rank is remembered once");

    player.learnSpell(100);
    player.learnOrder.clear();
    RestoreTrainedRanks(&player, 100);
    Expect(player.learnOrder == std::vector<uint32>({ 101, 102 }), "re-learning rank 1 restores the trained ranks in order");
    Expect(!player.HasSpell(103), "a rank never trained is not granted");
    Expect(!player.HasSpell(202), "another ability's ranks stay remembered, not granted");

    stored = player.FindPlayerSettings(TRAINED_RANKS_SETTING);
    Expect(std::count_if(stored->begin(), stored->end(), [](PlayerSetting s) { return s.value == 101 || s.value == 102; }) == 0,
           "restored ranks are no longer remembered");

    Forget(player, 102);
    Expect((*stored)[0].value == 102 || (*stored)[1].value == 102, "a freed slot is reused");

    player.learnSpell(200);
    player.learnOrder.clear();
    RestoreTrainedRanks(&player, 200);
    Expect(player.learnOrder == std::vector<uint32>({ 202 }), "a ladder with a gap restores its remembered rank");

    player.learnOrder.clear();
    RestoreTrainedRanks(&player, 200);
    Expect(player.learnOrder.empty(), "restoring twice grants nothing more");

    std::cout << "PASS: trained ranks come back when a Hero re-learns the ability\n";
    return 0;
}
