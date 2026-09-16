# CoA client DBC set

The worldserver reads `DataDir/dbc` once at startup. It must hold the DBC set the players'
client loads: the launcher's archives plus the distributed client patch, taken in client load
order. A stock or partially copied set makes the core discard content that references rows only
the CoA client has, for example item limit categories, gameobject spawns and currencies.
Game files are never committed; build the set locally from a client installation.

Gameplay changes to client tables ship as client patch changes, so the server and the players'
clients read the same rows. `*_dbc` world tables must not replace rows the client already has.

## Requirements

Python 3.11 or newer. Extraction also needs [mpqcli](https://github.com/TheGrayDot/mpqcli)
(StormLib).

## Build and install

```sh
python apps/coa-dbc/client_dbc.py extract "C:/CoA/client/Data" out/client-dbc --mpqcli path/to/mpqcli.exe
python apps/coa-dbc/client_dbc.py check out/client-dbc
python apps/coa-dbc/client_dbc.py diff "C:/CoA/server/data/dbc" out/client-dbc
python apps/coa-dbc/client_dbc.py install out/client-dbc "C:/CoA/server/data" --dry-run
python apps/coa-dbc/client_dbc.py install out/client-dbc "C:/CoA/server/data"
```

`extract` reads `Data/*.MPQ` and `Data/<locale>/*.MPQ` in load order: base archives, locale
base archives, `patch.MPQ`, `patch-<digit>`, locale patches, then letter patches in
lexicographic order. The last archive holding a table wins. `client-dbc.manifest.json` records
each table's archive, hash, size and the archives it overrides. `--archive NAME=PATH` reads
another file in place of a client archive, for example to compare against an unpatched copy.

`check` uses this checkout's `DBCfmt.h` and `DBCStores.cpp`. It reports missing tables, file
names a case-sensitive system cannot open, and field layouts the core cannot read. String
fields that point outside a table's string block are reported as notes: the core loads them as
empty strings. CoA's Spell.dbc has such offsets in locale slots the client never reads.

`diff` compares the values the core reads, with strings resolved, so a rebuilt string block is
not a change. Tables the core does not load are compared as raw records.

`install` checks the set, moves the current top-level `dbc/*.dbc` files to
`DataDir/dbc-backups/client-dbc-<time>/`, copies the set and renames tables to the names the
core opens. Subdirectories such as `dbc/Ascension/` are left unchanged. Restart the worldserver
afterwards.

## Checks

```sh
python apps/coa-dbc/test_client_dbc.py
```
