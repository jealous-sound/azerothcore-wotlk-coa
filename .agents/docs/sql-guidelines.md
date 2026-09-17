# SQL guidelines

## Adding SQL updates

1. `cd data/sql/updates/pending_db_world/` (or `pending_db_auth` / `pending_db_characters`).
2. `./create_sql.sh` generates an empty `rev_<timestamp>.sql` to write into.
3. Conventions (linted): every `INSERT` preceded by a matching `DELETE` (idempotency); no double semicolons; no multiple blank lines; InnoDB engine.

For SQL changes, lint the affected files: `python apps/codestyle/codestyle-sql.py --files <path> ...`.
Use `--base <local-ref>` when selecting changes against a branch. The linter does not fetch from the network.
The no-argument command retains broad SQL checks and resolves a locally available default comparison branch.

## Data conventions

- `smart_scripts` edits always rewrite the full block — `DELETE` + `INSERT` of every row for the `(entryorguid, source_type)` pair, with the `DELETE` matching both columns — never a partial `UPDATE`, not even for a comment-only fix.
- `creature_immunities`: negative ids are curated shared sets — reference them via `creature_template.CreatureImmunitiesId`, never edit them or allocate new ones. Positive ids are single-creature sets — reuse an existing set only on an exact match; to extend a creature's immunities, insert a superset under a new id and point the creature's `CreatureImmunitiesId` at it.

## The three databases

- `acore_auth` — accounts, realm list, IP/account bans, session keys. Shared across all realms.
- `acore_characters` — per-character state: characters, inventory, in-progress quests, mail, guilds, arena teams, achievements. One per realm.
- `acore_world` — static game content: creature/gameobject/item/quest templates, spawn lists, loot tables, SmartAI scripts, gossip, conditions. Read-mostly; rebuilt from SQL.

## Verification and updater semantics

- During authorized runtime validation, apply migrations through the normal server updater; do not manually
  apply the same SQL as well. Verify affected rows and the migration's recorded name/hash.
- `updates.state = PENDING` can describe an already applied pending-directory migration. Inspect its recorded
  name/hash and the actual pending-file comparison rather than inferring execution status from that field alone.
- Empty or truncated MySQL output does not prove that a query returned no rows. Metadata queries such as
  `SHOW COLUMNS` must return meaningful output. Use existing query tooling, bound read retries, and never blindly
  retry ambiguous writes or expose credentials in output.
- Verify the rows and relationships affected by the change. Respawn timers and Wintergrasp state can advance
  after restart without players; whole-database equality is not a useful routine assertion. Preserve character
  state, inventory, spells, and collections while investigating relevant unexpected changes.
