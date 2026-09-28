CLI_DESCRIPTION = """Regress Flay Corpse's automatic choice of the nearest eligible corpse (#5104)."""
import argparse
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from source_paths import git_source  # noqa: E402

ROOT = Path(__file__).resolve().parents[3]
PATH = "src/server/coa/AscensionXorothAbilities.cpp"


def braced(source, start):
    end = source.index("{", start) + 1
    depth = 1
    while depth:
        depth += (source[end] == "{") - (source[end] == "}")
        end += 1
    return source[start:end]


def helpers(source):
    return "\n".join(braced(source, source.index(signature)) for signature in
                     ("bool Flayable(Unit* unit)", "float FlayRange(Player* player, Spell* spell)",
                      "Creature* NearestFlayable(Player* player, float range)") if signature in source)


HARNESS = r"""
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <list>
#include <set>
using uint32 = std::uint32_t;
constexpr float INTERACTION_DISTANCE = 5.5f;
enum SpellCastResult { SPELL_CAST_OK, SPELL_FAILED_AFFECTING_COMBAT, SPELL_FAILED_BAD_TARGETS };
enum class DeathState { Alive, Corpse, Dead };
enum CreatureType { CREATURE_TYPE_BEAST = 1, CREATURE_TYPE_DEMON = 3, CREATURE_TYPE_ELEMENTAL = 4,
                    CREATURE_TYPE_HUMANOID = 7 };
struct Creature;
struct Unit
{
    float x = 0;
    virtual ~Unit() = default;
    virtual Creature* ToCreature() { return nullptr; }
    float GetExactDistSq(Unit const* other) const { return (x - other->x) * (x - other->x); }
};
struct Creature : Unit
{
    DeathState state = DeathState::Corpse;
    CreatureType type = CREATURE_TYPE_HUMANOID;
    Creature(float position, CreatureType kind, DeathState death = DeathState::Corpse) : state(death), type(kind)
    {
        x = position;
    }
    Creature* ToCreature() override { return this; }
    DeathState getDeathState() const { return state; }
    CreatureType GetCreatureType() const { return type; }
};
struct Player : Unit
{
    bool combat = false;
    std::list<Creature*> world;
    std::set<Creature*> hidden;
    bool IsInCombat() const { return combat; }
    bool IsWithinLOSInMap(Creature* creature) const { return !hidden.count(creature); }
    void GetDeadCreatureListInGrid(std::list<Creature*>& found, float range, bool excludeAlive) const
    {
        assert(excludeAlive);
        for (Creature* creature : world)
            if (creature->state != DeathState::Alive && GetExactDistSq(creature) <= range * range)
                found.push_back(creature);
    }
};
struct Spell;
struct SpellInfo
{
    uint32 Id = 801042;
    float range = 0;
    float GetMaxRange(bool positive, Player*, Spell*) const
    {
        assert(!positive);
        return range;
    }
};
struct SpellCastTargets
{
    Unit* unit = nullptr;
    Unit* GetUnitTarget() const { return unit; }
    void SetUnitTarget(Unit* target) { unit = target; }
};
struct Spell
{
    SpellInfo spellInfo;
    SpellCastTargets m_targets;
    SpellInfo const* GetSpellInfo() const { return &spellInfo; }
};
/*HELPERS*/
SpellCastResult Check(Player* player, Spell* spell)
{
    SpellInfo const* info = spell->GetSpellInfo();
    SpellCastResult result = SPELL_CAST_OK;
    /*BLOCK*/
    return result;
}
int main()
{
    Player player;
    Creature far(40, CREATURE_TYPE_HUMANOID), elemental(2, CREATURE_TYPE_ELEMENTAL), beast(6, CREATURE_TYPE_BEAST),
        demon(-9, CREATURE_TYPE_DEMON), living(1, CREATURE_TYPE_HUMANOID, DeathState::Alive),
        walled(3, CREATURE_TYPE_HUMANOID), bones(4, CREATURE_TYPE_HUMANOID, DeathState::Dead);
    player.world = {&far, &elemental, &beast, &demon, &living, &walled, &bones};
    player.hidden = {&walled};
    Spell spell;
    spell.spellInfo.range = 10;

    assert(Check(&player, &spell) == SPELL_CAST_OK);
    assert(spell.m_targets.GetUnitTarget() == &beast);

    spell.m_targets.SetUnitTarget(&demon);
    assert(Check(&player, &spell) == SPELL_CAST_OK);
    assert(spell.m_targets.GetUnitTarget() == &demon);

    spell.m_targets.SetUnitTarget(&living);
    assert(Check(&player, &spell) == SPELL_CAST_OK);
    assert(spell.m_targets.GetUnitTarget() == &beast);

    spell.m_targets.SetUnitTarget(&elemental);
    assert(Check(&player, &spell) == SPELL_CAST_OK);
    assert(spell.m_targets.GetUnitTarget() == &beast);

    player.hidden.clear();
    spell.m_targets.SetUnitTarget(nullptr);
    assert(Check(&player, &spell) == SPELL_CAST_OK);
    assert(spell.m_targets.GetUnitTarget() == &walled);
    player.hidden = {&walled};

    player.combat = true;
    spell.m_targets.SetUnitTarget(nullptr);
    assert(Check(&player, &spell) == SPELL_FAILED_AFFECTING_COMBAT);
    player.combat = false;

    spell.spellInfo.range = 0;
    spell.m_targets.SetUnitTarget(nullptr);
    assert(Check(&player, &spell) == SPELL_FAILED_BAD_TARGETS);
    assert(spell.m_targets.GetUnitTarget() == nullptr);
    beast.x = 5;
    assert(Check(&player, &spell) == SPELL_CAST_OK);
    assert(spell.m_targets.GetUnitTarget() == &beast);

    player.world = {&far, &elemental, &living, &bones};
    spell.spellInfo.range = 100;
    spell.m_targets.SetUnitTarget(&living);
    assert(Check(&player, &spell) == SPELL_CAST_OK);
    assert(spell.m_targets.GetUnitTarget() == &far);
    player.world = {&elemental, &living, &bones};
    spell.m_targets.SetUnitTarget(&living);
    assert(Check(&player, &spell) == SPELL_FAILED_BAD_TARGETS);
    assert(spell.m_targets.GetUnitTarget() == &living);
}
"""


