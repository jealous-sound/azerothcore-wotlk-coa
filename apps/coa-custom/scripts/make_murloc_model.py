# Murloc players come in two bodies, picked with the Male / Female button at character creation:
#   * male   - the Whim murloc (Creature\WhimMurloc: armored, jump and the other modern animations).
#              Skin Color = one of the 5 Whim bodies, "Armor" (the race's hair style option) = none or armor 1-4.
#   * female - the classic murloc (Creature\Murloc), 18 skins (9 classic + 9 recolours of the green one).
# In game the server swaps in the creature display of the (skin, armor) picked (custom_race_display,
# gen_race_looks.py); Ascension's Whim displays pick one armor set through CreatureDisplayInfoGeosetData, so the
# copies get those rows too (gen_races.py). The character screens paint the body on a copy of the model whose skin
# slot is a character skin (type 1):
#   * WhimPlayer.m2  - the 4 armor sets are hair geosets 1-4 drawn with the hair texture slot (type 6), so the
#                      "Armor" option swaps them like hair, textured by the hair row (the Whim armor texture);
#   * MurlocPlayer.m2 - the classic murloc.
# In game the male wears WhimGame.m2 and the female MurlocJump.m2: copies with the character Back / Helm attachment
# points added (cosmetic wings and other back auras attach there).
# The classic murloc has no jump animation: its animation lookup sends JumpStart / Jump / JumpEnd to existing moves
# (battle roar, swim idle, unarmed strike) - in the character-screen copy and in MurlocJump.m2, its in-game model.
# Painted skins (WhimPlayerSkinNN / MurlocPlayerSkinNN.blp) are palettized BLP2 512x512, what the body painter reads.
# Input: python make_murloc_model.py --list | mpqget.py into m2\. Run from the scratchpad
#   -> out_races\model\Creature\{WhimMurloc,Murloc}\..., out_races\murloc_skins.json
import struct, os, sys, json
import numpy as np
from PIL import Image
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from make_player_models import add_slots, companions

B = chr(92)
WHIM_MODEL = 'Creature' + B + 'WhimMurloc' + B + 'WhimMurloc.m2'
CLASSIC_MODEL = 'Creature' + B + 'Murloc' + B + 'Murloc.m2'
WHIM_BODIES = ['whim_murloc_redwithe', 'whim_murloc_purple', 'whim_murloc_green', 'whim_murloc_yellow', 'whim_murloc_redblack']
WHIM_ARMORS = ['whim_armor01', 'whim_armor02', 'whim_armor03', 'whim_armor04']       # armor N = geoset 10N in game
CLASSIC = ['sahauginskin', 'sahauginskinblue', 'sahauginskingray', 'sahauginskinorange', 'sahauginskinwhite',
           'sahauginskinyellowblack', 'sahauginskinbluepurple', 'sahauginskin_custom_pearl',
           'murloc2__custom_bloodiedwhite']
# recolours of the green murloc: (name, hue shift in degrees, saturation factor, brightness factor)
RECOLOURS = [('red', 265, 1.1, 1.0), ('pink', 235, 0.75, 1.15), ('purple', 185, 1.0, 0.95), ('teal', 80, 1.0, 1.0),
             ('gold', 310, 1.2, 1.1), ('crimson', 255, 1.3, 0.7), ('shadow', 0, 0.25, 0.45), ('ice', 105, 0.45, 1.3),
             ('toxic', 340, 1.6, 1.25)]               # the green skin sits near hue 95: shift = target hue - 95
# classic murloc: animation id -> animation id it plays instead (no jump animation in the model)
JUMP_STAND_INS = {37: 57, 38: 41, 39: 16}             # JumpStart -> battle roar, Jump -> swim idle, JumpEnd -> strike
# missing character attachment point -> murloc point it copies (Back <- ChestBloodBack, Helm <- Head)
BACK_ATTACHMENTS = {12: 16, 11: 20}


def local(path):
    return os.path.join('m2', *path.lower().split(B))


