# Tinker source reconstruction — 2026-09-10

The Tinker class audit contains 139 findings: 117 implemented or extended,
21 retained native mechanisms, and one unresolved coefficient. This package is
source ready with that question open. It has not been built into a server,
installed, or accepted in gameplay.

## Evidence and unresolved scaling

The audit used active client spell descriptions and records, copied creature,
model and object records, archived changelog entries and focused source tests.
Name matches alone do not establish spell, creature or display identity.
The official backend is unavailable.

Nanobot Reconstruction's first rank 801809 has a DBC coefficient of 0.05; twelve higher ranks have
zero. The installed bonus table has no exact or first-rank override, the active descriptions expose
only calculated total/tick values, and nine archived mentions do not establish a stat term. Keep the
existing coefficients. The owned single-target lifecycle is implemented independently. Do not invent
an all-rank scaling fix from this inconsistency.

## Resource, ownership and finite effects

- Keep the existing 100 Scrap cap and Scrapper implementation. Completed player Scrap Shot and
  Sticky Bomb casts grant three and ten Scrap, including misses; triggered copies do not generate
  again. This follows the local cast-completion interpretation. Existing Makeshift hit gains remain.
- Mechsuit drains five Scrap per second, requires positive Scrap, removes its owned helpers on exit
  and interrupts its active channels at zero. Engine and entry regeneration run on actual entry,
  without treating normal aura loading as another entry reward.
- Finite procs snapshot a generation at cast start and spend it on completion. A replacement granted
  during that cast survives consumption of the old generation. Aftermath has two charges; the other
  selected bonuses have one. Normal aura charge storage preserves remaining charges.
- Gear Grind keeps its four fixed channel ticks and Makeshift charge category. Its temporary spell
  remains known until the active channel finishes, even after the selected proc has been spent.
- Only one Module kind may be active per caster across recipients. Hidden normally saved dummy aura
  707495 stores the selected identity; loaded recipients reconcile against their online owner. The
  duplicate old pet Synergy identity is replaced by the single 707278 route. Other casters stay separate.
- Copied damage/healing uses the resolved source amount and cannot add its coefficient again. Zap's
  Rejuvenating Gas is a noncritical owned HoT with an exact family 34 / 706255 native exception.
  Napalm, Oil Pylon, Drone, Sprocket marks, shields and devices check the originating Tinker GUID.

## Explicit local choices

- Owner coefficients distinguish AP, RAP, Fire spell power and bonus healing. The 77 selected effect
  slots suppress native automatic coefficients only where a matching explicit route owns the amount.
  Retained augmentation and Combustion coefficients are unchanged.
- The highest recovered Rocket Launcher rank adds 0.45 Fire SP to the child's 0.35 for 0.8 total.
  Other recovered ranks retain 0.35. Crash Site's malformed `AP+.1` term is interpreted as 0.1 AP.
  Molten Thrower's total 0.15 Fire SP + 0.085 AP is divided across its four periodic ticks.
- Device scaling inherits 30% owner Stamina and Intellect; health is 35 per level + 10 per Stamina,
  mana 15 per level + 15 per Intellect, AP 40% RAP + 40% Intellect, armor half the owner's armor,
  and base weapon damage one to 1.5 times level. Refresh preserves health/mana fractions. These are
  local reconstruction formulas. Scoped Guardian branches prevent a second native stat derivation.
- Deathball emits three devices. Over 40 yards, its damage rises from one to two times the base and
  speed from one to three times the base. Each explosion selects its target set once.
- Firepot starts five yards above the origin, follows its target while flying, drops one intermediate
  payload/oil patch and explodes on contact. Real map collision and flight appearance remain untested.
- Battle Turrets use native vehicle 116 with action slots 706689 and 706694, share health across owned
  turrets and supply the owned damage field. Vehicle choice and controllability require live acceptance.
- Heavy Spider Bomb uses free creature identity 840028, copied Spider display 408331 and local scale
  1.5. Portable Sawmill uses native woodpile display 1248 with the actual required focus 1653.
  Other new creature identities use their copied-client displays. Upgrade keeps the base appearance;
  the unmatched transform is disabled. The old Well-Oiled label maps to current 806757 Overclocked Machine.

## SQL and model dependencies

The [Tinker migration](../../../data/sql/updates/pending_db_world/rev_20260910_15_tinker_completion.sql)
contains 200 exact script bindings, 23 HIT proc rows and suppression rows for
explicit coefficients/copies.
Eight existing proc identities retain all fields except HitMask, changed to 9283 to admit normal,
critical, block, absorb and full block. Miss, dodge, parry, evade and full resist remain excluded;
the retained scripts still enforce their source, positive-damage, phase and charge rules.

There are 93 non-destructive guarded inserts: 34 creature templates, 34 model links, 16 model-info
rows, two objects and seven creature action slots. AzerothCore stores those slots in
`creature_template_spell(CreatureID, Index, Spell)`, not `creature_template.spell1/spell2`.
The read-only pre-install checker validates the actual schema and rejects conflicting definitions,
including conflicting or unexpected action slots. SQL does not overwrite a conflict by itself.

The required external server data package adds 16 displays, 14 model-data records
and one object display across three DBCs. All original records and string prefixes
are preserved. Model chains resolve; rendered assets, sizes and vehicle controls
are unverified. No client archive or Spell.dbc change is required.

Future installation must include the matching pending Pyromancer/Cultist/Sun Cleric/Venomancer/Tinker
source and SQL11–15, these three server DBC candidates, and a fresh absent-or-exact dependency check.
Reject mismatching base DBC hashes or occupied conflicting identities. Never rerun previous SQL
generators or restore an old whole source tree. Build/install still requires an explicit request.

## Verification limits

Focused tests exercise actual source functions with bounded dependencies. Native `/Zs` validates
syntax against pinned project headers; it is not server linking or runtime registration. Earlier
failed phases are retained. Four old whole-source/hash assertions are historical exclusions, not
passing regressions. The later SQL schema correction has its own replay and review; lint refresh
is not another functional regression run.

Combat, groups, pets forwarding real proc events, movement, rendered models,
vehicles, relog and replacement UI require live acceptance. The recorded source
validation did not launch a game client. Client test launches require explicit
authorization.
