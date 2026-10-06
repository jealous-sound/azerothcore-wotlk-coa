# Bronzebeard talents (ids >= 100000, copies of stock talent id + 110000) keep their stock prerequisites
# (DependsOn = stock id), which a Bronzebeard player never has. Point them at the Bronzebeard copy.
#  server: Data\dbc\Talent.dbc (all talents kept)    client: patch-T Talent.dbc (vanilla tabs show copies only)
import struct, collections, os
ORIG = 'C:/CoA-Repack/Custom/backups/dbc_original/Talent.dbc'
OUT = 'out_races/'
d = open(ORIG, 'rb').read()
_, n, fc, rs, ss = struct.unpack('<4s4I', d[:20])
recs = [list(struct.unpack_from('<%dI' % fc, d, 20 + i * rs)) for i in range(n)]
sb = d[20 + n * rs:]
by_id = {r[0]: r for r in recs}
fixed = 0
for r in recs:
    if r[0] < 100000:
        continue
    for k in range(13, 16):
        dep = r[13 + (k - 13)]
        if dep and dep < 100000 and dep + 110000 in by_id and by_id[dep + 110000][1] == r[1]:
            r[k] = dep + 110000
            fixed += 1
# a required rank cannot exceed the ranks the prerequisite really has; a prerequisite that does not exist is dropped
for r in recs:
    for k in range(3):
        if r[0] >= 100000 and r[13 + k] and r[13 + k] not in by_id:
            r[13 + k] = 0
            r[16 + k] = 0
            fixed += 1
        dep = by_id.get(r[13 + k])
        if dep:
            ranks = sum(1 for x in dep[4:9] if x)
            if ranks and r[16 + k] > ranks - 1:
                r[16 + k] = ranks - 1
                fixed += 1
def save(path, rows):
    open(path, 'wb').write(struct.pack('<4s4I', b'WDBC', len(rows), fc, rs, len(sb)) + b''.join(struct.pack('<%dI' % fc, *x) for x in rows) + sb)
save(OUT + 'server_Talent.dbc', recs)
# client: vanilla tabs keep only the copies, clashing extras dropped, sorted by tab/row/column (client sends Learn in this order)
tabs = open('C:/CoA-Repack/Data/dbc/TalentTab.dbc', 'rb').read()
_, tn, tfc, trs, tss = struct.unpack('<4s4I', tabs[:20])
vt = {struct.unpack_from('<%dI' % tfc, tabs, 20 + i * trs)[0] for i in range(tn)
      if 0 < struct.unpack_from('<%dI' % tfc, tabs, 20 + i * trs)[20] < 2048 and not struct.unpack_from('<%dI' % tfc, tabs, 20 + i * trs)[21]}
client = [list(r) for r in recs if (r[1] not in vt or r[0] >= 100000) and r[0] not in (101848, 101324)]
ids = {r[0] for r in client}
for r in client:
    if r[1] in vt:
        for k in range(3):
            if r[13 + k] and r[13 + k] not in ids:
                r[13 + k] = 0; r[16 + k] = 0
client.sort(key=lambda r: (r[1], r[2], r[3], r[0]))
save(OUT + 'client_Talent.dbc', client)
print('prerequisites repointed to Bronzebeard talents:', fixed, '| client talents', len(client))
