import struct, collections, pickle
def load(p):
    d=open(p,'rb').read(); _,n,fc,rs,ss=struct.unpack('<4s4I',d[:20])
    return [list(struct.unpack_from('<%dI'%fc,d,20+i*rs)) for i in range(n)]
D='C:/CoA-Repack/Data/dbc/'
sla=load(D+'SkillLineAbility.dbc'); srci=load(D+'SkillRaceClassInfo.dbc')
n=pickle.load(open('spellnames.pkl','rb')); eff=pickle.load(open('spelleff.pkl','rb'))
VAN=[1,2,3,4,5,6,7,8,9,11]; bit=lambda c:1<<(c-1); VMASK=sum(bit(c) for c in VAN)
OFF=1100000; ALL=0xFFFFFFFF
# ---- 1. combined trainer rows: class trainers + class quest spells
rows=[[int(x) for x in l.split('\t')] for l in open('trainer_spells.txt').read().strip().splitlines()]
tr={}  # spell -> dict
owners=collections.defaultdict(int)
for cls,tid,s,lvl,rsk,a1,a2,a3,cost in rows:
    if cls not in VAN: continue
    owners[s]|=bit(cls)
    cur=tr.get(s)
    if not cur or lvl<cur['lvl']: tr[s]=dict(lvl=lvl,cost=cost,rsk=rsk,a=(a1,a2,a3))
for l in open('class_quests.txt').read().strip().splitlines():
    qid,cm,rs,rds,minl,ql=map(int,l.split('\t'))
    cls=[c for c in VAN if cm&bit(c)]
    if not cls or len(cls)>2: continue
    e=eff.get(rs or rds)
    if not e: continue
    for k,t in zip(*e):
        if k!=36 or not t: continue
        s=t-OFF if t>=OFF and (t-OFF) in n else t
        if s not in n: continue
        for c in cls: owners[s]|=bit(c)
        lvl=max(minl,1)
        if s not in tr or lvl<tr[s]['lvl']: tr[s]=dict(lvl=lvl,cost=0,rsk=0,a=(0,0,0))
# ---- 1b. prerequisites nobody teaches (rank 1 starting spells): add them free at level 1
import collections as _c
prev={}
_r=_c.defaultdict(list)
for l in open('spell_ranks.txt').read().splitlines():
    if l.strip():
        f,s_,r_=map(int,l.split()); _r[f].append((r_,s_))
for f,lst in _r.items():
    lst.sort()
    for (a1,b1),(a2,b2) in zip(lst,lst[1:]): prev[b2]=b1
talentspells=set()
for _t in load(D+'Talent.dbc'):
    for x in _t[4:13]:
        if not x: continue
        for y in (x, x-OFF if x>=OFF else x):
            talentspells.add(y)
            e_=eff.get(x)
            if e_:
                for k_,t_ in zip(*e_):
                    if k_==36 and t_: talentspells.add(t_-OFF if t_>=OFF else t_)
lv=pickle.load(open('spelllvl.pkl','rb'))
added=[]
changed=True
while changed:
    changed=False
    for s,d in list(tr.items()):
        needs=[a for a in d['a'] if a]+([prev[s]] if s in prev else [])
        for a in needs:
            if a not in tr and a in n and a not in talentspells:
                lvl_=20 if a==33388 else max(1,lv.get(a,(0,0,1))[2])
                tr[a]=dict(lvl=lvl_,cost=0,rsk=0,a=(0,0,0)); owners[a]|=owners[s]; added.append(a); changed=True
            elif a in tr and not owners[a]&owners[s]:
                owners[a]|=owners[s]
print('added starting/prerequisite spells:',len(added))
for a in added[:60]: print('   ',a,n[a])
# ---- 2. SLA edits so that fit(spell,c) <=> c owns it (for every trainer spell); copies give class to stock spells
bysp=collections.defaultdict(list)
for r in sla: bysp[r[2]].append(r)
edit={}
def rec(r): return edit.setdefault(r[0],list(r))
for r in sla:   # stock spell inherits the vanilla classes of its Ascension copy
    if OFF<=r[2]<OFF+1000000 and r[4]&VMASK:
        for s in bysp.get(r[2]-OFF,[]):
            e=rec(s); e[4]=(e[4] or 0)|(r[4]&VMASK) if e[4] else e[4]
srci_by=collections.defaultdict(list)
for r in srci: srci_by[r[1]].append(r)
newsrci=[]; nid=max(r[0] for r in srci)+1
def srci_allows(skill,c):
    return any(not(r[3] and not r[3]&bit(c)) and not(r[2] and False) for r in srci_by.get(skill,[]))
def ensure_srci(skill,mask):
    global nid
    need=[c for c in VAN if mask&bit(c) and not srci_allows(skill,c)]
    if not need: return
    base=(srci_by.get(skill) or [None])[0]
    nr=list(base) if base else [0,skill,0,0,0,0,0,0]
    nr[0]=nid; nid+=1; nr[1]=skill; nr[2]=0; nr[3]=sum(bit(c) for c in need)
    newsrci.append(nr); srci_by[skill].append(nr)
# main skill line per class (most common skill of its own trainer spells)
mainskill={}
for c in VAN:
    cnt=collections.Counter(r[1] for s,m in owners.items() if m==bit(c) for r in bysp.get(s,[]) if r[1]<11000)
    mainskill[c]=cnt.most_common(1)[0][0] if cnt else 0
newsla=[]; slaid=max(r[0] for r in sla)+1
for s,om in owners.items():
    recs=bysp.get(s,[])
    if recs:
        for r in recs:
            e=rec(r); m=e[4]
            e[4]=((ALL if m==0 else m) & ~VMASK) | om
            ensure_srci(e[1],om)
    else:
        c=next(c for c in VAN if om&bit(c))
        nr=[slaid,mainskill[c],s,0,om,0,0,0,0,0,0,0,0,0]; slaid+=1
        newsla.append(nr); bysp[s].append(nr); ensure_srci(nr[1],om)
# ---- 3. verify
allsla={r[0]:r for r in sla}; allsla.update(edit)
for r in newsla: allsla[r[0]]=r
b2=collections.defaultdict(list)
for r in allsla.values(): b2[r[2]].append(r)
def fit(s,c):
    b=b2.get(s)
    if not b: return True
    return any((not r[4] or r[4]&bit(c)) and (not r[3]) and srci_allows(r[1],c) for r in b) or \
           any((not r[4] or r[4]&bit(c)) and r[3] and srci_allows(r[1],c) for r in b)
errs=[(s,c) for s,om in owners.items() for c in VAN if fit(s,c)!=bool(om&bit(c))]
print('trainer spells',len(tr),'SLA edits',len(edit),'new SLA',len(newsla),'new SRCI',len(newsrci),'mismatches',len(errs))
for s,c in errs[:15]: print('  mismatch',s,n.get(s),'class',c,'owners',bin(owners[s]))
changed=[e for k,e in edit.items() if e!=next(r for r in sla if r[0]==k)] if False else None
pickle.dump(dict(tr=tr,owners=dict(owners),edit=edit,newsla=newsla,newsrci=newsrci,orig={r[0]:r for r in sla}),open('gen.pkl','wb'))
