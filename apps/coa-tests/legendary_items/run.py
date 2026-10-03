import importlib.util
from pathlib import Path
import unittest


ROOT = Path(__file__).resolve().parents[3]
MODULE = ROOT / 'modules/mod-coa-legendary-items'
SPEC = importlib.util.spec_from_file_location('legendary_catalog', MODULE / 'tools/generate_catalog.py')
CATALOG = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(CATALOG)


class LegendaryCatalogTest(unittest.TestCase):
    def test_generated_artifacts_match_the_catalog(self):
        for path, content in CATALOG.outputs().items():
            with self.subTest(path=path):
                self.assertEqual(path.read_text(), content)

    def test_stats_and_descriptions_match_a_level_29_drop(self):
        catalog = CATALOG.load_catalog()
        self.assertEqual(CATALOG.item_stats(catalog[0], 41), [4, 14, 7, 18, 38, 41, 0, 0])
        self.assertEqual(CATALOG.item_stats(catalog[3], 41), [5, 14, 7, 18, 45, 22, 0, 0])
        self.assertEqual(CATALOG.item_stats(catalog[6], 41), [3, 14, 7, 18, 38, 41, 45, 14])
        self.assertEqual(CATALOG.power_text(catalog[0], 29),
            'Gain 82 melee and ranged attack power for 8 sec after killing a non-gray creature.')
        self.assertEqual(CATALOG.power_text(catalog[63], 29), 'Gain 20% armor while at or below 35% health.')

    def test_movement_is_exclusive_to_boots(self):
        catalog = CATALOG.load_catalog()
        movements = [item for item in catalog if item['power'] == 'Movement']
        self.assertEqual(len(movements), 21)
        self.assertTrue(all(item['inventory_type'] == 8 for item in movements))
        self.assertEqual(catalog[63]['power'], 'Armor')

    def test_native_stat_auras_cover_the_advertised_bonuses(self):
        catalog = CATALOG.load_catalog()
        self.assertEqual(CATALOG.aura_effects(catalog[0]), [(99, 0), (124, 0)])
        self.assertEqual(CATALOG.aura_effects(catalog[3]), [(13, 126), (135, 0)])
        self.assertEqual(CATALOG.aura_effects(catalog[6]), [(99, 0), (13, 126), (135, 0)])
        self.assertEqual(CATALOG.aura_effects(catalog[1]), [(31, 0)])
        self.assertEqual(CATALOG.aura_effects(catalog[63]), [(101, 1)])


if __name__ == '__main__':
    unittest.main()
