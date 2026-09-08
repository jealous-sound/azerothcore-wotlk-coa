# Local compatibility release — 2026-09-08

The implemented class contracts and the earlier Manastorm pilot were committed and pushed in
`6fb63aefc`. The subsequent Manastorm implementation is now built and installed locally, together
with the Guardian follow-up source and its two migrations. Complete official class/backend parity
is not asserted. [Manastorm behavior and balance](manastorm.md) describes the actual supported game loop.

## Installed state

The current worldserver uses the frozen source at
`C:/Ascension/runtime/releases/manastorm-complete-20260908/source`, built with VS 2022 in the fresh
`C:/Ascension/Build-ManastormComplete` directory, RelWithDebInfo. Its SHA-256 is
`E7E3A1C932C61CF1F970B069D880A8A42712399EDD455BD5F8E3D6E5D747BF71`.
The final process started at 09:23:33 Moscow and reached readiness on ports 8085/3443.
Auth and MySQL were not restarted. The canceled Retail build is unrelated and remains archived.

The normal updater applied these five migrations, with their recorded SHA-1 values:

| Database / migration | SHA-1 |
| --- | --- |
| characters / rev_20260908_01_manastorm_progression.sql | B64CF2B1122B1479BA1E37AE38FFAB151DE30347 |
| world / rev_20260908_01_guardian_coefficients.sql | EF2802BD605EE38F94C7E441E89D9D517DFD7959 |
| world / rev_20260908_02_guardian_favor.sql | 95F9521CAF83DA656870194407CD268684A697D9 |
| world / rev_20260908_03_manastorm_rewards.sql | C6A895AB80846726B2A254A5A9FF324E1309E162 |
| world / rev_20260908_04_manastorm_reward_tiers.sql | 7FBEEBEB62D794B0141ACF4B27B3E3AA25323755 |

The reward-tier follow-up only replaces three cache loot pools already covered by the original backup.
It adds suitable level-61–79 leveling equipment and level-80 endgame gear. It was applied by a second
normal updater restart; the already applied reward migration was not edited. The prior 38 class world
migrations and the pilot character migration remain intact. Do not rerun historical installers.

Manastorm supports levels 10–80, solo and parties of up to five, eight separately persisted modes,
native checkpoints through depth 16,384, real floor rewards, XP, caches, currencies, loadouts and
Cogsley services. Runtime geometry admits 30 of 61 candidate scenes, including one opening room.
Only three implemented affixes rotate. The chance/scaling policy is explicitly local; exact official
rates were not recovered from the cited publications. Unfinished class mechanics remain separate.

The copied client started through the guarded launcher at 09:32:53 Moscow. Its fresh proof confirms
stock auth, addon permission, Wardrobe lighting, 42,753 initialized native appearance entries, and
the installed patch-B SHA-256
`27375ECF21CFCBF370AAA4A977D38C2BFB27B34C5C7A553428343757A20C35D6`.
Four Lua members changed and 1,398 unrelated content members were preserved. Patch-T remains
`AA2679D59A300F8337ABBE2FC4913C39B45A042B350042EA15DEECEE8AEC228C`.
The earlier Ranger secondary-mana/Advantage and trusted scenario saved-position fixes are retained.
Client overlays and MPQs live outside this core repository. No original-client, cache, on-disk
EXE/DLL or server Spell.dbc change belongs to this package.

## Validation and limits

One verified backup at
`C:/Ascension/runtime/server/backups/before-manastorm-complete-20260908-090540-219076` protects all
113 character and 22 auth tables, eight reviewed world tables with scoped rows for large templates,
682 WTF files and relevant binaries/configs/archives. This is not a full world-database dump.
The final live audit checked 146 tables, preserved personal data, matched the exact migration plan,
and found zero new unique startup errors. Normal realm/uptime, outdoor Northrend respawn and
Wintergrasp timer changes are recorded; strict whole-database equality is false.

The linked build and eight focused tests pass. They exercise production reward/queue/loadout methods
against transaction doubles, native packet/checkpoint contracts, mapless ownership, movement scope
and Lua 5.1 behavior. The standalone repository harness is in `tests/manastorm/run.py`.
Native C++ codestyle and diff whitespace pass. The reward-tier migration passes all eight SQL checks.
The original reward migration retains one reviewed SQL-linter exception: its guarded insertion of a
missing gameobject template has no destructive preceding DELETE. Historical global SQL lint is not
reported as clean.

No automated game login or combat control was used. Fresh in-game combat, cache opening and group
acceptance are still manual checks; a linked build and geometric scene validation do not prove every
boss or gadget in combat. No official-backend parity claim follows from this release.

Phase evidence and effective source hashes are in
`C:/Ascension/runtime/releases/manastorm-complete-20260908`; the final outcome is recorded at
`C:/Ascension/runtime/validation/manastorm-complete-20260908/implemented.md`.
Historical class/pilot/Guardian evidence remains in its original dated directories. Player data,
credentials, generated builds, client binaries and private runtime databases are not in this repository.
