# Local compatibility release — 2026-09-08

The Witch Doctor package completed on 2026-09-09 is **source ready, not installed**.
It addresses 119 source findings and includes a new world migration and three staged server DBCs.
See `witch-doctor-completion.md` and
`runtime/validation/witch-doctor-completion-20260908/implemented.md`.
Its guarded model/template definitions need absent-or-exact checks before a future coherent installation.
The independent Manastorm solo/cache and Witch Hunter/Ranger follow-ups are preserved.

The later Witch Hunter / Ranger follow-up is **source ready, not installed**.
It changes Advantage generation and Witch Hunter channel/pet/DoT handling.
See `witch-hunter-ranger-followup.md` and
`runtime/validation/witch-hunter-ranger-followup-20260908/implemented.md`.
The installed release described below remains the live deployment.

Guardian, Barbarian and Witch Hunter completion packages are installed together
with the existing Manastorm loop. The user explicitly requested the server build
and copied-client launch. The source checkout remains dirty over a98f3c59ce7ec708fce5160bbc1c68d9f5493384;
this deployment does not commit or publish that source.

The current worldserver uses `C:/Ascension/runtime/releases/class-completion-20260908/source`,
built in fresh `C:/Ascension/Build-ClassCompletion` with VS2022 RelWithDebInfo.
Its SHA-256 is `4CAD52F7814A599EFB1C6D32823E96BB73060186DC69649A91910459AF3FB5CA`.
Final startup was 20:40:43 Moscow; readiness on 8085/3443 and zero new unique
startup errors are verified. Auth and MySQL were not restarted.

## Applied data and startup corrections

| World migration | SHA-1 |
| --- | --- |
| rev_20260908_05_guardian_completion.sql | BDC84D48C514B1479972B8E66B1B22398779AABF |
| rev_20260908_06_barbarian_completion.sql | F540FCB4CDFD90F971B24769DF1C8EA6934C32AC |
| rev_20260908_07_witch_hunter_completion.sql | B490D871FD16A724680FCABA06461D85317B8B57 |
| rev_20260908_08_class_completion_runtime.sql | B20BD14B01A7B13561C3F6C2285D2F28D824DE0B |

These files were applied once through the normal updater and are now immutable.
The earlier Guardian 01/02, Manastorm 03/04 and all preceding applied migrations
remain unchanged. Before its first application, Witch Hunter 07 was corrected to
retain native shared model-info values rather than infer them from mesh geometry.
All 105 guarded creature/model/pet/gameobject definitions passed conflict checks.

The first linked build crashed while validating Barbarian's ability registration:
there is no cast object during Register. It now resolves metadata by m_scriptSpellId.
Slayer retains its native raid crit aura and separate personal bonus. Migration 08
clears unused proc masks and an unsupported hit bit on 48 exact rows; event flags,
chance, cooldown, charges and bindings remain. Native selection is equivalent over
140,400 tested cases. Failed artifacts and every correction remain in the report.

Two server model DBCs are updated. Server Spell.dbc is unchanged. Client B remains
`27375ECF21CFCBF370AAA4A977D38C2BFB27B34C5C7A553428343757A20C35D6`; T remains
`AA2679D59A300F8337ABBE2FC4913C39B45A042B350042EA15DEECEE8AEC228C`.
The successful copied-client launch at 20:47:20 has fresh stock-auth, addon,
lighting and 42,753-entry native catalog proof. No original client, on-disk
client executable/DLL or cache reset was involved. No automatic login occurred.

## Evidence and acceptance

Report: `C:/Ascension/runtime/releases/class-completion-20260908/implemented.md`.
Current machine-readable status: its `validation/deployment-current.json`.
One verified backup protects all 116 character and 22 auth tables, 11 scoped world
tables, relevant files and 682 WTF files. The final audit of 149 tables matches
intended SQL and preserves personal data; reviewed operational timer/realm deltas
mean whole-database equality is false. Do not rerun historical installers or restore
old source/database snapshots wholesale.

The class packages address their 93 Guardian, 73 Barbarian and 125 Witch Hunter
source findings. See [Guardian policy](guardian-completion.md), [Barbarian policy](barbarian-completion.md)
and [Witch Hunter policy](witch-hunter-completion.md). Deployment and startup checks
do not establish complete combat, pet, party, movement or replacement-UI acceptance.
[Manastorm behavior and balance](manastorm.md) continues to describe the installed
local game loop; official backend parity is not claimed.
