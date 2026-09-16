# mod-worldforged-pickups

Worldforged items are picked up off the ground on CoA: a pouch, a bucket, a pile of bones, a
packet, a crate - a world object **named after the base item it holds**. Every character may
open each one **once**; after that it is spent for that character, permanently.

The objects themselves are data, restored by this module's
`data/sql/db-world/2026_09_16_00_worldforged_pickups.sql`: **1,555 pickups, 1,510 of them
placed in the world, each holding exactly its own base item**. What data cannot express - *who*
has already looted *which* pickup - is this module.

## What it restores

| | |
| --- | --- |
| Unplaced objects | 1,555 objects have no usable observation, leaving 1,510 spawns in a fixed GUID block (6900001+) |
| Items | every pickup holds the one item its own name and the realm's catalog say it holds, at 100% |
| Placement | the community's own observed coordinates; the realm's dump corroborates them (see below) |
| Rule | one open per character, then inert for that character and only that character |
| Discovery | an unspent pickup sparkles for the character who can still loot it |

Nothing is invented: every template field is a captured value, every loot row is the realm
catalog's own row, and every position is an observed position.

## How it works

| Piece | Where |
| --- | --- |
| Marker: every pickup carries `ScriptName = 'worldforged_pickup'` | `gameobject_template`, `gameobject` |
| Ledger: `acore_characters.character_worldforged_loot (guid, spawn_id, entry, looted_at)` | `data/sql/db-characters/` |
| `WorldforgedPickupAI` | `src/WorldforgedPickups.cpp` |
| Core hook: `GameObjectAI::BuildClientFlags`, called from `GameObject::BuildValuesUpdate` | `src/server/game/` |

* **Sparkle**: a pickup this character may still loot gets
  `GO_DYNFLAG_LO_ACTIVATE | GO_DYNFLAG_LO_SPARKLE` - that is how they are found in the world.
* **Spent, for one character only**: a pickup this character already looted stays visible
  (as on the realm) but is given `GO_FLAG_LOCKED | GO_FLAG_NOT_SELECTABLE`, so it cannot be
  opened again. Every other character still sees it sparkling and lootable.
* **Recording**: `OnPlayerLootItem` fires the moment a base item leaves the pickup, so
  clicked, auto-stored and group-window loot are all covered. The item and the ledger row are
  written in one character-database transaction, so a crash cannot mark a pickup spent
  without the item it was spent for.
* **`OnAllowedForPlayerLootCheck` refuses the item itself** for a character who has already
  looted this pickup. This is the one that closes the remaining hole: a loot session that
  outlives its claim. The chest holds a single shared loot, so the next character's open
  re-rolls it under the first character's still-open window, and `Player::StoreLootItem`
  would otherwise hand over a second copy to whoever clicks the slot first. Note that the
  core's hook reads backwards: `ScriptMgrMacros.h` treats a script returning `true` as a
  refusal, so true is what withholds the item.
* **Four server-side refusals in total**, because a client can always ask anyway:
  `BuildClientFlags` (what this viewer is shown), `GossipHello` (whether the use opens
  anything - `GameObject::Use` returns before `SendLoot`), the loot-slot check above (whether
  the item may be handed over), and `OnStateChanged` (which opens nothing, and clears a spent
  loot the player left open).

Identity is the **spawn id** (`gameobject`.`guid`), never the runtime object GUID: this core
uses map-local generated GUIDs (`Map::GenerateLowGuid`), which are neither the database row
nor stable across grid reloads. The restoration writes its spawns in the fixed block
6900001+, so ids stay stable across re-imports.

## Requirements

1. **The client's gameobject display table.** A spawn whose `displayId` is missing from the
   server's `Data/dbc/GameObjectDisplayInfo.dbc` is thrown away at load:

   ```
   Gameobject (GUID: 6900001 Entry 1344099 GoType: 3) has an invalid displayId (87226), not loaded.
   ```

   A stock table holds 3,792 rows; CoA's client ships 120,871, and the restored pickups use
   1,009 ids from it. Against the stock table 1,489 of 1,510 pickups never reach the world and
   the world stays empty however good the data is (it also silently drops ~1,222 stock objects).
   Install the client's DBC set with the [client DBC tool](../../apps/coa-dbc/README.md); it
   includes this table.

2. **The SQL applied.** With `Updates.EnableDatabases = 7` (all three databases) the core
   applies `data/sql/db-world/` and `data/sql/db-characters/` at startup by itself; use `6`
   if you want the characters and world databases only. `1` is the auth database alone and
   would leave both migrations unapplied. On a repack that runs with the updater off, apply
   the two files by hand, exactly as they are:
   `data/sql/db-world/2026_09_16_00_worldforged_pickups.sql` and
   `data/sql/db-characters/2026_09_16_00_worldforged_loot.sql`. The first is idempotent: the
   templates and loot rows are written with `REPLACE INTO` (keyed on `entry`, and on
   `Entry, Item`), and each block of spawns is preceded by a `DELETE` on its own `guid`
   range - so re-applying it is safe, twice in a row or a hundred times.

## Verify it went in

Boot log:

