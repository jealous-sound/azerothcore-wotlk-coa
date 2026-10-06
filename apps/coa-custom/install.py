# CoA Custom installer: puts the custom races / vanilla classes / incarnation build on a Jealous-Sound CoA repack
# that already runs CoA Bots. Run through Install-Custom.bat (uses the repack's own Python).
#   install.py [--repack <folder>] [--client <Ascension folder>] [--uninstall] [--check] [--yes]
#   --check: report what would be done, change nothing
import json, os, re, shutil, subprocess, sys, datetime, gzip
from pathlib import Path

HERE = Path(__file__).resolve().parent
FILES = HERE / 'files'
MANIFEST_PRE = json.loads((HERE / 'manifest.json').read_text(encoding='utf-8'))
BACKUP = HERE / ('backup-' + MANIFEST_PRE['repackRevision'])   # one per repack release: a 1.6-era backup must not
                                                               # be put back on a repack updated to CoA Bots 1.8
STATE = HERE / 'installed.json'
MANIFEST = json.loads((HERE / 'manifest.json').read_text(encoding='utf-8'))
SETTINGS = {                                        # template file -> {key: value}
    'worldserver.conf.template': {'CharactersPerRealm': '120', 'CharactersPerAccount': '120'},
    'coa.conf.template': {'CoA.CharacterSelectionMaxActive': '120'},
}


def say(text=''):
    print(text, flush=True)


def fail(text):
    say()
    say('ERROR: ' + text)
    sys.exit(1)


def arg(name):
    return sys.argv[sys.argv.index(name) + 1] if name in sys.argv and sys.argv.index(name) + 1 < len(sys.argv) else None


def ask(question, default=None):
    if '--yes' in sys.argv and default is not None:
        return default
    answer = input(question + (' [%s]' % default if default else '') + ': ').strip().strip('"')
    return answer or default


def find_repack():
    for candidate in (arg('--repack'), HERE.parent):
        if candidate and (Path(candidate) / 'Start_All_Server.bat').exists():
            return Path(candidate).resolve()
    while True:
        folder = ask('Repack folder (the one with Start_All_Server.bat; you can drag it into this window)')
        if folder and (Path(folder) / 'Start_All_Server.bat').exists():
            return Path(folder).resolve()
        say('  Start_All_Server.bat is not in that folder.')


def find_client(previous):
    guesses = [arg('--client'), previous, r'C:\Ascension Local', r'C:\Program Files\Ascension Launcher\resources\client',
               r'C:\Ascension\resources\client']
    default = next((g for g in guesses if g and (Path(g) / 'Data').is_dir() and (Path(g) / 'Ascension.exe').exists()), None)
    while True:
        folder = ask('Ascension game folder (the one with Ascension.exe)', default)
        if folder and (Path(folder) / 'Ascension.exe').exists() and (Path(folder) / 'Data').is_dir():
            return Path(folder).resolve()
        say('  Ascension.exe and its Data folder are not in that folder.')


def running(exe):
    out = subprocess.run(['tasklist', '/FI', 'IMAGENAME eq ' + exe], capture_output=True, text=True).stdout
    return exe.lower() in out.lower()


def python(root, *arguments, cwd=None):
    exe = root / 'Runtime' / 'python' / 'python.exe'
    subprocess.run([str(exe), '-B', *map(str, arguments)], cwd=str(cwd or root), check=False)


def mysql_file(root, sql_file):
    exe = root / 'mysql' / 'bin' / 'mysql.exe'
    with open(sql_file, 'rb') as source:
        result = subprocess.run([str(exe), '--defaults-file=' + str(root / 'mysql' / 'admin-client.ini')], stdin=source,
                                capture_output=True)
    if result.returncode:
        fail('%s failed:\n%s' % (sql_file.name, result.stderr.decode('utf-8', 'replace')))


def dump_tables(root, target):
    exe = root / 'mysql' / 'bin' / 'mysqldump.exe'
    with open(target, 'wb') as out:
        result = subprocess.run([str(exe), '--defaults-file=' + str(root / 'mysql' / 'admin-client.ini'),
                                 '--skip-triggers', '--no-tablespaces', '--add-drop-table', 'acore_world',
                                 *MANIFEST['worldTables']], stdout=out, stderr=subprocess.PIPE)
    if result.returncode:
        fail('could not back up the world tables:\n' + result.stderr.decode('utf-8', 'replace'))


