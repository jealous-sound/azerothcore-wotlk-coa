"""Check actual Air Elemental ownership/procs and native Invigoration stacking."""
import argparse
import importlib.util
from pathlib import Path
import re
import sqlite3
import struct
import tempfile

ROOT = Path(__file__).resolve().parents[4]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--workspace-tools", type=Path, default=ROOT.parent / "tools")
    parser.add_argument("--spell-dbc", type=Path)
    args = parser.parse_args()
    spec = importlib.util.spec_from_file_location("air_compile", args.workspace_tools / "Test-LocalLoginCollections.py")
    native = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(native)
    code = Path(__file__).with_name("harness.cpp").read_text()
    pet_defines = (ROOT / "src/server/game/Entities/Pet/PetDefines.h").read_text()
    enums = [re.search(r"constexpr auto MAX_PET_STABLES = \d+;", pet_defines)[0]]
    for path, names in [
        ("src/server/shared/SharedDefines.h", ["Classes", "SpellAttr1"]),
        ("src/server/game/Entities/Pet/PetDefines.h", ["PetType", "PetSaveMode"]),
        ("src/server/game/Spells/Auras/SpellAuraDefines.h", ["AuraRemoveMode"]),
    ]:
        source = (ROOT / path).read_text()
        enums.extend(native.extractor.extract(source, r"enum " + name + r"\b") + ";" for name in names)
    code = code.replace("// NATIVE_ENUMS", "\n".join(enums))
    stack = native.extractor.extract((ROOT / "src/server/game/Spells/Auras/SpellAuras.cpp").read_text(), r"bool Aura::ModStackAmount\(")
    code = code.replace("// NATIVE_STACK", stack)
    source = (ROOT / "modules/mod-ascension-compat/src/AscensionStormbringerPet.cpp").read_text()
    source = re.sub(r"^#include.*\n", "", source, flags=re.M)
    source = source.replace(": public SpellScript\n{", ": public SpellScript\n{\npublic:")
    source = source.replace(": public AuraScript\n{", ": public AuraScript\n{\npublic:")
    code = code.replace("// ACTUAL_SOURCE", source)
    with tempfile.TemporaryDirectory(prefix="coa-air-elemental-") as directory:
        native.OUT = Path(directory)
        result = native.compile_run(code, "air-elemental")
        assert result.returncode == 0, result.stdout + result.stderr
    db = sqlite3.connect(":memory:")
    db.executescript("""
        CREATE TABLE creature_template (entry INT, name TEXT, minlevel INT, maxlevel INT, faction INT,
            unit_class INT, type INT, family INT, spell1 INT, spell2 INT, spell3 INT);
        CREATE TABLE creature_template_model (CreatureID INT, Idx INT, CreatureDisplayID INT,
            DisplayScale FLOAT, Probability FLOAT);
        CREATE TABLE spell_script_names (spell_id INT, ScriptName TEXT);
        CREATE TABLE spell_proc (SpellId INT, ProcFlags INT, SpellTypeMask INT, SpellPhaseMask INT,
            HitMask INT, AttributesMask INT, Chance INT);
        INSERT INTO spell_script_names VALUES (806020, 'unrelated');
    """)
    sql = (ROOT / "data/sql/updates/pending_db_world/rev_20260914_09_air_elemental.sql").read_text()
    db.executescript(sql)
    before = list(db.iterdump())
    db.executescript(sql)
    assert before == list(db.iterdump())
    assert db.execute("SELECT CreatureDisplayID FROM creature_template_model WHERE CreatureID=500941").fetchone() == (8714,)
    assert db.execute("SELECT spell1, spell2, spell3, family FROM creature_template WHERE entry=500941").fetchone() == (806016, 300836, 804022, 0)
    assert db.execute("SELECT * FROM spell_proc").fetchone() == (806020, 332116, 1, 2, 3, 2, 100)
    assert db.execute("SELECT COUNT(*) FROM spell_script_names").fetchone() == (3,)
    if args.spell_dbc:
        raw = args.spell_dbc.read_bytes()
        count = struct.unpack_from("<I", raw, 4)[0]
        wanted = {804019, 806010, 806020, 500348, 680918, 806016, 300836, 804022}
        rows = {r[0]: r for r in struct.iter_unpack("<234I", raw[20:20 + count * 936]) if r[0] in wanted}
        assert rows[804019][71] == 56 and rows[804019][110] == 500941
        assert rows[806010][71:74] == (6, 6, 140) and rows[806010][86:89] == (5, 5, 5)
        assert rows[806010][118] == 806020 and rows[806020][95] == 42 and rows[806020][116] == 500348
        assert rows[500348][72] == 64 and rows[500348][117] == 680918
        assert rows[680918][49] == 10 and rows[680918][95] == 79 and rows[680918][80:81] == (2,)
        assert rows[806016][71] == 2 and rows[806016][104] == 2
        assert rows[300836][71:73] == (2, 183) and rows[300836][117] == 300838
        assert rows[804022][71] == 65 and rows[804022][95] == 216
        raw = (args.spell_dbc.parent / "CreatureDisplayInfo.dbc").read_bytes()
        count = struct.unpack_from("<I", raw, 4)[0]
        displays = {r[0]: r for r in struct.iter_unpack("<16I", raw[20:20 + count * 64])}
        assert displays[8714][1] == 591
    print("PASS: Air Elemental native pet type, load-safe passives, damage procs, native stack duration and SQL")


if __name__ == "__main__":
    main()
