#!/usr/bin/env python3
"""Run gameplay scenarios in a dedicated worldserver with disposable local databases."""

import argparse
import configparser
from dataclasses import dataclass, field
import hashlib
import json
import math
import os
from pathlib import Path
import re
import secrets
import socket
import subprocess
import sys
import tempfile
import time

ROOT = Path(__file__).resolve().parents[2]
CREATE_FLAGS = subprocess.CREATE_NO_WINDOW if os.name == 'nt' else 0
IDENTIFIER = re.compile(r'[A-Za-z_][A-Za-z0-9_]*\Z')
ACTOR_ID = re.compile(r'[a-z][a-z0-9_]{0,31}\Z')
LOCAL_HOSTS = {'127.0.0.1', 'localhost', '::1'}
METRICS = {
    'health', 'max_health', 'power', 'max_power', 'alive', 'combat', 'casting', 'level',
    'aura', 'aura_stacks', 'aura_charges', 'aura_duration_ms', 'aura_amount',
    'knows_spell', 'has_talent', 'talent_points', 'cooldown_ms', 'item_count', 'bank_bag_slots',
    'pet_entry', 'pet_aura_stacks', 'owned_creature_count',
}
METRIC_FIELDS = {'actor', 'metric', 'spell', 'power', 'caster', 'effect', 'item', 'entry', 'relative_to'}
ACTIONS = {
    'console': ({'command'}, {'command'}),
    'wait': ({'ms'}, {'ms'}),
    'snapshot': ({'actor', 'metric', 'save_as'}, METRIC_FIELDS | {'save_as'}),
    'assert': ({'actor', 'metric'}, METRIC_FIELDS | {'equals', 'min', 'max', 'within_ms'}),
    'learn': ({'actor', 'spell'}, {'actor', 'spell'}),
    'unlearn': ({'actor', 'spell'}, {'actor', 'spell'}),
    'cast': ({'actor', 'spell'}, {'actor', 'spell', 'target'}),
    'talent': ({'actor', 'talent', 'rank'}, {'actor', 'talent', 'rank'}),
    'reset_talents': ({'actor'}, {'actor'}),
    'add_item': ({'actor', 'item'}, {'actor', 'item', 'count'}),
    'equip': ({'actor', 'item', 'slot'}, {'actor', 'item', 'slot'}),
    'use_item': ({'actor', 'item', 'spell'}, {'actor', 'item', 'spell', 'target'}),
    'set_health': ({'actor', 'value'}, {'actor', 'value'}),
    'set_power': ({'actor', 'value'}, {'actor', 'value', 'power'}),
}


def require(condition, message):
    if not condition:
        raise ValueError(message)


def keys(value, required, allowed, where):
    require(isinstance(value, dict), f'{where}: expected an object')
    require(required <= value.keys(), f'{where}: missing {sorted(required - value.keys())}')
    require(value.keys() <= allowed, f'{where}: unknown fields {sorted(value.keys() - allowed)}')


def number(value, where, minimum=None, maximum=None, integer=False):
    require(type(value) in (int, float) and math.isfinite(value), f'{where}: expected a finite number')
    require(not integer or type(value) is int, f'{where}: expected an integer')
    require(minimum is None or value >= minimum, f'{where}: value is too small')
    require(maximum is None or value <= maximum, f'{where}: value is too large')


def unique_object(pairs):
    result = {}
    for key, value in pairs:
        require(key not in result, f'Duplicate JSON key: {key}')
        result[key] = value
    return result


def read_json(path):
    return json.loads(Path(path).read_text(encoding='utf-8-sig'), object_pairs_hook=unique_object)


