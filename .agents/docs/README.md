# .agents/docs

Task references routed from AGENTS.md. Read only relevant sections and keep guidance compact.

## Taxonomy

- Root — task-type docs (`build.md`, `cpp-guidelines.md`, `sql-guidelines.md`, …), kebab-case.
  `<lang>-<topic>.md` is reserved for language-scoped specializations (e.g. `cpp-scripts.md`).
- `systems/` — cross-language subsystem docs, plain kebab-case subsystem names
  (e.g. `battleground.md`).

## Placing new guidance

- The most specific applicable doc wins.
- Generic language lesson → the language doc (e.g. generic C++ → `cpp-guidelines.md`).
- Subsystem-specific guidance → the relevant document in `systems/`.
- Extend an existing doc before creating a new one.
- Create a document only when requested or when substantial reusable guidance needs a home.
- Add routing only when needed for discovery; do not make every new lesson mandatory reading.
- Keep task histories, hashes, process IDs, failed phases, and acceptance receipts in existing task/release records.
  Ordinary changes need no new document or report. Do not automatically suggest skills or lesson capture.
