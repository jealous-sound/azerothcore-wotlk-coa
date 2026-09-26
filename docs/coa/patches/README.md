# Ascension client patches

Server-driven, in-memory patches for the original Ascension client. No client file edits are required.

## Scope and loading

The patch system is part of the required CoA core component in `src/server/coa`.
`AscensionPatchSpells.cpp`, `AscensionPatchItems.cpp` and `AscensionPatchAchievements.cpp` implement the packet
encoders. Their headers expose the Encode/Send APIs. These update client tables, not
server gameplay. Packet examples and layout manifests live alongside this document.

`CoAScriptLoader.cpp` registers `AddAscensionPatchTooltipScripts()` exactly once.
There is no optional module loader or runtime module-presence check. Reconfigure
CMake before rebuilding so core source discovery picks up the migrated files.
`AscensionPatch.Tooltips = 0` in `src/server/coa/conf/coa.conf.dist` keeps automatic
patching disabled by default. To enable it, set the same key in the installed
`coa.conf`. The former `mod_ascension_patch.conf` is no longer loaded; transfer any
explicit setting you want to retain into `coa.conf`.

## SQL spell text patches

The world database table `patch_spell` stores one override per spell ID:

| Column | Meaning |
| --- | --- |
| `spell_id` | Primary key; must have a compiled original client baseline |
| `enabled` | Only rows with value 1 are loaded |
| `name` | Nullable replacement spell name |
| `rank` | Nullable replacement rank text |
| `description` | Nullable replacement spellbook description |
| `aura_description` | Nullable replacement aura tooltip |
| `comment` | Administrator note; never sent to the client |

For every text column, SQL NULL preserves the original string and an empty string
clears it. All numeric client fields are preserved. Text containing NUL or exceeding
4096 bytes is rejected. Unknown baseline IDs and invalid rows are skipped with an
error naming the spell. Currently only Rippling Renewal (560384) and Rippling
Protection (560394) have verified baselines in `AscensionPatchSpellData.h`; inserting
another spell ID alone does not add support for that spell.

The core migration is
`data/sql/updates/pending_db_world/rev_1790407756703836600.sql`. The normal world
database updater creates the table and seeds the current Renewal/Protection text.
The core prepared statement references this table even with automatic patching off,
so apply the migration before using the rebuilt server (the normal startup updater
runs before statement preparation).

The migration's filename and contents are unchanged from its former module location,
which has been removed. The updater recognizes an applied file by name and hash;
moving it does not require rerunning the seed SQL. Do not import it manually again,
since the seed statements replace those two rows.

The core loads enabled SQL rows on world startup and caches their encoded packets.
`AscensionPatch.Tooltips = 1` sends those packets to every player at login; the option
defaults to 0, which sends nothing and disables the tooltip amount hooks. Restart
worldserver after editing SQL. Reloading configuration only changes the enable switch;
it does not reread SQL or resend patches to online clients. Restart the client to
remove previously received patches, including when disabling/deleting a row.

The general patch API has no Chronomancer dependency. Renewal's dynamic `${$w1}`
amount still uses the existing Chronomancer gameplay and 0x0673 aura-amount transport
in `src/server/coa/AscensionChronomancerTime.cpp`. Its hook runs only when an enabled Renewal row was successfully
cached and the configuration switch is on. The hook stores the outgoing pulse preview
in Renewal's otherwise unused `PERIODIC_TRIGGER_SPELL` amount and refreshes it every
500 ms. Protection already exposes its live absorb pool.

Renewal's preview precedes recipient modifiers and critical healing. Preview
calculations temporarily detach spell-modifier consumption tracking so displaying
a value cannot mark modifiers for consumption by an in-progress cast.

## Packet contract

Each packet contains exactly one row, without an outer row count. `fields` contains numeric wire words, not necessarily a disk or runtime DBC row. Supply all prefix words followed by all suffix words; the encoder inserts `strings` between them. Floats use their IEEE-754 bit representation. Known pointer placeholders are replaced by zero.

Strings normally use a uint32 byte length with no terminator. TagTypes and ActivationOverlays instead use NUL-terminated strings. Embedded NULs, incorrect field/string counts, unknown opcodes and oversized payloads are rejected.

ItemEnchantment requires sixteen localized strings in client locale-slot order; its first suffix word is the locale flags word skipped by the client. ShapeshiftForm omits its runtime name pointer. VisualEffectName and MissileMotion put strings immediately after the ID, before their trailing numeric fields. Spell takes its 170-word runtime projection plus name, rank, description and aura tooltip.

