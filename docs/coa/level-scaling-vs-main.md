# Open World Scaling — how the per-character system relates to the existing realm-wide path

This branch adds a per-character scaling system on top of the realm-wide lift that already exists here.
The two do not have to agree, because they are not the same question; the tables below state, row by row,
how each thing the realm-wide path does is either carried, superseded or deliberately not repeated —
including the fixes made to it (`CreatureMaxLift`, and the quest-level re-send on login and accept), and
the exclusion list (pets, totems, triggers, critters, non-combat pets, charmed or owned units, scripted
private instances) that the per-viewer path now applies as well. The last sections are what a crowded
realm needs from the rest of it, and what a reviewer should look at.

---

## 1. What `main` has today

| where | what |
|---|---|
| `src/server/game/Miscellaneous/LocalLevelScaling.h` | the shared rules: `CreatureEnabled`, `QuestEnabled`, `CreatureOffset{3}`, `ScaleCreatureLevel()`, `ScaleQuestLevel()` |
| `src/server/game/Entities/Player/PlayerQuest.cpp:45` | `Player::GetQuestLevel` asks `ScaleQuestLevel(quest->GetQuestLevel(), GetLevel())` |
| `src/server/game/Quests/QuestDef.cpp:202` | `Quest::XPValue` prices a quest at its scaled level |
| `src/server/coa/AscensionCompat.cpp` (`AscensionCompatLevelScalingScript`, from line 5794) | the creature half: lift the **object** |
| `AscensionCompat.cpp:335-337` | config: `CoA.LevelScaling`, `CoA.QuestLevelScaling` |

The creature half in one paragraph, because everything else is a comparison with it: on
`OnBeforeCreatureSelectLevel` the authored level is remembered in a per-guid map; on
`OnAllCreatureUpdate` every **1000 ms**, and only while the creature is **alive, out of combat and at
full health**, `DesiredLevel()` is recomputed as the **maximum** of `original` and
`playerLevel - CreatureOffset` over every player who is alive, not a GM, in the same phase, within the
creature's **sight range** and able to attack it (`Player::IsValidAttackTarget`). If that differs from
the creature's level, `creature->SelectLevel()` re-reads the stock health/mana/attack-power/damage
curves and the armour is re-applied by hand under `UNIT_MOD_ARMOR`. `CanScale()` refuses pets, totems,
triggers, critters, non-combat pets, anything with a charmer or owner, and scripted private instances.
`OnCreatureRemoveWorld` cleans the map.

### The two fixes the question is about

* **`#4232` — "bound creature level scaling, and follow the nearest player"** (`StevenLeclerc`). It
  added a ceiling (`CreatureMaxLift`, `ScaleCreatureLevel` clamps the floor to `original + lift`) and
  replaced "the highest-level player in sight range decides" with "the nearest one does". The commit
  message is explicit about why: measured on a realm running 200 bots, a level-30 player crossing a
  starting zone turned its wildlife into level 27 and the level-1 bots fighting them died to creatures
  that were never theirs.
* **`#4240` — "resend scaled quest levels on login and accept"** (`Roddan`). The client caches
  `SMSG_QUEST_QUERY_RESPONSE` by quest id across characters and sessions, so a quest cached earlier kept
  its old level and showed grey even with scaling on; the fix re-sends on accept, on login, and on every
  level change.

## 2. Do we account for them?

