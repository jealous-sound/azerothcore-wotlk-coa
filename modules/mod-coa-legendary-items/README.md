# CoA Legendary Items

64 original non-set leveling legendaries: three per custom class (IDs 12–32), plus a shared cloak.
See the [interactive preview](preview.html) or the [complete catalog](CATALOG.md).

Eligible creature kills have a configurable 0.5% chance to add one legendary to normal corpse loot.
The loot owner determines the class pool. Normal group loot rules apply. Pets can earn a drop for their owner;
player-owned creatures, critters, totems, creatures without XP eligibility and gray creatures cannot drop one.
Players at level 60 or above receive no new drops by default. Previously earned equipment keeps working.

Required level is `creature->getLevelForTarget(lootOwner)`, the same per-player level used by Destiny Weaver.
Item level is always required level + 12. A level-32 player killing a wolf scaled from 10 to 29 receives a
level-29, item-level-41 variant. Without scaling, that gray level-10 wolf is ineligible. Normal and quest loot
continue through their existing paths. Each design has level-1 through level-80 variants with fixed stats;
equipping one does not increase its level.

Movement powers are exclusive to boots. Boots use cloth armor and class-specific stats, allowing their intended
class to equip even the earliest variants. Conditional powers refresh within 250 ms and disappear when
unequipped, broken, dead or disabled. After-kill powers last eight seconds and require a non-gray creature kill.
Only the strongest equipped copy of a design grants its power. Movement uses native aura speed stacking rules.
Signature trinkets boost the named ability's main direct hit by 12%, across its native rank chain.
Secondary triggered spells and periodic effects retain their normal calculations.

## Integration

The module is discovered automatically by CMake. Its configuration is
`conf/mod-coa-legendary-items.conf.dist`. To exclude it, configure with
`-DMODULE_MOD-COA-LEGENDARY-ITEMS=disabled`.

The pending migration `data/sql/updates/pending_db_world/rev_1791051693307538000.sql` supplies 5,120 item templates,
matching `item_dbc` rows and hidden native power auras. Apply it through the normal worldserver updater.
The module validates its item and spell records at startup and disables drops and powers if they are incomplete.

`AscensionCompat` already streams `item_dbc` through `SMSG_PATCH_ITEM` (`0x0932`, eight 32-bit fields).
All variants use that existing stream and existing client appearances. Names, orange legendary quality,
requirements, stats and legendary descriptions come from native item query responses. Keep
`CoA.SendDisplayPatches=1` enabled. No client DBC file edit is needed for these items.

The authoritative designs are in `data/catalog.json`. Regenerate the C++ catalog, pending migration, catalog
and standalone HTML preview with `python3 -B modules/mod-coa-legendary-items/tools/generate_catalog.py`.
The HTML works directly from disk and makes no external requests.

## Verification

Use `python3 -B tools/verify_all.py` for source, build, unit, catalog and gameplay checks.
The gameplay scenarios use a separate test module configuration with `CoALegendaryItems.DropChance=100`
so drop eligibility can be asserted deterministically.
