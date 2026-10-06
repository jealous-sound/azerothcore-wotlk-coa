# Playable extra races for the CoA repack + Ascension client.
#
# The core's RaceMgr already reads playable races (and the Alliance/Horde race masks) from ChrRaces.dbc,
# so the work is data:
#   * client + server DBC: ChrRaces (playable, faction), CharBaseInfo (classes), CharSections / hair /
#     facial hair / NameGen / emote sounds / creation camera (copied for renumbered races),
#     CharStartOutfit, Faction / SkillRaceClassInfo / SkillLineAbility race masks
#   * world DB: playercreateinfo (+item, action), player_race_stats, ascension_custom_class_race,
#     race masks of quests, items, conditions, create-info skills/spells, spell_area, mail rewards.
#
# Race masks are 32 bits, so races above 32 in the client data are renumbered into free ids.
# Run from the scratchpad: python gen_races.py   -> writes out_races\*.dbc and out_races\races.sql
import struct, os, collections

D = 'C:/CoA-Repack/Data/dbc/'
ORIGINALS = 'C:/CoA-Repack/Custom/backups/dbc_original/'   # this script's outputs are installed into D, so read the originals
_open_dbc = open
def _orig(path):
    name = os.path.basename(path)
    if path.startswith(D) and os.path.exists(ORIGINALS + name):
        return ORIGINALS + name
    return path
OUT = 'out_races/'
os.makedirs(OUT, exist_ok=True)

A, H = 'A', 'H'
# race id: (source race in the client data, faction, display name or None, stats from race)
RACES = {
    9:  (9,  H, None,        7),   # Goblin
    12: (12, H, None,        8),   # Zandalari Troll
    13: (13, A, None,        4),   # Worgen
    14: (10, A, 'High Elf',  10),  # High Elf (Blood Elf model)
    15: (39, A, None,        6),   # Tuskarr
    16: (16, A, None,        3),   # Kul Tiran
    17: (41, H, None,        6),   # Taunka
    18: (38, A, None,        3),   # Vrykul
    19: (19, H, None,        7),   # Vulpera
    20: (20, A, None,        6),   # Pandaren (Alliance)
    21: (35, H, None,        4),   # Naga
    22: (36, A, None,        11),  # Broken
    23: (34, H, None,        2),   # Fel Orc
    24: (40, H, None,        8),   # Forest Troll
    25: (43, H, None,        8),   # Ice Troll
    26: (37, H, None,        5),   # Skeleton
    27: (62, A, None,        3),   # Earthen
    28: (44, H, None,        8),   # Drakkari Troll
    29: (20, H, None,        6),   # Pandaren (Horde)
    30: (46, A, 'Murloc',    7),   # Murloc (Alliance) - Troglodyte body, murloc transform in game
    31: (46, H, 'Murloc',    7),   # Murloc (Horde)
}
FACTION = {A: dict(faction=1, team=7, side=0), H: dict(faction=2, team=1, side=1)}
TEMPLATE = {A: 1, H: 2}                     # Human / Orc: start zone, outfit, create items
PLAYABLE_FLAGS = 0xC
bit = lambda r: 1 << (r - 1)
OLD_A = bit(1) | bit(3) | bit(4) | bit(7) | bit(11)
OLD_H = bit(2) | bit(5) | bit(6) | bit(8) | bit(10)
NEW_A = sum(bit(r) for r, v in RACES.items() if v[1] == A)
NEW_H = sum(bit(r) for r, v in RACES.items() if v[1] == H)
CLONE = {r: v[0] for r, v in RACES.items() if v[0] != r}           # renumbered / cloned races (model data)


# race whose racials / race-specific content each new race borrows (same faction)
RACIAL = {9: 2, 12: 8, 13: 4, 14: 10, 15: 3, 16: 1, 17: 6, 18: 1, 19: 8, 20: 11, 21: 5, 22: 11, 23: 2,
          24: 8, 25: 8, 26: 5, 27: 3, 28: 8, 29: 6, 30: 5, 31: 5}


RACIAL_SKILLS = {101, 124, 125, 126, 220, 733, 753, 754, 756, 760, 11125, 11760}


def expand(mask, racial=False):
    """Faction masks gain the new races of that faction; race-specific masks (and every racial skill row)
    gain the races that borrow from them."""
    m = mask & 0xFFFFFFFF
    if m in (0, 0xFFFFFFFF):
        return mask
    full_a = m & OLD_A == OLD_A
    full_h = m & OLD_H == OLD_H
    if full_a:
        m |= NEW_A
    if full_h:
        m |= NEW_H
    if racial or (not full_a and not full_h):
        for new, base in RACIAL.items():
            if m & bit(base):
                m |= bit(new)
    return m