def texture_files():
    return (['creature' + B + 'whimmurloc' + B + n + '.blp' for n in WHIM_BODIES + WHIM_ARMORS] +
            ['creature' + B + 'murloc' + B + n + '.blp' for n in CLASSIC])


def recolour(img, hue, sat, val):
    """recolour the green body: hues near the body green (~95 deg) are rotated in HSV - fins, belly and eyes keep
    their colour (blending shifted and original colours in RGB turned pale greens grey-beige)"""
    alpha = np.asarray(img.convert('RGBA'))[..., 3]
    hsv = np.asarray(img.convert('RGB').convert('HSV')).astype(np.float32)
    distance = np.abs(((hsv[..., 0] * 360.0 / 255.0) - 95.0 + 180.0) % 360.0 - 180.0)
    weight = np.clip((58.0 - distance) / 4.0, 0.0, 1.0)      # whole body (hue 37..153), sharp edge: no fringes
    out = hsv.copy()
    out[..., 0] = (hsv[..., 0] + weight * hue * 255.0 / 360.0) % 256
    out[..., 1] = np.clip(hsv[..., 1] * (1 + weight * (sat - 1)), 0, 255)
    out[..., 2] = np.clip(hsv[..., 2] * (1 + weight * (val - 1)), 0, 255)
    rgb = np.asarray(Image.fromarray(out.astype(np.uint8), 'HSV').convert('RGB'))
    return Image.fromarray(np.dstack([rgb, alpha]), 'RGBA')


