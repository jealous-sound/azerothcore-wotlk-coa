# Checks every vanilla-class talent the client shows against what the server will accept.
import struct, collections, subprocess
def load(p):
    d=open(p,'rb').read(); n,fc,rs,sb=struct.unpack('<4I',d[4:20])
    return [struct.unpack_from('<%dI'%fc,d,20+i*rs) for i in range(n)]
VAN_TABS={41,61,81,161,163,164,181,182,183,201,202,203,261,262,263,281,282,283,301,302,303,361,362,363,381,382,383,398,399,400}
srv={r[0]:r for r in load('C:/CoA-Repack/Data/dbc/Talent.dbc')}
cli=[r for r in load('C:/CoA-Repack/Custom/server_dbc/client_Talent.dbc') if r[1] in VAN_TABS]
spells=set(r[0] for r in load('C:/CoA-Repack/Data/dbc/Spell.dbc'))
out=subprocess.run(['C:/CoA-Repack/mysql/bin/mysql.exe','--defaults-file=C:/CoA-Repack/mysql/admin-client.ini','-N','-e','SELECT ID FROM acore_world.spell_dbc'],capture_output=True,text=True).stdout
spells|=set(int(x) for x in out.split())
ranks=lambda r:[x for x in r[4:13] if x]
owner=collections.defaultdict(list)
for r in srv.values():
    for x in ranks(r): owner[x].append(r[0])
pos=collections.Counter((r[1],r[2],r[3]) for r in cli)
problems=collections.Counter()
for r in cli:
    tid=r[0]; s=srv.get(tid)
    def bad(msg): problems[msg]+=1; print(r[1],tid,msg)
    if not s: bad('not in server'); continue
    if s[1:4]!=r[1:4]: bad('position differs server %s client %s'%(s[1:4],r[1:4]))
    if ranks(s)!=ranks(r): bad('ranks differ')
    if s[13:19]!=r[13:19]: bad('prereq differs')
    for x in ranks(s):
        if x not in spells: bad('rank spell %d missing'%x)
        if len(owner[x])>1: bad('rank spell %d shared by %s'%(x,owner[x]))
    if pos[(r[1],r[2],r[3])]>1: bad('two talents at same spot')
    for k in range(3):
        dep=s[13+k]
        if not dep: continue
        d=srv.get(dep)
        if not d: bad('prereq %d missing'%dep); continue
        if d[1]!=s[1]: bad('prereq in other tab')
        if d[2]>s[2]: bad('prereq below')
        if s[16+k]>=len(ranks(d)): bad('prereq rank %d >= %d ranks'%(s[16+k],len(ranks(d))))
        if not any(c[0]==dep for c in cli): bad('prereq %d not shown in client'%dep)
    if s[2]>10: bad('row %d'%s[2])
print(len(cli),'client talents checked', dict(problems))
