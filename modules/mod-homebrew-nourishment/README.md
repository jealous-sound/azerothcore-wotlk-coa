# Homebrew Nourishment

Homebrew Nourishment by PMZFX adds three lasting, independent meal and drink
effects to curated World of Warcraft 3.3.5a food and drink while preserving
each item's normal health and mana recovery.

This repository contains both halves of the feature:

- a standard AzerothCore server module;
- native client spell rows that let all three effects remain visible together;
- an optional WoW 3.3.5a addon that displays exact recipe values in tooltips.

The client spell patch is required for the three native meal auras to stack and
display correctly. It changes `Spell.dbc` only; it does not modify the game
executable.

## Gameplay

- Eat or drink for the configured uninterrupted completion time (10 seconds by
  default). Movement, standing, combat, death, logout, map changes, or losing
  the normal meal aura cancels the attempt.
- The item's ordinary health or mana recovery is never replaced.
- Food and drinks share three slots. The same item refreshes its current slot;
  new items fill empty slots, and a fourth item replaces the effect expiring
  soonest.
- Each slot keeps its own native aura, so three different meal and drink
  bonuses can run together. Recipe tier and grade determine each effect's
  values; the addon tooltip reports those exact values.
- Ordinary effects last 30 minutes and Premium effects last 60 minutes by
  default. Every profiled food item created by a Cooking recipe receives 30%
  stronger Nourishment stats by default, including ordinary tier-1 cooked food.
  Drinks and non-Cooking food are unchanged. The bonus changes Nourishment
  stats only and does not extend effect duration. Whole-number stat rounding
  guarantees at least one additional point when a small value would otherwise
  round back to its original value.
- Effects persist through logout and restart. Death and arena entry clear them
  by default; both policies are configurable.
- Human players and Playerbots use the same item-cast and completion path.

The included baseline catalogs 300 items across nine leveling tiers and nine
effect families. The companion audit records 195 additional food/drink items
that were deliberately excluded as quest, binding, alcohol-only, pet, holiday,
test, unsupported utility, or otherwise unsuitable items.

| Family | Lasting effect |
| --- | --- |
| Hearty | Strength and Stamina |
| Nimble | Agility and Stamina |
| Fortifying | Spirit and Stamina |
| Insightful | Spell power and Spirit |
| Keen | Critical strike rating and Stamina |
| Precise | Hit rating and Stamina |
| Energizing | Haste rating and Stamina |
| Clear-Minded | Mana per 5 seconds and Stamina |
| Banquet | Attack power, spell power, and Stamina |

## Compatibility

Continuous integration builds the module against both:

- upstream AzerothCore; and
- the `Playerbot` branch of mod-playerbots/azerothcore-wotlk with
  mod-playerbots installed.

The Playerbots header is detected at compile time. On a plain AzerothCore
server, `HomebrewNourishment.IncludePlayerbots` has no effect. On a
mod-playerbots server, it controls whether bots may earn the lasting effect.

The addon targets the original WoW 3.3.5a interface number `30300` and has no
dependencies on other addons.

## Server installation

Back up your databases and server configuration before installing any module.

1. Clone this repository directly into the AzerothCore source tree:

   ```bash
   cd /path/to/azerothcore/modules
   git clone https://github.com/PMZFX/mod-homebrew-nourishment.git
   ```

