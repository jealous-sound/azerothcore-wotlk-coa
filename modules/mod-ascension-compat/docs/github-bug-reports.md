# In-game reports to GitHub

The existing Ascension Help UI submits local-realm reports to
`jealous-sound/azerothcore-wotlk-coa`. The adapter applies only to the exact realm
`AzerothCore`. Other realms retain the original submit, events and link actions.

Status: implemented in source, disabled by default, not installed. No live GitHub
issue, worldserver build, game launch or player-data change was used to test it.

## Player experience

Open Help → Bug Report and fill in the existing category, severity, title,
description, expected/actual results and reproduction fields. Press **Send to GitHub**.
The form names the repository and explains that repository readers can see reports.
The original public/private checkbox is hidden locally: GitHub repository visibility
determines access, and this feature does not offer private issues in a public repository.

The server acknowledges a saved report first. The form displays **GitHub issue #N
created** and a copyable GitHub URL only after the relay confirms creation. A queue
acknowledgement, timeout or ambiguous response is not displayed as success.

The payload includes all category fields (including spell/item identifiers where
the selected layout provides them), selected severity and client version. The server
adds class ID, level, map, coordinates and core Git revision. It does not automatically
publish account IDs, character names, IP addresses, chat logs, credentials or saved settings.
The core hash does not describe uncommitted source changes; retain the deployment receipt
when diagnosing a locally modified build. Text explicitly typed into the form is submitted.

One outstanding report is kept in `CoABugReportDB`, a normal account SavedVariables
table. A normal logout/reload saves this draft; a client crash before that save can lose
an unacknowledged draft. A completed upload is already on the server and does not depend
on the client remaining open. Reopening the form resumes/checks the same request ID.

## Components

The client adapter is supplied separately in the matching client overlay. Its
archive member is
`Interface/AddOns/Ascension_HelpUI/BugReport/LocalGitHubBugReport.lua`.
`Ascension_HelpUI.toc` declares its SavedVariables and loads it after the original
frames.

Server and relay paths below are relative to `modules/mod-ascension-compat/`:

- `src/CoABugReport.cpp`: authenticated self-whisper receiver registered by `MP_loader.cpp`.
- `src/CoABugReportService.h`: bounded upload protocol, persistent queue and status lookup.
- `apps/bugreport/relay.py`: a separate Python 3.10+ process using only the standard library.
- `conf/coa_bugreport.conf.dist`: disabled startup configuration.

Messages use the `COABUG` addon prefix, 180-byte chunks and stop-and-wait acknowledgements.
The full message fits the core's 255-byte limit. Chunk boundaries may split UTF-8;
the receiver reassembles bytes before the relay validates UTF-8. Reports are limited
to 12,000 bytes, including a title of 3–200 bytes. Idle uploads expire after 120 seconds;
there are at most 128 partial uploads. The receiver limits protocol requests to 24 per
second per authenticated account and defaults to one submitted report per 120 seconds.
Account limits survive relogging but reset on worldserver restart.

The complete request is written to a temporary file, flushed/closed, then renamed to
`<account>-<request>.report`. The relay ignores partial files. These are private service
files: their filenames contain account IDs, which are not included in GitHub issues.
The game server performs no HTTP calls, holds no GitHub credential and creates no new
game database tables. The small amount of local file I/O happens in the world-thread
chat hook; keep the spool on a local disk, not a network share.

## Activation after an explicitly authorized build

1. Build the matching server sources with the module's loader entry and C++
   receiver, preserving the other class implementations. Install the matching
   server and client overlay together.
2. Create a private local spool directory outside the source checkout, accessible
   only to the worldserver and relay service identities. Grant those identities
   read/write access; do not expose the directory through a web server or
   distribute it to players.
3. Install `coa_bugreport.conf` from the dist template into the server's module config
   directory. Set `CoABugReport.Enable = 1` and `CoABugReport.SpoolDirectory` to the
   absolute directory above. These settings are read at startup, not on config reload.
4. Configure `COA_BUGREPORT_GITHUB_TOKEN` in the relay process's environment using the
   server's secret storage. Do not put it in Lua, MPQs, Git, launch arguments or chat.
   Use a repository-scoped fine-grained token with **Issues: read and write**, or a
   GitHub App installation token with the corresponding access to this repository.
   Enable Issues on the repository. Reports appear under the token's user/bot identity;
   players do not need GitHub accounts. Token refresh/rotation is the operator's responsibility.
