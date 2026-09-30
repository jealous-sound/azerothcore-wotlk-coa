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

## Talent rules

`SetTalentRank` is the one owner of the rules: class, active specialization, rank count, level, automatic entries
immutable, budget. A rank above the held one must fit the tree's budget (`Spent` over the known entries versus
`GetCoATalentBudget`); rank 0 and lower ranks always go through, so an over-budget character can always come back
under it. `SwitchSpecialization` removes every class talent spell when the specialization changes, then grants the
new specialization's automatic entries. `ResetPaidTalents` removes every paid rank of the class; automatic grants
and the specialization stay. Clients reach them only through the native packets below; there is no chat command.

Row gates and choice nodes: a paid node's row is `PositionY` (byte `0x192`) and its gate is
`RequiredTabAEInvestment` (dword 38, class tree: 0/9/24 for rows 0-3/4-7/8-9) or `RequiredTabTEInvestment`
(dword 39, specialization tree: 0/8/23). The gate counts points in the same tree's earlier rows. Paid nodes
sharing a `Group` (field 29) are one choice node: taking one option replaces the other. `SetTalentRank` refuses a
change that leaves a held node below its gate (a removal as well as an addition) unless that node was already
below it, and a known-entries upload is refused the same way as a whole; `RestoreBuilds` restores in row order;
`OnPlayerLogin` refunds, deepest first, paid nodes that break a gate or share a choice node
(`RepairTalentLayout`). The tree's parent links (`ConnectedNodes`) are not in the DBC, so the server does not
check which parent a node hangs from.

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

Opcodes and layouts come from the reconstructed `Extensions.dll` (`firstoni-dev/ascension-extensions-reconstruction`,
`docs/DLL_REFERENCE.md`, module `AscCAMgr`); the client build is the authority.

- `SMSG 0x0725` active specialization: `u32 slotIndex, u32 slotCount`. Its first arrival builds the
  per-character container the next packet needs, so it always goes first. The server sends slot 0 of 1. The
  client does not read the specialization from it: `GetActiveChrSpec` is the first `ChrSpecs` row whose identity
  entry (+0x70, `CoASpecialization::IdentityEntryId`) the build holds.
- `SMSG 0x0726` known entries: `u32 count`, then `u32 entryId, u32 rank, u32 0, u8 0, u32 0, u32 0` per record.
  Always the complete set; the client diffs it, fires `ASCENSION_KNOWN_ENTRY_UPDATED/REMOVED` and answers
  `C_CharacterAdvancement.IsKnownID` / `GetTalentRankByID` from it. Sent after the initial 0x0725 and again
  after every upload, reset, specialization switch and level change.
- `CMSG 0x0727` known-entries upload (`ApplyPendingBuild`): same records, the client's complete wanted set (an
  unlearn is a smaller set, never a delta). It arrives on the network thread (`CanPacketReceiveEarly`), so it is
  queued by account and applied on the player's own update. `SwitchActiveChrSpec` only edits the pending build:
  it drops the old specialization's entries and adds the new identity entry and the entry of the
  specialization's signature spell (`ChrSpecs` +0x60, `SignatureEntryId`). `SpecializationOf` reads the
  specialization from the upload's ranked specialization entries; entries of two specializations refuse it.
  A specialization other than the active one, with no paid or free-choice entry of it, is a switch:
  `SwitchSpecialization` runs, restores that specialization's stored build, and the rest of the upload is
  ignored. With such entries the switch runs first and the upload then applies as the complete set. Otherwise
  `ApplyKnownEntriesUpload` checks every entry, prices the state the set leads to (including ranks the
  progression pass hands back to omitted entries) and applies removals before additions through
  `SetTalentRank`; an upload that changes nothing still runs the progression pass. Any failure refuses the
  whole upload. 0x0726 always follows, then `SMSG 0x072C` {result, learn result, u32 entry, u32 rank}
  (`CHARACTER_ADVANCEMENT_UPDATE_ENTRIES_RESULT`): `CA_UPDATE_ENTRIES_OK`, or `_BAD_ENTRY` (unknown or
  foreign entry, rank past the entry, missing server spell, mixed or invalid specialization),
  `_NOT_TRAVERSIBLE` (`CA_LEARN_LOW_LEVEL`, `CA_LEARN_MISSING_AE` / `_TE` over budget) or `_UNKNOWN`
  (malformed upload, missing budget row). The CoA frame plays its apply sound on success; the general CA
  frame shows a refusal as a red error.
- `CMSG_UNLEARN_TALENTS` (0x0213, `C_CharacterAdvancement.UnlearnAllTalents`): queued with the uploads in
  arrival order and handled by `ResetPaidTalents`, then 0x0726 and `SMSG 0x072B` with `CA_PURGE_TALENTS_OK`,
  or `CA_PURGE_TALENTS_NO_KNOWN_TALENTS` when nothing was removed. The CoA talent frame resets through
  `ClearPendingBuild` and an upload instead.
- `CMSG 0x06E1` inspect (`C_CharacterAdvancement.InspectUnit`, u64 guid): `SMSG 0x06E2` answers a `CA_INSPECT_*`
  string; on `CA_INSPECT_OK` the guid, active slot 0, one slot and the target's known entries in the 0x0726 layout.
  Missing, not-in-world and out-of-range (`INSPECT_DISTANCE`) targets get their own results.
- Timing: the client keys this state off its local player object, which does not exist during the loading
  screen. `OnPlayerLogin` only queues the state; the first `CMSG_SET_ACTIVE_MOVER` of the session sends it
  (`OnPlayerActiveMover`). The Ghost harness login never sends that packet: a test that needs the state calls
  `bot.ActivateCharacterAdvancement(t)` after registering its packet hook.
- Opcode identities: `0x0523` is `CMSG_CUSTOM_ASCENSION_POINT_SPEND_REQUEST`, `0x061A` is
  `CMSG_CREATURE_QUERY_BULK`, `0x064A` is `SMSG_PATCH_CHARACTER_ADVANCEMENT` (unused: the client loads its own
  catalogue). Despite its name, `0x0523` carries no talent points: its five senders are the vanity-collection Lua
  functions (`C_VanityCollection` and `RequestDeliver*CollectionItem`), each writing
  `{u8 Enum.VanityCurrency, u32 item}`. Currency 2 (Donation Points) comes from both Deliver and the web-shop
  buy; the server handles it as a delivery through `DeliverVanityItem`.

## Checks

- `python -B tools/verify_all.py --stages harness --harness talent_state`: budgets, rank derivation, point
  accounting, packet layout, upload parsing and the specialization each upload selects, compiled against the
  real catalog.
- Gameplay scenarios pick specializations and ranks with the `specialization` and `advancement_rank` actions,
  which inject the same 0x0727 upload through the early packet hook.
- Ghost `e2e/coa/talents/authority_test.go`: persistence across a relog, budget refusal, reset, the 0x0725/0x0726
  sequence and the 0x0727 upload against a running slot.