Use this sparingly: on-disk client data, patched client data and server SpellInfo can disagree. Preserve unrelated fields and prefer updates of existing rows. Shared radius, duration, range, icon and visual records can affect other spells. Do not overwrite CustomAttr changes supplied by CoASpellbook.

## Verified layouts

`layouts.json` records DLL handler RVAs and pointer slots. Verified DLL SHA-256: `0f8d847b3adc44a963606f0cd4f7938ad6fcd6f4d87feac3131c7153bdf3bb11`. Handler decoding establishes wire layout, not rendered-client acceptance or the meaning of every numeric field.

| Opcode | Name | Prefix words | Strings | Suffix words | String encoding |
|---|---|---:|---:|---:|---|
| `0x0570` | `SMSG_PATCH_SPELL_CHARGES` | 2 | 0 | 0 | length32 |
| `0x0571` | `SMSG_PATCH_SPELL_CHARGES_CATEGORY` | 3 | 0 | 0 | length32 |
| `0x0574` | `SMSG_PATCH_SPELL_AFFECT` | 3 | 0 | 0 | length32 |
| `0x05F4` | `SMSG_PATCH_SPELL_CUSTOM_ATTR` | 11 | 0 | 0 | length32 |
| `0x0686` | `SMSG_PATCH_SPELL_ADDON` | 23 | 0 | 0 | length32 |
| `0x06C0` | `SMSG_PATCH_SPELL_TAG_TYPES` | 29 | 6 | 0 | nul |
| `0x06C1` | `SMSG_PATCH_SPELL_TAGS` | 3 | 0 | 0 | length32 |
| `0x06CE` | `SMSG_PATCH_SPELL_SPELL_SUGGESTIONS` | 4 | 0 | 0 | length32 |
| `0x06D2` | `SMSG_PATCH_SPELL_STAT_SUGGESTIONS` | 4 | 0 | 0 | length32 |
| `0x06F6` | `SMSG_PATCH_SPELL_ACTIVATION_OVERLAYS` | 14 | 2 | 0 | nul |
| `0x092A` | `SMSG_PATCH_SPELL` | 170 | 4 | 0 | length32 |
| `0x092C` | `SMSG_PATCH_SPELL_VISUAL` | 32 | 0 | 0 | length32 |
| `0x092D` | `SMSG_PATCH_SPELL_VISUAL_KIT` | 38 | 0 | 0 | length32 |
| `0x092E` | `SMSG_PATCH_SPELL_VISUAL_KIT_MODEL_ATTACH` | 10 | 0 | 0 | length32 |
| `0x092F` | `SMSG_PATCH_SPELL_VISUAL_EFFECT_NAME` | 1 | 2 | 4 | length32 |
| `0x0930` | `SMSG_PATCH_SPELL_MISSILE` | 15 | 0 | 0 | length32 |
| `0x0931` | `SMSG_PATCH_SPELL_MISSILE_MOTION` | 1 | 2 | 2 | length32 |
| `0x0935` | `SMSG_PATCH_SPELL_ITEM_ENCHANTMENT` | 14 | 16 | 8 | length32 |
| `0x0949` | `SMSG_PATCH_SPELL_SHAPESHIFT_FORM` | 18 | 1 | 0 | length32 |
| `0x094A` | `SMSG_PATCH_SPELL_ICON` | 1 | 1 | 0 | length32 |
| `0x094F` | `SMSG_PATCH_SPELL_CATEGORY` | 2 | 0 | 0 | length32 |
| `0x0950` | `SMSG_PATCH_SPELL_DURATION` | 4 | 0 | 0 | length32 |
| `0x0951` | `SMSG_PATCH_SPELL_CAST_TIMES` | 4 | 0 | 0 | length32 |
| `0x0952` | `SMSG_PATCH_SPELL_RADIUS` | 4 | 0 | 0 | length32 |
| `0x0953` | `SMSG_PATCH_SPELL_RANGE` | 8 | 2 | 0 | length32 |
| `0x0954` | `SMSG_PATCH_SPELL_DIFFICULTY` | 5 | 0 | 0 | length32 |
| `0x0955` | `SMSG_PATCH_SPELL_DESCRIPTION_VARIABLES` | 2 | 1 | 0 | length32 |
| `0x0956` | `SMSG_PATCH_SPELL_EFFECT_CAMERA_SHAKES` | 4 | 0 | 0 | length32 |
| `0x0957` | `SMSG_PATCH_SPELL_CHAIN_EFFECTS` | 45 | 2 | 0 | length32 |
| `0x0958` | `SMSG_PATCH_SPELL_ITEM_ENCHANTMENT_CONDITION` | 16 | 0 | 0 | length32 |
| `0x0959` | `SMSG_PATCH_SPELL_VISUAL_KIT_AREA_MODEL` | 3 | 1 | 0 | length32 |
| `0x095A` | `SMSG_PATCH_SPELL_VISUAL_PRECAST_TRANSITIONS` | 3 | 2 | 0 | length32 |
| `0x095F` | `SMSG_PATCH_SPELL_RUNE_COST` | 5 | 0 | 0 | length32 |
| `0x0961` | `SMSG_PATCH_SPELL_FOCUS_OBJECT` | 2 | 1 | 0 | length32 |

