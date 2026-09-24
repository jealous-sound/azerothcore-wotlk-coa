# CoA gameplay tests

Execute repeatable scenarios inside a real worldserver, using its loaded DBCs, SQL, scripts, maps and updates.
The runtime component is `src/server/coa/CoAGameplayTest.cpp`; it is disabled by default.

## Find and verify a mechanic

The searchable catalog derives spell IDs, classes, assertions, metrics and contracts from the scenarios.
`checks.json` binds the existing numerical result checkers to their exact scenario definitions and mode arguments.
Use the [agent DBC viewer](../coa-dbc/README.md#agent-retrieval-coa-dbc-viewer) to inspect source data first.
The [mechanic map](../coa-mechanics/README.md) connects spells and quests to ranks/acquisition, expected behavior
and reviewed execution paths. `catalog.py --quest ID` and `workflow.py --quest ID` select quest investigations.

```sh
python apps/coa-gameplay-test/catalog.py --spell 1257670
python apps/coa-gameplay-test/catalog.py --query 'replenishment'
python apps/coa-gameplay-test/workflow.py --spell 1257670
python apps/coa-gameplay-test/catalog.py --check
```

`workflow.py` returns candidate scenarios and an investigation sequence in JSON. It does not infer that a
report is a defect or create a PR. Establish expectations independently, select a metric that observes the
behavior, reproduce it, then classify the result as a defect, already working, an incorrect test or unresolved.
Use the existing issue-to-PR workflow when that scope is requested.

`run.py run` now combines native execution with every registered numerical check for an exact catalog
scenario. Checks requiring companion scenarios (currently the two Rockslide selection cases) run both cases
automatically. With `--output`, their directories live under that output. `verification.json` is written in
the first case's result directory, includes executable/scenario/checker identities, and controls the final exit code.
`NATIVE STAGE COMPLETE` is intermediate progress; only `VERIFICATION PASSED` means the combined checks passed.
Native failures also produce a failed combined outcome, with required checks marked `blocked` when their
native evidence is unavailable. A failure stops further native runs because cleanup may have failed. If native
setup fails before creating a result directory, the combined outcome is printed to standard output.

For example, the existing Primalist conversion script has two registered modes:

| Scenario | Required checker | Mode argument |
| --- | --- | --- |
| `primalist-everlasting-rage` | `check_primalist_native_conversions.py` | `everlasting` |
| `primalist-king-mountain` | `check_primalist_native_conversions.py` | `king` |

Run the scenario once with the usual isolated-runner options; there is no separate checker command to remember:

```sh
python apps/coa-gameplay-test/run.py run apps/coa-gameplay-test/scenarios/primalist-everlasting-rage.json \
  --worldserver /path/to/worldserver \
  --config /path/to/worldserver.conf \
  --mysql /path/to/mysql \
  --mysqldump /path/to/mysqldump \
  --server-modules-dir /path/to/compiled-conf-dir/modules
```

The default command rejects modified/exploratory definitions absent from the catalog. Use `--native-only`
explicitly for those experiments; its exit code covers native execution only and produces no combined pass.
Native success alone cannot pass a failed, missing or timed-out required numerical check.

For new spell or quest regressions, save the reusable definition under `scenarios/`. Express direct assertions
in that definition and add any extra numerical checker to `checks.json`, listing scenario IDs in its expected
argument order, followed by mode arguments in `args`. Shared checks can list multiple scenarios; the runner
selects their companions transitively. Registry validation runs before native execution and in CI, and rejects
missing files, duplicate bindings and any `check_*.py` script left unregistered. Scenarios with no extra checker
still receive combined verification of native completion, assertions and cleanup.

Recheck existing result bundles without starting a server:

```sh
python apps/coa-gameplay-test/verification.py .cache/coa-gameplay-tests/RUN
python apps/coa-gameplay-test/verification.py path/to/rockslide-first path/to/rockslide-highest
python -B apps/coa-gameplay-test/test_verification.py
```

Rechecks require the exact current scenario definition, its recorded hash, matching native step/run identity,
passing native assertions and cleanup, all companion results, and matching companion executable hashes.
They do not establish that old results cover current source, database, config or client changes. The registry
ensures checkers run; it does not prove every expected mechanic has an adequate scenario or independent contract.

## Prevent recurring mistakes

Run the fast source checks for the working diff, including staged and untracked files:

```sh
python -B tools/check_source.py --base HEAD
python -B tools/check_source.py --base origin/main --plan
```

The JSON outcome lists the selected checks, failures and timings. `--all` runs every fast suite. SQL boundaries
and new C++/Python comments in CoA-owned code are always checked; changed CoA sources select loader registration
checks; gameplay tooling/scenario changes
select scenario validation, combined-verification tests, and runner/cache ownership and cleanup tests. Reviewed
execution-path changes also check mechanic-map references. Documentation-only changes skip these test suites.
CI uses the same selection for pull requests and retains the repository publication checks. Main-branch and
manual runs perform a full audit; SQL-boundary exceptions are reviewed on the pull request. The existing compiled
client-compatibility harness runs only for relevant changes or a full audit. The local command never builds or
starts a server.

CoA-owned code uses names, structure and tests to express intent. The comment check covers
`src/server/coa/`, `apps/coa-tests/`, `apps/coa-bugreport/`, `apps/coa-dbc/`, `apps/coa-gameplay-test/`,
`apps/coa-mechanics/`, `tools/` and `.github/scripts/`. It rejects explanatory comments and docstrings on added lines, while preserving legal
headers, recognized tool directives and native test-generator markers. Strings and runtime CLI help remain data.
`python -B tools/check_comments.py --all` also checks unchanged C++ and Python files in those directories.
Upstream source, dependencies, SQL and configuration documentation remain outside this check.

The loader check requires each CoA `AddSC_*`, `AddAscension*Scripts` and `AddCoA*Scripts` definition to have
exactly one call from the flat CoA script loader, and each call to have exactly one definition. It ignores comments
and string literals. It does not prove SQL bindings, hook reachability, or gameplay behavior; those require data
inspection and behavioral tests. The checker intentionally reports an unsupported conditional loader for review.

For behavior, choose the existing scenario that observes the changed mechanic and run it through `run.py run`:

- **Duplicated calculations:** `bloodmage-dominion-of-blood-vampiric-fang` compares healing with the same cast's
  damage; doubled healing fails the ratio. `ascension-replenishment` checks exact amounts for different recipient
  pools. For a new coefficient change, vary AP/RAP/SP independently and check the final affected hit or tick.
- **Wrong ownership:** `primalist-sharpened-claws` includes a wrong-caster pet control;
  `wisdomball-dungeon-quests` checks the interacting player's quest state without changing the summoner's state.
- **Rank replacement:** `pyromancer-fix-4104-ascension-rank-supersede` and `runemaster-tattoo-rank-supersede`
  verify that a superseded rank cannot apply its aura, the current rank can, and the lower aura stays absent
  while the higher aura is active. A stat-stacking change also
  needs its effective stat/damage assertion; checking only the learned spell list is insufficient.
- **Cleanup:** `primalist-protectors-hand` verifies armor returns after removing all sources;
  `primalist-sharpened-claws` covers expiry and unlearning. Runner/cache tests separately cover database ownership,
  collisions, leases, failed audits and unstopped processes; infrastructure cleanup is part of combined verification.

Keep expectations independent of implementation. Use distinguishable players/stats, positive and negative
controls, and bounded final values; a broad “damage increased” assertion can miss double scaling. The fast tests
inject doubled amounts, wrong-recipient values and retained auras into synthetic recorded results to prove those
errors cannot pass verification. These checks validate the verifier, not current worldserver behavior. Source
patterns cannot reliably establish these gameplay rules, and passing source checks is never a gameplay pass.

## Run

Python 3.11+, MySQL 8 client tools, a local MySQL server and a worldserver built with the runtime component
are required. The commands below run the runner directly (Windows example); Docker installations on Linux use
the [Compose test service](#linux-docker), which provides all of them.
Build a matching test executable when needed. Adding the new source requires CMake
reconfiguration before building; running an older binary will fail the readiness check.
The CoA server component requires Boost.PropertyTree headers. Component-based vcpkg installations need
`boost-property-tree` for the same triplet as the existing Boost libraries. CMake checks this dependency.

```powershell
python apps/coa-gameplay-test/run.py validate apps/coa-gameplay-test/scenarios/frostbolt.json

python apps/coa-gameplay-test/run.py run apps/coa-gameplay-test/scenarios/frostbolt.json `
  --worldserver C:/path/to/test-build/worldserver.exe `
  --config C:/path/to/worldserver.conf `
  --mysql C:/path/to/mysql.exe `
  --mysqldump C:/path/to/mysqldump.exe
```

The runner creates fresh `coa_test_<run-id>_auth` and `coa_test_<run-id>_characters` schemas on each run.
It creates a reusable `coa_test_<cache-id>_world` copy on the first run, then reuses that world database.
The copies include world data, auth/character schemas, RBAC, realm definitions, active arena season and
migration metadata.
Existing accounts and characters are not copied. The MySQL user needs read access to the sources and permission
to create/import/drop the test schemas. Sources must be local. No authserver or game client is needed.
The copies omit MySQL triggers, routines and scheduled events.

If the normal server account cannot create schemas, pass `--database-client-config <admin-client.ini>`.
The existing MySQL `[client]` file must provide `host`, `port`, `user` and `password`, with the same host/port
as all source connections. Its credentials are used for cloning, the isolated server and cleanup. For the
local Repack, this file is `C:/Ascension/CoA-Repack/mysql/admin-client.ini`. Credentials stay in temporary
config files, never command arguments or reports; no grants or existing accounts are changed.

The generated config binds the test worldserver to loopback on an unused port, uses a private log directory,
disables map worker threads and points all three database connections to isolated schemas. Source SQL updates
run normally against the copies. Source configuration and the installed server are not changed. Relative
`DataDir` is resolved against the binary's directory; use an absolute path when that differs from your setup.
Module `.conf` files beside the source config (in `modules/`) are copied into the directory the worldserver
reads module configs from, then removed at the end. Use `--modules-config-dir` for a different source location.
On Windows that directory is `configs/modules/` relative to the working directory (the test directory), the
default. Elsewhere the worldserver reads `CONF_DIR/modules/`, fixed at build time, so `--server-modules-dir`
is required; use an empty directory in an isolated test installation. Existing files there are never replaced,
and additional `.conf` files are rejected so they cannot change the test settings. Copied file hashes appear
in the summary. Module configs cannot override database isolation or harness controls.

Source settings follow the server's precedence: an `AC_*` environment variable (for example `AC_DATA_DIR`)
replaces the value in the source config. The test worldserver inherits the runner's environment except
variables that would replace generated harness values, such as `AC_UPDATES_ENABLE_DATABASES` or
`AC_LOGIN_DATABASE_INFO`, so the generated config always controls isolation, logging and updates.

When the scenario ends, the runtime logs out its test players and shuts down. The runner waits for process
exit before dropping the auth/character schemas, auditing the cached world and removing generated credentials.
Credentials are written only to mode-600 option files and a generated `worldserver.conf` in a private temporary
directory of the runner, never under the result directory; the directory is removed when the run ends.
Startup, scenario, shutdown and copy operations have timeouts. An interrupted run (Ctrl+C, or SIGTERM as sent by `docker stop`/`compose stop`)
performs the same cleanup. A hard termination or cleanup failure can leave schemas and a cache lease behind.
Inspect `summary.json` and the lease before removing any leftovers.

## Reusing the world database

Reuse is the default. Each run still starts a new worldserver and fresh accounts/characters, so inventory,
talents, quests and other character state cannot carry over. The cache saves the full world import on later runs.
Metadata lives under `.cache/coa-gameplay-tests/world-cache/`; `--world-cache-dir` selects a different directory.
This directory contains ownership metadata and an exclusive lease, not credentials or SQL dumps.

Add one of these options to the same `run` command when needed:

- `--refresh-world`: replace the owned world copy before running, then retain it if the scenario leaves it clean.
- `--fresh-databases`: bypass the cache, copy all three schemas and drop them after the run. Use this for final
  verification when a completely fresh database is required. It also works with older harness binaries.

The runner checks source table contents and definitions, repository SQL files under `data/sql/updates`,
`data/sql/custom` and `modules`, the source/main module configs and inherited `AC_*` settings. Changes refresh
the cache automatically. Environment values enter only the fingerprint, never the cache metadata as plain text.
Source SQL stored elsewhere needs an explicit `--refresh-world`. C++ edits and a new binary alone do not force
a full copy. Cache ownership also includes the MySQL server identity, host, port and source world schema.

Startup migrations run before the native startup barrier. The runner records the world state at that barrier,
then releases character loading and scenario actions. After shutdown it compares the world again. Persistent
world writes, including console edits, discard that copy. A failed gameplay assertion can still leave a clean,
reusable world. Auth/character databases are always discarded after the owned server stops.

Content checks use MySQL `CHECKSUM TABLE ... EXTENDED` and table definitions. They still scan the data and can
briefly block writes to a table while it is read; reuse skips copying/importing rather than all database work.
Checksums can collide, so reuse is a regression-testing optimization, not a proof of byte-for-byte equality.
See the [MySQL checksum documentation](https://dev.mysql.com/doc/refman/8.4/en/checksum-table.html).
World views, stored triggers/routines/events or unavailable checksums require `--fresh-databases`.
Avoid changing source data during a run.

Only one run can lease a given cache. A second run fails before using it; `--fresh-databases` allows an independent
concurrent run. A hard interruption or an unkillable server keeps the lease blocked. Check the lease's runner PID
and result directory and verify that its worldserver has exited before recovering it; never delete a lease to
override a running test. Refresh does not bypass a lease or an ownership mismatch.

`summary.json` records `world_cache.mode` (`created`, `reused`, `refreshed` or `fresh`), the retained world schema,
invalidation reason, and preparation/server/total seconds. A retained world with `retained: true` is intentional.
Generated credential and module configuration files are removed on every normal cleanup.

Results default to `.cache/coa-gameplay-tests/<run-id>/`:

- `scenario.json`: exact scenario used.
- `worldserver.log`: process output, including startup and script errors.
- `result.json`: server version, actual values and step outcomes.
- `summary.json`: native-stage result, binary/scenario SHA-256 and any cleanup failure.
- `verification.json`: combined native and registered numerical verification for catalog scenarios.

Combined exit code zero requires every expected assertion and step to complete, matching run identity, a clean server
exit, successful cleanup/cache audit and every registered numerical check. A submitted cast alone is never a pass.
Numeric fields in the server's property-tree JSON are strings; the Python runner converts and rechecks assertion values.

### Linux (Docker)

`docker/compose.yml` adds the `ac-gameplay-test` service to the root Compose stack. It uses the locally built
worldserver image plus Python and shares the `ac-database` network namespace, so MySQL is reachable on
`127.0.0.1` and the isolation checks are unchanged. The repository is mounted read-only (runner, scenarios and
SQL updates), the live `DOCKER_VOL_ETC` configs are read-only sources, and `DOCKER_AC_ENV_FILE` applies the same
`AC_*` settings as the live worldserver. The service selects the same `acore_auth`, `acore_characters` and
`acore_world` source schemas as the root Compose stack, using `127.0.0.1:3306` inside the shared network namespace.
Its database environment variables override any stale connections in `worldserver.conf` or the environment file.
If another Compose override changes the live schema names, mirror those names in this service's
`AC_*_DATABASE_INFO` variables while retaining the loopback endpoint. Build the worldserver image from the same
checkout first, with CoA and the cache startup barrier; rebuild the test image after it. A mounted
source checkout does not update the compiled server. Prefer rebuilding only the required test targets.

```bash
mkdir -p .cache/coa-gameplay-tests
docker compose -f docker-compose.yml -f apps/coa-gameplay-test/docker/compose.yml --profile tests \
  build ac-gameplay-test
docker compose -f docker-compose.yml -f apps/coa-gameplay-test/docker/compose.yml --profile tests \
  run --rm --no-deps ac-gameplay-test run apps/coa-gameplay-test/scenarios/frostbolt.json
```

Run these commands from the checkout that owns the running stack (matching project name and `.env`), or pass
`--project-name`/`--env-file` explicitly. The example assumes the database is already healthy and configs and game
data have been initialized. `--no-deps` prevents starting or changing dependencies. For `validate <scenario>`,
also use `--no-deps` so Compose does not start MySQL; validation itself does not connect to it. The entrypoint fixes
`--worldserver`, `--config`, `--modules-config-dir`, `--server-modules-dir`, `--mysql`, `--mysqldump`,
`--database-client-config`, `--world-cache-dir` and `--output`; passing any of them after the scenario has no effect.
The results mount also stores persistent cache metadata at `/results/world-cache`, corresponding to
`.cache/coa-gameplay-tests/world-cache/` on the host. The default reuse, `--refresh-world` and `--fresh-databases`
options have the same behavior as direct invocation. A clean retained world schema is intentional; only fresh
mode drops all three schemas after each run. Cache lease PIDs and result paths refer to the container; verify the
owning container has stopped before recovering a lease.

The service connects as MySQL `root` with `DOCKER_DB_ROOT_PASSWORD` (it must not contain `"`, `;`, or a carriage
return/line feed; the runner rejects such characters before connecting). Credentials are written only to
mode-600 files in a private temporary directory inside the disposable container, never to the result
directory. The runner removes its generated files during cleanup; the entrypoint's admin file disappears when
the disposable container is removed (`--rm`). SIGTERM from `docker stop`/`compose stop` triggers cleanup,
including the world audit; allow enough time for database checks (individual checksums can take five minutes).
The service allows 15 minutes; for a manual stop use `docker stop -t 900 <container>`. A hard termination or
cleanup failure may leave schemas or a lease behind; inspect `summary.json` and the lease. Results appear in
`.cache/coa-gameplay-tests/<UTC timestamp with nanoseconds>/`. The service does not stop the running worldserver.

With matching server sources, game data, source databases and effective config, Docker invokes the same Python
runner and native runtime, including scenario validation, actions, assertions, SQL updates and cleanup. The
skill's experiment design and result-inspection steps still apply. This does not validate client rendering or
network login, and a static review alone cannot establish runtime parity between Windows and Linux binaries.

## Scenario format

Start from [scenarios/frostbolt.json](scenarios/frostbolt.json). Schema version 1 accepts up to eight players,
eight creatures and 10,000 sequential steps. Optional `timeout_ms` bounds setup plus execution (default 90s,
maximum 10 minutes). Optional `contract` records the independently established expected behavior.
The [talent and item scenario](scenarios/talent-and-items.json) exercises talent learning, passive removal,
equipping a shirt and consuming a healing potion. It does not measure the talent's damage coefficient.
The [Shadowblast scenario](scenarios/shadowblast-shadow-rage.json) reproduces a Shadow Rage pet-targeting crash
and checks the buff's recipient, with ordinary Frostbolt casts as a control.
The [Shadow Effigy scenario](scenarios/shadow-effigy.json) checks combat casts, one active effigy per owner,
nearby-enemy debuffs, replacement by another effigy and timed despawn.
The [Dusk Blade scenario](scenarios/dusk-blade.json) checks dual-wield damage, Rage spending and healing
the wounded caster across repeated melee casts.
The [Who scenarios](scenarios/who-lists-bots.json) check both sides of `Who.ShowBots`: bot sessions are listed
like players with the shipped `Who.ShowBots=1`, and the [hidden case](scenarios/who-hides-bots.json) requires
`Who.ShowBots=0` in the source config, where the same roster leaves only the two real players in the response
and a name search for a bot returns nothing.
The [resource talents scenario](scenarios/resource-talents.json) checks the live-tree 1% resource bonuses.
Arm of Thorim rolls 133–144 base damage at the fixture level, so two independent rolls need ratio ranges
of 1.10–1.31 with its 20% bonus and 0.91–1.09 without it (including integer rounding). Charged Conduit
preserves Static and must leave the talent without a depletion bonus.

The [damage-led scaling scenario](scenarios/level-scaling-damage-engagement.json) checks that an
out-of-range attacker scales a fresh creature before a nonlethal or lethal opening hit, and that
later damage leaves its combat level fixed. It requires `CoA.LevelScaling=1`,
`CoA.LevelScalingMaxLift=5` and `MonsterSight=50`. The level-1 fixtures stand 80–85 yards
away and must scale to level 6, so both declare `level_scaling`. One fixture has only one maximum HP to
expose damage-before-scaling.
Spell 705798 is learned as a fixture: its one damage and zero initial threat exercise damage-led
engagement through the normal cast handler. This tests the damage path, not an Overload proc or pet AI.

Players require `id`, numeric `race` and `class`; `level` defaults to 80. Optional `bot` logs the actor in on a
session flagged as a bot, the way playerbots flags the sessions it creates, so a scenario can check what the
server does differently for them. Optional `spell_hit_rating`,
`spell_crit_rating`, `melee_crit_rating`, `ranged_hit_rating`, `melee_hit_rating` and `expertise_rating` add fixture ratings through
normal calculations, useful for preventing misses, dodges and parries in deterministic tests.
Optional `allow_regeneration: false` suppresses only that fixture player's ordinary health/power regeneration
through the native regeneration hook. Spell costs, healing, energize effects and combat remain enabled.
It defaults to true and has no effect on other players or on a disabled harness.
Characters are created and loaded through the existing character creation, enumeration and login
handlers with ordinary player security. Optional `location` supplies `map`, `x`, `y`, `z`, `o` for a fixture
teleport. `location.ignore_access` optionally bypasses entry requirements for a fixture (for example a solo
raid test), without enabling GM mode during combat. Actors share phase `1 << 30` to isolate ordinary spawns.

Creatures require `id`, player `owner` and template `entry`. Optional `distance` offsets X from their owner
(default 3 yards); `faction`, `level`, `health` default to 14, 80, 100000. They retain template data and AI,
with passive reaction and health regeneration disabled. Pick a template whose scripts suit the experiment.
Setup clears combat initiated by spawn-time AI before starting the scenario. Later combat follows normal rules.
Local level scaling ignores fixtures, because it rebuilds a creature through `SelectLevel()` and would discard
the declared `level` and `health`; optional `level_scaling` (default false) opts a fixture back into it, which
only the damage-led scaling scenario above needs. Creature AI can still change initial fixture levels and
maximum health. Let them settle before taking baselines; assert stable maximums and final levels when testing
damage coefficients.

| Action | Fields and behavior |
| --- | --- |
| `console` | `command`: execute one console command on the test server; capture its output. |
| `command` | `actor`, `command` beginning with `.`: execute with the player's normal permissions. |
| `learn`, `unlearn` | `actor`, `spell`: configure learned spells/passives through player APIs. `unlearn` accepts `all_specs: true` to remove the fixture grant from every specialization before testing a lower weapon rank. |
| `money` | `actor`, `copper`: fixture purse, so a priced trainer row can be bought on a character that starts with none. |
| `set_aura` | `actor`, `spell`, `stacks`: fixture aura state, within its stack limit; zero removes it. Optional `pet: true` selects the actor's current pet. |
| `cancel_aura` | Player `actor`, `spell`: native `CMSG_CANCEL_AURA` handler; assert the resulting aura state. |
| `talent` | `actor`, `talent`, zero-based `rank`: learn with normal point/prerequisite checks. |
| `reset_talents` | `actor`: reset active talents through normal removal, without a trainer fee. |
| `cast` | `actor`, `spell`, optional `target` (self by default): normal session cast handler. |
| `attack` | `actor`, `target`: native melee attack request; optional `pet: true` sends the pet's attack command. Verify combat or damage with assertions. |
| `stop_attack` | Player `actor`: native melee stop request. |
| `pvp` | Player `actor`, boolean `enabled`: native PvP toggle request. Disabling retains the ordinary flag-removal timer. |
| `set_moving` | Player `actor`, boolean `enabled`: fixture the native forward movement flag for cast restriction tests. |
| `group` | `actor`, `target`: fixture party; creates the actor's group if needed and adds an ungrouped player. |
| `cast_charm` | Same fields: native pet-cast handler, with the charmed unit as the default target. |
| `gossip_hello` | `actor`, optional `target`: native gossip handler; defaults to the actor's summoned companion. |
| `banker_activate` | `actor`, optional `target`, or optional `owner` + `entry`: native banker click (`CMSG_BANKER_ACTIVATE`); defaults to the actor's summoned companion, and `owner` aims it at a companion another actor summoned, walking up to it first. |
| `area_trigger` | `actor`, `id`: native area-trigger packet, as the client sends on walking into one; inn triggers are what set the rested flag. |
| `gossip_select` | `actor`, zero-based `option`: select from the current menu through the session handler. |
| `who` | `actor`, optional name-filter `target`, `class_mask`, `race_mask`: submit a native Who query. |
| `add_item` | `actor`, `item`, optional `count` (default 1): grant fixture inventory. |
| `fill_bags` | `actor`, optional `slots` (default 0): fill the bags with distinct non-stacking armor until that many free slots remain, so a scenario can prove what a full inventory does. Fails if the bags cannot be filled. |
| `equip` | `actor`, `item`, `slot` (0..18): equip an owned item through the session handler. |
| `use_item` | `actor`, `item`, `spell`, optional `target` and `destination`: normal item-use handler. |
| `use_gameobject` | `actor`, `entry`: native use request for the actor's single nearby owned gameobject. |
| `set_skill` | `actor`, `skill`, `value`, `maximum`: fixture a native profession skill. |
| `gather_skill` | `actor`, gathering `skill`, `required`: native gathering XP and skill-up attempt. |
| `set_xp_enabled` | `actor`, boolean `enabled`: fixture the native XP-lock flag. |
| `set_level` | `actor`, `value` (1..80): fixture level change through native `GiveLevel`, including level-change hooks. |
| `set_health`, `set_power` | `actor`, `value` within native maximums; `set_power` accepts `power` (default 0). Optional `pet: true` selects the player's current pet. |
| `reset_cooldown` | Player `actor`, `spell`: reset that native spell cooldown between independent cases. |
| `restore_charges` | Player `actor`, `spell`: restore the native charge pool between independent cases. Separate from ordinary cooldowns. |
| `wait` | `ms`: let the real world continue updating. |
| `snapshot` | `actor`, `metric`, `save_as`: remember a numeric observation. |
| `assert` | `actor`, `metric`, `equals` and/or `min`/`max`: check an observation. |

`set_health` also accepts an explicit `maximum` for a player or their pet, using native `SetMaxHealth`.
This fixture supports exact health-percentage boundaries without granting GM permissions.

`gather_skill` calls `UpdateGatherSkill`; it does not harvest a node or prove loot delivery.

`xp` and `next_level_xp` read the player's XP fields; `skill_value` requires `skill` and reads pure skill.
XP-delta assertions must also keep the level stable, or crossing a level would wrap the XP bar.

Every step accepts a descriptive `label`. Assertions optionally accept `within_ms`: poll until the expected
state appears, failing at the deadline. This means "eventually", not "remains true throughout the window".
Equipment changes obey combat restrictions. Prepare gear before starting combat, including combat caused
by other nearby fixture actors. Rejected equipment actions include native inventory error codes in the result.

`spell_charges` requires a player's `spell` and reads its currently available native charges. Charge tests
must assert consumption and recovery after normal casts; restoring fixture charges does not prove recovery.
For absence checks, wait through the relevant cast/proc window first, then assert. `relative_to` subtracts
a previously named snapshot of the same metric; it is available on snapshots and assertions.
`ratio_to` then divides by a nonzero snapshot, including a different numeric metric such as healing/damage.
`cast` accepts an optional `destination` with `x`, `y`, `z` to send an explicit ground target.

Metrics: `health`, `max_health`, `power`, `max_power`, `alive`, `combat`, `casting`, `level`, `quest_objective_count` (needs `quest`, optional `index`), `knows_spell`,
`has_talent`, `talent_points`, `cooldown_ms`, `item_count`, `carried_item_count`, `bank_bag_slots`, `aura`, `aura_stacks`, `aura_charges`,
`aura_duration_ms`, `aura_amount`, `pet_entry`, `pet_aura_stacks`, `owned_creature_count`,
`charm_entry`, `charm_aura_stacks`, `controls_self`, `private_instance`, `dynamic_object`,
`dynamic_object_duration_ms`, `distance`, `spell_proc_count`, `spell_cast_count`, `temporary_spell_replacement`,
`bank_shows`, `system_messages`, `cast_failure`, `pet_is_banker`, `pet_display`, `pet_scale`.
`free_inventory_slots` is how many bag slots the player could still fill, so `fill_bags` plus
`free_inventory_slots` `equals: 0` is how a scenario states "the bags are full". `mail_count` is the
number of mails the player holds and `mail_item_count` the items inside them, which is how a reward
that the bags could not take proves it was posted rather than lost; `mail_has_item` takes `item` and
returns whether any mail carries it, and `mail_pool_item_count` takes `cache` (optional `table`) and
counts only the mail items that are in that cache's own pool. `notifications` counts the
centre-screen notices a session has been sent and `notification_contains` takes `text` and returns
whether one carried it, which is how a test proves a player was told something in the middle of the
screen and not only in chat. The cache metrics are `carried_pool_item_count` (needs `cache`,
optional `table`), `pool_variant_count`, `pool_retired_item_count`, `pool_row_count`,
`pool_item_present` (needs `item`), and `cache_token_count`, `cache_token_stage`, `cache_token_present`
(need `cache`, the last also `item`), which read the token table the realm loads and answer how many
tier tokens a cache may pay, the highest tier among them, and whether one named token is among them.
Boolean metrics use 0/1. Spell/aura metrics require `spell`; `item_count` requires `item`.
`stunned` reads the unit's native stun state, including changes caused by aura removal.
`carried_item_count` sums the stack counts of equipped items (bags included), the backpack and the bags' contents.
`aura_positive` reads the applied aura's beneficial flag; check `aura` separately to distinguish absence from a debuff.
`gossip_options` counts the player's current server-side gossip options; it does not verify client rendering.
`trainer_list_packets` counts the trainer windows the session has been sent, `trainer_window_rows` is the row
count of the last one, and `trainer_window_state` requires `spell` and returns the state byte that window gave
the spell's row (`0` available, `1` unavailable, `2` known), or `-1` when the window does not hold that row.
They read what a client draws and gates **Train** on, so a window that stopped selling a spell is distinct
from one that still offers it.
`who_count` counts players in the actor's last native Who response; `who_class` requires a player `target`
and returns that player's class ID, or zero if absent. These inspect packets from socketless test sessions,
not client packet delivery. Masks use native Who bits (`1 << classID`, `1 << raceID`), with class 32 in bit zero;
omitted masks mean all. The custom-class scenario expects ordinary player RBAC, including faction separation.
`health_pct` observes current health as a percentage of maximum health.
`cast_speed_multiplier` observes the native cast-time multiplier; smaller values mean faster casts.
`spell_crit_chance` observes the player's Shadow spell critical chance, in percentage points.
`spell_damage_done` and `melee_damage_done` require `target` and query native outgoing damage calculations
with a fixed base of 1000. The spell metric also requires `spell` and accepts `effect` (default 0); the melee metric uses a main-hand white hit.
`spell_damage_taken` and `melee_damage_taken` query the corresponding incoming bonus calculations for any
unit; `target` identifies the attacker, and `melee_damage_taken` optionally accepts a weapon-strike `spell`.
These queries do not execute attacks or include hit rolls, critical hits, armor/resistance mitigation, or proc
effects.
`spell_power_cost` requires `spell` and queries its current native resource cost; it does not submit a cast.
`spell_crit_chance` optionally accepts `school` (0..6, default Shadow). `melee_crit_chance`, `dodge_chance` and
`parry_chance` read the player's percentage fields; `expertise` reads main-hand expertise; `combat_rating` requires
`rating` (0..24, native `CombatRating`) and reads the rating value. `stat` requires `stat` (0..4), `resistance`
requires `school` (1..6); `armor`, `attack_power`, `ranged_attack_power`, the hasted `attack_time_ms` (optional
`hand`, 0..2) and `run_speed_rate` read the unit's current totals. `aura_amplitude_ms` reads an aura effect's
periodic interval.
`block_chance` reads the player's percentage field; `block_value` reads native shield block value;
`critical_block_chance` reads the total modifier used by the native critical block roll.
`moving` reads the unit's native movement state. `water_walk` reports whether the unit has a water-walking aura.
`distance_2d` requires `target` and measures horizontal center distance.
`forced_forward` reads the server's force-movement flag; it does not simulate client movement or navigation.
`cast_remaining_ms` requires `spell` and returns its active cast/channel timer, or zero when inactive.
`cast_pushback_ms` reads the player's cumulative native cast-delay notifications, excluding elapsed cast time.
`weapon_damage_min` reads the calculated minimum damage, including weapon-dependent passive bonuses;
optional `hand` selects main hand (0, default), off hand (1), or ranged (2).
`spell_critical_damage` requires `spell` and `target` and calculates a critical hit from a fixed base of 1000,
including native critical damage modifiers, without executing an attack or applying mitigation.
`armor_reduced_damage` requires `spell` and `target` and applies native armor mitigation to a fixed base of
1000, including the attacker's armor penetration; optional `pet: true` selects the player's current pet.
It does not execute an attack.
`spell_uses_armor` separately checks whether the native damage path applies armor to `spell` and `effect`
(default 0), including spell school, armor bypass and bleed mechanics.
`aoe_damage_taken` applies native area damage avoidance to 1000 damage for `school` (0..6).
`reputation_gain` calculates a native spell reputation reward of 1000 for faction `id`, without granting it.
`spell_immune` and `spell_effect_immune` query native immunity against `spell` from `target`; the latter
accepts `effect` (default 0). These queries submit no attack.
`melee_attack_count` counts the actor's native melee combat packets, including extra attacks and misses.
`melee_damage_count` counts only those dealing positive damage. Both accept `hand` (0 main hand, 1 off hand).
These observations do not test delivery to a network client.
`spell_damage_count` and `spell_damage_total` require `spell` and count positive direct or periodic spell
damage events, or sum their post-mitigation damage, from the actor's native combat packets. Optional `target`
filters the victim, `pet: true` selects the actor's current pet as caster, and `critical` filters critical or
noncritical hits. Values accumulate throughout the scenario; use snapshots and `relative_to` around a cast.
They exclude zero damage, melee swing packets, and healing, and do not test network delivery.
`spell_heal_count`, `spell_heal_total` and `spell_effective_heal_total` similarly observe native direct and
periodic healing logs, counting positive heals, summing healing including overhealing, or summing effective
healing. They accept the same caster/critical filters; `target_pet: true` selects the current pet of a player
`target`. Absorbed healing is excluded. These metrics avoid confusing normal regeneration with spell healing.
The result's optional `cast_failures` array records native `SMSG_CAST_FAILED` spell IDs, cast counters and
numeric `SpellCastResult` reasons. These diagnose a rejected submission; effect assertions still establish success.
`distance` requires `target` and measures the native two-dimensional distance, in yards, between the actor and
that target. It reads position and nothing else, so displacement from a knockback, pull or teleport shows up as
the difference between two observations; take a `snapshot` first and assert `relative_to` it. Height is excluded.
`spell_proc_count` requires `spell` and counts the procs of that spell's aura on the actor since the scenario
started. What is counted is each spell the proc cast while the aura was named as its trigger, which is the one
place the server records both the proc and its owner; an aura whose proc does not cast anything counts zero.
Use it for a proc whose chance is below 100%, where a single roll proves nothing: cast the trigger often enough
that the false-failure probability is acceptable, and assert a `min` on the count.
`spell_cast_count` requires `spell` and counts the casts of that exact spell the actor completed since the scenario
started, triggered casts included. Use it where a script casts the effect directly, so no aura is named as the trigger
and `spell_proc_count` reads zero.
Spell queries require `spell` and submit nothing: `spell_modifier` applies the player's native spell modifiers for
`op` (`SpellModOp`) to the number `base`; `spell_effect_value` (optional `effect`) returns the effect's value as the
player would cast it, including module base-value hooks; `spell_cast_time_ms`, `spell_max_range` and
`spell_max_stacks` return the modified native values; `spell_healing_done` requires `target` and optional
`effect`, with a fixed base of 1000. `spell_healing_done` and `spell_damage_done` accept `periodic: true`
to query the native periodic coefficient path instead of direct healing/damage.
`spell_effect_value` and `spell_damage_done` accept `pet: true` to calculate using the player's current pet.
`melee_hit_chance`/`spell_hit_chance` read the player's hit modifiers and `spell_power` (`school` 1..6) its base
spell damage bonus. `spell_done_crit_chance` and `melee_spell_damage_done` require `spell` and `target`: the native
crit chance for that spell, and the weapon-spell damage bonus from a fixed base of 1000. `spell_done_crit_chance`
only reflects native `ApplySpellMod(SPELLMOD_CRITICAL_CHANCE)` modifiers (a bare `Unit::SpellDoneCritChance` query);
it does not invoke `AllSpellScript::OnSpellCritChance`, which only runs mid-cast (`Spell::DoAllEffectOnTarget`).
`spell_done_crit_chance_scripted` requires the same `spell`/`target` and additionally runs that hook via a
throwaway, never-cast `Spell` instance, so a script-hook-only crit bonus is observable without a real cast.
`aura_crit_chance` reads a
periodic aura effect's snapshotted crit chance; `aura_script_value` requires `key`. `script_melee_damage_taken`,
`script_spell_damage_taken` and `script_periodic_damage_taken` require `target` as the attacker (and `spell` for
the latter two) and return 1000 after the registered module damage-taken hooks. `script_heal_received` requires
`spell` and `target` as the healer and returns 1000 after the registered heal-received hooks, with the actor as recipient.
`set_health` also accepts a creature actor, or `pet: true` with a player actor to set its current pet's health.
`open_item` takes `actor` and `item` and submits the native container-open packet, offering it to the
packet hooks first as `WorldSession::Update` does. `close_loot` takes `actor`
and closes its current loot window. `collect_loot` takes `actor`, collects slot zero, verifies that its full rolled
quantity reached inventory and records the item/count. It supports ordinary container loot, not quest-only slots.
`loot_count` and `loot_entry` report the actor's current uncollected item slots and first entry; `loot_received`
reports the inventory increase from its last successful `collect_loot`. Closed windows return zero slots/entry.
`quest_rewarded` requires `quest` and reads the player's native rewarded status.
`prepare_quest` takes `actor` and `quest`, adds the quest and required delivery items, then completes its objectives (unless `complete` is false, which leaves the quest in progress)
as fixture setup. `reward_quest` takes the same fields and optional zero-based `choice` (default 0); it checks normal
reward eligibility and invokes native reward delivery. These actions do not test quest-giver interaction or objectives.
`restore_quest_spells` takes `actor` and invokes the native restoration of spells from rewarded quests.
`login_hooks` takes `actor` and replays registered player-login hooks on the current character; it does not reconnect
or reload the character from the database. Use it to exercise a repair against deliberately seeded fixture state.
`temporary_spell_replacement` requires `spell` and returns the spell ID currently standing in for it on the
player's bars. `Player::GetTemporarySpellReplacement` returns the queried spell itself when nothing replaces
it, so the unreplaced reading is that spell's own ID, never zero. It reads server-side state, not what the
client draws.
`has_talent` requires the talent rank's spell ID; passive talents are separate from the learned spellbook.
`talent_points` measures unspent points in the active specialization.
`bank_bag_slots` measures the player's unlocked standard bank bag slots (0..7).
`pet_entry` measures the player's current guardian pet entry, or the entry of the companion it summoned
(a minipet, which never occupies the guardian slot), or zero if absent; `pet_display`, `pet_scale`
and `pet_is_banker` read the same unit.
`bank_shows` counts the native bank windows the actor's session has been sent, which is what a
banker click is answered with. `system_messages` counts the chat lines the session has been sent.
`cast_failure` requires `spell` and reports the reason the client was told the last submitted cast of
that spell was refused, or zero if it was not refused since (the record is cleared when the scenario
submits that spell again).
`pet_aura_stacks`
requires `spell`, accepts `caster` for aura ownership, and returns zero if the pet or aura is absent.
`pet_aura_amount` and `pet_aura_amplitude_ms` accept `effect` and read its amount or tick interval.
`pet_aura_duration_ms` reads the same pet aura's remaining duration, and returns zero if the pet or aura is absent.
`spell_energize_count` and `spell_energize_total` observe native instant and periodic energize logs, excluding
ordinary regeneration. They require `spell`; optional `power`, `target`, `pet` and `target_pet` filter
resource type, recipient and current pets. The total is the logged nominal gain before the resource cap.

`pet_max_health`, `pet_attack_power` and `pet_run_speed_rate` read the current pet's totals and require a present pet.
`charm_entry` and `charm_aura_stacks` observe the player's charmed unit in the same way.
`controls_self` checks that the player's movement controller is their own character.
`private_instance` checks membership in a scripted private map such as Manastorm.
`dynamic_object` checks for the player's ground effect with the specified `spell`.
`dynamic_object_duration_ms` measures its remaining duration, or zero when absent.
`display_id` reads the unit's selected server display ID; it does not verify client rendering or animations.
`global_cooldown_ms` requires `spell` and reads the remaining native global cooldown for its recovery category.
Player commands retain normal permission and gameplay checks; verify their effects with assertions.
`owned_creature_count` requires a player and `entry`. It counts living creatures of that entry owned, created or summoned by
the player, in the same phase and within 100 yards, including summons outside the guardian-pet slot.
An optional `spell` restricts the count to creatures with that aura; `caster` can select its aura owner. `min_distance` keeps creatures at least that many yards from the player (2D), and `owner_display: true` those wearing the player's display.
`owned_gameobject_count` requires a player and `entry`. It counts their summoned gameobjects of that entry
in the same phase and within 100 yards. `gameobject_remaining_ms` uses the same lookup and requires exactly
one object when present; it returns the remaining lifetime with one-second precision, zero when absent,
or -1 for an object without an expiry. Moving out of range is not proof of despawn.
`at_homebind` checks that the player is on their homebind map and within five yards of its position.
`use_gameobject` keeps normal interaction-distance and usability checks. It does not inspect a rendered UI.
The [portable gadgets scenario](scenarios/portable-gadgets.json) checks item summons, lifetimes, portal
teleports and expiry. It requires `mod-portablemail`; mailbox and altar client interfaces are not tested.
`power`/`max_power` and `pet_power`/`pet_max_power` accept a numeric `power` (0..6).
The pet queries require a player with a current pet. Aura metrics optionally accept `caster` to select
ownership; `aura_amount` also accepts an effect index (0..2, default 0). Missing auras yield zero;
check aura presence separately when zero is a valid effect amount. Permanent aura duration is -1.

### Destiny Weaver regressions

`scenarios/destiny-weaver-scaling.json` checks deferred scaling choices, armor debuffs, creature values
updates after level changes, fractional damage accumulation, and ordinary damage with scaling off.
It requires `DestinyWeaver.Enable=1`, `DestinyWeaver.LevelScaling=1`, `DestinyWeaver.Scaling.Offset=3`,
and `CoA.QuestLevelScaling=1`. Spell 705798 supplies one base damage without critical hits;
Faerie Fire (770) supplies a 5% armor reduction. Spell 705798 uses melee hit resolution, so the fixture
sets melee hit and expertise as well as spell hit. Template 1501 has HealthModifier 0.93: the level-1
fixture's real pool remains 40 HP while its level-57 view has 2,590 HP. Ten one-damage hits cannot remove
a whole real HP; 67 remove one.

`scenarios/destiny-weaver-quest-fallback.json` requires a separate run with `DestinyWeaver.Enable=0`
and `CoA.QuestLevelScaling=1`. Quest 7 must still scale to the player's level and award XP.

The `level_scaling_packet` action takes a player `actor` and `value` (0 or 1). It sends the existing
four-byte request through the early packet hook on a worker, verifies that player state has not changed
before a player update, then leaves subsequent assertions to verify the queued choice took effect.
It tests dispatch and deferral, not a real socket, packet delivery, or every possible concurrent schedule.

`view_level` takes a player `actor` and unit `target` and queries the target-relative combat level.
`sent_level` and `sent_max_health` use the same fields and observe values-only object updates emitted to
the socketless session. They return zero until the corresponding field has been observed; they do not
force updates or inspect client rendering. `quest_level` and `quest_xp` take a player `actor` and `quest`
and query the native quest level and XP calculations without awarding a reward.

## Evidence boundaries

### Optional character names

`scenarios/optional-character-names.json` creates a single-word character, two characters sharing its
first name, and a 25-character full name through normal creation and login handlers. Player fixtures
accept an optional `name`. `player_name` compares the loaded name with the supplied `name` (0/1);
`name_lookup` checks online and character-cache resolution against the actor (0/1). Who assertions
inspect native response packets. These checks require the corresponding server build and SQL update;
they do not validate native client input, rendering, or transport.

The test owns socketless sessions outside the network session manager. Map updates and normal spell/item
handlers execute; character database loading and login hooks execute. Authentication, transport encryption,
network session discovery, actual client packets, rendering, tooltips and UI input are outside this mode's
coverage. Transfers receive synthetic client acknowledgements. Movement/navigation, reconnect and restart
scenarios need additional driver support.

Health and power observations are net state changes. They include regeneration, absorbs, intervening procs
and other effects; they are not per-spell combat-log measurements. Control fixture conditions and use expected
ranges where appropriate. The bundled Frostbolt scenario tests behavior, not exact damage coefficients.
Random proc-rate claims require enough independent trials and a statistical assertion; this version does
not provide an automatic statistical test. Keep intended values independent of the implementation under test.

## Runner checks

```powershell
python -m unittest discover -s apps/coa-gameplay-test -p 'test_*.py'
python apps/codestyle/codestyle-cpp.py --files src/server/coa/CoAGameplayTest.cpp
```

Runner checks cover invalid scenarios, incorrect/partial results, owned-process timeouts, isolation,
partial-clone cleanup, cache reuse/invalidation, ownership and exclusive leases. They do not substitute for
building and running the native scenario.

`spell_go_count` counts the actor's native `SMSG_SPELL_GO` packets for `spell`; optional `pet: true`
selects the current pet. Invisible triggered spells may omit this packet. A cast count proves dispatch,
so pair it with effect assertions. It does not test network delivery or client rendering.
`spell_hit_bonus_taken` reads the victim aura contribution to the native spell hit calculation for `spell`.
`rooted` reads the unit's native root state. `spell_healing_taken` queries incoming healing from `target`
with a base amount of 1000 and requires `spell`; `periodic: true` selects the native HoT path.

`stealth_detection` reads native general stealth detection. `can_detect` requires `target` and invokes
the observer's native `CanSeeOrDetect` check; neither metric covers client rendering.
