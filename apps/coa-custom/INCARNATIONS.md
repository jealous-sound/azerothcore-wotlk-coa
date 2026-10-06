# Incarnation slots (Wardrobe > Incarnations)

Generated from the client's AppearanceCategories.dbc and Appearances.dbc.

| Slot (category) | Name | Applies to | Classes that see it | Looks available |
|---|---|---|---|---|
| 17 | Bear Form | form 5 (Bear) | Hero, Druid, Venomancer | 46 |
| 18 | Cat Form | form 1 (Cat) | Hero, Druid, Venomancer | 64 |
| 19 | Travel Form | form 3 (Travel) | Hero, Druid, Venomancer | 13 |
| 20 | Aquatic Form | form 4 (Aquatic) | Hero, Druid, Venomancer | 2 |
| 21 | Flight Form | form 29 (Flight) | Hero, Druid | 7 |
| 22 | Moonkin Form | form 31 (Moonkin) | Hero, Druid, Venomancer | 24 |
| 23 | Tree Form | form 2 (Tree of Life) | Hero, Druid | 30 |
| 24 | Ghost Wolf | form 16 (Ghost Wolf) | Shaman, Hero | 9 |
| 25 | Summoned Imp | summoned creature 416 / Bronzebeard 1100416 | Warlock, Hero | 21 |
| 26 | Summoned Voidwalker | summoned creature 1860 / Bronzebeard 1101860 | Warlock, Hero | 6 |
| 27 | Summoned Succubus | summoned creature 1863 / Bronzebeard 1101863 | Warlock, Hero | 6 |
| 28 | Summoned Felhunter | summoned creature 417 / Bronzebeard 1100417 | Warlock, Hero | 11 |
| 29 | Summoned Felguard | summoned creature 17252 / Bronzebeard 1117252 | Warlock, Hero | 17 |
| 30 | (DND) Summoned Felguard Weapon | - | all classes | 0 |
| 31 | Metamorphosis | form 22 (Metamorphosis) | Warlock, Hero | 13 |
| 32 | Ammunition | ammunition | all classes | 43 |
| 33 | Call Pet | hunter pet (any family) | Hunter, Hero | 1034 |
| 34 | Summon Demon | warlock demons (when no slot 25-29 look is chosen), Infernal, Doomguard | Warlock, Hero | 318 |
| 35 | Raise Undead | Death Knight ghoul and gargoyle | Death Knight, Hero | 668 |
| 36 | Call Dragonkin | Water Elemental | Mage, Hero | 704 |
| 37 | Raise Elemental | Greater Fire / Earth Elemental, Spirit Wolves | Shaman, Hero | 596 |
| 65 | Knight of Xoroth | spell 804703 Xorothian Warsteed | all classes | 1 |
| 66 | Pyromancer Draconic Form | spell 520829 (Draconic Aspect Draconic Form) | Pyromancer | 2 |
| 67 | Necromancer Lich Form | spell 735821 (?) | Necromancer | 1 |
| 68 | Shadowhound | summoned creature 1793 | Witch Hunter | 4 |

Slots 33-37 are Ascension's class-pet collections: each look is a creature, and the server puts that creature's
model on the vanilla class's own pets (core `apply_class_pets.py`). A hunter pet only wears the look; its saved
model is kept, so clearing the slot gives the old pet back. Spell-visual slots (39-54) are applied by the client
itself (Disable Spell Visuals button).