## Item patches

Include `AscensionPatchItems.h` and use `AscensionPatchItems::Encode` or `Send`. Core source discovery includes `AscensionPatchItems.cpp`; it needs no script registration. There are no automatic item patches. Examples for all sixteen IDs are in `patch_item-examples.md`. Send patches to each client session that needs them.

The fifteen verified packets contain one complete numeric record followed by strings
encoded as uint32 byte length plus bytes, without NUL terminators. Numeric fields
include zeroed pointer placeholders; `item_layouts.json` records their indices and
handler RVAs for the same DLL fingerprint as the spell audit. Preserve unrelated
fields. Client memory changes do not update server item templates or client files.

`SMSG_PATCH_ITEM` updates the eight-word Item.dbc row, not the full item template.
`SMSG_PATCH_ITEM_STAT` exists in the DLL name table, but no registered reader or
payload layout was verified. Its enum is reserved and both Encode and Send reject
it. This is deliberately not advertised as supported.

| Opcode | Name | Numeric words | Strings |
| --- | --- | ---: | ---: |
| `0x0569` | `SMSG_PATCH_ITEM_ADDON` | 15 | 2 |
| `0x056A` | `SMSG_PATCH_ITEM_DISPLAY_INFO_COLLECTIONS` | 8 | 2 |
| `0x059F` | `SMSG_PATCH_ITEM_STAT` | unverified | - |
| `0x0693` | `SMSG_PATCH_ITEM_APPEARANCES` | 3 | 0 |
| `0x06EC` | `SMSG_PATCH_ITEM_SET_APPEARANCES` | 3 | 0 |
| `0x0932` | `SMSG_PATCH_ITEM` | 8 | 0 |
| `0x0944` | `SMSG_PATCH_ITEM_EXTENDED_COST` | 16 | 0 |
| `0x0964` | `SMSG_PATCH_ITEM_VISUALS` | 6 | 0 |
| `0x0965` | `SMSG_PATCH_ITEM_VISUAL_EFFECTS` | 2 | 1 |
| `0x0966` | `SMSG_PATCH_ITEM_SET` | 37 | 1 |
| `0x0967` | `SMSG_PATCH_ITEM_RANDOM_SUFFIX` | 13 | 2 |
| `0x0968` | `SMSG_PATCH_ITEM_RANDOM_PROPERTIES` | 8 | 2 |
| `0x0969` | `SMSG_PATCH_ITEM_PURCHASE_GROUP` | 10 | 1 |
| `0x096A` | `SMSG_PATCH_ITEM_LIMIT_CATEGORY` | 4 | 1 |
| `0x096B` | `SMSG_PATCH_ITEM_DISPLAY_INFO` | 25 | 14 |
| `0x096C` | `SMSG_PATCH_ITEM_COND_EXT_COSTS` | 4 | 0 |

## Refreshing cached item descriptions

Item descriptions such as Bazaar Token (975001) come from `item_template.description`
through the normal item-query response, not `SMSG_PATCH_ITEM`. The client can save
these responses in its disk cache, so restarting WoW alone may leave old text visible.

To refresh cached data for connecting players:

1. Apply the SQL update to the world database and confirm the item's description is correct.
2. Set `ClientCacheVersion` in the active `worldserver.conf` to a new, unused value.
   For example, if the server currently advertises version 17, use:

   ```ini
   ClientCacheVersion = 18
   ```

3. Restart worldserver to load both the changed item template and configuration, then reconnect clients.
   The changed version requests a client cache reset; it affects all connecting players.

`ClientCacheVersion = 0` uses `version.cache_id` from the world database; it does not
turn caching off. Check the startup log for the advertised version and choose a value
that differs from it and has not recently been used. A cache reset cannot fix an
unapplied SQL update: clients would simply cache the old description again.

For a single-client diagnostic, fully close WoW and rename its active `Cache` folder
to `Cache.old` (choose another name if that already exists), then relaunch and reconnect.

This differs from the in-memory spell patches described above: fully exiting the
client discards those patches, while saved item-query data can survive client restarts.
Changing the cache version does not reload `patch_spell` rows or replace their login delivery.