def validate(scenario):
    keys(scenario, {'schema', 'name', 'players', 'steps'},
         {'schema', 'name', 'players', 'creatures', 'steps', 'timeout_ms', 'location', 'contract'}, 'scenario')
    require(type(scenario['schema']) is int and scenario['schema'] == 1, 'Unsupported scenario schema')
    require(isinstance(scenario['name'], str) and scenario['name'].strip(), 'Scenario needs a name')
    number(scenario.get('timeout_ms', 90000), 'timeout_ms', 1, 600000, True)
    players = scenario['players']
    creatures = scenario.get('creatures', [])
    require(isinstance(players, list) and 1 <= len(players) <= 8, 'Expected 1..8 players')
    require(isinstance(creatures, list) and len(creatures) <= 8, 'Expected at most eight creatures')
    player_ids = set()
    actor_ids = set()
    for player in players:
        keys(player, {'id', 'race', 'class'},
             {'id', 'race', 'class', 'level', 'spell_hit_rating', 'ranged_hit_rating'}, 'player')
        identity = player['id']
        require(isinstance(identity, str) and ACTOR_ID.fullmatch(identity), 'Invalid player id')
        require(identity not in actor_ids, 'Duplicate actor id')
        actor_ids.add(identity)
        player_ids.add(identity)
        for key in ('race', 'class'):
            number(player[key], key, 1, 255, True)
        number(player.get('level', 80), 'level', 1, 255, True)
        number(player.get('spell_hit_rating', 0), 'spell_hit_rating', 0, 100000, True)
        number(player.get('ranged_hit_rating', 0), 'ranged_hit_rating', 0, 100000, True)
    for creature in creatures:
        keys(creature, {'id', 'owner', 'entry'},
             {'id', 'owner', 'entry', 'distance', 'faction', 'level', 'health'}, 'creature')
        identity = creature['id']
        require(isinstance(identity, str) and ACTOR_ID.fullmatch(identity), 'Invalid creature id')
        require(identity not in actor_ids, 'Duplicate actor id')
        actor_ids.add(identity)
        require(creature['owner'] in player_ids, 'Creature owner must be a player')
        for key, default in (('entry', None), ('faction', 14), ('health', 100000)):
            number(creature.get(key, default), key, 1, 2**31 - 1, True)
        number(creature.get('level', 80), 'creature level', 1, 255, True)
        number(creature.get('distance', 3), 'distance', 0, 100)
    if 'location' in scenario:
        location = scenario['location']
        keys(location, {'map', 'x', 'y', 'z'}, {'map', 'x', 'y', 'z', 'o'}, 'location')
        number(location['map'], 'map', 0, 2**32 - 1, True)
        for key in ('x', 'y', 'z'):
            number(location[key], key, -17000, 17000)
        number(location.get('o', 0), 'orientation', 0, 2 * math.pi)
    steps = scenario['steps']
    require(isinstance(steps, list) and 1 <= len(steps) <= 10000, 'Expected 1..10000 steps')
    snapshots = {}
    assertions = 0
    for index, step in enumerate(steps):
        where = f'step {index}'
        require(isinstance(step, dict) and step.get('action') in ACTIONS, f'{where}: unknown action')
        action = step['action']
        required, allowed = ACTIONS[action]
        keys(step, required | {'action'}, allowed | {'action', 'label'}, where)
        if action == 'console':
            require(isinstance(step['command'], str) and step['command'].strip()
                    and '\n' not in step['command'] and '\r' not in step['command'],
                    f'{where}: expected one console command')
        if 'actor' in step:
            require(step['actor'] in actor_ids, f'{where}: unknown actor')
            require(action in {'snapshot', 'assert'} or step['actor'] in player_ids,
                    f'{where}: action needs a player')
        for key in ('target', 'caster'):
            if key in step:
                require(step[key] in actor_ids, f'{where}: unknown {key}')
        for key in ('spell', 'item', 'talent', 'count', 'entry'):
            if key in step:
                number(step[key], f'{where}.{key}', 1, 2**31 - 1, True)
        for key, maximum in (('rank', 4), ('effect', 2), ('slot', 18), ('power', 6)):
            if key in step:
                number(step[key], f'{where}.{key}', 0, maximum, True)
        for key in ('ms', 'within_ms'):
            if key in step:
                number(step[key], f'{where}.{key}', 0, scenario.get('timeout_ms', 90000), True)
        if 'value' in step:
            number(step['value'], f'{where}.value', 1 if action == 'set_health' else 0, 2**31 - 1, True)
        if action in {'snapshot', 'assert'}:
            metric = step['metric']
            require(metric in METRICS, f'{where}: unknown metric')
            if metric.startswith('aura') or metric in {'knows_spell', 'cooldown_ms', 'has_talent', 'pet_aura_stacks'}:
                require('spell' in step, f'{where}: metric needs spell')
            if metric == 'item_count':
                require('item' in step, f'{where}: metric needs item')
            if metric == 'owned_creature_count':
                require('entry' in step, f'{where}: metric needs creature entry')
            if metric in {'knows_spell', 'has_talent', 'talent_points', 'cooldown_ms', 'item_count', 'bank_bag_slots',
                          'pet_entry', 'pet_aura_stacks', 'owned_creature_count'}:
                require(step['actor'] in player_ids, f'{where}: metric needs a player')
            if 'relative_to' in step:
                require(snapshots.get(step['relative_to']) == metric, f'{where}: missing or incompatible snapshot')
            if action == 'snapshot':
                name = step['save_as']
                require(isinstance(name, str) and ACTOR_ID.fullmatch(name), f'{where}: invalid snapshot name')
                require(name not in snapshots, f'{where}: duplicate snapshot')
                snapshots[name] = metric
            else:
                assertions += 1
                require(any(key in step for key in ('equals', 'min', 'max')), f'{where}: no expected value')
                for key in ('equals', 'min', 'max'):
                    if key in step:
                        number(step[key], f'{where}.{key}')
                require(step.get('min', -math.inf) <= step.get('max', math.inf), f'{where}: reversed range')
                if 'equals' in step:
                    require(step.get('min', -math.inf) <= step['equals'] <= step.get('max', math.inf),
                            f'{where}: contradictory assertion')
    require(assertions > 0, 'Scenario must contain assertions')
    return scenario


