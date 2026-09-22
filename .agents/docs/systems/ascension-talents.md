# CoA talents: catalog, commands and the client's character-advancement service

Use this reference for anything touching CoA talent points, ranks, specializations or the talent window's
state. The live pieces are `src/server/coa/AscensionCoATalentData.*` (catalog),
`AscensionCoATalentState.*` (spellbook-derived state and wire form) and `AscensionClassService` in
`AscensionCompat.cpp` (rules, commands, packets).

## Where the state lives

- The server holds no talent table. A held rank is the rank's spell in `character_spell`; the active
  specialization is the player setting `core.ascension_active_spec` (`character_settings`, loaded and saved
  even with `PlayerSettings.Enabled = 0`). `AscensionCoATalentState::KnownRank` derives an entry's rank as the
  highest rank spell the character owns.
- The catalog is `CharacterAdvancement.dbc` (entries, ranks, costs, level, tab) with `ChrSpecs.dbc`
  (specializations and their identity passive), read from the client DBC set. `CharacterAdvancementEssence.dbc`
  gives the cumulative point budget: key = class id (12–32), all match flags 0, one row per level. At level 10
  the class tree has 1 point, at 11 both trees have 1, at 60 it is 26/25, at 80 36/35. Every paid entry costs
  1 point per rank; automatic entries (both costs 0, not a selectable free group) are granted by
  `SynchronizeProgression`, never bought.

## Specialization names: internal token vs displayed name

`ChrSpecs.dbc` carries **two** name columns and they are not interchangeable:

- field 2 — the internal token (`DISPLACEMENT`, `FIREARMS`, `MOONBOW`…), matching
  `CharacterAdvancementTabTypes.dbc` field 1. This is the join key: `LoadCoATalentData` maps a
  `CharacterAdvancement` row's tab to a spec through (`ChrClasses` token, tab token), all uppercased.
- field 29 — the name the client displays. **The core never reads it**: `LoadCoATalentData` loads
  `ChrSpecs.dbc` with 29 fields, so index 29 is out of range by one, and the field is also absent from the
  talent wire form.

For **36 of the 101 specs** the two differ, so the internal token is not a safe label for a spec in an issue,
a PR description or a report. The Chronomancer is the trap, because its three names are also offset against
each other:

| Spec id | Internal token | Displayed | Skill line |
|---|---|---|---|
| 31 | `DISPLACEMENT` | Time | 81 `Time` |
| 32 | `DUALITY` | Infinite | 79 `Infinite` |
| 33 | `TIME` | Artificer | 80 `Artificer` |