def backup_characters(root):
    """accounts + characters, before anything changes: CoA-Custom/character-backups (kept on uninstall)"""
    folder = HERE / 'character-backups'
    folder.mkdir(exist_ok=True)
    target = folder / ('characters_%s.sql.gz' % datetime.datetime.now().strftime('%Y-%m-%d_%H-%M-%S'))
    say('Backing up accounts and characters to %s ...' % target)
    dump = subprocess.Popen([str(root / 'mysql' / 'bin' / 'mysqldump.exe'),
                             '--defaults-file=' + str(root / 'mysql' / 'admin-client.ini'), '--single-transaction',
                             '--routines', '--no-tablespaces', '--add-drop-database',
                             '--databases', 'acore_auth', 'acore_characters'], stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE)
    with gzip.open(target, 'wb', compresslevel=6) as out:
        shutil.copyfileobj(dump.stdout, out, 1 << 20)
    error = dump.stderr.read().decode('utf-8', 'replace')
    if dump.wait() != 0:
        target.unlink(missing_ok=True)
        fail('the character backup failed, nothing was changed:\n' + error)


def check_release(root):
    release = json.loads((root / 'RELEASE.json').read_text(encoding='utf-8')) if (root / 'RELEASE.json').exists() else {}
    if not str(release.get('mainRevision', '')).startswith(MANIFEST['repackRevision']):
        fail('this package is for the CoA repack %s; this repack is %s.'
             % (MANIFEST['repackRevision'], release.get('mainRevision', 'unknown')))
    bots = root / 'CoA-Bots' / 'release.json'
    if not bots.exists() or not (root / 'CoA-Bots' / 'Core' / 'worldserver.exe').exists():
        fail('CoA Bots %s must be installed first (CoA-Bots\\Installer-Bots.bat).' % MANIFEST['botsVersion'])
    version = json.loads(bots.read_text(encoding='utf-8')).get('version')
    if version != MANIFEST['botsVersion']:
        fail('this package needs CoA Bots %s; this repack has CoA Bots %s.' % (MANIFEST['botsVersion'], version))


def set_settings(root):
    for name, values in SETTINGS.items():
        path = root / 'Settings' / name
        if not path.exists():
            continue
        text = path.read_text(encoding='utf-8')
        for key, value in values.items():
            text = re.sub(r'(?m)^(%s\s*=\s*).*$' % re.escape(key), r'\g<1>' + value, text)
        path.write_text(text, encoding='utf-8', newline='\n')


def stop_servers(root):
    say('Stopping the servers...')
    python(root, root / 'Scripts' / 'manage.py', 'stop-all')


def start_servers(root):
    say('Starting the servers with CoA Bots...')
    python(root, root / 'CoA-Bots' / 'coa_bots.py', 'start-all', cwd=root / 'CoA-Bots')


def install(root):
    state = json.loads(STATE.read_text(encoding='utf-8')) if STATE.exists() else {}
    client = find_client(state.get('client'))
    stop_servers(root)
    python(root, root / 'Scripts' / 'manage.py', 'start-mysql')
    backup_characters(root)
    targets = {
        'worldserver.exe': root / 'CoA-Bots' / 'Core' / 'worldserver.exe',
        'patch-T.MPQ': client / 'Data' / 'patch-T.MPQ',
    }
    for dbc in sorted((FILES / 'dbc').glob('*.dbc')):
        targets['dbc/' + dbc.name] = root / 'Data' / 'dbc' / dbc.name
    for name in SETTINGS:
        targets['settings/' + name] = root / 'Settings' / name
    # the server's copy of the client display table: without it the display patch stream re-sends every display
    # id >= 652000 with no textures, which turns other players' Murlocs white in the client
    targets['dbc_clientset/CreatureDisplayInfo.dbc'] = root / 'Data' / 'dbc_clientset' / 'CreatureDisplayInfo.dbc'
    # Book of Ascension settings missing from the bot server (warning spam); never replaced once it exists
    targets['bots/spellbook.conf'] = root / 'CoA-Bots' / 'Core' / 'configs' / 'modules' / 'spellbook.conf'
    created = state.get('created', [])
    if not BACKUP.exists():                          # first install on this repack release: keep the originals
        created = [key for key, target in targets.items() if not target.exists()]
        say('Backing up the original files and world tables to %s ...' % BACKUP)
        older = HERE / 'backup'                      # a 1.0 / 1.1 install's backup still holds the true originals of
        for key, target in targets.items():          # the files the CoA Bots update does not replace (DBCs, patch)
            source = older / key if key != 'worldserver.exe' and (older / key).exists() else target
            if source.exists():
                (BACKUP / key).parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(source, BACKUP / key)
        dump_tables(root, BACKUP / 'world_tables.sql')
    say('Copying files...')
    shutil.copy2(FILES / 'worldserver.exe', targets['worldserver.exe'])
    shutil.copy2(FILES / 'client' / 'patch-T.MPQ', targets['patch-T.MPQ'])
    for dbc in sorted((FILES / 'dbc').glob('*.dbc')):
        shutil.copy2(dbc, targets['dbc/' + dbc.name])
    targets['dbc_clientset/CreatureDisplayInfo.dbc'].parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(FILES / 'dbc_clientset' / 'CreatureDisplayInfo.dbc', targets['dbc_clientset/CreatureDisplayInfo.dbc'])
    if not targets['bots/spellbook.conf'].exists():
        shutil.copy2(FILES / 'bots' / 'spellbook.conf', targets['bots/spellbook.conf'])
    set_settings(root)
    say('Applying the database changes...')
    for sql in sorted((FILES / 'sql').glob('*.sql')):
        say('  ' + sql.name)
        mysql_file(root, sql)
    STATE.write_text(json.dumps({'version': MANIFEST['version'], 'repack': str(root), 'client': str(client),
                                 'created': created,
                                 'installed': datetime.datetime.now().isoformat(timespec='seconds')}, indent=1),
                     encoding='utf-8')
    start_servers(root)
    say()
    say('CoA Custom %s installed. Start the game once the worldserver says it is ready.' % MANIFEST['version'])


