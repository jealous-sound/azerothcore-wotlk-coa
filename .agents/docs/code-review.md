# Code review

Review the requested changeset or PR and affected dependencies. Use relevant language/subsystem guidance.
Routine self-checks do not require a formal report, a clean working tree, or a committed review round.

- A posted review carries findings only: no summary, no praise. Nothing to report means one line
  saying so.
- A finding names what is wrong, why it matters and the fix, on the offending line. Findings are
  bugs, crashes, lifetime and memory errors, data-integrity problems, injection, and violations of
  a rule written down in AGENTS.md or `.agents/docs/`; taste no rule covers is not a finding.
- Run the affected language's linter on changed files with `--files <path> ...`:
  `python apps/codestyle/codestyle-cpp.py` or `python apps/codestyle/codestyle-sql.py`.
  Do not run unrelated linters or full-tree scans for a small diff. Documentation-only changes need no code lint.
- Title and description follow the [commit message guidelines](https://www.azerothcore.org/wiki/commit-message-guidelines).
- Prefer data over code: when a C++ or script change is also achievable through world DB data
  (SmartAI, conditions, templates), flag the DB-only alternative (see `cpp-scripts.md`).
- Never take a claim as fact, neither the PR description's nor a comment's. Verify game-data
  claims (spell/creature/quest ids, mechanics) against the world DB, DBC data, or cited sources;
  verify "fixed in the latest push" against the current diff.
- When a change touches control flow (early returns, new branches, removed guards), trace the
  states it can leave behind (null, empty, fall-through) into every consumer of them — including
  unchanged lines the new flow now reaches — and verify what runs when an acquire/attack/GetX
  call leaves a null result, not just what enables it.
- Check the change against the actual target branch (`main` in this fork): the surrounding code may have moved,
  or another change may have landed the same fix.
- On an existing PR, read unresolved discussions and resolved items affected by the current diff, including relevant
  bot findings. Fetch review bodies/inline threads when needed; use the actual PR repository, not a fixed upstream URL.
  A complete historical discussion audit is needed only when requested or relevant to an unresolved issue.
- Do not post reviews, comments, or change PR labels unless the user authorized that action.
