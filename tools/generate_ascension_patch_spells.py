import argparse
import hashlib
import json
from pathlib import Path
import struct


SPELLS = (560384, 560394)
ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / "src/server/coa/AscensionPatchSpellData.h"


def extract(blob):
    if len(blob) < 20:
        raise ValueError("Truncated Spell.dbc header")
    magic, count, fields, size, strings_size = struct.unpack_from("<4s4I", blob)
    if magic != b"WDBC" or fields != 234 or size != 936:
        raise ValueError("Expected the preserved 234-field CoA Spell.dbc")
    strings_start = 20 + count * size
    if len(blob) != strings_start + strings_size:
        raise ValueError("Spell.dbc size does not match its header")
    strings = blob[strings_start:]
    found = {}
    for row in struct.iter_unpack("<234I", blob[20:strings_start]):
        if row[0] not in SPELLS:
            continue
        if row[0] in found:
            raise ValueError("Duplicate tooltip spell")
        text = []
        for column in (136, 153, 170, 187):
            offset = row[column]
            end = strings.find(b"\0", offset)
            if offset >= len(strings) or end < 0:
                raise ValueError("Invalid tooltip string offset")
            text.append(strings[offset:end].decode("utf-8"))
        found[row[0]] = (list(row[:136]) + [0] * 4 + list(row[204:]), text)
    if set(found) != set(SPELLS):
        raise ValueError("Missing Ripple tooltip records")
    return found


def render(blob):
    records = extract(blob)
    lines = ['/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */',
             '#ifndef ASCENSION_PATCH_SPELL_DATA_H', '#define ASCENSION_PATCH_SPELL_DATA_H',
             '', '#include "Define.h"', '#include <array>', '#include <string_view>', '',
             'namespace AscensionPatchSpellData', '{',
             'inline constexpr std::string_view SourceSha256 =',
             '    "' + hashlib.sha256(blob).hexdigest() + '";', '',
             'struct Record', '{', '    std::array<uint32, 170> Fields;',
             '    std::array<std::string_view, 4> Strings;', '};', '',
             'inline constexpr std::array<Record, 2> Records = {{']
    for sid in SPELLS:
        fields, strings = records[sid]
        lines += ['    {', '        {{']
        for start in range(0, len(fields), 8):
            lines.append('            ' + ', '.join(str(v) + 'u' for v in fields[start:start+8]) + ',')
        lines += ['        }},', '        {{']
        lines.extend('            ' + json.dumps(s, ensure_ascii=True) + ',' for s in strings)
        lines += ['        }}', '    },']
    lines += ['}};', '}', '', '#endif', '']
    return '\n'.join(lines)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--spell-dbc', type=Path, required=True)
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    expected = render(args.spell_dbc.read_bytes())
    if args.check:
        if OUTPUT.read_text(encoding='utf-8') != expected:
            raise SystemExit('Generated tooltip spell records are stale')
        print('PASS: original spell fields and text preserved')
    else:
        OUTPUT.write_text(expected, encoding='utf-8', newline='\n')


if __name__ == '__main__':
    main()
