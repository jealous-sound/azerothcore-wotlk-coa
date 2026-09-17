# Verification checklist

## Automated checks

From the package directory:

```sh
python3 tools/check_manifest.py
python3 -m unittest discover -s tests -p 'test_*.py'
lua tests/test_rulesets.lua .
mkdir -p test-bin
g++ -std=c++20 -Wall -Wextra -Werror -I mod-highrisk/src tests/test_drops.cpp -o test-bin/test_drops
g++ -std=c++20 -Wall -Wextra -Werror -I mod-highrisk/src tests/test_power.cpp -o test-bin/test_power
g++ -std=c++20 -Wall -Wextra -Werror -I mod-highrisk/src tests/test_chests.cpp -o test-bin/test_chests
./test-bin/test_drops
./test-bin/test_power
./test-bin/test_chests
```

Use Lua 5.2+ or the matching interpreter shipped with ALE. These tests do not
start a game server. Compiler syntax checks are separate from an actual linked
worldserver build. See `VERIFICATION.md` for checks performed on this package.

## Gameplay on the receiving realm

- At level 14, no mode unlock/drop. At 15, learn the three spells and Opening.
- Change each mode in a rest area; confirm the native UI and auras, then relog.
- Reject changes during combat, while dead/flying and outside a rest area.
- Compare creature-kill XP in High Risk and other modes. Check quest, instance,
  level-cap and bot exclusions, and no double XP callback.
- Kill eligible world creatures at several levels. Confirm the original loot
  remains, and the added item has the expected Bloodforged name/description.
- Check grey creatures, pets, summons, dungeons and raids yield no custom drop.
- Test mixed High Risk/No Risk groups, tap ownership and the configured group
  loot modes. Confirm no duplicate item or unauthorized claim.
- Die to a creature, a player and an environmental source in open world. Verify
  losses match the chest exactly, including gold and item random properties.
- Retrieve with the victim and another High Risk player. Reject No Risk, dead,
  out-of-range and different-phase players. Check a full backpack and gold cap.
- Try simultaneous item/gold claims. Restart with an unclaimed deposit and verify
  recovery without duplication. Never manipulate real inventories for these tests.
- At level 60, equip/remove supported PvP Power gear. Measure direct/melee/periodic
  damage, creature damage reduction, healing and finite shields in each context.

## What existing gameplay evidence covers

A player confirmed a rare Bloodforged Haunting Blade drop and the Bloodforged
tooltip marker on the original integration. This is one successful drop, not
statistical validation of rates, mixed-group behavior or the standalone package.
