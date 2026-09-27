import json
from pathlib import Path
import re
import unittest

ROOT = Path(__file__).resolve().parents[3]
SOURCE = ROOT / 'src/server/coa'
DOCS = ROOT / 'docs/coa/patches'
EXPECTED = {0x570, 0x571, 0x574, 0x5F4, 0x686, 0x6C0, 0x6C1, 0x6CE, 0x6D2, 0x6F6,
            0x92A, 0x92C, 0x92D, 0x92E, 0x92F, 0x930, 0x931, 0x935, 0x949, 0x94A,
            0x94F, 0x950, 0x951, 0x952, 0x953, 0x954, 0x955, 0x956, 0x957, 0x958,
            0x959, 0x95A, 0x95F, 0x961}


class ModuleTests(unittest.TestCase):
    def test_all_requested_opcodes_have_encoders_and_examples(self):
        header = (SOURCE / 'AscensionPatchSpells.h').read_text()
        source = (SOURCE / 'AscensionPatchSpells.cpp').read_text() + (DOCS / 'patch_spell-examples.md').read_text()
        entries = dict(re.findall(r'    (\w+) = (0x[0-9A-F]+)', header))
        self.assertEqual({int(value, 16) for value in entries.values()}, EXPECTED)
        layouts = json.loads((DOCS / 'layouts.json').read_text())['layouts']
        self.assertEqual(len(layouts), 34)
        self.assertEqual({r['table'] for r in layouts}, set(entries))
        for row in layouts:
            with self.subTest(table=row['table']):
                self.assertEqual(row['opcode'], entries[row['table']])
                self.assertTrue(row['name'].startswith('SMSG_PATCH_SPELL'))
                self.assertTrue(int(row['handler_rva'], 16))
                self.assertIn('case Table::' + row['table'] + ': return Layout{', source)
                self.assertIn('Send(player, Table::' + row['table'] + ',', source)

    def test_item_layouts_and_unverified_stat(self):
        expected = {0x569, 0x56A, 0x59F, 0x693, 0x6EC, 0x932, 0x944, 0x964,
                    0x965, 0x966, 0x967, 0x968, 0x969, 0x96A, 0x96B, 0x96C}
        header = (SOURCE / 'AscensionPatchItems.h').read_text()
        source = (SOURCE / 'AscensionPatchItems.cpp').read_text() + (DOCS / 'patch_item-examples.md').read_text()
        entries = dict(re.findall(r'    (\w+) = (0x[0-9A-F]+)', header))
        self.assertEqual({int(value, 16) for value in entries.values()}, expected)
        rows = json.loads((DOCS / 'item_layouts.json').read_text())['layouts']
        self.assertEqual(len(rows), 16)
        self.assertEqual({row['table'] for row in rows}, set(entries))
        for row in rows:
            self.assertEqual(entries[row['table']], row['opcode'])
            self.assertIn(row['name'], source)
            if row['table'] == 'Stat':
                self.assertEqual(row['status'], 'unverified')
                self.assertIsNone(row['handler_rva'])
                self.assertIsNone(row['prefix_words'])
                self.assertIn('case Table::Stat: return std::nullopt;', source)
            else:
                self.assertEqual(row['status'], 'verified')
                self.assertTrue(int(row['handler_rva'], 16))
                self.assertEqual(row['string_encoding'], 'length32')
                self.assertIn('case Table::' + row['table'] + ': return Layout{'
                              + str(row['prefix_words']) + ', ' + str(row['strings']) + '};', source)
                self.assertIn('Send(player, Table::' + row['table'] + ',', source)
                self.assertEqual(len(row['pointer_words']), row['strings'])
                self.assertTrue(all(0 < slot < row['prefix_words'] for slot in row['pointer_words']))

    def test_achievement_layouts_and_examples(self):
        header = (SOURCE / 'AscensionPatchAchievements.h').read_text()
        source = (SOURCE / 'AscensionPatchAchievements.cpp').read_text() + (DOCS / 'patch_achievement-examples.md').read_text()
        entries = dict(re.findall(r'    (\w+) = (0x[0-9A-F]+)', header))
        self.assertEqual(entries, {'Achievement': '0x0936', 'Criteria': '0x0937', 'Category': '0x0938'})
        rows = json.loads((DOCS / 'achievement_layouts.json').read_text())['layouts']
        self.assertEqual(len(rows), 3)
        self.assertEqual({row['table'] for row in rows}, set(entries))
        expected = {'Achievement': (14, 3, 0), 'Criteria': (9, 16, 5), 'Category': (2, 16, 2)}
        for row in rows:
            self.assertEqual(row['opcode'], entries[row['table']])
            counts = tuple(row[key] for key in ('prefix_words', 'strings', 'suffix_words'))
            self.assertEqual(counts, expected[row['table']])
            self.assertIn('case Table::' + row['table'] + ': return Layout{'
                          + ', '.join(map(str, counts)) + ', false};', source)
            self.assertTrue(int(row['handler_rva'], 16))
            self.assertIn(row['name'], source)
            self.assertIn('Send(player, Table::' + row['table'] + ',', source)

    def test_core_owns_sql_config_and_registration(self):
        migration = 'rev_1790407756703836600.sql'
        self.assertTrue((ROOT / 'data/sql/updates/pending_db_world' / migration).is_file())
        self.assertFalse((ROOT / 'modules/mod-ascension-patch/src').exists())
        for name in ('WorldDatabase.h', 'WorldDatabase.cpp'):
            database = ROOT / 'src/server/database/Database/Implementation' / name
            self.assertIn('WORLD_SEL_PATCH_SPELL', database.read_text())
        self.assertIn('WORLD_SEL_PATCH_SPELL', (SOURCE / 'AscensionPatchTooltips.cpp').read_text())
        loader = (SOURCE / 'CoAScriptLoader.cpp').read_text()
        self.assertEqual(loader.count('    AddAscensionPatchTooltipScripts();'), 1)
        self.assertIn('AscensionPatch.Tooltips = 0', (SOURCE / 'conf/coa.conf.dist').read_text())

    def test_manifest_captures_nonuniform_wire_formats(self):
        rows = {r['table']: r for r in json.loads((DOCS / 'layouts.json').read_text())['layouts']}
        self.assertEqual(rows['TagTypes']['strings'], 6)
        self.assertEqual(rows['TagTypes']['string_encoding'], 'nul')
        self.assertEqual(rows['ActivationOverlays']['string_encoding'], 'nul')
        for table, counts in [('Spell', (170, 4, 0)), ('ItemEnchantment', (14, 16, 8)),
                              ('ShapeshiftForm', (18, 1, 0)), ('VisualEffectName', (1, 2, 4)),
                              ('MissileMotion', (1, 2, 2))]:
            self.assertEqual(tuple(rows[table][key] for key in ('prefix_words', 'strings', 'suffix_words')), counts)


if __name__ == '__main__':
    unittest.main()
