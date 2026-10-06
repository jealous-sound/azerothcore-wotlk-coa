# Signature racial borrowed from another race, on top of the base racial line (gen_races.py RACIAL).
# Given at character creation through playercreateinfo_spell_custom (all classes, vanilla and CoA).
import os
bit = lambda n: 1 << (n - 1)
PHYS = bit(1) | bit(3) | bit(4) | bit(6)                     # warrior, hunter, rogue, death knight
CASTER = bit(5) | bit(8) | bit(9)                            # priest, mage, warlock
HYBRID = 0xFFFFFFFF & ~(PHYS | CASTER) & ~bit(10)            # paladin, shaman, druid, CoA classes
VARIANTS = {
    'Blood Fury': [(20572, PHYS), (33702, CASTER), (33697, HYBRID)],
    'Arcane Torrent': [(25046, bit(4)), (50613, bit(6)), (28730, 0xFFFFFFFF & ~bit(4) & ~bit(6) & ~bit(10))],
}
SINGLE = {'Berserking': 26297, 'Escape Artist': 20589, 'War Stomp': 20549, 'Stoneform': 20594,
          'Frost Resistance': 20596, 'Gift of the Naaru': 28880, 'Will of the Forsaken': 7744,
          'Every Man for Himself': 59752, 'Shadowmeld': 58984}
COMBO = {13: 'Berserking', 9: 'Escape Artist', 15: 'War Stomp', 16: 'Stoneform', 17: 'Frost Resistance',
         18: 'Blood Fury', 19: 'Escape Artist', 20: 'War Stomp', 29: 'Gift of the Naaru', 21: 'Arcane Torrent',
         22: 'Will of the Forsaken', 23: 'Arcane Torrent', 24: 'Shadowmeld', 25: 'Stoneform', 26: 'Stoneform',
         27: 'War Stomp', 28: 'Blood Fury', 12: 'Gift of the Naaru', 14: 'Every Man for Himself',
         30: 'Shadowmeld', 31: 'Escape Artist'}
rows = []
for race, name in COMBO.items():
    for spell, classes in VARIANTS.get(name, [(SINGLE.get(name), 0)]):
        rows.append((bit(race), classes, spell, 'racial combo: %s' % name))
sql = ['USE acore_world;', "DELETE FROM playercreateinfo_spell_custom WHERE Note LIKE 'racial combo:%';"]
sql.append('INSERT INTO playercreateinfo_spell_custom (racemask, classmask, Spell, Note) VALUES ' +
           ','.join("(%d,%d,%d,'%s')" % (r, c if c != 0xFFFFFFFF else 0, s, n) for r, c, s, n in rows) + ';')
here = os.path.dirname(os.path.abspath(__file__))
open(os.path.join(here, '..', 'sql', 'racial_combos.sql'), 'w').write('\n'.join(sql) + '\n')
print(len(rows), 'rows')
