"""Exercise Cloudburst's completed-cast route and captured native knockback data."""
import argparse
import importlib.util
from pathlib import Path
import re
import struct
import tempfile

ROOT = Path(__file__).resolve().parents[4]
HARNESS = r'''
#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <vector>
using uint32 = std::uint32_t;
// NATIVE_CLASSES
constexpr int ALLSPELLHOOK_ON_CAST = 1;
struct SpellInfo { uint32 Id = 801838, SpellFamilyName = 22; };
struct Spell { bool triggered = false; bool IsTriggered() const { return triggered; } };
struct Player;
struct Unit { virtual ~Unit() = default; virtual Player* ToPlayer() { return nullptr; } };
struct Player : Unit
{
    uint32 cls = CLASS_STORMBRINGER;
    std::vector<uint32> casts;
    Player* ToPlayer() override { return this; }
    uint32 getClass() const { return cls; }
    void CastSpell(Unit* target, uint32 id, bool triggered)
    {
        assert(target == this && triggered);
        casts.push_back(id);
    }
};
struct AllSpellScript
{
    AllSpellScript(char const*, std::initializer_list<int>) { }
    virtual void OnSpellCast(Spell*, Unit*, SpellInfo const*, bool) { }
};
// ACTUAL_SOURCE
int main()
{
    Player player;
    Unit creature;
    Spell spell;
    SpellInfo info;
    stormbringer_talent_casts hook;
    hook.OnSpellCast(&spell, &player, &info, false);
    assert(player.casts == std::vector<uint32>{802385});
    info.Id = 802385;
    hook.OnSpellCast(&spell, &player, &info, false);
    info.Id = 801838;
    spell.triggered = true;
    hook.OnSpellCast(&spell, &player, &info, false);
    spell.triggered = false;
    info.SpellFamilyName = 3;
    hook.OnSpellCast(&spell, &player, &info, false);
    info.SpellFamilyName = 22;
    player.cls = CLASS_MAGE;
    hook.OnSpellCast(&spell, &player, &info, false);
    hook.OnSpellCast(&spell, &creature, &info, false);
    hook.OnSpellCast(&spell, nullptr, &info, false);
    assert(player.casts.size() == 1);
}
'''


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--workspace-tools", type=Path, default=ROOT.parent / "tools")
    parser.add_argument("--spell-dbc", type=Path)
    args = parser.parse_args()
    spec = importlib.util.spec_from_file_location("storm_compile", args.workspace_tools / "Test-LocalLoginCollections.py")
    native = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(native)
    classes = native.extractor.extract((ROOT / "src/server/shared/SharedDefines.h").read_text(), r"enum Classes\b")
    source = (ROOT / "modules/mod-ascension-compat/src/AscensionStormbringerTalents.cpp").read_text()
    code = HARNESS.replace("// NATIVE_CLASSES", classes + ";")
    code = code.replace("// ACTUAL_SOURCE", re.sub(r"^#include.*\n", "", source, flags=re.M))
    with tempfile.TemporaryDirectory(prefix="coa-stormbringer-passives-") as directory:
        native.OUT = Path(directory)
        result = native.compile_run(code, "stormbringer-passives")
        assert result.returncode == 0, result.stdout + result.stderr
    if args.spell_dbc:
        raw = args.spell_dbc.read_bytes()
        count = struct.unpack_from("<I", raw, 4)[0]
        rows = {r[0]: r for r in struct.iter_unpack("<234I", raw[20:20 + count * 936]) if r[0] in {801838, 802385}}
        parent, child = rows[801838], rows[802385]
        assert parent[71:74] == (3, 64, 0) and parent[92] == 0 and parent[117] == 32991
        assert parent[208:212] == child[208:212] == (22, 0, 0, 8388608)
        assert child[71:74] == (98, 0, 0) and child[86] == 18 and child[89] == 16
        assert child[110] == 180 and child[80] + child[74] == 101 and child[92] == 45
        raw = (args.spell_dbc.parent / "SpellRadius.dbc").read_bytes()
        count = struct.unpack_from("<I", raw, 4)[0]
        radius = {r[0]: r[1] for r in struct.iter_unpack("<I3f", raw[20:20 + count * 16])}
        assert radius[45] == 10
    print("PASS: Cloudburst casts the native ten-yard knockback once; class/family/triggered guards and helper data")


if __name__ == "__main__":
    main()
