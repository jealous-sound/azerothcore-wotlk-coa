# CoA gameplay tests

Execute repeatable scenarios inside a real worldserver, using its loaded DBCs, SQL, scripts, maps and updates.
The runtime component is `modules/mod-ascension-compat/src/CoAGameplayTest.cpp`; it is disabled by default.

## Run

Python 3.11+, MySQL 8 client tools, a local MySQL server and a worldserver built with the runtime component
are required. The commands below run the runner directly (Windows example); Docker installations on Linux use
the [Compose test service](#linux-docker), which provides all of them.
Follow the repository's build authorization rules. Adding the new source requires CMake
reconfiguration before building; running an older binary will fail the readiness check.
The module requires Boost.PropertyTree headers. Component-based vcpkg installations need
`boost-property-tree` for the same triplet as the existing Boost libraries. CMake checks this dependency.

```powershell
python apps/coa-gameplay-test/run.py validate apps/coa-gameplay-test/scenarios/frostbolt.json

python apps/coa-gameplay-test/run.py run apps/coa-gameplay-test/scenarios/frostbolt.json `
  --worldserver C:/path/to/test-build/worldserver.exe `
  --config C:/path/to/worldserver.conf `
  --mysql C:/path/to/mysql.exe `
  --mysqldump C:/path/to/mysqldump.exe
```

The runner creates three unique `coa_test_<run-id>_*` schemas on the source connections. It copies the
world database, the auth/character schemas, RBAC, realm definitions, active arena season and migration metadata.
Existing accounts and characters are not copied. The MySQL user needs read access to the sources and permission
to create/import/drop the test schemas. Sources must be local. No authserver or game client is needed.
The copies omit MySQL triggers, routines and scheduled events.

If the normal server account cannot create schemas, pass `--database-client-config <admin-client.ini>`.
The existing MySQL `[client]` file must provide `host`, `port`, `user` and `password`, with the same host/port
as all source connections. Its credentials are used for cloning, the isolated server and cleanup. For the
local Repack, this file is `C:/Ascension/CoA-Repack/mysql/admin-client.ini`. Credentials stay in temporary
config files, never command arguments or reports; no grants or existing accounts are changed.

The generated config binds the test worldserver to loopback on an unused port, uses a private log directory,
disables map worker threads and points all three database connections to the new schemas. Source SQL updates
run normally against the copies. Source configuration and the installed server are not changed. Relative
`DataDir` is resolved against the binary's directory; use an absolute path when that differs from your setup.
Module `.conf` files beside the source config (in `modules/`) are copied into the directory the worldserver
reads module configs from, then removed at the end. Use `--modules-config-dir` for a different source location.
On Windows that directory is `configs/modules/` relative to the working directory (the test directory), the
default. Elsewhere the worldserver reads `CONF_DIR/modules/`, fixed at build time, so `--server-modules-dir`
is required; existing files there are never replaced. Their hashes appear in the summary. Module configs cannot
override database isolation or harness controls.

Source settings follow the server's precedence: an `AC_*` environment variable (for example `AC_DATA_DIR`)
replaces the value in the source config. The test worldserver inherits the runner's environment except
variables that would replace generated harness values, such as `AC_UPDATES_ENABLE_DATABASES` or
`AC_LOGIN_DATABASE_INFO`, so the generated config always controls isolation, logging and updates.

When the scenario ends, the runtime logs out its test players and shuts down. The runner waits for process
exit before dropping its schemas and removing generated credentials. Startup, scenario, shutdown and copy
operations have timeouts. An interrupted run performs the same cleanup; a hard termination may leave the
named schemas behind. Inspect `summary.json` before removing any leftovers.

Results default to `.cache/coa-gameplay-tests/<run-id>/`:

- `scenario.json`: exact scenario used.
- `worldserver.log`: process output, including startup and script errors.
- `result.json`: server version, actual values and step outcomes.
- `summary.json`: overall result, binary/scenario SHA-256 and any cleanup failure.

Exit code zero requires every expected assertion and step to complete, matching run identity, a clean server
exit and successful database cleanup. A submitted cast alone is never a pass. Numeric fields in the server's
property-tree JSON are strings; the Python runner converts and rechecks assertion values.

### Linux (Docker)

`docker/compose.yml` adds the `ac-gameplay-test` service to the root Compose stack. It uses the locally built
worldserver image plus Python and shares the `ac-database` network namespace, so MySQL is reachable on
`127.0.0.1` and the isolation checks are unchanged. The repository is mounted read-only (runner, scenarios and
SQL updates), the live `DOCKER_VOL_ETC` configs are read-only sources, and `DOCKER_AC_ENV_FILE` applies the same
`AC_*` settings as the live worldserver. Build the worldserver image first; rebuild the test image after it.

```bash
mkdir -p .cache/coa-gameplay-tests
docker compose -f docker-compose.yml -f apps/coa-gameplay-test/docker/compose.yml --profile tests \
  build ac-gameplay-test
docker compose -f docker-compose.yml -f apps/coa-gameplay-test/docker/compose.yml --profile tests \
  run --rm ac-gameplay-test run apps/coa-gameplay-test/scenarios/frostbolt.json
```

`validate <scenario>` works the same way without touching MySQL. The service connects as MySQL `root` with
`DOCKER_DB_ROOT_PASSWORD`, written only to a mode-600 file inside the disposable container. Results appear in
`.cache/coa-gameplay-tests/<UTC timestamp>/`. The service does not stop the running worldserver; stop it to free CPU
if needed.

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

Players require `id`, numeric `race` and `class`; `level` defaults to 80. Optional `spell_hit_rating`,
`ranged_hit_rating`, `melee_hit_rating` and `expertise_rating` add the corresponding fixture rating through
normal calculations, useful for preventing misses, dodges and parries in deterministic tests.
Characters are created and loaded through the existing character creation, enumeration and login
handlers with ordinary player security. Optional `location` supplies `map`, `x`, `y`, `z`, `o` for a fixture
teleport. `location.ignore_access` optionally bypasses entry requirements for a fixture (for example a solo
raid test), without enabling GM mode during combat. Actors share phase `1 << 30` to isolate ordinary spawns.

Creatures require `id`, player `owner` and template `entry`. Optional `distance` offsets X from their owner
(default 3 yards); `faction`, `level`, `health` default to 14, 80, 100000. They retain template data and AI,
with passive reaction and health regeneration disabled. Pick a template whose scripts suit the experiment.
Setup clears combat initiated by spawn-time AI before starting the scenario. Later combat follows normal rules.
Creature AI and local level scaling can still change initial fixture levels and maximum health. Let them settle
before taking baselines; assert stable maximums and final levels when testing damage coefficients.

| Action | Fields and behavior |
| --- | --- |
| `console` | `command`: execute one console command on the test server; capture its output. |
| `command` | `actor`, `command` beginning with `.`: execute with the player's normal permissions. |
| `learn`, `unlearn` | `actor`, `spell`: configure learned spells/passives through player APIs. |
| `talent` | `actor`, `talent`, zero-based `rank`: learn with normal point/prerequisite checks. |
| `reset_talents` | `actor`: reset active talents through normal removal, without a trainer fee. |
| `cast` | `actor`, `spell`, optional `target` (self by default): normal session cast handler. |
| `cast_charm` | Same fields: native pet-cast handler, with the charmed unit as the default target. |
| `add_item` | `actor`, `item`, optional `count` (default 1): grant fixture inventory. |
| `equip` | `actor`, `item`, `slot` (0..18): equip an owned item through the session handler. |
| `use_item` | `actor`, `item`, `spell`, optional `target`: normal item-use handler. |
| `set_health`, `set_power` | `actor`, `value` within native maximums; `set_power` accepts `power` (default 0). |
| `wait` | `ms`: let the real world continue updating. |
| `snapshot` | `actor`, `metric`, `save_as`: remember a numeric observation. |
| `assert` | `actor`, `metric`, `equals` and/or `min`/`max`: check an observation. |

Every step accepts a descriptive `label`. Assertions optionally accept `within_ms`: poll until the expected
state appears, failing at the deadline. This means "eventually", not "remains true throughout the window".
Equipment changes obey combat restrictions. Prepare gear before starting combat, including combat caused
by other nearby fixture actors. Rejected equipment actions include native inventory error codes in the result.
For absence checks, wait through the relevant cast/proc window first, then assert. `relative_to` subtracts
a previously named snapshot of the same metric; it is available on snapshots and assertions.
`cast` accepts an optional `destination` with `x`, `y`, `z` to send an explicit ground target.

Metrics: `health`, `max_health`, `power`, `max_power`, `alive`, `combat`, `casting`, `level`, `knows_spell`,
`has_talent`, `talent_points`, `cooldown_ms`, `item_count`, `bank_bag_slots`, `aura`, `aura_stacks`, `aura_charges`,
`aura_duration_ms`, `aura_amount`, `pet_entry`, `pet_aura_stacks`, `owned_creature_count`,
`charm_entry`, `charm_aura_stacks`, `controls_self`, `private_instance`, `dynamic_object`,
`dynamic_object_duration_ms`.
Boolean metrics use 0/1. Spell/aura metrics require `spell`; `item_count` requires `item`.
`has_talent` requires the talent rank's spell ID; passive talents are separate from the learned spellbook.
`talent_points` measures unspent points in the active specialization.
`bank_bag_slots` measures the player's unlocked standard bank bag slots (0..7).
`pet_entry` measures the player's current guardian pet entry, or zero if absent. `pet_aura_stacks`
requires `spell`, accepts `caster` for aura ownership, and returns zero if the pet or aura is absent.
`charm_entry` and `charm_aura_stacks` observe the player's charmed unit in the same way.
`controls_self` checks that the player's movement controller is their own character.
`private_instance` checks membership in a scripted private map such as Manastorm.
`dynamic_object` checks for the player's ground effect with the specified `spell`.
`dynamic_object_duration_ms` measures its remaining duration, or zero when absent.
Player commands retain normal permission and gameplay checks; verify their effects with assertions.
`owned_creature_count` requires a player and `entry`. It counts living creatures of that entry owned by
the player, in the same phase and within 100 yards, including summons outside the guardian-pet slot.
An optional `spell` restricts the count to creatures with that aura; `caster` can select its aura owner.
`power`/`max_power` accept a numeric `power` (0..6). Aura metrics optionally accept `caster` to select
ownership; `aura_amount` also accepts an effect index (0..2, default 0). Missing auras yield zero;
check aura presence separately when zero is a valid effect amount. Permanent aura duration is -1.

## Evidence boundaries

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
python apps/coa-gameplay-test/test_runner.py
python apps/codestyle/codestyle-cpp.py --files modules/mod-ascension-compat/src/CoAGameplayTest.cpp
```

Runner checks cover invalid scenarios, incorrect/partial results, owned-process timeouts, isolation and
partial-clone cleanup. They do not substitute for building and running the native scenario.
