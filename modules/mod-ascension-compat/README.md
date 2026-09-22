# Historical CoA migrations

CoA is built into the server from `src/server/coa/`. See the
[server component documentation](../../docs/coa/README.md).

This directory retains historical SQL at its existing paths. The worldserver
database loader always includes these migrations, even when optional modules are disabled. It has no source
loader, build configuration or runtime configuration. Do not edit applied SQL;
new migrations belong in `data/sql/updates/pending_db_*/`.
