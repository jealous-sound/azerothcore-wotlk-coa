"""Checks for the effective spell report validator.

The fixture summary is written by hand from the documented count definitions, not produced by the validator.
"""

from contextlib import redirect_stderr, redirect_stdout
import copy
import io
import json
from pathlib import Path
import struct
import tempfile
import unittest

import spell_report


def slot(index, effect, aura=0, *, misc=0, trigger=0, effect_handler='handler', aura_handler='none',
         trigger_aura=False, raw=None):
    return {
        'slot': index, 'effect': effect, 'aura': aura, 'base_points': 0, 'die_sides': 0, 'misc': misc, 'misc_b': 0,
        'trigger': trigger, 'target_a': 1, 'target_b': 0, 'class_mask': [0, 0, 0], 'effect_handler': effect_handler,
        'aura_handler': aura_handler, 'trigger_aura': trigger_aura, 'raw': raw or {},
    }


def spell(spell_id, *, effective=True, depth=0, via=None, scope='grant', classes=(12,), roots=(100,),
          proc_flags=0, proc_chance=0, proc_entry=False, bonus_data=False, scripts=(), rank=None, linked=(),
          raw=None, slots=()):
    return {
        'id': spell_id, 'name': f'Spell {spell_id}', 'effective': effective, 'depth': depth, 'via': via,
        'root_scope': scope, 'classes': list(classes), 'roots': list(roots), 'proc_flags': proc_flags,
        'proc_chance': proc_chance, 'proc_entry': proc_entry, 'bonus_data': bonus_data, 'scripts': list(scripts),
        'rank': rank, 'linked': list(linked), 'raw': {} if raw is None else raw, 'slots': list(slots),
    }


def metrics(**values):
    result = {key: 0 for key in spell_report.METRIC_COUNTS}
    result.update({key: {} for key in spell_report.METRIC_COUNTERS})
    result.update(values)
    return result


def fixture():
    effects = ['handler'] * spell_report.TOTAL_SPELL_EFFECTS
    effects[0] = effects[168] = 'null'
    effects[13] = 'unused'
    auras = ['handler'] * spell_report.TOTAL_AURAS
    auras[0] = 'null'
    auras[42] = 'no_immediate'
    auras[354] = 'missing'
    spells = [
        spell(100, proc_chance=101, raw={'proc_flags': 20}, slots=[
            slot(0, 6, 42, trigger=101, aura_handler='no_immediate', trigger_aura=True),
            slot(1, 3, raw={'effect': 6, 'aura': 4}),
        ]),
        spell(101, depth=1, via={'from': 100, 'slot': 0, 'kind': 'trigger'}, bonus_data=True, scripts=['spell_x'],
              rank={'first': 101, 'prev': 0, 'next': 0, 'last': 101, 'rank': 1},
              linked=[{'kind': 'remove', 'spells': [-100], 'key_aliases': []}], slots=[
            slot(0, 168, effect_handler='null', raw={'trigger': 300}),
            slot(2, 6, 354, trigger=999, aura_handler='missing'),
        ]),
        spell(200, scope='skill_line', classes=(13,), roots=(200,), slots=[
            slot(0, 6, 112, misc=20003, aura_handler='handler'),
            slot(1, 6, 107, misc=40, aura_handler='handler'),
        ]),
        spell(300, effective=False, depth=1, via={'from': 101, 'slot': 0, 'kind': 'raw_trigger'}, slots=[
            slot(0, 3),
        ]),
    ]
    rewrites = {'proc_flags': 1, 'effect': 1, 'aura': 1, 'trigger': 1}
    masking = {'proc_flags_zeroed': 1, 'effect_to_dummy': 1, 'trigger_cleared': 1}
    grant_metrics = metrics(
        spells=2, slots=4, silent_slots=2, no_immediate_aura_slots=1, dummy_slots_without_bindings=1,
        proc_aura_spells_without_proc_entry=1, proc_aura_slots_without_proc_entry=1,
        trigger_aura_spells_without_proc_entry=1, rewritten_slots=2, rewritten_spells=2, spells_with_scripts=1,
        spells_with_bonus_data=1, effect_handlers={'handler': 3, 'null': 1},
        aura_handlers={'no_immediate': 1, 'missing': 1}, silent_by_primitive={'effect 168': 1, 'aura 354': 1},
        rewrites=rewrites, masking=masking)
    all_metrics = copy.deepcopy(grant_metrics)
    all_metrics.update(spells=3, slots=6, spellmod_op_out_of_range_slots=1, aura112_private_selector_slots=1,
                       effect_handlers={'handler': 5, 'null': 1},
                       aura_handlers={'no_immediate': 1, 'missing': 1, 'handler': 2})
    by_source = {source: 0 for source in spell_report.ROOT_SOURCES}
    by_source.update(class_spell=1, class_skill_line=1)
    return {
        'schema': spell_report.SCHEMA, 'server': 'test', 'generated_unix': 1, 'max_depth': 6, 'limits': [],
        'dispatch': {'effects': effects, 'auras': auras, 'trigger_auras': [4, 42]},
        'roots': [
            {'spell': 100, 'exists': True, 'grant': True, 'classes': [12], 'sources': ['class_spell']},
            {'spell': 200, 'exists': True, 'grant': False, 'classes': [13], 'sources': ['class_skill_line']},
        ],
        'summary': {
            'roots': 2, 'missing_roots': 0, 'roots_by_source': by_source, 'closure_spells': 4, 'raw_only_spells': 1,
            'missing_spells': 1, 'all': all_metrics, 'grant': grant_metrics,
            'per_class': {
                '12': {'spells': 2, 'silent_slots': 2, 'dummy_slots_without_bindings': 1,
                       'proc_aura_spells_without_proc_entry': 1, 'rewritten_spells': 2},
                '13': {'spells': 1, 'silent_slots': 0, 'dummy_slots_without_bindings': 0,
                       'proc_aura_spells_without_proc_entry': 0, 'rewritten_spells': 0},
            },
        },
        'missing_spells': [{'id': 999, 'from': 101, 'slot': 2, 'kind': 'trigger'}],
        'spells': spells,
    }


