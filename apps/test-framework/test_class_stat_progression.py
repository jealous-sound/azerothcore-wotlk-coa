"""Exercise custom-class progression SQL on a disposable MySQL instance.

Usage: python apps/test-framework/test_class_stat_progression.py --mysql-bin /path/to/mysql/bin
Reuses the isolated named-pipe/socket fixture; no live database or worldserver is accessed.
"""

import argparse
import json
from pathlib import Path
import unittest
import zipfile

import test_enchantment_migrations as isolated_mysql


ROOT = Path(__file__).resolve().parents[2]
MIGRATION = ROOT / "data/sql/updates/pending_db_world/rev_20260912_00_custom_class_stat_progression.sql"
TABLES = ("player_class_stats", "ascension_custom_class")
FIELDS = ("BaseHP", "BaseMana", "Strength", "Agility", "Stamina", "Intellect", "Spirit")


class ClassStatProgressionTests(unittest.TestCase):
    mysql_bin = None

    @classmethod
    def setUpClass(cls):
        cls.database = isolated_mysql.EnchantmentMigrations
        cls.database.mysql_bin = cls.mysql_bin
        cls.addClassCleanup(cls.database.doClassCleanups)
        cls.database.setUpClass()
        cls.database.query("CREATE DATABASE class_stats_seed; CREATE DATABASE class_stats_test;", database=None)
        directory = ROOT / "data/coa-world"
        manifest = json.loads((directory / "baseline.json").read_text(encoding="utf-8"))
        with zipfile.ZipFile(directory / manifest["archive"]) as archive:
            for table in TABLES:
                cls.database.query(archive.read(table + ".sql").decode("utf-8"), database="class_stats_seed")
        cls.donors = {int(c): int(d) for c, d in cls.database.query(
            "SELECT class, fallback_class FROM ascension_custom_class;", database="class_stats_seed")}

    def query(self, sql):
        return self.database.query(sql, database="class_stats_test")

    def setUp(self):
        for table in TABLES:
            self.query(f"DROP TABLE IF EXISTS `{table}`; "
                       f"CREATE TABLE `{table}` LIKE `class_stats_seed`.`{table}`; "
                       f"INSERT INTO `{table}` SELECT * FROM `class_stats_seed`.`{table}`;")

    def rows(self):
        return {(int(row[0]), int(row[1])): tuple(map(int, row[2:])) for row in self.query(
            "SELECT Class, Level, BaseHP, BaseMana, Strength, Agility, Stamina, Intellect, Spirit "
            "FROM player_class_stats ORDER BY Class, Level;")}

    def apply(self):
        self.query(MIGRATION.read_text(encoding="utf-8"))

    def test_reported_level_two_collapses_are_repaired(self):
        before = self.rows()
        self.assertEqual(sum(before[c, 2][0] < before[c, 1][0] for c in range(12, 33)), 21)
        self.assertEqual(before[12, 1], (74, 0, 27, 26, 24, 14, 18))
        self.assertEqual(before[12, 2], (32, 0, 22, 24, 22, 20, 20))
        self.apply()
        after = self.rows()
        self.assertEqual(after[12, 2], (81, 0, 28, 27, 25, 14, 18))
        self.assertEqual(after[17, 2], (74, 0, 32, 22, 29, 18, 12))
        self.assertEqual(after[16, 2], (72, 106, 16, 15, 20, 33, 29))
        self.assertEqual(after[26, 2], (80, 91, 28, 16, 26, 28, 15))

    def test_every_level_keeps_legacy_gains_from_the_custom_start(self):
        before = self.rows()
        self.apply()
        after = self.rows()
        self.assertEqual(before.keys(), after.keys())
        checked = 0

        def total_mana(stats):
            mana, intellect = stats[1], stats[5]
            return mana + min(intellect, 20) + 15 * max(intellect - 20, 0) if mana else 0

        for custom in range(12, 33):
            donor = self.donors[custom]
            for level in range(2, 81):
                for index, field in enumerate(FIELDS):
                    with self.subTest(custom=custom, level=level, field=field):
                        actual_gain = after[custom, level][index] - after[custom, level - 1][index]
                        expected_gain = before[donor, level][index] - before[donor, level - 1][index]
                        if field == "BaseMana" and before[custom, 1][index] == 0:
                            expected_gain = 0
                        self.assertEqual(actual_gain, expected_gain)
                        if field != "BaseMana":
                            self.assertGreaterEqual(actual_gain, 0)
                # Legacy base mana can dip when Intellect rises; the resulting mana pool must not drop.
                self.assertGreaterEqual(total_mana(after[custom, level]), total_mana(after[custom, level - 1]))
                checked += 1
        self.assertEqual(checked, 1659)

    def test_replay_preserves_level_one_legacy_and_out_of_scope_rows(self):
        self.query("INSERT INTO player_class_stats VALUES (12,81,9999,0,500,500,500,500,500), "
                   "(33,2,123,456,20,21,22,23,24);")
        before = self.rows()
        self.apply()
        after = self.rows()
        self.assertEqual(before.keys(), after.keys())
        for key in before:
            if not (12 <= key[0] <= 32 and 2 <= key[1] <= 80):
                self.assertEqual(before[key], after[key], key)
        self.apply()
        self.assertEqual(after, self.rows())
        self.assertGreater(MIGRATION.name, "rev_20260903_01_live_class_baseline.sql")

    def test_zero_mana_and_signed_stat_offsets(self):
        self.apply()
        rows = self.rows()
        for custom in (12, 14, 17, 18, 19, 20, 21, 30):
            for level in range(1, 81):
                self.assertEqual(rows[custom, level][1], 0, (custom, level))
        self.assertEqual(rows[13, 2][1], 68)  # Shaman mana curve minus the 23-point starting offset.
        self.assertEqual(rows[32, 2][1], 51)
        self.assertEqual(rows[12, 2][5], 14)  # Negative Intellect offset must not underflow unsigned SQL.
        self.assertEqual(rows[23, 2][3], 13)  # Negative Agility offset.

    def test_missing_donor_level_does_not_delete_custom_stats(self):
        self.query("DELETE FROM player_class_stats WHERE Class=4 AND Level=2;")
        before = self.rows()
        self.apply()
        after = self.rows()
        self.assertEqual(before.keys(), after.keys())
        for custom in (12, 14, 19, 30):
            self.assertEqual(before[custom, 2], after[custom, 2])


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--mysql-bin", required=True, type=Path)
    args, remaining = parser.parse_known_args()
    ClassStatProgressionTests.mysql_bin = args.mysql_bin.resolve()
    unittest.main(argv=[__file__, *remaining], verbosity=2)
