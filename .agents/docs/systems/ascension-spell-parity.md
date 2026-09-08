# Ascension spell calculations and tooltip parity

Reusable lessons verified in this local fork through 2026-09-07. Reinspect the current code and effective
data before applying them to another spell. The official backend is not available as a parity oracle.
Acquisition, displayed values, server mechanics and observed combat are separate verification layers.

## Establish the calculation contract first

1. Pin the active ability ID/rank, its visible description and the effective copied-client/server
   records. Resolve MPQ precedence, SQL overrides, rank chains and scripts; do not assume a matching
   source DBC is the file actually installed. Identical DBC fields do not prove implemented mechanics.
2. Follow the parent ability through every triggered hit, heal and periodic helper. Record effect
   slot/type, forwarded value, count of hits/ticks, school, damage class, equipment mask, stat source,
   coefficient and timing. A periodic aura can trigger a direct-damage child with a direct coefficient.
3. Compare active-rank descriptions with hidden-helper descriptions, not just matching names. A helper
   can carry stale text: Berserker Axe's active ranks specify 40% RAP per axe, while helper 806960's
   hidden text says 35%. Preserve conflicts as evidence; corroborate before choosing a contract.
4. Check `SpellMgr::GetSpellBonusData`: lookup is exact ID first, then the first rank. Inspect both
   and verify the rank chain. Missing a row for a higher rank does not establish a missing coefficient.
5. Separate flat spell damage/healing from weapon-percent and normalized-weapon effects. Weapon
   calculations already incorporate weapon/AP terms. Do not add a second coefficient merely because
   `spell_bonus_data` is empty. Maiming Spear ranks 2-7 needed a description correction, not extra AP.

## AP, RAP and SP are not interchangeable

- In the reviewed damage path, effect `BonusMultiplier` and SQL `direct_bonus`/`dot_bonus` supply
  spell-power scaling. SQL `ap_bonus`/`ap_dot_bonus` supply attack-power scaling separately.
  An existing SP coefficient does not implement an AP term. Zeroing a SQL SP coefficient changes
  behavior and requires its own justification; do not copy the Barbarian zero-SP policy globally.
- By default the damage path selects RAP only when `IsRangedWeaponSpell()` is true **and** `DmgClass`
  is not `SPELL_DAMAGE_CLASS_MELEE`. The default-false `UseRangedAttackPowerForDamage` runtime field
  can override only the two damage coefficient stat selectors. Its sole current exact metadata
  opt-in is Tinker Combustion helper 801388 (SQL38, source-tested and not deployed). Healing, hit,
  weapon, range and proc classification do not read the field. Preserve native per-victim AP
  bonuses, coefficient modifiers and LAUNCH_TARGET sampling. Inspect family/flags, equipment mask
  and `SPELL_ATTR0_USES_RANGED_SLOT`; names, animations and range alone do not select the stat.
- Evaluate the metadata of the child doing damage, not just its parent. Rush helper 560519 has
  `SPELL_DAMAGE_CLASS_NONE` and no ranged mask: a plain SQL AP coefficient would choose the wrong stat.
  Do not change damage class/equipment flags solely to redirect AP; these affect other mechanics.
- Native bonus calculation also includes victim AP modifiers, stack count and spell modifiers.
  A custom RAP callback is not automatically equivalent to that entire path. Specify and test the
  intended modifiers, snapshot/recalculation timing and healing path independently.
- Prefer native coefficient data when the native path matches the contract. Use a narrowly guarded
  callback only for a demonstrated mismatch; preserve unrelated flags, resource costs and proc timing.

## Preserve forwarded values and apply a bonus once

- Inspect `SpellEffectInfo::CalcValue`, `CalcBaseValue`, the triggering effect handler and
  `Spell::SetSpellValue`. Raw `BasePoints` are not always a final amount: the reviewed Axe helper has
  BP=15, DieSides=1, yielding 16 before bonuses. `CalcBaseValue` subtracts one when DieSides is nonzero.
- When modifying a forwarded value, preserve its rank contribution and the setter's encoding.
  Rush's guarded DieSides=1 case compensates the setter's subtraction exactly once. Do not generalize
  this to random dice, level-scaled effects or negative amounts without following their calculation.
- Choose the real result-producing stage: do not add the same coefficient to both parent and child,
  or treat a coefficient per hit as a total shared between hits. Two Axe children each receive 40% RAP.