def uninstall(root):
    if not BACKUP.exists() or not STATE.exists():
        fail('nothing to uninstall (no backup in %s).' % BACKUP)
    state = json.loads(STATE.read_text(encoding='utf-8'))
    client = Path(state['client'])
    stop_servers(root)
    python(root, root / 'Scripts' / 'manage.py', 'start-mysql')
    backup_characters(root)
    say('Restoring the original files...')
    restore = {'worldserver.exe': root / 'CoA-Bots' / 'Core' / 'worldserver.exe',
               'patch-T.MPQ': client / 'Data' / 'patch-T.MPQ'}
    for item in (BACKUP / 'dbc').glob('*.dbc') if (BACKUP / 'dbc').exists() else []:
        restore['dbc/' + item.name] = root / 'Data' / 'dbc' / item.name
    for item in (BACKUP / 'settings').glob('*') if (BACKUP / 'settings').exists() else []:
        restore['settings/' + item.name] = root / 'Settings' / item.name
    restore['dbc_clientset/CreatureDisplayInfo.dbc'] = root / 'Data' / 'dbc_clientset' / 'CreatureDisplayInfo.dbc'
    restore['bots/spellbook.conf'] = root / 'CoA-Bots' / 'Core' / 'configs' / 'modules' / 'spellbook.conf'
    for key, target in restore.items():
        if (BACKUP / key).exists():
            shutil.copy2(BACKUP / key, target)
        elif key in state.get('created', []) and target.exists():
            target.unlink()                          # the install added it: take it away again
    say('Restoring the world tables...')
    mysql_file(root, BACKUP / 'world_tables.sql')
    for table in MANIFEST['newWorldTables']:
        subprocess.run([str(root / 'mysql' / 'bin' / 'mysql.exe'),
                        '--defaults-file=' + str(root / 'mysql' / 'admin-client.ini'),
                        '-e', 'DROP TABLE IF EXISTS acore_world.`%s`;' % table])
    shutil.rmtree(BACKUP)
    STATE.unlink()
    start_servers(root)
    say()
    say('CoA Custom removed. Characters of the custom races or classes that only this package allows will not be able'
        ' to log in until it is installed again.')


def main():
    say('CoA Custom %s - for CoA repack %s + CoA Bots %s' % (MANIFEST['version'], MANIFEST['repackRevision'],
                                                            MANIFEST['botsVersion']))
    say()
    if running('Ascension.exe') and '--check' not in sys.argv:
        fail('close the game (Ascension.exe) first, then run this again.')
    root = find_repack()
    check_release(root)
    if '--check' in sys.argv:
        client = find_client(None)
        say('Repack: %s (release and CoA Bots %s OK)' % (root, MANIFEST['botsVersion']))
        say('Client: %s' % client)
        say(r'Would replace: CoA-Bots\Core\worldserver.exe, %d files in Data\dbc, %s'
            % (len(list((FILES / 'dbc').glob('*.dbc'))), client / 'Data' / 'patch-T.MPQ'))
        say('Would apply: %s' % ', '.join(p.name for p in sorted((FILES / 'sql').glob('*.sql'))))
        say('Would back up %d world tables first (%s).' % (len(MANIFEST['worldTables']),
                                                          'already done' if BACKUP.exists() else 'first install'))
        return
    if '--uninstall' in sys.argv:
        uninstall(root)
    else:
        say('This replaces the bot worldserver, some server DBC files and the client patch-T.MPQ, and changes the')
        say('world database. The originals are backed up on the first install (Uninstall-Custom.bat puts them back).')
        say('Back up your repack folder first if you care about your characters.')
        if ask('Continue? (y/n)', 'y').lower() != 'y':
            sys.exit(0)
        install(root)


if __name__ == '__main__':
    main()
