# CoA gameplay tests

Execute repeatable scenarios inside a real worldserver, using its loaded DBCs, SQL, scripts, maps and updates.
The runtime component is `modules/mod-ascension-compat/src/CoAGameplayTest.cpp`; it is disabled by default.

## Run

Windows, Python 3.11+, MySQL 8 client tools, a local MySQL server and a worldserver built with the runtime component
are required. Follow the repository's build authorization rules. Adding the new source requires CMake
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
Module `.conf` files beside the source config (in `modules/`) are copied into the test directory's
`configs/modules/` location used by the Windows server, then removed at the end. Their hashes appear in the
summary. Use `--modules-config-dir` for a different source location. Module configs cannot override database
isolation or harness controls. Other platforms require adapting module configuration discovery.

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

## Scenario format

Start from [scenarios/frostbolt.json](scenarios/frostbolt.json). Schema version 1 accepts up to eight players,
eight creatures and 10,000 sequential steps. Optional `timeout_ms` bounds setup plus execution (default 90s,
maximum 10 minutes). Optional `contract` records the independently established expected behavior.
The [talent and item scenario](scenarios/talent-and-items.json) exercises talent learning, passive removal,
equipping a shirt and consuming a healing potion. It does not measure the talent's damage coefficient.

Players require `id`, numeric `race` and `class`; `level` defaults to 80. Optional `spell_hit_rating` adds a
fixture rating through normal rating calculations, useful for eliminating misses in a deterministic smoke
test. Characters are created and loaded through the existing character creation, enumeration and login
handlers with ordinary player security. Optional `location` supplies `map`, `x`, `y`, `z`, `o` for a fixture
teleport. Actors share phase `1 << 30` to isolate them from ordinary spawns.

Creatures require `id`, player `owner` and template `entry`. Optional `distance` offsets X from their owner
(default 3 yards); `faction`, `level`, `health` default to 14, 80, 100000. They retain template data and AI,
with passive reaction and health regeneration disabled. Pick a template whose scripts suit the experiment.
Setup clears combat initiated by spawn-time AI before starting the scenario. Later combat follows normal rules.
Creature AI and local level scaling can still change initial fixture levels and maximum health. Let them settle
before taking baselines; assert stable maximums and final levels when testing damage coefficients.

| Action | Fields and behavior |
| --- | --- |
| `console` | `command`: execute one console command on the test server; capture its output. |
| `learn`, `unlearn` | `actor`, `spell`: configure learned spells/passives through player APIs. |
| `talent` | `actor`, `talent`, zero-based `rank`: learn with normal point/prerequisite checks. |
| `reset_talents` | `actor`: reset active talents through normal removal, without a trainer fee. |
| `cast` | `actor`, `spell`, optional `target` (self by default): normal session cast handler. |
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

Metrics: `health`, `max_health`, `power`, `max_power`, `alive`, `combat`, `casting`, `level`, `knows_spell`,
`has_talent`, `talent_points`, `cooldown_ms`, `item_count`, `aura`, `aura_stacks`, `aura_charges`,
`aura_duration_ms`, `aura_amount`.
Boolean metrics use 0/1. Spell/aura metrics require `spell`; `item_count` requires `item`.
`has_talent` requires the talent rank's spell ID; passive talents are separate from the learned spellbook.
`talent_points` measures unspent points in the active specialization.
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