2. Re-run CMake and rebuild/install AzerothCore using the same options and
   compiler used by your existing server. Modules must be enabled (normally
   `-DMODULES=static`). Follow the official
   [AzerothCore module installation guide](https://www.azerothcore.org/wiki/installing-a-module)
   for your platform.

3. Apply the module database files. With AzerothCore's automatic updater
   enabled, start the newly built worldserver once. You can instead run the
   installed `dbimport` tool. Module SQL is discovered only when the module was
   present and enabled during CMake configuration; see the official
   [database updater guide](https://www.azerothcore.org/wiki/database-keeping-the-server-up-to-date).

4. Apply `data/sql/db-characters/2026_09_24_00_homebrew_nourishment_slots.sql`
   once to the characters database after the original active-state table has
   been installed. It preserves the current meal as slot 1 and expands the
   table to three slots.

5. Confirm that your installed module configuration directory contains
   `HomebrewNourishment.conf.dist`. Copy it to `HomebrewNourishment.conf` if
   your installation process did not create a live copy, then review the
   settings.

6. Start worldserver and look for a message similar to:

   ```text
   mod-homebrew-nourishment loaded 300 item profiles
   ```

The module owns only these tables:

- world: `mod_homebrew_nourishment_profile`
- world: `mod_homebrew_nourishment_stock_aura`
- characters: `mod_homebrew_nourishment_active`

It does not edit `item_template`, stock spells, or player inventories.

## Client addon installation

Install the signed client update with the HXC launcher before testing native
meal stacking. It supplies a separate Spell.dbc aura row for every recipe
family and slot, preventing WoW's normal food/drink replacement rules from
erasing another meal. The addon is optional for gameplay and provides exact
recipe tooltips plus the `/nourishment` status command.

1. Download `HomebrewNourishment-1.1.0.zip` from the
   [latest GitHub release](https://github.com/PMZFX/mod-homebrew-nourishment/releases/latest).
2. Extract it into the WoW client so the final layout is:

   ```text
   World of Warcraft/
   └── Interface/
       └── AddOns/
           └── HomebrewNourishment/
               ├── HomebrewNourishment.lua
               ├── HomebrewNourishment.toc
               ├── LICENSE.txt
               └── README.md
   ```

3. Enable **Homebrew Nourishment** at character selection.
4. Enter the world and hover a profiled food or drink. Its aura and addon
   tooltip identify the slot, family, tier, grade, exact effect, completion
   time, and duration.

Use `/nourishment` or `/mealbuff` to print the current lasting effect. Cooked
food tooltips identify the +30% recipe bonus and show the final rounded stats.

The addon and server module must use the same protocol version. An incompatible
addon prints a clear message instead of interpreting mismatched data.

## Configuration

All settings and defaults are documented in
[`conf/HomebrewNourishment.conf.dist`](conf/HomebrewNourishment.conf.dist).

```ini
HomebrewNourishment.Enable = 1
HomebrewNourishment.CompletionSeconds = 10
HomebrewNourishment.MovementTolerance = 0.35
HomebrewNourishment.OrdinaryDurationSeconds = 1800
HomebrewNourishment.PremiumDurationSeconds = 3600
HomebrewNourishment.PersistThroughDeath = 0
HomebrewNourishment.AllowInBattlegrounds = 1
HomebrewNourishment.AllowInArenas = 0
HomebrewNourishment.IncludePlayerbots = 1
HomebrewNourishment.NotifyPlayers = 1
```

Restart worldserver after changing configuration. The configured completion
time and durations are sent to the addon; no Lua edit is needed.

## GM commands

The module uses AzerothCore's existing GM and reload permissions:

```text
.nourishment status
.nourishment profile ITEM_ID
.nourishment grant ITEM_ID
.nourishment clear
.nourishment reload
```

Commands operate on the selected online player, or the issuing GM when no
player is selected.

## Customized item catalogs

Most servers should use the committed baseline SQL. If your server has
customized `item_template` food/drink rows, use the included generator to
produce a replacement catalog and a complete audit. The generator reads
`SkillLineAbility.dbc` beside `Spell.dbc` to identify Cooking recipe outputs:

```bash
python3 tools/generate-nourishment-profiles.py \
  --spell-dbc /path/to/enUS-3.3.5a/Spell.dbc \
  --skill-line-ability-dbc /path/to/enUS-3.3.5a/SkillLineAbility.dbc \
  --world-database acore_world \
  --mysql-defaults-extra-file /path/to/mysql-client.cnf \
  --output /tmp/nourishment-profiles.sql \
  --audit-output /tmp/nourishment-profile-audit.tsv
```

Requirements:

- Python 3.10 or newer;
- the MySQL/MariaDB command-line client;
- read access to the selected world database; and
- enUS WoW 3.3.5a `Spell.dbc` (234 fields) and `SkillLineAbility.dbc` (69 fields).

The generated SQL replaces data only in the two module-owned world tables.
Review the audit, back up the world database, import the SQL, and run
`.nourishment reload`. Do not distribute Blizzard DBC files in this repository.

The tested baseline audit is available at
[`data/catalog/profile-audit.tsv`](data/catalog/profile-audit.tsv).

## Verification

Run the fast local release checks with:

```bash
./tests/test-release.sh
```

They validate Python and Lua syntax, generator fixtures, protocol agreement,
catalog counts, SQL ownership boundaries, archive contents, and deterministic
addon packaging. GitHub Actions additionally performs clean worldserver builds
for both supported server stacks.

To create the addon archive locally:

```bash
./scripts/package-addon.sh
```

## Upgrade and rollback

To upgrade, pull the desired release in the module directory, reconfigure if
module files changed, rebuild/install, and run the database updater. Replace
the client addon with the matching release ZIP.

For a quick gameplay rollback, set `HomebrewNourishment.Enable = 0` and restart
worldserver. This intentionally leaves module tables intact so an effect can be
re-enabled or the server can be downgraded without discarding state. Database
removal is therefore a deliberate manual administrator action, not part of an
automatic uninstall script.

## Troubleshooting

### The server reports that a nourishment table does not exist

The module SQL was not applied. Confirm the module was present during CMake,
that it was not disabled, and that module updates are allowed in the updater
configuration. Run `dbimport` or import the files under `data/sql` into the
database named by their directory.

### Food restores health or mana but gives no lasting effect

Check `.nourishment profile ITEM_ID`. The item may be deliberately excluded,
the character may be below its minimum level, or the normal meal channel may
have been interrupted. Also review arena/battleground configuration.

### The addon shows no green tooltip block

Confirm the addon is enabled, the server module is loaded, and the versions are
compatible. `/reload` clears the session cache and retries the handshake.

### A stock Well Fed tooltip briefly shows a different value

Install or update the companion addon. The server values are authoritative;
the stock client description is baked into the reused spell record.

## License

Homebrew Nourishment is licensed under GPL-2.0-or-later. See [LICENSE](LICENSE).
