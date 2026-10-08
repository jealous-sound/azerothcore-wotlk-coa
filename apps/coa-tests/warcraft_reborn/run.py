CLI_DESCRIPTION = """Check the Warcraft Reborn class spell rules without a server.

Compiles the Warcraft Reborn rule code against the client DBC set the server loads (--dbc-dir), then checks the
starting spells each stock class gets from its Reborn Character Advancement page, the abilities learned on their own
as a character levels, the Reborn rank ladders, and how a stock class trainer's list becomes the Reborn one.
Hero, Conquest of Azeroth and other realms must get nothing from these rules.
No database, server build or game client is needed.
"""

import argparse
import os
from pathlib import Path
import shutil
import re
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
sys.path.insert(0, str(HERE.parent))
from coa_talent_catalog import STUBS  # noqa: E402
from client_data import dbc_dir  # noqa: E402

MAIN = r"""
#include "AscensionWarcraftRebornRules.h"
#include "ClientDBC.h"
#include "DBCStores.h"
#include <algorithm>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace AscensionWarcraftReborn;

namespace
{
int failures = 0;
void Check(bool value, char const* name)
{
    failures += !value;
    std::printf("%s: %s\n", value ? "PASS" : "FAIL", name);
}

bool Same(std::vector<std::uint32_t> spells, std::vector<std::uint32_t> expected)
{
    std::sort(spells.begin(), spells.end());
    std::sort(expected.begin(), expected.end());
    return spells == expected;
}

bool Holds(std::vector<std::uint32_t> const& spells, std::uint32_t spellId)
{
    return std::find(spells.begin(), spells.end(), spellId) != spells.end();
}

TrainerSpell const* Row(std::vector<TrainerSpell> const& rows, std::uint32_t spellId)
{
    for (TrainerSpell const& row : rows)
        if (row.SpellId == spellId)
            return &row;
    return nullptr;
}

TrainerSpell Stock(std::uint32_t spellId, std::uint32_t cost, std::uint32_t level, std::uint32_t ability = 0)
{
    TrainerSpell row;
    row.SpellId = spellId;
    row.MoneyCost = cost;
    row.ReqLevel = level;
    row.ReqAbility[0] = ability;
    return row;
}

constexpr std::uint32_t WARRIOR = 1, PALADIN = 2, HUNTER = 3, PRIEST = 5, MAGE = 8, WARLOCK = 9, HERO = 10;
constexpr std::uint32_t RANGER = 12;
constexpr std::uint32_t IMPROVED_SHADOW_BOLT = 13354, BANE = 13359, UNBREAKABLE_WILL = 12678;
constexpr std::uint32_t SHADOW_BOLT_ENTRY = 18581;
constexpr std::uint32_t SPELL_LEVEL = 39;

AscensionFreepick::Realm FreepickRealm()
{
    AscensionFreepick::Realm realm;
    realm.Live = true;
    realm.Seasonal = true;
    return realm;
}
}

int main(int, char** argv)
{
    DbcDirectory = std::string(argv[1]) + "/";
    Data data;
    Check(LoadData(data), "the advancement, spell rank and trainer tables load");

    ClientDBC spellStore;
    std::unordered_map<std::uint32_t, std::uint32_t> spells;
    if (spellStore.Load(GetClientDBCPath("Spell.dbc"), SPELL_LEVEL + 1))
        for (std::uint32_t index = 0; index < spellStore.GetRecordCount(); ++index)
            spells[spellStore.GetRecord(index).GetUInt32(0)] = spellStore.GetRecord(index).GetUInt32(SPELL_LEVEL);
    SpellExists const exists = [&spells](std::uint32_t spellId) { return spells.contains(spellId); };

    AscensionFreepick::Realm reborn;
    reborn.Live = true;
    reborn.WarcraftReborn = true;

    Check(Same(StartingSpells(data, reborn, PRIEST, 1), { 1100585, 1101243, 1102050 }),
        "a level 1 Reborn Priest starts with Smite, Power Word: Fortitude and Lesser Heal");
    Check(Same(StartingSpells(data, reborn, WARLOCK, 1), { 1100686, 1100687, 1100688 }),
        "a level 1 Reborn Warlock starts with Shadow Bolt, Demon Skin and Summon Imp");
    Check(Same(StartingSpells(data, reborn, WARRIOR, 1), { 1100078, 1100772, 1102457, 1106673 }),
        "a level 1 Reborn Warrior starts with Heroic Strike, Rend, Battle Stance and Battle Shout");
    std::vector<std::uint32_t> const hunter = StartingSpells(data, reborn, HUNTER, 1);
    Check(Holds(hunter, 75) && Holds(hunter, 1102973) && Holds(hunter, 1121004) && !Holds(hunter, 1578104),
        "a Reborn Hunter starts with Auto Shot, Raptor Strike and Quick Shot, not the alternate Shadow Artillery");
    Check(!Holds(StartingSpells(data, reborn, MAGE, 1), 1100116), "abilities above level 1 are not starting spells");
    for (std::uint32_t classId = 1; classId <= 11; ++classId)
        if (classId != HERO && classId != 6 && StartingSpells(data, reborn, classId, 1).empty())
            Check(false, "every stock class has Reborn starting spells");

    Check(Same(ClassSkillLines(data, reborn, WARLOCK), { 11354, 11355, 11593 }),
        "a Reborn Warlock learns Demonology, Affliction and Destruction, not a demon's skill line");
    Check(Same(ClassSkillLines(data, reborn, PRIEST), { 11056, 11078, 11613 }),
        "a Reborn Priest learns Holy, Shadow Magic and Discipline");
    Check(ClassSkillLines(data, FreepickRealm(), PRIEST).empty(), "no skill lines are taught off a Reborn realm");
    Check(StartingSpells(data, reborn, HERO, 1).empty() && StartingSpells(data, reborn, RANGER, 1).empty(),
        "a Hero or a Conquest of Azeroth class gets no Reborn starting spells");
    AscensionFreepick::Realm freepick;
    freepick.Live = true;
    freepick.Seasonal = true;
    AscensionFreepick::Realm conquest = freepick;
    conquest.ConquestOfAzeroth = true;
    Check(StartingSpells(data, freepick, PRIEST, 1).empty() && StartingSpells(data, conquest, PRIEST, 1).empty() &&
            AutomaticSpells(data, freepick, PALADIN, 80).empty(),
        "free-pick and Conquest of Azeroth realms get no Reborn spells");

    std::vector<std::uint32_t> const judgements = AutomaticSpells(data, reborn, PALADIN, 12);
    Check(Holds(judgements, 1120271) && Holds(judgements, 1153408) && !Holds(judgements, 1153407),
        "a level 12 Reborn Paladin is given Judgement of Light and of Wisdom, not yet Judgement of Justice");
    Check(!Holds(StartingSpells(data, reborn, PALADIN, 80), 1120271),
        "an automatic ability is never a starting spell");

    std::vector<std::uint32_t> const* smite = data.LadderOf(1100591);
    Check(smite && smite->size() == 12 && smite->front() == 1100585 && smite->back() == 1148123,
        "Reborn Smite climbs twelve ranks, 1100585 to 1148123");
    bool chainsReborn = true;
    for (std::vector<std::uint32_t> const& chain : RankChains(data))
        chainsReborn = chainsReborn && chain.size() >= 2;
    Check(chainsReborn && RankChains(data).size() > 300, "hundreds of Reborn rank chains are added, none single");
    std::vector<std::uint32_t> const* shadowBolt = data.LadderOf(1100686);
    std::vector<std::uint32_t> const* demonArmor = data.LadderOf(1100706);
    Check(shadowBolt && shadowBolt->size() == 13 && demonArmor && demonArmor->size() == 8,
        "Reborn Shadow Bolt has 13 ranks, and Demon Armor, trained but on no advancement page, has 8");

    std::vector<std::uint32_t> const* soulGorge = data.LadderOf(DARK_APOTHEOSIS_SPELLS[1]);
    std::vector<std::uint32_t> const* maw = data.LadderOf(DARK_APOTHEOSIS_SPELLS[2]);
    Check(soulGorge && soulGorge->size() == 10 && maw && maw->size() == 9 && spells.at(soulGorge->front()) == 10 &&
            spells.at(maw->front()) == 18,
        "Dark Apotheosis's Soul Gorge (10 ranks, from 10) and Maw of Dread (9 ranks, from 18) are ranked");
    Check(RebornSpell(591, exists) == 1100591 && RebornSpell(750, exists) == 750 &&
            RebornSpell(1100591, exists) == 1100591,
        "a stock spell trains as its Reborn copy, and a spell without one stays itself");

    AscensionFreepick::Build const warlock(data.Catalog, reborn, 20, {}, WARLOCK);
    Check(warlock.TEBudget() == 11 && AscensionFreepick::Build(data.Catalog, reborn, 9, {}, WARLOCK).TEBudget() == 0,
        "a Reborn Warlock has its own class's talent essence: 11 at level 20, none before level 10");
    Check(warlock.ValidateLearn(IMPROVED_SHADOW_BOLT, nullptr) == AscensionFreepick::LEARN_OK,
        "a level 20 Reborn Warlock may learn Improved Shadow Bolt");
    Check(warlock.ValidateLearn(UNBREAKABLE_WILL, nullptr) == AscensionFreepick::LEARN_WRONG_CLASS,
        "a Reborn Warlock is refused a Priest talent");
    Check(warlock.ValidateLearn(SHADOW_BOLT_ENTRY, nullptr) == AscensionFreepick::LEARN_DISPLAY_ENTRY,
        "a Reborn ability is a trainer spell, not a talent pick");
    Check(AscensionFreepick::Build(data.Catalog, freepick, 20).ValidateLearn(IMPROVED_SHADOW_BOLT, nullptr) ==
        AscensionFreepick::LEARN_WRONG_CLASS, "a free-pick Hero is refused a Reborn talent");
    AscensionFreepick::ApplyCheck const picked = AscensionFreepick::CheckApply(warlock,
        { { IMPROVED_SHADOW_BOLT, 1 }, { BANE, 1 } }, nullptr, {});
    Check(picked.Result == AscensionFreepick::UPDATE_OK && picked.Entries.size() == 2,
        "a Reborn Warlock's talent upload applies");

    RebornRow const added = [&spells](std::uint32_t spellId) { return Stock(spellId, 7, spells.at(spellId)); };
    std::vector<TrainerSpell> const priest = TrainerSpells(data, reborn, PRIEST,
        { Stock(591, 100, 6), Stock(2053, 300, 10, 2052), Stock(750, 1000, 40), Stock(48123, 5000, 79) }, exists,
        added);
    TrainerSpell const* smite2 = Row(priest, 1100591);
    TrainerSpell const* heal = Row(priest, 1102053);
    Check(smite2 && smite2->ReqLevel == 6 && !Row(priest, 591),
        "the stock Smite rank 2 row teaches Reborn Smite rank 2 at its own level");
    Check(heal && heal->ReqAbility[0] == 1102052, "a required ability becomes its Reborn copy too");
    Check(Row(priest, 750) != nullptr, "a stock row without a Reborn copy stays on the list");
    Check(Row(priest, 1148046) != nullptr && Row(priest, 1100585) == nullptr,
        "Reborn Mind Sear, missing from the stock lists, is added; Smite rank 1 is not trainable");

    std::vector<TrainerSpell> const searing = TrainerSpells(data, reborn, WARLOCK, { Stock(5676, 900, 18) }, exists,
        added);
    Check(Row(searing, 1105676) && Row(searing, 1105676)->ReqLevel == 14,
        "Reborn Searing Pain trains at 14, its retuned level, not the stock trainer's 18");
    std::vector<TrainerSpell> const hunterTrainer = TrainerSpells(data, reborn, HUNTER,
        { Stock(14260, 100, 8), Stock(14261, 1000, 16) }, exists, added);
    Check(Row(hunterTrainer, 1101579) != nullptr,
        "a Reborn Hunter trainer sells Tame Beast, trainable on Beast Mastery");
    std::vector<TrainerSpell> const shaman = TrainerSpells(data, reborn, 7,
        { Stock(529, 100, 8), Stock(49271, 5000, 80) }, exists, added);
    Check(Row(shaman, 1954861) != nullptr && Row(shaman, 1182010) != nullptr,
        "a Reborn Shaman trainer sells Healing Rain and Unleash Earth, trainable on the class's own lines");
    std::vector<TrainerSpell> const paladin = TrainerSpells(data, reborn, PALADIN,
        { Stock(639, 100, 6), Stock(48782, 5000, 80) }, exists, added);
    Check(Row(paladin, 1120271) == nullptr && Row(paladin, 1120376) != nullptr,
        "automatic Judgement of Light is not sold; Seal of Command's higher ranks are");
    auto const grant = [](std::uint32_t source) -> std::vector<std::uint32_t>
    {
        for (Grant const& entry : Grants())
            if (entry.Source == source)
                return entry.Spells;
        return {};
    };
    Check(Same(grant(1101579), { 1101515, 1100883, 1102641, 1100982, 1356991 }) && Same(grant(1160103), { 1100674 }) &&
            Same(grant(1165139), { 1133891, 1105420 }),
        "Tame Beast teaches the five pet spells, Lava Lash teaches Dual Wield, Tree of Life its form and passive");
    bool grantsExist = true;
    for (Grant const& entry : Grants())
        for (std::uint32_t spellId : entry.Spells)
            grantsExist = grantsExist && exists(spellId);
    Check(grantsExist, "every granted spell exists in the client spell table");
    std::vector<TrainerSpell> const portals = TrainerSpells(data, reborn, MAGE, { Stock(3561, 1000, 20) }, exists,
        added);
    Check(portals.size() == 1, "a trainer teaching no Reborn ability gets nothing added");
    std::vector<TrainerSpell> const starter = TrainerSpells(data, reborn, PRIEST, { Stock(591, 100, 6) }, exists,
        added);
    Check(!Row(starter, 1148046), "a starting-zone trainer is not given spells above its own levels");
    Check(TrainerSpells(data, freepick, PRIEST, { Stock(591, 100, 6), Stock(48123, 5000, 79) }, exists, added)
            .size() == 2,
        "on a free-pick realm the rules add nothing to a trainer");

    return failures ? 1 : 0;
}
"""


