import struct, mpyq
# --- CharBaseInfo: every race x every class ---
races=[1,2,3,4,5,6,7,8,10,11]
recs=b''.join(bytes([r,c]) for c in range(1,33) if c not in (6,10) for r in races)
cbi=struct.pack('<4s4I',b'WDBC',len(recs)//2,2,2,0)+recs
# --- MPQ crypto ---
ct=[0]*0x500; seed=0x00100001
for i in range(0x100):
    idx=i
    for j in range(5):
        seed=(seed*125+3)%0x2AAAAB; t1=(seed&0xFFFF)<<16
        seed=(seed*125+3)%0x2AAAAB; t2=seed&0xFFFF
        ct[idx]=t1|t2; idx+=0x100
def h(s,t):
    s1,s2=0x7FED7FED,0xEEEEEEEE
    for ch in s.upper():
        v=ord(ch); s1=ct[(t<<8)+v]^((s1+s2)&0xFFFFFFFF); s2=(v+s1+s2+(s2<<5)+3)&0xFFFFFFFF
    return s1
def enc(data,key):
    s=0xEEEEEEEE; out=[]
    for (v,) in struct.iter_unpack('<I',data):
        s=(s+ct[0x400+(key&0xFF)])&0xFFFFFFFF
        out.append(v^((key+s)&0xFFFFFFFF))
        key=((~key<<0x15)+0x11111111 | key>>0x0B)&0xFFFFFFFF
        s=(v+s+(s<<5)+3)&0xFFFFFFFF
    return struct.pack('<%dI'%len(out),*out)
old=mpyq.MPQArchive(r'C:\Ascension Local\Data\patch-T.MPQ',listfile=True)
lua=open('out/Interface/GlueXML/CharacterCreate.lua','rb').read()
files={r'DBFilesClient\CharBaseInfo.dbc':open('out_races/CharBaseInfo.dbc','rb').read(),
       r'DBFilesClient\Spell.dbc':open('out_Spell.dbc','rb').read(),
       r'Interface\GlueXML\CharacterCreate.lua':lua}
S=chr(92)
for rel in ['Interface/GlueXML/GlueParent.lua','Interface/AddOns/Blizzard_TalentUI/Blizzard_TalentUI.lua','Interface/AddOns/Blizzard_TalentUI/Blizzard_TalentUI.xml','Interface/FrameXML/UIParent.lua',
            'Interface/FrameXML/DressUpFrame.lua','Interface/FrameXML/Util/AppearanceUtil.lua','Interface/FrameXML/MainMenuBarMicroButtons.lua']:
    files[rel.replace('/',S)]=open('out/'+rel,'rb').read()
files['DBFilesClient'+S+'Talent.dbc']=open('out_races/client_Talent.dbc','rb').read()
files['DBFilesClient'+S+'SkillLineAbility.dbc']=open('out_races/client_SkillLineAbility.dbc','rb').read()
files['DBFilesClient'+S+'SkillRaceClassInfo.dbc']=open('out_races/client_SkillRaceClassInfo.dbc','rb').read()
files['DBFilesClient'+S+'AppearanceCategories.dbc']=open('out_AppearanceCategories.dbc','rb').read()
files['DBFilesClient'+S+'ChrRaces.dbc']=open('out_races/client_ChrRaces.dbc','rb').read()   # glue models for old NPC races
files['DBFilesClient'+S+'CharSections.dbc']=open('out_races/CharSections.dbc','rb').read()
files['DBFilesClient'+S+'HDCharSections.dbc']=open('out_races/HDCharSections.dbc','rb').read()   # HD textures for the renumbered races
files['DBFilesClient'+S+'CreatureDisplayInfoGeosetData.dbc']=open('out_races/CreatureDisplayInfoGeosetData.dbc','rb').read()   # Whim murloc armor sets
files['DBFilesClient'+S+'CharHairGeosets.dbc']=open('out_races/CharHairGeosets.dbc','rb').read()
files['DBFilesClient'+S+'CharacterFacialHairStyles.dbc']=open('out_races/CharacterFacialHairStyles.dbc','rb').read()
files['DBFilesClient'+S+'NameGen.dbc']=open('out_races/NameGen.dbc','rb').read()
files['DBFilesClient'+S+'EmotesTextSound.dbc']=open('out_races/EmotesTextSound.dbc','rb').read()
files['DBFilesClient'+S+'UICameraAppearanceChrRaces.dbc']=open('out_races/UICameraAppearanceChrRaces.dbc','rb').read()
files['DBFilesClient'+S+'CharStartOutfit.dbc']=open('out_races/CharStartOutfit.dbc','rb').read()
for n in ('ClassDetails','ArchetypeDetails','ShapeshiftDetails','PetDetails'):   # creation previews of CoA classes / Hero / forms / pets
    files['DBFilesClient'+S+'CharacterCreation'+n+'.dbc']=open('out_races/CharacterCreation'+n+'.dbc','rb').read()
files['DBFilesClient'+S+'Faction.dbc']=open('out_races/Faction.dbc','rb').read()
files['DBFilesClient'+S+'CreatureModelData.dbc']=open('out_races/CreatureModelData.dbc','rb').read()
files['DBFilesClient'+S+'CreatureDisplayInfo.dbc']=open('out_races/CreatureDisplayInfo.dbc','rb').read()
import os
for root,_,names in os.walk('out_races/model'):          # character-screen murloc model (make_murloc_model.py)
    for fn in names:
        full=os.path.join(root,fn)
        files[os.path.relpath(full,'out_races/model').replace('/',S)]=open(full,'rb').read()
files['(listfile)']=('\r\n'.join(k for k in files)+'\r\n').encode()
HS=1
while HS < 2*len(files): HS*=2      # hash table: power of two, at least twice the file count
body=b''; blocks=[]; off=32
for name,data in files.items():
    blocks.append((off+len(body),len(data),len(data),0x81000000)); body+=data
ht=[(0xFFFFFFFF,0xFFFFFFFF,0xFFFF,0xFFFF,0xFFFFFFFF)]*HS
for i,name in enumerate(files):
    p=h(name,0)%HS
    while ht[p][4]!=0xFFFFFFFF: p=(p+1)%HS
    ht[p]=(h(name,1),h(name,2),0,0,i)
htb=enc(b''.join(struct.pack('<IIHHI',*e) for e in ht),h('(hash table)',3))
btb=enc(b''.join(struct.pack('<4I',*b) for b in blocks),h('(block table)',3))
hto=32+len(body); bto=hto+len(htb); size=bto+len(btb)
hdr=struct.pack('<4sIIHHIIII',b'MPQ\x1a',32,size,0,3,hto,bto,HS,len(blocks))
open('out/patch-T.MPQ','wb').write(hdr+body+htb+btb)
# verify
n=mpyq.MPQArchive('out/patch-T.MPQ',listfile=True)
for name,data in files.items():
    assert n.read_file(name)==data, name
print('verified', size, [f.decode() for f in n.files])
