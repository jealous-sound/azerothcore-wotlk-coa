# Player-ready copies of race models that lack texture slots the character code fills: skin (1), cape (2),
# hair (6), skin extra (8). Ascension's own player models have all four; a model missing several crashed the
# client (Naga as a player: 0x008CFF83; Murloc at character creation: null read in ResetCharCustomize after the
# hair texture). The copies <Model>Player.m2 append the missing slots (no mesh uses them).
#
# Run from the scratchpad after make_murloc_model.py:
#   python make_player_models.py --list      -> files to extract with mpqget.py into m2\
#   python make_player_models.py             -> out_races\model\<path>\<Model>Player.m2 (+ .skin, .anim)
import struct, os, sys

B = chr(92)
SLOTS = (1, 2, 6, 8)
# (race, ChrRaces field 4 = male / 5 = female) -> source model (client path, .m2)
MODELS = {
    (21, 4): r'Character\Naga_\Male\Naga_Male.m2',
    (21, 5): r'Character\Naga_\Female\Naga_Female.m2',
    (23, 4): r'Character\FelOrc\Male\FelOrcMale.m2',
    (26, 4): r'Character\Skeleton\Male\SkeletonMale.m2',
    (26, 5): r'Character\Skeleton\Female\SkeletonFemale.m2',
}


def player_path(path):
    return path[:-3] + 'Player.m2'


def local(path, root='m2'):
    return os.path.join(root, *path.lower().split(B))


def companions(d, path):
    """skin and external animation files of a model, as client paths"""
    stem = path[:-3]
    files = ['%s%02d.skin' % (stem, v) for v in range(struct.unpack_from('<I', d, 0x44)[0])]
    n_seq, o_seq = struct.unpack_from('<II', d, 0x1C)
    for i in range(n_seq):
        anim, sub = struct.unpack_from('<HH', d, o_seq + i * 64)
        if not struct.unpack_from('<I', d, o_seq + i * 64 + 12)[0] & 0x20:
            files.append('%s%04d-%02d.anim' % (stem, anim, sub))
    return files


def add_slots(d):
    d = bytearray(d)
    n_tex, ofs_tex = struct.unpack_from('<II', d, 0x50)
    have = {struct.unpack_from('<I', d, ofs_tex + 16 * i)[0] for i in range(n_tex)}
    new = [t for t in SLOTS if t not in have]
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


if __name__ == '__main__':
    if '--list' in sys.argv:
        for path in MODELS.values():
            print(path)
            if os.path.exists(local(path)):
                print('\n'.join(companions(open(local(path), 'rb').read(), path)))
        sys.exit()
    for path in MODELS.values():
        data = open(local(path), 'rb').read()
        out, added = add_slots(data)
        new_path = player_path(path)
        dst = os.path.join('out_races/model', *new_path.split(B))
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        open(dst, 'wb').write(out)
        stem, new_stem = path[:-3], new_path[:-3]
        for f in companions(data, path):
            if not os.path.exists(local(f)):
                continue                                 # sequence without its own file
            target = os.path.join('out_races/model', *(new_stem + f[len(stem):]).split(B))
            open(target, 'wb').write(open(local(f), 'rb').read())
        print(new_path, 'added slots', added)
