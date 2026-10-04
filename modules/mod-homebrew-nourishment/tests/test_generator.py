#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later

from __future__ import annotations

import importlib.util
import sys
import tempfile
import unittest
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parents[1]
GENERATOR_PATH = PROJECT_ROOT / "tools" / "generate-nourishment-profiles.py"
SPEC = importlib.util.spec_from_file_location("nourishment_generator", GENERATOR_PATH)
assert SPEC and SPEC.loader
GENERATOR = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = GENERATOR
SPEC.loader.exec_module(GENERATOR)


class GeneratorTests(unittest.TestCase):
    def test_basic_food_profile_and_sql(self) -> None:
        source = GENERATOR.Spell(
            spell_id=433,
            name="Food",
            tooltip="Restores health per second.",
            effects=(6, 0, 0),
            auras=(84, 0, 0),
            misc=(0, 0, 0),
            base_points=(1, 0, 0),
            trigger_spells=(0, 0, 0),
        )
        marker_spells = {
            spell_id: GENERATOR.Spell(
                spell_id=spell_id,
                name="Marker",
                tooltip="",
                effects=(6, 0, 0),
                auras=(29, 29, 0),
                misc=(2, 4, 0),
                base_points=(1, 1, 0),
                trigger_spells=(0, 0, 0),
            )
            for spell_id in GENERATOR.FAMILY_TEMPLATES.values()
        }
        spells = {433: source, **marker_spells}
        item = GENERATOR.Item(
            entry=117,
            name="Tough Jerky",
            quality=1,
            flags=0,
            item_level=5,
            required_level=1,
            required_reputation=0,
            spell_id=433,
            spell_trigger=0,
            bonding=0,
            description="",
            start_quest=0,
            area=0,
            map_id=0,
            holiday_id=0,
        )

        profiles, excluded = GENERATOR.build_profiles([item], spells)
        self.assertEqual([], excluded)
        self.assertEqual(1, len(profiles))
        self.assertEqual("hearty", profiles[0].family)
        self.assertEqual((1, 1, 0), (profiles[0].amount_1, profiles[0].amount_2, profiles[0].amount_3))
        GENERATOR.validate(profiles, spells)

        with tempfile.TemporaryDirectory() as temporary:
            output = Path(temporary) / "profiles.sql"
            GENERATOR.write_sql(output, profiles, spells)
            sql = output.read_text(encoding="utf-8")
            self.assertIn("mod_homebrew_nourishment_profile", sql)
            self.assertIn("(117,'Tough Jerky'", sql)

    def test_unsafe_items_are_excluded(self) -> None:
        item = GENERATOR.Item(
            entry=999999,
            name="QA Test Food",
            quality=1,
            flags=0,
            item_level=1,
            required_level=1,
            required_reputation=0,
            spell_id=0,
            spell_trigger=0,
            bonding=0,
            description="",
            start_quest=0,
            area=0,
            map_id=0,
            holiday_id=0,
        )
        self.assertEqual("missing-source-spell", GENERATOR.exclusion_reason(item, None, {}))


if __name__ == "__main__":
    unittest.main()
