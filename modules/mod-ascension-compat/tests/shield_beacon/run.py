"""Regression for GitHub issue #4075: Tinker Shield Beacon granted no armor.

Extracts the actual Shield Beacon helper-cast block from AscensionTinkerSummons.cpp and
compiles it against a tiny Unit stub that records how it was cast. Helper spells
801256/803804-803808 use effect SPELL_EFFECT_PERSISTENT_AREA_AURA with implicit target
TARGET_DEST_DYNOBJ_ALLY (confirmed via out/client-dbc/Spell.dbc): that effect needs an
explicit destination or Spell::CheckDst falls back to the caster's own position, so the
persistent-area-aura granting armor centers on the device instead of the ally standing
near it. No server build, database service or installed files are used.
"""

import argparse
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]

START_ANCHOR = "uint32 helper = entry == 50036 ? 801256"
END_ANCHOR = "aura->SetDuration(2000);"


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source-ref", help="Read C++ from a local Git ref to reproduce the pre-fix failure.")
    args = parser.parse_args()

    def source(path):
        if args.source_ref:
            return subprocess.check_output(["git", "show", f"{args.source_ref}:{path}"], cwd=ROOT).decode()
        return (ROOT / path).read_text(encoding="utf-8")

    summons = source("modules/mod-ascension-compat/src/AscensionTinkerSummons.cpp")
    start = summons.index(START_ANCHOR)
    end = summons.index(END_ANCHOR, start) + len(END_ANCHOR)
    block = summons[start:end]

    harness = (HERE / "harness.cpp").read_text(encoding="utf-8")
    harness = harness.replace("    // ACTUAL_BLOCK", block)

    vc_tools = os.environ.get("VCToolsInstallDir")
    compiler = (str(Path(vc_tools) / "bin/Hostx64/x64/cl.exe") if vc_tools else
                shutil.which(os.environ.get("CXX", "cl.exe" if os.name == "nt" else "c++")))
    if not compiler:
        raise RuntimeError("Enable a C++20 compiler (VS Developer PowerShell on Windows).")
    with tempfile.TemporaryDirectory(prefix="shield-beacon-") as directory:
        out = Path(directory)
        cpp = out / "harness.cpp"
        cpp.write_text(harness, encoding="utf-8")
        executable = out / ("shield_beacon.exe" if os.name == "nt" else "shield_beacon")
        if Path(compiler).stem.lower() == "cl":
            flags = ["/nologo", "/std:c++20", "/EHsc", "/W4", "/WX", "/utf-8",
                     str(cpp), "/Fe" + str(executable)]
        else:
            flags = ["-std=c++20", "-Wall", "-Wextra", "-Werror", str(cpp), "-o", str(executable)]
        subprocess.run([compiler, *flags], cwd=out, check=True)
        subprocess.run([str(executable)], cwd=out, check=True)


if __name__ == "__main__":
    main()