def dbc_record(spell_id, proc_flags=0, proc_chance=0, slots=()):
    fields = [0] * spell_report.DBC_FIELDS
    fields[0] = spell_id
    fields[spell_report.DBC_PROC_FLAGS] = proc_flags
    fields[spell_report.DBC_PROC_CHANCE] = proc_chance
    for index, values in slots:
        for name, (base, _) in spell_report.DBC_SLOT_OFFSETS.items():
            fields[base + index] = values.get(name, 1 if name == 'target_a' else 0)
    return struct.pack('<' + 'i' * spell_report.DBC_FIELDS, *fields)


def write_dbc(path, records):
    header = struct.pack('<4s4I', b'WDBC', len(records), spell_report.DBC_FIELDS, spell_report.DBC_FIELDS * 4, 1)
    path.write_bytes(header + b''.join(records) + b'\0')


def fixture_dbc_records():
    return [
        dbc_record(100, 20, 101, [(0, {'effect': 6, 'aura': 42, 'trigger': 101}), (1, {'effect': 6, 'aura': 4})]),
        dbc_record(101, 0, 0, [(0, {'effect': 168, 'trigger': 300}), (2, {'effect': 6, 'aura': 354,
                                                                         'trigger': 999})]),
        dbc_record(200, 0, 0, [(0, {'effect': 6, 'aura': 112, 'misc': 20003}),
                               (1, {'effect': 6, 'aura': 107, 'misc': 40})]),
    ]


def write_source(root, report):
    names = {'handler': 'EffectSomething', 'null': 'EffectNULL', 'unused': 'EffectUnused'}
    aura_names = {'handler': 'HandleSomething', 'null': 'HandleNULL', 'no_immediate': 'HandleNoImmediateEffect'}
    effect_rows = ''.join(f'    &Spell::{names[kind]}, // {index}\n'
                          for index, kind in enumerate(report['dispatch']['effects']))
    aura_rows = ''.join(('    nullptr, // ' if kind == 'missing' else f'    &AuraEffect::{aura_names[kind]}, // ')
                        + f'{index}\n' for index, kind in enumerate(report['dispatch']['auras']))
    spells = root / 'src/server/game/Spells'
    (spells / 'Auras').mkdir(parents=True)
    (spells / 'SpellEffects.cpp').write_text(
        'pEffect SpellEffects[TOTAL_SPELL_EFFECTS] =\n{\n' + effect_rows + '};\n', encoding='utf-8')
    (spells / 'Auras/SpellAuraEffects.cpp').write_text(
        'pAuraEffectHandler AuraEffectHandler[TOTAL_AURAS] =\n{\n' + aura_rows + '};\n', encoding='utf-8')
    (spells / 'Auras/SpellAuraDefines.h').write_text(
        'SPELL_AURA_DUMMY = 4,\nSPELL_AURA_PROC_TRIGGER_SPELL = 42,\n', encoding='utf-8')
    (spells / 'SpellMgr.cpp').write_text(
        '    isTriggerAura[SPELL_AURA_DUMMY] = true;\n'
        '    isTriggerAura[SPELL_AURA_PROC_TRIGGER_SPELL] = true;\n', encoding='utf-8')


