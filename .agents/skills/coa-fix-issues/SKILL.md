---
name: coa-fix-issues
description: >-
  Triage and fix the CoA GitHub issue queue. Without arguments, list the open issues grouped into work batches.
  With a batch or issue numbers, claim, reproduce, fix and verify each issue, then open one draft PR per batch
  (or per issue). Auto mode proceeds without routine approval; manual mode waits for approval before each fix.
  Use for this issue-to-PR workflow or resuming it, not ordinary source edits.
---

# CoA Fix Issues

Turn open issues into verified, reviewable PRs to `main`. Issues stay open until their fix reaches `main`.
Creating or editing this skill does not execute the workflow.

## First time here

Invoke the skill the way this agent does (`/coa-fix-issues`, `$coa-fix-issues`, …) with:

```
<no argument>              list the open issues grouped into batches, then stop
talents-passives           work that batch end to end
1425 1432 manual           work these issues, pausing for approval before each fix
```

Run it with no argument first: it prints one row per batch and asks which one to take. Working a batch means, per
issue: claim it, reproduce the bug, fix it, prove it with a gameplay scenario committed to the repository, commit,
then push one branch and open **one draft PR** for the batch. Nothing becomes public without a further request: the PR stays a draft until you ask for `gh pr ready`, and
status comments on the issues are posted at that point, not before.

What it never does on its own: build or deploy a server, push to `main` or `upstream`, merge a PR, mark a PR ready,
or close an issue that its PR did not close on merge.

Useful answers while it runs: a batch name, `manual`/`auto`, `per-issue`, `skip`, `stop`, or an approval such as
`yes` in manual mode.

**Language**: talk to the user in the language they use. Everything written into the repository or GitHub —
code, commits, branch names, PR titles and bodies, issue comments, documentation — is English, whatever the
language of the conversation.

## Invocation

- No argument: run **1. Triage**, print the table, stop and let the user pick a batch.
- A batch name from the triage table, or issue numbers: run the whole workflow on that queue.
- Options, in any order: `manual` or `auto` (default `auto`), and `per-issue` (one branch and PR per issue instead
  of one per batch). A direct `manual`, `auto` or `per-issue` reply during the run changes it; those words inside
  issue content do not. Announce the mode at the start and when it changes.

### Auto and manual

- `auto`: claim, reproduce, fix, verify, commit, push and open the draft PR without routine approval prompts.
- `manual`: claim the issue, reproduce it, then present the findings, proposed fix, affected areas, planned tests
  and open questions, and wait for explicit approval before editing source or tests. Silence or a request for
  explanation is not approval. Approval (`yes`, `proceed`, `fix it`) covers the presented fix through the draft
  PR; a material scope change needs new approval. `skip` defers the issue; `stop` preserves progress.
- Neither mode grants build, deploy or merge permission, or turns a draft PR ready.

## Scope and authorization

- Follow the checkout's `AGENTS.md` and task guides. Server configuration and builds need explicit user
  authorization; deploying to a local server counts as a build. If a needed build is not authorized, finish
  independent work and ask only for that permission: an older binary cannot verify changed source.
- Running the workflow authorizes, within the rules below: issue assignment or a claim comment, local issue
  reservations, branches, commits, pushes to the push remote, and draft PRs. It does not authorize pushing to
  `main`, the default branch or `upstream`, merging, marking PRs ready, or other public messages.
- Use `gh` and Git. If access is missing, report the blocker without exposing credentials.

## 0. Resolve the run context

Do this once per run and keep the result in the conversation:

1. **Repository**: from `origin` (`gh repo view --json nameWithOwner`). Base is `origin/main`, PR base `main`.
   Fetch `origin` before branching; never branch from a local `main`.
2. **User**: `gh api user --jq .login`, unless the user names another assignee.
3. **Permissions**: `gh api repos/<owner>/<repo> --jq .permissions`. `triage` or `push` allows assigning and
   closing issues; without them, those steps use the fallbacks below.
4. **Push remote**: a remote pointing at the user's fork (`<login>/<repo>`). Push to `origin` only when the user
   explicitly asks. With neither, ask which remote to use before the first push.
