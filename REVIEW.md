# Reaper sweep review (branch `fix/reaper-sweep`)

This file covers two rounds of work on `fix/reaper-sweep`:

- **Round 1:** a line-by-line review of the five original fix commits (kept below as history).
- **Round 2:** the branch was reconciled with lostmind84's open PR #5416 on jealous-sound/azerothcore-wotlk-coa,
  and the still-broken issues were fixed from John's CoA client DBC and Ascension DB data.

Nothing was compiled or run in game in this sandbox. Every change still needs a build and the gameplay runs on
the PC.

## Current state

| Issue | Verdict | Commit(s) on this branch |
|---|---|---|
| #3335 Harvest Time | DROPPED: #5416 fixes it (ed4942c) | reverted in 9b66cb0 |
| #1339 Dominion | DROPPED: #5416 fixes it (eb9bdf8). See the auto-attack finding below | reverted in 25cf774 |
| #494 Soulstorm | FIXED-IN-REVIEW (40%) | 488dc5c |
| #2663 Crimson Death | FIXED-IN-REVIEW (20%, hit phase) | 488dc5c |
| #4244 Ghost Claw | FIXED-IN-REVIEW (any kill; charge refund on 807234) | 3737830 (old versions reverted in 466595c, 8c3e1d9) |
| #1135 Soulbender | DEFERRED: reverted, the data is contradictory | reverted in a2519cd |
| #2669 Anima Ambusher | FIXED-IN-REVIEW | 319173e |
| #2684 Mind Screech | FIXED-IN-REVIEW | ee34996 |
| #3096 Unbound | Left to #5416 (eb5776d wires both 706785 effects) | none |
| #3493 Blood Binding | FIXED-IN-REVIEW | 332d361 |
| #5361 Tormented Souls, Soulfused Constitution | FIXED-IN-REVIEW | 5cf1cd1 |
| #2659 Harnessed Life | FIXED-IN-REVIEW (native once #5361 procs; test only) | 230e91a |
| #4227 Spectral Scythe | DEFERRED: the data does not settle the damage or the display | none |
| #5351, #5377 | Out of scope for this branch, as instructed | none |

- **Reverts:** all of them are `git revert` commits. There were no force-pushes, PRs or issue comments.
- **Trial merge with #5416:** a local `git merge --no-commit pr-5416` on top of this branch merges with no
  conflicts. It was aborted afterwards.

## Round 2: reconciliation with PR #5416

### What #5416 contains and how it was read

