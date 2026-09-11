# Guarded local Ascension deployment

Use the relevant sections for an authorized deployment or a change to deployment tooling.
Routine source/documentation edits do not need this workflow. Follow current root authorization and build rules.
Historical task installers are fixed plans, not generic tools. Both Launcher copies contain local changes.

## Keep the change set coherent

1. Identify the affected source, SQL, binary, and client data. Preserve unrelated work and use existing tools.
   Match the candidate to the requested change; do not create source snapshots or manifests by default.
2. A new module `.cpp` required reconfiguring the existing CMake build to refresh its source glob.
   Do not infer inclusion from a successful incremental build. Configure/build only when authorized;
   follow `build.md` and the current tested parallelism limits rather than unbounded `/MP`.
3. Build the final client package from canonical candidates. Resolve the effective MPQ carrier for changed members;
   patch-B is not a universal DBC carrier. Check the final changed members and preserve unrelated content through
   the existing packaging tool. Full archive comparisons are for packaging changes or a concrete corruption concern.
4. Before replacing live files, stop the exact affected process gracefully and preserve one recoverable copy of
   the affected state. Reuse the tool's backup or an equivalent existing copy; do not add a second manual backup.
   Back up databases consistently when the deployment changes them, not for unrelated UI/source edits.
5. Install the coherent candidate with existing guards, then check installed identity and affected behavior.
   Do not compare unrelated files, whole databases, or every historical phase as a routine deployment requirement.
6. Apply SQL through the normal updater at server startup; do not manually apply it as well. Check affected rows,
   migration identity, readiness, and new startup errors. Preserve unrelated/player data.
   `updates.state = PENDING` can describe an already applied pending-directory migration; inspect its
   recorded name/hash and the actual pending-file comparison, not that state string alone.
7. Launch the client for testing only with the user's permission, using the existing helper. State any untested
   gameplay limits; an installed package or running process alone does not prove combat or rendered UI behavior.

## Treat verification reads as data with failure modes

- Empty/truncated MySQL output is not proof that a query returned no rows. Verify meaningful output;
  metadata queries such as SHOW COLUMNS must return columns. Reuse the existing query tooling.
- Bound retries of read-only queries. Never blindly retry ambiguous writes or hide malformed output.
  Keep credentials out of tool output and reports.
- Check the rows and relationships affected by the migration. Respawn timers and Wintergrasp state normally
  advance after restart, even without players. Do not demand whole-database equality or restart to force it.
  Investigate an unexpected change relevant to the task; preserve characters, inventory, spells, and collections.
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

These checks apply when changing the corresponding installer behavior, not when merely using a working installer.

- Python launched from pwsh/VS can pass a PowerShell 7 `PSModulePath` into Windows PowerShell 5.1.
  This caused `Get-FileHash` to be unavailable in an otherwise valid isolated installer test.
  Check the child interpreter/module environment before blaming the installer or removing hash checks.
- For built-in-only fixtures, set the **child process** module path to that interpreter's `Modules`
  directory. Test the affected production interpreter; add x86/x64 coverage when architecture behavior changes.
  Do not globally change the user's module path or assume the fixture setup replaces production checks.
- Assert the expected failure message, not merely a nonzero exit status: a missing cmdlet must not
  masquerade as a successful wrong-hash or running-client rejection. Verify target bytes and backups.
- Cover initial install, idempotent re-run, tampered source, unknown live archive and active copied
  client. Reject linked/out-of-scope paths. Verify the recoverable backup before replacement and the
  installed hash afterward. A hash-identical no-op may be read-only while the client is running.

## Handoff and rollback

For a deployment, record the result, installed version, applied SQL, rollback location, and material limits once
in the existing release/task record. Reuse the tool output; do not generate a separate receipt per check or phase.
Routine source work needs only the conversation summary. Keep reusable rules here and release identity in its record.
An old executable alone is not necessarily a valid rollback after a SQL migration/client data change.
Review a coherent reverse plan and preserve player progress acquired since the backup; do not restore
the entire character database merely to undo a spell coefficient or to make a comparison pass.

Examples to inspect, not blindly rerun for a new task:

- `C:/Ascension/Sync-LocalClientSpellPatch.ps1` — fixed-candidate archive synchronization with backup.
- `C:/Ascension/Sync-LocalClientUiPatch.ps1` — client UI synchronization with backup and identity checks.
- `C:/Ascension/runtime/validation/barbarian-damage-20260906/implemented.md` — evidence and limitations.

## Native lifecycle checks learned from Manastorm

- `Player::LoadFromDB` can update achievement criteria before `SetMap`. Global player hooks must
  handle a missing map with `FindMap`; `GetMap` asserts. A mapless player must retain ordinary
  achievement behavior. Scene entry and ownership must also reject mapless transfer states.
- Readiness alone does not exercise character loading. For a lifecycle correction, use a focused regression
  for the failing callback and affected map states. A new standalone harness or archived failure is not required.
  Keep real login and gameplay acceptance distinct from source checks.
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
- If a deployment's scope grows to another table, preserve its relevant pre-change state once. Reuse existing
  recoverable backups; do not rerun a full backup merely to satisfy an old table-count check.