## Checks

From the repository root, run focused verification through the repository runner:

```powershell
python -B tools/verify_all.py --stages source,build,harness --base HEAD --harness test_patch_module --harness test_patch_packets --harness test_patch_spell_data
```

The native packet harness uses MSVC in a developer shell, or a C++20 compiler selected
by `CXX`/`c++`. It compiles the actual encoders and SQL override builder against a small
packet/session fixture. Cases cover every supported layout, NULL-versus-empty text,
original-field preservation, invalid IDs/text, zero-length append assertions and
session guards. This does not exercise a real database or prove client rendering.

## Achievement patches

`AscensionPatchAchievements.h` exposes `AscensionPatchAchievements::Encode` and `Send`.
Core source discovery includes the implementation. Experimental Descension login patches
are independently gated by `AscensionPatch.Achievements`, `AscensionPatch.AchievementCriteria`
and `AscensionPatch.AchievementCategories`, all defaulting to 0 and requiring
`AscensionPatch.Tooltips = 1`. The achievement-only patch reproduced the level gate
on a level-60 character. Resending existing achievement state restored the UI and
showed the patched title. Login delivery now performs that resend after any enabled
achievement patches; the automatic sequence still needs client verification.
Keep criteria and category patches disabled until their layouts are verified.
See `patch_achievement-examples.md` for the disputed layouts and isolation procedure.
These packets update client records, not server criteria or earned achievements.
The Descension set covers 336 achievements, 189 associated criteria and 16 categories.
The SQL seeds cover all enUS category names and achievement titles/descriptions containing
Ascension in the inspected baseline DBCs.

### SQL text overrides

Achievement text overrides live in the world database. The migration
`data/sql/updates/pending_db_world/rev_20260926_30_patch_achievements.sql` creates
and seeds all three tables with the current Descension corrections.

| Table | Primary key | Nullable text overrides |
| --- | --- | --- |
| `patch_achievement` | `achievement_id` | `name`, `description`, `reward` |
| `patch_achievement_criteria` | `criteria_id` | `description` (enUS) |
| `patch_achievement_category` | `category_id` | `name` (enUS) |

Each table also has `enabled` (default 1) and an optional `comment`.
`NULL` retains the original baseline text; `''` explicitly clears it.
Disabled or deleted rows are not sent. An enabled row with all text columns NULL
sends the original baseline record. Numeric fields and server requirements are never overridden.

```sql
UPDATE `patch_achievement`
SET `description` = 'Obtain 10000000 Runes of Descension.'
WHERE `achievement_id` = 41191;

UPDATE `patch_achievement_category`
SET `name` = 'Path to Descension'
WHERE `category_id` = 22000;
```

After installing this implementation and applying the migration, future text edits
for supported records require only SQL changes and a worldserver restart. Startup
loads enabled rows through prepared queries and caches validated packets; login
performs no SQL queries. Config reload changes delivery gates, not cached SQL data.
Fully exit and restart the client when testing removals or disabling patches:
previously patched client records are not reverted by deleting a SQL row.

`AscensionPatchAchievementData.h` retains original client baselines, not renamed text.
Only its 336 achievement, 189 criterion and 16 category IDs are currently supported.
Adding an ID outside that set requires a complete baseline and a rebuild; unsupported
IDs or invalid text are logged and skipped. The existing master and per-family config
switches still apply. Category and criterion packet formats remain unverified.

| Opcode | Table | Prefix words | Strings | Suffix words |
| --- | --- | ---: | ---: | ---: |
| `0x0936` | `SMSG_PATCH_ACHIEVEMENT` | 14 | 3 | 0 |
| `0x0937` | `SMSG_PATCH_ACHIEVEMENT_CRITERIA` | 9 | 16 | 5 |
| `0x0938` | `SMSG_PATCH_ACHIEVEMENT_CATEGORY` | 2 | 16 | 2 |

Fields contain prefix and suffix words consecutively. Strings are inserted between
those groups and use uint32 byte lengths. Achievement strings are title, description,
and reward text; its pointer words 4, 5 and 11 are zeroed. Criteria and Category require
all sixteen localized strings in the current encoder. These Criteria and Category
layouts conflict with the newer atlas evidence and remain unverified on the client.
See `achievement_layouts.json` for handler RVAs and the verified DLL fingerprint.

With explicit compilation permission, run the native packet fixture using:

```powershell
python -B tools/verify_all.py --stages harness --harness test_patch_packets
```

The fixture covers all three layouts, numeric/string ordering, pointer placeholders,
UTF-8 lengths, malformed payloads and session guards. Rendered client behavior remains untested.
