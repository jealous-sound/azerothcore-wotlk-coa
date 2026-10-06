# Bronzebeard switch: vanilla classes learn the +1100000 retuned copies from the Book,
# and the copies get the server support data (ranks, scripts, procs, ...) of their stock spell.
import struct, collections, pickle

OFF = 1100000
ALL = 0xFFFFFFFF
VAN = [1, 2, 3, 4, 5, 6, 7, 8, 9, 11]
bit = lambda c: 1 << (c - 1)
VMASK = sum(bit(c) for c in VAN)
D = 'C:/CoA-Repack/Data/dbc/'


def load(p):
    d = open(p, 'rb').read()
    _, n, fc, rs, ss = struct.unpack('<4s4I', d[:20])
    return [list(struct.unpack_from('<%dI' % fc, d, 20 + i * rs)) for i in range(n)]


def tsv(name):
    rows = []
    unesc = lambda v: v.replace('\\t', '\t').replace('\\n', '\n').replace('\\0', '\0').replace('\\\\', '\\')
    for line in open('tbl/%s.tsv' % name, encoding='utf-8', errors='replace').read().splitlines():
        line = line.rstrip('\r')
        if line:
            rows.append([unesc(v) for v in line.split('\t')])
    return rows


names = pickle.load(open('spellnames.pkl', 'rb'))
g = pickle.load(open('gen.pkl', 'rb'))


def copy_of(s):
    c = s + OFF
    if 0 < s < 100000 and c in names and names[c][0] == names[s][0]:
        return c
    return None


cmap = {s: copy_of(s) for s in names if s < 100000 and copy_of(s)}
print('stock spells with a Bronzebeard copy:', len(cmap))

# ---------------- 1. port the support tables ----------------
sql = []


def q(v):
    if v == 'NULL':
        return 'NULL'
    try:
        float(v)
        return v
    except ValueError:
        return "'" + v.replace('\\', '\\\\').replace("'", "\\'") + "'"


def mapid(v):
    i = int(v)
    c = cmap.get(abs(i))
    return None if c is None else (c if i >= 0 else -c)


def port(table, cols_to_map, required_col=0):
    rows = tsv(table)
    existing = {tuple(r) for r in rows}
    out = []
    for r in rows:
        k = mapid(r[required_col])
        if k is None:
            continue
        nr = list(r)
        nr[required_col] = str(k)
        for ci in cols_to_map:
            m = mapid(nr[ci]) if nr[ci] not in ('NULL', '0') else None
            if m is not None:
                nr[ci] = str(m)
        if tuple(nr) in existing:
            continue
        out.append(nr)
    if out:
        for i in range(0, len(out), 400):
            sql.append('INSERT IGNORE INTO acore_world.%s VALUES %s;' % (
                table, ','.join('(' + ','.join(q(v) for v in r) + ')' for r in out[i:i + 400])))
    print('  %-26s +%d' % (table, len(out)))


# spell_ranks: whole chains only, and never touch an id that is already in a chain
ranks = tsv('spell_ranks')
inchain = {int(r[1]) for r in ranks}
chains = collections.defaultdict(list)
for f, s, rk in ranks:
    chains[int(f)].append((int(rk), int(s)))
rk_out = []
for f, lst in chains.items():
    cf = cmap.get(f)
    if not cf:
        continue
    mapped = [(rk, cmap.get(s)) for rk, s in sorted(lst)]
    mapped = [(rk, c) for rk, c in mapped if c]
    if any(c in inchain for _, c in mapped):
        continue
    rk_out += [(cf, c, rk) for rk, c in mapped]
for i in range(0, len(rk_out), 400):
    sql.append('INSERT IGNORE INTO acore_world.spell_ranks VALUES %s;' % ','.join('(%d,%d,%d)' % r for r in rk_out[i:i + 400]))
print('  %-26s +%d' % ('spell_ranks', len(rk_out)))

port('spell_required', [1])
port('spell_script_names', [])
port('spell_bonus_data', [])
port('spell_proc', [])
port('spell_proc_event', [])
port('spell_linked_spell', [1])
port('spell_custom_attr', [])
port('spell_group', [], required_col=1)
port('spell_threat', [])
port('spell_pet_auras', [3])
port('spell_cooldown_overrides', [])
port('spell_cone', [])
port('spell_jump_distance', [])
port('spell_target_position', [])

# ---------------- 2. the Book teaches the copies ----------------
tr, owners = {}, collections.defaultdict(int)
for s, d in g['tr'].items():
    c = cmap.get(s, s)
    a = tuple(cmap.get(x, x) if x else 0 for x in d['a'])
    tr[c] = dict(d, a=a)
    owners[c] |= g['owners'][s]
print('Book spells:', len(tr), 'of which copies:', sum(1 for s in tr if s >= OFF))

