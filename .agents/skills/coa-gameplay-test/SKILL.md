---
name: coa-gameplay-test
description: >-
  Run isolated CoA worldserver scenarios to test spells, talents, items and server-side effects.
  Use for requested gameplay/runtime validation; source-only edits and rendered client/UI testing have separate scopes.
---

# CoA gameplay test

Use the gameplay harness in `C:/Ascension/azerothcore-wotlk-coa`, or the user's explicitly selected CoA checkout.
Read that checkout's `AGENTS.md` and `apps/coa-gameplay-test/README.md` before the first run.

## Establish the experiment

- Translate the requested behavior into actions and independent expected results. For Ascension scaling,
  triggered spells or tooltip parity, use `.agents/docs/systems/ascension-spell-parity.md` to establish the
  contract. A calculation copied from the current implementation is not independent evidence.
- Start from `apps/coa-gameplay-test/scenarios/frostbolt.json`. Use the scenario actions and metrics actually
  supported by the README. Name actors, snapshots and assertions clearly. Store a new reusable regression
  in the scenario directory when it belongs to the requested change; use `.cache/coa-gameplay-tests/` for
  an exploratory scenario.
- Configure level, equipment, learned abilities and target conditions explicitly. A `learn` action is fixture
  setup; use `talent` to exercise talent point/prerequisite checks. A normal `cast` or `use_item` submission
  requires effect assertions to prove behavior. Keep normal costs, cooldowns and proc rules enabled.
- Check passive talents with `has_talent` using their rank's spell ID; they are stored separately from
  learned spells. Check `talent_points` for point consumption and use `reset_talents` for fixture removal.
- `within_ms` means eventually true. To prove an effect does not occur, wait through its possible trigger
  window before checking absence. Net health/power deltas include regeneration and other effects. Random
  proc rates require repeated trials and a statistical check beyond this harness's built-in assertions.
- Let spawn AI and level scaling settle before measuring damage. Verify stable maximum health and take a
  fresh health baseline before each tested cast. Prepare equipment while the actor is out of combat.

## Execute

1. Validate the scenario with `python apps/coa-gameplay-test/run.py validate <scenario>`.
2. Resolve a worldserver containing the runtime module, the matching game data/config, and local MySQL 8
   `mysql.exe`/`mysqldump.exe`. Read credentials through the source config without printing them. Check
   the candidate build's source and freshness; do not assume an installed binary includes current edits.
   If normal credentials cannot create schemas, use `--database-client-config` with an authorized existing
   MySQL `[client]` file on the same endpoint (see README). Do not change existing account grants.
3. Follow existing build authorization. The repository requires an explicit request before configuring or
   building. If a suitable executable is unavailable, finish the scenario and source checks, then explain
   the exact build needed and request authorization. A requested runtime run permits the runner's isolated
   database copies and owned test process; it does not authorize replacing the installed server.
4. Run `python apps/coa-gameplay-test/run.py run <scenario> --worldserver <exe> --config <conf>
   --mysql <mysql> --mysqldump <mysqldump>`. Invoke as one shell command with properly quoted arguments.
   The runner reuses its owned world copy by default, with fresh accounts/characters on each run. It checks
   source data, repository SQL and configs for changes, applies startup updates and audits persistent world
   writes after shutdown. A clean world copy is retained; disposable character/auth schemas and credentials
   are removed. Use `--refresh-world` to replace a cache or `--fresh-databases` for a fully disposable run.
   Never point the enabled runtime module at normal databases or reuse a result directory.
5. Inspect `summary.json`, `result.json` and relevant startup/runtime log errors. A pass requires the runner's
   zero exit code and completed assertions. Missing readiness, a crash, a partial result, a timeout or cleanup
   failure is a failed run. Diagnose infrastructure failures before interpreting gameplay outcomes. Use a
   new run after a correction; preserve evidence of the failed attempt while investigating it.

6. Check `world_cache` in the summary: a retained, verified world is intentional. A scenario that writes world
   data discards that copy. Reuse needs the current startup-barrier binary; older builds require fresh mode.
   Source SQL outside the documented repository directories needs explicit refresh. For clean acceptance,
   use fresh mode when the task needs an independent database baseline. A cache lease blocks concurrent
   users; do not remove it until its runner and worldserver are confirmed stopped. Fresh mode can run
   independently while a cache is leased.

## Report

State the tested scenario, actual versus expected results, executable identity and any untested behavior.
Link the result files. Distinguish validation of the runner from execution of the native scenario.

This mode uses real game objects and character loading with socketless sessions. It does not cover network
authentication/session discovery, client packet delivery, rendered tooltips, animations or UI input.
Do not claim those were tested. Add a supported driver or a separate client test when the task requires them.
