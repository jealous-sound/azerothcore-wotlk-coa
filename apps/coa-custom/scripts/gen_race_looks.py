# NPC looks for custom races without a dressable player model (custom_race_display), chosen in game by skin colour.
import struct, collections, pickle, os, json
HERE = os.path.dirname(os.path.abspath(__file__))
cs = open('C:/CoA-Repack/Custom/backups/dbc_original/CharSections.dbc', 'rb').read()
_, n, fc, rs, ss = struct.unpack('<4s4I', cs[:20])
skins = collections.defaultdict(set)
for i in range(n):
    r = struct.unpack_from('<%dI' % fc, cs, 20 + i * rs)
    if r[3] == 0:
        skins[(r[1], r[2])].add(r[9])
LOOKS = {}            # race: {gender: [display ids]}  (the Naga is a player model again, see make_naga_model.py)
# Murloc: male = Whim murloc (idx = armor (hair style) * 32 + body (skin colour)), female = classic murloc (idx = skin)
# (gen_races.py -> out_races/murloc.json)
MURLOCS = {int(sex): {int(idx): disp for idx, disp in displays.items()}
           for sex, displays in json.load(open('out_races/murloc.json'))['displays'].items()}
LOOKS[30] = dict(MURLOCS)
LOOKS[31] = dict(MURLOCS)
sql = ['USE acore_world;',
       'CREATE TABLE IF NOT EXISTS custom_race_display (race TINYINT UNSIGNED NOT NULL, gender TINYINT UNSIGNED NOT NULL, '
       'idx TINYINT UNSIGNED NOT NULL, displayId INT UNSIGNED NOT NULL, PRIMARY KEY (race, gender, idx)) '
       'COMMENT=\'NPC look worn in game by custom races, chosen by skin colour (skin index modulo count)\';',
       'DELETE FROM custom_race_display WHERE race IN (%s);' % ','.join(map(str, sorted(set(LOOKS) | {21})))]
for race, by_gender in LOOKS.items():
    for gender, displays in by_gender.items():
        for idx, disp in sorted(displays.items()):
            sql.append('INSERT INTO custom_race_display VALUES (%d,%d,%d,%d);' % (race, gender, idx, disp))
open(os.path.join(HERE, '..', 'sql', 'race_looks.sql'), 'w').write('\n'.join(sql) + '\n')
print('Murloc looks', {k: len(v) for k, v in MURLOCS.items()},
      '| rows', sum(len(d) for g in LOOKS.values() for d in g.values()))
