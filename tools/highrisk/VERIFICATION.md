# Package verification

Date: 2026-09-17.

The original standalone extraction passed the following checks against its
original AzerothCore baseline. The module source and tests are unchanged here:

- The original core patch applied cleanly to its original reference commit.
- C++ syntax checks passed for the four module translation units and the four
  patched core translation units. The checks used the package's patched headers.
- 460,000 deterministic drop-roll outcomes and tier boundaries checked.
- PvP Power mappings, caps, scaling, healing contexts and overflow checks passed.
- Chest eligibility, equipment loss limits and gold calculation checks passed.
- 52 Lua checks passed for ruleset selection, persistence, context restrictions,
  XP source filtering, exclusions and the standalone XP callback.
- Four DBC-tool tests passed: original data preservation, conflicting existing
  records, missing base records and duplicate IDs.
- DBC dependency preflight passed against the preserved reference server data.
- C++ style checks passed. SQL formatting checks passed except the intentional
  INSERT/DELETE convention exception: upserts preserve existing template rows,
  and the database preflight requires collision review before import. The style
  checks were run offline against the packaged files only.
- Package contents were checked against an explicit feature allowlist, scanned
  for private deployment identifiers, and recorded in `MANIFEST.json`.

These checks did not install the module, start a game server or modify a live
database. They do not constitute a complete linked worldserver build or an
in-game acceptance test of the standalone extraction. Build and test on the
receiving realm as described in `README.md` and `TESTING.md`.

## Target fork adaptation

The patch context was rebased onto jealous-sound/azerothcore-wotlk-coa at
`77e98c4f853a1b24b0b6b313afa312344f09591d`. Its seven touched files were checked with
`git apply --check`; the patch preserves the fork's existing CoA changes.
The adapted patch has not been compiled or deployed on this fork. The full
installation and gameplay checks remain required on the receiving realm.
