"""Exercise the pending enchantment migrations on a disposable MySQL 8 database.

Usage: python apps/test-framework/test_enchantment_migrations.py --mysql-bin /path/to/mysql/bin
Add --before-ref <commit> to reproduce the original duplicate-key failure before checking the fix.
The server uses a unique local socket/named pipe, no TCP listeners, and a temporary data directory.
"""

import argparse
import os
from pathlib import Path
import re
import subprocess
import tempfile
import time
import unittest
import uuid


ROOT = Path(__file__).resolve().parents[2]
PENDING = ROOT / "data/sql/updates/pending_db_world"
MIGRATIONS = [
    PENDING / "rev_20260903_02_local_enchantment_levels.sql",
    PENDING / "rev_20260903_03_local_enchantment_level_scripts.sql",
    PENDING / "rev_20260903_04_local_enchantment_level_descriptions.sql",
]
TABLES = ("item_template", "spell_script_names", "spell_linked_spell", "spellitemenchantment_dbc")
REMOVED_BINDINGS = {
    ("64441", "spell_gen_proc_above_75"),
    ("64579", "spell_gen_proc_above_75"),
    ("59917", "spell_gen_disabled_above_70"),
}
PROCESS_OPTIONS = {"creationflags": subprocess.CREATE_NO_WINDOW} if os.name == "nt" else {}


