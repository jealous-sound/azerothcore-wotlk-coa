CLI_DESCRIPTION = """Regress Barbaric Rage's cooldown contract: ability cooldowns, not the shared global cooldown (#3945)."""
import argparse
from pathlib import Path
import os
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]


def extract_if_body(source, needle):
    start = source.index(needle)
    after_cond = source.index(")", start) + 1
    brace = source.index("{", after_cond)
    semicolon = source.index(";", after_cond)
    if brace < semicolon:
        end = brace + 1
        depth = 1
        while depth:
            depth += (source[end] == "{") - (source[end] == "}")
            end += 1
        return source[start:end]
    return source[start:semicolon + 1]


def main():
    parser = argparse.ArgumentParser(description=CLI_DESCRIPTION)
    parser.add_argument("--source-ref", help="Use an earlier ApplyContracts as a negative control")
    args = parser.parse_args()
    path = "modules/mod-ascension-compat/src/AscensionBarbarianCompletion.cpp"
    if args.source_ref:
        source = subprocess.check_output(["git", "show", f"{args.source_ref}:{path}"],
                                          cwd=ROOT).decode("utf-8")
    else:
        source = (ROOT / path).read_text(encoding="utf-8")
    block = extract_if_body(source, "if (id == 804337)")
    code = r"""
#include <cstdint>
#include <cassert>
using uint32 = std::uint32_t;
using int32 = std::int32_t;
constexpr uint32 EFFECT_0 = 0;
constexpr int32 SPELLMOD_COOLDOWN = 11, SPELLMOD_GLOBAL_COOLDOWN = 21;
struct SpellEffectInfo { int32 MiscValue = SPELLMOD_GLOBAL_COOLDOWN; };
struct SpellInfo { uint32 Id = 0; SpellEffectInfo Effects[3]; };
void ApplyBarbaricRageContract(SpellInfo* info)
{
    uint32 id = info->Id;
""" + block + r"""
}
int main()
{
    SpellInfo rage;
    rage.Id = 804337;
    ApplyBarbaricRageContract(&rage);
    // Barbaric Rage must reduce ability cooldowns, not the ~1.5s global cooldown it shipped with.
    assert(rage.Effects[EFFECT_0].MiscValue == SPELLMOD_COOLDOWN);
    SpellInfo other;
    other.Id = 801761; // An unrelated spell must not be touched by this contract.
    ApplyBarbaricRageContract(&other);
    assert(other.Effects[EFFECT_0].MiscValue == SPELLMOD_GLOBAL_COOLDOWN);
}
"""
    with tempfile.TemporaryDirectory(prefix="coa-barbaric-rage-") as directory:
        out = Path(directory)
        cpp, exe = out / "rage.cpp", out / "rage.exe"
        cpp.write_text(code, encoding="utf-8")
        compiler = Path(os.environ["VCToolsInstallDir"]) / "bin/Hostx64/x64/cl.exe"
        subprocess.run([str(compiler), "/nologo", "/std:c++20", "/EHsc", "/W4", "/WX", "/utf-8",
                        str(cpp), "/Fe" + str(exe)], cwd=out, check=True, timeout=60)
        subprocess.run([str(exe)], cwd=out, check=True, timeout=15)
    print("PASS: Barbaric Rage retargets ability cooldowns; unrelated spells are untouched")


if __name__ == "__main__":
    main()
