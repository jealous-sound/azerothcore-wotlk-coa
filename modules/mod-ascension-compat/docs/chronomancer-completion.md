# Chronomancer reconstruction policy — 2026-09-19

This note records which Chronomancer audit reports describe spells no player can acquire, so they can be closed
with their evidence instead of being carried as open work. It makes no claim of parity with the official
Ascension backend.

## Why an absence means something for this class

The fork is a reconstruction, so its own database cannot settle "could a player acquire this?": a missing
acquisition row is also what lost server data looks like. The question is settled by calibrating each outside
source against the spells this class is already known to acquire, with `~/CoaServer/reference/coa-obtainable`
(community snapshots, never the live server):

```
class 22 (CHRONOMANCER): ours 169; live-client capture covers 169 of 169, extra in capture 0, identical true;
                         calculator covers 157 of 169; class_spell covers 35 of 169
```

The `CharacterAdvancement` set harvested through the live client's own `GetAllEntries` API and the shipped table
hold **the same 169 spells, in both directions**. The acquisition table is complete for this class, which is what
makes an absence from it evidence rather than a gap.

## The 90 reports

None of the 99 spells below has a `CharacterAdvancement` entry, a module grant, a `class_spell` or trainer row,
or a reachable trigger from another spell, and none appears in the live-client capture or in the CoA Build Hub
calculator. 89 sit on one of the three Chronomancer specialization skill lines (79 Infinite, 80 Artificer,
81 Time) with `acquire = 0` — a client record with no acquisition path, the same shape as the 73 Templar spells
settled this way — and 10 appear on no skill line at all.

No server change is warranted: there is nothing to script for a spell no character can hold or trigger.

