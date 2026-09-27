# Reaper sweep review (branch `fix/reaper-sweep`)

This is a line-by-line correctness review of the five fix commits on `fix/reaper-sweep`, plus spot checks of
verdicts in `REAPER_HANDOFF.md`. Nothing was compiled or run in game. The Reaper fixes still need a build and
an in-game test on the PC.

## Evidence sources

- **The live-parity sources were blocked.** `hertigservices.github.io` (ascension-data JSON),
  `ascension-db.ascension-archive.workers.dev` and `ascension-archive.vercel.app` all returned a 403 CONNECT
  rejection from the sandbox egress proxy. None of them could be cited.
- **Fallback sources:**
  - the tooltips and DBC effect summaries copied from the upstream issues into `.reaper-run/ISSUES.md` (removed
    in f698811; read back with `git show 43346d1:.reaper-run/ISSUES.md`);
  - DBC facts that merged gameplay scenarios record (`apps/coa-gameplay-test/scenarios/reaper-chasing-death.json`);
  - the repo's C++ and SQL.
- There is no DBC set in the sandbox.

## Verdicts

| Issue | Verdict | Commit |
|---|---|---|
| #3335 Harvest Time (and #4005 in part) | CORRECT | d0c1ed9 |
| #494 Soulstorm | CORRECT (one DEFERRED question) | c79ca0f |
| #2663 Crimson Death | CORRECT | c79ca0f |
| #1339 Dominion | CORRECT | fef4d10 |
| #4244 Ghost Claw not resetting | FIXED-IN-REVIEW | 7a35ae0 + a43a9a7 |
| #1135 Soulbender | CORRECT | 419b1eb |

### #3335 Harvest Time (803995): CORRECT (commit d0c1ed9)

- **Tooltip** (issue list): "...gives your spells that require Soul Infusion a 50% to not consume the effect
  for 15 sec."
- **Code:** `HarvestTimePreserves` (`src/server/coa/AscensionCompat.cpp:2935`) now rolls
  `roll_chance_i(50)`, after its checks for `CasterAuraSpell == 803031` and `HasAura(803995)`.
- **Call path:** it has a single call site, `ConsumeReaperSouls` (`AscensionCompat.cpp:2977`). That runs once
  per cast from the resource-cost handler (`AscensionCompat.cpp:2485`), so each cast rolls exactly once.
- **Behaviour:**
  - On a successful roll nothing is consumed.
  - On a failed roll the normal consumption runs. For an all-soul consumer such as Soulrend, whose ranks
    573316–573322 and 572341/2 are all in `REAPER_ALL_SOUL_CONSUMERS`, this removes the Reaped Souls and Soul
    Infusion.
- **Existing rule engine:** its preserve roll (`PreserveCostChancePercent`, `AscensionCompat.cpp:2465`) has no
  entry for 803995, so the Harvest Time roll is not applied twice.
- **In game:**
  1. `.learn 803995` and `.learn 573316`. Give Soul Infusion with `.aura 803031` (plus 3× `.aura 500363` for the
     souls).
  2. Cast Harvest Time, then Soulrend. Re-apply 803031 after each cast that consumes it.
  3. Expected: over 20 casts, about 10 keep 803031. Before the fix, all 20 did.

### #494 Soulstorm (705403) and #2663 Crimson Death (705414): CORRECT (commit c79ca0f)

- **Tooltips:**
  - Soulstorm: "Requiem and Soulrend now have a 40% chance to refund a Reaped Soul."
  - Crimson Death: "Your Slaughter now has a 20% chance to cast an additional time free of cost."
- **Why the `Chance` column matters:**
  - `rev_20260920_17` says it set Chance 100 in every row on purpose.
  - A non-zero `spell_proc.Chance` overrides the DBC ProcChance. So 100 made both talents always proc.
- **The new file `rev_20260927_95`:**
  - It has a DELETE before the INSERT.
  - All columns match the effective state of the original rows. Soulstorm keeps `69652/1/2`. Crimson Death
    keeps `87060/7/1`, which includes the ProcFlags widening from `rev_20260921_20:258`.
  - Only `Chance` changes, to 40 and 20.
  - No later pending file touches these ids. `spell_script_names` still binds both ids to
    `spell_ascension_reaper_talent_proc`. Its `CheckProc` filters on the rule lists in
    `AscensionReaperTalentProcs.h:38-39`, and those lists contain every Soulrend rank.
