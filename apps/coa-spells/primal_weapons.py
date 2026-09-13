"""Prepare the Primal Weapons selector in a copied CoA Spell.dbc.

This is source tooling for the matching client/server data delivery. It never
opens an MPQ or changes an installed client. Supply a separate output path when
packaging is requested; the input and all unrelated spell rows are preserved.
"""

import argparse
from pathlib import Path
import struct


SPELL_ID = 537218
EDITS = {4: 0, 6: 0, 32: 0, 40: 0, 41: 0, 71: 3, 74: 0, 80: 0,
         95: 0, 110: 0, 122: 0, 123: 0, 124: 0, 204: 15}
DESCRIPTION = ("Empower yourself according to your equipped weapon: one-handed weapons grant "
               "Bestial Might; two-handed weapons grant Primal Might.@s:801242:0@@s:704098:0@")


def transform(raw):
    magic, count, fields, size, strings_size = struct.unpack_from("<4s4I", raw)
    if magic != b"WDBC" or fields != 234 or size != 936 or len(raw) != 20 + count * size + strings_size:
        raise ValueError("Expected a complete 234-field CoA Spell.dbc")
    strings_at = 20 + count * size
    strings = bytearray(raw[strings_at:])
    records = bytearray(raw[20:strings_at])
    found = []
    for index in range(count):
        if struct.unpack_from("<I", records, index * size)[0] == SPELL_ID:
            found.append(index * size)
    if len(found) != 1:
        raise ValueError("Primal Weapons must have exactly one spell row")
    offset = found[0]
    row = list(struct.unpack_from("<234I", records, offset))
    if row[208] != 37 or row[71] not in (3, 6) or row[95] not in (0, 4) or any(row[72:74]):
        raise ValueError("Unexpected Primal Weapons effect layout")
    for field, value in EDITS.items():
        row[field] = value
    row[153] = 0  # The old Specialization rank is not an active ability rank.
    encoded = DESCRIPTION.encode("utf-8") + b"\0"
    previous = strings.find(encoded)
    if previous < 0:
        previous = len(strings)
        strings.extend(encoded)
    row[170] = previous
    struct.pack_into("<234I", records, offset, *row)
    return struct.pack("<4s4I", magic, count, fields, size, len(strings)) + records + strings


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    if args.input.resolve() == args.output.resolve():
        parser.error("Use a separate output path; never overwrite the input")
    candidate = transform(args.input.read_bytes())
    with args.output.open("xb") as output:
        output.write(candidate)
    print(f"Prepared Primal Weapons spell {SPELL_ID}: {args.output}")


if __name__ == "__main__":
    main()