def read_config(path):
    values = {}
    for line in Path(path).read_text(encoding='utf-8-sig').splitlines():
        match = re.match(r'^\s*([A-Za-z0-9_.]+)\s*=\s*(.*)$', line)
        if not match:
            continue
        key, value = match.groups()
        if value.startswith('"'):
            require('"' in value[1:], f'Unterminated config value: {key}')
            value = value[1:value.index('"', 1)]
        else:
            value = value.split('#', 1)[0].strip()
        values[key] = value
    return values


@dataclass(frozen=True)
class Connection:
    host: str
    port: int
    user: str
    password: str = field(repr=False)
    database: str

    @classmethod
    def parse(cls, value):
        parts = value.split(';')
        require(len(parts) == 5, 'Expected five database connection fields')
        host, port, user, password, database = parts
        require(host in LOCAL_HOSTS, 'Only local database sources are supported')
        require(IDENTIFIER.fullmatch(database), 'Invalid source database name')
        require(port.isdecimal() and 0 < int(port) <= 65535, 'Invalid database port')
        return cls(host, int(port), user, password, database)

    def with_database(self, name):
        return ';'.join((self.host, str(self.port), self.user, self.password, name))


def cnf_quote(value):
    return '"' + value.replace('\\', '\\\\').replace('"', '\\"').replace('\n', '\\n').replace('\r', '\\r') + '"'


