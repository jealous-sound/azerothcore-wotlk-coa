# Private repository organization — 2026-09-09

The core and vendored compatibility module remain in `jealous-sound/azerothcore-wotlk-coa`.
The private companion `jealous-sound/coa-local-kit` owns new client package tooling and preserves a
redacted snapshot of the historical workspace generators, policies, tests and operational scripts.
Its provenance manifest records original/exported hashes and redaction categories.

Operational workspace files were not moved, so installed binaries/configuration and their frozen source paths
remain valid. Client paths in the new kit target `ascension-live` directly; the surrounding Launcher is not a dependency.
No local server build, migration, restart, client launch or live client replacement occurs as part of this organization.

The inherited upstream CI and issue templates are kept under `.github/upstream-*` for reference.
Active workflows target this fork's `main` and perform source checks without configuring/building the server.
Both repositories are private; no existing private history is force-rewritten and no public copy is created.

Future work is tracked in the kit roadmap: complete client delta inventory, reproducible MPQ/DBC recipes,
portable process helper, full-profile recovery, input-source provenance and real clean-install acceptance.
Prepared-file Apply/Restore is implemented and tested independently; that does not certify the complete game package.

All pre-existing core C++ and SQL bytes are preserved by this metadata change. In particular, the applied September 9
SQL00–10 and earlier world/character migrations remain immutable. Do not rerun historical generators against them.