class SpellReportTests(unittest.TestCase):
    def assert_invalid(self, report, text):
        with self.assertRaises(spell_report.ReportError) as raised:
            spell_report.validate(report)
        self.assertIn(text, str(raised.exception))

    def test_hand_written_fixture_is_valid(self):
        report = fixture()
        self.assertEqual(spell_report.validate(report), report['summary'])

    def test_wrong_summary_count_is_rejected(self):
        report = fixture()
        report['summary']['all']['dummy_slots_without_bindings'] = 0
        self.assert_invalid(report, 'summary.all')

    def test_handler_kind_must_match_dispatch(self):
        report = fixture()
        report['dispatch']['effects'][168] = 'handler'
        self.assert_invalid(report, 'effect_handler differs from dispatch')

    def test_raw_differences_must_differ(self):
        report = fixture()
        report['spells'][0]['slots'][1]['raw']['effect'] = 3
        self.assert_invalid(report, 'raw must list only differing fields')

    def test_via_must_name_the_parent_payload(self):
        report = fixture()
        report['spells'][1]['via']['slot'] = 2
        self.assert_invalid(report, 'has no slot 2')

    def test_effective_spell_cannot_come_from_raw_edge(self):
        report = fixture()
        report['spells'][3]['effective'] = True
        self.assert_invalid(report, 'effective edges')

    def test_skill_line_root_is_not_a_grant(self):
        report = fixture()
        report['roots'][1]['grant'] = True
        self.assert_invalid(report, 'skill-line-only roots are not grants')

    def test_expected_roots(self):
        report = fixture()
        spell_report.check_expected_roots(report, ['100:class_spell', '200'])
        with self.assertRaises(spell_report.ReportError):
            spell_report.check_expected_roots(report, ['200:class_spell'])

    def test_dbc_cross_check(self):
        report = fixture()
        records = fixture_dbc_records()
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'Spell.dbc'
            write_dbc(path, records)
            self.assertEqual(spell_report.check_dbc(report, path), 1)
            records[2] = dbc_record(200, 0, 0, [(0, {'effect': 6, 'aura': 112, 'misc': 20004}),
                                                (1, {'effect': 6, 'aura': 107, 'misc': 40})])
            write_dbc(path, records)
            with self.assertRaises(spell_report.ReportError) as raised:
                spell_report.check_dbc(report, path)
            self.assertIn('raw misc 20003 != Spell.dbc 20004', str(raised.exception))

    def test_source_cross_check(self):
        report = fixture()
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            write_source(root, report)
            spell_report.check_source(report, root)
            report['dispatch']['auras'][42] = 'handler'
            with self.assertRaises(spell_report.ReportError):
                spell_report.check_source(report, root)

    def test_same_report_ignores_generation_time(self):
        report = fixture()
        other = copy.deepcopy(report)
        other['generated_unix'] = 2
        spell_report.check_same(report, other)
        other['spells'][0]['name'] = 'Changed'
        with self.assertRaises(spell_report.ReportError):
            spell_report.check_same(report, other)

    def test_runner_entry_point_checks_every_layer(self):
        report = fixture()
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            run_dir = root / 'run'
            (run_dir / 'dbc').mkdir(parents=True)
            write_source(root, report)
            write_dbc(run_dir / 'dbc' / 'Spell.dbc', fixture_dbc_records())
            path = run_dir / 'report.json'
            path.write_text(json.dumps(report), encoding='utf-8')
            repeat = copy.deepcopy(report)
            repeat['generated_unix'] = report['generated_unix'] + 5
            (run_dir / 'repeat.json').write_text(json.dumps(repeat), encoding='utf-8')
            options = {'same_as': 'repeat.json', 'expect_roots': ['100:class_spell']}
            context = {'run_dir': run_dir, 'data_dir': run_dir, 'source_root': root}
            result = spell_report.check(path, options, context)
            self.assertEqual(result['checks'], ['schema', 'references', 'summary_recomputed', 'expect_roots',
                                                'source_tables', 'dbc', 'same_as'])
            self.assertEqual(result['summary'], report['summary'])
            self.assertEqual(result['dbc_absent_spells'], 1)
            (run_dir / 'dbc' / 'Spell.dbc').unlink()
            with self.assertRaisesRegex(spell_report.ReportError, 'Spell.dbc'):
                spell_report.check(path, options, context)

    def test_runner_entry_point_rejects_a_differing_repeat(self):
        report = fixture()
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            run_dir = root / 'run'
            (run_dir / 'dbc').mkdir(parents=True)
            write_source(root, report)
            write_dbc(run_dir / 'dbc' / 'Spell.dbc', fixture_dbc_records())
            path = run_dir / 'report.json'
            path.write_text(json.dumps(report), encoding='utf-8')
            differing = copy.deepcopy(report)
            differing['spells'][0]['name'] = 'Changed'
            (run_dir / 'repeat.json').write_text(json.dumps(differing), encoding='utf-8')
            context = {'run_dir': run_dir, 'data_dir': run_dir, 'source_root': root}
            with self.assertRaisesRegex(spell_report.ReportError, 'Reports differ'):
                spell_report.check(path, {'same_as': 'repeat.json'}, context)

    def test_command_line_reports_errors(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'report.json'
            path.write_text(json.dumps(fixture()), encoding='utf-8')
            output = io.StringIO()
            with redirect_stdout(output), redirect_stderr(output):
                self.assertEqual(spell_report.main([str(path), '--expect-root', '100:class_spell']), 0)
                broken = fixture()
                broken['summary']['roots'] = 3
                path.write_text(json.dumps(broken), encoding='utf-8')
                self.assertEqual(spell_report.main([str(path)]), 1)
            self.assertIn('summary.roots: reported 3, recomputed 2', output.getvalue())


if __name__ == '__main__':
    unittest.main()
