# Level scaling: the formulas

Open-world scaling, as it is implemented here. Both halves follow each character's own choice at the
Destiny Weaver (`mod-destiny-weaver`), which the character-creation screen offers as well:

* **creatures** are scaled **per viewer**. Each character is sent their own version of a creature, at
  their own level minus `DestinyWeaver.Scaling.Offset`, while the creature object keeps its authored
  level. A level 30 and a level 20 character facing the same level 15 creature see it at 27 and 17 at
  the same time, and a character with scaling off sees 15. There is no realm-wide creature lift: with
  `DestinyWeaver.Enable` or `DestinyWeaver.LevelScaling` at 0, every creature keeps its authored level.
* **quests** are per character as well (`CoA.QuestLevelScaling` is the realm switch; the character's
  own choice decides). The quest level is sent to that one client, so it can genuinely differ per
  character.

## 1. The shared header

`src/server/game/Miscellaneous/LocalLevelScaling.h`

```
ScaleCreatureLevelForViewer(original, viewerLevel, offset = 3):
    floor = max(1, viewerLevel - offset)
    return max(original, floor)                         # up only, no ceiling

ScaleDungeonCreatureLevelForViewer(original, viewerLevel, offset = 3):
    return min(ScaleCreatureLevelForViewer(...), viewerLevel + offset)

ScaleQuestLevel(original, playerLevel):
    if original <= 0: return playerLevel                # -1 = "follow the player"
    return max(min(original, 255), playerLevel)          # rebase onto the player
```

`CreatureOffset` comes from `DestinyWeaver.Scaling.Offset` (default 3). A view has no ceiling: it is
told to one client only, so the creature in front of a character comes all the way up to that
character's band. That is the whole point of the feature: content in front of a character is relevant
to that character. Inside a normal five-player dungeon the view is also held down to the viewer's
level plus the offset, because the dungeon finder admits a group well below a dungeon's authored level.

`QuestScalingEnabled(player)` is the per-character gate for **quests**. The realm switch
`CoA.QuestLevelScaling` must be on for it to be consulted at all; the resolver then
answers for one character. No resolver, or no opinion, means "take the realm default".
`mod-destiny-weaver` installs the resolver and stores the choice in `character_settings` under
`core.destiny_weaver` (index 0 = the choice, 2 = off). `ScalingChoiceEnabled(player)` is the same
choice with no realm switch in front of it, and it is what the per-viewer creature paths ask.

### A creature's stats, per character

A character with scaling on fights *their* version of a creature, and everybody else fights the
authored one, at the same time, against the same corpse. Two mechanisms carry that:

- **fields** — level, max health, health, max mana, mana are rewritten per recipient in the values
  block (`OnPatchValuesUpdate`, offsets from `ShouldTrackValuesUpdatePosByIndex`). The core builds
  one buffer per `(visible flag, update type)` and patches a copy per player, so a view never leaks
  into another client. Health is carried as the same *share*, so the bar is that version's pool.
- **levels** — `Creature::getLevelForTarget` answers with the view level
  (`LocalLevelScaling::CreatureViewLevelOwner`, installed by the module). This is the lever the rest
  of the fight hangs off, because everything level-derived in a roll is asked for through it:

| term | how it follows the viewer |
|---|---|
| spell hit and resistance tables (`MagicSpellHitResult`) | level difference, both directions |
| weapon and defence skill (`GetMaxSkillValueForLevel`, `GetUnitMeleeSkill` → `GetWeaponSkillValue`, `GetDefenseSkillValue`) | the view level × 5 |
| melee, ranged and physical-ability miss chance | `GetWeaponSkillValue(attType, victim)`, both directions |
| glancing and crushing tables | `getLevelForTarget` on both sides |
| ranged abilities that are not weapon spells | `getLevelForTarget(victim) * 5` |
| block chance adjustment | attacker skill against victim max skill, both with a target |
| stealth/detection and aggro radius | `Object::isVisibleForOrDetect`, `Creature::GetAggroRange`, `GetAttackDistance` |
| kill experience | `Acore::XP::Gain` (`Formulas.cpp`), and the gray checks in `KillRewarder` |
| armour a blow lands against | `Unit::CalcArmorReducedDamage` asks `LocalLevelScaling::ViewArmorFor` |
| damage the creature deals | `CreatureView::DamageTakenFactor` — the `creature_classlevelstats` row at the view level (`BaseDamage + AttackPower / 14`) over the same row at the authored level |
| damage the character deals to it | `CreatureView::DamageDealtToPool` takes the matching share out of the real pool, so the bar falls by exactly the number their client was shown |

