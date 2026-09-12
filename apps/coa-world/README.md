# CoA world content

The CoA repack contains harvested world data that a stock AzerothCore database plus this
fork's historical migrations does not fully reproduce. `data/coa-world/baseline.json` pins
the reviewed world-content package used for new CoA installations. Its table fingerprints
also make differences in an existing installation visible without changing it.

This is the preserved local CoA baseline, not a claim of complete official-server parity.
It contains world templates and reference data only. It excludes the authentication and
character databases and three historical item-template backup tables. The migration ledger
is retained so already incorporated migrations are not replayed against their final state;
`updates_include` uses repository-relative paths.

## New source installations

Use Python 3.11 or newer and a MySQL 8 client/server. Create an empty world schema using the
normal database setup, then import the baseline **before the first worldserver startup**:

```sh
python apps/coa-world/world_data.py verify
python apps/coa-world/world_data.py bootstrap --defaults-file /path/to/world-client.cnf --database acore_world
```

Use `--mysql /path/to/mysql` if the client is not on PATH. The client option file contains the
connection details; do not put passwords in command arguments or commit the option file.
The database name may differ from `acore_world`. Point worldserver's `SourceDirectory` at the
checkout whose baseline you imported.

Bootstrap validates the entire package before writing, refuses any nonempty schema, imports
the migration ledger last, and compares every content table after import. It does not create
accounts or characters, run the game servers, or change client files. Continue with the normal
server setup afterward; subsequent tracked SQL migrations use the native updater.

The historical SQL includes committed CRLF files. Bootstrap verifies their LF-normalized
content against the baseline and binds covered updater hashes to the host's native text-mode
read. This avoids replaying an old migration merely because Windows and Unix read its line
endings differently. A real change to a covered migration is rejected, not relabelled as applied.

If import fails, the partially populated **new** schema is left for inspection. A retry refuses
that nonempty schema. Recreate only that disposable installation schema before retrying; never
use bootstrap to reset or upgrade an established server.

## Existing repacks and source installations

```sh
python apps/coa-world/world_data.py audit --defaults-file /path/to/world-client.cnf --database acore_world
```

Audit is read-only. It compares ordered, NULL-safe row fingerprints and reports missing tables,
column differences, changed content, and additional tables. A difference is a review input:
later migrations and intentional local customization can legitimately change the baseline.
It does not automatically overwrite those changes. Updater history and server-version metadata
are excluded from the content comparison.

For PR #18's items, the existing repack already has populated definitions. Its fractional weapon
damage values are retained in this baseline. Changing those values to the rounded values in a
different client cache is a separate gameplay change. The baseline also avoids creating the
appearance placeholders on new installations, so newly created equipment starts with the proper
durability. Existing copies from older placeholder-based installations require a separately
scoped upgrade that handles their saved durability; a world-template update alone does not.

## Maintaining the baseline

Make reviewed content changes through new `data/sql/updates/pending_db_world/` migrations.
Keep the versioned baseline stable between explicit baseline releases. When advancing it:

1. Apply the accepted migrations to an isolated clean world database and verify the intended
   content changes. Keep account and character data outside this workflow.
2. Produce a clean repack snapshot with the existing packaging tools and import its world
   section into an isolated MySQL schema.
3. Export a new baseline directory:

   ```sh
   python apps/coa-world/export_baseline.py --snapshot /path/to/databases.sql.gz \
     --defaults-file /path/to/isolated-client.cnf --database reference_world \
     --output /path/to/new-baseline --id coa-world-YYYYMMDD --repack-release RELEASE_ID
   ```

4. Verify and bootstrap the new package into another empty world schema, then run the normal
   SQL updater and audit the content again. Review the changed table fingerprints and migration
   identities before replacing the tracked package.
5. Build future repack world snapshots from that accepted baseline plus the tracked migrations.
   Do not introduce another untracked world-data import in the packaging step.

The exporter records the source snapshot checksum, repository revision, repack release, per-table
SQL checksums, and content fingerprints. ZIP timestamps are fixed for reproducibility. Both the
package and its manifest belong in the same reviewed change. Upstream attribution remains in
the repository's existing history and SQL migrations; the repack snapshot is the immediate
source for this preserved content.

Run the focused safety checks with:

```sh
python apps/coa-world/test_world_data.py
```

To compare a fresh repository SQL installation with the baseline and verify a complete
baseline import, run the isolated MySQL check:

```sh
python apps/coa-world/test_mysql.py --mysql-bin /path/to/mysql/bin --report /tmp/coa-world-validation.json
```

This creates temporary databases on a private socket/named pipe with TCP disabled. It checks
every content table, refuses a repeat import without changing data, checks current migration
hashes for unexpected replays, and compares the two starter items with their repack values.
It does not start worldserver or connect to an installed server.