def database_credentials(connections, path):
    """Read optional existing admin credentials without changing the source endpoint or schema."""
    parser = configparser.ConfigParser(interpolation=None)
    try:
        parser.read_string(path.read_text(encoding='utf-8-sig'))
    except configparser.Error:
        raise ValueError('Invalid database client config') from None
    require('client' in parser, 'Database client config needs a [client] section')
    settings = parser['client']

    def value(key):
        require(key in settings, f'Database client config needs {key}')
        text = settings[key].strip()
        if text.startswith(('"', "'")):
            require(len(text) >= 2 and text[-1] == text[0], f'Invalid client config quoting for {key}')
            text = text[1:-1]
        escapes = {'b': '\b', 't': '\t', 'n': '\n', 'r': '\r', 's': ' ', '\\': '\\', '"': '"', "'": "'"}
        return re.sub(r'\\(.)', lambda match: escapes.get(match[1], match[0]), text)

    host, port, user, password = (value(key) for key in ('host', 'port', 'user', 'password'))
    require(host in LOCAL_HOSTS and port.isdecimal(), 'Client config must select a local MySQL endpoint')
    require(user and not any(char in user + password for char in ';\r\n"'),
            'Client credentials cannot be represented in a worldserver connection string')
    result = {}
    for role, source in connections.items():
        require(host == source.host and int(port) == source.port,
                'Client config endpoint must match every source database connection')
        result[role] = Connection(source.host, source.port, user, password, source.database)
    return result


class Databases:
    def __init__(self, mysql, dump, directory, connections, run_id):
        require(re.fullmatch(r'[0-9a-f]{12}', run_id), 'Invalid run id')
        self.mysql = str(mysql)
        self.dump = str(dump)
        self.directory = directory
        self.connections = connections
        self.names = {role: f'coa_test_{run_id}_{role}' for role in connections}
        self.created = []
        self.option_files = {}
        try:
            for role, connection in connections.items():
                require(self.names[role] != connection.database, 'Source and test database must differ')
                path = directory / f'{role}-client.cnf'
                self.option_files[role] = path
                # Never put credentials in process arguments or test reports.
                content = '[client]\n' + '\n'.join(f'{key}={cnf_quote(str(value))}' for key, value in {
                    'host': connection.host, 'port': connection.port, 'user': connection.user,
                    'password': connection.password, 'protocol': 'TCP', 'default-character-set': 'utf8mb4',
                }.items()) + '\n'
                path.write_text(content, encoding='utf-8')
                path.chmod(0o600)
        except BaseException:
            self.remove_credentials()
            raise

    def redact(self, detail):
        for connection in self.connections.values():
            if connection.password:
                detail = detail.replace(connection.password, '[redacted]')
        return detail.strip()[-4000:]

    def sql(self, role, statement):
        result = subprocess.run([self.mysql, f'--defaults-extra-file={self.option_files[role]}',
                                 '--batch', '--skip-column-names'], input=statement, text=True,
                                capture_output=True, timeout=60, creationflags=CREATE_FLAGS)
        if result.returncode:
            detail = self.redact(result.stderr)
            raise ValueError(f'MySQL operation failed for {role}: {detail or result.returncode}')
        return result.stdout.strip()

    def copy(self, role, schema_only=False, tables=()):
        connection = self.connections[role]
        arguments = [self.dump, f'--defaults-extra-file={self.option_files[role]}',
                     '--single-transaction', '--skip-lock-tables', '--no-tablespaces', '--skip-add-locks',
                     '--set-gtid-purged=OFF', '--column-statistics=0', '--skip-triggers']
        if schema_only:
            arguments.append('--no-data')
        elif tables:
            arguments.append('--no-create-info')
        arguments.extend([connection.database, *tables])
        # Stream the snapshot, keeping the world database out of Python memory.
        with tempfile.TemporaryFile() as errors:
            source = subprocess.Popen(arguments, stdout=subprocess.PIPE, stderr=errors, creationflags=CREATE_FLAGS)
            target = None
            try:
                target = subprocess.Popen([self.mysql, f'--defaults-extra-file={self.option_files[role]}',
                                           self.names[role]], stdin=source.stdout, stdout=subprocess.DEVNULL,
                                          stderr=errors, creationflags=CREATE_FLAGS)
                source.stdout.close()
                target.wait(timeout=1800)
                source.wait(timeout=60)
                if source.returncode or target.returncode:
                    errors.seek(0)
                    detail = self.redact(errors.read().decode('utf-8', errors='replace'))
                    raise ValueError(f'Database copy failed for {role}: {detail}')
            finally:
                for process in (target, source):
                    if process is not None and process.poll() is None:
                        process.kill()
                        process.wait()

    def prepare(self):
        for role in ('auth', 'characters', 'world'):
            name = self.names[role]
            # CREATE without IF NOT EXISTS fails closed on collisions; only our creations are dropped.
            self.sql(role, f'CREATE DATABASE `{name}` CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;')
            self.created.append(role)
            print(f'Preparing isolated {role} database...', flush=True)
            self.copy(role, schema_only=(role != 'world'))
            if role != 'world':
                tables = ['updates', 'updates_include']
                if role == 'auth':
                    tables += ['rbac_permissions', 'rbac_linked_permissions', 'rbac_default_permissions', 'realmlist']
                else:
                    # Required realm metadata: ArenaSeasonMgr asserts if this table is empty.
                    tables += ['active_arena_season']
                self.copy(role, tables=tables)

    def cleanup(self):
        failures = []
        for role in reversed(self.created):
            try:
                self.sql(role, f'DROP DATABASE `{self.names[role]}`;')
            except (ValueError, subprocess.SubprocessError):
                failures.append(self.names[role])
        return failures

    def remove_credentials(self):
        for path in self.option_files.values():
            path.unlink(missing_ok=True)


