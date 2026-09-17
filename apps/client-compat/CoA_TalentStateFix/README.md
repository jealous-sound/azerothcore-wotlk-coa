# CoA_TalentStateFix

Client-side half of the Character Advancement sync. Drop the folder into the
client's `Interface/AddOns/`.

`Ascension_Collections/CharacterAdvancementStateCompat.lua` rebuilds each talent
node's rank from the client's own spellbook:

```lua
if IsSpellKnown then known = SafeCall(IsSpellKnown, spellID) end
if not known and IsPlayerSpell then known = SafeCall(IsPlayerSpell, spellID) end
```

Two kinds of talent spell never reach that spellbook, so the client cannot see
them and draws their nodes as unspent after every relog:

* hidden spells (`SPELL_ATTR0_DO_NOT_DISPLAY`), which is most passives;
* the base rank of a ranked spell, deactivated by `Player::addSpell` as soon as a
  higher rank is learned and therefore skipped by `SendInitialSpells`.

Measured on a level 60 Knight of Xoroth owning 37 talents: the client accounted
for 4.

The server now pushes the truth at login as `CoATalentSync:` lines, and this addon
answers from them. It defines `IsPlayerSpell`, which this client does not have, in
preference to overriding `IsSpellKnown` -- replacing that global taints every
execution path that reaches it, and the default UI calls it constantly, which is
enough to block secure calls such as `WorldMapBlobFrame:Hide()`.

It also wraps `C_CharacterAdvancement.GetActiveChrSpec`, which returns nil here
because the advancement service it belongs to does not exist on AzerothCore, and
reduces item links in dot commands to a bare item id so AtlasLoot's short links
are accepted.