class DBC:
    def __init__(self, path):
        d = open(_orig(path), 'rb').read()
        _, n, self.fc, self.rs, ss = struct.unpack('<4s4I', d[:20])
        self.raw = [d[20 + i * self.rs:20 + (i + 1) * self.rs] for i in range(n)]
        self.sb = bytearray(d[20 + n * self.rs:])
        self.words = self.rs // 4 == self.fc

    def recs(self):
        return [list(struct.unpack('<%dI' % (self.rs // 4), r)) for r in self.raw]

    def set_recs(self, recs):
        self.raw = [struct.pack('<%dI' % (self.rs // 4), *[v & 0xFFFFFFFF for v in r]) for r in recs]

    def string(self, text):
        off = len(self.sb)
        self.sb.extend(text.encode('utf-8') + b'\0')
        return off

    def save(self, name):
        open(OUT + name, 'wb').write(struct.pack('<4s4I', b'WDBC', len(self.raw), self.fc, self.rs, len(self.sb))
                                     + b''.join(self.raw) + bytes(self.sb))
        print('%-36s %6d records' % (name, len(self.raw)))


# (race, sex) that wears another race's body instead of its own: the Earthen female's own model is the old
# pre-HD Dwarf female (4891 vertices), which the HD textures don't fit; she becomes the HD Dwarf female, with the
# Dwarf female's display, sections, HD sections and hair (the Earthen male already has the HD Dwarf model).
SEX_FROM = {(27, 1): 3}


def copy_race_rows(name, race_field, idfield=0, sex_field=None):
    """Copy every row of a source race to the new race id (renumbered races only)."""
    t = DBC(D + name)
    recs = t.recs()
    have = {r[race_field] for r in recs}
    next_id = max(r[idfield] for r in recs) + 1 if idfield is not None else None
    for new, src in CLONE.items():
        if new in have:
            continue
        for r in [r for r in recs if r[race_field] == src]:
            nr = list(r)
            nr[race_field] = new
            if idfield is not None:
                nr[idfield] = next_id
                next_id += 1
            recs.append(nr)
    if sex_field is not None:
        for (race, sex), src in SEX_FROM.items():
            recs = [r for r in recs if not (r[race_field] == race and r[sex_field] == sex)]
            for r in [r for r in recs if r[race_field] == src and r[sex_field] == sex]:
                nr = list(r)
                nr[race_field] = race
                if idfield is not None:
                    nr[idfield] = next_id
                    next_id += 1
                recs.append(nr)
    t.set_recs(recs)
    t.save(name)


# ---------------------------------------------------------------- ChrRaces
# Only Goblin of the new races has its own helmet models in the client; the others borrow the closest body.
HELM_PREFIX = {12: 'Tr', 13: 'Hu', 15: 'Ta', 16: 'Hu', 17: 'Ta', 18: 'Hu', 19: 'Gn', 20: 'Dw', 21: 'Be', 22: 'Dr',
               23: 'Or', 24: 'Tr', 25: 'Tr', 26: 'Sc', 28: 'Tr', 29: 'Dw', 30: 'Gn', 31: 'Gn'}
t = DBC(D + 'ChrRaces.dbc')
recs = t.recs()
by_id = {r[0]: r for r in recs}
for rid, (src, side, label, _) in RACES.items():
    row = by_id.get(rid)
    if row is None or rid != src:
        row = list(by_id[src])
        row[0] = rid
        if rid in by_id:
            recs.remove(by_id[rid])
        recs.append(row)
        by_id[rid] = row
    f = FACTION[side]
    row[1], row[2], row[7], row[13], row[68] = PLAYABLE_FLAGS, f['faction'], f['team'], f['side'], 0
    row[12] = 0                      # no intro cinematic
    if rid in HELM_PREFIX:           # helmets are race models (Helm_x_<prefix><M/F>.m2): borrow a base race's
        row[6] = t.string(HELM_PREFIX[rid])
    if label:
        off = t.string(label)
        for k in list(range(14, 30)) + list(range(31, 47)) + list(range(48, 64)):
            row[k] = off
recs.sort(key=lambda r: r[0])
t.set_recs(recs)
t.save('ChrRaces.dbc')

# ---------------------------------------------------------------- player-ready models
# Ascension's client only dresses a character model as a player (skin, face, hair, gear) when its
# CreatureModelData carries flag 0x800. NPC-only races lack it, so they get player copies of their
# model and display (the NPC rows are left alone).
cmd = DBC(D + 'CreatureModelData.dbc')
cmd_recs = cmd.recs()
cmd_by = {r[0]: r for r in cmd_recs}
cdi = DBC(D + 'CreatureDisplayInfo.dbc')
cdi_recs = cdi.recs()
cdi_by = {r[0]: r for r in cdi_recs}
next_model = max(cmd_by) + 1
next_display = max(cdi_by) + 1
# Murloc wears NPC looks in game (custom_race_display). Models missing texture slots the character code fills
# (Naga, Fel Orc male, Skeleton) use <Model>Player.m2 copies that have them (make_player_models.py).
NPC_LOOK_RACES = {30, 31}
import sys as _sys
_sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from make_player_models import MODELS as _PLAYER_MODELS, player_path as _player_path
PLAYER_MODEL_FILE = {key: _player_path(path)[:-3] + '.mdx' for key, path in _PLAYER_MODELS.items()}
copies = 0
DISPLAY_COPIES = []
for row in recs:
    if row[0] not in RACES or row[0] in NPC_LOOK_RACES:
        continue
    for k in (4, 5):
        disp = cdi_by.get(row[k])
        if not disp:
            continue
        model = cmd_by.get(disp[1])
        if not model or model[1] & 0x800:
            continue
        nm = list(model)
        nm[0] = next_model
        nm[1] |= 0x804
        if (row[0], k) in PLAYER_MODEL_FILE:
            nm[2] = cmd.string(PLAYER_MODEL_FILE[(row[0], k)])
        cmd_recs.append(nm)
        cmd_by[next_model] = nm
        nd = list(disp)
        nd[0] = next_display
        nd[1] = next_model
        cdi_recs.append(nd)
        cdi_by[next_display] = nd
        DISPLAY_COPIES.append((next_display, disp[0], 0 if k == 4 else 1))
        row[k] = next_display
        next_model += 1
        next_display += 1
        copies += 1
cmd.set_recs(cmd_recs)
cmd.save('CreatureModelData.dbc')
cdi.set_recs(cdi_recs)
cdi.save('CreatureDisplayInfo.dbc')
t.set_recs(recs)
t.save('ChrRaces.dbc')
print('player-ready model copies:', copies)
# Murloc: two bodies (make_murloc_model.py), picked with the gender button.
#  * male = Whim murloc: Skin Color = body, "Armor" (the hair style option) = none / armor 1-4. One display per
#    (body, armor) - Ascension's Whim displays choose the armor set (and fin variant) through
#    CreatureDisplayInfoGeosetData (group 1: armor N -> N, 5 = none; group 2: fins), so the copies get those rows.
#  * female = classic murloc on MurlocJump.m2 (jump animations mapped to existing moves), 18 skins.
#  Both in-game models (WhimGame.m2 / MurlocJump.m2) get the character Back attachment point: cosmetic wings.
# In game the server picks the display by skin colour and hair style (custom_race_display idx = hair * 32 + skin,
# gen_race_looks.py); the character screens paint the same body (WhimPlayerSkinNN / MurlocPlayerSkinNN, CharSections
# below) on the character-screen copy of the body (WhimPlayer.m2 / MurlocPlayer.m2).
import json as _json
_murloc = _json.load(open(OUT + 'murloc_skins.json'))
ONE = struct.unpack('<I', struct.pack('<f', 1.0))[0]
WHIM_DISPLAY, CLASSIC_DISPLAY = 141098, 31
MURLOC_SKIN_FILES = {0: _murloc['male']['skins'], 1: _murloc['female']['skins']}
MURLOC_FOLDER = {0: 'Creature' + chr(92) + 'WhimMurloc', 1: 'Creature' + chr(92) + 'Murloc'}
MURLOC_ARMORS = _murloc['male']['armors']
MURLOC_DISPLAYS = {0: {}, 1: {}}                       # sex -> {custom_race_display idx: display}
MURLOC_GEOSETS = []                                     # (display, geoset group, value)


def _murloc_display(source, model, textures, sex):
    global next_display
    nd = list(cdi_by[source])
    nd[0], nd[4] = next_display, ONE
    if model:
        nd[1] = model
    nd[6], nd[7], nd[8] = [cdi.string(t) if t else 0 for t in textures]
    cdi_recs.append(nd)
    DISPLAY_COPIES.append((next_display, source, sex))
    next_display += 1
    return nd[0]


whim_model = list(cmd_by[cdi_by[WHIM_DISPLAY][1]])   # in-game Whim murloc: WhimGame.m2 (+ Back attachment)
whim_model[0], whim_model[2] = next_model, cmd.string('Creature' + chr(92) + 'WhimMurloc' + chr(92) + 'WhimGame.mdx')
cmd_recs.append(whim_model)
next_model += 1
for armor in range(len(MURLOC_ARMORS) + 1):            # 0 = no armor
    for skin, body in enumerate(_murloc['male']['bodies']):
        display = _murloc_display(WHIM_DISPLAY, whim_model[0], (body, MURLOC_ARMORS[armor - 1] if armor else None, None), 0)
        MURLOC_DISPLAYS[0][armor * 32 + skin] = display
        MURLOC_GEOSETS += [(display, 1, armor if armor else 5), (display, 2, 1)]
jump_model = list(cmd_by[cdi_by[CLASSIC_DISPLAY][1]])  # in-game classic murloc: MurlocJump.m2
jump_model[0], jump_model[2] = next_model, cmd.string('Creature' + chr(92) + 'Murloc' + chr(92) + 'MurlocJump.mdx')
cmd_recs.append(jump_model)
next_model += 1
for skin, name in enumerate(MURLOC_SKIN_FILES[1]):
    MURLOC_DISPLAYS[1][skin] = _murloc_display(CLASSIC_DISPLAY, jump_model[0], (name, None, None), 1)
MURLOC_GLUE = {}
for sex, source, model_name in ((0, WHIM_DISPLAY, 'WhimPlayer'), (1, CLASSIC_DISPLAY, 'MurlocPlayer')):
    nm = list(cmd_by[cdi_by[source][1]])
    nm[0], nm[2] = next_model, cmd.string(MURLOC_FOLDER[sex] + chr(92) + model_name + '.mdx')
    cmd_recs.append(nm)
    next_model += 1
    MURLOC_GLUE[sex] = _murloc_display(source, nm[0], (None, None, None), sex)
cmd.set_recs(cmd_recs)
cmd.save('CreatureModelData.dbc')
cdi.set_recs(cdi_recs)
cdi.save('CreatureDisplayInfo.dbc')
import json
json.dump({'displays': MURLOC_DISPLAYS, 'glue_display': MURLOC_GLUE}, open(OUT + 'murloc.json', 'w'), indent=1)
t_geo = DBC(D + 'CreatureDisplayInfoGeosetData.dbc')       # Whim armor / fin geoset per display
geo = t_geo.recs()
next_geo = max(r[0] for r in geo) + 1
for display, group, value in MURLOC_GEOSETS:
    geo.append([next_geo, group, value, display])
    next_geo += 1
t_geo.set_recs(geo)
t_geo.save('CreatureDisplayInfoGeosetData.dbc')
for (race, sex), src in SEX_FROM.items():                 # body borrowed from another race (see SEX_FROM)
    source_row = next(r for r in recs if r[0] == src)
    next(r for r in recs if r[0] == race)[4 + sex] = source_row[4 + sex]
for row in recs:
    if RACES.get(row[0], (0, 0, None))[2] == 'Murloc':
        row[4], row[5] = MURLOC_GLUE[0], MURLOC_GLUE[1]
        row[67] = t.string('MURLOCARMOR')          # hair style option = armor (label HAIR_MURLOCARMOR_STYLE, Lua)
t.set_recs(recs)
t.save('ChrRaces.dbc')
t.save('client_ChrRaces.dbc')                     # same table for the client (build.py packs this name)
print('Murloc: displays %s, character-screen displays %s, geoset rows %d' % ({k: len(v) for k, v in MURLOC_DISPLAYS.items()}, MURLOC_GLUE, len(MURLOC_GEOSETS)))

# ---------------------------------------------------------------- per-race customisation data
copy_race_rows('CharSections.dbc', 1, sex_field=2)
# The sections are copied exactly as Ascension ships them for the source race (no flags, paths or sizes changed:
# blanking / borrowing / upscaling missing or old textures only broke faces). Ascension draws its HD character
# models from HDCharSections, which also has rows for the old NPC races under their own ids (Fel Orc 34 ...
# Ice Troll 43) and the Blood Elf: copied below, without them the renumbered races fell back to the 256 textures.
copy_race_rows('HDCharSections.dbc', 1, sex_field=2)
# Earthen (client race 62, "Npc_Earthen") wears copies of the Dwarf textures but has no HD rows: its 256 faces were
# pasted into the 512 body (a squashed patch on the face). It gets HD rows for its own sections, each taking the
# Dwarf HD row that paints the same texture (matched by file name).
HD_TEXTURES_FROM = {27: 3}
t = DBC(OUT + 'HDCharSections.dbc')
hd = t.recs()
own = DBC(OUT + 'CharSections.dbc')
own_recs, own_name = own.recs(), lambda o: own.sb[o:own.sb.index(b'\0', o)].decode('utf-8', 'replace') if o else ''
hd_name = lambda o: t.sb[o:t.sb.index(b'\0', o)].decode('utf-8', 'replace') if o else ''
next_hd = max(r[0] for r in hd) + 1
for race, src in HD_TEXTURES_FROM.items():
    have_hd = {(r[2], r[3], r[8], r[9]) for r in hd if r[1] == race}    # e.g. the female copied whole (SEX_FROM)
    by_file = {}
    for r in hd:
        if r[1] == src and r[4]:
            by_file.setdefault((r[2], r[3], hd_name(r[4]).split(chr(92))[-1].lower()), r)
    added = 0
    for r in own_recs:
        if r[1] != race or not r[4] or (r[2], r[3], r[8], r[9]) in have_hd:
            continue
        match = by_file.get((r[2], r[3], own_name(r[4]).split(chr(92))[-1].lower()))
        if match:
            nr = list(match)
            nr[0], nr[1], nr[8], nr[9] = next_hd, race, r[8], r[9]
            hd.append(nr)
            next_hd += 1
            added += 1
    print('HD rows for race %d from race %d textures: %d' % (race, src, added))
t.set_recs(hd)
t.save('HDCharSections.dbc')
# Murloc skin colours: the Troglodyte rows repeated up to one skin colour per look (every colour keeps its face and
# underwear rows: a colour without them crashed character creation). The skin rows paint MurlocPlayerSkinNN; face,
# facial hair, hair and underwear are left blank (the murloc has no human layout to paint them on).
t = DBC(OUT + 'CharSections.dbc')
recs = t.recs()
next_section = max(r[0] for r in recs) + 1
skin_paths = {sex: [t.string(MURLOC_FOLDER[sex] + chr(92) + '%s.blp' % name) for name in names]
              for sex, names in MURLOC_SKIN_FILES.items()}
scalp_paths = {sex: [t.string(MURLOC_FOLDER[sex] + chr(92) + 'MurlocScalp%s.blp' % part) for part in ('Lower', 'Upper')]
               for sex in (0, 1)}
crop_paths = {sex: [{part: t.string(MURLOC_FOLDER[sex] + chr(92) + '%s_%s.blp' % (name, part))
                     for part in ('FaceLower', 'FaceUpper', 'Pelvis', 'Torso')} for name in names]
              for sex, names in MURLOC_SKIN_FILES.items()}
for race in [r for r, v in RACES.items() if v[2] == 'Murloc']:
    for sex in (0, 1):
        own = [r for r in recs if r[1] == race and r[2] == sex and r[3] in (0, 1, 4)]
        colours = len({r[9] for r in own if r[3] == 0})
        if not colours:
            continue
        recs = [r for r in recs if not (r[1] == race and r[2] == sex and r[3] in (0, 1, 4) and r[9] >= len(skin_paths[sex]))]
        own = [r for r in own if r[9] < len(skin_paths[sex])]
        for colour in range(colours, len(skin_paths[sex])):
            for r in [r for r in own if r[9] == colour % colours]:
                nr = list(r)
                nr[0], nr[9] = next_section, colour
                recs.append(nr)
                next_section += 1
        for r in recs:
            if r[1] == race and r[2] == sex:
                # 0x10: 512x512 texture, as on every race that paints correctly at creation; without it the
                # painter takes the 512 murloc skin for a 256 one, scales it x2 and covers the body with one quarter
                r[7] |= 0x10
                crop = lambda part: crop_paths[sex][r[9] % len(crop_paths[sex])][part]
                if r[3] == 0 and r[9] < len(skin_paths[sex]):
                    r[4], r[5], r[6] = skin_paths[sex][r[9]], 0, 0
                elif r[3] == 1:                      # face: lower, upper (crops of the skin, make_murloc_model.py)
                    r[4], r[5], r[6] = crop('FaceLower'), crop('FaceUpper'), 0
                elif r[3] == 4:                      # underwear: pelvis, torso
                    r[4], r[5], r[6] = crop('Pelvis'), crop('Torso'), 0
                elif r[3] == 3:                      # hair: no texture, transparent scalp (lower, upper)
                    r[4], r[5], r[6] = 0, scalp_paths[sex][0], scalp_paths[sex][1]
                else:
                    r[4], r[5], r[6] = 0, 0, 0
# Whim murloc armor = hair style: style 0 none, style N armor N, its hair texture is the Whim armor texture
armor_paths = [0] + [t.string('Creature' + chr(92) + 'WhimMurloc' + chr(92) + name + '.blp') for name in MURLOC_ARMORS]
for race in [r for r, v in RACES.items() if v[2] == 'Murloc']:
    template = next(r for r in recs if r[1] == race and r[2] == 0 and r[3] == 0)
    recs = [r for r in recs if not (r[1] == race and r[2] == 0 and r[3] == 3)]
    for style, path in enumerate(armor_paths):
        nr = list(template)
        nr[0], nr[3], nr[4], nr[5], nr[6], nr[8], nr[9] = (next_section, 3, path, scalp_paths[0][0], scalp_paths[0][1],
                                                            style, 0)                     # transparent scalp
        recs.append(nr)
        next_section += 1
t.set_recs(recs)
t.save('CharSections.dbc')
copy_race_rows('CharHairGeosets.dbc', 1, sex_field=2)
t = DBC(OUT + 'CharHairGeosets.dbc')                 # fields: ID, race, sex, style, geoset, show scalp
hair = [r for r in t.recs() if not (RACES.get(r[1], (0, 0, None))[2] == 'Murloc' and r[2] == 0)]
next_hair = max(r[0] for r in hair) + 1
for race in [r for r, v in RACES.items() if v[2] == 'Murloc']:
    for style in range(len(armor_paths)):            # WhimPlayer.m2: armor N = hair geoset N
        # no armor = geoset 5, which the model doesn't have (as the in-game Whim displays): geoset 0 left all four
        # armor sets showing at creation
        hair.append([next_hair, race, 0, style, style or 5, 0])
        next_hair += 1
t.set_recs(hair)
t.save('CharHairGeosets.dbc')
copy_race_rows('CharacterFacialHairStyles.dbc', 0, idfield=None, sex_field=1)
copy_race_rows('NameGen.dbc', 2)
copy_race_rows('EmotesTextSound.dbc', 2)
# Wardrobe / dressing-room zoom per (race, sex, gear slot): fields ID, race, sex, slot, camera. Only the 10 stock
# races have rows, so each new race borrows the cameras of a base race of similar height.
CAMERA_RACE = {9: 7, 12: 8, 13: 6, 14: 10, 15: 2, 16: 6, 17: 6, 18: 6, 19: 7, 20: 2, 21: 1, 22: 11, 23: 2, 24: 8,
               25: 8, 26: 5, 27: 3, 28: 8, 29: 2, 30: 7, 31: 7}
t = DBC(D + 'UICameraAppearanceChrRaces.dbc')
cams = t.recs()
next_cam = max(r[0] for r in cams) + 1
for rid, base in CAMERA_RACE.items():
    for r in [r for r in cams if r[1] == base]:
        nr = list(r)
        nr[0], nr[1] = next_cam, rid
        cams.append(nr)
        next_cam += 1
t.set_recs(cams)
t.save('UICameraAppearanceChrRaces.dbc')

# ---------------------------------------------------------------- CharStartOutfit (race/class/gender are bytes)
# Every (new race, class, sex) gets an outfit: its own (High Elf / Pandaren), else the faction template race's, else
# another race of the faction that has one for the class (no Orc druid). Rows without any item count as missing.
t = DBC(D + 'CharStartOutfit.dbc')
has_items = lambda r: any(x > 0 for x in struct.unpack_from('<24i', r, 8))
rows = [bytearray(r) for r in t.raw if r[4] not in RACES or has_items(r)]
key = lambda r: (r[4], r[5], r[6])
have = {key(r) for r in rows}
by_key = {key(r): r for r in rows if has_items(r)}
next_id = max(struct.unpack_from('<I', r, 0)[0] for r in rows) + 1
SOURCES = {A: [1, 4, 3, 11, 7], H: [2, 6, 8, 5, 10]}
classes = sorted({r[5] for r in rows})
for rid, (src, side, _, _) in RACES.items():
    order = ([src] if src in (10, 20) else []) + SOURCES[side]
    for cls in classes:
        for sex in (0, 1):
            if (rid, cls, sex) in have:
                continue
            source = next((by_key[(o, cls, sex)] for o in order if (o, cls, sex) in by_key), None)
            if source is None:
                continue
            nr = bytearray(source)
            struct.pack_into('<I', nr, 0, next_id)
            next_id += 1
            nr[4] = rid
            rows.append(nr)
            have.add(key(nr))
t.raw = [bytes(r) for r in rows]
t.save('CharStartOutfit.dbc')

# ---------------------------------------------------------------- Ascension creation previews (client only)
# The creation screen dresses CoA classes from CharacterCreationClassDetails (class, race, sex -> outfit items),
# Hero archetypes from CharacterCreationArchetypeDetails, and previews forms / pets from the Shapeshift / Pet
# details, all keyed by race 1-11: the new races were shown naked. Each gets the rows of the same source races as
# CharStartOutfit.
def copy_creation_rows(name, race_field, key_fields):
    t = DBC(D + name)
    recs = t.recs()
    by_race = collections.defaultdict(dict)                 # race -> {(class / spell, sex): row}
    for r in recs:
        by_race[r[race_field]][tuple(r[k] for k in key_fields)] = r
    next_id = max(r[0] for r in recs) + 1
    for rid, (src, side, _, _) in RACES.items():
        order = ([src] if src in (10, 20) else []) + SOURCES[side]
        slots = {}
        for o in reversed(order):                            # the first source race of the order wins
            slots.update(by_race.get(o, {}))
        for slot, r in slots.items():
            if slot in by_race.get(rid, {}):                 # Goblin has its own row for one class
                continue
            nr = list(r)
            nr[0], nr[race_field] = next_id, rid
            next_id += 1
            recs.append(nr)
    t.set_recs(recs)
    t.save(name)


copy_creation_rows('CharacterCreationClassDetails.dbc', 2, (1, 3))
copy_creation_rows('CharacterCreationArchetypeDetails.dbc', 2, (1, 3))
copy_creation_rows('CharacterCreationShapeshiftDetails.dbc', 2, (1,))
copy_creation_rows('CharacterCreationPetDetails.dbc', 2, (1,))

# ---------------------------------------------------------------- race masks
t = DBC(D + 'Faction.dbc')
recs = t.recs()
for r in recs:
    for k in range(2, 6):
        r[k] = expand(r[k])
t.set_recs(recs)
t.save('Faction.dbc')


def expand_dbc(src_path, name, field):
    t = DBC(src_path)
    recs = t.recs()
    for r in recs:
        r[field] = expand(r[field], racial=r[1] in RACIAL_SKILLS)
    t.set_recs(recs)
    t.save(name)


# server files from the repack, client files from the class work (out_*.dbc in the scratchpad)
expand_dbc(D + 'SkillRaceClassInfo.dbc', 'server_SkillRaceClassInfo.dbc', 2)
expand_dbc(D + 'SkillLineAbility.dbc', 'server_SkillLineAbility.dbc', 3)
expand_dbc('out_SkillRaceClassInfo.dbc', 'client_SkillRaceClassInfo.dbc', 2)
expand_dbc('out_SkillLineAbility.dbc', 'client_SkillLineAbility.dbc', 3)

# ---------------------------------------------------------------- CharBaseInfo (client allows 30 classes per race)
c = open('client_CharBaseInfo.dbc', 'rb').read()
cn = struct.unpack('<I', c[4:8])[0]
pairs = [(c[20 + i * 2], c[21 + i * 2]) for i in range(cn)]
human = [cl for r, cl in pairs if r == 1]
pairs = [(r, cl) for r, cl in pairs if r not in RACES] + [(rid, cl) for rid in sorted(RACES) for cl in human]
open(OUT + 'CharBaseInfo.dbc', 'wb').write(struct.pack('<4s4I', b'WDBC', len(pairs), 2, 2, 0) + b''.join(bytes(p) for p in pairs))
print('%-36s %6d records' % ('CharBaseInfo.dbc', len(pairs)))

# ---------------------------------------------------------------- world database
ids = ','.join(str(r) for r in RACES)
sql = ['USE acore_world;', 'START TRANSACTION;']
sql.append('DELETE FROM playercreateinfo WHERE race IN (%s);' % ids)
sql.append('DELETE FROM playercreateinfo_item WHERE race IN (%s);' % ids)
sql.append('DELETE FROM playercreateinfo_action WHERE race IN (%s);' % ids)
sql.append('DELETE FROM player_race_stats WHERE Race IN (%s);' % ids)
sql.append('DELETE FROM ascension_custom_class_race WHERE race IN (%s);' % ids)
for rid, (src, side, _, stats) in RACES.items():
    tpl = TEMPLATE[side]
    sql.append('INSERT INTO playercreateinfo SELECT %d,class,map,zone,position_x,position_y,position_z,orientation FROM playercreateinfo WHERE race=%d;' % (rid, tpl))
    sql.append('INSERT INTO playercreateinfo_item SELECT %d,class,itemid,amount,Note FROM playercreateinfo_item WHERE race=%d;' % (rid, tpl))
    sql.append('INSERT INTO playercreateinfo_action SELECT %d,class,button,action,type FROM playercreateinfo_action WHERE race=%d;' % (rid, tpl))
    sql.append('INSERT INTO player_race_stats SELECT %d,Strength,Agility,Stamina,Intellect,Spirit FROM player_race_stats WHERE Race=%d;' % (rid, stats))
    sql.append('INSERT INTO ascension_custom_class_race SELECT DISTINCT class,%d FROM ascension_custom_class_race;' % rid)


def mask_sql(table, col, where=''):
    # same rule as expand(), in SQL. Also undoes an earlier rule that copied Blood Elf -> High Elf and
    # Pandaren (A) -> Pandaren (H) bits into faction masks of the other faction.
    m = '(%s & 4294967295)' % col
    full_a = '(%s & %d = %d)' % (m, OLD_A, OLD_A)
    full_h = '(%s & %d = %d)' % (m, OLD_H, OLD_H)
    valid = '%s NOT IN (0, -1, 4294967295)%s' % (col, where)
    sql.append('UPDATE %s SET %s = %s & ~%d WHERE %s AND %s AND NOT %s;' % (table, col, col, bit(14), valid, full_h, full_a))
    sql.append('UPDATE %s SET %s = %s & ~%d WHERE %s AND %s AND NOT %s;' % (table, col, col, bit(29), valid, full_a, full_h))
    sql.append('UPDATE %s SET %s = %s | IF(%s, %d, 0) | IF(%s, %d, 0) WHERE %s;' % (table, col, col, full_a, NEW_A, full_h, NEW_H, valid))
    # race-specific masks lose the new races first and get them back from RACIAL below, so a changed RACIAL entry
    # takes its race out of the old base race's rows (the Alliance Murloc kept the Gnome racials next to the
    # Undead ones). Rows naming only new races (racial combos) are left alone.
    sql.append('UPDATE %s SET %s = %s & ~%d WHERE %s AND NOT %s AND NOT %s AND (%s & %d) <> 0;'
               % (table, col, col, NEW_A | NEW_H, valid, full_a, full_h, m, OLD_A | OLD_H))
    racial = ' | '.join('IF(%s & %d, %d, 0)' % (m, bit(base), bit(new)) for new, base in RACIAL.items())
    sql.append('UPDATE %s SET %s = %s | %s WHERE %s AND NOT %s AND NOT %s;' % (table, col, col, racial, valid, full_a, full_h))


mask_sql('quest_template', 'AllowableRaces')
mask_sql('item_template', 'AllowableRace')
mask_sql('conditions', 'ConditionValue1', ' AND ConditionTypeOrReference = 16')
mask_sql('playercreateinfo_skills', 'raceMask')
mask_sql('playercreateinfo_cast_spell', 'raceMask')
mask_sql('playercreateinfo_spell_custom', 'racemask')
mask_sql('spell_area', 'racemask')
mask_sql('mail_level_reward', 'raceMask')
mask_sql('skilllineability_dbc', 'RaceMask')
mask_sql('skillraceclassinfo_dbc', 'RaceMask')
# creature_model_info of the copies: size of the source display; the gender is the one of the race slot / look,
# because Unit::SetDisplayId sets the unit's gender from it (a female got a male NPC row and turned male)
for new_disp, old_disp, gender in DISPLAY_COPIES:
    sql.append('DELETE FROM creature_model_info WHERE DisplayID=%d;' % new_disp)
    sql.append('INSERT INTO creature_model_info (DisplayID,BoundingRadius,CombatReach,Gender,DisplayID_Other_Gender) '
               'SELECT %d,BoundingRadius,CombatReach,%d,0 FROM creature_model_info WHERE DisplayID=%d;' % (new_disp, gender, old_disp))
    # some NPC displays have no model info: fall back to the human's
    sql.append('INSERT IGNORE INTO creature_model_info (DisplayID,BoundingRadius,CombatReach,Gender,DisplayID_Other_Gender) '
               'SELECT %d,BoundingRadius,CombatReach,%d,0 FROM creature_model_info WHERE DisplayID=%d;' % (new_disp, gender, 50 if gender else 49))
sql.append('COMMIT;')
open(OUT + 'races.sql', 'w').write('\n'.join(sql) + '\n')
print('races.sql: %d statements; Alliance new mask 0x%X, Horde new mask 0x%X' % (len(sql), NEW_A, NEW_H))