def unused_port():
    with socket.socket() as listener:
        listener.bind(('127.0.0.1', 0))
        return listener.getsockname()[1]


def write_config(source, destination, overrides):
    lines = []
    for line in source.read_text(encoding='utf-8-sig').splitlines():
        match = re.match(r'^\s*([A-Za-z0-9_.]+)\s*=', line)
        if not match or match.group(1) not in overrides:
            lines.append(line)
    for key, value in overrides.items():
        require('"' not in str(value) and '\n' not in str(value) and '\r' not in str(value),
                f'Unsupported config characters in {key}')
        lines.append(f'{key} = "{value}"')
    destination.write_text('\n'.join(lines) + '\n', encoding='utf-8')
    destination.chmod(0o600)


def stage_modules(source, directory, reserved):
    staged = []
    try:
        for path in sorted(source.glob('*.conf')):
            settings = read_config(path)
            require(not settings.keys() & reserved
                    and not any(key.startswith('CoAGameplayTest.') for key in settings),
                    f'Module config overrides harness controls: {path.name}')
            destination = directory / 'configs' / 'modules' / path.name
            destination.parent.mkdir(parents=True, exist_ok=True)
            destination.write_bytes(path.read_bytes())
            destination.chmod(0o600)
            staged.append(destination)
        return staged
    except BaseException:
        for path in staged:
            path.unlink(missing_ok=True)
        raise


class ServerStillRunning(RuntimeError):
    def __init__(self, pid):
        super().__init__(f'Test process {pid} could not be stopped; databases were retained')
        self.pid = pid


def check_report(report, run_id, scenario, returncode):
    require(int(report.get('schema', 0)) == 1, 'Unsupported result schema')
    require(report.get('run_id') == run_id, 'Result belongs to a different run')
    require(report.get('scenario') == scenario['name'], 'Result belongs to a different scenario')
    require(report.get('execution') == 'socketless-session-handlers', 'Unexpected execution mode')
    require(report.get('status') == 'passed', report.get('message', 'Scenario failed'))
    require(returncode == 0, f'Worldserver exited with code {returncode}')
    expected = sum(step['action'] == 'assert' for step in scenario['steps'])
    require(int(report.get('assertions', 0)) == expected, 'Not all assertions ran')
    require(int(report.get('completed_steps', 0)) == len(scenario['steps']), 'Scenario did not complete')
    records = report.get('steps', [])
    require(isinstance(records, list) and len(records) == len(scenario['steps']), 'Missing step evidence')
    for index, (record, step) in enumerate(zip(records, scenario['steps'])):
        require(int(record.get('index', -1)) == index and record.get('action') == step['action'],
                'Step evidence does not match the scenario')
        if step['action'] == 'assert':
            require(record.get('status') == 'passed', 'A recorded assertion failed')
            actual = float(record['actual'])
            require(math.isfinite(actual), 'Non-finite assertion result')
            require('equals' not in step or actual == step['equals'], 'Equality assertion failed')
            require('min' not in step or actual >= step['min'], 'Minimum assertion failed')
            require('max' not in step or actual <= step['max'], 'Maximum assertion failed')
        else:
            require(record.get('status') == 'completed', 'An action did not complete')


