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
- `summary.json`: overall result, binary/scenario SHA-256 and any cleanup failure.

Exit code zero requires every expected assertion and step to complete, matching run identity, a clean server
exit and successful cleanup/cache audit. A submitted cast alone is never a pass. Numeric fields in the server's
property-tree JSON are strings; the Python runner converts and rechecks assertion values.

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
checkout first, with the runtime module and cache startup barrier; rebuild the test image after it. A mounted
source checkout does not update the compiled server. Build authorization is still required.

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
The [resource talents scenario](scenarios/resource-talents.json) checks the live-tree 1% resource bonuses.
Arm of Thorim rolls 133–144 base damage at the fixture level, so two independent rolls need ratio ranges
of 1.10–1.31 with its 20% bonus and 0.91–1.09 without it (including integer rounding). Charged Conduit
preserves Static and must leave the talent without a depletion bonus.

The [damage-led scaling scenario](scenarios/level-scaling-damage-engagement.json) checks that an
out-of-range attacker scales a fresh creature before a nonlethal or lethal opening hit, and that
later damage leaves its combat level fixed. It requires `AscensionCompat.LevelScaling=1`,
`AscensionCompat.LevelScalingMaxLift=5` and `MonsterSight=50`. The level-1 fixtures stand 80–85 yards
away and must scale to level 6. One fixture has only one maximum HP to expose damage-before-scaling.
Spell 705798 is learned as a fixture: its one damage and zero initial threat exercise damage-led
engagement through the normal cast handler. This tests the damage path, not an Overload proc or pet AI.

Players require `id`, numeric `race` and `class`; `level` defaults to 80. Optional `spell_hit_rating`,
`spell_crit_rating`, `ranged_hit_rating`, `melee_hit_rating` and `expertise_rating` add fixture ratings through
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
| `set_aura` | `actor`, `spell`, `stacks`: fixture aura state, within its stack limit; zero removes it. Optional `pet: true` selects the actor's current pet. |
| `talent` | `actor`, `talent`, zero-based `rank`: learn with normal point/prerequisite checks. |
| `reset_talents` | `actor`: reset active talents through normal removal, without a trainer fee. |
| `cast` | `actor`, `spell`, optional `target` (self by default): normal session cast handler. |
| `attack` | `actor`, `target`: native melee attack request; optional `pet: true` sends the pet's attack command. Verify combat or damage with assertions. |
| `group` | `actor`, `target`: fixture party; creates the actor's group if needed and adds an ungrouped player. |
| `cast_charm` | Same fields: native pet-cast handler, with the charmed unit as the default target. |
| `gossip_hello` | `actor`, optional `target`: native gossip handler; defaults to the actor's summoned companion. |
| `gossip_select` | `actor`, zero-based `option`: select from the current menu through the session handler. |
| `who` | `actor`, optional name-filter `target`, `class_mask`, `race_mask`: submit a native Who query. |
| `add_item` | `actor`, `item`, optional `count` (default 1): grant fixture inventory. |
| `equip` | `actor`, `item`, `slot` (0..18): equip an owned item through the session handler. |
| `use_item` | `actor`, `item`, `spell`, optional `target` and `destination`: normal item-use handler. |
| `use_gameobject` | `actor`, `entry`: native use request for the actor's single nearby owned gameobject. |
| `set_level` | `actor`, `value` (1..80): fixture level change through native `GiveLevel`, including level-change hooks. |
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
`ratio_to` then divides by a nonzero snapshot of the same metric, for comparisons such as boosted/base damage.
`cast` accepts an optional `destination` with `x`, `y`, `z` to send an explicit ground target.

