import hashlib
import struct
import unittest

from generate_ascension_patch_spells import SPELLS, extract, render


def fixture(ids=SPELLS):
    strings = b"\0Renewal\0Rank\0Description\0Tooltip\0"
    rows = []
    for sid in ids:
        row = list(range(234))
        row[0] = sid
        for column, offset in zip((136, 153, 170, 187), (1, 9, 14, 26)):
            row[column] = offset
        rows.append(struct.pack("<234I", *row))
    return struct.pack("<4s4I", b"WDBC", len(rows), 234, 936, len(strings)) + b"".join(rows) + strings


class PatchSpellDataTests(unittest.TestCase):
    def test_preserves_numeric_fields_and_string_order(self):
        records = extract(fixture())
        for sid in SPELLS:
            fields, strings = records[sid]
            self.assertEqual(fields, [sid] + list(range(1, 136)) + [0] * 4 + list(range(204, 234)))
            self.assertEqual(strings, ["Renewal", "Rank", "Description", "Tooltip"])
            self.assertEqual(len(fields) * 4, 680)

    def test_missing_and_duplicate_records_rejected(self):
        for ids in ((SPELLS[0],), (SPELLS[0], SPELLS[0], SPELLS[1])):
            with self.subTest(ids=ids), self.assertRaises(ValueError):
                extract(fixture(ids))

    def test_truncated_and_wrong_layout_rejected(self):
        blob = fixture()
        for invalid in (blob[:19], blob[:-1], blob + b"x", b"WDB2" + blob[4:]):
            with self.subTest(size=len(invalid)), self.assertRaises(ValueError):
                extract(invalid)

    def test_out_of_bounds_string_rejected(self):
        blob = bytearray(fixture())
        struct.pack_into("<I", blob, 20 + 136 * 4, 99999)
        with self.assertRaises(ValueError):
            extract(blob)

    def test_generation_is_deterministic(self):
        self.assertEqual(render(fixture()), render(fixture(tuple(reversed(SPELLS)))).replace(
            hashlib.sha256(fixture(tuple(reversed(SPELLS)))).hexdigest(),
            hashlib.sha256(fixture()).hexdigest()))


if __name__ == "__main__":
    unittest.main()
