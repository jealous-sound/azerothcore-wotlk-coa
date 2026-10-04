import argparse
import importlib.util
from pathlib import Path
import unittest


ROOT = Path(__file__).resolve().parents[3]
MODULE = ROOT / 'modules/mod-coa-legendary-items'
PENDING = ROOT / 'data/sql/updates/pending_db_world'
spec = importlib.util.spec_from_file_location(
    'isolated_mysql', ROOT / 'apps/test-framework/test_enchantment_migrations.py')
fixture = importlib.util.module_from_spec(spec)
spec.loader.exec_module(fixture)


class OptionalLegendaryModule(unittest.TestCase):
    mysql_bin = None
    query = staticmethod(fixture.EnchantmentMigrations.query)

    @classmethod
    def setUpClass(cls):
        fixture.EnchantmentMigrations.mysql_bin = cls.mysql_bin
        cls.addClassCleanup(fixture.EnchantmentMigrations.doClassCleanups)
        fixture.EnchantmentMigrations.setUpClass()
        for table in ('item_dbc', 'spell_dbc'):
            cls.query((ROOT / 'data/sql/base/db_world' / (table + '.sql')).read_text(encoding='utf-8'),
                      database='enchantment_seed')
            cls.query(f'CREATE TABLE `{table}` LIKE `enchantment_seed`.`{table}`;')

    def setUp(self):
        for table in ('item_template', 'item_dbc', 'spell_dbc', 'spell_script_names'):
            self.query(f'TRUNCATE TABLE `{table}`;')
        self.query("INSERT INTO item_template (entry, name) VALUES (9699999, 'unrelated');"
                   'INSERT INTO item_dbc (ID) VALUES (9699999);'
                   'INSERT INTO spell_dbc (ID) VALUES (9710064);'
                   "INSERT INTO spell_script_names (spell_id, ScriptName) VALUES (9710064, 'unrelated');")

    def core_updates(self):
        return sorted({*PENDING.glob('*coa_legendary*.sql'),
                       *PENDING.glob('rev_1791051693307538000.sql')})

    def apply(self, paths):
        for path in sorted(paths, key=lambda path: path.name):
            self.query(path.read_text(encoding='utf-8'))

    def legendary_counts(self):
        return self.query('SELECT COUNT(*) FROM item_template WHERE entry >= 9700000 AND entry < 9706400;'
                          'SELECT COUNT(*) FROM item_dbc WHERE ID >= 9700000 AND ID < 9706400;'
                          'SELECT COUNT(*) FROM spell_dbc WHERE ID >= 9710000 AND ID < 9710064;'
                          'SELECT COUNT(*) FROM spell_script_names WHERE spell_id >= 9710000 AND spell_id < 9710064;')

    def test_core_installs_no_legendary_data(self):
        self.apply(self.core_updates())
        self.assertEqual(self.legendary_counts(), [('0',)] * 4)

    def test_upgrade_cleans_legacy_core_data_and_preserves_unrelated_rows(self):
        self.query('INSERT INTO item_template (entry) VALUES (9700001), (9706380);'
                   'INSERT INTO item_dbc (ID) VALUES (9700001), (9706380);'
                   'INSERT INTO spell_dbc (ID) VALUES (9710000), (9710063);'
                   "INSERT INTO spell_script_names (spell_id, ScriptName) VALUES "
                   "(9710000, 'aura_coa_legendary_signature'), (9710063, 'aura_coa_legendary_signature');")
        self.assertEqual(self.legendary_counts(), [('2',)] * 4)
        self.apply(self.core_updates())
        self.assertEqual(self.legendary_counts(), [('2',), ('0',), ('0',), ('0',)])
        self.assertEqual(self.query('SELECT name FROM item_template WHERE entry = 9699999;'
                                    'SELECT ID FROM item_dbc WHERE ID = 9699999;'
                                    'SELECT ID FROM spell_dbc WHERE ID = 9710064;'
                                    'SELECT ScriptName FROM spell_script_names WHERE spell_id = 9710064;'),
                         [('unrelated',), ('9699999',), ('9710064',), ('unrelated',)])

    def test_module_reinstalls_after_cleanup_and_is_idempotent(self):
        paths = self.core_updates() + list((MODULE / 'data/sql/db-world').glob('*.sql'))
        self.apply(paths)
        self.assertEqual(self.legendary_counts(), [('5120',), ('3840',), ('64',), ('0',)])
        self.apply(paths)
        self.assertEqual(self.legendary_counts(), [('5120',), ('3840',), ('64',), ('0',)])


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--mysql-bin', required=True, type=Path)
    options, remaining = parser.parse_known_args()
    OptionalLegendaryModule.mysql_bin = options.mysql_bin.resolve()
    unittest.main(argv=[__file__, *remaining])
