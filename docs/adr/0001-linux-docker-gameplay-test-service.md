# 0001. Run gameplay tests on Linux through a Docker Compose service

- Status: accepted
- Date: 2026-09-15

## Context

The CoA gameplay test runner (`apps/coa-gameplay-test/run.py`) was written and verified on Windows. A Linux spike
against the Docker installation passed the Frostbolt scenario only with three workarounds: module configs staged in
the Windows-only `configs/modules/` location (Linux worldservers read `CONF_DIR/modules/`, fixed at build time), a
relative `DataDir` resolved against the binary directory, and `AC_*` environment variables from the image replacing
generated harness values. The harness also requires a loopback MySQL endpoint, while Compose services reach MySQL as
`ac-database`.

## Considered options

1. Docker recipe with manual workarounds (`docker run` flags, copied and edited configs).
2. Fix the runner and add a fork-owned Compose service sharing the database network namespace.
3. Change the core so module configs are read beside the `-c` config file.
4. Relax the harness's local-database check to accept Compose host names.

## Decision

Option 2. The runner no longer lets inherited `AC_*` variables replace generated values, reads source settings with
the server's environment precedence and stages module configs into an explicit server directory that is never
overwritten. `apps/coa-gameplay-test/docker/compose.yml` adds `ac-gameplay-test` (profile `tests`), built from the
local worldserver image plus Python, with `network_mode: service:ac-database` so MySQL is `127.0.0.1`.

Option 1 is not repeatable. Option 3 changes behavior for every Linux server and diverges from upstream AzerothCore.
Option 4 weakens a safety guard protecting real databases.

## Consequences

- Linux users run scenarios with one Compose command once a matching worldserver image with the runtime module
  and cache startup barrier is available. The wrapper adds no C++ changes.
- Upstream Docker files stay untouched; the service must be combined with the root `docker-compose.yml` via `-f`.
- The test image must be rebuilt after rebuilding the worldserver image.
- The service selects the same source schemas as the root Compose worldserver through the shared loopback
  endpoint. Custom schema overrides must be mirrored in the test service.
- Default world-cache reuse and explicit fresh/refresh modes match direct invocation. Metadata and leases persist
  in the writable results mount, not the read-only checkout. Inherited `AC_*` changes invalidate the cache.
- The service uses the MySQL root password already provided to the Compose stack. Credentials are written only to
  mode-600 files in a private temporary directory of the runner (inside the disposable container), never to the
  result directory. The runner removes its files during cleanup; the entrypoint's admin file disappears with the
  disposable container. SIGTERM triggers cleanup and the world-cache audit, with a 15-minute stop grace period.
  Clean cached world schemas are intentionally retained; hard termination or cleanup failure may also leave
  schemas or leases behind (check `summary.json` and the lease before recovery).