An audit grouped by internal token therefore files the Artificer wand talents under a heading reading "Time",
which a player reads as wrong data (PR #4324). Tab 87 `Class` is the shared class tree and belongs to no spec.

When naming a spec for a human, read field 29 (or `SkillLine.dbc`, which uses the displayed names); when
joining tables, use field 2.

## Commands (the shipped client's channel)

- `.localspec <specId>`: `SwitchSpecialization`. A different specialization removes every class talent spell,
  then grants the new specialization's automatic entries.
- `.localtalent <entryId> <rank>`: `SetTalentRank`, the one owner of the rules: class, active specialization,
  rank count, level, automatic entries immutable, budget. A rank above the held one must fit the tree's budget
  (`Spent` over the known entries versus `GetCoATalentBudget`); rank 0 and lower ranks always go through, so an
  over-budget character can always come back under it.
- `.localtalent reset`: `ResetPaidTalents`, every paid rank of the class; automatic grants and the
  specialization stay.
- `.localtalent sync`: the character-advancement state again (bridge message, and the native known entries
  once the session's container exists), for a client whose listener loaded after the login push.

## Stored builds

Ascension keeps a build per specialization and swaps between them. A switch here removes every talent spell,
so `SwitchSpecialization` first writes down the build being left (`StoreBuilds`: the shared class tree and the
specialization's own tree, paid and free-choice ranks read from the spellbook as `entryId * 10 + rank`) in the
player settings `core.ascension_build.<spec>` (0 for the class tree, index 0 = count), then puts back the build
of the specialization being entered (`RestoreBuilds`), each rank through `SetTalentRank` so a stored rank the
character can no longer afford is skipped, then runs the progression pass. Entering a specialization from
none restores the same way. While a specialization is active the spellbook stays the truth; the record is
only read on entry, never at login, so a rank the player removed is not resurrected. Idea from #4031.

## The client's character-advancement service (Extensions.dll)

Opcodes and layouts come from the community measurements in `hertigservices/Ascension_preservation`
(`server/world_server.py`); the client build is the authority, so re-measure before extending them.

- `SMSG 0x0725` active specialization: `u32 slotIndex, u32 slotCount`. Its first arrival builds the
  per-character container the next packet needs, so it always goes first. The server sends slot 0 of 1; how a
  slot maps to a `ChrSpecs` id, and the CMSG of a native specialization switch, are unknown — the
  specialization id stays with `.localspec`.
- `SMSG 0x0726` known entries: `u32 count`, then `u32 entryId, u32 rank, u32 0, u8 0, u32 0, u32 0` per record.
  Always the complete set; the client diffs it, fires `ASCENSION_KNOWN_ENTRY_UPDATED/REMOVED` and answers
  `C_CharacterAdvancement.IsKnownID` / `GetTalentRankByID` from it. Sent after the initial 0x0725 and again
  after every `.localtalent`, `.localtalent reset`, `.localspec` and level change.
- `CMSG 0x0727` known-entries upload: same records, the client's complete wanted set after a native learn or
  unlearn (an unlearn is a smaller set, never a delta). It arrives on the network thread
  (`CanPacketReceiveEarly`), so it is queued by account and applied on the player's own update.
  `ApplyKnownEntriesUpload` checks every entry, prices the state the set leads to (including ranks the
  progression pass hands back to omitted entries), selects the specialization when none is active (one per
  upload), then applies removals before additions through `SetTalentRank`; any failure refuses the whole
  upload, and 0x0726 always follows.
- Timing: the client keys this state off its local player object, which does not exist during the loading
  screen. `OnPlayerLogin` only queues the state; the first `CMSG_SET_ACTIVE_MOVER` of the session sends it
  (`OnPlayerActiveMover`). The Ghost harness login never sends that packet: a test that needs the state calls
  `bot.ActivateCharacterAdvancement(t)` after registering its packet hook.
- `ASC_LOCAL_CAD` bridge (from #4030): an addon-channel whisper from the character to itself, sent at
  `OnPlayerLogin` and again with every known-entries resend. Message
  `ASC_LOCAL_CAD<TAB>1:<specId>:<chunk>:<chunks>:<entry,rank;entry,rank...>`, payload chunked at 180 bytes,
  one-based chunk index, total repeated in every chunk; an empty set is one chunk with an empty payload. It
  carries what the native packets cannot: the `ChrSpecs` id. The patch-B `FrameXML/LocalCharacterAdvancementBridge.lua`
  consumes it and the compat layer trusts it over its SavedVariable and spellbook reconstruction.
- Three-message form (from #4031, read by its client half): `ASC_LOCAL_SPEC<TAB><specId>`,
  `ASC_LOCAL_RECORDS<TAB>1 1` (the server always holds a record; an empty list is an empty tree) and one
  `ASC_LOCAL_TALENTS<TAB><entry:rank entry:rank ...>` with the paid and free-choice ranks only, adopted whole
  by that client and therefore never split. Sent with the bridge; `.localspecstate` is that client's request
  for it (same handler as `.localtalent sync`). One of the two forms retires with the client patch that ships.
- Opcode identities (from #4030, decoded from the `Extensions.dll` name-stub table): `0x0523` is
  `CMSG_CUSTOM_ASCENSION_POINT_SPEND_REQUEST` (the native point purchase, never sent by the patch-B shim),
  `0x061A` is `CMSG_CREATURE_QUERY_BULK`, `0x064A` is `SMSG_PATCH_CHARACTER_ADVANCEMENT` (unused: the client
  loads its own catalogue).

## The shipped compat client (patch-B.MPQ, `Ascension_Collections`)

`CharacterAdvancementCompat.lua` and `CharacterAdvancementStateCompat.lua` override the native API: the
specialization is a per-character SavedVariable, ranks are rebuilt from `IsSpellKnown`/`IsPlayerSpell`, the
budget is a local formula equal to the essence table, and Save sends `.localspec` then one `.localtalent` per
changed entry. Known defects (client repository, not this one): the SavedVariable is read at file scope before
WoW restores it, so the specialization overlay returns on every reload or relog; `IsSpellKnown` misses
`SPELL_ATTR0_DO_NOT_DISPLAY` passives, so hidden ranks read as 0 and a reset never removes them. The server's
0x0726 exists so a client can take its ranks from `GetTalentRankByID` instead.

## Checks

- `python3 apps/coa-tests/talent_state/run.py --dbc-dir <client dbc dir>`: budgets, rank
  derivation, point accounting, packet layout and upload parsing, compiled against the real catalog.
- Ghost `e2e/coa/talents/authority_test.go`: persistence across a relog, budget refusal, reset, the 0x0725/0x0726
  sequence and the 0x0727 upload against a running slot.