def compile_and_run(code):
    with tempfile.TemporaryDirectory(prefix="coa-flay-corpse-", ignore_cleanup_errors=True) as directory:
        out = Path(directory)
        cpp, exe = out / "flay.cpp", out / "flay.exe"
        cpp.write_text(code, encoding="utf-8")
        msvc = shutil.which("cl")
        if msvc is None and os.environ.get("VCToolsInstallDir"):
            msvc = str(Path(os.environ["VCToolsInstallDir"]) / "bin/Hostx64/x64/cl.exe")
        if msvc:
            command = [msvc, "/nologo", "/std:c++20", "/EHsc", "/W4", "/WX", "/utf-8", "/UNDEBUG",
                       str(cpp), "/Fe" + str(exe)]
        else:
            compiler = shutil.which(os.environ.get("CXX", "g++"))
            assert compiler, "Run from a Visual Studio developer prompt or set CXX to a C++17 compiler."
            command = [compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-UNDEBUG", str(cpp), "-o", str(exe)]
        subprocess.run(command, cwd=out, check=True, timeout=120)
        subprocess.run([str(exe)], cwd=out, check=True, timeout=15)


def main():
    parser = argparse.ArgumentParser(description=CLI_DESCRIPTION)
    parser.add_argument("--source-ref", help="Use an earlier Xoroth abilities source as a negative control")
    args = parser.parse_args()
    source = (git_source(["git", "show", f"{args.source_ref}:{PATH}"], cwd=ROOT).decode("utf-8")
              if args.source_ref else (ROOT / PATH).read_text(encoding="utf-8"))
    check = braced(source, source.index("void OnSpellCheckCast(Spell* spell, bool, SpellCastResult& result)"))
    block = braced(check, check.index("if (info->Id == 801042)"))
    compile_and_run(HARNESS.replace("/*HELPERS*/", helpers(source)).replace("/*BLOCK*/", block))
    print("PASS: Flay Corpse keeps a chosen corpse and otherwise takes the nearest eligible one in sight and reach")


if __name__ == "__main__":
    main()
