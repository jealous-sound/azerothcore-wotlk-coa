import struct, pickle, os
def load_spells(p='C:/CoA-Repack/Data/dbc/Spell.dbc'):
    d=open(p,'rb').read(); _,n,fc,rs,ss=struct.unpack('<4s4I',d[:20])
    sb=d[20+n*rs:]
    out={}
    for i in range(n):
        o=20+i*rs; rec=struct.unpack_from('<%dI'%fc,d,o)
        nm=sb[rec[136]:sb.index(b'\0',rec[136])].decode(errors='replace')
        rk=sb[rec[153]:sb.index(b'\0',rec[153])].decode(errors='replace')
        out[rec[0]]=(nm,rk,rec)
    return out,fc
if __name__=='__main__':
    sp,fc=load_spells(); print(len(sp),fc)
    pickle.dump({k:(v[0],v[1]) for k,v in sp.items()},open('spellnames.pkl','wb'))