- Guard custom callbacks by exact IDs, caster/class, triggered state and expected effect metadata.
  Check finite/range-safe arithmetic and duplicate invocation. Recheck native coefficient presence
  to prevent a future SQL migration and a callback from both adding AP.
- `TryMarkScriptEventHandled` event indices are shared state, not private to a new file. Inspect all
  consumers before allocating one; index 17 is already used by Barbarian Rush. Verify hook order
  relative to validation, launch, target selection and damage, rather than inferring it from the name.

## Tooltip versus observed damage

Do not infer exact armor mitigation from floating combat text alone. Capture rank, attacker level,
AP/RAP, buffs/talents, target identity/effective armor, hit/crit/block/absorb and separate hit events.
Distinguish the promised pre-mitigation amount from the final damage after native modifiers/rounding.

The verified illustration is `16 + 0.4 * 45 = 34` per Axe before defense. In the isolated native armor
calculation, level 15 and armor 150 produce 32, not 34. This is a controlled example, not a measurement
of the user's character. The original 15/15 report was compatible with 16 raw damage but did not prove
the target's armor or reconstruct the two events.

## Verification that can be reused

- Extract current native calculation functions/blocks into bounded harnesses where practical. Verify
  stub constants, enum masks, rank lookup, signed fields and encodings against source; wrong test
  doubles can invent a failure. Do not rewrite the expected formula into both sides of the test.
- Cover all relevant ranks/slots, distinct AP and RAP, zero/nonzero SP, modifiers, helper counts,
  periodic timing, repeated callback calls, wrong spell/class/NPC, existing SQL bonus and overflow.
  Include a negative control that recreates the original defect and asserts the intended failure.
- Test client description-only edits by restoring changed offsets/header fields and comparing all
  original bytes. Preserve old shared strings and numeric fields. Re-extract the packaged member and
  verify its hash; edit generator-owned candidates through their generator, not live-only files.
- Report coverage precisely: an extracted formula/armor block is not a full hit/crit/proc test;
  a successful build or open window is not combat validation. Keep unresolved contracts explicit.
- Keep pinned before-evidence immutable. A diagnostic asserting missing coefficients should reject
  the installed fix, not regenerate the old conclusion from changed state.

## Stationary summons and formation bundles

Lessons from the Guardian correction; these are source/test findings, not a claim of in-game parity.

- A summon requires the creature template, template-model link, server display/model DBC records
  and an actual client asset. A valid summon spell or a separately triggered self buff proves none
  of these. Validate new model rows against the effective copied-client DBC, not just a datamine.
- Inspect the entire native summon path. SummonProperties 61 takes the stock Guardian path, which
  starts following the owner after `IsSummonedBy`; setting idle movement only in that callback is
  insufficient. The local standard implementation uses a stationary timed TempSummon instead.
- Native effect 64 (trigger spell) runs at launch/launch-target, while the reviewed summon effect
  runs at hit. A hit-only trigger suppression is too late. Target selection precedes these hooks:
  an owner-GUID Reclaim script also needs a scoped self-target correction to avoid the old area scan.
- Keep only GUIDs in cross-map summon bookkeeping. Create the replacement successfully before
  removing the previous summon, validate owner and entry, and make an old AI destructor erase the
  mapping only if it still points to that old summon. Preserve the old standard if creation fails.
- Keep a stationary area aura's caster as its summon so native range/ownership/cleanup stay centered
  there. Resolve the living owner on each custom tick for explicit STR/AP terms and original-caster
  credit; suppress the replaced default tick and prevent a second native coefficient. Test owner
  death, logout/map changes, foreign summons, replacement and manual recall separately.
- A formation with a negative damage-done effect can be classified as harmful even when its overall
  contract is a buff. Preserve the penalty and narrowly override polarity. Cancellation is a second
  contract: distinguish the main aura, hidden stance/stat helpers and visual helpers, and test their
  whole-bundle cleanup rather than only disappearance of one icon.
- Native aura 142's armor path did not honor Guardian Tower's shield subclass in MiscValueB. Check
  the actual stat bucket, not just that field's presence. The scoped fix adds the equipped, intact
  shield's armor contribution before existing armor modifiers and recalculates on apply/removal;
  cover broken, removed and wrong-type offhands as well as other-class negative controls.
