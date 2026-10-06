# Wardrobe incarnation slots opened to more classes (client AppearanceCategories.dbc, field 23 = class mask).
import struct
bit = lambda c: 1 << (c - 1)
OPEN = {
    17: [29], 18: [29, 20], 20: [29],   # druid forms: Venomancer; Cat: Bloodmage (Eternal Curse)
    31: [20],                                              # Metamorphosis: Bloodmage (Inner Demon)
    35: [6],                                               # Raise Undead looks: Death Knight ghoul / gargoyle
    24: [30],                                              # Ghost Wolf: Reaper (Underwalk)
    19: [29, 30],                                          # Travel Form: Venomancer, Reaper (Underwalk fallback)
    22: [29, 26],                                          # Moonkin: Venomancer (Vizier), Starcaller (Celestial Form)
}
d = bytearray(open('C:/CoA-Repack/Data/dbc/AppearanceCategories.dbc', 'rb').read())
_, n, fc, rs, ss = struct.unpack('<4s4I', d[:20])
# spell-look slots: a CoA spell in the free third spell field (19/20 = normal + Bronzebeard spell), test
SPELL_SLOTS = {39: (800926, 29),   # Power Word: Shield  -> Venomancer Emerald Veil
               40: (500038, 16),   # Healing Wave        -> Stormbringer Invigorating Surge
               41: (801448, 19),   # Lay on Hands        -> Templar Benediction
               43: (503625, 27),   # Chain Heal          -> Sun Cleric Daybreak
               48: (800135, 31)}   # Lesser Healing Wave -> Primalist Hand of the Earthmother
for i in range(n):
    o = 20 + i * rs
    r = list(struct.unpack_from('<%dI' % fc, d, o))
    if r[0] in SPELL_SLOTS:
        spell, cls = SPELL_SLOTS[r[0]]
        r[21] = spell
        r[23] |= bit(cls)
        struct.pack_into('<%dI' % fc, d, o, *r)
        print('spell slot', r[0], 'third spell', spell, 'class mask', hex(r[23]))
    if r[0] in OPEN:
        for c in OPEN[r[0]]:
            r[23] |= bit(c)
        struct.pack_into('<%dI' % fc, d, o, *r)
        print('slot', r[0], 'class mask', hex(r[23]))
open('out_AppearanceCategories.dbc', 'wb').write(d)
