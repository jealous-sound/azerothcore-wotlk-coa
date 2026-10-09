import argparse
import os
from pathlib import Path
import subprocess
import tempfile
import time
import unittest
import uuid


ROOT = Path(__file__).resolve().parents[2]
MIGRATION = ROOT / 'data/sql/updates/pending_db_characters/rev_20261009_4569_account_trial_completion.sql'
MODULE_SCHEMA = ROOT / 'modules/mod-coa-challenges/data/sql/db-characters/2026_09_16_00_mod_coa_challenges.sql'
PROCESS_OPTIONS = {'creationflags': subprocess.CREATE_NO_WINDOW} if os.name == 'nt' else {}


class TrialCompletionMigration(unittest.TestCase):
    mysql_bin = None

    @classmethod
    def setUpClass(cls):
        suffix = '.exe' if os.name == 'nt' else ''
        mysql = cls.mysql_bin / ('mysql' + suffix)
        mysqld = cls.mysql_bin / ('mysqld' + suffix)
        if not mysql.is_file() or not mysqld.is_file():
            raise RuntimeError('--mysql-bin must contain mysql and mysqld')
        cls.temporary = tempfile.TemporaryDirectory(prefix='coa-trial-migration-')
        cls.directory = Path(cls.temporary.name).resolve()
        cls.process = None
        cls.addClassCleanup(cls.stop_mysql)
        cls.log = cls.directory / 'mysql.log'
        server = [str(mysqld), '--no-defaults', f'--basedir={cls.mysql_bin.parent}',
                  f'--datadir={cls.directory / "data"}', f'--log-error={cls.log}',
                  f'--secure-file-priv={cls.directory}', '--innodb-buffer-pool-size=32M']
        if os.name != 'nt' and os.getuid() == 0:
            server.append('--user=root')
        subprocess.run(server + ['--initialize-insecure'], check=True, capture_output=True,
                       timeout=90, **PROCESS_OPTIONS)
        endpoint = ('coa_trial_' + uuid.uuid4().hex if os.name == 'nt'
                    else str(cls.directory / 'mysql.sock'))
        cls.client = [str(mysql), '--no-defaults', '--no-login-paths', '--user=root', '--batch',
                      '--skip-column-names', '--default-character-set=utf8mb4', '--connect-timeout=1',
                      f'--socket={endpoint}']
        server += ['--skip-networking', '--mysqlx=OFF', f'--socket={endpoint}']
        if os.name == 'nt':
            server.append('--enable-named-pipe')
            cls.client += ['--protocol=PIPE', '--host=.']
        else:
            cls.client.append('--protocol=SOCKET')
        cls.process = subprocess.Popen(server, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
                                       **PROCESS_OPTIONS)
        deadline = time.monotonic() + 40
        while time.monotonic() < deadline:
            if cls.process.poll() is not None:
                raise RuntimeError(cls.log.read_text(encoding='utf-8'))
            result = subprocess.run(cls.client, input='SELECT 1;\n', text=True, capture_output=True,
                                    timeout=5, **PROCESS_OPTIONS)
            if result.returncode == 0 and result.stdout.strip() == '1':
                break
            time.sleep(0.2)
        else:
            raise RuntimeError('Isolated MySQL startup timed out: ' + cls.log.read_text(encoding='utf-8'))
        cls.query('CREATE DATABASE trial_migration_test;', database=None)

    @classmethod
    def stop_mysql(cls):
        if cls.process is not None and cls.process.poll() is None:
            try:
                subprocess.run(cls.client, input='SHUTDOWN;\n', text=True, capture_output=True,
                               timeout=10, **PROCESS_OPTIONS)
                cls.process.wait(timeout=15)
            except subprocess.TimeoutExpired:
                cls.process.kill()
                cls.process.wait(timeout=10)
        cls.temporary.cleanup()

    @classmethod
    def query(cls, sql, database='trial_migration_test'):
        result = subprocess.run(cls.client + ([database] if database else []), input=sql, text=True,
                                encoding='utf-8', capture_output=True, timeout=60, **PROCESS_OPTIONS)
        if result.returncode:
            raise AssertionError(result.stderr)
        return [tuple(line.split('\t')) for line in result.stdout.splitlines()]

    def setUp(self):
        self.query('DROP DATABASE trial_migration_test; CREATE DATABASE trial_migration_test;', database=None)

    def apply_migration(self):
        self.query(MIGRATION.read_text(encoding='utf-8'))

    def test_install_without_optional_module(self):
        self.apply_migration()
        self.apply_migration()
        self.assertEqual(self.query('SELECT COUNT(*) FROM coa_account_challenge_completion;'), [('0',)])

    def test_existing_completions_backfill_once_per_account_and_trial_level(self):
        for table in ('characters', 'character_achievement', 'character_achievement_progress'):
            self.query((ROOT / 'data/sql/base/db_characters' / (table + '.sql')).read_text(encoding='utf-8'))
        self.query(MODULE_SCHEMA.read_text(encoding='utf-8'))
        self.query("INSERT INTO characters (guid,account,name,taximask,innTriggerId) VALUES "
                   "(1,7,'First','',0),(2,7,'Alt','',0),(3,8,'Other','',0);"
                   'INSERT INTO coa_challenge_completion (guid,challengeId,level,completeTime,startTime) VALUES '
                   '(1,64,1,200,20),(2,64,1,100,10),(3,64,1,150,15),'
                   '(1,278,1,300,30),(1,64,2,400,40),(999,64,1,50,5);')
        self.apply_migration()
        expected = [('7', '64', '1', '100', '10'), ('7', '64', '2', '400', '40'),
                    ('7', '278', '1', '300', '30'), ('8', '64', '1', '150', '15')]
        query = ('SELECT account,challengeId,level,completeTime,startTime FROM coa_account_challenge_completion '
                 'ORDER BY account,challengeId,level;')
        self.assertEqual(self.query(query), expected)
        self.query('DELETE FROM coa_challenge_completion WHERE guid=2; DELETE FROM characters WHERE guid=2;'
                   'UPDATE coa_challenge_completion SET completeTime=1 WHERE guid=1 AND challengeId=64 AND level=1;')
        self.apply_migration()
        self.assertEqual(self.query(query), expected)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--mysql-bin', type=Path, required=True)
    args, remaining = parser.parse_known_args()
    TrialCompletionMigration.mysql_bin = args.mysql_bin.resolve()
    unittest.main(argv=[__file__, *remaining])