- **DEFERRED:** Soulstorm uses SpellPhaseMask 2 (HIT), so it rolls once per target hit. If Requiem (572179) hits
  several enemies, one cast can refund more than one soul. That behaviour predates this branch. Changing it to
  the CAST phase would interact with the order of `ConsumeReaperSouls`, so it needs an in-game check before
  anyone changes it.
- **In game:**
  1. Run `SELECT SpellId, ProcFlags, Chance FROM spell_proc WHERE SpellId IN (705403,705414);`. Expected: 69652/40
     and 87060/20.
  2. Soulstorm: `.learn 705403`, then cast Soulrend 30 times with souls. Expected: a 500363 stack is refunded on
     about 12 of the casts.
  3. Crimson Death: `.learn 705414`, then cast Slaughter 500373 30 times. Expected: the combat log shows a second
     free Slaughter on about 6 of the casts.

### #1339 Dominion (803999): CORRECT (commit fef4d10)

- **Tooltip:** "Gaining Soul Infusion now increases your Armor by 585 for 6 sec."
- **Hook:** `ApplyAscensionReaperSoulInfusionGained` (`src/server/coa/AscensionReaperTalents.cpp:516`). Its
  only caller is the single place that grants Soul Infusion: `AscensionCompat.cpp:3089`, which runs when the
  player has 3 or more souls and no Infusion. So the hook fires once per gain.
- **`CastOwnProcTrigger`:**
  - It requires `HasAura(803999)`, so the passive must be learned.
  - It takes the first `PROC_TRIGGER_SPELL` effect's `TriggerSpell` from the DBC, checks that the spell exists,
    and self-casts it as triggered. No id is hard-coded.
  - Unlike `RollTalent`, it ignores the DBC ProcChance. That is right here: the tooltip has no chance, and the
    branch's `rev_20260920_17` header documents that these passives carry unreliable proc data.
- **Double fire:** nothing adds a `spell_proc` row for 803999, and the DBC ProcTypeMask is 0 for these
  passives. So no second proc path exists.
- **In game:**
  1. `.learn 803999`, then build 3 souls (3× `.aura 500363` and one cast that syncs resources, or build them
     normally).
  2. Expected: one 6-second buff, and armor on the character sheet rises by 585.
  3. Spend the Infusion and gain it again. Expected: the buff is applied again.

### #4244 Ghost Claw not resetting (803985): FIXED-IN-REVIEW (commits 7a35ae0 + a43a9a7)

- **Report:** "The cooldown of the ability doesnt reset when I kill an enemy who is affected by Ghost Claw."
- **Original script (7a35ae0):**
  - `AfterEffectRemove` on `EFFECT_ALL`/`SPELL_AURA_ANY`, with `GetTarget()` read inside a remove hook, where
    it is valid.
  - It is gated on `AURA_REMOVE_BY_DEATH`, which `Unit::RemoveAllAurasOnDeath` sets.
  - The cooldown is written in `Spell::_cast` (`Spell.cpp:3988`) before any hit. So a kill by Ghost Claw's own
    hit still finds a cooldown to clear.
  - Several aura effects would each call the hook, but `HasSpellCooldown` guards it.
- **Defect found in review:** `AscensionTalentReplacementData.h:61` makes talent 504269 (spec 56) replace
  803985 with 807234 through `SynchronizeTalentReplacements` (`AscensionCompat.cpp:806`). Players with that
  talent cast 807234. Its aura had no script, and the original script cleared only the hard-coded 803985
  cooldown. This is the "only the first id handled" pattern.
- **Fix (a43a9a7):**
  - `Validate` accepts both ids.
  - The script clears the cooldown of `GetId()`.
  - New `rev_20260927_98_reaper_ghost_claw_replacement_reset.sql` binds 807234, with a DELETE before the
    INSERT.
- **Remaining assumption:** 803985 and 807234 must apply an aura to the enemy. If either is direct damage only,
  the script never runs for it (AuraScripts are created only for auras). Check this with
  `coa-dbc-viewer record --id 803985 --field 'Effect*'` and the same command for 807234.
- **In game:**
  1. `.learn 803985` and cast it on a low-health mob. Kill the mob while it has the debuff. Expected: the cooldown
     clears at once.
  2. Kill a mob without the debuff. Expected: the cooldown stays.
  3. Repeat as spec 56 with 504269 learned, using 807234.

### #1135 Soulbender (804004): CORRECT (commit 419b1eb)

- **Tooltip:** "Significantly increases the Runic Power generated by Deathchaser."
- **Row:** `(804004, 0, 36, 4, 0, 0, 16, 7, 2, 0, ..., 100, 0, 0)`. It is a copy of the merged Chasing Death
  row (`rev_20260927_86`).
