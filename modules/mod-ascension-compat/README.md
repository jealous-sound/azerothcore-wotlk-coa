# Ascension / CoA compatibility module

This module is the local compatibility boundary between AzerothCore and the
copied Ascension 3.3.5a client. It includes the protocol adapters, account-wide
collections, character advancement, custom classes/resources and the targeted
Barbarian, Guardian and Ranger corrections documented by the workspace.
It is an independent reconstruction: source presence is not proof of complete
official gameplay parity. The canceled Retail/native-class experiment is absent.

The module is included directly in `azerothcore-wotlk-coa`; a normal clone has
its source without a submodule initialization step. Its previous Git history is
retained in the combined repository's ancestry. Keep its license and upstream
attribution. Runtime configuration is supplied by
`conf/mod_ascension_compat.conf.dist`, not by committing live local credentials.

Canonical client files, generated candidates, runtime databases and deployment
evidence remain in sibling workspace directories and are not part of this
server repository. Use the corresponding generators in the workspace `tools`
directory, preserve applied SQL hashes, and follow
`.agents/docs/systems/ascension-local-deployment.md` in the server checkout.
Do not run historical installers or clear client caches to compensate for a
protocol, data or process problem.

## World database installation

Import the [world database package](../../apps/coa-world/README.md) into an empty
world schema before the first worldserver startup. The same guide covers auditing
an existing database and updating the package.

## Login and natural regeneration

The copied client's `Extensions.dll` patches the ping timer at executable address
`0x632DE5` from -30000 to -5000 milliseconds (DLL write at `0x10A689AC`). The inspected
DLL SHA-256 is `f7b713095aab17a1e376f487290d4b7c4c18931635e4d91136d76db2592be8fa`.
Stock AzerothCore counts pings less than 27 seconds apart as overspeed; ordinary
accounts are disconnected after exceeding `MaxOverspeedPings`, while GM permission
23 bypasses that check. Local connections with `AscensionCompat.Enable = 1` accept
the five-second cadence with a one-second jitter margin. Faster sustained flooding
still reaches the strike limit. Other connections retain the stock limit.

The `gtOCTRegenHP`, `gtRegenHPPerSpt` and `gtRegenMPPerSpt` client files each contain
3,200 single-float rows indexed by class and level. Their SQL tables contain explicit
IDs, with the stock dataset covering only IDs 0–1099. The DBC loader accepts implicit
row IDs for these game tables while preserving explicit-ID files and SQL overrides.
This lets the existing regeneration formulas reach the supplied custom-class rows;
focus and energy use separate formulas.

Run `python -B modules/mod-ascension-compat/tests/client_compat/run.py` with a C++20
compiler available. `--dbc-dir <server-data/dbc>` also compares every installed
regeneration coefficient with the native loader's result. The tests compile the
actual storage/loader and ping/regeneration methods, using isolated session, clock
and database boundaries. They do not launch the server or client. Rebuilding and
deploying the corrected server is required before checking real logins and gameplay.
