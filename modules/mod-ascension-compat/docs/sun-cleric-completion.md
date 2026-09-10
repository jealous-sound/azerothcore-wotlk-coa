# Sun Cleric reconstruction policy — 2026-09-10

This package addresses the 158 original Sun Cleric audit findings:
157 implementations/extensions and one retained native critical-damage mechanism.
Class 27 uses spell family 33.
These are local source decisions, not evidence of official backend parity or live gameplay.

## Evidence and scope

The active client records and descriptions are the primary contracts. The package pins 1,150
records and the original 158 finding identities. Archived Sun Cleric changelog entries provide
context; an archive entry does not silently replace a conflicting active description. For example,
Daybreak uses 0.595 bonus healing and Heaven's Appraisal uses the active Gavel enhancements.
Corrupt Mind acquisition is outside this package and remains deferred at the user's request.

The initial source baseline contains the pending Pyromancer and Cultist packages. Shared changes
are limited to registration, contract/resource dispatch and native SpellInfo cost/equipment/spellmod
selection. All 78 captured preceding SQL files remain byte-identical. New SQL13 is unapplied.

## Dawn, Vows and finite effects

Solar Power is 0..20; its availability marker follows the actual total. Dawn requires/spends 20
and arms ten qualifying casts. Each qualifying non-triggered cast consumes one charge, including
a miss. A cast can select fulfillment at most once per second. Triggered hand hits and channel
children inherit that cast's selection; they do not consume another charge. One selected cast
dispatches the general fulfillment talents once, while its individual target results carry its
Vow modifier. Incoming spells from another player cannot borrow that player's Dawn selection.
Vow of Light therefore fulfills from a compatible own selected event, such as a self heal.

Generation is blocked while Dawn exists and during its selected result handlers, including the
last charged channel. Dawn's school-choice flag is stored in its otherwise unused effect-1 amount
so ordinary aura saves preserve whether Sunrise/Sunset has already been selected. Pendant refreshes
charges/duration without rearming that choice. Finite bonuses snapshot aura generations before
effects and consume only those generations; Hot Spot and Learning Light keep two/three charges.
Bulwark consumes only block/full-block events and preserves saved remaining charges on load.

Owned Eclipse ticks accumulate without refreshing duration or the next tick. Healing events feed
the cleric's current hostile victim when available: this is a local targeting choice for an
ambiguous healing-to-damage contract. Copy spells preserve resolved damage/effective healing and
normally suppress recursive procs. Holy Form's extra heal is an explicit exception for eligible
healing procs, but cannot recursively sacrifice mana again.

## Explicit local choices

- Grace with Final Vows uses ten percent remaining cooldown reduction. The active description
  references 807446 effect 2, whose recovered amount is zero; talent 707528 supplies ten. This is
  a reconstruction choice, not recovered official behavior. Grace's mana-cost reduction is 50 percent.
- Sun's Gift contributes 0.25 AP to Horusath Blast; its description does not specify a number.
- Sunflare uses the first recovered rank's 0.25 SP coefficient for all ranks.
- Radiant Cascade reaches the primary plus four additional allies, total five. The native field
  says four while the description calls those additional jumps; native bounce attenuation remains.
- Pious retains native critical-bonus-portion semantics for aura 163. No new total-critical-damage
  multiplier is invented from the ambiguous wording.
- Daytime means realm-local time from 06:00 inclusive to 18:00 exclusive. Conditional helpers update
  every 500 ms. They are not linked to a new client calendar or weather implementation.
- Holy Form follows the archived worked example: at full mana, 2,000 effective healing gives an
  additional 1,000 healing for 250 mana. The form's calculated effect-1 percentage scales the result,
  including Empowered Holy Form. Current mana fraction scales it down; below 30 percent it stops.
- Adjudicator's single-target 50-percent increase applies to its additional echo. Healing uses actual
  echo damage. It does not add a second modifier to the triggering Justicar hit.
- Suncharged follows the active Judgement Day contract, not the stale Suncharge/Sunflare description.
  Owned half-second applications stack up to the native cap; release multiplies the complete amount
  by `1 + 0.25 * stacks` once. The native wildcard aura removal is suppressed at HIT.
- The empty Gateway to Heaven helper is a Hegemony alias. Either enables the same Radiance extension;
  possessing both does not double the grant.

## Healing, defenses and spatial effects

Bless owns self plus one ally per cleric. Replacing that ally removes only the same caster's old
Bless. Blessings validate ownership both before casting and during effect execution. Devotions
are exclusive per recipient and caster. Copies use effective healing; Radiance's heal explicitly
uses SP, while Illumination, Cascade, Daybreak and Sunlight use their named bonus-healing terms.
Hammer of Kings reads Holy power even though its damage school is Holyfire. Justice hand hits
read Fire SP for their flat component while retaining native weapon damage.

The Chosen detects depletion after actual absorption, including native absorb bypass. Only actual
depletion releases Hope, whose damage is split across selected enemies. Divine Retribution saves
raw accumulated damage, divides by four on natural expiry and includes the original target.
Circle of Valor's split link requires the caster's ground aura; damage taking the caster below
ten percent removes the dynamic object. Health-gated and armor/Intellect-derived effects update
from current caster values, with exact plate/shield equipment selectors in SpellInfo.

Sun Gate uses new guarded creature 50331 and existing Shattrath portal display 23719/model 2721.
This native model is a local visual substitute. No DBC/client file changes are required. The gate
stays at the starting point, replaces only its owner's old gate and checks group, distance and
Sunstroke on interaction. Valkyr resolves GUIDs after its delay, starts native movement, and
releases landing damage only after the spline finishes near its destination. No delayed callback
retains a raw Player, Unit or Spell pointer. Rendering and real map movement remain unverified.

## Validation and future installation

Focused tests execute actual callback bodies with bounded dependencies. Separate native tests
execute the complete current cost function and native proc admission. Original prior-class tests
remain unchanged; only two legacy resource fixtures receive dependency declarations in the new
runner. MSVC `/Zs` compiles actual translation units against actual project headers without linking.
Validation included failed phases and reconciliation of the final input hashes;
an earlier passing phase does not establish the final source's validity.

SQL13 has two deliberately non-destructive guarded inserts for the gate/template model. Before
installation, reject conflicting definitions instead of silently reusing them. Install matching
pending Pyromancer, Cultist and Sun Cleric source/SQL together. Preserve the
preceding class implementations when integrating these changes. A linked build,
SQL application, restart and live combat, group, movement, relog and UI acceptance
are separate steps. Client test launches require explicit authorization.
