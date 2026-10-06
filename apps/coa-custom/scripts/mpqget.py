# usage: python mpqget.py outdir name1 name2 ...  (names case-insensitive, backslash paths). Later archives win.
import mpyq, glob, sys, os, re
B=chr(92)
out=sys.argv[1]; want={n.lower():n for n in sys.argv[2:]}
arcs=sorted(glob.glob(r'C:\Ascension Local\Data\*.MPQ'))+sorted(glob.glob(r'C:\Ascension Local\Data\*\*.MPQ'))
def order(p):
    n=p.lower().split(B)[-1]; loc=B+'data'+B not in p.lower().rsplit(B,1)[0]+B
    m=re.match(r'patch(?:-[a-z]{4})?(?:-(\w+))?\.mpq',n)
    if not m: return (0,loc,n)
    s=m.group(1) or ''
    return (1, 0 if s=='' else 1 if s.isdigit() else 2, s, loc)
for p in sorted(arcs,key=order):
    try: a=mpyq.MPQArchive(p, listfile=True)
    except Exception as e: continue
    for f in a.files or []:
        fl=f.decode(errors='replace').lower()
        if fl in want:
            try: d=a.read_file(f.decode())
            except Exception: d=None
            if d:
                dst=os.path.join(out,fl.replace(B,'/')); os.makedirs(os.path.dirname(dst),exist_ok=True)
                open(dst,'wb').write(d); print(p.split(B)[-1], fl, len(d))
