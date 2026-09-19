# High Risk and Bloodforged

A standalone source package for AzerothCore 3.3.5a realms using an existing
Ascension-compatible client. It provides native ruleset spells, open-world death
chests, Bloodforged world drops and PvP Power. It is an independent implementation,
not Ascension's original server code.

## Included

- PvE, War Mode and High Risk selection through the existing native interface
  and General spellbook. Modes unlock at level 15 and can be changed in rest areas.
- Persistent ruleset selection and a 20% open-world creature-kill XP bonus in High
  Risk below level 60. Quest and instance XP are unchanged.
- Losses on all eligible open-world deaths, including deaths to creatures.
  Tradeable equipment, bag resources and gold can enter a persistent chest.
- Any living High Risk player nearby can loot the chest, including its original
  owner. Opening uses the native interaction; eligibility is checked server-side.
- One independent Bloodforged roll per eligible corpse, starting at player level
  15. The supplied catalogue has 2,058 reviewed items: 9 uncommon, 816 rare and
  1,233 epic. The first usable epic in this catalogue requires level 40.
- PvP Power from supported equip spells, active from level 60, capped at 495.
  Damage: +0.05% per point against players, and against uncontrolled creatures
  in High Risk open world. Creature damage received there: -0.05% per point.
  Healing and shields: +0.02% per point in War/High Risk open world or battlegrounds
  and arenas. This standalone version has no custom-class or primary-stat dependency.

## Requirements

- AzerothCore with C++20 and the script hooks used by this package. The patch is
  based on commit `77e98c4f853a1b24b0b6b313afa312344f09591d`; see `patches/base.json`.
  Forks and other revisions require a compatibility review, not a blind replacement.
- ALE/Eluna supporting `RegisterPlayerEvent`, `Player:IsBot()` and the XP callback
  `(event, player, amount, victim, source)`, where source 0 is a creature kill.
- MySQL/MariaDB with InnoDB and stored generated columns; Python 3.10+ and Git.
- Compatible server DBC files and base world database. The preflight tools check
  required record semantics and enchantment pools. Missing supported spell/display/
  limit-category rows can be staged; conflicting existing rows are never replaced.
- An Ascension client containing the referenced ruleset spells, Bloodforged item
  displays, lock 43 and Opening spell 22810. No client executable, MPQ, launcher,
  account data, credentials or realm configuration is included.

This package does not set the server level cap. Its drop catalogue is designed
for a level-60 realm. Do not run a second copy of the ruleset, XP, loss or drop
handlers alongside it.

## Installation

This directory is an optional, manually installed package. Merely checking out
this branch does not enable losses, import items or change core behavior.

Use a staging realm first. Stop worldserver before installing. Back up the world
and character databases, server DBC files, source tree and configuration.

1. From the repository root, enter `tools/highrisk` and verify the package:

   ```sh
   cd tools/highrisk
   python3 tools/check_manifest.py
   ```

2. Return to the repository root, then check and apply the small core patch:

   ```sh
   cd ../..
   git apply --check tools/highrisk/patches/core-hooks.patch
   git apply tools/highrisk/patches/core-hooks.patch
   ```

   The patch adds prepared statements for escrow and two healing/absorption hooks.
   It contains no other gameplay systems. Stop and review if `--check` fails.

3. The remaining paths are relative to `tools/highrisk`. Copy `mod-highrisk` into
   the core's `modules` directory. Copy
   `lua_scripts/highrisk_rulesets.lua` into the configured ALE script directory.
   If you already centralize XP callbacks, call `HighRisk.Rulesets.Experience`
   from that pipeline and remove this file's event 12 registration to avoid
   applying the bonus twice. Keep source 0 filtering and round only once.

4. Check world database compatibility from this package directory:

   ```sh
   python3 tools/check_database.py --database acore_world
   ```

   The command only reads data. Use `--defaults-extra-file /path/to/mysql.cnf`
   when needed; never put credentials into the shared package. It rejects chest
   ID collisions, scripted/non-Bloodforged item collisions and incompatible base
   enchantment pools. Review differing Bloodforged templates and back them up;
   `--accept-item-updates` explicitly permits those differences during preflight.
   That flag does not perform an import. The item SQL updates its listed columns
   on existing Bloodforged IDs; it must not be run before this review.

5. Check and prepare server-only DBC changes:

   ```sh
   python3 tools/prepare_dbc.py --dbc-dir /path/to/server/data/dbc
   python3 tools/prepare_dbc.py --dbc-dir /path/to/server/data/dbc --output /path/to/new-staged-dbc
   ```

   Copy the staged files to the server's DBC directory after backing up the originals.
   Do not put them into a client MPQ: they contain server marker auras with native
   XP effects disabled, because this module applies kill XP once in Lua. If an
   existing record conflicts, compare it manually before proceeding.