class EnchantmentMigrations(unittest.TestCase):
    mysql_bin = None
    before_ref = None

    @classmethod
    def setUpClass(cls):
        suffix = ".exe" if os.name == "nt" else ""
        cls.mysql = cls.mysql_bin / ("mysql" + suffix)
        cls.mysqld = cls.mysql_bin / ("mysqld" + suffix)
        if not cls.mysql.is_file() or not cls.mysqld.is_file():
            raise RuntimeError("--mysql-bin must contain both mysql and mysqld")

        cls.temporary = tempfile.TemporaryDirectory(prefix="coa-enchant-")
        cls.directory = Path(cls.temporary.name).resolve()
        cls.process = None
        cls.addClassCleanup(cls.stop_mysql)
        cls.log = cls.directory / "mysql.log"
        server = [str(cls.mysqld), "--no-defaults", f"--basedir={cls.mysql_bin.parent}",
                  f"--datadir={cls.directory / 'data'}", f"--log-error={cls.log}"]
        print("Initializing isolated MySQL data directory...", flush=True)
        subprocess.run(server + ["--initialize-insecure"], check=True, capture_output=True,
                       timeout=90, **PROCESS_OPTIONS)

        endpoint = ("coa_enchant_" + uuid.uuid4().hex if os.name == "nt"
                    else str(cls.directory / "mysql.sock"))
        cls.client = [str(cls.mysql), "--no-defaults", "--no-login-paths", "--user=root", "--batch",
                      "--skip-column-names", "--default-character-set=utf8mb4", "--connect-timeout=1",
                      "--max-allowed-packet=1GB", f"--socket={endpoint}"]
        server += ["--skip-networking", "--mysqlx=OFF", "--max-allowed-packet=1GB", f"--socket={endpoint}"]
        if os.name == "nt":
            server.append("--enable-named-pipe")
            cls.client += ["--protocol=PIPE", "--host=."]
        else:
            cls.client.append("--protocol=SOCKET")
        cls.process = subprocess.Popen(server, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
                                       **PROCESS_OPTIONS)
        deadline = time.monotonic() + 40
        while time.monotonic() < deadline:
            if cls.process.poll() is not None:
                raise RuntimeError(cls.log.read_text(encoding="utf-8"))
            result = subprocess.run(cls.client, input="SELECT 1;\n", text=True, capture_output=True,
                                    timeout=5, **PROCESS_OPTIONS)
            if result.returncode == 0 and result.stdout.strip() == "1":
                break
            time.sleep(0.2)
        else:
            raise RuntimeError("Isolated MySQL startup timed out: " + cls.log.read_text(encoding="utf-8"))

        print("Loading the four repository base tables...", flush=True)
        cls.query("CREATE DATABASE `enchantment_seed`; CREATE DATABASE `enchantment_test`;", database=None)
        for table in TABLES:
            source = ROOT / "data/sql/base/db_world" / (table + ".sql")
            cls.query(source.read_text(encoding="utf-8"), database="enchantment_seed")
            cls.query(f"CREATE TABLE `{table}` LIKE `enchantment_seed`.`{table}`;")
        cls.columns = [row[0] for row in cls.query("SHOW COLUMNS FROM `item_template`;")]

    @classmethod
    def stop_mysql(cls):
        if cls.process is not None and cls.process.poll() is None:
            try:
                subprocess.run(cls.client, input="SHUTDOWN;\n", text=True, capture_output=True,
                               timeout=10, **PROCESS_OPTIONS)
                cls.process.wait(timeout=15)
            except subprocess.TimeoutExpired:
                cls.process.kill()
                cls.process.wait(timeout=10)
        # Only clean the exact temporary directory created by this test, after its server exits.
        if cls.directory.parent != Path(tempfile.gettempdir()).resolve() or not cls.directory.name.startswith(
                "coa-enchant-"):
            raise RuntimeError("Unexpected test data directory; refusing cleanup")
        cls.temporary.cleanup()

    @classmethod
    def query(cls, sql, database="enchantment_test", expected_error=None):
        marker = "enchant_test_" + uuid.uuid4().hex
        command = cls.client + ([database] if database else [])
        result = subprocess.run(command, input=f"SELECT '{marker}_begin';\n{sql}\nSELECT '{marker}_end';\n",
                                text=True, encoding="utf-8", capture_output=True, timeout=60, **PROCESS_OPTIONS)
        lines = result.stdout.splitlines()
        if not lines or lines[0] != marker + "_begin":
            raise AssertionError("Missing MySQL result frame: " + result.stderr)
        if expected_error:
            if result.returncode == 0 or expected_error not in result.stderr:
                raise AssertionError("Expected SQL failure was not reproduced: " + result.stderr)
            if marker + "_end" in lines:
                raise AssertionError("MySQL continued after a failed migration")
            return result.stderr
        if result.returncode or lines[-1] != marker + "_end":
            raise AssertionError("Migration/query failed: " + result.stderr)
        return [tuple(line.split("\t")) for line in lines[1:-1]]

    def setUp(self):
        for table in TABLES:
            self.query(f"TRUNCATE TABLE `{table}`; "
                       f"INSERT INTO `{table}` SELECT * FROM `enchantment_seed`.`{table}`;")

    def apply_migrations(self):
        for path in MIGRATIONS:
            # The updater runs every file in a separate mysql process, without --force.
            self.query(path.read_text(encoding="utf-8"))

    def item_hashes(self, exclude=()):
        columns = ",".join(f"`{column}`" for column in self.columns if column not in exclude)
        return self.query(f"SELECT `entry`, SHA2(CAST(JSON_ARRAY({columns}) AS CHAR), 256) "
                          "FROM `item_template` ORDER BY `entry`;")

    def other_rows(self):
        return {table: self.query(f"SELECT * FROM `{table}` ORDER BY 1, 2;") for table in TABLES[1:]}

    def test_original_guard_reproduces_report_without_changing_items(self):
        if not self.before_ref:
            self.skipTest("Pass --before-ref to verify the original regression")
        original = subprocess.run(
            ["git", "show", f"{self.before_ref}:{MIGRATIONS[0].relative_to(ROOT).as_posix()}"],
            cwd=ROOT, text=True, encoding="utf-8", capture_output=True, check=True, **PROCESS_OPTIONS,
        ).stdout
        before = self.item_hashes()
        error = self.query(original, expected_error="ERROR 1062")
        self.assertIn("_local_enchantment_levels_v1_guard", error)
        self.assertEqual(before, self.item_hashes())

    def test_fresh_base_and_reapply_preserve_unrelated_fields(self):
        before = self.item_hashes(exclude=("RequiredLevel", "description"))
        other = self.other_rows()
        levels = dict(self.query("SELECT `entry`, `RequiredLevel` FROM `item_template` ORDER BY `entry`;"))
        listed = set(re.findall(r"^\((\d+),", MIGRATIONS[0].read_text(encoding="utf-8"), re.MULTILINE))
        self.assertEqual(947, len(listed))
        self.assertEqual(528, len(listed & levels.keys()))
        self.apply_migrations()
        expected = {entry: "0" if entry in listed else level for entry, level in levels.items()}
        self.assertEqual(expected, dict(self.query(
            "SELECT `entry`, `RequiredLevel` FROM `item_template` ORDER BY `entry`;")))
        self.assertEqual(before, self.item_hashes(exclude=("RequiredLevel", "description")))
        other["spell_script_names"] = [row for row in other["spell_script_names"] if row not in REMOVED_BINDINGS]
        self.assertEqual(other, self.other_rows())
        after = self.item_hashes()
        self.apply_migrations()
        self.assertEqual(after, self.item_hashes())
        self.assertEqual(other, self.other_rows())

    def test_missing_custom_items_mixed_levels_and_custom_requirements(self):
        self.query("""
            TRUNCATE TABLE `item_template`;
            INSERT INTO `item_template` (`entry`, `RequiredLevel`, `name`) VALUES
            (2304, 1, 'Original level'), (2313, 77, 'Custom level'), (2863, 0, 'Already applied'),
            (18262, 50, 'Stock level'), (3100563, 58, 'Local item'), (3100564, 0, 'Local already applied'),
            (2000000000, 60, 'Unrelated item');
        """)
        before = self.item_hashes(exclude=("RequiredLevel",))
        self.apply_migrations()
        self.assertEqual([
            ("2304", "0"), ("2313", "77"), ("2863", "0"), ("18262", "0"),
            ("3100563", "0"), ("3100564", "0"), ("2000000000", "60"),
        ], self.query("SELECT `entry`, `RequiredLevel` FROM `item_template` ORDER BY `entry`;"))
        self.assertEqual(before, self.item_hashes(exclude=("RequiredLevel",)))

    def test_custom_item_fields_and_enchantment_overrides_are_preserved(self):
        self.query("""
            UPDATE `item_template` SET `name` = 'Custom armor kit', `BuyPrice` = 123456,
            `RequiredSkill` = 333, `RequiredSkillRank` = 450, `requiredspell` = 12345,
            `ItemLevel` = 80, `description` = 'Keep this custom description' WHERE `entry` = 2304;
            INSERT INTO `spellitemenchantment_dbc` (`ID`, `MinLevel`, `RequiredSkillRank`) VALUES (999999, 60, 450);
        """)
        before = self.item_hashes(exclude=("RequiredLevel", "description"))
        overrides = self.other_rows()["spellitemenchantment_dbc"]
        self.apply_migrations()
        self.assertEqual([("0", "Keep this custom description")], self.query(
            "SELECT `RequiredLevel`, `description` FROM `item_template` WHERE `entry` = 2304;"))
        self.assertEqual(before, self.item_hashes(exclude=("RequiredLevel", "description")))
        self.assertEqual(overrides, self.other_rows()["spellitemenchantment_dbc"])

    def test_descriptions_require_exact_original_text_and_no_item_level_gate(self):
        self.query("""
            TRUNCATE TABLE `item_template`;
            INSERT INTO `item_template` (`entry`, `RequiredLevel`, `description`) VALUES
            (18262, 0, 'Requires Level 58.'), (20748, 0, 'Requires Level 58. Additional custom effect.'),
            (20749, 12, 'Requires Level 58.'), (38881, 0, 'requires level 58.'),
            (38882, 0, 'Requires Level 58. '), (38885, 0, ''),
            (967155, 0, 'Can''t be used after level 60.'),
            (2000000000, 0, 'Requires Level 58.');
        """)
        before = self.item_hashes(exclude=("description",))
        self.query(MIGRATIONS[2].read_text(encoding="utf-8"))
        self.assertEqual([
            ("18262", ""), ("20748", "Requires Level 58. Additional custom effect."),
            ("20749", "Requires Level 58."), ("38881", "requires level 58."),
            ("38882", "Requires Level 58. "), ("38885", ""), ("967155", ""),
            ("2000000000", "Requires Level 58."),
        ], self.query("SELECT `entry`, `description` FROM `item_template` ORDER BY `entry`;"))
        self.assertEqual(before, self.item_hashes(exclude=("description",)))

    def test_script_update_preserves_custom_bindings_and_lifecycle_links(self):
        self.query("""
            DELETE FROM `spell_script_names` WHERE `spell_id` IN (59917, 64441);
            INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
            (59917, 'custom_mount_helper'), (64441, 'custom_enchant_helper'),
            (-64441, 'spell_gen_proc_above_75'), (64441, 'SPELL_GEN_PROC_ABOVE_75');
            INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`)
            VALUES (13927, 12345, 0, 'Custom lifecycle link');
        """)
        before = self.other_rows()
        before["spell_script_names"] = [row for row in before["spell_script_names"] if row not in REMOVED_BINDINGS]
        self.apply_migrations()
        self.assertEqual(before, self.other_rows())
        self.apply_migrations()
        self.assertEqual(before, self.other_rows())

    def test_empty_tables_are_a_noop(self):
        for table in TABLES:
            self.query(f"TRUNCATE TABLE `{table}`;")
        self.apply_migrations()
        self.apply_migrations()
        for table in TABLES:
            self.assertEqual([("0",)], self.query(f"SELECT COUNT(*) FROM `{table}`;"))


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--mysql-bin", type=Path, required=True)
    parser.add_argument("--before-ref", help="Git revision containing the original failing migration")
    options, remaining = parser.parse_known_args()
    EnchantmentMigrations.mysql_bin = options.mysql_bin.resolve()
    EnchantmentMigrations.before_ref = options.before_ref
    unittest.main(argv=[__file__, *remaining], verbosity=2)