def encode_palettized(img):
    """BLP2, format 1 (8-bit palette + 8-bit alpha plane when needed), full mip chain - as stock character textures"""
    img = img.convert('RGBA')
    has_alpha = np.asarray(img)[..., 3].min() < 255
    pal_img = img.convert('RGB').quantize(256, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)
    palette = (pal_img.getpalette()[:768] + [0] * 768)[:768]
    mips, level = [], img
    while True:
        data = np.asarray(level.convert('RGB').quantize(palette=pal_img, dither=Image.Dither.NONE), dtype=np.uint8).tobytes()
        if has_alpha:
            data += np.asarray(level, dtype=np.uint8)[..., 3].tobytes()
        mips.append(data)
        if level.size == (1, 1) or len(mips) == 16:
            break
        level = level.resize((max(1, level.size[0] // 2), max(1, level.size[1] // 2)), Image.LANCZOS)
    header = struct.pack('<4sIBBBBII', b'BLP2', 1, 1, 8 if has_alpha else 0, 8, 1, *img.size)
    offs, sizes, pos = [], [], 1172
    for m in mips:
        offs.append(pos); sizes.append(len(m)); pos += len(m)
    offs += [0] * (16 - len(offs)); sizes += [0] * (16 - len(sizes))
    bgra = b''.join(struct.pack('<4B', palette[3 * i + 2], palette[3 * i + 1], palette[3 * i], 0) for i in range(256))
    return header + struct.pack('<16I16I', *offs, *sizes) + bgra + b''.join(mips)


# character texture regions (512 layout) the face / underwear sections paint over the body skin: the murloc sections
# get crops of their own skin, an empty section was painted with a default human texture (tan head, hands and feet)
SECTION_CROPS = {'FaceLower': (0, 384, 256, 128), 'FaceUpper': (0, 320, 256, 64),
                 'Pelvis': (256, 192, 256, 128), 'Torso': (256, 0, 256, 128)}


def write_section_crops(img, dst, name):
    img = img.convert('RGBA').resize((512, 512), Image.LANCZOS)
    for part, (x, y, w, h) in SECTION_CROPS.items():
        open(dst + name + '_' + part + '.blp', 'wb').write(encode_palettized(img.crop((x, y, x + w, y + h))))


def write_clear_scalp(dst):
    """hair rows paint a scalp over the face regions; an empty one was painted with a default human scalp (tan head
    and fins): fully transparent scalp textures paint nothing"""
    for part, size in (('Lower', (256, 128)), ('Upper', (256, 64))):
        open(dst + 'MurlocScalp' + part + '.blp', 'wb').write(encode_palettized(Image.new('RGBA', size, (0, 0, 0, 0))))


def retype_slots(d, mapping):
    """texture slot type -> new type (11 creature skin -> 1 painted character skin, 12 -> 6 hair); index by new type"""
    n_tex, ofs_tex = struct.unpack_from('<II', d, 0x50)
    found = {}
    for i in range(n_tex):
        t = struct.unpack_from('<I', d, ofs_tex + 16 * i)[0]
        if t in mapping:
            struct.pack_into('<I', d, ofs_tex + 16 * i, mapping[t])
            found[mapping[t]] = i
    n_rep, ofs_rep = struct.unpack_from('<II', d, 0x68)
    for k in range(n_rep):
        if k in mapping:
            struct.pack_into('<h', d, ofs_rep + 2 * k, -1)
        if k in found:
            struct.pack_into('<h', d, ofs_rep + 2 * k, found[k])
    return found


def add_jump_stand_ins(d):
    """animation lookup: point the missing jump animations at existing sequences"""
    n_seq, ofs_seq = struct.unpack_from('<II', d, 0x1C)
    first = {}
    for i in range(n_seq):
        first.setdefault(struct.unpack_from('<H', d, ofs_seq + 64 * i)[0], i)
    n_lk, ofs_lk = struct.unpack_from('<II', d, 0x24)
    done = {}
    for anim, stand_in in JUMP_STAND_INS.items():
        if anim < n_lk and anim not in first and stand_in in first:
            struct.pack_into('<h', d, ofs_lk + 2 * anim, first[stand_in])
            done[anim] = stand_in
    return done


def add_attachments(d):
    """character attachment points the murlocs lack, for cosmetic auras on players (the Holo-Wings attach to Back):
    copies of the closest murloc point, appended after the model with the attachment array moved there"""
    d = bytearray(d)
    n, ofs = struct.unpack_from('<II', d, 240)
    entries = [bytes(d[ofs + 40 * i:ofs + 40 * (i + 1)]) for i in range(n)]
    ids = [struct.unpack_from('<I', e)[0] for e in entries]
    added = []
    for new_id, like in BACK_ATTACHMENTS.items():
        if new_id not in ids and like in ids:
            e = bytearray(entries[ids.index(like)])
            struct.pack_into('<I', e, 0, new_id)
            entries.append(bytes(e))
            ids.append(new_id)
            added.append(new_id)
    if not added:
        return bytes(d), added
    while len(d) % 16:
        d.append(0)
    new_ofs = len(d)
    d += b''.join(entries)
    struct.pack_into('<II', d, 240, len(entries), new_ofs)
    n_lk, ofs_lk = struct.unpack_from('<II', d, 248)
    for i, a in enumerate(ids):
        if a < n_lk:
            struct.pack_into('<h', d, ofs_lk + 2 * a, i)
    return bytes(d), added


def write_model(d, skins, src_path, dst_dir, name, extra_slots=True):
    out, added = add_slots(bytes(d)) if extra_slots else (bytes(d), [])
    open(dst_dir + name + '.m2', 'wb').write(out)
    for i, s in enumerate(skins):
        open(dst_dir + name + '%02d.skin' % i, 'wb').write(bytes(s))
    stem = os.path.basename(src_path)[:-3]
    for f in companions(bytes(d), src_path)[len(skins):]:              # external animations
        if os.path.exists(local(f)):
            open(dst_dir + name + os.path.basename(f)[len(stem):], 'wb').write(open(local(f), 'rb').read())
    return added


def make_whim():
    dst = 'out_races/model/Creature/WhimMurloc/'
    os.makedirs(dst, exist_ok=True)
    src = local(WHIM_MODEL)
    d = bytearray(open(src, 'rb').read())
    s = bytearray(open(src[:-3] + '00.skin', 'rb').read())
    retype_slots(d, {11: 1, 12: 6})                                   # body painted, armor = hair slot
    n_sub, ofs_sub = struct.unpack_from('<II', s, 28)
    for i in range(n_sub):
        sid = struct.unpack_from('<H', s, ofs_sub + 48 * i)[0]
        if 101 <= sid <= 104:
            new = sid - 100                                           # armor N -> hair geoset N
        elif sid == 201:
            new = 0                                                   # fins: the variant every look uses in game
        elif sid // 100 == 2:
            new = 299                                                 # other fin variants: never shown
        else:
            continue
        struct.pack_into('<H', s, ofs_sub + 48 * i, new)
    game, points = add_attachments(open(src, 'rb').read())          # in game: the Whim murloc + Back point
    write_model(game, [open(src[:-3] + '00.skin', 'rb').read()], WHIM_MODEL, dst, 'WhimGame', extra_slots=False)
    print('Whim in game: attachment points added %s' % points)
    added = write_model(d, [s], WHIM_MODEL, dst, 'WhimPlayer')
    names = []
    for i, body in enumerate(WHIM_BODIES):
        name = 'WhimPlayerSkin%02d' % i
        img = Image.open(os.path.dirname(src) + '/' + body + '.blp').convert('RGBA').resize((512, 512), Image.LANCZOS)
        open(dst + name + '.blp', 'wb').write(encode_palettized(img))
        write_section_crops(img, dst, name)
        names.append(name)
    write_clear_scalp(dst)
    print('Whim (male): %d bodies x %d armors, added slots %s' % (len(names), len(WHIM_ARMORS), added))
    return names


def make_classic():
    dst = 'out_races/model/Creature/Murloc/'
    os.makedirs(dst, exist_ok=True)
    src = local(CLASSIC_MODEL)
    src_dir = os.path.dirname(src) + '/'
    green = Image.open(src_dir + 'sahauginskin.blp').convert('RGBA')
    skins = [Image.open(src_dir + name + '.blp').convert('RGBA') for name in CLASSIC]
    skins += [recolour(green, h, s, v) for _, h, s, v in RECOLOURS]
    names = []
    for i, img in enumerate(skins):
        name = 'MurlocPlayerSkin%02d' % i
        open(dst + name + '.blp', 'wb').write(encode_palettized(img.resize((512, 512), Image.LANCZOS)))
        write_section_crops(img, dst, name)
        names.append(name)
    views = struct.unpack_from('<I', open(src, 'rb').read(), 0x44)[0]
    skin_files = [open(src[:-3] + '%02d.skin' % v, 'rb').read() for v in range(views)]
    game = bytearray(open(src, 'rb').read())                         # in game: the murloc + jump stand-ins
    jumps = add_jump_stand_ins(game)
    game, points = add_attachments(game)                             # + Back point (wings)
    print('classic in game: attachment points added %s' % points)
    write_model(game, skin_files, CLASSIC_MODEL, dst, 'MurlocJump', extra_slots=False)
    glue = bytearray(open(src, 'rb').read())                         # character screens: painted skin + jump
    retype_slots(glue, {11: 1})
    add_jump_stand_ins(glue)
    added = write_model(glue, skin_files, CLASSIC_MODEL, dst, 'MurlocPlayer')
    write_clear_scalp(dst)
    print('classic (female): %d skins, jump stand-ins %s, added slots %s' % (len(names), jumps, added))
    return names


if __name__ == '__main__':
    if '--list' in sys.argv:
        files = [WHIM_MODEL, CLASSIC_MODEL] + texture_files()
        for model in (WHIM_MODEL, CLASSIC_MODEL):
            if os.path.exists(local(model)):
                files += companions(open(local(model), 'rb').read(), model)
        print('\n'.join(files))
        sys.exit()
    import shutil
    for folder in ('out_races/model/Creature/WhimMurloc', 'out_races/model/Creature/Murloc'):
        shutil.rmtree(folder, ignore_errors=True)
    json.dump({'male': {'skins': make_whim(), 'bodies': WHIM_BODIES, 'armors': WHIM_ARMORS},
               'female': {'skins': make_classic(), 'looks': CLASSIC + [r[0] for r in RECOLOURS]}},
              open('out_races/murloc_skins.json', 'w'), indent=1)
