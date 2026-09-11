# AGENTS.md

AzerothCore is a C++ MMORPG server emulator for World of Warcraft 3.3.5a (WotLK), built with CMake, backed by MySQL.

## Agent rules

- **Do not configure or build unless explicitly asked.** Builds are slow and rarely needed for code changes.
- **Never edit SQL files outside `data/sql/updates/pending_db_*/` unless explicitly requested.** `data/sql/base/`, `data/sql/archive/`, and `data/sql/updates/db_*/` are immutable.
- Formatting follows `.editorconfig`: UTF-8, LF, max 120 cols, trailing newline, no trailing whitespace; 4-space indent for C++ (tabs forbidden), 2-space for JSON/YAML/sh/ts/js.
- Keep ordinary plans and results in the conversation. If a planning document is requested or necessary,
  use `.agents/plans/<task-slug>/` (gitignored); no per-task document is required.
- Use existing tools and the smallest relevant checks. Routine edits need no source snapshots, backup folders,
  receipts, or standalone reports. Preserve unique untracked work and use Git diffs for tracked files.
- A source-only task ends with the requested change and relevant checks. Builds, deployment, and in-game
  acceptance are separate scopes. Repeat passing checks only after changes or a specific unresolved concern.

## Task references

Read the relevant sections when needed for the work. Do not read every guide or turn examples into extra tasks.

- Authorized CMake configuration/build or native test setup → `.agents/docs/build.md`
- Writing or modifying C++ → `.agents/docs/cpp-guidelines.md`
  - Script work (under `src/server/scripts/`) → also `.agents/docs/cpp-scripts.md`
- Creating or modifying SQL → `.agents/docs/sql-guidelines.md`
  - SmartAI work (`smart_scripts` data) → also `.agents/docs/cpp-scripts.md`
- Reviewing a changeset or PR → `.agents/docs/code-review.md`
- Preparing an actual PR → `.agents/docs/self-review-rules.md`
- Subsystem-specific questions → the relevant section in `.agents/docs/systems/`
- Ascension damage/healing, AP/RAP/SP coefficients, triggered spells or tooltip parity →
  `.agents/docs/systems/ascension-spell-parity.md`
- Local SQL/binary/client deployment, MPQ synchronization or deployment-verifier tests →
  `.agents/docs/systems/ascension-local-deployment.md`
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

External modules live in `modules/`, each a subdir with its own `CMakeLists.txt`. Disable with `-DDISABLED_AC_MODULES="mod1;mod2"`. See `modules/how_to_make_a_module.md`.

In this private fork, `modules/mod-ascension-compat` is vendored into this repository, not a submodule
or a separate working tree. `origin` is the private CoA fork; `upstream` is the original AzerothCore
repository. Fetching upstream is separate from reviewing, merging, building or deploying its changes.

## Maintaining guidance

Keep only stable conventions and essential routing here. Update guidance when requested or when a durable
correction is needed; do not automatically offer lesson capture, install skills, or append task histories.
