"""Prepare Raging Zephyr's captured model records in copied DBCs; never touches MPQs."""
import argparse
import importlib.util
from pathlib import Path

spec = importlib.util.spec_from_file_location("summon_dbc", Path(__file__).with_name("runemaster_travel.py"))
dbc = importlib.util.module_from_spec(spec)
spec.loader.exec_module(dbc)

ROWS = {
    "CreatureDisplayInfo": [[407828, 407828, 0, 0, 1.0, 255, "", "", "", "", 0, 0, 0, 0, 0, 0]],
    "CreatureModelData": [[407828, 0, r"SPELLS\tornado_malfurion_01.mdx", 1, 1.0, 0, 0,
                           0.0, 0.0, 0.0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 0.0,
                           0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0]],
}


def transform(raw, table):
    return dbc.merge_rows(raw, ROWS[table], table)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--table", choices=ROWS, required=True)
    parser.add_argument("--input", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    if args.input.resolve() == args.output.resolve():
        parser.error("Use a separate output path; never overwrite the input")
    output = transform(args.input.read_bytes(), args.table)
    with args.output.open("xb") as stream:
        stream.write(output)


if __name__ == "__main__":
    main()