Metrics: `health`, `max_health`, `power`, `max_power`, `alive`, `combat`, `casting`, `level`, `quest_objective_count` (needs `quest`, optional `index`), `knows_spell`,
`has_talent`, `talent_points`, `cooldown_ms`, `item_count`, `carried_item_count`, `bank_bag_slots`, `aura`, `aura_stacks`, `aura_charges`,
`aura_duration_ms`, `aura_amount`, `pet_entry`, `pet_aura_stacks`, `owned_creature_count`,
`charm_entry`, `charm_aura_stacks`, `controls_self`, `private_instance`, `dynamic_object`,
`dynamic_object_duration_ms`.
Boolean metrics use 0/1. Spell/aura metrics require `spell`; `item_count` requires `item`.
`carried_item_count` sums the stack counts of equipped items (bags included), the backpack and the bags' contents.
`aura_positive` reads the applied aura's beneficial flag; check `aura` separately to distinguish absence from a debuff.
`gossip_options` counts the player's current server-side gossip options; it does not verify client rendering.
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
`weapon_damage_min` reads the calculated minimum damage, including weapon-dependent passive bonuses;
optional `hand` selects main hand (0, default), off hand (1), or ranged (2).
`spell_critical_damage` requires `spell` and `target` and calculates a critical hit from a fixed base of 1000,
including native critical damage modifiers, without executing an attack or applying mitigation.
`armor_reduced_damage` requires `spell` and `target` and applies native armor mitigation to a fixed base of
1000, including the attacker's armor penetration; it does not execute an attack.
`aoe_damage_taken` applies native area damage avoidance to 1000 damage for `school` (0..6).
`reputation_gain` calculates a native spell reputation reward of 1000 for faction `id`, without granting it.
`spell_immune` and `spell_effect_immune` query native immunity against `spell` from `target`; the latter
accepts `effect` (default 0). These queries submit no attack.
`melee_attack_count` counts the actor's native melee combat packets, including extra attacks and misses;
it observes server output without testing delivery to a network client.
`spell_damage_count` and `spell_damage_total` require `spell` and count positive direct or periodic spell
damage events, or sum their post-mitigation damage, from the actor's native combat packets. Optional `target`
filters the victim, `pet: true` selects the actor's current pet as caster, and `critical` filters critical or
noncritical hits. Values accumulate throughout the scenario; use snapshots and `relative_to` around a cast.
They exclude zero damage, melee swing packets, and healing, and do not test network delivery.
The result's optional `cast_failures` array records native `SMSG_CAST_FAILED` spell IDs, cast counters and
numeric `SpellCastResult` reasons. These diagnose a rejected submission; effect assertions still establish success.
Spell queries require `spell` and submit nothing: `spell_modifier` applies the player's native spell modifiers for
`op` (`SpellModOp`) to the number `base`; `spell_effect_value` (optional `effect`) returns the effect's value as the
player would cast it, including module base-value hooks; `spell_cast_time_ms`, `spell_max_range` and
`spell_max_stacks` return the modified native values; `spell_healing_done` requires `target` and optional
`effect`, with a fixed base of 1000.
`melee_hit_chance`/`spell_hit_chance` read the player's hit modifiers and `spell_power` (`school` 1..6) its base
spell damage bonus. `spell_done_crit_chance` and `melee_spell_damage_done` require `spell` and `target`: the native
crit chance for that spell, and the weapon-spell damage bonus from a fixed base of 1000. `aura_crit_chance` reads a
periodic aura effect's snapshotted crit chance; `aura_script_value` requires `key`. `script_melee_damage_taken`,
`script_spell_damage_taken` and `script_periodic_damage_taken` require `target` as the attacker (and `spell` for
the latter two) and return 1000 after the registered module damage-taken hooks. `set_health` also accepts a
creature actor.
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
`owned_gameobject_count` requires a player and `entry`. It counts their summoned gameobjects of that entry
in the same phase and within 100 yards. `gameobject_remaining_ms` uses the same lookup and requires exactly
one object when present; it returns the remaining lifetime with one-second precision, zero when absent,
or -1 for an object without an expiry. Moving out of range is not proof of despawn.
`at_homebind` checks that the player is on their homebind map and within five yards of its position.
`use_gameobject` keeps normal interaction-distance and usability checks. It does not inspect a rendered UI.
The [portable gadgets scenario](scenarios/portable-gadgets.json) checks item summons, lifetimes, portal
teleports and expiry. It requires `mod-portablemail`; mailbox and altar client interfaces are not tested.
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
python -m unittest discover -s apps/coa-gameplay-test -p 'test_*.py'
python apps/codestyle/codestyle-cpp.py --files modules/mod-ascension-compat/src/CoAGameplayTest.cpp
```

Runner checks cover invalid scenarios, incorrect/partial results, owned-process timeouts, isolation,
partial-clone cleanup, cache reuse/invalidation, ownership and exclusive leases. They do not substitute for
building and running the native scenario.
