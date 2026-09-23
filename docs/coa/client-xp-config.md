# Client XP configuration

With `CoA.Enable` enabled, character login sends `SMSG_COA_CONFIG` (`0x58D`) before the
character-advancement login state. Reloading world configuration resends the current values to sessions
with a player in the world.

The packet follows the six-section layout from PR #4128, revision
`1340ce91e16b6583f608249c0630c90d709ffe07`: integers, booleans, floats, rates, integer vectors and float
vectors. Each section begins with a uint32 entry count. Rate entries contain a uint32 byte length, an
ASCII key without a terminator, and an IEEE-754 float. Scalars use the normal little-endian packet writer.

This implementation populates only the rates section, with 31 supported XP settings:

- `RATE_XP_GLOBAL` and `RATE_XP_PROFESSION` come from `Rate.XP.Global` and `Rate.XP.Profession`.
- `RATE_XP_KILL`, `RATE_XP_KILL_TBC` and `RATE_XP_KILL_WOTLK` come from `Rate.XP.Kill`, `Rate.XP.Kill.TBC`
  and `Rate.XP.Kill.WotLK`; the quest keys likewise from `Rate.XP.Quest`, `Rate.XP.Quest.TBC` and
  `Rate.XP.Quest.WotLK`.
- `RATE_XP_EXPLORE`, `RATE_XP_ELITE` and `RATE_XP_DUNGEON_ELITE` come from `Rate.XP.Explore`,
  `Rate.XP.Elite` and `Rate.XP.DungeonElite`.
- `RATE_XP_PROFESSION_<DIFFICULTY>_MODIFIER` comes from the gray, green, yellow and orange settings.
- `RATE_XP_PROFESSION_<PROFESSION>_MODIFIER` comes from each of the 16 implemented profession settings.
  The client's `FIRST_AID` spelling maps to `Rate.XP.Profession.FirstAid`.

All values come from the core's validated, cached configuration, whose defaults equal the captured
realm's rates. The capture itself is not replayed. Empty sections do not implement the other
configuration features from #4128, and this change does not include its talent-tree packets or
feature gates.

The table describes realm base rates. Dynamic XP's player choices, XP buffs, and the provisional
`Rate.XP.Profession.BaseFraction` remain server-side; no verified client keys exist here for those values.
The packet does not make the client calculate an exact XP award or add a new visible UI panel.

Run `python3 -B apps/coa-tests/coa_config/run.py` to compile the production sender
with the real WorldPacket/ByteBuffer writer and isolated world/session dependencies. The test decodes
its bytes independently, checking the opcode, section framing, all 31 mappings, zero rates, changed
rates and a missing session. Actual client consumption and display require a separate client test.