- #5416 was fetched read-only with `git fetch https://github.com/jealous-sound/azerothcore-wotlk-coa
  pull/5416/head`.
- Its 40 commits and diff were read. They contain no mention of Soulbender or Ghost Claw.
- The PR description and comments could not be read: the anonymous GitHub API returned 403. The Soulbender and
  Ghost Claw findings below therefore come from John's summary of the PR.

### Harvest Time (#3335) and Dominion (#1339): reverted (9b66cb0, 25cf774)

- **Harvest Time:** #5416 ed4942c changes the same `HarvestTimePreserves` lines. It reads the 50% from
  803995's effect 1 (SPELLMOD_CHANCE_OF_SUCCESS, value -50).
- **Dominion:** #5416 eb9bdf8 adds Dominion to the same `ApplyAscensionReaperSoulInfusionGained`. With both
  changes in place, the 804000 armor buff was cast twice per gain.
- **New finding for #5416, DEFERRED to that PR:** eb9bdf8's own message says 803999 ships DBC ProcFlags 4
  (DONE_MELEE_AUTO_ATTACK).
  - The Reaper-wide "ProcTypeMask 0" claim in REAPER_HANDOFF.md is therefore not true for Dominion. Our old
    review also wrongly said there was no second proc path.
  - With no `spell_proc` row, `SpellMgr::LoadSpellProcs` builds a default entry from DBC flags
    (`SpellMgr.cpp:2203-2280`). So Dominion can also proc 804000 on the Reaper's own auto attacks, both with
    #5416 and with our reverted version.
  - **In game:**
    1. Learn 803999 and auto-attack with no Soul Infusion.
    2. Expected per the tooltip: no 804000 buff.
    3. If it appears, the fix is a `spell_proc` row or `DoCheckProc` that never matches.

### Soulstorm (#494) and Crimson Death (#2663): 488dc5c (replaces c79ca0f, reverted in 61bc026)

- **Tooltip chances:** 705403 is 40% and 705414 is 20% (the issue list's live tooltips).
- **Why the old file had to go:** our old DELETE+INSERT file `rev_20260927_95` sorted after #5416's
  `rev_20260927_18`, which is an UPDATE of `SpellPhaseMask` to 2. It would have quietly reset Crimson Death to the
  cast phase.
- **#5416's evidence for the hit phase** (f8cb127): a cast-phase recast of Slaughter has no target and fails with
  cast result 27.
- **The new `rev_20260927_99`:**
  - It is two UPDATEs: Chance 40 on 705403, and Chance 20 plus `SpellPhaseMask` 2 on 705414.
  - It gives the same result in either order relative to #5416.
  - After a merge the rows end as 69652/1/2/40 and 87060/7/2/20.
  - A recast cannot chain: an aura never procs from a spell it triggered (`SpellAuras.cpp:2166`).
- **New scenarios:**
  - `reaper-crimson-death-chance.json`: 40 Slaughters. It asserts 1–19 `spell_proc_count` and 41–59 casts.
    False failure is about 1.5e-4.
  - `reaper-soulstorm-chance.json`: 40 Soulrends. It asserts 5–29 casts of 500363. False failure is about 3.6e-5.
  - Both reject a 100% row. The Crimson Death one also rejects the cast phase.
- **Conflict with #5416's tests (for lostmind84, not changed here):** `reaper-crimson-death.json` (min 2
  Slaughter casts after one cast) and `reaper-soulstorm-refund.json` (exactly one refund after one Soulrend)
  assert 100%. With the tooltip chances they fail about 80% and 60% of the time. They need the same
  trial-and-bound form, or to be dropped in favour of the two scenarios here.
- **Deferred (unchanged):** Soulstorm rolls per target hit, so an AoE Requiem can refund more than one soul
  per cast.

### Ghost Claw (#4244): 3737830 (replaces 7a35ae0 and a43a9a7)

- **What the tooltips say:** Ghost Claw resets on any kill. 807234, the talent-504269 replacement, refunds a
  charge instead of clearing a cooldown.
- **Why the old version could not work:** #5416 found the Ghost Claw debuff never landed in its setup, and the
  old aura-removal script depended on that debuff. Both old commits were reverted.
- **New `AscensionReaperGhostClaw.cpp`:** a `PlayerScript` on creature and PvP kills. For each of 803985 and
  807234 that the Reaper knows:
  - a charge spell (`MaxCharges`) gets `RestoreSpellCharge`;
  - otherwise the cooldown is removed.
- **Why `HasSpell(807234)` works:** `SynchronizeTalentReplacements` learns the replacement with `learnSpell`.
- **Scenario `reaper-ghost-claw-kill-reset.json`:**
  1. Cast Ghost Claw on one creature and confirm the cooldown holds without a kill.
  2. Kill a second, untouched creature; the cooldown clears.
- **In game:** repeat as spec 56 with 504269 learned. Spend a charge of 807234, get a kill, and expect
  `spell_charges` to rise by one.

### Soulbender (#1135): DEFERRED, reverted in a2519cd

- **The contradiction:** John's summary of #5416 says 804004's class mask matches neither Deathchaser nor its
  Runic Power child. The merged Chasing Death scenario records every Deathchaser rank as SpellFamilyFlags[0]
  0x4.
- **Why the row came out:** with no Soulbender DBC record in the data provided, it is not established that a
  proc on the Deathchaser cast is the right mechanism, rather than a spell mod on the RP child.
- **Needed:** 804004's full record (effects, trigger spell, class masks) and the id of Deathchaser's RP child.

## Round 2: data-backed fixes

- **Evidence:** John's CoA client DBC and Ascension DB extract (conversation, 2026-09-27). In it, bp is the raw
  EffectBasePoints and the real value is bp+1.
- **Structure:** each fix is its own `src/server/coa/AscensionReaper*.cpp` file with one `AddSC_` loader line,
  plus a new pending SQL file and a gameplay scenario. None of them edits a file #5416 touches.

### #2669 Anima Ambusher (705424): 319173e

- **Data:** effect 0 is aura 354 (no core handler) with value 25 and trigger 705425. Anima Ambush 705425 does
  periodic damage every 1000 ms for 5 s.
- **Live:** "an additional 125% of its damage dealt over 5 seconds".
- **Fix:**
  - `spell_ascension_reaper_anima_ambusher` is bound to 803742, the damage spell every Spectre Stride rank
    triggers (801624, 802422–802428).
  - In `AfterHit` it casts 705425 on each target hit.
  - Its tick is the talent's effect value (25%) of `GetHitDamage()`, so 5 ticks give 125%.
  - This follows the house pattern (`AscensionPyromancer.cpp` `Copy`).
- **Scenario `reaper-anima-ambusher.json`:** 4–5 ticks, a total of 0.9–2.0× the Stride hit, and no ticks
  without the talent.

### #2684 Mind Screech (705448): ee34996

- **Data:** the trigger is 802086: 200% weapon damage, a 3.5 s silence, and effect 168.
- **Live:** "up to 200% bonus weapon damage based on how low your health is. Fails on silence immune
  enemies." Live Shrieking Scythe is the client's Soulrend (SpellFamilyFlags[1] 0x2000).
- **Fix:**
  - A `spell_proc` row with family 36 and mask1 0x2000 uses the flags that Soulstorm already uses on Soulrend
    (#5416's scenario lands Soulstorm with them).
  - `aura_ascension_reaper_mind_screech` casts 802086 with weapon percent = 200 × (1 − health%).
  - It skips targets immune to the spell or to its silence effect (`IsImmunedToSpell`,
    `IsImmunedToSpellEffect(EFFECT_1)`).
- **Scenario `reaper-mind-screech.json`:** a full-health Reaper silences with zero bonus damage, and a Reaper
  at 10% health does both.
- **Not tested:** the silence-immune skip, since there is no fixture with silence immunity.

### #3493 Blood Binding (805200): 332d361

- **Data:** the damage is 801334 (nearby enemies) and the root is 802752 (3 s). Cull is 800930/800940 and
  Scythe Rush is 500359.
- **Fix:**
  - A `PlayerScript` spell-cast hook remembers a Cull until the Reaper's next non-triggered cast.
  - If that cast is Scythe Rush, it casts 801334 at the Reaper's position.
  - `spell_ascension_reaper_blood_binding_damage` on 801334 roots each enemy it hits.
- **Assumption:** the strike fires where Scythe Rush starts. The data does not say whether live fires it at the
  start or the end of the rush.
- **Scenario `reaper-blood-binding.json`:** Scythe Rush after Cull gives 801334 damage and the 802752 root.
  Scythe Rush without a preceding Cull gives nothing.

### #5361 Tormented Souls and Soulfused Constitution: 5cf1cd1

- **Soul count:**
  - `spell_ascension_reaper_tormented_souls` on 500483 counts Reaped Souls in `BeforeCast`. Consumption runs
    later, in `AllSpellScript::OnSpellCast` (`Spell.cpp:4134`).
  - In `AfterCast` it sets 500481's stacks to souls × 2 with 561100, plus 2 with 525013.
- **Delayed reapplications:**
  - It prevents effects 1 and 2 (effect 183, delayed re-cast of 500481).
  - A refresh through `Aura::ModStackAmount(+1)` would reset the stack count (to 1 on a non-stacking aura).
- **Consuming stacks:**
  - A `spell_proc` row for 500481 (ProcFlags 139944 = taken melee, ranged and spell damage; direct only, type 1,
    hit phase) lets it proc on attacks taken.
  - `aura_ascension_reaper_tormented_souls` heals through 500482 and removes one stack.
  - The heal is 50 + 0.5/level + 0.24 × Stamina in the script, plus 0.058 × AP through `spell_bonus_data`
    (`ap_bonus`; heals honour it at `Unit.cpp:10409`). There is no SP term.
- **561234** is cast while the buff is up for owners of 561100, and removed with it.
- **Open questions:**
  - 561234's aura 87 has MiscValue 1 (Physical only) in the DBC, while the tooltip says all damage. It was left
    as the DBC has it.
  - 500481's effect 2 (the 355461 RP trigger) is left native. If it is a proc aura, it now also procs on hits
    taken.
- **Scenario `reaper-tormented-souls.json`:**
  - 3 souls give 3 stacks, still 3 after the delayed window.
  - The Soulfused Reaper gets 6 stacks and 561234.
  - Hits from a PvP attacker heal and consume stacks until the buff ends: 3 heals in total, the first at least 90.

### #2659 Harnessed Life (705406): 230e91a (test only)

- **Data:** 705406 is a flat SPELLMOD_CHANCE_OF_SUCCESS of -25 whose mask matches 500481.
- **Why no code is needed:** `Aura::CalcProcChance` applies SPELLMOD_CHANCE_OF_SUCCESS for the aura's own id
  (`SpellAuras.cpp:2320`). Once #5361 lets 500481 proc, a quarter of hits natively keep their stack (and do not
  heal). A second 25% roll would double-apply it.
- **Scenario `reaper-harnessed-life.json`:** it keeps Tormented Souls up across 30 or more landed hits and bounds
  heals per hit to 0.5–0.95.

### #3096 Unbound: left to #5416

- #5416 eb5776d adds both 706785 rows to `AscensionConditionalCombatData.h`.
- Its mapping matches the data provided: misc 20000 is always crit and misc 20001 is +25% crit damage, below 20%
  health, with mask 0x800000 (Soul Bolt 500627).
- Duplicating it here would conflict.

### #4227 Spectral Scythe: DEFERRED

- **Damage:** the data has no damage formula for 500489 Scythe Sweep, whose SP coefficient is missing.
- **Display:** 707188 (display 427586) is triggered by 500485 only every 150 s, while the summon lasts 20 s.
- **Why the display was not changed:**
  - `rev_20260916_04` records that the server's `CreatureDisplayInfo.dbc` has no scythe model.
  - Nothing provided confirms 427586 exists server-side, or whether 707188's transform MiscValue is a display id
    or a creature entry.
  - A `creature_template_model` row for a missing display would leave the summon without a model.
- **Needed:**
  - `coa-dbc-viewer record --data <server DataDir/dbc> --table CreatureDisplayInfo --id 427586`;
  - 707188's EffectMiscValue.

## Handoff verdicts settled by the data

- **Fixed on this branch:** #2669, #2684, #3493, #5361 and #2659 (previously REAL BUG / NEEDS IN-GAME CHECK).
- **Fixed by #5416:** #3096 and #495 Life Tap (f43528e).
- **REAPER_HANDOFF.md** was updated in the same commit as this file.

## Summary

| Verdict | Count | Issues |
|---|---|---|
| Correct as originally committed | 0 | The originals were each reverted, replaced, or left to #5416 |
| Fixed in review | 9 | #494, #2663, #4244, #2669, #2684, #3493, #5361, #2659, plus the Crimson Death hit phase |
| Left to #5416 | 3 | #3335, #1339, #3096 |
| Still broken on this branch | 0 | Everything with data is fixed |
| Deferred | 4 | #1135 Soulbender (conflicting data), #4227 (no formula or display confirmation), the Soulstorm per-target roll, and #5416's Dominion auto-attack path |

## Round 2 commits

| Commit | Change |
|---|---|
| 8c3e1d9 | Revert the Ghost Claw talent replacement |
| 466595c | Revert the Ghost Claw death reset |
| a2519cd | Revert Soulbender |
| 25cf774 | Revert Dominion |
| 9b66cb0 | Revert Harvest Time |
| 61bc026 | Revert the Soulstorm/Crimson Death DELETE+INSERT |
| 488dc5c | fix(CoA/Reaper): Soulstorm and Crimson Death proc at their stated chance |
| 3737830 | fix(CoA/Reaper): Ghost Claw resets on any kill |
| 319173e | fix(CoA/Reaper): Anima Ambusher adds its damage over time to Spectre Stride |
| ee34996 | fix(CoA/Reaper): Mind Screech silences and strikes on Soulrend |
| 332d361 | fix(CoA/Reaper): Blood Binding strikes and roots when Scythe Rush follows Cull |
| 5cf1cd1 | fix(CoA/Reaper): Tormented Souls stacks per soul and heals as hits consume it |
| 230e91a | test(CoA/Reaper): prove Harnessed Life keeps a Tormented Soul on a quarter of hits |
| (final) | This REVIEW.md and REAPER_HANDOFF.md |

## Verification (round 2)

- **`python3 -B tools/verify_all.py --stages source --base main`:**
  `VERIFY ALL: PASSED .cache/verify-all/20260927-200421/report.json`.
  - The source stage passed 10/10.
  - The build, unit, harness and gameplay stages were skipped. Nothing was compiled or run.
- **`python3 apps/codestyle/codestyle-cpp.py --files`** on every changed `.cpp`/`.h` (`AscensionReaperGhostClaw`,
  `AnimaAmbusher`, `MindScreech`, `BloodBinding`, `TormentedSouls`, `CoAScriptLoader`): "Everything looks good".
- **`python3 apps/codestyle/codestyle-sql.py --files`** on `rev_20260927_99`, `rev_20260928_21`, `_22`, `_23` and
  `_24`: "✅ Everything looks good".
- **`run.py validate`** accepts all seven new scenarios (authoring check only, not verification).
- **To do on the PC:**
  1. `python -B tools/verify_all.py --base origin/main`.
  2. The focused runs, `--stages build,gameplay --scenario <id>`, for `reaper-crimson-death-chance`,
     `reaper-soulstorm-chance`, `reaper-ghost-claw-kill-reset`, `reaper-anima-ambusher`, `reaper-mind-screech`,
     `reaper-blood-binding`, `reaper-tormented-souls` and `reaper-harnessed-life`.
  3. Confirm the timing-sensitive ones with `--gameplay-clock real`: these are the Tormented Souls, Harnessed Life
     and Anima Ambusher scenarios.
- **Measurements:**
  - Time to a verified outcome: unknown, since there was no runtime.
  - Regressions or flaky checks: none observed, since only the source stage ran.
  - Human corrections:
    - John's comparison against #5416 overturned the round-1 Harvest Time, Dominion, Soulstorm/Crimson Death
      ordering, Ghost Claw and Soulbender verdicts.
    - John's data corrected the heal and spell ids.

---

## Round 1 review (history; superseded where the table above says so)

- **Evidence:** the live-parity sites were blocked by the sandbox proxy (403). The evidence was the issue-list
  tooltips (`git show 43346d1:.reaper-run/ISSUES.md`), the Chasing Death scenario's recorded DBC facts, and the
  repo.
- **Round-1 verdicts:** Harvest Time (d0c1ed9), Soulstorm/Crimson Death (c79ca0f), Dominion (fef4d10) and
  Soulbender (419b1eb) were judged CORRECT.
- **Ghost Claw:** it was FIXED-IN-REVIEW for the unbound replacement 807234 (7a35ae0 + a43a9a7).
- **Later corrections:**
  - Soulstorm/Crimson Death's SQL ordering reset #5416's hit phase.
  - Dominion duplicated #5416 and has a DBC auto-attack proc path.
  - Ghost Claw's trigger was wrong (any kill, not the debuffed target's death).
  - Soulbender's mask is disputed.
- **Round-1 spot checks, still standing:**
  - #2170 Shade, #2257 Decimation, #3338 Reaper's Pact and #3744 Red Wake moved from FALSE POSITIVE to NEEDS
    IN-GAME CHECK (3319f53).
  - #3098, #2671, #2169 and #2647 were confirmed.
  - #3095 Dark Soul likely does the reverse of its tooltip.
- **Round-1 verification:** `VERIFY ALL: PASSED .cache/verify-all/20260927-191759/report.json` (source 5/5); both
  linters were clean.
