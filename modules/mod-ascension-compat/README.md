# Historical CoA migrations

CoA is built into the server from `src/server/coa/`. See the
[server component documentation](../../docs/coa/README.md).

This directory is not a module: nothing is built, loaded or configured from it.
It only keeps CoA's historical SQL migrations at the paths databases recorded when
they applied them. The database updater finds module SQL only under
`modules/<name>/data/sql/`, so the CoA build passes this directory name to the
worldserver and dbimport updaters (`COA_DATABASE_MODULE_LIST`); it is not listed
among enabled modules. Do not edit applied SQL; new migrations belong in
`data/sql/updates/pending_db_*/`.