| Issue | Spell | Id | Where it appears |
|---|---|---|---|
| #1765 | Timeline Mastery | 300761 | nothing |
| #1766 | Maw of Balance | 300762 | nothing |
| #1854 | Revolving Sands | 500113 | skill line 79 Infinite |
| #2064 | Plaguestorm | 500095 | skill line 80 Artificer |
| #2142 | Time Stretch | 800863 | skill line 81 Time |
| #2289 | Rust Creation | 704479 | skill line 80 Artificer |
| #2290 | Artificer's Amplification | 704483 | skill line 80 Artificer |
| #2293 | Chromatic Shards | 704492 | skill line 79 Infinite |
| #2942 | Reconstruction | 706043 | skill line 79 Infinite |
| #2943 | Elder Tome | 706044 | skill line 79 Infinite |
| #2944 | Black Hole | 706045 | skill line 79 Infinite |
| #2946 | Master of Chaos | 706047 | skill line 79 Infinite |
| #2947 | Master of Duality | 706049 | skill line 79 Infinite |
| #2948 | Master of Order | 706050 | skill line 79 Infinite |
| #2949 | Improved Reverse Wound | 706052 | skill line 79 Infinite |
| #2950 | Wound Removal | 706054 | skill line 79 Infinite |
| #2951 | Slipstream | 706056 | skill line 79 Infinite |
| #2952 | Dark Star | 706062 | skill line 79 Infinite |
| #2953 | Luck or Fate? | 706065 | skill line 80 Artificer |
| #2953 | Luck or Fate? | 706082 | skill line 80 Artificer |
| #2954 | Enchanted Collar | 706066 | skill line 80 Artificer |
| #2955 | Runed Rod | 706067 | skill line 80 Artificer |
| #2955 | Runed Rod | 706068 | skill line 80 Artificer |
| #2956 | Plentiful Orbs | 706069 | skill line 80 Artificer |
| #2956 | Plentiful Orbs | 706070 | skill line 80 Artificer |
| #2958 | Eternity's End | 706072 | skill line 80 Artificer |
| #2959 | Protector Training | 706073 | skill line 80 Artificer |
| #2960 | Discs Upon Discs | 706074 | skill line 80 Artificer |
| #2960 | Discs Upon Discs | 706075 | skill line 80 Artificer |
| #2962 | Ancient Text | 706081 | skill line 80 Artificer |
| #2964 | Vivid Memory | 706086 | skill line 81 Time |
| #2965 | Truly Infinite | 706087 | skill line 81 Time |
| #2966 | Improved Time Stop | 706088 | skill line 81 Time |
| #2966 | Improved Time Stop | 706089 | skill line 81 Time |
| #2967 | Empowered Restoration | 706090 | skill line 81 Time |
| #2968 | Improved Past Self | 706091 | skill line 81 Time |
| #2968 | Improved Past Self | 706092 | skill line 81 Time |
| #2969 | Spatial Distortion | 706094 | skill line 81 Time |
| #2971 | Age of Empires | 706097 | skill line 81 Time |
| #2972 | Carbon Dating | 706101 | skill line 81 Time |
| #2973 | Chrono Herald | 706103 | skill line 81 Time |
| #2973 | Chrono Herald | 706104 | skill line 81 Time |
| #2974 | Wisdom | 706105 | skill line 80 Artificer |
| #2974 | Wisdom | 706106 | skill line 80 Artificer |
| #2975 | Balance of Power | 706108 | skill line 79 Infinite |
| #2976 | Saviour | 706109 | skill line 79 Infinite |
| #2977 | Beam of Time | 706110 | skill line 79 Infinite |
| #2978 | Biting Chaos | 706111 | skill line 79 Infinite |
| #2979 | Unearthed Passage | 706112 | skill line 79 Infinite |
| #2981 | Chrono Sorcery | 706115 | skill line 79 Infinite |
| #2982 | Timemancy | 706118 | skill line 81 Time |
| #2982 | Timemancy | 706119 | skill line 81 Time |
| #2983 | Chromatic Misalignment | 706120 | skill line 81 Time |
| #2985 | Cursed ID - Procs wont work on it | 706123 | skill line 80 Artificer |
| #2986 | Shield of The Keeper | 706124 | skill line 79 Infinite |
| #2987 | Timewarped Shield | 706125 | skill line 79 Infinite |
| #2988 | Wand of Entropy | 706126 | skill line 80 Artificer |
| #2989 | Improved Temporal Focus | 706127 | skill line 81 Time |
| #2991 | Quickcaster | 706132 | skill line 81 Time |
| #2993 | Dimensional Phasing | 706135 | skill line 81 Time |
| #2994 | Future Vision | 706136 | skill line 81 Time |
| #3044 | Accelerated Mending | 706249 | skill line 79 Infinite |
| #3166 | Dazzled | 800050 | skill line 81 Time |
| #3203 | Mass Decomposition | 801283 | skill line 79 Infinite |
| #3204 | Chronicles of History | 801288 | skill line 81 Time |
| #3205 | Time Loop | 801297 | skill line 81 Time |
| #3385 | Artificer's Toughness | 804420 | skill line 80 Artificer |
| #3386 | Artificer's Magic | 804422 | skill line 80 Artificer |
| #3387 | Artificer's Spellplate | 804431 | skill line 80 Artificer |
| #3393 | Shattered | 804447 | skill line 80 Artificer |
| #3394 | Collapse | 804448 | skill line 80 Artificer |
| #3398 | Order Generation Passive | 804485 | nothing |
| #3399 | Chaos Generation Passive | 804486 | nothing |
| #3400 | Echo | 804487 | skill line 80 Artificer |
| #3401 | Incarnation | 804493 | skill line 79 Infinite |
| #3402 | Remake | 804501 | skill line 79 Infinite |
| #3403 | Chronobeam Passive | 804502 | nothing |
| #3404 | Order Baseline Negative | 804516 | nothing |
| #3405 | Chaos Baseline Negative | 804517 | nothing |
| #3423 | Order and Chaos WATCHER | 804659 | nothing |
| #3482 | Stormcloak | 805161 | nothing |
| #3577 | Timeline Guardian | 805845 | skill line 79 Infinite |
| #3578 | Timeline Destroyer | 805846 | skill line 79 Infinite |
| #3579 | Tome of Chaos | 805849 | skill line 79 Infinite |
| #3580 | Celestial Glaives | 805850 | nothing |
| #3609 | Last Wish | 806207 | skill line 80 Artificer |
| #3610 | Disenchant Weapon | 806208 | skill line 80 Artificer |
| #3619 | Sandpage | 806244 | skill line 81 Time |
| #3751 | Epoch | 801275 | skill line 79 Infinite |
| #3752 | Past Self | 801278 | skill line 81 Time |
| #3753 | Buy Time | 801280 | skill line 81 Time |
| #3754 | Discordant Blast | 801290 | skill line 79 Infinite |
| #3755 | Fracture Timeline | 801306 | skill line 81 Time |
| #3793 | Stasis | 804419 | skill line 80 Artificer |
| #3794 | Flow of Infinity | 804497 | skill line 80 Artificer |
| #3843 | Dilation | 806246 | skill line 80 Artificer |
| #3847 | Maw of Chaos | 806316 | skill line 79 Infinite |
| #3849 | Gift of the Timeways | 806332 | skill line 79 Infinite |
| #3883 | Infinite - Level 30 Passive | 706058 | skill line 79 Infinite |

## Boundaries

These snapshots are community captures, not the live server. If a spell below is later shown to be acquirable —
a trainer, a quest, a client build carrying a `CharacterAdvancement` row for it — that finding wins over this
note and the report should be reopened. The reverse case, spells the capture offers that this fork's
`CharacterAdvancement.dbc` lacks, is a real gap and belongs in its own report.