FORM_ONLY_SQL = ROOT / "data/sql/updates/pending_db_world/rev_20261005_31_coa_warcraft_reborn_dark_apotheosis.sql"
FORM_SPELLS = (1159674, 1161363, 1161294)


def check_form_only_bindings(dbc):
    data = (dbc / "SpellRank.dbc").read_bytes()
    count, fields, size, _ = struct.unpack_from("<4I", data, 4)
    ranks = {FORM_SPELLS[0]}
    for row in range(count):
        record = struct.unpack_from("<%dI" % fields, data, 20 + row * size)
        if record[1] in FORM_SPELLS:
            ranks.add(record[2])
    bound = {int(spell) for spell in re.findall(r"\((\d+), 'spell_ascension_reborn_dark_apotheosis_only'\)",
                                                 FORM_ONLY_SQL.read_text(encoding="utf-8"))}
    ok = len(ranks) == 20 and bound == ranks
    print(f"{'PASS' if ok else 'FAIL'}: every rank of Provoke, Soul Gorge and Maw of Dread is cast only in "
          "Dark Apotheosis")
    return ok


def main():
    parser = argparse.ArgumentParser(description=CLI_DESCRIPTION)
    parser.add_argument("--dbc-dir", type=Path)
    args = parser.parse_args()
    args.dbc_dir = args.dbc_dir or dbc_dir()

    compiler = shutil.which(os.environ.get("CXX", "cl.exe" if os.name == "nt" else "c++"))
    assert compiler, "Enable a C++20 compiler (VS Developer PowerShell on Windows)."
    with tempfile.TemporaryDirectory(prefix="coa-warcraft-reborn-") as directory:
        out = Path(directory)
        for name, text in STUBS.items():
            (out / name).write_text(text, encoding="utf-8")
        (out / "main.cpp").write_text(MAIN, encoding="utf-8")
        includes = [out, ROOT / "src/server/coa", ROOT / "src/server/shared/DataStores", ROOT / "src/common"]
        sources = [out / "main.cpp", ROOT / "src/server/coa/AscensionWarcraftRebornRules.cpp",
                   ROOT / "src/server/coa/AscensionFreepickRules.cpp",
                   ROOT / "src/server/shared/DataStores/ClientDBC.cpp"]
        executable = out / ("rules.exe" if os.name == "nt" else "rules")
        if Path(compiler).stem.lower() == "cl":
            flags = ["/nologo", "/std:c++20", "/EHsc", "/utf-8", "/D_CRT_SECURE_NO_WARNINGS",
                     *["/I" + str(p) for p in includes], *map(str, sources), "/Fe" + str(executable)]
        else:
            flags = ["-std=c++20", "-Wall", "-Wextra", *["-I" + str(p) for p in includes], *map(str, sources),
                     "-o", str(executable)]
        build = subprocess.run([compiler, *flags], cwd=out, capture_output=True, text=True, errors="replace")
        if build.returncode:
            raise SystemExit("Warcraft Reborn rules harness did not compile:\n" + build.stdout + build.stderr)
        result = subprocess.run([str(executable), str(args.dbc_dir.resolve())], text=True)
        bindings_ok = check_form_only_bindings(args.dbc_dir.resolve())
        raise SystemExit(result.returncode or (0 if bindings_ok else 1))


if __name__ == "__main__":
    main()
