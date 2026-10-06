# Vanilla classes: give a starting outfit to every playable race/class pair whose CharStartOutfit is empty.
import struct, collections
d=open('out_races/CharStartOutfit.dbc','rb').read(); _,n,fc,rs,ss=struct.unpack('<4s4I',d[:20])
outfit=collections.defaultdict(list)
for i in range(n):
    r=d[20+i*rs:20+(i+1)*rs]
    items=[x for x in struct.unpack_from('<24i',r,8) if x>0]
    if r[6]==0: outfit[(r[4],r[5])]=items
cr=open('out_races/ChrRaces.dbc','rb').read(); _,cn,cfc,crs,css=struct.unpack('<4s4I',cr[:20])
races=[struct.unpack_from('<%dI'%cfc,cr,20+i*crs) for i in range(cn)]
playable=[r[0] for r in races if not r[1]&1]
VAN=[1,2,3,4,5,6,7,8,9,11]
kit={}
for c in VAN:
    for r in (1,2,3,4,5,6,7,8,10,11):
        if outfit.get((r,c)): kit[c]=outfit[(r,c)]; break
kit.setdefault(11,[6123,6124,3661,6948,159,4536])     # stock druid kit: robe, pants, staff, hearthstone, water, apple
rows=[]
for race in playable:
    for c in VAN:
        if not outfit.get((race,c)):
            for it in kit[c]:
                rows.append((race,c,it,1))
sql=['USE acore_world;','DELETE FROM playercreateinfo_item WHERE Note = "vanilla starter kit";']
for i in range(0,len(rows),400):
    sql.append('INSERT IGNORE INTO playercreateinfo_item (race,class,itemid,amount,Note) VALUES '+','.join('(%d,%d,%d,%d,"vanilla starter kit")'%r for r in rows[i:i+400])+';')
open('out_races/outfits.sql','w').write('\n'.join(sql)+'\n')
print('pairs filled',len({(r[0],r[1]) for r in rows}),'rows',len(rows),'kits',{c:len(v) for c,v in kit.items()})