* `>> Loaded 98138 Gameobjects` - equal to `SELECT COUNT(*) FROM acore_world.gameobject`, so
  nothing was skipped for a display id
* 3,784 C++ scripts with this module loaded (`GameObjectScript`, `GlobalScript`,
  `PlayerScript`)
* no `Script named 'worldforged_pickup' is assigned in the database, but has no code!`
* no `has an invalid displayId (...)`, and no invalid-rotation warnings

```sql
SELECT COUNT(*) FROM acore_world.gameobject WHERE guid BETWEEN 6900001 AND 6999999;   -- 1510
SELECT COUNT(*) FROM acore_world.gameobject_template WHERE ScriptName='worldforged_pickup';  -- 1555
SELECT COUNT(*) FROM acore_world.gameobject_loot_template WHERE Comment LIKE 'AscensionWorldforged:%';  -- 1555
```

In play: `.go xyz -8769.1 -174.1 83.9` (Wax Stained Bag, 185 yd from Stormwind). It should
sparkle, hand over its own base item once, and then be inert **for that character** while
another character still sees it sparkling. A character's ledger:

```sql
SELECT * FROM acore_characters.character_worldforged_loot WHERE guid = <character guid>;
```

## Two things that only show up in play

* **When the ledger is read matters.** It is read in `Player::LoadFromDB`, not in
  `OnPlayerLogin`. The core sends a player the gameobjects around them before
  `CharacterHandler` calls `OnPlayerLogin`, so a ledger loaded at login arrives after the
  client has already been told the pickup sparkles and can be opened - and nothing corrects
  it, because the object's own state never changes. The symptom is: loot a pickup, log out
  and back in, and it is glowy and clickable again (the server still hands over nothing).
  Loaded during `LoadFromDB`, the first update a player receives already carries the right
  flags.
* **A loot window closed by logging out leaves the object `GO_ACTIVATED` with empty loot**,
  and `Player::SendLoot` only re-rolls a chest that is `GO_READY`. `OnStateChanged` notices
  the spent loot and re-arms the object for that character - once, guarded, never in a loop -
  so an abandoned loot window cannot deny the next character their item.

## Why the pickups can be re-looted at all

The data deliberately sets `Data3` (consumable) to `0` and `Data2` (restock) to `0`. A
consumable chest despawns on loot and returns on a respawn timer - that would make a pickup a
realm-wide roll per respawn instead of one open per character. Non-consumable keeps the object
in the world for everyone, and the core re-rolls its loot for each new opener:
`Player::SendLoot` clears and re-fills the loot while the object is `GO_READY`, and
`GameObject::Update` puts a chest back to `GO_READY` once a loot is released.

No respawn timer is involved either, so none is invented: the spawns carry
`spawntimesecs = 0`. `GameObject::LoadGameObjectFromDB` copies that into `m_respawnDelayTime`,
which is only consulted when something despawns the object - and for a chest with
`consumable = 0` the deactivation branch sets `GO_READY` and returns before it can schedule a
respawn. Any other value would be dead data; 0 also happens to be what
`ObjectMgr::LoadGameobjects` accepts silently, since it only reports a zero spawn time as an
error when `IsDespawnAtAction()` is true and for a chest that is `chest.consumable`.

## Placement: where the positions come from, and what is not recoverable

Positions are the archive's per-object observations - one world XYZ each, the object's own
coordinates, not the looter's. Two independent checks say so: the realm's own dump records,
for each item, the distance from the loot position to the nearest object of that entry, and
that distance equals the distance from the loot position to the observed position (median
difference 0.0 yd, 89% within 1.5 yd, n=2,954); and where a placement is more than 10 yd from
the dump's loot position it is re-anchored to it (13 spawns). Heights are the observation's
own, corroborated by the dump's stored object height (175 of 182 spawns that sit >15 yd below
this repack's terrain grid - real interiors - agree within 1.5 yd).

**Facing is not recoverable.** None of the six sources available holds the realm's own
orientation for these objects: not the client's gameobject cache, not the realm's catalog
export (its `gameobject_spawn` table is empty and has no orientation column anyway), not the
community position dumps, not the archive atlas. Each restored spawn therefore carries a
stable per-object facing with the matching unit quaternion
(`rotation0 = rotation1 = 0`, `rotation2 = sin(a/2)`, `rotation3 = cos(a/2)`, the convention
the rest of `gameobject` uses). Stable rather than random so a re-import reproduces the same
world. A zero quaternion - what the captures actually carry - is not unit length and the core
rejects it row by row.

## Undo

```sql
SOURCE data/sql/manual/worldforged-pickups-revert.sql;
DROP TABLE IF EXISTS acore_characters.character_worldforged_loot;
```

The undo lives under `data/sql/manual/` rather than beside the migration because it *deletes*
rows from `gameobject_template`, which the repository's SQL lint asks updates never to do.
Both migration files pass that lint; the undo is the one file that deletes on purpose.

To re-test one pickup from scratch, clear the character's row and re-log (the module drops its
in-memory copy on logout):

```sql
DELETE FROM acore_characters.character_worldforged_loot WHERE guid = <character guid>;
```
