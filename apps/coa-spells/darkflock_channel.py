"""Mark Darkflock as a channeled spell in a copied CoA Spell.dbc.

All five Darkflock ranks carry the channel-only ChannelInterruptFlags and
SPELL_ATTR5_ALLOW_ACTION_DURING_CHANNEL, but not SPELL_ATTR1_IS_CHANNELED. The
server now channels them; the client still builds the tooltip from its own row
and reads "Instant" until it receives the same attribute.

This is source tooling for the matching client/server data delivery. It never
opens an MPQ or changes an installed client. Supply a separate output path when
packaging is requested; the input and all unrelated spell rows are preserved.
The server side of the same change lives in
modules/mod-ascension-compat/src/AscensionWitchHunterCompletion.cpp.
"""

import argparse
from pathlib import Path
import struct


SPELL_IDS = (520270, 520708, 520709, 562024, 562025)
SPELL_FAMILY_WITCH_HUNTER = 21
SPELL_ATTR1_IS_CHANNELED = 0x4
SPELL_ATTR5_ALLOW_ACTION_DURING_CHANNEL = 0x1
SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE = 227
FIELD_ATTRIBUTES_EX = 5
FIELD_ATTRIBUTES_EX5 = 9
FIELD_EFFECT_APPLY_AURA_NAME = 95
FIELD_SPELL_FAMILY = 208


def transform(raw):
    magic, count, fields, size, strings_size = struct.unpack_from("<4s4I", raw)
    if magic != b"WDBC" or fields != 234 or size != 936 or len(raw) != 20 + count * size + strings_size:
        raise ValueError("Expected a complete 234-field CoA Spell.dbc")
    strings_at = 20 + count * size
    strings = raw[strings_at:]
    records = bytearray(raw[20:strings_at])
    found = {}
    for index in range(count):
        spell = struct.unpack_from("<I", records, index * size)[0]
        if spell in SPELL_IDS:
            found.setdefault(spell, []).append(index * size)
    if sorted(found) != sorted(SPELL_IDS) or any(len(rows) != 1 for rows in found.values()):
        raise ValueError("Each Darkflock rank must have exactly one spell row")
    for spell, (offset,) in found.items():
        row = list(struct.unpack_from("<234I", records, offset))
        if (row[FIELD_SPELL_FAMILY] != SPELL_FAMILY_WITCH_HUNTER or
                row[FIELD_EFFECT_APPLY_AURA_NAME] != SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE or
                not row[FIELD_ATTRIBUTES_EX5] & SPELL_ATTR5_ALLOW_ACTION_DURING_CHANNEL):
            raise ValueError(f"Unexpected Darkflock record {spell}")
        row[FIELD_ATTRIBUTES_EX] |= SPELL_ATTR1_IS_CHANNELED
        struct.pack_into("<234I", records, offset, *row)
    return struct.pack("<4s4I", magic, count, fields, size, strings_size) + bytes(records) + strings


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
    print(f"Prepared {len(SPELL_IDS)} Darkflock ranks as channeled spells: {args.output}")


if __name__ == "__main__":
    main()
