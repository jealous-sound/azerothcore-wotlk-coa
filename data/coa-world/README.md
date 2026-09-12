# CoA world baseline: 2026-09-12

This is the world content from the clean snapshot shipped with CoA Repack release
`client-compat-20260911`. It contains 318 tables: 315 content tables plus the three
updater/version tables. There are 562,396 item templates and 9,464 quest templates.

Use the [installation and maintenance workflow](../../apps/coa-world/README.md) to
verify the package, import an empty world schema, or audit an existing installation.

## Provenance and scope

- Source: the release's `Database/Clean/databases.sql.gz`, restricted to `acore_world`.
- Source snapshot SHA-256:
  `c1ec932ebf64a3e37797e8887ee7573712ac25157ef16984a9c37b34f9d23ca4`.
- Repository migration revision: `8fefb6f0ae253c25014da13eab04ba2e3d0685c0`.
- Archive SHA-256:
  `06a3a4d331c61cff569391369427308b70c3a5d9cdf790c44f78bbae72d8afe3`.
- The manifest records table schemas, SQL checksums, row counts, content fingerprints,
  and 82 covered pending/module migrations.

Authentication and character databases are excluded. The three historical item-template
backup tables listed in the manifest are excluded; active compatibility/provenance tables
are retained. Updater paths are rewritten to repository-relative paths. Content rows retain
the repack values, including fractional weapon damage. This is a reproducible installation
baseline, not a claim that every harvested gameplay value has been independently verified.

The initial read-only audit of the running CoA Repack matched every content table exactly.
Its only additional tables were the three excluded historical backups.

## Comparison with the source SQL

A fresh import of the repository base SQL followed by 356 applicable migrations was
compared with this baseline on isolated MySQL 8.4.9, using UTC sessions:

| Content | Source SQL | Repack baseline | Difference |
| --- | ---: | ---: | --- |
| Item templates | 190,250 | 562,396 | 372,146 missing; 190,215 existing rows differ |
| Quest templates | 9,464 | 9,464 | 8,459 existing rows differ |
| Trainer spells | 6,417 | 11,156 | 4,739 missing |
| Trainers | 126 | 127 | One missing |
| Creature templates | 31,218 | 31,219 | One missing, plus its spawn, model and trainer binding |

The source also lacks two heirloom compatibility/provenance tables with 112 rows each.
There are 159 event rows with differing schedule timestamps: the baseline preserves the
repack's UTC instants, including its three-hour offset from base SQL timestamp literals.
Generated compatibility-table timestamps are ignored in this source comparison.

The complete baseline import matched all content fingerprints. Repeating bootstrap was
refused without changing content, and current migration hashes required no replays.
Package-integrity and refusal checks also passed the nine focused unit tests.

## Starter items discussed in PR #18

| Entry | Name | Quality | Damage | Armor | Maximum durability |
| --- | --- | --- | --- | --- | --- |
| 484319 | Decayed Dagger | 1 | 0.9375–3.75 | 0 | 18 |
| 967757 | Noxious Kilt | 0 | 0–0 | 6 | 35 |

The repack already contains these populated definitions. A source installation built only
from the old base SQL and migrations can instead retain generated appearance placeholders.
Importing this baseline before first startup gives new installations the repack definitions.
Upgrading an older populated source database requires reviewed differences and, for existing
placeholder equipment, handling saved item durability in the character database separately.
