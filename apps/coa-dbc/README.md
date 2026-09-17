# CoA client DBC set

The worldserver loads `DataDir/dbc` at startup. It must hold the original CoA client's tables: with a stock
or partial set it drops content that references CoA-only rows, such as item limit categories, gameobjects and
currencies. Players use the same tables, so client patches must not replace DBC archives, and `*_dbc` world
tables must not override client rows. Game files are never committed.

Requires Python 3.11+ and [mpqcli](https://github.com/TheGrayDot/mpqcli).

```sh
python apps/coa-dbc/client_dbc.py extract "C:/CoA/client/Data" out/client-dbc --original --mpqcli path/to/mpqcli.exe
python apps/coa-dbc/client_dbc.py check out/client-dbc
python apps/coa-dbc/client_dbc.py diff "C:/CoA/server/data/dbc" out/client-dbc
```

Copy the extracted files into `<DataDir>/dbc` and restart the worldserver.

- `extract` reads the client's archives in load order; the last archive holding a table wins. `--original`
  reads the launcher's untouched `NAME.ORIGINAL` copies, `--archive NAME=PATH` substitutes any archive. Tables
  are named as the worldserver opens them. `client-dbc.manifest.json` records each table's archive and hash.
- `check` validates the set against this checkout's `DBCfmt.h`: missing tables, file name case and field
  layouts. Strings outside the string block and rows with index -1 are reported as notes.
- `diff` compares the values the core reads, so a rebuilt string block is not a change.

Tests: `python apps/coa-dbc/test_client_dbc.py`
