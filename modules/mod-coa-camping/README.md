# CoA Camping

An optional AzerothCore module implementing basic camping from the Forever handoff captured on
4 October 2026, build `1.60.1.70009`. Camping is disabled by default. The module uses existing core hooks;
its core-side additions are native gameplay test actions and measurements.

## Build and enable

The normal `MODULES=static` build discovers this directory and `Addmod_coa_campingScripts`. Reconfigure CMake
after adding it. Set `-DMODULE_MOD-COA-CAMPING=disabled` to exclude the entire module, including its unit tests.

The four pending camping migrations add supplies, furniture and service templates and adjust their scale.
Let the normal updater apply them to the chosen deployment. Reserved template collisions are preserved;
enabled startup rejects incompatible mappings. The already applied migrations stay unchanged.
No existing fire, spell or Ranger template is changed.

Copy `conf/mod-coa-camping.conf.dist` to the server's module configuration directory without `.dist`, set
`CoACamping.Enable=1`, and restart. Enabled startup rejects missing or incompatible spells, templates,
displays, materials, skills and invalid configuration. Settings are immutable until the next restart.

## Playing

1. Cast Basic Campfire (818) on allowed outdoor terrain while alive and out of combat. The default map
   allowlist is 0, 1, 530 and 571. Flight, transport, vehicles and underwater placement are excluded.
2. Click the nearby **Campsite Supplies** alchemy set and select a contribution. The menu lists the required
   profession rank and materials. A camp holds five features by default. Solo contributions are enabled by
   default, so one character can place every feature family without a shared contribution cooldown.
3. Sit within 20 yards of the fire for 60 uninterrupted seconds to receive its available stat benefits for
   one hour. Clicking the chair seats the player through the normal chair handler. Standing idle does not count.
4. With a tent present, 30 seated seconds replenishes rested XP to 5% of the XP needed for the next level.
   This has a separate one-hour character lockout. A larger rested pool is preserved and consumes no lockout.
5. Click a service bot to open its normal vendor window. Repair bots also offer the normal paid repair button.

| Contribution | Profession | Materials | Benefit or service |
| --- | --- | --- | --- |
| Incense Candle | Herbalism 20 | 1 Peacebloom, 1 Silverleaf | +2 Intellect |
| Camp Tent | Leatherworking 20 | 5 Light Leather | Rested XP floor after 30 seated seconds |
| Camp Chair | Skinning 20 | 3 Light Leather, 2 Simple Wood | +2% melee, ranged and spell crit |
| Faction Banner | Tailoring 20 | 1 Bolt of Linen Cloth, 1 Coarse Thread | Same-faction Spirit: 14/19/27/32 |
| Reagent Bot | Engineering 20 | 1 Handful of Copper Bolts, 1 Copper Tube | Standard reagents at native prices |
| Repair Bot | Engineering 140 | 2 Whirring Bronze Gizmos, 1 Bronze Tube, 1 Heavy Leather | Reagents and paid repair |

Supplies and incense sit 1.6 yards from the fire, the chair 2.4 yards, service bots 2.6 yards and the banner
2.8 yards. The full-size tent sits 6 yards away with its open entrance facing the fire. The smaller props
occupy the other side of the fire, keeping the entrance clear. Template scales apply to both the visible models
and their native collision geometry. The scale migration preserves foreign templates and custom scales.

Spirit bands start at levels 1, 40, 50 and 60. Banner appearance and eligibility use the contributor's faction.
The two service choices share one family slot; repair bots can be placed directly in a camp without a service
bot, but cannot replace or upgrade an existing reagent bot. All other features have one slot per family.
Profession stations, upgrade paths and recipe acquisition remain disabled.

Movement, standing, combat, death, logout, teleport, phase changes or removal of the camp reset unfinished rest.
Eligibility is checked every player update; camp selection and completion are scanned once per second.
The stat rest clock starts once an eligible stat feature is present. The tent has its own rest clock.
A completed rest grants once; stand and sit again to begin another rest. A completed rest may refresh a
camping-owned reward, including one retained after relog.

Contribution requests are committed on the map update lane and rechecked after opening gossip. Skill,
carried materials, range, line of sight, phase, capacity and expiry must still be valid. Rejected requests or
failed prop creation consume nothing. Successful contribution saves inventory and, when restricted, its
cooldown in one character-database transaction. Set `CoACamping.AllowSoloContributions=0` to enforce one
contribution per character per camp and the shared cooldown, which defaults to one hour across every camp and
survives relog/restart. Solo mode ignores existing contribution deadlines and preserves them without starting
new ones. Profession ranks, material costs, capacity and duplicate-family checks still apply. Set
`CoACamping.Capacity=3` to restore the original shared camp size. Tent pool changes and their deadline are saved
together through the native character save transaction. Max-level players receive no tent reward or lockout.
The `core.coa.camping` setting is mandatory
gameplay state even when optional player preferences are disabled; no new character table is needed.

