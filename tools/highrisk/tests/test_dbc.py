"""Check preservation and fail-closed behavior using small synthetic DBC files."""
from pathlib import Path
import importlib.util
import json
import struct
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location("prepare_dbc", ROOT / "tools/prepare_dbc.py")
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)


class DbcTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name)
        self.original_package = module.PACKAGE
        module.PACKAGE = self.root
        (self.root / "data").mkdir()
        self.dbc = self.root / "dbc"
        self.dbc.mkdir()
        self.raw = struct.pack("<4s4I3I", b"WDBC", 1, 3, 12, 5, 1, 7, 1) + b"\0old\0"
        (self.dbc / "Test.dbc").write_bytes(self.raw)
        self.definition = {"Test": {"fields": 3, "format": "nis", "rows": [
            {"id": 1, "values": [1, 7, 0], "semantic": [1, 7], "strings": {"2": "old"}},
            {"id": 2, "values": [2, 9, 0], "semantic": [2, 9], "strings": {"2": "new"}},
        ]}}

    def tearDown(self):
        module.PACKAGE = self.original_package
        self.temp.cleanup()

    def prepare(self):
        (self.root / "data/server-dbc-rows.json").write_text(json.dumps(self.definition))
        return module.prepare(self.dbc, self.root / "staged")

    def test_preserves_old_rows_and_strings(self):
        self.prepare()
        _, rows, strings = module.read(self.root / "staged/Test.dbc")
        self.assertEqual(rows[1], (1, 7, 1))
        self.assertTrue(strings.startswith(b"\0old\0"))
        self.assertEqual(strings[rows[2][2]:], b"new\0")
        self.assertEqual((self.dbc / "Test.dbc").read_bytes(), self.raw)

    def test_conflict_writes_nothing(self):
        self.definition["Test"]["rows"][0]["semantic"] = [1, 8]
        with self.assertRaisesRegex(ValueError, "Conflicting Test ID 1"):
            self.prepare()
        self.assertFalse((self.root / "staged").exists())

    def test_missing_baseline_writes_nothing(self):
        self.definition["Test"]["rows"][1]["values"] = None
        with self.assertRaisesRegex(ValueError, "Missing baseline"):
            self.prepare()
        self.assertFalse((self.root / "staged").exists())

    def test_duplicate_id_rejected(self):
        raw = struct.pack("<4s4I6I", b"WDBC", 2, 3, 12, 1, 1, 2, 0, 1, 3, 0) + b"\0"
        (self.dbc / "Test.dbc").write_bytes(raw)
        with self.assertRaisesRegex(ValueError, "Duplicate ID"):
            self.prepare()
