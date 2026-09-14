"""Check Hellfire Imp initialization against the native neutral-target and immunity checks."""
import argparse
import os
from pathlib import Path
import runpy
import shutil
import subprocess
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
method = runpy.run_path(str(HERE.parent / "client_compat/run.py"))["method"]

CASES = r"""
struct Player : Unit {};
struct Creature : Unit
{
    struct Threat
    {
        void RegisterRedirectThreat(uint32 spell, uint32 owner, uint32 percent)
        {
            assert(spell == 706571 && owner == 1 && percent == 100);
        }
    } threat;
    Threat& GetThreatMgr() { return threat; }
};
struct StateData { std::vector<uint32> imps; } state;
StateData& State(Player*) { return state; }
void Cast(Player*, Creature*, uint32 spell) { assert(spell == 800443); }
void Initialize(Creature* me, Player* player)
{
    uint32 owner = 1;
    // ACTUAL_IMP
}
int main()
{
    Player player;
    player.pvp = 5;
    Unit neutral;
    Creature imp;
    imp.entry = 50301;
    assert(!imp.NativeAttackAdmission(&neutral));
    Initialize(&imp, &player);
    assert(imp.m_ControlledByPlayer && imp.HasUnitFlag(UNIT_FLAG_PLAYER_CONTROLLED));
    assert(imp.pvp == player.pvp);
    assert(imp.NativeAttackAdmission(&neutral));
    neutral.immuneNPC = true;
    assert(imp.NativeAttackAdmission(&neutral));
    neutral.immunePC = true;
    assert(!imp.NativeAttackAdmission(&neutral));
    neutral.immunePC = neutral.immuneNPC = false;
    neutral.flags = UNIT_FLAG_NON_ATTACKABLE;
    assert(!imp.NativeAttackAdmission(&neutral));
    for (uint32 entry : {50375u, 50268u, 51323u})
    {
        Creature other;
        other.entry = entry;
        Initialize(&other, &player);
        assert(!other.m_ControlledByPlayer && !other.flags && !other.pvp);
    }
}
"""


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source-ref", help="Use older Imp initialization as a negative control.")
    args = parser.parse_args()
    path = "modules/mod-ascension-compat/src/AscensionXorothSummons.cpp"
    source = (subprocess.check_output(["git", "show", f"{args.source_ref}:{path}"], cwd=ROOT).decode("utf-8")
              if args.source_ref else (ROOT / path).read_text(encoding="utf-8"))
    initialization = method(source, "void IsSummonedBy(WorldObject* summoner)")
    imp = method(initialization, "if (me->GetEntry() == 50301)")
    unit = (ROOT / "src/server/game/Entities/Unit/Unit.cpp").read_text(encoding="utf-8")
    attack = method(unit, "bool Unit::_IsValidAttackTarget(")
    attack = attack[attack.index("    // check flags"):attack.index("    // PvP, PvC, CvP case")]
    # Reuse the small unit fixture already covering these exact native checks for Sentry.
    harness = (HERE.parent / "tinker_sentry/harness.cpp").read_text(encoding="utf-8")
    harness = harness[:harness.index("// ACTUAL_TIMER")].replace("// ACTUAL_ADMISSION", attack)
    harness += CASES.replace("// ACTUAL_IMP", imp)
    compiler = shutil.which(os.environ.get("CXX", "g++"))
    assert compiler, "Set CXX to a C++20 compiler."
    with tempfile.TemporaryDirectory(prefix="coa-hellfire-imp-") as directory:
        out = Path(directory)
        cpp, exe = out / "imp.cpp", out / "imp.exe"
        cpp.write_text(harness, encoding="utf-8")
        if Path(compiler).stem.lower() == "cl":
            flags = ["/nologo", "/std:c++20", "/EHsc", "/W4", "/WX", "/utf-8", str(cpp), "/Fe" + str(exe)]
        else:
            flags = ["-std=c++20", "-Wall", "-Wextra", "-Werror", str(cpp), "-o", str(exe)]
        subprocess.run([compiler, *flags], cwd=out, check=True)
        subprocess.run([str(exe)], cwd=out, check=True)
    print("PASS: Imp control flags, neutral-target admission, PC/NPC immunities, PvP state and other summons")


if __name__ == "__main__":
    main()
