# Handoff: N-key spec-selection popup & talent-tree visual desync

Status report from an investigation session on 2026-09-17, testing `main` (`77e98c4f8`, includes PR #1498's
client-DBC migration) against a locally-built repack. Written for whoever (human or agent) picks these two
bugs up next, so the same ground doesn't get re-covered.

## TL;DR

- **N-key popup bug**: confirmed, again, that it cannot be fixed from server-side C++ alone. Root cause and a
  working fix are known; the fix has to ship as a client-side Lua change in the next official CoA client patch
  (`patch-B.MPQ`), not as a PR to this repo. Exact snippet below.
- **Talent-tree visual desync bug** (tree looks unspent even though the server has the real data; not specific
  to any one class -- reported across multiple characters, only tested against Templar/Oathkeeper this session):
  found and fixed two genuinely real bugs in the client's own compat addon, verified them in a Lua sandbox and
  live in-game — but neither one is the cause of the visible symptom. That symptom's root cause is compiled into
  the closed-source `Ascension.exe` client itself. This is a dead end for this repo and for any addon; it needs
  the Ascension client team.
- PR #1498 (client-DBC migration) does **not** fix either bug by itself. Confirmed by testing a from-scratch
  local build with the real client DBC set installed.

## 1. N-key spec-selection popup (classes 12-32)

**Symptom:** pressing N re-shows the spec-selection screen instead of going straight to the talent tree, even
for a character that already picked a spec in a previous session.

**Root cause:** `C_CharacterAdvancement.GetActiveChrSpec()` is a **client-only, Lua-closure-internal** value.
AzerothCore has no implementation of Ascension's private character-advancement backend service, so the native
client function always returns `nil`/rejects. The client's own compat shim
(`Interface/AddOns/Ascension_Collections/CharacterAdvancementCompat.lua`, shipped inside `patch-B.MPQ`)
overrides `C_CharacterAdvancement.GetActiveChrSpec`/`SwitchActiveChrSpec` to track the active spec locally
(`ASCENSION_LOCAL_ACTIVE_SPEC_ID`, a SavedVariable) instead of asking the server. Nothing calls
`SwitchActiveChrSpec` automatically after `PLAYER_ENTERING_WORLD`, so on a fresh login `GetActiveChrSpec()`
reads `nil` and the client shows the spec-selection screen again, even though the server
(`character_settings.core.ascension_active_spec`) and the client's own persisted SavedVariable both already
know the right answer.

