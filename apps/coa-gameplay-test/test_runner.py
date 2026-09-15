"""Behavioral checks for scenario validation, process failures and database ownership."""

import copy
import json
from pathlib import Path
import sys
import tempfile
from types import SimpleNamespace
import unittest
from unittest.mock import patch

import run


class RunnerTests(unittest.TestCase):
    def setUp(self):
        self.scenario = run.read_json(Path(__file__).parent / 'scenarios' / 'frostbolt.json')

    def report(self):
        records = []
        for index, step in enumerate(self.scenario['steps']):
            record = {'index': str(index), 'action': step['action'], 'status': 'completed'}
            if step['action'] == 'assert':
                record.update(status='passed', actual=str(step.get('equals', step.get('max', step.get('min')))))
            records.append(record)
        return {
            'schema': '1', 'run_id': '012345abcdef', 'scenario': self.scenario['name'],
            'execution': 'socketless-session-handlers', 'status': 'passed',
            'assertions': str(sum(step['action'] == 'assert' for step in self.scenario['steps'])),
            'completed_steps': str(len(records)), 'steps': records,
        }

    def test_example_is_valid(self):
        for path in (Path(__file__).parent / 'scenarios').glob('*.json'):
            scenario = run.read_json(path)
            with self.subTest(path=path):
                self.assertIs(run.validate(scenario), scenario)

    def test_malformed_scenarios_fail_before_starting_processes(self):
        for change in (
            lambda s: s.update(schema=True),
            lambda s: s.update(timeout_ms=float('inf')),
            lambda s: s['players'][0].update(level=True),
            lambda s: s['players'][0].update(race=0),
            lambda s: s['players'][0].update(ranged_hit_rating=-1),
            lambda s: s['players'][0].update(melee_hit_rating=-1),
            lambda s: s['players'][0].update(expertise_rating=True),
            lambda s: s['creatures'][0].update(id='caster'),
            lambda s: s['steps'].append({'action': 'cast', 'actor': 'caster', 'spell': 116, 'target': 'missing'}),
            lambda s: s.update(location={'map': 33, 'x': 0, 'y': 0, 'z': 0, 'ignore_access': 1}),
            lambda s: s['steps'].append({'action': 'cast', 'actor': 'caster', 'spell': 502329,
                                         'destination': {'x': 0, 'y': 0}}),
            lambda s: s['steps'].append({'action': 'cast', 'actor': 'caster', 'spell': 502329,
                                         'destination': {'x': float('nan'), 'y': 0, 'z': 0}}),
            lambda s: s['steps'].append({'action': 'assert', 'actor': 'target', 'metric': 'dynamic_object',
                                         'spell': 502329, 'equals': 1}),
            lambda s: s['steps'].append({'action': 'assert', 'actor': 'caster', 'metric': 'dynamic_object',
                                         'equals': 1}),
            lambda s: s['steps'].append({'action': 'cast_charm', 'actor': 'target', 'spell': 802176}),
            lambda s: s['steps'].append({'action': 'gossip_hello', 'actor': 'caster', 'target': 'missing'}),
            lambda s: s['steps'].append({'action': 'gossip_select', 'actor': 'caster', 'option': -1}),
            lambda s: s['steps'].append({'action': 'assert', 'actor': 'target', 'metric': 'gossip_options',
                                         'equals': 1}),
            lambda s: s['steps'].append({'action': 'assert', 'actor': 'target', 'metric': 'health', 'equlas': 0}),
            lambda s: s['steps'].append({'action': 'assert', 'actor': 'target', 'metric': 'health',
                                         'equals': float('nan')}),
            lambda s: s['steps'].append({'action': 'set_power', 'actor': 'caster', 'value': 1, 'power': 7}),
            lambda s: s['steps'].append({'action': 'assert', 'actor': 'target', 'metric': 'pet_entry', 'equals': 0}),
            lambda s: s['steps'].append({'action': 'command', 'actor': 'target', 'command': '.manastorm enter 1'}),
            lambda s: s['steps'].append({'action': 'command', 'actor': 'caster', 'command': 'manastorm enter 1'}),
            lambda s: s['steps'].append({'action': 'command', 'actor': 'caster', 'command': '.a\n.b'}),
            lambda s: s['steps'].append({'action': 'assert', 'actor': 'target', 'metric': 'charm_entry', 'equals': 0}),
            lambda s: s['steps'].append({'action': 'assert', 'actor': 'caster', 'metric': 'charm_aura_stacks',
                                         'equals': 0}),
            lambda s: s['steps'].append({'action': 'assert', 'actor': 'caster', 'metric': 'pet_aura_stacks',
                                         'equals': 0}),
            lambda s: s['steps'].append({'action': 'assert', 'actor': 'caster', 'metric': 'owned_creature_count',
                                         'equals': 0}),
            lambda s: s['steps'].append({'action': 'assert', 'actor': 'target', 'metric': 'owned_creature_count',
                                         'entry': 36, 'equals': 0}),
            lambda s: s['steps'].append({'action': 'assert', 'actor': 'caster', 'metric': 'owned_creature_count',
                                         'entry': 36, 'caster': 'caster', 'equals': 0}),
            lambda s: s.update(steps=[{'action': 'wait', 'ms': 1}]),
            lambda s: s['steps'].insert(0, {'action': 'assert', 'actor': 'target', 'metric': 'health',
                                           'relative_to': 'missing', 'equals': 0}),
        ):
            scenario = copy.deepcopy(self.scenario)
            change(scenario)
            with self.subTest(scenario=scenario), self.assertRaises(ValueError):
                run.validate(scenario)

    def test_duplicate_json_keys_are_rejected(self):
        with self.assertRaisesRegex(ValueError, 'Duplicate'):
            json.loads('{"schema": 1, "schema": 2}', object_pairs_hook=run.unique_object)

    def test_result_is_rechecked_against_expected_values(self):
        report = self.report()
        assertion = next(i for i, step in enumerate(self.scenario['steps']) if step['action'] == 'assert')
        run.check_report(report, report['run_id'], self.scenario, 0)
        mutations = [
            lambda r: r.update(schema='2'),
            lambda r: r.update(run_id='different'),
            lambda r: r.update(assertions='0'),
            lambda r: r.update(completed_steps='1'),
            lambda r: r['steps'].pop(),
            lambda r: r['steps'][assertion].update(actual='1'),
            lambda r: r['steps'][assertion].update(actual='nan'),
            lambda r: r['steps'][assertion].update(status='failed'),
            lambda r: r['steps'][assertion].update(index='99'),
            lambda r: r['steps'][0].update(status='failed'),
        ]
        for mutate in mutations:
            candidate = copy.deepcopy(report)
            mutate(candidate)
            with self.subTest(report=candidate), self.assertRaises(ValueError):
                run.check_report(candidate, report['run_id'], self.scenario, 0)
        with self.assertRaises(ValueError):
            run.check_report(report, report['run_id'], self.scenario, 1)

    def test_only_local_valid_database_names_are_accepted(self):
        connection = run.Connection.parse('127.0.0.1;3306;user;secret;acore_world')
        self.assertEqual(connection.database, 'acore_world')
        for text in ('external.example;3306;u;p;world', 'localhost;3306;u;p;world`; DROP DATABASE auth;',
                     'localhost;0;u;p;world', 'localhost;3306;u;p;world-name'):
            with self.subTest(text=text), self.assertRaises(ValueError):
                run.Connection.parse(text)

    def database(self, directory):
        connections = {role: run.Connection('127.0.0.1', 3306, 'user', 'secret', f'source_{role}')
                       for role in ('auth', 'characters', 'world')}
        return run.Databases('mysql', 'mysqldump', directory, connections, '012345abcdef')

    def test_admin_credentials_preserve_source_endpoint_and_schema(self):
        with tempfile.TemporaryDirectory() as temporary:
            path = Path(temporary) / 'admin.ini'
            original = '[client]\nhost=127.0.0.1\nport=3306\nuser=root\npassword="literal%secret\\svalue"\n'
            path.write_text(original)
            sources = {'world': run.Connection('127.0.0.1', 3306, 'user', 'old', 'acore_world')}
            result = run.database_credentials(sources, path)['world']
            self.assertEqual(result.database, 'acore_world')
            self.assertEqual(result.host, '127.0.0.1')
            self.assertEqual(result.user, 'root')
            self.assertEqual(result.password, 'literal%secret value')
            self.assertEqual(path.read_text(), original)
            self.assertEqual(sources['world'].user, 'user')
            path.write_text(original.replace('3306', '3307'))
            with self.assertRaisesRegex(ValueError, 'endpoint must match'):
                run.database_credentials(sources, path)
            path.write_text('[client]\npassword=secret\nnot-valid secret\n')
            with self.assertRaisesRegex(ValueError, '^Invalid database client config$'):
                run.database_credentials(sources, path)

    def test_partial_clone_only_drops_databases_created_by_this_run(self):
        with tempfile.TemporaryDirectory() as temporary:
            database = self.database(Path(temporary))
            statements = []
            with patch.object(database, 'sql', side_effect=lambda role, sql: statements.append(sql)), \
                    patch.object(database, 'copy', side_effect=ValueError('copy failed')):
                with self.assertRaisesRegex(ValueError, 'copy failed'):
                    database.prepare()
                self.assertEqual(database.cleanup(), [])
            self.assertEqual(database.created, ['auth'])
            self.assertEqual(len(statements), 2)
            self.assertEqual(statements[-1], 'DROP DATABASE `coa_test_012345abcdef_auth`;')
            self.assertNotIn('source_', '\n'.join(statements))
            database.remove_credentials()
            self.assertFalse(any(Path(temporary).glob('*.cnf')))

    def test_existing_database_is_never_adopted_or_dropped(self):
        with tempfile.TemporaryDirectory() as temporary:
            database = self.database(Path(temporary))
            with patch.object(database, 'sql', side_effect=ValueError('exists')) as execute:
                with self.assertRaises(ValueError):
                    database.prepare()
                self.assertEqual(database.cleanup(), [])
                self.assertEqual(execute.call_count, 1)
            self.assertEqual(database.created, [])

    def test_database_diagnostics_do_not_expose_credentials(self):
        with tempfile.TemporaryDirectory() as temporary:
            database = self.database(Path(temporary))
            result = SimpleNamespace(returncode=1, stderr='ERROR 1045 password=secret', stdout='')
            with patch.object(run.subprocess, 'run', return_value=result), self.assertRaises(ValueError) as raised:
                database.sql('auth', 'SELECT 1;')
            self.assertIn('1045', str(raised.exception))
            self.assertNotIn('secret', str(raised.exception))
            self.assertNotIn('secret', repr(database.connections['auth']))

    def test_configuration_override_removes_old_values_without_editing_source(self):
        with tempfile.TemporaryDirectory() as temporary:
            source = Path(temporary) / 'source.conf'
            output = Path(temporary) / 'test.conf'
            original = '[worldserver]\nBindIP = "0.0.0.0"\nBindIP = "::"\nDataDir = "C:/data"\n'
            source.write_text(original)
            run.write_config(source, output, {'BindIP': '127.0.0.1'})
            self.assertEqual(source.read_text(), original)
            self.assertEqual(output.read_text().count('BindIP'), 1)
            self.assertEqual(run.read_config(output)['BindIP'], '127.0.0.1')

    def test_module_settings_are_copied_and_cannot_override_isolation(self):
        with tempfile.TemporaryDirectory() as temporary:
            directory = Path(temporary)
            source = directory / 'modules'
            source.mkdir()
            config = source / 'module.conf'
            config.write_text('AscensionCompat.Enable = 1\n')
            staged = run.stage_modules(source, directory / 'run', {'BindIP'})
            self.assertEqual(staged[0].read_bytes(), config.read_bytes())
            config.write_text('BindIP = "0.0.0.0"\n')
            with self.assertRaisesRegex(ValueError, 'overrides harness'):
                run.stage_modules(source, directory / 'other-run', {'BindIP'})
            self.assertFalse((directory / 'other-run').exists())

    def fake_process(self, code, startup_timeout=3):
        with tempfile.TemporaryDirectory() as temporary:
            directory = Path(temporary)
            script = directory / 'fake_server.py'
            script.write_text('from pathlib import Path\nimport json, sys, time\n' + code, encoding='utf-8')
            return run.run_process([sys.executable, str(script)], directory, directory / 'ready.json',
                                   directory / 'result.json', '012345abcdef', startup_timeout, 3)

    def test_zero_exit_without_result_is_a_failure(self):
        with self.assertRaisesRegex(ValueError, 'without a result'):
            self.fake_process('sys.exit(0)\n')

    def test_readiness_timeout_stops_owned_child(self):
        with self.assertRaisesRegex(ValueError, 'readiness timed out'):
            self.fake_process('sys.stdin.readline()\n', startup_timeout=0.15)

    def test_wrong_run_readiness_is_rejected(self):
        with self.assertRaisesRegex(ValueError, 'Invalid readiness'):
            self.fake_process('Path("ready.json").write_text(json.dumps({"status":"ready","run_id":"other"}))\n'
                              'sys.stdin.readline()\n')

    def test_matching_completed_process_returns_its_report(self):
        report = self.report()
        result, returncode = self.fake_process(
            'Path("ready.json").write_text(json.dumps({"status":"ready","run_id":"012345abcdef"}))\n'
            f'Path("result.json").write_text({json.dumps(json.dumps(report))})\n')
        run.check_report(result, '012345abcdef', self.scenario, returncode)

    def test_success_without_readiness_is_rejected(self):
        with self.assertRaisesRegex(ValueError, 'missing harness readiness'):
            self.fake_process(f'Path("result.json").write_text({json.dumps(json.dumps(self.report()))})\n')


if __name__ == '__main__':
    unittest.main()
