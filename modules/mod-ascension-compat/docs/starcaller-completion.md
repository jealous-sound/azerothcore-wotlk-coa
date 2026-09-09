# Starcaller reconstruction policy — 2026-09-09

This source package addresses the 164 findings captured in
`C:/Ascension/runtime/validation/starcaller-completion-20260909/original-contracts.json`.
The individual ledger is `findings.md` beside that capture. Class 26 uses spell family 32.
The package follows recovered client spell records, helper chains and the archived changelog;
the choices below fill gaps in that evidence. It does not claim official backend parity.

## Resources, stars and Eclipse

Mana remains the primary resource; Mana and Energy are active for restoration and regeneration.
The selected zero-cost or base-mana parents now pay their authored percentage of **maximum** mana.
A default-false `SpellInfo::UsesMaxManaForCost` keeps other spells on the native base-mana path.
All native cost modifiers remain in that path. Refunds and Font of Magic use the amount actually paid.
Moonblade restores fifteen percent missing mana. Celestial Strike/Cleave restore mana on landed
hits, including killing blows, while misses do not generate these class-specific refunds.

Eclipse acquisition starts the four-second Phase driver. Four stacks enable activation and are
reserved at its start; later gains are preserved. One eligible cast reserves the activation's
generation. Silverstream doubles cost and healing; Splash is instant and adds five-percent target
maximum health; Touch refunds its actual mana and grants one Phase. Moonflow resets its cooldown
and consumes owned stars only during Eclipse. Selected charges and temporary replacements have
generation checks so an old cast cannot consume a newly refreshed buff.

Native Lance, Trueshot, Starfire, Umbral and Starsweep consumer chains remain. Each consumption
tick reserves one caster-owned Scattered Star before damage, eight-percent maximum-mana restoration,
cooldown reduction and ten/eight-star counters. Empty or foreign stacks grant no rewards. The legacy
573282 helper's stale Starcall description does not add a new ungranted Starcall-consumption passive.
Goddess healing is enabled only by its aspect. Capacity modifiers remain native; the unadvertised
general critical-chance aura on each star is disabled.

Dancing and Celestial Glaives each multiply consumption effectiveness by 1.5. They multiply one
another; Moonwell separately doubles consumed-star damage and mana. This stacking rule is a local
choice. Goddess healing and each cooldown reduction receive the applicable effectiveness factor.
Trueshot has two charges with eighteen-second recovery, shared by its actual parent ranks.

## Conditional effects, targeting and finite state

Burning means an active harmful periodic-damage or periodic-leech effect whose school includes Fire.
Lunar Splinters guarantees Lance criticals and adds twenty-five-percent critical damage. Tyrande's
Training adds twenty-five critical percentage points and critical damage to Starfire and actual
Trueshot damage helpers. Private raw selectors are disabled for these exact records. Critical chance
worded as a percentage on Lunar Knight, Revelation and Tyrande's Guidance is treated as percentage
points; this is an explicit local interpretation.

Goddess removes the miss component of spells/abilities. Other native outcomes, including immunity,
reflection, avoidance and resistance, remain separate. Copied result helpers do not receive another
coefficient, critical roll or mitigation pass. Warden's three extra hits use actual triggering damage.
Aspect Stars forwards the distinct base amount of each of its three ranks. Starflare haste requires
actual helper damage. Scattering increases only the Warden aspect's proc chance.

Vengeance retains its native owned thirty-percent Astral damage modifier and ten-second mark;
no additional scripted distance multiplier is applied. The next-cast range buff requires successful
mark application. Lunar Charge instead has its own local factor: one percent per yard of straight-line
displacement from Shooting Star's start, capped at fifty yards. The next qualifying attack ends
active forced movement and reserves the charge once; zero distance still allows the base hit.
Every Shooting Star rank receives forced movement and temporary slow immunity.

Lunar Prophecy acts on completed Moon Arrow casts, reducing Prayer by three seconds and Moonflow/
Celestial Form by two. Starcall damage reduces Arrows in the Night by two seconds. Celestial Form
uses at most five enemies within five yards. Precision launches two blades; Pulverize has five uses
and clears its accumulated caster bonus with the parent. Starfire Power opens a five-second
Drawstring replacement. Barrage, Moonblade, free spells and healing/damage bonuses have finite selectors.

## Local numerical and visual choices

| Gap in recovered evidence | Implemented local choice |
| --- | --- |
| Lunar Combatant weapon contribution | 0.20 Arcane spell power added before normal weapon multipliers |
| Asteroid Belt | Each rating: 0.005 current mana; shield block value: 0.02 current mana |
| Starburst unnamed terms | 0.50 shield block value and 0.02 maximum mana |
| Sentinel Glaive unnamed term | 0.03 maximum mana; remove the conflicting native 0.60 SP coefficient |
| Umbral rank text says 0.20 / 0.22 AP | Use 0.20 AP for the shared helper, preserving native forwarded rank base |
| Second Moon's extra 199-point hit | One authored comet amount per affected enemy; no second unexplained damage effect |
| Moonwater's unquantified heal | Ten percent of the last effective granting heal, stored on the owned stack aura |
| Night boundary | Realm-local UTC+3, 18:00–05:59 |
| Missing saber template 912358 | Native riding Frostsaber display 9991, without a global template/model edit |

Avatar keeps native creature-template 22989 (Maiev), whose base template uses display 20628.
It must not use 22989 as a direct display ID: that display is a Lobstrok. Both selected model chains
resolve in the pinned installed DBCs. Rendering, size and animation remain unverified in game.
No client/DBC file changes are included. Existing Necromancer/Templar client dependencies remain.

The coefficient table contains 124 exact spell/effect slots. Original active rank base amounts,
native weapon components and valid chain targeting remain. SQL suppresses duplicate stock coefficients.
Healing-specific recovered formulas use bonus healing; direct damage uses Arcane spell power unless
the native weapon component already supplies its own calculation. The separate coefficient slots,
including current-mana shields and Intellect contributions, are executable-test inputs.

## Stagger and shared proc correction

Shrouded in Night delays thirty percent of direct magical damage. Refreshing the pool preserves its
next tick. Integer remainder is conserved; early aura removal settles the unpaid balance. Normal
logout settles before the native character save, and death clears the remainder. This runtime pool
does not introduce a crash-recovery database journal. It does not delay its own periodic payments.

SQL06 binds Starcaller scripts. Its original raid-group entry was corrected at linked deployment:
SQL09's intermediate group 1038 inferred stat-percent aura 137, so applied SQL10 moves Shrouded Stars
704785 into dedicated damage-taken group 2000185 with Sanctuary 67480 and Vigilance 50720. Native
inference selects aura 87; the largest reduction applies once and the independent stagger is preserved.
The deployed AuraScript registers its periodic callback only when the actual spell has a periodic
dummy aura. SQL09 removes Moonblade's inert aura binding while retaining its native/global handling.

SQL08 updates exactly 64 Felsworn/Knight proc rows from old mask 63 to 9331, adding native
BLOCK, ABSORB and FULL_BLOCK admission. Earlier SQL05/07 remain unchanged. Their earlier proc fixture
tested only six low bits and missed this failure; the new 24,948-case native test supersedes that
admission evidence. Script-level talent, ownership, hit and cooldown filters still decide actual procs.

The separately requested linked build is now installed as `class-followup-20260909`, with matching
Necromancer/Templar UI dependencies. Final startup has no new unique errors. SQL03–10 are applied and
immutable; earlier source-only receipts are historical. Combat, movement, group behavior and rendered
UI acceptance remain separate from build, native callback tests and startup validation.