- **DBC evidence:** `reaper-chasing-death.json` records that every Deathchaser rank (805190 through 573053) is
  SpellFamilyName 36 with SpellFamilyFlags[0] 0x4 and DmgClass melee. So ProcFlags 16
  (DONE_SPELL_MELEE_DMG_CLASS) with the family mask covers all ranks, and no rank id is hard-coded.
- **Registration:** no script is needed. The row has no `spell_script_names` entry, so the family mask alone
  filters the spell.
- **Trigger timing:** the proc fires once per landed cast, not per periodic tick. Ticks raise
  PROC_FLAG_DONE_PERIODIC, which the row does not include. That matches a per-Deathchaser RP bonus.
- **Unverified:** the size of the trigger spell's energize.
- **In game:**
  1. `.learn 804004`, then cast Deathchaser 805190 with Soul Infusion up (`.aura 803031`).
  2. Expected: one Soulbender trigger per cast in the combat log, and more RP per cast than without the
     talent.
  3. Also check that another family-36 spell with flag 0x4, if any exists, does not trigger it.

## Spot checks of handoff verdicts

These were re-checked against the issue-list tooltips. REAPER_HANDOFF.md was updated in 3319f53.

| Issue | Old | New | Reason |
|---|---|---|---|
| #2170 Shade (573038) | FALSE POSITIVE | NEEDS IN-GAME CHECK | The tooltip clause "entering Underwalk" is unreferenced in the tree. The handoff itself asked for an in-game check. |
| #2257 Decimation (704193) | FALSE POSITIVE | NEEDS IN-GAME CHECK | "Every 6th cast" has no counter in the tree. The row procs on every Reap cast at Chance 100, so correctness depends on unverified trigger-spell stacking. |
| #3338 Reaper's Pact (804001) | FALSE POSITIVE | NEEDS IN-GAME CHECK | SPLIT_DAMAGE_PCT comes from APPLY_AURA (6), not an area aura, so nothing enforces the 30-yd fall-off. |
| #3744 Red Wake (801041) | FALSE POSITIVE | NEEDS IN-GAME CHECK | "5 RP for each target struck" is not handled in the tree. |

These spot checks were confirmed, and their verdicts are unchanged:

- **#3098 Empyrean Fortitude:** ProcFlags 40 is taken melee auto + melee spell, and HitMask 32 is PARRY.
- **#2671 Souls for the Slaughter:** HitMask 2 is CRITICAL.
- **#2169 The Time Has Come:** `AscensionConditionalCombat.cpp:44`.
- **#2647 Spirit of Savagery:** aura 344 goes to `HandleAscensionModAttackPowerFlat`
  (`SpellAuraEffects.cpp:5092`).
- **#3095 Dark Soul:** the NEEDS IN-GAME CHECK verdict stands. The aura grants rating from a stat, which is the
  reverse of the tooltip, so it is likely a real bug.

## Deferred

- **Soulstorm (#494):** it rolls per target hit (phase HIT). For AoE Requiem this can give several refunds per
  cast. This predates the branch.

## Summary

- **Fixes:** 4 correct, 1 fixed in review (#4244), 0 still broken, 1 deferred item (the Soulstorm
  per-target roll).
- **Verdicts:** 4 handoff verdicts changed.

## Review commits

- **a43a9a7:** `fix(CoA/Reaper): Ghost Claw talent replacement also resets on a kill`.
- **3319f53:** `docs(CoA/Reaper): handoff verdicts after the fix review`.
- **Final commit:** adds this REVIEW.md and runs `git rm .claude/settings.json`.

## Verification

- **`python3 -B tools/verify_all.py --stages source --base main`:**
  `VERIFY ALL: PASSED .cache/verify-all/20260927-191759/report.json`. The source stage passed 5/5. The build,
  unit, harness and gameplay stages were skipped as instructed, and nothing was compiled.
- **`python3 apps/codestyle/codestyle-cpp.py --files src/server/coa/AscensionReaperTalents.cpp`:**
  "Everything looks good".
- **`python3 apps/codestyle/codestyle-sql.py --files
  data/sql/updates/pending_db_world/rev_20260927_98_reaper_ghost_claw_replacement_reset.sql`:**
  "✅ Everything looks good".
- **To do on the PC:** `python -B tools/verify_all.py --base origin/main` (build, unit, harness), then the
  in-game checks above.
- **Measurements:** the time from start to a verified outcome is unknown (there was no runtime verification).
  No regressions or flaky checks were observed, and no human corrections were needed.
