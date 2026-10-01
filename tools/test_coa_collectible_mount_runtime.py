#!/usr/bin/env python3
import unittest

import coa_collectible_mount_runtime as generator


class SelectionTests(unittest.TestCase):
    def select(self, **overrides):
        inputs = dict(
            existing_entries=set(),
            scripted_spells=set(),
            relevant_spells={100},
            misc={100: 10157259},
            names={100: "Armored Spectral Tiger"},
            creature_map={10157259: (470000, "Swift Armored Spectral Tiger")},
            display_model={470000: 805000},
            models={805000},
        )
        inputs.update(overrides)
        return generator.select_mount_runtime(**inputs)

    def test_selects_the_armored_spectral_tiger_mapping(self):
        self.assertEqual(self.select(), [{"entry": 10157259, "name": "Armored Spectral Tiger", "display": 470000}])

    def test_skips_entries_the_world_already_defines(self):
        self.assertEqual(self.select(existing_entries={10157259}), [])

    def test_skips_wrapper_scripted_mounts(self):
        self.assertEqual(self.select(scripted_spells={100}), [])

    def test_skips_placeholder_mounts(self):
        self.assertEqual(self.select(names={100: "[PH] mountcathbad"}), [])

    def test_skips_display_without_a_model(self):
        self.assertEqual(self.select(display_model={}, models=set()), [])

    def test_skips_display_whose_model_is_missing(self):
        self.assertEqual(self.select(display_model={470000: 805000}, models={1}), [])

    def test_skips_mount_without_a_creature_mapping(self):
        self.assertEqual(self.select(creature_map={}), [])

    def test_skips_zero_misc_spells(self):
        self.assertEqual(self.select(misc={100: 0}), [])

    def test_deduplicates_one_mount_used_by_several_spells(self):
        rows = self.select(relevant_spells={100, 101}, names={100: "A", 101: "B"})
        self.assertEqual(len(rows), 1)
        self.assertEqual(rows[0]["entry"], 10157259)


class ParsingTests(unittest.TestCase):
    def test_collection_mounts_keep_mount_category_rows(self):
        rows = [(1, 10, generator.MOUNT_CATEGORY, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 100),
                (2, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)]
        items, spells = generator.collection_mounts(rows)
        self.assertEqual(items, {10})
        self.assertEqual(spells, {100})

    def test_taught_spells_read_every_slot(self):
        row = [0] * 37
        row[generator.ITEM_FIELD] = 42
        row[2] = 111
        row[9] = 222
        row[16] = 333
        self.assertEqual(generator.taught_spells([row], {42}), {111, 222, 333})
        self.assertEqual(generator.taught_spells([row], {7}), set())


class RenderTests(unittest.TestCase):
    def test_render_upserts_the_template_and_replaces_the_model(self):
        sql = generator.render_sql([{"entry": 10157259, "name": "Armored Spectral Tiger", "display": 470000}])
        self.assertIn("ON DUPLICATE KEY UPDATE", sql)
        self.assertIn("DELETE FROM `creature_template_model` WHERE `CreatureID` IN (10157259);", sql)
        self.assertIn("(10157259, 'Armored Spectral Tiger', 1, 1, 35, 1, 12, 2000, 2000)", sql)
        self.assertIn("(10157259, 0, 470000, 1, 1)", sql)
        self.assertNotIn("DELETE FROM `creature_template`", sql)
        self.assertTrue(sql.endswith("\n"))

    def test_render_is_empty_without_rows(self):
        self.assertEqual(generator.render_sql([]), "")


if __name__ == "__main__":
    unittest.main()
