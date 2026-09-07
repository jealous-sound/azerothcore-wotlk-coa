# Conquest of AzerothCore

Private AzerothCore 3.3.5a server source with the local Ascension / CoA
compatibility implementation. This is an independent reconstruction, not the
official Ascension backend. The unfinished Retail / native-class experiment
was canceled and is not part of this working version.

## Repository layout

- `src/`: AzerothCore with the existing local protocol and gameplay corrections.
- `modules/mod-ascension-compat/`: included CoA module source, not a submodule.
- `data/sql/updates/pending_db_world/`: local generated migrations; keep their
  applied hashes intact and use the normal database updater.
- `.agents/docs/systems/`: maintained compatibility/deployment guidance.

The executable, live databases, credentials, copied client, DBC/MPQ assets,
toolchain and local backups are deliberately outside this Git repository.
In the local workspace this checkout is `C:/Ascension/azerothcore-wotlk-coa`;
the installed server remains under the sibling `runtime/server` directory.
Moving the source repository does not deploy or rebuild the running server.

## Git remotes

- `origin`: `git@github.com:jealous-sound/azerothcore-wotlk-coa.git` (private).
- `upstream`: `https://github.com/azerothcore/azerothcore-wotlk.git`.

Fetch upstream changes without changing the local CoA code:

```sh
git fetch upstream
git log --oneline main..upstream/master
```

Review and test any later merge separately. Do not automatically reset or rebase
the CoA branch onto upstream. The initial migration preserves the original
AzerothCore ancestry, the compatibility-module history and the private
repository's initial commit. Read `AGENTS.md` before changes; builds and database
deployments require their own explicit task authorization.
