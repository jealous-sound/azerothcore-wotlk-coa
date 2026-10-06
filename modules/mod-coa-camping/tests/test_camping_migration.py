import argparse
import importlib.util
from pathlib import Path
import unittest


ROOT = Path(__file__).resolve().parents[3]
MIGRATION = ROOT / 'data/sql/updates/pending_db_world/rev_20261005_01_coa_camping.sql'
FURNITURE = ROOT / 'data/sql/updates/pending_db_world/rev_20261006_01_coa_camping_furniture.sql'
LAYOUT = ROOT / 'data/sql/updates/pending_db_world/rev_20261006_02_coa_camping_layout.sql'
spec = importlib.util.spec_from_file_location(
    'camping_mysql_fixture', ROOT / 'apps/test-framework/test_enchantment_migrations.py')
fixture = importlib.util.module_from_spec(spec)
spec.loader.exec_module(fixture)


class CampingMigration(unittest.TestCase):
    mysql_bin = None
    query = staticmethod(fixture.EnchantmentMigrations.query)

    @classmethod
    def setUpClass(cls):
        fixture.EnchantmentMigrations.mysql_bin = cls.mysql_bin
        cls.addClassCleanup(fixture.EnchantmentMigrations.doClassCleanups)
        fixture.EnchantmentMigrations.setUpClass()
        for table in ('gameobject_template', 'creature_template', 'creature_template_model', 'npc_vendor'):
            schema = (ROOT / f'data/sql/base/db_world/{table}.sql').read_text(encoding='utf-8')
            cls.query(schema.split('-- Dumping data for table', 1)[0])

    def setUp(self):
        self.query('TRUNCATE TABLE `gameobject_template`;')
        for table in ('creature_template', 'creature_template_model', 'npc_vendor'):
            self.query(f'TRUNCATE TABLE `{table}`;')
        self.query("INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`) "
                   "VALUES (29784, 8, 192, 'Basic Campfire');")

    def apply(self):
        self.query(MIGRATION.read_text(encoding='utf-8'))

    def rows(self):
        return self.query('SELECT `entry`, `type`, `displayId`, `name`, `size` '
                          'FROM `gameobject_template` ORDER BY `entry`;')

    def test_installation_and_repeat_preserve_native_fire(self):
        self.apply()
        expected = [('29784', '8', '192', 'Basic Campfire', '1'),
                    ('9500200', '10', '345', 'Campsite Supplies', '1'),
                    ('9500201', '5', '100', 'Incense Candle', '1')]
        self.assertEqual(self.rows(), expected)
        self.apply()
        self.assertEqual(self.rows(), expected)

    def test_colliding_entries_are_never_overwritten(self):
        self.query("INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`) "
                   "VALUES (9500200, 3, 42, 'Other module', 2), (9500201, 5, 999, 'Incense Candle', 4);")
        before = self.rows()
        self.apply()
        self.assertEqual(self.rows(), before)
        self.apply()
        self.assertEqual(self.rows(), before)

    def furniture_rows(self):
        return {
            'objects': self.rows(),
            'bots': self.query('SELECT `entry`, `name`, `faction`, `npcflag` '
                               'FROM `creature_template` ORDER BY `entry`;'),
            'models': self.query('SELECT * FROM `creature_template_model` ORDER BY `CreatureID`, `Idx`;'),
            'vendors': self.query('SELECT * FROM `npc_vendor` ORDER BY `entry`, `slot`, `item`;'),
        }

    def test_furniture_repeat_and_native_service_flags(self):
        self.query(FURNITURE.read_text(encoding='utf-8'))
        before = self.furniture_rows()
        self.assertEqual(before['bots'], [('9500220', 'Camp Reagent Bot', '190', '128'),
                                         ('9500221', 'Camp Repair Bot', '190', '4224')])
        self.assertEqual(len(before['models']), 2)
        self.assertEqual(len(before['vendors']), 18)
        self.assertEqual(len(before['objects']), 5)
        self.assertEqual(self.query('SELECT `Data0`, `Data1` FROM `gameobject_template` WHERE `entry` = 9500203;'),
                         [('1', '1')])
        self.query(FURNITURE.read_text(encoding='utf-8'))
        self.assertEqual(self.furniture_rows(), before)

    def test_furniture_collision_keeps_foreign_models_and_stock(self):
        self.query("INSERT INTO `gameobject_template` (`entry`, `name`, `displayId`) VALUES "
                   "(9500202, 'Other tent', 42), (9500203, 'Other chair', 42), "
                   "(9500204, 'Other banner', 42), (9500205, 'Other banner', 42);")
        self.query("INSERT INTO `creature_template` (`entry`, `name`) VALUES "
                   "(9500220, 'Other vendor'), (9500221, 'Other repairer');")
        self.query('INSERT INTO `creature_template_model` (`CreatureID`, `CreatureDisplayID`) '
                   'VALUES (9500220, 42), (9500221, 42);')
        self.query('INSERT INTO `npc_vendor` (`entry`, `item`) VALUES (9500220, 42), (9500221, 42);')
        before = self.furniture_rows()
        self.query(FURNITURE.read_text(encoding='utf-8'))
        self.assertEqual(self.furniture_rows(), before)
        self.query(FURNITURE.read_text(encoding='utf-8'))
        self.assertEqual(self.furniture_rows(), before)

    def test_layout_scales_owned_templates_and_repeats_without_other_changes(self):
        self.query(FURNITURE.read_text(encoding='utf-8'))
        self.query(LAYOUT.read_text(encoding='utf-8'))
        self.assertEqual(self.query('SELECT `entry`, `size` FROM `gameobject_template` '
                                    'WHERE `entry` IN (9500202, 9500204, 9500205) ORDER BY `entry`;'),
                         [('9500202', '1'), ('9500204', '0.35'), ('9500205', '0.35')])
        self.assertEqual(self.query('SELECT `type`, `displayId`, `name`, `size` '
                                    'FROM `gameobject_template` WHERE `entry` = 29784;'),
                         [('8', '192', 'Basic Campfire', '1')])
        before = self.furniture_rows()
        self.query(LAYOUT.read_text(encoding='utf-8'))
        self.assertEqual(self.furniture_rows(), before)

    def test_layout_preserves_foreign_templates_and_custom_scales(self):
        self.query(FURNITURE.read_text(encoding='utf-8'))
        self.query('UPDATE `gameobject_template` SET `size` = 0.75 WHERE `entry` = 9500202;')
        self.query("UPDATE `gameobject_template` SET `name` = 'Other banner' WHERE `entry` = 9500204;")
        self.query('UPDATE `gameobject_template` SET `displayId` = 999 WHERE `entry` = 9500205;')
        before = self.furniture_rows()
        self.query(LAYOUT.read_text(encoding='utf-8'))
        self.assertEqual(self.furniture_rows(), before)
        self.query(LAYOUT.read_text(encoding='utf-8'))
        self.assertEqual(self.furniture_rows(), before)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--mysql-bin', required=True, type=Path)
    options, remaining = parser.parse_known_args()
    CampingMigration.mysql_bin = options.mysql_bin.resolve()
    unittest.main(argv=[__file__, *remaining])
