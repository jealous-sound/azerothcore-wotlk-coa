"""Prepare copied Runemaster marker DBCs; never creates or modifies an MPQ.

Rows are from the captured CreatureDisplayInfo/CreatureModelData tables. Creature
50063 uses display 460733; creature 51335 uses display 131041. The captured asset
inventory resolves the rune to common-2.MPQ and the dagger to patch-N.MPQ, including
their 00.skin files. Verify those assets in the target client when packaging.
"""
import argparse
from pathlib import Path
import struct


ROWS = {
    "CreatureDisplayInfo": [
        [131041, 104891, 0, 0, 1.0, 255, "", "", "", "", 1, 0, 0, 0, 0, 0],
        [460733, 460733, 0, 0, 1.0, 255, "", "", "", "", 0, 0, 0, 0, 0, 0],
    ],
    "CreatureModelData": [
        [104891, 2, r"Spells\8fx_toldagor_jeshowlis_flashingdaggers_missile.m2", 1, 1.0, 1, 0,
         18.0, 12.0, 1.0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.5, 1.0, 1.0, 0.0, 0.0, 0.0],
        [460733, 0, r"world\goober\g_RuneBlue02.mdx", 1, 0.5, 0, 0,
         0.0, 0.0, 0.0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0],
    ],
}
ECHO_RETURN_DESCRIPTION = ("Return to your Echo Rune and heal for "
                           "${$500272m1+$500272ppl1+$AP*1.5} health.")


def read(raw, fields):
    magic, count, width, size, strings_size = struct.unpack_from("<4s4I", raw)
    end = 20 + count * size
    if magic != b"WDBC" or width != fields or size != fields * 4 or len(raw) != end + strings_size:
        raise ValueError(f"Expected a complete {fields}-field WDBC")
    records = list(struct.iter_unpack(f"<{fields}I", raw[20:end]))
    if len({r[0] for r in records}) != count:
        raise ValueError("Duplicate record IDs")
    return records, bytearray(raw[end:])


def transform(raw, table):
    if table == "Spell":
        return echo_button(raw)
    return merge_rows(raw, ROWS[table], table)


def merge_rows(raw, required, table):
    """Append reviewed rows without overwriting existing data; reject ID conflicts."""
    fields = len(required[0])
    records, strings = read(raw, fields)
    existing = {r[0]: r for r in records}
    for source in required:
        encoded = []
        for value in source:
            if isinstance(value, str):
                text = value.encode("utf-8") + b"\0"
                offset = strings.find(text)
                if offset < 0:
                    offset = len(strings)
                    strings.extend(text)
                value = offset
            elif isinstance(value, float):
                value = struct.unpack("<I", struct.pack("<f", value))[0]
            encoded.append(value & 0xFFFFFFFF)
        previous = existing.get(source[0])
        if previous:
            for index, value in enumerate(source):
                if isinstance(value, str):
                    start = previous[index]
                    if strings[start:strings.index(0, start)].decode("utf-8") != value:
                        raise ValueError(f"Conflicting {table} row {source[0]}")
                elif previous[index] != encoded[index]:
                    raise ValueError(f"Conflicting {table} row {source[0]}")
        else:
            records.append(tuple(encoded))
    return (struct.pack("<4s4I", b"WDBC", len(records), fields, fields * 4, len(strings)) +
            b"".join(struct.pack(f"<{fields}I", *r) for r in records) + strings)


def echo_button(raw):
    records, strings = read(raw, 234)
    index = next((i for i, row in enumerate(records) if row[0] == 500272), None)
    if index is None or records[index][208] != 38 or records[index][71] != 10:
        raise ValueError("Missing native Echo heal")
    row = list(records[index])
    text = ECHO_RETURN_DESCRIPTION.encode("utf-8") + b"\0"
    offset = strings.find(text)
    if offset < 0:
        offset = len(strings)
        strings.extend(text)
    row[153] = 0  # The native 'Heal' helper rank becomes the temporary return button.
    row[170] = offset
    records[index] = row
    return (struct.pack("<4s4I", b"WDBC", len(records), 234, 936, len(strings)) +
            b"".join(struct.pack("<234I", *r) for r in records) + strings)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--table", choices=[*ROWS, "Spell"], required=True)
    parser.add_argument("--input", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    if args.input.resolve() == args.output.resolve():
        parser.error("Use a separate output path; never overwrite the input")
    result = transform(args.input.read_bytes(), args.table)
    with args.output.open("xb") as output:
        output.write(result)


if __name__ == "__main__":
    main()