6. Apply the SQL to the appropriate databases:

   ```sh
   mysql acore_characters < sql/characters/001_highrisk.sql
   mysql acore_world < sql/world/001_chest.sql
   mysql acore_world < sql/world/002_bloodforged.sql
   ```

   The character SQL creates empty state/escrow tables. It includes no characters
   or item instances. Do not drop or clear these tables once players have used them.

7. Copy `data/bloodforged-reviewed.tsv` to a permanent server data directory.
   Merge `mod-highrisk/conf/highrisk.conf.dist` into `worldserver.conf`, setting
   `Bloodforged.Catalogue` to the absolute path of that file. After the checks pass,
   set `HighRisk.Losses`, `Bloodforged.Enable` and `PvpPower.Enable` to `1`.
   Losses and drops ship disabled. No service names or installation paths are assumed.

8. Reconfigure and rebuild using your normal AzerothCore build procedure with
   modules and ALE enabled. Start worldserver and check for:

   ```text
   [High Risk] Native rulesets ready; open-world kill XP bonus: 20%
   Restored 0 High Risk chests; losses true
   Bloodforged world drops ready: 2058 verified catalogue entries
   ```

   The restored chest count can be nonzero after use. Verify there are no SQL,
   Lua, missing spell or item dependency errors before opening the realm.

9. Run the scenarios in `TESTING.md`, including actual death, recovery and mixed
   group tests. Automated checks do not replace gameplay validation on your fork.

## Drop rates

Percent per eligible corpse; these are configurable realm values, not recovered
official CoA rates. There is no counter, pity timer or fixed kill interval.

| Effective tier | Uncommon | Rare | Epic | Total |
| --- | ---: | ---: | ---: | ---: |
| 15–24 | 4.50% | 0.50% | 0% | 5.00% |
| 25–34 | 4.00% | 1.00% | 0% | 5.00% |
| 35–39 | 3.80% | 1.40% | 0% | 5.20% |
| 40–44 | 3.80% | 1.40% | 0.15% | 5.35% |
| 45–54 | 3.60% | 1.80% | 0.25% | 5.65% |
| 55–60 | 3.40% | 2.20% | 0.40% | 6.00% |

The tier is the lower of the creature's native level and the eligible recipient's
level, clamped to 15–60. Grey creatures do not qualify. An epic roll with no
eligible epic item produces nothing; it is not converted into another quality.
Item level is limited to tier+10, or 92 at tier 60, with a nearby-level selection
window. The initial green pool is sparse and may repeat the same item.

## Boundaries and unfinished work

- Fel Commutation insurance is **not implemented**. No insurance fee is charged.
  An existing client insurance tooltip does not mean the feature is active.
- Soulbound/account-bound, quest, temporary, wrapped, refundable and other
  protected items are excluded from death losses. No automatic soulbound-to-
  Bloodforged conversion or Bloody Jar system is provided.
- Full risk-mode phasing, flight restrictions and separate matchmaking are not
  implemented. PvP/FFA flags are not a substitute for world visibility isolation.
- The supplied XP handler grants the High Risk creature-kill bonus only. Existing
  client tooltips may describe War Mode XP, player-kill XP or other unimplemented
  Ascension features; the package does not replace those client strings.
- Bloodforged drops require a persistent, uncontrolled, non-critter world creature
  processed through native creature loot. Creatures without a native loot table
  do not reach this hook. Instances, pets and summoned creatures are excluded.
- Group ownership follows the native tap/loot system. The High Risk and minimum
  level conditions remain on the added item, but mixed-group edge cases need tests.
- 1,490 additional captured item candidates remain excluded. Do not activate them
  simply by removing the dependency checks.
- PvP Power is not inherited by pets. Percentage-health periodic damage, potion
  healing and unlimited shields are not modified by this implementation.
- Gear/resource/gold loss categories each default to a 50% roll. At most one gear
  item below level 60, or two at level 60+, and one resource stack are selected.
  A fallback selects an available enabled category when all rolls miss. Gold loss
  is 1–5% of carried gold; eligible fractional fallback losses round up to one copper.
  This is realm tuning, not a complete reconstruction of Ascension's rules.

## Updating or removing

Do not install this standalone package over another implementation with live
escrow data. Its table and script names are separate; migration requires explicit
mapping. Do not remove a module while players still have items in its escrow.
Disable new losses/drops first, retain recovery handlers, and settle deposits.
Restore source/config/DBC backups only after reviewing affected inventory and
database state; never restore an old character database over live player progress.

## Sources and licensing

The C++ integration targets AzerothCore and is distributed under its included
GPL-2.0 license. Data provenance is recorded in `SOURCES.md`; third-party client
and game data retain their original ownership. No private backend source is claimed.