# class-fit for the copies (same rule as before: visible to exactly the owning classes)
sla = load(D + 'SkillLineAbility.dbc')
srci = load(D + 'SkillRaceClassInfo.dbc')
edit = {k: list(v) for k, v in g['edit'].items()}
for r in g['newsla']:
    edit[r[0]] = list(r)
allsla = {r[0]: list(r) for r in sla}
allsla.update(edit)
bysp = collections.defaultdict(list)
for r in allsla.values():
    bysp[r[2]].append(r)
srci_all = srci + [list(r) for r in g['newsrci']]
srci_by = collections.defaultdict(list)
for r in srci_all:
    srci_by[r[1]].append(r)
newsrci = [list(r) for r in g['newsrci']]
nid = max(r[0] for r in srci_all) + 1
slaid = max(allsla) + 1


def srci_allows(skill, c):
    return any(not (r[3] and not r[3] & bit(c)) for r in srci_by.get(skill, []))


def ensure_srci(skill, mask):
    global nid
    need = [c for c in VAN if mask & bit(c) and not srci_allows(skill, c)]
    if not need:
        return
    base = (srci_by.get(skill) or [None])[0]
    nr = list(base) if base else [0, skill, 0, 0, 0, 0, 0, 0]
    nr[0] = nid; nid += 1; nr[1] = skill; nr[2] = 0; nr[3] = sum(bit(c) for c in need)
    newsrci.append(nr)
    srci_by[skill].append(nr)


mainskill = {}
for c in VAN:
    cnt = collections.Counter(r[1] for s, m in owners.items() if m == bit(c) and s >= OFF for r in bysp.get(s, []))
    mainskill[c] = cnt.most_common(1)[0][0] if cnt else g_main if False else 0
for s, om in owners.items():
    recs = bysp.get(s, [])
    if recs:
        for r in recs:
            m = r[4]
            r[4] = ((ALL if m == 0 else m) & ~VMASK) | om
            edit[r[0]] = r
            ensure_srci(r[1], om)
    else:
        c = next(c for c in VAN if om & bit(c))
        nr = [slaid, mainskill[c], s, 0, om, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        slaid += 1
        edit[nr[0]] = nr
        bysp[s].append(nr)
        ensure_srci(nr[1], om)


def fit(s, c):
    b = bysp.get(s)
    if not b:
        return True
    return any((not r[4] or r[4] & bit(c)) and srci_allows(r[1], c) for r in b)


bad = [(s, c) for s, om in owners.items() for c in VAN if fit(s, c) != bool(om & bit(c))]
print('class-fit mismatches:', len(bad))

orig = {r[0]: r for r in sla}
sla_rows = [r for k, r in edit.items() if k not in orig or r != orig[k]]
sv = lambda v: str(v if v < 2 ** 31 else v - 2 ** 32)
for i in range(0, len(sla_rows), 500):
    sql.append('REPLACE INTO acore_world.skilllineability_dbc VALUES ' + ','.join('(' + ','.join(sv(v) for v in r) + ')' for r in sla_rows[i:i + 500]) + ';')
sql.append('REPLACE INTO acore_world.skillraceclassinfo_dbc VALUES ' + ','.join('(' + ','.join(sv(v) for v in r) + ')' for r in newsrci) + ';')
TID = 900100
sql.append('DELETE FROM acore_world.trainer_spell WHERE TrainerId=%d;' % TID)
rows = ['(%d,%d,%d,%d,0,%d,%d,%d,%d,0)' % (TID, s, d['cost'], d['rsk'], d['a'][0], d['a'][1], d['a'][2], d['lvl']) for s, d in sorted(tr.items())]
for i in range(0, len(rows), 500):
    sql.append('INSERT INTO acore_world.trainer_spell (TrainerId,SpellId,MoneyCost,ReqSkillLine,ReqSkillRank,ReqAbility1,ReqAbility2,ReqAbility3,ReqLevel,VerifiedBuild) VALUES ' + ','.join(rows[i:i + 500]) + ';')

# ---------------- 3. the druid relearns from the Book ----------------
known = [int(x) for x in open('druid_spells2.txt').read().split()]
drop = [s for s in known if s in cmap and s in g['tr']]
sql.append('UPDATE acore_characters.characters SET at_login = at_login | 4 WHERE guid=6736;')
if drop:
    sql.append('DELETE FROM acore_characters.character_spell WHERE guid=6736 AND spell IN (%s);' % ','.join(map(str, drop)))
print('druid stock class spells to drop:', len(drop))

open('bronzebeard.sql', 'w', encoding='utf-8').write('START TRANSACTION;\n' + '\n'.join(sql) + '\nCOMMIT;\n')
pickle.dump(dict(edit=edit, newsrci=newsrci, orig=orig, tr=tr, owners=dict(owners)), open('gen2.pkl', 'wb'))
print('written bronzebeard.sql')
