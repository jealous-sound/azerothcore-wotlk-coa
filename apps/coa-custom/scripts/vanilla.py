import struct, collections, pickle
def load(p):
    d=open(p,'rb').read(); _,n,fc,rs,ss=struct.unpack('<4s4I',d[:20])
    return [list(struct.unpack_from('<%dI'%fc,d,20+i*rs)) for i in range(n)]
D='C:/CoA-Repack/Data/dbc/'
sla=load(D+'SkillLineAbility.dbc'); srci=load(D+'SkillRaceClassInfo.dbc')
VAN={1,2,3,4,5,6,7,8,9,11}; VMASK=sum(1<<(c-1) for c in VAN)
OFF=1100000
bysp=collections.defaultdict(list)
for r in sla: bysp[r[2]].append(r)
newsla={}; skillpairs=collections.defaultdict(int)
for r in sla:
    if OFF<=r[2]<OFF+1000000 and r[4]&VMASK:
        for s in bysp.get(r[2]-OFF,[]):
            cur=newsla.get(s[0],list(s))
            nm=cur[4]|(r[4]&VMASK)
            if nm!=cur[4] or s[0] in newsla:
                cur[4]=nm; newsla[s[0]]=cur
            skillpairs[(s[1],r[1])]|= r[4]&VMASK
# SRCI additions: stock skill gets the classes its copy skill allows
srci_by=collections.defaultdict(list)
for r in srci: srci_by[r[1]].append(r)
def classes_ok(skill,cls):
    for r in srci_by.get(skill,[]):
        if r[3] and not r[3]&(1<<(cls-1)): continue
        return True
    return False
nid=max(r[0] for r in srci)+1; newsrci=[]
for (stock,copy),mask in sorted(skillpairs.items()):
    need=[c for c in VAN if mask&(1<<(c-1)) and not classes_ok(stock,c)]
    if not need: continue
    tmpl=next((r for r in srci_by.get(copy,[]) if r[3]&mask), None) or (srci_by.get(stock) or [None])[0]
    if not tmpl: print('no template for skill',stock,copy); continue
    nr=list(tmpl); nr[0]=nid; nid+=1; nr[1]=stock; nr[2]=0; nr[3]=sum(1<<(c-1) for c in need)
    newsrci.append(nr)
print('SLA overrides',len(newsla),'SRCI new rows',len(newsrci),'skill pairs',len(skillpairs))
pickle.dump((newsla,newsrci),open('vanilla_dbc.pkl','wb'))
# simulate fit
sla2={r[0]:r for r in sla}; sla2.update(newsla)
srci2=srci+newsrci
bysp2=collections.defaultdict(list)
for r in sla2.values(): bysp2[r[2]].append(r)
sb=collections.defaultdict(list)
for r in srci2: sb[r[1]].append(r)
def srci_ok(skill,cls):
    return any(not(r[3] and not r[3]&(1<<(cls-1))) for r in sb.get(skill,[]))
def fit(sp,cls):
    b=bysp2.get(sp)
    if not b: return True
    return any((not r[4] or r[4]&(1<<(cls-1))) and srci_ok(r[1],cls) for r in b)
rows=[[int(x) for x in l.split('\t')] for l in open('trainer_spells.txt').read().strip().splitlines()]
n=pickle.load(open('spellnames.pkl','rb'))
for cls in sorted(VAN):
    own=[r for r in rows if r[0]==cls]; ownset={r[2] for r in own}
    ok=sum(1 for r in own if fit(r[2],cls))
    foreign=[r[2] for r in rows if r[0]!=cls and r[2] not in ownset and fit(r[2],cls)]
    bad=[ (r[2],n.get(r[2],('?',))[0]) for r in own if not fit(r[2],cls)][:6]
    print(f'class {cls:2}: own trainer spells fit {ok}/{len(own)}  other-class spells leaking {len(set(foreign))}  e.g. not-fit {bad}')
print('--- leaks')
for cls in (1,11):
    ownset={r[2] for r in rows if r[0]==cls}
    lk=sorted({(r[2],r[0]) for r in rows if r[0]!=cls and r[2] not in ownset and fit(r[2],cls)})
    print(cls,[(s,c,n.get(s,('?',))[0],'noSLA' if not bysp2.get(s) else 'sla') for s,c in lk])