Managed fires last fifteen minutes by default, together with their supplies, reward helper, linked trap and
attachments. Static fires, differently summoned fires and Ranger spell 807955 never enter the registry.
Only one camp per owner is allowed, with 40-yard spacing in the same phase. Camps are removed when the owner
logs out, leaves the map or changes phase, when the fire/controller disappears, on expiry or map unload.
Owner death keeps the camp. Camps do not survive restart. Paid materials are not refunded on cleanup.
Completed rewards retain their own one-hour lifetime after the camp is gone.

## Mappings and fidelity

`src/CoACampingMapping.h` is the mapping source. The core checks its entries against effective startup data.

| Role | Mapping |
| --- | --- |
| Fire | Spell 818, object 29784, display 192 |
| Supplies | New object 9500200, stock alchemy-set display 345, gossip goober |
| Candle | New object 9500201, stock candle display 100 |
| Tent | New object 9500202, human-tent display 7194, scale 1 |
| Chair | New object 9500203, wooden-chair display 39, native single medium-height seat |
| Banners | New objects 9500204/9500205, Alliance/Horde displays 5771/5773, scale 1 |
| Services | New creatures 9500220/9500221, repair-bot display 14379, neutral faction 190 |
| Rewards | Effect 0 only: 1459 Intellect, 14752 Spirit, CoA's 34833 all-crit aura |
| Contribution | Herbalism 182, base rank 20; items 2447 and 765, one each |

The profession gate and reagents match the catalog's placement-item rank and crafting materials. The first
reward deliberately uses the lowest stock Arcane Intellect amount at every level; it does not implement
Forever's level bands. Only the Intellect effect is applied, even when CoA's effective Arcane Intellect has
additional effects. Existing Arcane/Dalaran Intellect and Brilliance ranks are preserved, including caster
ownership and remaining duration. Divine/Prayer of Spirit and Moonkin Aura are also preserved. Class effects
take priority over their camping proxy; successful native aura applications, including area auras, remove only
the conflicting camping reward. Separate caster records keep each reward identifiable after relog and camp expiry.
The crit mapping requires CoA's effective Master Tactician aura type 290, which changes all three crit chances.
Its stock client icon/name is a proxy; no existing spell definition is modified.

The allowlists, radii, spacing, lifecycle and seated-only policy are implementation choices from the handoff,
not observed Forever server behavior. The capacity setting excludes the fire, supplies and helper;
each feature occupies one slot. Profession stations, upgrades, recipe unlocks, seed nodes and legacy perks are
outside this implementation. The handoff did not capture a service inventory or economy: both bots therefore
sell nine standard class reagents at normal item prices with unlimited stock, and repairs use the core's normal
durability fees. There are no free repairs, custom prices, replacement refunds or bot charge mechanics.

The display and spell IDs are present in the locally extracted CoA DBCs. An unmodified build-12340 client
and the broader fork's stock compatibility still require separate in-game acceptance. The module adds no
client patch and does not replace any existing DBCs. Native scenarios exercise server behavior only.

## Verification

Reconfigure the build, then use the repository's verification command:

Windows runs require the linked MySQL and OpenSSL DLLs and OpenSSL's `legacy.dll` beside the build
executables. Run configuration profiles sequentially because the default reusable world cache is
leased by one verification batch at a time.

```sh
python -B tools/verify_all.py --base HEAD --stages source,build,unit --query camping
python -B tools/verify_all.py --stages gameplay --scenario coa-camping-solo-contributions \
  --settings path/to/camping-solo-verify.json
python -B tools/verify_all.py --stages gameplay --settings path/to/camping-verify.json \
  --scenario coa-camping-class-buffs coa-camping-contribution-revalidation coa-camping-furniture \
  coa-camping-rest-interruption coa-camping-services coa-camping-shared-rest coa-camping-tent-rested-xp
```

Gameplay settings must select module configurations with `CoACamping.Enable=1`. The solo profile exercises
the defaults, `CoACamping.AllowSoloContributions=1` and `CoACamping.Capacity=5`. The shared profile requires
`CoACamping.AllowSoloContributions=0` and `CoACamping.Capacity=3`; the expiry profile also uses these shared
limits. One configuration cannot satisfy both contribution modes. The scenarios keep native
spell costs/cooldowns and gossip handlers, and cover shared contribution, revalidation, persistent cooldown,
rest interruption, class-buff ownership in both orders, faction filtering, native seating, rested XP consumption,
paid purchases/repairs, fire identification and cleanup. Unit tests cover
elapsed-time rest transitions and contribution gates. Deploying the module and checking client rendering
are separate from this source change.

The deadline check is separate because its two-minute lifetime differs from the default fifteen-minute
profile. Use another module configuration directory with `CoACamping.LifetimeSeconds=120` and select that
directory through `path/to/camping-expiry-verify.json`:

```sh
python -B tools/verify_all.py --stages gameplay --settings path/to/camping-expiry-verify.json \
  --scenario coa-camping-contribution-revalidation modules/mod-coa-camping/tests/expiry/coa-camping-expiry.json
python -B tools/verify_all.py --stages harness --harness test_camping_migration --settings path/to/camping-verify.json
```
