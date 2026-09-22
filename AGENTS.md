# AGENTS.md

AzerothCore is a C++ MMORPG server emulator for World of Warcraft 3.3.5a (WotLK), built with CMake, backed by MySQL.

## Agent rules

- Configure and build when needed to implement or verify the requested work; prefer focused incremental targets.
- **Never edit SQL files outside `data/sql/updates/pending_db_*/` unless explicitly requested.** `data/sql/base/`, `data/sql/archive/`, and `data/sql/updates/db_*/` are immutable.
- Formatting follows `.editorconfig`: UTF-8, LF, max 120 cols, trailing newline, no trailing whitespace; 4-space indent for C++ (tabs forbidden), 2-space for JSON/YAML/sh/ts/js.
- Keep ordinary plans and results in the conversation. If a planning document is requested or necessary,
  use `.agents/plans/<task-slug>/` (gitignored); no per-task document is required.
- Use existing tools and the smallest relevant checks. Routine edits need no source snapshots, backup folders,
  receipts, or standalone reports. Preserve unique untracked work and use Git diffs for tracked files.
- A source-only task ends with the requested change and relevant checks. Deployment and in-game acceptance
  are separate scopes. Repeat passing checks only after changes or a specific unresolved concern.
- In CoA-owned code, express intent through names, structure and tests; do not add explanatory comments or
  docstrings. Preserve legal notices, tool directives and test generator markers. Scope is
  `src/server/coa/`, `apps/coa-tests/`, `apps/coa-bugreport/`, `apps/coa-{dbc,gameplay-test,mechanics}/`,
  `tools/` and `.github/scripts/`.
  `tools/check_source.py` enforces this for C++ and Python; `tools/check_comments.py --all` audits the full scope.
- In inherited AzerothCore source, keep existing comments so upstream merges stay clean; change a comment only
  with the code it describes. Add comments there only for non-obvious external constraints or rationale that
  the code cannot express. Preserve legal attribution and machine-consumed directives.

## Investigating reported defects

Follow this sequence within the task's authorized scope:
**Report → inspect effective data → establish expected behavior → reproduce → minimal fix → relevant checks → PR.**

- Treat the report as evidence to investigate. For spells and quests, inspect effective DBC/SQL data, acquisition
  and the execution path using the inspector and mechanic map. Derive expectations independently of the current
  implementation and use a metric that can distinguish correct behavior from the reported failure.
- Before changing gameplay, classify the finding as **confirmed defect**, **already works**, **incorrect test**,
  or **uncertain expectation**. Correct a faulty test; preserve evidence of working behavior; keep unresolved
  expectations explicit. Apply a gameplay fix only to an established defect.
- Seek a focused reproduction and a regression that fails before the fix and passes after it. State when runtime
  reproduction is unavailable; explain the missing prerequisites and complete independent source checks.
  Use the combined scenario verification and scoped source checks where relevant. Report their actual scope.
- Review the final diff. Continue to a PR when publication is part of the requested workflow; a local edit alone
  does not authorize a push or PR. Use the issue-to-PR skill for an explicitly requested issue queue workflow.
- In existing task/PR results, summarize available time from investigation start to verified outcome, observed
  regressions, flaky checks (different outcomes with unchanged inputs), and required human corrections.
  Mark unavailable measurements as unknown; do not infer zero or create separate tracking files for routine work.

## Task references

Read the relevant sections when needed for the work. Do not read every guide or turn examples into extra tasks.

- CMake configuration/build or native test setup → `.agents/docs/build.md`
- Writing or modifying C++ → `.agents/docs/cpp-guidelines.md`
  - Script work (under `src/server/scripts/`) → also `.agents/docs/cpp-scripts.md`
- Creating or modifying SQL → `.agents/docs/sql-guidelines.md`
  - SmartAI work (`smart_scripts` data) → also `.agents/docs/cpp-scripts.md`
- Reviewing a changeset or PR → `.agents/docs/code-review.md`
- Preparing an actual PR → `.agents/docs/self-review-rules.md`
- Requested issue queue / issue-to-PR workflow → `.agents/skills/coa-fix-issues/SKILL.md`
- Subsystem-specific questions → the relevant section in `.agents/docs/systems/`
- Ascension damage/healing, AP/RAP/SP coefficients, triggered spells or tooltip parity →
  `.agents/docs/systems/ascension-spell-parity.md`
- DBC record retrieval, links, provenance or data comparison → `apps/coa-dbc/README.md` (`coa-dbc-viewer`, JSON CLI)
- Spell/quest ranks, acquisition, expected behavior, execution paths and regression links →
  `apps/coa-mechanics/README.md` (compact mechanic maps)
- Finding gameplay regression scenarios or checking recorded results → `apps/coa-gameplay-test/README.md`
- Scoped source checks (boundaries, registrations, tooling regressions) →
  `python -B tools/check_source.py --base <ref>`; add `--plan` to inspect selection without running checks.
- SQL change boundaries → `python tools/check_change_boundaries.py --base <ref>`; CI exceptions need a
  maintainer's `sql-change-authorized` PR label. A label does not replace the task's explicit SQL authorization.
- Ascension race/class availability or character creation → `.agents/docs/systems/ascension-character-creation.md`
- CoA talent points, ranks, specializations or the talent window's state → `.agents/docs/systems/ascension-talents.md`
- Capturing a lesson or adding/updating agent docs → `.agents/docs/README.md`

## Repository layout

- `src/common/` — networking (Asio), crypto, config, logging, shared utilities.
- `src/server/game/` — core gameplay; compiled into worldserver.
- `src/server/scripts/` — content scripts grouped by region (`EasternKingdoms/`, `Northrend/`, …), class (`Spells/spell_mage.cpp`, …), and domain (`Commands/`, `Pet/`, `OutdoorPvP/`, `World/`).
- `src/server/database/` — DB abstraction and schema updater.
- `src/server/shared/` — code shared by auth and world servers.
- `src/server/apps/{authserver,worldserver}/` — entry points (ports 3724 and 8085).
- `src/test/` — unit tests + mocks.
- `data/sql/` — `base/` (historical schema), `updates/db_*/` (merged), `updates/pending_db_*/` (in-flight), `custom/` (gitignored).
- `modules/` — external modules (see below).
- `apps/` — helper scripts; `apps/codestyle/` holds the lint scripts.
- `conf/dist/` — distributed config templates; `conf/*.conf` is gitignored.
- `deps/` — vendored third-party dependencies.

## Modules

External modules live in `modules/`, each with its sources in `src/`. Disable one with `-DMODULE_<NAME>=disabled`
(for example `-DMODULE_MOD_SPELLBOOK=disabled`). See `modules/how_to_make_a_module.md`.

In this fork, CoA is a required server component in `src/server/coa/`, with focused tests in
`apps/coa-tests/`. `modules/mod-ascension/data/sql/` keeps its historical SQL migrations only.
`origin` is the CoA fork; `upstream` is the original AzerothCore
repository. Fetching upstream is separate from reviewing, merging, building or deploying its changes.

## Maintaining guidance

Keep only stable conventions and essential routing here. Update guidance when requested or when a durable
correction is needed; do not automatically offer lesson capture, install skills, or append task histories.
