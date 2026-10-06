# Player-ready copies of the Naga models: Character\Naga_\{Male,Female}\Naga_{Male,Female}Player.m2.
# Dressed as a player (CreatureModelData flag 0x800) the original Naga crashed the client (0x008CFF83, also at
# character select). It is the only race model without a cape (type 2) and a hair (type 6) texture slot; every
# model that works as a player has both. The copies append those two slots (no mesh uses them).
# Input (extracted with mpqget.py into m2\): naga_<sex>.m2, naga_<sex>00-03.skin, naga_<sex>0060-00.anim,
# naga_<sex>0062-00.anim. Run from the scratchpad: python make_naga_model.py -> out_races\model\Character\Naga_\...
import struct, os, glob

def add_slots(d, types):
    d = bytearray(d)
    n_tex, ofs_tex = struct.unpack_from('<II', d, 0x50)
    have = {struct.unpack_from('<I', d, ofs_tex + 16 * i)[0] for i in range(n_tex)}
    new = [t for t in types if t not in have]
    if not new:
        return bytes(d), []
    d.extend(bytes(-len(d) % 16))
    empty_name = len(d)
    d.extend(bytes(16))                                  # "" filename
    ofs_new = len(d)
    d.extend(d[ofs_tex:ofs_tex + 16 * n_tex])
    for t in new:
        d.extend(struct.pack('<IIII', t, 0, 1, empty_name))
    struct.pack_into('<II', d, 0x50, n_tex + len(new), ofs_new)
    return bytes(d), new

for sex, name in (('male', 'Male'), ('female', 'Female')):
    src = 'm2/character/naga_/%s/' % sex
    dst = 'out_races/model/Character/Naga_/%s/' % name
    os.makedirs(dst, exist_ok=True)
    m2, added = add_slots(open(src + 'naga_%s.m2' % sex, 'rb').read(), (2, 6))
    base = 'Naga_%sPlayer' % name
    open(dst + base + '.m2', 'wb').write(m2)
    for f in glob.glob(src + 'naga_%s*.skin' % sex) + glob.glob(src + 'naga_%s*.anim' % sex):
        suffix = os.path.basename(f)[len('naga_%s' % sex):]
        open(dst + base + suffix, 'wb').write(open(f, 'rb').read())
    print(base, 'added texture slots', added, 'files', sorted(os.listdir(dst)))