`DamageTakenFactor` and `DamageDealtToPool` are both ratios of the *same* two rows, which is why the
view is one definition rather than several: level, pool, mana, armour, damage and skills all read the
row at the view level, and the factors are 1 for a character with scaling off.

### Groups: the leader sets the switch, never the level

While a character is grouped, `DestinyWeaver::LevelScalingEnabled` answers with the **leader's**
switch — on, or off, for every member. It answers with nothing else of the leader's, and that is
load-bearing rather than incidental:

- the level a creature is shown at is `ScaleCreatureLevelForViewer(original, viewer->GetLevel(), offset)` —
  the *viewer's* level;
- the level a quest is played at is `ScaleQuestLevel(questLevel, playerLevel)` — the *viewer's* level.

So a level-31 leader with scaling on and a level-12 member with it off: the member's scaling turns on
(leader's switch), and the member then meets the world at *level 12's* answer — level 9 versions of
what they can still reach, level 31 content untouched because nothing is ever lowered. The leader
sees level 28 versions of the same creatures, from their own level. Two members of one party, one
creature, two versions, and neither is derived from the leader's level. The same holds while several
of them attack the same creature: the level and pool are patched **per recipient**, the fight inputs
ask `getLevelForTarget` **per opponent**, and nothing about a creature is made universal.

The switch is a group override, never a write: no member's stored choice is touched, so leaving or
being disbanded restores exactly what they had, and leadership handed over is read live on the next
query. What needs help is not the rule but the *wire*:

| event | what changed | who is re-sent |
|---|---|---|
| leader's switch flipped at the Weaver | the group's switch | the actor and every online member |
| member added | the newcomer now follows the leader | the newcomer |
| member removed | they go back to their own choice | that member |
| **leader** removed or disconnected | every member's switch is gone | every remaining member |
| leadership handed over | the new leader's switch | every member |
| group disbanded | everyone back to their own choice | every member |
| leader logs in, or is handed the lead | their switch applies again | every member |

A client caches the level and pool it was told for every creature, and the quest data it was told for
every quest; only a fresh message replaces either. So each of those events calls
`DestinyWeaver::RefreshClient` / `RefreshGroup` / `RefreshScalingClients`, which records the client
in a small registry (`g_viewRefresh`, guarded, holding the guid, a `CreatureUntil` stamp, a
`QuestLogDue` flag and the creatures already sent this episode). The registry is served on the thread
that owns the thing being refreshed — never a packet across threads:

- **creatures**, `destiny_weaver_view_refresh_script` (`AllCreatureScript`): a relaxed atomic read per
  creature update while nothing is pending, and while something is, the first pending viewer within
  `VIEW_REFRESH_RANGE` gets its five view fields marked changed. The creature then broadcasts those
  fields once, exactly as it would after any change, and `OnPatchValuesUpdate` rewrites them per
  recipient on the way out — so every viewer is refreshed to its own version, a viewer whose view did
  not move receives its authored values, and the window (`VIEW_REFRESH_WINDOW_MS`) is what catches
  creatures that had not ticked yet without broadcasting for ever.
- **the quest log**, `destiny_weaver_view_client_script` (`PLAYERHOOK_ON_UPDATE`): on the character's
  own thread, once per episode, `RefreshQuestLogQueries()` — the same call the Weaver's own toggle
  makes, and the reason a member's log follows a leader's switch.

**One consequence worth knowing:** a group is one group, so a leader who logs off leaves their switch
unreadable (`ObjectAccessor::FindConnectedPlayer` finds nobody) and every member falls back to their
own choice — the logout path above refreshes them so the change is never stale, and their choice is
restored when the leader returns. Excluding battleground and arena groups from the override would be
a one-line change if a stranger leading a BG group should not decide anyone's scaling.

### What the character is told

A switch that only the menu reports is a switch nobody notices mid-fight, so each real change is
announced in the middle of the screen (`SMSG_NOTIFICATION`, one packet per line, the same opcode the
realm's autobroadcasts use) and in the chat log, with the state word coloured and the rest plain
yellow. There are two sentences, because there are two different things to say:

- **the group's**, when what moved is the group's switch — `Your group has Level Scaling ENABLED` /
  `DISABLED`. Sent by `NotifyGroupScaling`.
- **the Weaver's own**, when the change is the character's own — `You have enabled open world creature
  scaling!` / `You have disabled open world creature scaling!`, the off line carrying the second
  sentence the live realm showed with it: `Quest items and credits will not be awarded if creatures
  are grey level.` Sent by `NotifyPersonalScaling`.

Which one a change gets is decided by `GroupScalingApplies` — a group sentence is only true while
there is a leader present to have a switch, so a character who left, was disbanded, or lost their
leader to a logout is answered in the Weaver's words instead, handing them their default back.
Leaving a group and being disbanded pass `remindDefault`, which makes the Weaver's line speak
**whether or not the state moved**: they are a reminder of what the character is left holding, not a
report of a change, and a character whose own choice happens to match their group's would otherwise
hear nothing at all. The reminder is the Weaver's line even while a group is still on their screen,
because that is what it is about — and the value it names comes from `PersonalLevelScalingChoice`,
never from the effective state: inside a group hook the effective answer is still the leader's switch
(the character has not finished leaving), so naming it told people the opposite of their own setting.
A leader leaving is *not* a reminder, because those members are handed the lead to one of them and end
up on a leader's switch rather than their own; it stays a change-only report in the group's words.
The
acting character is always answered in the Weaver's words, because they are the one who threw it:
`SetLevelScaling` calls `NotifyScalingSelf`, which records the state they are now being *shown* (the
leader's switch, while the group's is in effect) and then speaks, so the refresh that follows cannot
announce the same change back to them in the group's voice. `g_toldState` is the mechanism — what
each client currently believes, seeded on login, compared on every mark — so one real change is one
message whoever caused it and no event can double-notify. `g_lastSpoken` is the second half of
that guarantee: two hooks genuinely describe one event, because a member leaving a two-person group
fires the removal *and* the disband, and both hand the same member the same default. So a repeat of
the **same** state inside `SPEAK_REPEAT_WINDOW_MS` is the same news and is not spoken again - while a
change that moves still says so whichever way it moves and however quickly.

**The trap that made all of this look like it worked while none of it did.** The core dispatches a
unit hook only to the scripts that registered it — `CALL_ENABLED_HOOKS` walks
`ScriptRegistry<UnitScript>::EnabledHooks[hook]`, which is filled from the script's constructor
list — so an override that is not listed is never called and the fight silently keeps the authored
numbers. Module scripts that override `ModifyMeleeDamage`, `ModifySpellDamageTaken` or
`ModifyPeriodicDamageAurasTick` must name those hooks in their constructor. `DealDamage` is the
exception: it is dispatched to every registered unit script.

Not scaled, deliberately, and matching the reference implementation: **resistances** (template-based
and level-independent there too) and **loot**, which is one corpse shared by everyone who tagged it.

### Many characters, one creature

The question a crowded realm asks is: *can two characters change each other's world, and does anything
grow or race?* Audit result, by mechanism:

| property | how it holds |
|---|---|
| **No shared mutation.** | Nothing here writes a creature's level, health, stats or flags. The only writes are the transient values *mask* of a creature (`ForceValuesUpdateAtIndex`) and the per-recipient copy of the packet. Two characters cannot see each other's version, and a character with scaling off sees the authored creature exactly. |
| **Per-recipient delivery.** | `Map::SendObjectUpdates` builds one buffer per player and `Unit::PatchValuesUpdate` rewrites fields for that target; our five fields (`UNIT_FIELD_LEVEL`, `MAXHEALTH`, `HEALTH`, `MAXPOWER1`, `POWER1`) are registered through `ShouldTrackValuesUpdatePosByIndex`, which is called only for fields already in the update mask — so no bandwidth is added to a block that did not already carry them. |
| **Damage in both directions** | resolves the viewer from the unit that owns the hit (`GetCharmerOrOwnerPlayerOrPlayerItself`), never from "a player nearby". A pet's blows count as its owner's; a creature's blows on a pet follow the owner's view too, deliberately, because pets level with their owner — and pets themselves are never *given* a view. |
| **Lock discipline.** | One mutex (`g_viewRefreshLock`) guards the refresh registry and the three notification maps; nothing sends a packet while holding it, and the send is made on the thread that owns the client. The hot creature pass reads one relaxed `atomic<uint32>` count first and does nothing else while no refresh is pending. |
| **State lifetime.** | The refresh registry is erased when the episode is served or expires, and by `ForgetClient` on logout; `g_toldState` (what each client believes) and `g_lastSpoken` are erased on logout with it; `g_leadersSeen` holds one entry per online leader. No map is keyed by creature, so nothing accumulates per spawn. |
| **Hot-path cost.** | The realm switches and the offset are cached in atomics at config load (`g_scalingAvailable`, `LocalLevelScaling::CreatureOffset`), so `ViewFor` asks the config system nothing; it early-outs on "character has scaling off", then on the object checks, then on reaction, and only then reads the two `creature_classlevelstats` rows. |
| **`.reload config`.** | The resolvers and the cached switches are (re)installed in `ApplyTuning()` on `WORLDHOOK_ON_AFTER_CONFIG_LOAD`, so turning the feature on or off takes effect on the next creature rather than on the next restart. |

What the per-viewer model does not do: nothing reads a creature object at its view level. A script, a
creature that inspects itself, or a loot table keyed on level sees the authored creature. That is the
price of two characters fighting one wolf at two levels.

### How often the character is told

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

## 2. Quest experience

`Quest::XPValue(playerLevel, levelScaling)`

```
questLevel = levelScaling ? ScaleQuestLevel(Level, playerLevel)
                          : (Level == -1 ? playerLevel : Level)

diff = clamp(2 * (questLevel - playerLevel) + 20, 1, 10)
xp   = diff * QuestXP[questLevel][RewardXPDifficulty] / 10
xp   = round to 5 / 10 / 25 / 50 by the size of xp
```

A scaled quest therefore pays what a quest of its *effective* level pays. `QuestDef.h` carries the
`levelScaling` argument down from every caller so the choice is consulted per player, not per realm.

### Keeping the quest log honest

The client caches a quest's data by quest id, across characters and sessions, so once it has been
told a quest's level and rewards it keeps them until it is told again — which is how a quest picked
up with scaling on keeps showing that copy after the character turns scaling off. `Player::
RefreshQuestLogQueries()` re-sends the query response for every quest in the log, and is called:

* on **login** and on **level-up** (the CoA server component, gated on the realm switch, because a
  character with scaling off needs the resend just as much — the client is holding the scaled copy);
* on **accept** (`Player::AddQuest`), since the copy may have come from another character;
* whenever the character **changes the choice** (`DestinyWeaver::SetLevelScaling`), which is the one
  that was missing: the toggle used to change the server's answers while the open log still showed
  the numbers from pickup time.

## 3. Quest money

`Quest::GetRewOrReqMoney(playerLevel, levelScaling)`

```
rewardedMoney = RewardMoney                            # the authored value
if RewardMoneyDifficulty is a real tier (1..9):        # stock data
    rewardedMoney = QuestMoneyReward[playerLevel][tier]
elif levelScaling:
    tier = FindMoneyTier()                             # recovered, see below
    effective = ScaleQuestLevel(Level, playerLevel)
    share = Level * 100 / effective                    # how much of the range the quest spans
    keep  = 60 + 40 * share / 100                      # 60% at the extreme, 100% at your level
    rewardedMoney = RewardMoney * QuestMoneyReward[effective][tier] * keep
                                / (QuestMoneyReward[Level][tier] * 100)
    rewardedMoney = max(rewardedMoney, RewardMoney)    # a reward never shrinks
return rewardedMoney * Rate.RewardQuest.Money
```

### Why the discount, and why 60%

The point of scaling is that no zone is dead: content far below your level has to be worth playing
or the world shrinks back to the current level band. But the reward class says nothing about
level — the median tier is **5 in every level band**, from 1-10 through 71-80 — so the table cannot
tell a level 10 quest from a level 45 one of the same class. Paid at full strength across a large
gap, a rich low-level quest would pay exactly what a rich level-appropriate quest pays while being
trivial to complete, which turns the easy content into the profitable content.

The discount is the balance. Measured at player 50, as a share of what a normal level-50 quest pays
(`QuestMoneyReward[50][5] = 75s`):

| quest's own level | 5-9 | 10-14 | 15-19 | 20-24 | 25-29 | 30-34 | 35-39 | 40-44 | 45-49 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| median payout, no discount | 100% | 100% | 100% | 100% | 100% | 100% | 100% | 100% | 100% |
| median payout, 60% floor | 65% | 68% | 73% | 78% | 81% | 84% | 90% | 93% | 99% |

Old content pays two thirds of what level-appropriate content pays and rises steadily toward it, so
there is no dead zone — but nothing can ever pay *more* than the same class pays at your level, so
old content is never the most profitable content. Two properties are enforced and verified over
every money-paying quest at players 20/40/50/60: **nothing is ever lowered** (0 quests below the
authored value) and **nothing exceeds the level-appropriate price** (0 quests above
`QuestMoneyReward[playerLevel][tier]`). Pairs where a lower-level quest pays more than a
higher-level one fall from 25.9% to 25.1% overall and from 22.3% to 20.0% across gaps of 20+ levels;
the remainder is the source data's own 7.6% — the class, not the level, decides whether a quest is
rich, and it does so in the original data too.

### Tuning

The floor is not compiled in — it is `DestinyWeaver.Scaling.QuestMoneyKeepShare` in
`configs/modules/destiny_weaver.conf`, read into `LocalLevelScaling::QuestMoneyKeepSharePercent` by
`mod-destiny-weaver` on startup and on every config load, so `.reload config` retunes it without a
restart. `modules/mod-destiny-weaver/src/destiny_weaver_scaling.cpp` logs the live values:

```
scaled quest rewards keep 60/100% of the level-appropriate money/experience at the far end of the
level range (100 = no discount)
```

60 is the balance point; 50 is the tighter reading (57% for the worst band, 23.9% inversions) and 75
the looser one (78%, 25.9%).

### The same discount for experience

`DestinyWeaver.Scaling.QuestXpKeepShare` applies the identical curve to `Quest::XPValue`, and is
left at **100 — no discount** deliberately. The promise scaling makes is that scaled content always
awards experience, and levelling through the old zones is exactly what the system exists to allow;
a discount there would put the dead zones back, which is the failure money has to avoid but
experience must not. Lower it (60 matches the money curve) when the goal is instead to bound how
much of a character's progression can come from content far below them.

**This realm's data does not name a tier.** `quest_template.RewardMoneyDifficulty` holds the
client's `RewMoneyMaxLevel` value instead — quest 7 carries 67, which is exactly what the live
client cache stores for that quest, and it is never a usable index (`MAX_QUEST_MONEY_REWARDS = 10`),
so the stock lookup always fails and every quest used to pay its flat authored value at every level.

`FindMoneyTier()` recovers the missing index: `QuestMoneyReward[QuestLevel][tier]` is compared with
`RewardMoney` and the closest tier wins. Over the realm's 3 201 money-paying quests, 3 115 (97.3%)
match *exactly* at the quest's own level, 36 are within 25%, 3 have no usable row. Because the
ratio is 1 whenever the effective level equals the quest's own level, unscaled play — and a scaled
quest held by a player below its level — keeps paying exactly what it pays today.

## 4. Consequences of the shape

| | player 3 | player 50, scaling off | player 50, scaling on |
|---|---|---|---|
| quest 7 "Kobold Camp Cleanup" (level 2, 25c, 67 = max-level money) | | | |
| effective quest level | 2 | 2 | 50 |
| experience | 170 | 15 | 6 800 |
| money before | 25c | 25c | 25c |
| money after | 25c | 25c | **33s 55c** |

`QuestXP[2][5] = 170`, `QuestXP[50][5] = 6810`, `QuestMoneyReward[2][4] = 25`,
`QuestMoneyReward[50][4] = 5500` — quest 7's recovered tier is 4.

The XP figure for the scaled case is the same value a native level-50 quest of the same
`RewardXPDifficulty` pays, and the money follows the identical principle.

The high-level half, same shape (both quests are level 55, quest 12801 tier 8 / quest 5060 tier 7):

| | player 55, scaling off | player 80, scaling off | player 80, scaling on |
|---|---:|---:|---:|
| quest 12801 "The Light of Dawn" — eff. level | 55 | 55 | 80 |
| experience | 16 350 | 1 650 | 44 100 |
| money before / after | 3g 33s | 3g 33s | 3g 33s → **25g 75s 20c** |
| quest 5060 "Locked Away" — money | 2g 50s | 2g 50s | 2g 50s → **19g 31s 40c** |

The ratio is the table's own, per tier and level band (×8.9 from 55 to 80 for both tiers), less the
discount for the 25-level lift (keep 87%).

Because scaling only ever raises a quest's level, a level-55 quest held by a level-40 or level-55
player is untouched — effective level 55, ratio 1, authored money. And at max level the payout is
consistent by construction: the "money instead of experience" part is `GetRewMoneyMaxLevel()`
(`XPValue(80) × 6c`, already level-driven) *plus* the reward money, so a scaled low-level quest at
80 now totals what a native level-80 quest of the same tier pays, less the discount — 13g 23s +
3g 48s = 16g 71s for quest 7, where before it paid 13g 23s + 25c.

### Edges

* `quest_money_reward` stops at level 80; `QuestXP` goes to 100. The three quests above 80 have no
  table row, so the lookup returns 0 and the formula leaves their money at the authored value.
* 137 quests carry `QuestLevel <= 0` ("follow the player") and pay money — 6 421 891c between them.
  `ScaleQuestLevel` gives them the player's level, but there is no own-level row to form the ratio
  from, so their money does not scale. They are almost certainly custom content; giving them a rule
  needs a decision, not a formula.

## 5. Open

* `Quest::GetRewMoneyMaxLevel` (the "money instead of experience" payout at max level) is wired to
  the same flag, but whether it should use the scaled or the original level is a live-tuning
  question, not a formula one.
* The client's own "this quest is scaled" marker — the quest log draws grey rather than green —
  is set by a client-side flag whose packet field is still unidentified. Accepted quests are
  re-sent on accept and on login (`RefreshScaledQuestQueries`), which fixes a stale *level*, not
  the colour.