**Confirmed NOT fixable server-side.** Tried and ruled out in an earlier session (see this repo's prior work,
not included here as commits): sending `SMSG_LEARNED_SPELL`, sending 0x0562 (`SendCharacterAdvancementEntryList`
/ whatever it's called post-#1498 refactor) with valid entries, sending 0x0725 with `[specId, confirmed]` —
none of them touch `GetActiveChrSpec()`, because it is never read from a packet on the client side; it is pure
Lua state set only by `SwitchActiveChrSpec()`. This is architectural, not a matter of finding the right opcode.

**The fix that works (client-side only):** a tiny addon, `CoA_SpecFix`
(`Interface/AddOns/CoA_SpecFix/{CoA_SpecFix.toc,CoA_SpecFix.lua}`, dependent on `Ascension_Collections`):

```lua
-- CoA_SpecFix.lua
local frame = CreateFrame("Frame")
frame:RegisterEvent("PLAYER_ENTERING_WORLD")
frame:SetScript("OnEvent", function()
    if C_CharacterAdvancement.GetActiveChrSpec() == nil and ASCENSION_LOCAL_ACTIVE_SPEC_ID then
        C_CharacterAdvancement.SwitchActiveChrSpec(ASCENSION_LOCAL_ACTIVE_SPEC_ID)
    end
end)
```

This works today as a standalone addon players install. **Recommendation:** fold the same check directly into
`CharacterAdvancementCompat.lua` itself (the file that already owns `ASCENSION_LOCAL_ACTIVE_SPEC_ID` and
`SwitchActiveChrSpec`) so it ships in the official client patch and nobody needs a separate addon. That file
already has a `PLAYER_ENTERING_WORLD`-adjacent init path (it runs once at load, guarded by
`ASCENSION_LOCAL_CHARACTER_ADVANCEMENT_COMPAT`) — the same self-heal call can go right after that guard.

**Server-side is already correct and needs no further work.** The class-tree-inclusion fix from an earlier
session (send `SpecId == 0` entries, i.e. the shared class tree, alongside `SpecId == activeSpec` entries, not
just the active spec's own entries) is already present in current `main` post-#1498, in
`AscensionClassService` inside `modules/mod-ascension-compat/src/AscensionCompat.cpp` (~line 1350-1355):

```cpp
if (entry.ClassId != player->getClass() ||
    (entry.SpecId != 0 && entry.SpecId != specializationId) ||
    ...
```

No server-side PR is needed for this bug. It is 100% a client-patch change.

## 2. Talent-tree visual desync

**Symptom:** the talent tree shows as if nothing were spent, even though the abilities are learned and
castable, and the server has the correct data (`character_spell`, `character_settings.core.ascension_active_spec`
both correct; the server sends the right SMSG entry list on every login, confirmed via
`ascension_compat_debug.log`). Not specific to one class or character — reported across multiple toons; this
session's reproduction used a Templar/Oathkeeper (class 19) character (Oathkeeper showed only the anchor node;
the class tree showed almost nothing) simply because that's the character on hand, not because the bug is
Templar-specific.

### Two real bugs found and fixed this session (client-side, in `CharacterAdvancementStateCompat.lua`)

Both were reproduced and verified against a pure Lua 5.1 sandbox (Python's `lupa`, no client or server needed)
using the same technique as this repo's own
`modules/mod-ascension-compat/tests/test_automatic_talent_dependencies.py` (which already loads this exact file
with `--client-addon-dir` and exercises pieces of it). That test suite's `test_client_and_server_dependencies_match`
fails identically with or without these fixes (`3618 != 3616`, a pre-existing generated-catalog vs. today's
client-patch drift, unrelated) — no regression introduced.

**Bug A — `freshlyRefundedForSpecSwitch` never clears.** `ResetForSpecializationSwitch()` sets this flag `true`
on every spec switch (a deliberate, documented "don't trust the spellbook while removal packets are still in
flight" guard for `GetInitialRank`), but nothing ever sets it back to `false` except `EnsureStateOwner()`, which
only fires on a **different** `UnitName-Realm` — i.e. a different character, not a relog of the same one. So
after the first spec switch in a client session, every talent entry queried for the first time from then on
renders as unspent, forever, until the client fully restarts.

**Bug B — login/`/reload` race with the spellbook.** `EnsureEntryState` calls `GetInitialRank` **once** per
entry (cached forever in `initializedRanks`). If that first call happens before `IsSpellKnown`/`IsPlayerSpell`
reflect spells the server already restored (plausible right at `PLAYER_ENTERING_WORLD`), it caches a wrong `0`
that never gets re-derived — survives a plain `/reload` since `EnsureStateOwner`'s wipe only resets the
*tracking* tables, not this particular timing assumption.

**Fix for both:** the same pattern — schedule a `C_Timer.After(3, ...)` (confirmed available on this client:
`/run print(C_Timer.After)` → `function: ...`) that clears the stale state a few seconds later and re-derives
everything from the (by-then-settled) spellbook:

```lua
-- in ResetForSpecializationSwitch(), after `freshlyRefundedForSpecSwitch = true; RefreshTalentUI()`:
if C_Timer and C_Timer.After then
    C_Timer.After(3, function()
        freshlyRefundedForSpecSwitch = false
        wipe(pendingRanks); wipe(committedRanks); wipe(initializedRanks); wipe(synchronizedFreeEntries)
        RefreshTalentUI()
    end)
end
```

```lua
-- in EnsureStateOwner(), after the existing wipes (requires forward-declaring
-- `local RefreshTalentUI` above this function, since it's defined further down the file):
if C_Timer and C_Timer.After then
    C_Timer.After(3, function()
        wipe(pendingRanks); wipe(committedRanks); wipe(initializedRanks); wipe(synchronizedFreeEntries)
        if RefreshTalentUI then RefreshTalentUI() end
    end)
end
```

Both were confirmed live: instrumented with temporary chat prints, saw them fire exactly once per real trigger
(one spec switch → one "fired" + one "elapsed" message 3s later; one `/reload` → same pattern), no
data corruption (`character_spell` diffed byte-for-byte against a pre-test backup, identical).

### Why these two fixes do NOT solve the visible symptom

After both fixes landed and were confirmed firing correctly (live, with debug prints), the tree still rendered
exactly as broken as before — even after fully closing and reopening the talent frame, well after the 3-second
settle window. That ruled out both a stale-widget-cache theory and a stale-backing-data theory (the backing
functions were directly sandbox-tested to return the correct rank after settling).

**Confirmed dead end:** grepped every readable client Lua file that touches the talent tree —
`CoATalentFrame.lua`, `EmbeddedCoATalents/CoATreeViewMixin.lua`, `EmbeddedCoATalents/CoASpecViewMixin.lua`,
`EmbeddedTalentUI/TalentTreeBase.lua`, `CoATalentNodeData.lua` — for any call to
`GetPendingRankByEntryID`/`IsKnownSpell`/anything that would read the value our fixes correct. **None of them
call it.** The individual talent-node icons (learned/locked visual state) must be drawn by native code compiled
into `Ascension.exe` itself, not by any Lua file shipped in the MPQs. There is no addon-side or server-side fix
possible for this specific visible symptom without access to that binary.

### My suspicions (not verified — flagging for whoever continues)

- It's likely the actual rank read for node rendering goes through `nativeGetPendingRankByEntryID` (the
  original, un-overridden native function) rather than our `C_CharacterAdvancement.GetPendingRankByEntryID`
  override — i.e. the native UI widget may have grabbed its own reference to the native function before this
  compat file ever runs, making our override invisible to it regardless of what it computes. Worth checking
  load order between `CharacterAdvancementStateCompat.lua`/`CharacterAdvancementCompat.lua` and whatever
  registers the tree's node buttons, if anyone gets access to disassemble or trace the exe.
- The two bugs found and fixed here are real and worth keeping/shipping in the next client patch regardless —
  they're just a different, subtler manifestation (permanently-zeroed entries after any spec switch or a race
  at login) that could show up as its own bug report even once the main visual desync is fixed at the source.
- The comment in `CharacterAdvancementCompat.lua` explicitly says AzerothCore has no implementation of
  "Ascension's private character-advancement service." If that service ever gets a real server-side
  implementation (a bigger project than either of these two bugs), it's possible the entire client-side compat
  shim — and both of today's bugs, and the N-key bug — become moot, since `GetActiveChrSpec()` etc. would then
  be driven by the server as originally designed rather than reconstructed locally. Worth considering as a
  longer-term alternative to patching the compat shim repeatedly.
- Today's community client patch ("September 17 Patch") shipped a new `patch-B.MPQ`/`patch-T.MPQ`. Diffed the
  relevant file (`CharacterAdvancementStateCompat.lua`) against what shipped — byte-identical logic to before,
  so whatever Ascension's client team does next hasn't touched this file yet either.

## 3. Incidental confirmation: PR #1498 alone doesn't fix either bug

Built `main` (`77e98c4f8`) locally from scratch, extracted the real client DBC set with the new
`apps/coa-dbc/client_dbc.py` tool (`mpqcli` needed; a Windows binary isn't vendored — grabbed
`mpqcli-windows-amd64.exe` from `TheGrayDot/mpqcli`'s GitHub releases), installed it into the repack's
`DataDir/dbc`, and confirmed both bugs still reproduce with `CoA_SpecFix` explicitly disabled to isolate the
DBC migration's effect. Both root causes above are unrelated to the DBC migration.

## Build/tooling notes for whoever picks this up

- `mpqcli` supports PKWARE implode (0x08) extraction fine — an earlier session's pure-Python MPQ extractor
  couldn't read `CharacterAdvancementCompat.lua` because of this; that blocker is gone now that `mpqcli` exists.
- Local repack build must link against the **same MySQL client library version** the repack's MySQL server runs
  (8.4.9 here) — `main`'s `CMakeLists.txt`/`FindMySQL` picked up a different `MySQL Server 8.4` install
  (8.4.11) by default, which throws a `[1005]`-style `isSameClientDB` fatal error at connection time until
  `MYSQL_INCLUDE_DIR`/`MYSQL_LIBRARY` are pointed at the matching install explicitly.
- Building with MSVC through a plain shell hits stale x86 `LIB`/`LIBPATH` env vars; run through
  `vcvars64.bat` explicitly rather than relying on the ambient shell environment.
- A sandbox test harness for client Lua state logic (reproduce-then-verify a fix without ever touching a live
  character) is straightforward to build with `pip install lupa` (`from lupa.lua51 import LuaRuntime`) plus
  stub globals for whatever the file under test calls (`IsSpellKnown`, `SendChatMessage`, `C_Timer.After`
  collecting callbacks to fire manually, etc.) — the existing
  `modules/mod-ascension-compat/tests/test_automatic_talent_dependencies.py` already does a version of this and
  is worth extending rather than re-inventing, if this investigation continues.
