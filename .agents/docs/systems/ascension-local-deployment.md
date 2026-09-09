# Guarded local Ascension deployment

Reusable lessons from the 2026-09-06 deployment. Follow current root authorization and build rules;
this guide does not itself authorize builds, restarts or wider mutations. Only the copied local client
and isolated local services are in scope. Historical task installers are fixed plans, not generic tools.

## Keep the change set coherent

1. Identify the generator/policy and exact before/after scope for SQL, binary and client data. Preserve
   dirty worktrees. Record candidate hashes and package contents; never silently accept unknown drift.
2. A new module `.cpp` required reconfiguring the existing CMake build to refresh its source glob.
   Do not infer inclusion from a successful incremental build. Configure/build only when authorized;
   follow `build.md` and the current tested parallelism limits rather than unbounded `/MP`.
3. Build client packages from canonical candidates. Resolve the effective MPQ carrier for each member;
   patch-B is not a universal DBC carrier. Re-extract and hash-check the new member and prove that
   unrelated content is unchanged. A verified packaging manifest is not proof of live deployment.
4. Gracefully close the exact copied-client process so SavedVariables flush, stop the local world
   safely, then take and verify the relevant offline backup. Preserve original Launcher and caches.
   State backup scope accurately: all character tables plus selected world tables is not a whole-world dump.
5. Verify current state against the backup before installing anything. Install the coherent candidate
   set while offline, then verify files, pending migration list/hashes and protected data again.
6. Apply SQL through the normal updater at server startup; do not manually apply it as well. Verify the
   exact new rows, unchanged unrelated rows, updater name/hash, readiness and new unique errors before login.
   `updates.state = PENDING` can describe an already applied pending-directory migration; inspect its
   recorded name/hash and the actual pending-file comparison, not that state string alone.
7. Launch with the guarded client helper and verify fresh process/path, archive hashes, stock auth,
   addon permission and existing compatibility checks. Do not reuse an old success JSON as fresh proof.
   A running client and correct archives still do not prove a rendered tooltip or real combat result.

## Treat verification reads as data with failure modes

- A MySQL CLI exit code of zero and empty stdout did occur for nonempty/metadata reads. Do not interpret
  that as an empty table or a verification PASS. Frame read-only output with explicit begin/end markers
  and reject missing/truncated framing; metadata queries such as SHOW COLUMNS must also return columns.
- Bound retries to the identical SELECT/SHOW and record retries. Never retry ambiguous writes by analogy.
  Do not strip malformed rows or weaken fingerprints to make the comparison pass. Keep credentials out
  of tool output, normalized reports and manifests.
- Preserve exact pre/post row evidence for operational deltas. A live world legitimately changes
  respawn timers/worldstates, but this does not justify ignoring entire tables. Validate particular
  rows, keys, map/instance and source of change; unexpected changes stop verification.
- **User correction, 2026-09-06:** respawn timers and Wintergrasp state are expected to advance after
  restart even with no players online. Do not repeatedly compare them to an ever-stale exact snapshot,
  misdiagnose this as an installation failure, or restart solely to force equality. Keep personal-data
  and intended SQL checks strict; review a bounded operational snapshot separately and state its time.
  Verification scope should be proportional to the change, not an unrelated full-world immobility test.
- Report strict whole-database equality separately from a reviewed operational-delta result. The
  Barbarian deployment's 32 expired outdoor Northrend respawns are an example, not a blanket future
  allowance. Personal characters, inventory, spells, skills and collections remain protected.
- Compare new unique startup errors as well as readiness. Existing noisy errors should neither hide a
  new failure nor be called fixed by this deployment. A failed guard must not be relabelled PASS.

## Race/class availability is a two-sided data contract

- The copied client's native `IsRaceClassValid` reads `CharBaseInfo.dbc`; the local Glue wrapper's
  `CanCreateClass` controls the set of classes, not race compatibility. Preserve these distinct gates.
  A Lua-only "true" override would not prove that native selection or server creation accepts a pair.
- This client uses two **byte** fields per on-disk CharBaseInfo record, but eight-byte native records.
  Do not parse it as a conventional DWORD-per-field DBC. Preserve existing records, ordering and even
  duplicates; append only the reviewed missing pairs and update the record count.
- `ObjectMgr::LoadPlayerInfo` builds the server's race/class availability from `playercreateinfo`.
  Copy the race's unambiguous existing start, not an arbitrary class donor's spawn. Class and race
  statistics are separate tables here; verify all intended levels and class-wide starter-item gates.