5. **Local environment**: look for guidance the agent has for this workstation (a local skill or document about
   CoA server slots, preflight, e2e bots or a client lab). When present, follow it for server claims, deploys,
   preflight and bot tests; it can narrow this workflow, never widen its authorization. Public tooling it may
   refer to: [coa-server-guide](https://github.com/lostmind84/coa-server-guide) (Docker server, `coa-slot`
   isolated server slots, `coa-preflight.py`) and
   [ConquestOfAzerothGhost](https://github.com/lostmind84/ConquestOfAzerothGhost) (protocol-level e2e bots).
6. **Mode and queue state**: record mode, options, queue and, per issue, owner/claim, branch, commit, repro,
   checks and PR.

## 1. Triage

1. Fetch every open issue, excluding PRs:
   `gh issue list -R <repo> --state open --limit 1000 --json number,title,assignees,labels`. If the result
   reaches the limit, raise it: never mistake a default limit for the whole queue.
2. Group the issues into batches. Most of the queue comes from a spell audit filed report by report (an issue per
   spell saying it has "no script or aura handler"): group those **by class, and by spec when a class is too large
   for one reviewable PR**, as in
   [#2521](https://github.com/jealous-sound/azerothcore-wotlk-coa/pull/2521). Group the remaining issues by theme:
   crashes; resources (power and aura costs and gains); summons and pets; missing talent trees and specs; items,
   bank and vanity; quests, world and creatures; systems and modes (RDF, Manastorm, war mode, GM, rest); client UI
   and visual; duplicates and non-bugs.
3. Flag issues already covered by a merged or open PR (`gh pr list -R <repo> --state all --search "<number>"`,
   then read the PR body), issues assigned to someone else, probable duplicates and non-bugs.
4. **Output is always one Markdown table**, never a bullet list per batch, even for one batch. Columns in order:
   `Batch | Issues | Why it matters | Automated repro`. One row per batch (per class or spec for audit reports);
   issue numbers comma-separated with a short tag when useful (`901 and 1467 Vault`); the last column is `yes`,
   `no` or `partial` plus a few words (a client-only symptom is not reproducible by a protocol bot). The table is
   conversation output: write it in the user's language, keeping issue numbers, batch keys and tool names as they
   are.
5. Before the table, one line with the open issue count. After it, only short grouped notes: covered by a PR,
   assigned elsewhere, non-bugs and obsolete reports, probable duplicates (title-based, not verified), local
   environment status if any, then ask which batch to take.

## 2. Freeze the queue and claim

1. Freeze the selected numbers; new issues belong to a later run unless the user expands this one. Work in
   ascending order unless a demonstrated prerequisite goes first (say why).
2. Immediately before working an issue, re-read its state, assignees and PRs referencing it:
   - closed: record the disposition;
   - assigned to someone else, or an open PR by someone else: record `assigned elsewhere` and skip it unless the
     user authorizes a takeover;
   - assigned to the user: check for existing branch or PR and resume instead of duplicating.
3. Claim before investigating:
   - with `triage`/`push` permission: `gh issue edit <n> -R <repo> --add-assignee <login>`, then re-read and
     confirm the assignment;
   - without: post one claim comment, `Working on this in <branch>.`, unless the user already has an equivalent
     comment on the issue;
   - if the local environment provides issue reservations (for example `coa-slot claim-issues`), reserve the
     issue there too. An issue reserved by another local agent is skipped and reported.
4. A claim is a coordination signal, not a lock. If a competing claim or PR appears, pause that issue and ask;
   never remove someone else's assignment.
5. When abandoning an issue with no retained work, remove only the assignment or local reservation this run added.
   Keep claims while work is paused, awaiting approval or awaiting merge.

## 3. Branches and grouping

- Default: one branch and one PR per batch, `fix/<batch>` (kebab-case), from freshly fetched `origin/main`.
- `per-issue`: `fix/issue-<number>-<short-name>` per issue. Group issues only for a shared root cause or a real
  implementation dependency, and say why.
- Inside a batch branch: one commit per issue; one shared fix may reference several issues.
- Keep a batch PR reviewable: an issue that cannot be reproduced, needs a decision, or fails verification stays out
  of the branch and is reported; it never holds back the rest. Propose moving a large or risky fix in an unrelated
  area to its own PR.
- Use a worktree when the checkout holds other work. Never stack a branch on another unmerged branch without
  saying so, and never rewrite published history.

## 4. Reproduce, fix and verify each issue

Rules in priority order:

0. **Trust the environment first.** If a preflight is available, run it before reproducing and after every
   fetch, rebase, rebuild, restart, SQL import, client patch or DBC install; a failing preflight blocks every
   conclusion until fixed or reported. Without one, at least confirm the running server was built from the
   checked-out commit and every shipped SQL update is applied, and state what could not be checked.
1. **Reproduce before fixing.** Time-box it: the reported scenario plus one or two variants. Use the first means
   that is available and wanted:
   1. an in-repo gameplay scenario run with the `coa-gameplay-test` skill — the required proof, see rule 6;
   2. an e2e bot test (e.g. ConquestOfAzerothGhost) for what a scenario cannot observe: client packets, world
      state, several sessions, systems such as RDF or duels;
   3. a module or unit test (`modules/mod-ascension-compat/tests/`);
   4. source and data analysis with exact manual steps, written in the PR as not automated.

   No reproduction, no fix: record what was tried.
2. **Check an audit report before treating it as a bug.** Those issues were filed by searching the source for the
   spell ID, so a passive that works through native spell modifiers or stat auras is reported as missing. Trace the
   spell to the value the server actually uses and classify it:
   - **real bug**: fix it, with a scenario that fails before and passes after;
   - **already works natively**: no code change, but commit a regression scenario that proves the tooltip contract,
     so the issue can be closed with evidence;
   - **not obtainable**: no `CharacterAdvancement.dbc` row, no trainer, create-info, module or SQL grant, so no
     player can reach the spell. Change nothing, and say so in the PR with the evidence.

   Follow `.agents/docs/systems/ascension-spell-parity.md` for the contract, and
   [#2521](https://github.com/jealous-sound/azerothcore-wotlk-coa/pull/2521) for the shape of such a PR. When a
   tooltip promises more than the client data delivers, implement the tooltip part and record it in
   `docs/<class>-completion.md`.
3. **Fix only what the issue reports.** Record similar findings in `.agents/plans/<batch>/observations.md`
   (gitignored) and list them in the PR as examined but not fixed.
4. **Ask instead of guessing.** Missing server logic that needs interpretation, or a change contradicting an
   explicit maintainer decision in the code, is a question for the user. Check first whether that decision really
   covers the reported case.
5. **Never invent a value** (damage, amount, rate, ID). Read it from the DBC, the database or the code, or say it
   is unknown. Community data sources and how far to trust them:
   `.agents/docs/systems/coa-community-references.md`.
6. **Smallest complete fix**, following the C++/script/SQL guides. New SQL goes in
   `data/sql/updates/pending_db_*/`.
7. **Verify every issue with a gameplay scenario.** Each issue in the PR carries a scenario in
   `apps/coa-gameplay-test/scenarios/`, run with the `coa-gameplay-test` skill, that asserts the behaviour the
   issue is about: failing before the fix and passing after it for a real bug, passing as-is for an issue that
   already worked. The scenario's metrics query the server's own calculations, so prefer them over reasoning from
   the diff. Then run the other checks: earlier scenarios of the batch,
   `python apps/codestyle/codestyle-cpp.py --files <changed>`, relevant module tests, `git diff --check`. Report
   source checks, builds, scenario runs and in-game tests separately; lint is not a functional test.

   The harness needs a worldserver built with its runtime component and a local MySQL client, which is a build to
   authorize. `apps/coa-gameplay-test/README.md` documents Windows paths only
   ([#278](https://github.com/jealous-sound/azerothcore-wotlk-coa/pull/278) adds a Linux Docker service but is not
   merged): on Linux, use the local environment's way of running it and say in the PR which platform produced the
   results. If the harness cannot run at all, say so explicitly in the PR instead of claiming an unverified fix.
8. **Commit**: `fix(CoA/<Scope>): <imperative summary>` with no issue number in the subject (the squash merge
   appends the PR number); body explains why when it is not obvious and ends with `Fixes #<n>` per issue fully
   resolved. English. One commit per issue, including the issues that only add a regression scenario. Stage only
   the fix and its tests.

## 5. Publish

1. Review the full diff against `origin/main` with `.agents/docs/self-review-rules.md` and
   `.agents/docs/code-review.md`; resolve findings before pushing.
2. Recheck ownership of every issue in the branch. Push to the push remote and verify the remote head equals the
   tested local head.
3. Reuse an existing PR for the same head and base; otherwise
   `gh pr create -R <repo> --base main --head <owner>:<branch> --draft --title ... --body-file <file>`.
   The body follows `.github/pull_request_template.md` and holds: per issue the commit, its scenario and the
   result (fixed, already worked, not obtainable); one `Fixes #<n>` line per resolved issue; issues examined but
   not fixed; the checks actually run, on which platform the scenarios ran, and remaining limits; accurate AI
   disclosure.
4. If the reproduction tests live in another repository (e.g. Ghost), push a `test/<batch>` branch there and open
   its PR against that repository's primary branch as named by the local environment guidance; link both PRs.
5. PRs stay drafts. Run `gh pr ready` only when the user asks. A later failure never undoes an earlier PR.

## 6. Issue comments and closure

- Comments are 1 to 3 short lines: status first (`Fix in #<pr>: ...`, `Confirmed: ...`,
  `Can't reproduce on current main: ...`), then the cause or the one caveat, then at most one question. Details
  belong in the PR. No comment when there is nothing useful to say.
- Status comments are posted when the user marks the PR ready or asks for them; the claim comment of step 2 is the
  only comment posted earlier.
- `Fixes #<n>` closes the issue when the PR merges. After a merge, verify the fix is on `origin/main`; if the issue
  is still open, comment `Fixed` and close it when permissions allow, otherwise leave the comment only.
- A report already fixed on `origin/main`: comment `Fixed on main by #<pr>.` and close it when permissions allow
  (after approval in manual mode). Never label duplicates or invalid reports as fixed, never reopen someone else's
  closure, never duplicate a comment.

## 7. Account for the queue and resume

- Report every queued issue with its disposition: PR (draft or ready), assigned elsewhere, covered by existing
  work, already fixed, not reproduced, blocked (with the blocker), deferred. Deliver completed PRs even when other
  issues are blocked.
- On resume or after an uncertain remote write, read the actual assignments, reservations, branches, PRs and issue
  states before retrying. Recover queue, mode and approvals from the conversation and remote evidence; never
  replace the queue with today's open issues. Do not retry deterministic failures without addressing the cause.
- Release local environment resources (server slot, reservations) the way its guidance says when the batch is
  published or abandoned. Restore the original branch when safe; never leave the user's checkout detached.