def run_process(command, directory, ready_path, result_path, run_id, startup_timeout, timeout):
    with (directory / 'worldserver.log').open('wb') as log:
        process = subprocess.Popen(command, cwd=directory, stdin=subprocess.PIPE, stdout=log, stderr=log,
                                   creationflags=CREATE_FLAGS)
        start = time.monotonic()
        ready_at = None
        try:
            while process.poll() is None:
                now = time.monotonic()
                if ready_at is None and ready_path.exists():
                    ready = read_json(ready_path)
                    require(ready.get('run_id') == run_id and ready.get('status') == 'ready',
                            'Invalid readiness record')
                    ready_at = now
                    print('Worldserver harness is ready; executing scenario...', flush=True)
                if ready_at is None:
                    require(now - start < startup_timeout, 'Worldserver/harness readiness timed out')
                else:
                    require(now - ready_at < timeout, 'Gameplay scenario/shutdown timed out')
                time.sleep(0.1)
            require(result_path.exists(), 'Worldserver exited without a result; check the build and server log')
            report = read_json(result_path)
            if report.get('status') == 'passed':
                require(ready_path.exists(), 'Successful result is missing harness readiness')
                ready = read_json(ready_path)
                require(ready.get('run_id') == run_id and ready.get('status') == 'ready', 'Invalid readiness record')
            return report, process.returncode
        finally:
            if process.poll() is None:
                try:
                    process.stdin.write(b'server shutdown 0\n')
                    process.stdin.flush()
                    process.wait(timeout=15)
                except (OSError, subprocess.TimeoutExpired):
                    try:
                        process.kill()
                        process.wait(timeout=15)
                    except (OSError, subprocess.TimeoutExpired) as error:
                        if process.poll() is None:
                            raise ServerStillRunning(process.pid) from error
            process.stdin.close()


def sha256(path):
    with path.open('rb') as source:
        return hashlib.file_digest(source, 'sha256').hexdigest()


