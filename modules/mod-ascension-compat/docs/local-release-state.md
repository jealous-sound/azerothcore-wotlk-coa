# Local compatibility release — 2026-09-09

The Necromancer Life Force/summon-size/UI follow-up, Templar, Felsworn, Starcaller and Knight of Xoroth
are now committed, built and installed with the earlier class packages. The user requested a server
build for testing and explicitly excluded launching the client. No client was launched and no Git
push occurred. Current report: `C:/Ascension/runtime/releases/class-followup-20260909/implemented.md`;
current receipt: `validation/deployment-current.json` under that release.

World SHA-256: `FD43708B172B740E7F33F3A506A963DD4B2AA2B98E309DD876A79FBFE3742DAF`.
SourceDirectory: `C:/Ascension/runtime/releases/class-followup-20260909/source`.
Fresh VS2022 x64 RelWithDebInfo build in `C:/Ascension/Build-ClassFollowup`, with bounded `/m:2`
and `CL_MPCount=4`. Final world started at `2026-09-09T16:10:21.3949145+03:00`; readiness passed
at `16:11:42+03:00`. Ports 8085/3443 belong to that world process. New unique startup errors: zero.
MySQL and auth were started when initially absent and were retained during the two world repairs.

Core checkpoint `46bf8121e52c42796e72a400a8ec80dfe14e3c7c` is followed by startup corrections
`057e9fcbe1a248074f04ec60b8d68174af51fd5b` and `12d2cda4d7c8ad59e4da5a1fc0850fecff91f665`.
The final source manifest pins 10,565 files. Subsequent release documentation is separate from this
compiled source snapshot. Normal updater applied world SQL03 through SQL10; all applied migrations,
including the earlier SQL00/01/02, are immutable. Use a new migration for any later correction.

The first startup exposed invalid periodic AuraScript registrations, three inert aura bindings,
two proc disable masks and a mismatched raid group. Actual native tests and new SQL09/10 correct
these without rewriting applied SQL03–08. SQL10 groups Shrouded Stars' damage-taken reduction with
Sanctuary/Vigilance in 2000185, preserving its independent stagger. The first two startup results,
first binary/PDB and failed fixture runs remain in the release record.

The matching two-member Necromancer/Templar B is installed in both canonical and copied-client paths:
`1C35C1D6CC52FD52E6FECA6A1302FE26AD9AE752C97A0F892F0F9ADDF6481AC9`.
Exactly 1,400 archive members are preserved. Server Spell.dbc, three existing model DBCs, T, client
EXE/DLL, cache, WTF and the original client are unchanged. All 22 selected creature model chains and
13 gameobject dependencies resolve; rendered appearance and UI remain untested.

One verified full backup is `C:/Ascension/runtime/server/backups/before-class-followup-20260909-151045-513070`:
117 character tables, 22 auth tables, nine scoped world tables and 716 WTF files. A separate verified
`damage-reduction-repair` supplement protects `spell_group_stack_rules` before SQL10. The final 149-table
snapshot preserves personal data and matches every planned SQL row. Reviewed operational changes are
three zero-player uptime rows, outdoor respawns, Wintergrasp clock 3781 and 33 expired daily heroic
reset schedules. Whole-database equality is false. Reuse this bounded review; do not chase live timers.

Gameplay, summon appearance, movement and rendered resource/Oath UI acceptance remain separate.

## Historical state before this installation

The following source-only and earlier-release notes are superseded by the installation above.

Starcaller is now source ready and uninstalled, with 164 individual dispositions, 124 coefficient
slots, 99 bindings and 35 proc rows in pending SQL06. Its report is
`C:/Ascension/runtime/validation/starcaller-completion-20260909/implemented.md`.
Pending SQL08 corrects 64 Felsworn/Knight proc hit masks to include native block, absorb and full block.
Their earlier low-bit admission test missed those outcomes; the new 24,948-case test supersedes that
portion of the old evidence. Prior SQL remains unchanged. Twenty-seven focused tests, 73 unchanged
regressions, 80 native cost cases and 12 native syntax units pass. New SQL/C++ lint is clean.
No build or installation occurred; source/header hashes, unchanged runtime hashes and a scoped patch
against the post-Knight baseline are captured. The patch is checked in an isolated fixture.