5. Start the worker from the repository root, preferably supervised by the
   server service manager. Replace `<absolute-spool-directory>` with the same
   absolute path configured in `CoABugReport.SpoolDirectory`. This command
   enables real issue creation:

   ```powershell
   $bugReportModule = "modules/mod-ascension-compat"
   $bugReportSpool = "<absolute-spool-directory>"
   python -B "$bugReportModule/apps/bugreport/relay.py" --spool $bugReportSpool --send
   ```

   Without `--send`, the command only validates current `.report` files, then exits;
   it does not read the token, contact GitHub or write a journal. `--once --send` processes
   one pass, respecting any persisted delay; it does not drain a rate-limited queue.
6. Package the matching TOC and adapter into a new overlay on the **current**
   B archive. Verify changed member hashes and preservation of the realm-card
   and character-creation changes. Synchronize with the client closed.
   Client test launches require explicit authorization.

The repository destination is fixed in the adapter and relay. Changing it requires a
coordinated source/configuration review; a player cannot choose an API URL or repository.
Existing draft SavedVariables are created by normal client use, not by the installer.

GitHub's [create-issue endpoint and token permissions](https://docs.github.com/en/rest/issues/issues#create-an-issue)
define the API contract. The relay sends title/body only: no player-controlled assignees,
labels or issue-management actions. `@` mentions are neutralized before publishing to avoid
notifying arbitrary users/teams. HTTP redirects are refused, and credentials are only
sent to the fixed `api.github.com` endpoint. The API version is `2026-03-10`.

## Delivery and recovery

The relay uses a SQLite journal with committed intent before each POST, atomic status
files and one OS-locked worker per spool. Ordinary duplicate submissions with the same
request ID return the original queued report or issue. This is request deduplication;
two independently written reports about the same bug can still create separate issues.

Successful issue creation stores the number before publishing the status file. After
a restart, a missing status file can be rebuilt from the journal without another POST.
GitHub outages do not block game threads. Rate limits and definite permission failures
pause delivery; the token/configuration can be repaired and the worker restarted.
The worker serializes creation at a minimum of five seconds between attempts and honors
GitHub's rate-limit retry/reset headers. See [GitHub rate limits](https://docs.github.com/en/rest/using-the-rest-api/rate-limits-for-the-rest-api).

GitHub does not provide a create-issue idempotency key. A timeout, 5xx response, malformed
success response or interrupted `posting` journal entry becomes `uncertain`. The relay
looks for the report's opaque marker in up to ten pages of recently updated issues,
including closed issues. Finding it recovers the number. **An unsuccessful lookup never
automatically authorizes another POST.** This avoids blind duplicate creation but can
leave delivery needing administrator review even when no issue was created.

For `uncertain` reports, inspect the journal's marker and the repository. If the issue
exists, allow reconciliation to find it; if necessary an administrator can repair the
journal/receipt after checking the exact report. No automatic force-resend command is
provided. A generic `failed` status means the request was invalid or GitHub rejected its
content. `blocked` means credentials, repository access or configuration need attention.
Detailed API response bodies and tokens are never returned to the addon.

Keep `.report`, `.status` and `relay.sqlite3` together and back them up. Do not delete the
journal to clear an error: that discards duplicate protection. Completed records are retained;
automatic retention/archival and cross-machine failover are outside this implementation.
Atomic publication covers normal process crashes/restarts; abrupt disk or power failure is
not a claim of transactional durability across the game server, filesystem and GitHub.

## Verification

Recorded offline validation exercised the real Lua 5.1 adapter, a small
executable using the actual protocol header, and mocked GitHub delivery.
The combined checks used a separate harness with MSVC x64, Python and `lupa`;
they did not launch worldserver or the game.

Standalone relay tests need only Python. Run from the repository root:

```powershell
python -B modules/mod-ascension-compat/tests/bugreport/test_relay.py
```

Actual-source MSVC `/Zs` checks covered `CoABugReport.cpp` and `MP_loader.cpp`
against native project headers. Earlier failed validation phases remain distinct
from successful checks. These establish offline behavior and source syntax,
not an installed server build, rendered UI, live packet delivery or a successful
authenticated GitHub API request.

Live acceptance must exercise the normal Help entry point, all category fields, a normal
player account, successful creation/link copy, close/reopen during delivery and normal
relog. Confirm the issue in the target repository. Deliberate outage tests should use a
controlled worker fixture or stopped worker, and preserve the request/journal for recovery.
