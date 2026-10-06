# Backs up the accounts and characters (acore_auth + acore_characters) before any update.
# Writes Custom\backups\characters\characters_<date>.sql.gz and keeps the newest KEEP backups.
# Restore: unzip it, then  mysql\bin\mysql.exe --defaults-file=mysql\admin-client.ini < characters_<date>.sql
# (stop the server first). Run with the repack's Python: Runtime\python\python.exe -B backup_characters.py
import datetime, gzip, shutil, subprocess, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]            # <repack>\Custom\scripts\ -> <repack>
OUT = ROOT / 'Custom' / 'backups' / 'characters'
KEEP = 5


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    target = OUT / ('characters_%s.sql.gz' % datetime.datetime.now().strftime('%Y-%m-%d_%H-%M-%S'))
    partial = target.with_name(target.name + '.partial')
    dump = subprocess.Popen([str(ROOT / 'mysql' / 'bin' / 'mysqldump.exe'),
                             '--defaults-file=' + str(ROOT / 'mysql' / 'admin-client.ini'),
                             '--single-transaction', '--routines', '--no-tablespaces', '--add-drop-database',
                             '--databases', 'acore_auth', 'acore_characters'],
                            stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    with gzip.open(partial, 'wb', compresslevel=6) as out:
        shutil.copyfileobj(dump.stdout, out, 1 << 20)
    error = dump.stderr.read().decode('utf-8', 'replace')
    if dump.wait() != 0:
        partial.unlink(missing_ok=True)
        print('Character backup FAILED:\n' + error)
        sys.exit(1)
    partial.replace(target)
    print('Characters backed up to %s (%d MB)' % (target, target.stat().st_size // 1048576))
    for old in sorted(OUT.glob('characters_*.sql.gz'))[:-KEEP]:
        old.unlink()


if __name__ == '__main__':
    main()