def execute(args, scenario):
    binary = args.worldserver.resolve(strict=True)
    source_config = args.config.resolve(strict=True)
    mysql = args.mysql.resolve(strict=True)
    dump = args.mysqldump.resolve(strict=True)
    config = read_config(source_config)
    connections = {role: Connection.parse(config[key]) for role, key in {
        'auth': 'LoginDatabaseInfo', 'characters': 'CharacterDatabaseInfo', 'world': 'WorldDatabaseInfo',
    }.items()}
    if args.database_client_config:
        connections = database_credentials(connections, args.database_client_config)
    run_id = secrets.token_hex(6)
    output = (args.output or ROOT / '.cache' / 'coa-gameplay-tests' / run_id).resolve()
    output.mkdir(parents=True, exist_ok=False)
    result_path = output / 'result.json'
    ready_path = output / 'ready.json'
    scenario_path = output / 'scenario.json'
    scenario_path.write_text(json.dumps(scenario, indent=2) + '\n', encoding='utf-8')
    print(f'Run {run_id}: {output}', flush=True)
    generated_config = output / 'worldserver.conf'
    module_configs = []
    retain_databases = False
    summary = {'schema': 1, 'run_id': run_id, 'scenario': scenario['name'], 'status': 'failed',
               'binary_sha256': sha256(binary), 'scenario_sha256': sha256(scenario_path),
               'binary': str(binary)}
    database = Databases(mysql, dump, output, connections, run_id)
    summary['databases'] = database.names
    try:
        database.prepare()
        data_dir = Path(config.get('DataDir', '.'))
        if not data_dir.is_absolute():
            data_dir = binary.parent / data_dir
        overrides = {
            'LoginDatabaseInfo': connections['auth'].with_database(database.names['auth']),
            'CharacterDatabaseInfo': connections['characters'].with_database(database.names['characters']),
            'WorldDatabaseInfo': connections['world'].with_database(database.names['world']),
            'DataDir': data_dir.resolve().as_posix(), 'SourceDirectory': ROOT.as_posix(),
            'LogsDir': output.as_posix(), 'BindIP': '127.0.0.1', 'WorldServerPort': unused_port(),
            'Console.Enable': 1, 'Ra.Enable': 0, 'SOAP.Enabled': 0, 'MapUpdate.Threads': 0,
            'Warden.Enabled': 0, 'Network.UseSocketActivation': 0,
            'LoginDatabase.WorkerThreads': 1, 'CharacterDatabase.WorkerThreads': 1,
            'Updates.EnableDatabases': 7, 'CoAGameplayTest.Enable': 1, 'CoAGameplayTest.RunId': run_id,
            'CoAGameplayTest.ScenarioFile': scenario_path.as_posix(),
            'CoAGameplayTest.ReadyFile': ready_path.as_posix(),
            'CoAGameplayTest.ResultFile': result_path.as_posix(),
        }
        module_source = args.modules_config_dir or source_config.parent / 'modules'
        module_configs = stage_modules(module_source, output, set(overrides))
        summary['module_config_sha256'] = {path.name: sha256(path) for path in module_configs}
        write_config(source_config, generated_config, overrides)
        report, returncode = run_process([str(binary), '-c', str(generated_config)], output, ready_path,
                                         result_path, run_id, args.startup_timeout,
                                         scenario.get('timeout_ms', 90000) / 1000 + 30)
        check_report(report, run_id, scenario, returncode)
        summary.update(status='passed', assertions=int(report['assertions']))
    except ServerStillRunning as error:
        retain_databases = True
        summary.update(message=str(error), process_id=error.pid)
    except (ValueError, OSError, subprocess.SubprocessError, KeyError) as error:
        summary['message'] = str(error)
    finally:
        failures = list(database.names.values()) if retain_databases else database.cleanup()
        if failures:
            summary.update(status='failed', cleanup_failed=failures)
        generated_config.unlink(missing_ok=True)
        for path in module_configs:
            path.unlink(missing_ok=True)
        database.remove_credentials()
        (output / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n', encoding='utf-8')
    print(f"{summary['status'].upper()}: {scenario['name']}\nResults: {output}")
    if 'message' in summary:
        print(summary['message'])
    return 0 if summary['status'] == 'passed' else 1


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest='command', required=True)
    check = subparsers.add_parser('validate', help='Validate a scenario without starting a server')
    check.add_argument('scenario', type=Path)
    run = subparsers.add_parser('run', help='Clone local test DBs, run a scenario, and clean up')
    run.add_argument('scenario', type=Path)
    run.add_argument('--worldserver', type=Path, required=True)
    run.add_argument('--config', type=Path, required=True, help='Source worldserver config; never changed')
    run.add_argument('--mysql', type=Path, required=True)
    run.add_argument('--mysqldump', type=Path, required=True)
    run.add_argument('--database-client-config', type=Path,
                     help='Optional MySQL [client] file with credentials allowed to create/drop test schemas')
    run.add_argument('--modules-config-dir', type=Path, help='Defaults to the source config directory/modules')
    run.add_argument('--output', type=Path, help='New directory for logs and results')
    run.add_argument('--startup-timeout', type=float, default=600)
    args = parser.parse_args(argv)
    try:
        scenario = validate(read_json(args.scenario))
        if args.command == 'validate':
            print(f"Valid scenario: {scenario['name']} ({len(scenario['steps'])} steps)")
            return 0
        require(math.isfinite(args.startup_timeout) and args.startup_timeout > 0, 'Invalid startup timeout')
        return execute(args, scenario)
    except (ValueError, OSError, KeyError, TypeError) as error:
        print(f'ERROR: {error}', file=sys.stderr)
        return 1


if __name__ == '__main__':
    sys.exit(main())
