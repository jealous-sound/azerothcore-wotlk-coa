"""Regress the two #88 Xoroth passives using the existing completion fixture."""
import argparse
import importlib.util
from pathlib import Path
import re
import struct
import subprocess
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
CASES = r'''
int main()
{
    Player player;
    player.guid = 1;
    world[player.guid] = &player;
    Unit enemy;
    enemy.guid = 2;
    world[enemy.guid] = &enemy;
    SpellInfo info;
    info.Id = 806965;
    Spell spell;
    spell.caster = &player;
    spell.info = &info;
    spell.m_targets.target = &enemy;
    Casts casts;
    auto apocalypse = player.AddAura(560817, &enemy);
    apocalypse->maximum = 12000;
    apocalypse->effects[1].periodicTimer = 730;
    for (bool talent : {false, true})
    {
        if (talent)
            player.AddAura(805706, &player);
        for (uint32 fire : {0u, 3u, 4u, 6u})
        {
            apocalypse->duration = 3500;
            spell.SetScriptValue(500906, fire);
            casts.OnSpellCast(&spell, &player, &info, false);
            assert(apocalypse->duration == (talent && fire >= 4 ? 12000 : 3500));
            assert(apocalypse->effects[1].periodicTimer == 730);
        }
    }
    apocalypse->duration = 3500;
    spell.triggered = true;
    casts.OnSpellCast(&spell, &player, &info, false);
    assert(apocalypse->duration == 3500);
    spell.triggered = false;
    TargetInfo miss;
    miss.targetGUID = enemy.guid;
    miss.missCondition = SPELL_MISS_MISS;
    spell.unique = {miss};
    casts.OnSpellCast(&spell, &player, &info, false);
    assert(apocalypse->duration == 3500);
    spell.unique.clear();
    apocalypse->caster = 99;
    casts.OnSpellCast(&spell, &player, &info, false);
    assert(apocalypse->duration == 3500);
    apocalypse->caster = player.guid;
    player.cls = 14;
    casts.OnSpellCast(&spell, &player, &info, false);
    assert(apocalypse->duration == 3500);
    player.cls = 17;

    Creature imp;
    imp.entry = 50301;
    Death death;
    death.me = &imp;
    death.owner = player.guid;
    for (uint32 id : {805677u, 524897u})
    {
        manager.rows[id].Id = id;
        player.learnSpell(id, false);
        player.cooldowns[id] = 10000;
    }
    manager.rows[804012].Effects[0].BasePoints = -2001;
    manager.rows[804012].Effects[1].BasePoints = -1001;
    death.JustDied(nullptr);
    assert(player.cooldowns[805677] == 10000 && player.cooldowns[524897] == 10000);
    player.AddAura(804013, &player);
    death.JustDied(nullptr);
    assert(player.cooldowns[805677] == 8000 && player.cooldowns[524897] == 9000);
    imp.entry = 50375;
    death.JustDied(nullptr);
    assert(player.cooldowns[805677] == 8000 && player.cooldowns[524897] == 9000);
    imp.entry = 50301;
    death.owner = 99;
    death.JustDied(nullptr);
    assert(player.cooldowns[805677] == 8000 && player.cooldowns[524897] == 9000);
}
'''


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--workspace-tools", type=Path, default=ROOT.parent / "tools")
    parser.add_argument("--source-ref", help="Use earlier Xoroth casts for a negative control")
    parser.add_argument("--spell-dbc", type=Path)
    args = parser.parse_args()
    spec = importlib.util.spec_from_file_location("xoroth_completion_fixture",
                                                 args.workspace_tools / "Test-KnightOfXorothCompletion.py")
    fixture = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(fixture)
    fixture.M = ROOT / "modules/mod-ascension-compat/src"
    original_source = fixture.src
    if args.source_ref:
        fixture.src = lambda part: (subprocess.check_output([
            "git", "show", f"{args.source_ref}:modules/mod-ascension-compat/src/AscensionXorothAbilities.cpp"
        ], cwd=ROOT).decode("utf-8") if part == "Abilities" else original_source(part))
    production = fixture.methods("Abilities", ["RefundableMiss", "ConsumeSelected"])
    production += "\n#ifdef _MSC_VER\n#pragma warning(push)\n#pragma warning(disable: 4244)\n#endif\n"
    production += fixture.wrap("Casts", "Abilities", ["OnSpellCast"])
    production += "\n#ifdef _MSC_VER\n#pragma warning(pop)\n#endif\n"
    production += "namespace ObjectAccessor { Player* GetPlayer(Unit&, ObjectGuid id) { return FindPlayer(id); } }\n"
    production += "struct Death { Creature* me; ObjectGuid owner; "
    production += fixture.t.fn(fixture.src("Summons"), "JustDied").replace(" override", "") + "};"
    code = fixture.fixture().replace("/*PRODUCTION*/", production).replace("/*CASES*/", CASES)
    # This fixture previously extracted only narrower callbacks. The completed
    # cast also references unrelated pet and delayed-Sever APIs; bind them inertly.
    code, player_count = re.subn(r"struct Player\s*:\s*Unit\s*\{",
        "using Pet = Unit; struct Player:Unit { Pet* GetPet() { return nullptr; }", code)
    code, scheduler_count = re.subn(r"struct TaskScheduler\s*\{",
        "struct TaskContext {}; struct TaskScheduler { "
        "template<typename D, typename F> void Schedule(D, F) { assert(false); }", code)
    assert player_count == scheduler_count == 1
    with tempfile.TemporaryDirectory(prefix="coa-xoroth-passives-") as directory:
        fixture.native.OUT = Path(directory)
        result = fixture.native.compile_run(code, "passives")
        if result.returncode:
            raise AssertionError(result.stdout + result.stderr)
    if args.spell_dbc:
        raw = args.spell_dbc.read_bytes()
        count = struct.unpack_from("<I", raw, 4)[0]
        for offset in range(20, 20 + count * 936, 936):
            if struct.unpack_from("<I", raw, offset)[0] == 804012:
                row = struct.unpack_from("<234I", raw, offset)
                assert row[110:112] == (805677, 524897)
                assert tuple(struct.unpack("<i", struct.pack("<I", value))[0] for value in row[80:82]) == (-2001, -1001)
                break
        else:
            raise AssertionError("Missing Fiery Retribution cooldown contract")
    print("PASS: spent-resource threshold, refunds, owned DoT/tick preservation and owned imp-death cooldowns")


if __name__ == "__main__":
    main()