- A successful archive extraction does not settle override precedence. The bounded read-only probe
  `tools/Test-AnyRaceClassClient.py` compares the actual loaded table and unhooked native predicate
  against the candidate, using the exact copied executable path. This is not rendered UI or native
  character-creation testing. Glue uses UI race indices: index 9 means Blood Elf, not race ID 9.
- Racial skills with class-specific variants need an explicit resource/stat mapping for newly opened
  classes. Do not OR every class into every variant: that would grant multiple mutually exclusive
  racials. Record unhandled variants separately from the availability change.

## PowerShell and installer regressions

- Python launched from pwsh/VS can pass a PowerShell 7 `PSModulePath` into Windows PowerShell 5.1.
  This caused `Get-FileHash` to be unavailable in an otherwise valid isolated installer test.
  Check the child interpreter/module environment before blaming the installer or removing hash checks.
- For built-in-only fixtures, set the **child process** module path to that interpreter's `Modules`
  directory. Test both System32 and SysWOW64 Windows PowerShell 5.1 when the actual launcher is x86.
  Do not globally change the user's module path or assume the fixture setup replaces production checks.
- Assert the expected failure message, not merely a nonzero exit status: a missing cmdlet must not
  masquerade as a successful wrong-hash or running-client rejection. Verify target bytes and backups.
- Cover initial install, idempotent re-run, tampered source, unknown live archive and active copied
  client. Reject linked/out-of-scope paths. Verify the recoverable backup before replacement and the
  installed hash afterward. A hash-identical no-op may be read-only while the client is running.

## Handoff and rollback

Keep dated deployment records with exact hashes, applied SQL, backup scope/path, phase results,
known deltas and untested gameplay. Keep reusable rules here instead of duplicating old hashes.
An old executable alone is not necessarily a valid rollback after a SQL migration/client data change.
Review a coherent reverse plan and preserve player progress acquired since the backup; do not restore
the entire character database merely to undo a spell coefficient or to make a comparison pass.

Examples to inspect, not blindly rerun for a new task:

- `C:/Ascension/tools/Check-BarbarianDeployment.py` — framed read-only queries and bounded retries.
- `C:/Ascension/tools/Check-BarbarianDamageDeployment.py` — backup/preinstall/offline-installed/live phases.
- `C:/Ascension/Sync-LocalClientSpellPatch.ps1` — fixed-candidate archive synchronization with backup.
- `C:/Ascension/tools/Test-BarbarianDamageFix.py` — isolated x86/x64 success and failure scenarios.
- `C:/Ascension/runtime/validation/barbarian-damage-20260906/implemented.md` — evidence and limitations.

## Native lifecycle checks learned from Manastorm

- `Player::LoadFromDB` can update achievement criteria before `SetMap`. Global player hooks must
  handle a missing map with `FindMap`; `GetMap` asserts. A mapless player must retain ordinary
  achievement behavior. Scene entry and ownership must also reject mapless transfer states.
- Readiness alone does not exercise character loading. For a lifecycle correction, reproduce the
  failing callback with actual source and a mapless player, preserve the old-code failure, and test
  ordinary/private-map behavior. Keep real login and gameplay acceptance distinct from this harness.
- Native ChatCommandBuilder stores a reference to its child command vector. Keep child tables alive
  (for example, static storage); an inline temporary compiles but can crash command initialization.

## Native spell validation learned from the class follow-up

- Register AuraScript callbacks against the actual spell's valid aura effects. An unconditional
  periodic handler on a nonperiodic aura fails startup validation even if its body would do nothing.
  During registration, resolve `m_scriptSpellId` through SpellMgr; a cast object may not exist yet.
- A nonzero raw `ApplyAuraName` in Spell.dbc does not establish an aura when the effect itself is zero.
  Use native `IsAura`/`HasAura` for proc disable masks and group compatibility. Validate actual bindings,
  not just a simplified hand-built record with the expected flags.
- Group IDs are not semantic names. Expand subgroups and execute the native same-effect inference:
  group 1038 includes a stat-percent subgroup, despite also listing Sanctuary. A damage-taken aura
  requires a group that actually selects its aura type. Verify largest-only stacking and preservation
  of independent absorb/stagger effects using the real native group methods.
- When a later correction adds a previously unmodified table, preserve the original full backup and
  add one verified table supplement. Validate the amended exact plan against both baselines; do not
  rewrite the original backup plan or rerun a full backup merely to satisfy an old table-count check.