| the realm-wide fix | in this branch | where | notes |
|---|---|---|---|
| `#4232` the ceiling (`CreatureMaxLift`) | **carried, verbatim** | `LocalLevelScaling.h`, `ScaleCreatureLevel()` | same `max(original, min(floor, original + lift))`, applied before the max, so nothing is ever lowered. Our default is `0` (no ceiling) because with per-viewer scaling the failure it guards against cannot happen — see the next row — but a mixed realm can set it and gets `main`'s behaviour. |
| `#4232` "follow the nearest player" | **superseded, and then some** | `mod-destiny-weaver/src/destiny_weaver_scaling.cpp` → `ViewFor()` | There is no shared lift left to attribute to anybody: a view is built per **(creature, viewer)** and delivered per recipient, so a level-30 cannot change what a level-1 sees at any distance, nearest or furthest. The bug the PR fixed is *structurally* gone rather than narrowed — which is the honest way to say "we account for it". |
| `#4240` quest level re-send | **carried and extended** | `destiny_weaver_scaling.cpp` refresh registry; `destiny_weaver.cpp` `SetLevelScaling()` | Re-sent on accept, on login and on level change (`main`'s three cases), and additionally when the Weaver's switch flips, when a character joins or leaves a group, when leadership changes, when a group disbands, and when a leader logs in or out — because the effective level can move for all of those here (the group's leader sets the switch for the party). |
| `CanScale()` exclusions — pets, totems, triggers, critters, non-combat pets, charmed/owned units, scripted private instances | **was missing, now carried** | `ViewableCreature()` | This was the real gap the question exposed: our per-viewer path scaled *anything*, which meant a creature that belongs to somebody (a hunter's pet, a warlock's summon, a driven vehicle) could be handed a view, and — worse — the damage hooks would scale blows against it. Now mirrored term for term. |
| only creatures a player "can attack" scale | **carried in a stable form** | `ViewableBy()` | `main` asks `Player::IsValidAttackTarget`, which folds hostility together with stealth, invisibility, immunity and flags — all of which change at runtime. A client *caches* the level it was told, so a view keyed on that set would appear and vanish as a creature stealthed or walked out of line of sight. We ask the reaction instead (`GetReactionTo <= REP_NEUTRAL`): hostile and neutral creatures scale, friendly ones (vendors, trainers, quest givers, guards of your own faction) keep their authored level — the same outcome `main` gets, without the flapping. |
| GMs excluded | **deliberately not** | `ViewableBy()` | `main` excludes them because their level lifts the creature *for everyone nearby*. A view here is one client's, so a GM who turned scaling on sees what a player would; a GM with it off changes nothing. |
| no re-levelling during a fight | **structurally unnecessary, kept in spirit** | `OnPatchValuesUpdate()` | `main`'s freeze exists because `SelectLevel()` resizes the object mid-fight. We never touch the object: the pool is shared and the bar is the *share* of it, so a values update cannot heal or resize anybody's fight. What does change mid-fight is the number a viewer is shown, and only if their own level or their choice moved. |
| armour after `SelectLevel()` | **carried** | `ViewArmorFor()` + `Unit::CalcArmorReducedDamage` | Same row (`GenerateArmor`), except it is resolved per viewer instead of written onto the object. |
| lift-only, never lower | **carried** | `ScaleCreatureLevel()` | unchanged, both directions. |
| offset 3 | **carried, now configurable** | `DestinyWeaver.Scaling.Offset` → `LocalLevelScaling::CreatureOffset` | same default. |
| quest XP priced at the scaled level | **carried** | `QuestDef.cpp` + `PlayerQuest.cpp` | plus the rewards `main` has no rule for (money) and the toggle that turns quest scaling off again. |

## 3. Feature comparison

| capability | realm-wide path | this branch | where the per-character one lives |
|---|---|---|---|
| who scales | everybody on the realm (`CoA.LevelScaling`) | each character's own choice, on by default | `destiny_weaver.cpp` (`LevelScalingEnabled`, the Weaver menu), config `DestinyWeaver.LevelScaling.Default` |
| creature level | the object, lifted once for all | per recipient, sent only to them | `ViewFor` + `OnPatchValuesUpdate` |
| creature health / mana | object health (a genuine pool change) | the viewer's share of the one shared pool | `OnPatchValuesUpdate`, `DamageDealtToPool` |
| creature stats (armour, attack power, weapon damage, skills) | `SelectLevel()` + armour fix-up, object-wide | the `creature_classlevelstats` row at the view level, per viewer | `StatsAt`, `HitFrom`, `ViewArmorFor`, `ViewLevelForCore` |
| what a creature does to you | object stats vs your object stats | scaled to your version (`ModifyMeleeDamage`, `ModifySpellDamageTaken`, `ModifyPeriodicDamageAurasTick`) | `destiny_weaver_scaling.cpp` |
| hit / crit / dodge / parry / glancing | stock, against the lifted object | rolled at the view level via `Unit::getLevelForTarget` | core hook + `ViewLevelForCore` |
| kill XP | stock, against the lifted object | the view level, per killer | `KillRewarder.cpp` + `Acore::XP::Gain` |
| kill reputation | stock | the view level | `Player::RewardReputation` |
| "grey to you" gates (soul shards, DK effects, achievements) | stock, on the object level | the view level | `Player::isHonorOrXPTarget` |
| quest level | realm-wide, per quest (`ScaleQuestLevel`) | per character, and re-sent to the client | `PlayerQuest.cpp`, `TileDef`/`QuestDef`, refresh registry |
| quest XP | priced at the scaled level | same, plus a configurable keep-share | `LocalLevelScaling::QuestXpKeepSharePercent` |
| quest **money** | not scaled at all | scaled from the reward tier plus its own keep-share | `Quest::MoneyValue`, `LocalLevelScaling::QuestMoneyKeepSharePercent` |
| quest log showing a **scaled** level/greying | re-sent on accept/login/level change (`#4240`) | same, plus group and toggle events | refresh registry |
| turning it off | realm-wide only | per character, and the group leader's switch overrides it while grouped | `LevelScalingEnabled`, group script |
| weaver NPCs and their menu | absent | restored (see `restoration-map.md`) | `mod-destiny-weaver` |

## 4. Multi-player readiness

The question a crowded realm asks is: *can two characters change each other's world, and does anything
grow or race?* Audit result, by mechanism:

| property | how it holds |
|---|---|
| **No shared mutation.** | Nothing here writes a creature's level, health, stats or flags. The only writes are the transient values *mask* of a creature (`ForceValuesUpdateAtIndex`) and the per-recipient copy of the packet. Two characters cannot see each other's version, and a character with scaling off sees the authored creature exactly. |
| **Per-recipient delivery.** | `Map::SendObjectUpdates` builds one buffer per player and `Unit::PatchValuesUpdate` rewrites fields for that target; our five fields (`UNIT_FIELD_LEVEL`, `MAXHEALTH`, `HEALTH`, `MAXPOWER1`, `POWER1`) are registered through `ShouldTrackValuesUpdatePosByIndex`, which is called only for fields already in the update mask — so no bandwidth is added to a block that did not already carry them. |
| **Damage in both directions** | resolves the viewer from the unit that owns the hit (`GetCharmerOrOwnerPlayerOrPlayerItself`), never from "a player nearby". A pet's blows count as its owner's; a creature's blows on a pet follow the owner's view too, deliberately, because pets level with their owner — and pets themselves are never *given* a view, matching `main`'s exclusion. |
| **Lock discipline.** | One mutex (`g_viewRefreshLock`) guards the refresh registry and the three notification maps; nothing sends a packet while holding it, and the send is made on the thread that owns the client. The hot creature pass reads one relaxed `atomic<uint32>` count first and does nothing else while no refresh is pending. |
| **State lifetime.** | The refresh registry is erased when the episode is served or expires, and by `ForgetClient` on logout; `g_toldState` (what each client believes) and `g_lastSpoken` are erased on logout with it; `g_leadersSeen` holds one entry per online leader. No map is keyed by creature, so nothing accumulates per spawn. |
| **Hot-path cost.** | The realm switches and the offset are cached in atomics at config load (`g_scalingAvailable`, `LocalLevelScaling::CreatureOffset`), so `ViewFor` asks the config system nothing; it early-outs on "character has scaling off", then on the object checks, then on reaction, and only then reads the two `creature_classlevelstats` rows. |
| **`.reload config`.** | The resolvers and the cached switches are (re)installed in `ApplyTuning()` on `WORLDHOOK_ON_AFTER_CONFIG_LOAD`, so turning the feature on or off takes effect on the next creature rather than on the next restart. |
| **Two scaling systems at once.** | the CoA server component's realm-wide lift and this one must not both be on: a character with scaling *off* would have the world raised around them anyway. We log a loud error at config load if `CoA.LevelScaling = 1` while the per-character system is on (`ApplyTuning()` in `destiny_weaver_scaling.cpp`). Keep it `0`. |

## 5. Notification spam

Every player-facing message this feature sends, and every trigger it has:

| trigger | who is messaged | how often |
|---|---|---|
| a character throws the switch at the Weaver | that character | once per click |
| the leader's switch changes while grouped | each online member | once per member per change |
| a member joins a group whose switch differs from theirs | that member | once |
| a member leaves, a group disbands | each affected member, in the Weaver's words | once |
| a leader logs out / hands over the lead, and the switch moves | each member whose state moved | once |
| **any periodic path** | — | **none: nothing here is sent from a timer, a creature update, an aura tick or a combat hook.** |

Against a crowded realm, the arithmetic is bounded by *real changes*: a group of 40 where the leader
toggles costs 40 lines, one per member, and nothing else for as long as the switch stays put. Two
guards keep a single event from being announced twice — `g_toldState` (what each client already
believes) makes a non-change silent, and `g_lastSpoken` suppresses a repeat of the *same* state inside
2 seconds, which is what stops the removal-plus-disband pair a two-person group fires from saying the
same thing twice. What is *not* coalesced, on purpose: toggling on, off, on again is three pieces of
news, and a realm's own social pressure is a better brake on that than a silent client.

## 6. For the PR

* The header changes are **additive**: `CreatureMaxLift`, the two `…KeepSharePercent` values, the
  three resolver slots (`QuestScalingOwner`, `CreatureViewArmorOwner`, `CreatureViewLevelOwner`) and
  the `ScalingChoiceEnabled` / `ViewArmorFor` / `ViewLevelFor` accessors. With no owner installed every
  one of them is inert, so `main`'s current behaviour is byte-for-byte what a realm gets without the
  module — which is the compatibility claim, and it holds by construction rather than by review.
* Core touch points are the ones a reviewer must see: `Unit::getLevelForTarget` and
  `Unit::CalcArmorReducedDamage` (through the resolvers), `KillRewarder`, `Acore::XP::Gain`,
  `Player::isHonorOrXPTarget`, `Player::RewardReputation`, `QuestDef`/`PlayerQuest`, `GossipDef`.
* `AscensionCompatLevelScalingScript` should **stay** as the realm-wide fallback for realms that never
  install the module, gated by `CoA.LevelScaling` as it is today; the error above is what
  makes a realm that runs both notice.
* Config keys this adds: `DestinyWeaver.Enable`, `DestinyWeaver.LevelScaling`,
  `DestinyWeaver.LevelScaling.Default`, `DestinyWeaver.Scaling.Offset`,
  `DestinyWeaver.Scaling.QuestMoneyKeepShare` (60), `DestinyWeaver.Scaling.QuestXpKeepShare` (100),
  `DestinyWeaver.ExperienceBonusControl`, `DestinyWeaver.DisplayStream.Enable`.
* What we are **not** claiming: `main`'s object-level model has one property ours does not — a creature
  is *genuinely* that level, so anything that reads the object (a script, a creature that inspects
  itself, loot tables keyed on level) sees it. In our model those readers still see the authored
  creature. That is the price of two characters fighting one wolf at two levels, and it is worth
  stating in the PR rather than discovering in review.