Felsworn and Knight of Xoroth are also source ready and uninstalled. Their pending migrations are
SQL05 and SQL07; reports are `runtime/validation/felsworn-completion-20260909/implemented.md` and
`runtime/validation/knight-of-xoroth-completion-20260909/implemented.md` under `C:/Ascension`.
Knight has 122 implemented/extended and two retained dispositions, 65 coefficient slots, 113 bindings
and 26 proc entries. Its 96 guarded template/model/pet-level/forge inserts need conflict checks before
installation. Existing native models are used without a DBC/client change. Twenty-one focused tests,
52 regressions, 2,808 native proc cases, 80 Sever forwarding cases and 12 native syntax units pass.
Starcaller's subsequent completion and proc correction are above. These source packages do not change
the installed release below.

Pending after this installation: the focused Necromancer Life Force/model/UI correction and the
Templar completion. Both are source ready, uncommitted and not installed. Their reports are
`C:/Ascension/runtime/validation/necromancer-followup-20260909/implemented.md` and
`C:/Ascension/runtime/validation/templar-completion-20260909/implemented.md`.

Necromancer now prevents the duplicate native summon and enforces available Life Force on each paid
minion. Capacity grows through the recovered level-10/Animation level-30 grants and Master Animator;
the matched UI displays free and total points. Pending SQL03 scales 11 exact summon templates.
Templar has dispositions for all 25 findings, including Oaths, stagger, finite charges/replacements,
113 coefficient slots, proc routing and owned summons. Pending SQL04 supplies 86 bindings, 32 proc
entries and guarded Hope definitions. The combined two-member client candidate has SHA-256
`1C35C1D6CC52FD52E6FECA6A1302FE26AD9AE752C97A0F892F0F9ADDF6481AC9`; 1,400 other members are preserved.

No linked build, installation or gameplay acceptance occurred for these follow-ups. Use their scoped
patches and final manifests. Reject conflicting Hope definitions before installation, and retain the
matching Life Force client/server dependency. The installed release described below remains unchanged.

The Witch Doctor and Necromancer packages, Witch Hunter/Ranger follow-up and Manastorm solo/cache
changes are installed. Earlier Guardian, Barbarian, Witch Hunter and client fixes remain.
Report: `C:/Ascension/runtime/releases/necromancer-completion-20260909/implemented.md`.
Current receipt: `validation/deployment-current.json` under that release.

World SHA-256: `66CE233F53DEE19FFD55909BC4D6FBF6AB617FA3EAA10A4C81ACC1E8D566E720`.
SourceDirectory: `C:/Ascension/runtime/releases/necromancer-completion-20260909/source`.
Fresh VS2022 RelWithDebInfo build; final startup `2026-09-09T06:07:54.3965070+03:00`. Ports 8085/3443,
exact SQL and personal-data checks pass. New unique startup errors: zero. Auth/MySQL were not restarted.

Normal updater applied Witch Doctor 00, Necromancer 01, Necromancer runtime 02 and character cache
delivery 02. All are now immutable. The report records exact hashes and bounded compile/data fixes,
including local appearances for the three champions whose recovered display references were absent.
Use later migrations for further corrections; never rewrite these applied generator outputs.

Three model DBCs and the one-member Manastorm client B delta are installed. B is
`A5AA56895AE8A19B602461BE350B2ED3F2D59F36684C4FD7A230682DC7A100CF`; T and server Spell.dbc are unchanged.
The copied client started at `2026-09-09T06:16:20.9738231+03:00` with fresh stock-auth/addon/lighting/catalog proof.
No automatic login, cache reset, original-client change or progress rollback occurred.

One backup protects all 116 character/22 auth tables, 12 scoped world tables and 703 WTF files.
The 151-table audit preserves personal data and separates reviewed native daily/respawn state changes.
Gameplay acceptance remains distinct from deployment. Class policies remain in
`necromancer-completion.md`, `witch-doctor-completion.md`, `witch-hunter-ranger-followup.md` and `manastorm.md`.
The earlier class release is historical: `runtime/releases/class-completion-20260908/implemented.md`.