- A native area effect can intentionally leave a short independently timed debuff after its last
  refresh (Valiance's slow is 2.2 seconds). Distinguish that expiration from an orphaned permanent aura.

## Weapon helpers and cosmetic ammunition

- Effect 121 (`NORMALIZED_WEAPON_DMG`) adds a **flat** value to normalized weapon damage;
  a stale hidden description containing `% Weapon Damage` does not change that calculation.
  Wild Strike's shared off-hand helper 560962 had BP 318 / DieSides 1: +319 on every rank,
  then the native 0.5 off-hand modifier made that +159. Audit every incoming trigger and all
  parent ranks before changing a shared helper. The local correction keeps normalized weapon
  damage and removes that flat term, matching the visible parent; official combat parity is unmeasured.
- Cosmetic source-item display IDs, creature entries, CreatureDisplayInfo IDs and ItemDisplayInfo
  projectile IDs are different namespaces. Some numbers coincide; neither equality nor inequality
  proves the join. The copied native loader at VA 0x4EAB50 reads ItemDisplayInfo DWORDs 2 and 4
  (second model/texture), requires both strings, and prefixes `Item\ObjectComponents\Ammo\` for
  inventory type 25. Read the effective client DBC, not the stock server ItemDisplayInfo table.
- Ammunition previews often use `_camera` meshes; flight meshes and their authored ItemDisplayInfo
  records already exist separately. Match basename **and skin**, retaining distinct colors.
  The Ranger first-fix maps all 43 to existing native rows and changes only auto-attack packet
  cosmetics for bows, guns and crossbows. Abilities with their own missile, wands and thrown weapons
  deliberately remain unchanged. Test reset, ownership, visibility and separate player states.
- Direct `SetModel` cannot supply an external creature-display skin. The nine reviewed preview
  clones bind texture type 11 to the actual native type-0 filename, keep geometry/animation bytes
  unchanged, and copy the required `.skin` dependencies under the new model basename. Do not
  mark arbitrary skinned models as standalone. A self-textured model can still fail the native
  projectile loader when its required ItemDisplayInfo texture filename is missing (Elven Grace).

## Local examples

- Private spellmod indices need their own reviewed semantics. Cinder and Ashes 707317 is
  aura 107 (flat), operation 41: its amount 25 adds 0.25 coefficient points. Percentage aura
  108 forms elsewhere are a different contract. A masked override selector avoids native
  fixed-size spellmod arrays/packets. Weapon-effect spells use MeleeDamageBonusDone, so a
  school-damage coefficient fix alone cannot implement this talent. Preserve normal
  BONUS_MULTIPLIER, level penalty and final damage modifiers on both paths.
- Read effective metadata after native corrections. LoadSpellCustomAttr removes NORMAL from
  mixed NORMAL/magic masks and sets SCHOOLMASK_NORMAL_WITH_MAGIC before module hooks. Purifier
  helpers with raw mask 5 therefore execute the Fire weapon path. Applying a blanket physical
  off-hand factor from that raw mask would change their contract. Hybrid resistance is separate.
- Effect 142 already dispatches native trigger-with-value at launch/launch-target. Check its
  explicit-target routing to avoid double triggers; the native setter may encode all three
  custom basepoint values differently according to each helper effect's die sides. Preserve
  parent rank values and weapon hand. Exact rank coefficient overrides may be necessary when
  later ranks differ: native spell_bonus_data lookup falls back to the first rank, not the
  nearest preceding override. These lessons are source/harness evidence, not world combat QA.

- `C:/Ascension/tools/Audit-BarbarianDamage.py` — pinned pre-fix contract audit, not a post-fix health check.
- `C:/Ascension/tools/Test-BarbarianDamageFix.py` — native block and x86/x64 launcher fixtures.
- `C:/Ascension/tools/Generate-BarbarianDamageFix.py` — policy-owned SQL and description-only DBC generation.
- `C:/Ascension/azerothcore-wotlk-coa/modules/mod-ascension-compat/src/AscensionBarbarianScaling.cpp` — guarded RAP exception.
- `C:/Ascension/runtime/validation/barbarian-damage-20260906/implemented.md` — deployment facts and limits.
- `C:/Ascension/tools/Test-GuardianStandards.py` — actual-source summon/scaling/armor cases and model checks.
- `C:/Ascension/azerothcore-wotlk-coa/modules/mod-ascension-compat/src/AscensionGuardianStandards.cpp` — stationary standards.
