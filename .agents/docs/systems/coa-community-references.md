# CoA community references

Use these sources to find the right spell, support an interpretation of what a class, spec, talent or ability is
meant to do, or phrase a question. They are community snapshots, not the live server or backend.

## Trust rules

- When the installed DBC, the database or the code has the value, it wins. A snapshot that differs is a finding to
  report, not a value to copy.
- When they lack it, a value taken from a snapshot names its source and snapshot date in the commit and the PR,
  and the choice goes to the user first.
- Do not vendor the dumps into this repository.

## Classes, specs and talents

- [bindmysoul.com](https://bindmysoul.com/) (e.g. `?realm=voljin&class=primalist&spec=wildwalker&pane=talents`).
  Dataset `https://bindmysoul.com/conquest/talents.normalized.json`: every CoA talent node (21 classes, 70 specs,
  3612 nodes) with `spellId`, full tooltip text with numbers, tree kind, required level, point costs and
  dependencies. Snapshot of the CoA Build Hub calculator (`metadata.fetchedAt` 2026-07-22). Its spec IDs differ
  from the server's `.localspec` IDs: join on `spellId`.
- [ascensionsidekick.com](https://ascensionsidekick.com/) with `https://ascensionsidekick.com/data.js`
  (`window.ASC.coaKits[class].specs[].abilities[]`: name, level, talent flag, description without numbers or IDs;
  `coaGuide`/`coaSkill` playstyle notes).

## Spells, creatures, spawns, loot and client caches

Check these before calling server-missing data unknown:

- [hertigservices/ascension-data](https://github.com/hertigservices/ascension-data):
  - `supplemental/exiles-db-export/`: 2026-09-13 PostgreSQL/CSV export of the db.exil.es CoA database (spells,
    creatures, spawns with coordinates, loot, vendors, trainers, quests); columns in `SCHEMA_REFERENCE.md`, large
    files in the repository's Releases;
  - `datasets/cache.json`: captured WDB client caches (creature, item and quest records);
  - `supplemental/coa-databank/`: CoA Build Hub talent and skill scrapes.
- [AscensionDB](https://ascension-db.ascension-archive.workers.dev/) searches all of it by name or ID (creatures,
  objects, items, quests, gossip) and shows each captured record with its source, mode and date. WDB creature
  records give name, type, family and model IDs, not stats.
- [hertigservices/Ascension_preservation](https://github.com/hertigservices/Ascension_preservation) documents the
  client protocol (`reference/ascension_custom_opcodes.json`, `docs/WIRE-SPEC.md`).

The export's operator is anonymous and no license is asserted for it: cite rows as evidence only.

## New client and server DBC rows

[notacoder-dev/wxl-extended-dbc](https://github.com/notacoder-dev/wxl-extended-dbc) layers `Foo.dbc1-<project>`
add-on files over base tables (a WXL client extension plus the AzerothCore `mod-wxl-dbc` module and a core patch).
The CoA client runs `Ascension.exe`/`Extensions.dll` without a WXL loader, so adopting it is a decision for the
user and the maintainers, not a fix-time step.
