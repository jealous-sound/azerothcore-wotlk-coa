"""Regression for GitHub issue #4075: Tinker Shield Beacon granted no armor (then stacked it).

Three compounding bugs, found across two rounds of in-game testing. All are exercised here by
extracting the actual production blocks and compiling them against minimal native-shaped stubs.
No server build, database service or installed files are used.

1. AscensionTinkerSummons.cpp cast the armor helper (801256/803804-803808) through the plain
   unit-targeted Cast() helper. Those helpers use effect SPELL_EFFECT_PERSISTENT_AREA_AURA with
   implicit target TARGET_DEST_DYNOBJ_ALLY (confirmed via out/client-dbc/Spell.dbc), which needs
   an explicit destination or Spell::CheckDst falls back to the caster's own position.
2. Even with a correct destination, DynObjAura::FillTargetMap (SpellAuras.cpp) only special-cases
   TARGET_DEST_DYNOBJ_ALLY/TARGET_UNIT_DEST_AREA_ALLY on effect TargetB, not TargetA. These helper
   spells are authored with TARGET_DEST_DYNOBJ_ALLY on TargetA only, so the periodic area search
   fell back to AnyAoETargetUnitInObjectRangeCheck, which requires an attackable (hostile) target
   and so never selects the friendly ally the ground effect is meant to buff.
3. Once (1) and (2) landed, the device re-cast the helper on every ~1s AI tick regardless of
   whether the ally already had it applied. Each cast creates its own independent DynObjAura,
   which (unlike a plain unit aura) is not replaced by a later application of the same
   spell/caster, so every tick stacked another full +armor application on top of the last one.
"""

import argparse
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]

CAST_START = "uint32 helper = entry == 50036 ? 801256"
CAST_END = "me->CastSpell(ally->GetPositionX(),ally->GetPositionY(),ally->GetPositionZ(),helper,true);"

TARGET_START = "if (id == 801256 || id == 803804 || id == 803805 || id == 803806 || id == 803807 || id == 803808)"
TARGET_END = "info->Effects[EFFECT_0].TargetB = SpellImplicitTargetInfo(TARGET_DEST_DYNOBJ_ALLY);\n    }"


def compile_and_run(compiler, name, text):
    with tempfile.TemporaryDirectory(prefix=f"shield-beacon-{name}-", ignore_cleanup_errors=True) as directory:
        out = Path(directory)
        cpp = out / "harness.cpp"
        cpp.write_text(text, encoding="utf-8")
        executable = out / (f"{name}.exe" if os.name == "nt" else name)
        if Path(compiler).stem.lower() == "cl":
            flags = ["/nologo", "/std:c++20", "/EHsc", "/W4", "/WX", "/utf-8",
                     str(cpp), "/Fe" + str(executable)]
        else:
            flags = ["-std=c++20", "-Wall", "-Wextra", "-Werror", str(cpp), "-o", str(executable)]
        subprocess.run([compiler, *flags], cwd=out, check=True)
        subprocess.run([str(executable)], cwd=out, check=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source-ref", help="Read C++ from a local Git ref to reproduce the pre-fix failure.")
    args = parser.parse_args()

    def source(path):
        if args.source_ref:
            return subprocess.check_output(["git", "show", f"{args.source_ref}:{path}"], cwd=ROOT).decode()
        return (ROOT / path).read_text(encoding="utf-8")

    summons = source("modules/mod-ascension-compat/src/AscensionTinkerSummons.cpp")
    cast_start = summons.index(CAST_START)
    cast_end = summons.index(CAST_END, cast_start) + len(CAST_END)
    cast_block = summons[cast_start:cast_end]

    cast_harness = (HERE / "harness.cpp").read_text(encoding="utf-8")
    cast_harness = cast_harness.replace("    // ACTUAL_BLOCK", cast_block)

    contracts = source("modules/mod-ascension-compat/src/AscensionTinkerContracts.cpp")
    try:
        target_start = contracts.index(TARGET_START)
        target_end = contracts.index(TARGET_END, target_start) + len(TARGET_END)
        target_block = contracts[target_start:target_end]
    except ValueError:
        # Pre-fix source doesn't have this correction at all: an empty block reproduces
        # the bug (TargetB is left at its authored, unset value).
        target_block = ""

    target_harness = (HERE / "harness_target.cpp").read_text(encoding="utf-8")
    target_harness = target_harness.replace("        // ACTUAL_BLOCK", target_block)

    vc_tools = os.environ.get("VCToolsInstallDir")
    compiler = (str(Path(vc_tools) / "bin/Hostx64/x64/cl.exe") if vc_tools else
                shutil.which(os.environ.get("CXX", "cl.exe" if os.name == "nt" else "c++")))
    if not compiler:
        raise RuntimeError("Enable a C++20 compiler (VS Developer PowerShell on Windows).")

    compile_and_run(compiler, "shield_beacon_cast", cast_harness)
    compile_and_run(compiler, "shield_beacon_target", target_harness)


if __name__ == "__main__":
    main()
