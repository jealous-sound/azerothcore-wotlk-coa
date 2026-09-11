# Ascension spell calculations and tooltip parity

Read only the calculation or subsystem sections relevant to the task. These examples are technical references,
not a full verification checklist or a statement of current deployment. Reinspect relevant code/data before reuse.
The official backend is not available as a parity oracle.
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

- Start with existing functional tests. Add a focused regression when the changed behavior needs coverage;
  create a native extraction harness only when existing tests cannot exercise a material risk.
  Validate any necessary test doubles against source; do not copy the expected formula into both sides of a test.
- Select cases relevant to the change: affected ranks, distinct AP/RAP/SP, ownership, timing, duplicate callbacks,
  modifiers, or overflow as applicable. A small correction does not require the entire matrix or other classes.
- Edit generator-owned candidates through their generator. For description changes, check the changed strings
  and preservation of numeric fields. Verify the final changed MPQ member when packaging; do not create a new
  snapshot, byte-restoration harness, or phase manifest merely for a text edit.
- Report coverage precisely: an extracted formula/armor block is not a full hit/crit/proc test;
  a successful build or open window is not combat validation. Keep unresolved contracts explicit.
- Historical diagnostics and input-hash assertions describe their captured release. Keep them out of ordinary
  functional regression selection; preserve existing evidence and numerical assertions, and report stale tooling
  honestly. Routine tests need no new before/after trees, receipts, or permanent output folders.

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

## Pyromancer follow-up lessons

- Prefer the active parent description and native target selector over dormant helper text/amounts.
  Combustion's passive field is five, while Explode and the active passive both specify fifteen
  percent per owned DoT. Wild Magic's mana helper targets the caster; Binding Flames also restores
  the caster's missing mana even though Essence is cast on another ally. Target limits and radii
  are independent of private-effect basepoints: Overwhelming has one recipient within eight yards.
- `Aura::GetScriptValue` is transient. Finite charges can use native saved charges when automatic
  proc consumption is disabled. A few explicitly scoped unused effect slots can hold saved DoT
  snapshots/budgets with recalculation disabled; REAL application after loading must not reset
  them, while genuine REAPPLY can reset a fresh application's extension cap. Preserve the next
  periodic timer when extending/spreading. This is normal save continuity, not a crash journal.
- A resolved copy must not receive the original coefficient/resilience/taken multipliers twice.
  New shields/immunity still need their own policy, and Pyroclasm's transfer of remaining pre-PvP
  periodic damage is a new DoT rather than a copy of an already-resolved hit.
- A correct-looking private effect record is not executable evidence. Cataclysm 520868 still
  dispatched effect 178 to EffectNULL; its scoped dummy binding and owned six-second slow need
  verification. Suppress native summons in both HIT phases, because one phase's prevention does
  not automatically cover the other. A single script-instance guard prevents duplicate creation.
- Helpful-aura dispel resistance belongs to the caster's positive application, not an arbitrary
  aura recipient. Check the application polarity as well as offensive dispel direction so harmful
  auras do not inherit Lava-Drenched protection.

## Cultist follow-up lessons

- A successful spell hit is not proof of a successful dispel. EffectDispel returns early when
  the eligible or successful aura list is empty; attach Devour Magic's heal only after the
  successful removals, with exact class/family/spell scope. Keep native Warlock controls.
- Periodic healing may apply an outgoing healing-percent multiplier inside its tick handler
  even when the usual bonus functions already treat a copied amount as resolved. Vision needs
  a scoped exemption at that native site. Test the actual native branch and ordinary HoTs.
- One shared spell-family bit may cover unrelated active spells. Ward/Hammer/Dark Veil and
  Shadow of the Void/Covenant require reviewed exact modifier routing; do not change all masks.
  Native operation 31 is maximum aura stacks, while ability cast charges are separate state.
- Match summon callbacks to actual ownership and entry. A generic owned-summon set also includes
  portals and clones, which must not qualify for tentacle-only bonuses. Remove an outgoing
  tentacle silence by its caster GUID on death and before world removal, preserving other owners.
- Native creature/model IDs may have been claimed by a preceding package. Cultist Hallucination
  uses new entry 840025 because 840000 belongs to installed Witch Doctor. Appearance copies can
  run random native paths without inheriting another class's attack AI or editing its template.
- Ten ritual participants means ten distinct living group members actively channeling the
  same stone, not ten clicks. Keep native SummonRequest consent for the separate summoning
  object. Model-chain existence, /Zs and bounded fixtures do not establish rendered or group QA.
- Keep uncertain acquisition separate from functioning mechanics: Corrupting Whispers' learned
  proc can be implemented without inventing a free level-10 grant for unresolved identity 4041.

## Sun Cleric follow-up lessons

- A cast charge and a result event have different lifetimes. Preserve Dawn's selected values for
  native hand/channel children and delayed landing without charging again or retaining raw pointers.
  A foreign caster's Dawn does not fulfill the recipient's Vow. The last charged event must retain
  the no-generation rule even after the visible Dawn aura has been removed.
- Absorb depletion must use the actual AFTER_ABSORB amount, after native bypass, and before the
  engine subtracts remaining capacity. Dispel/expiry of unused capacity is not depletion. An area
  split-damage contract also needs a target count; a generic-family native meteor branch does not
  automatically apply to custom spell family 33.
- Preserve native duration/spellmod behavior when it already matches the contract. Halberd's
  native mask covers both Champion and Chains; adding another script bonus would double Champion.
  Test gear and finite-cast selectors with wrong families and unrelated targets.
- Tooltip symbols matter even for a heal: Radiance uses SP, while Illumination uses bonus healing.
  Holyfire damage does not imply that an explicit Holy-power coefficient can read Fire power.
  Use different test values for damage power, healing power and schools to expose these mistakes.
- A private aura-removal effect must be suppressed in its actual HIT phase. Suncharge clears by
  caster GUID, so it must not also run a native removal that clears another cleric's stacks.
- Portal model candidates need inspection: display 19283 is a practice dummy. Sun Gate uses
  existing Shattrath portal 23719 as an explicit local substitute; dependency resolution and
  controlled movement fixtures do not establish rendered appearance or live navigation.

## Venomancer completion, 2026-09-10

Source package: `runtime/validation/venomancer-completion-20260910`; new SQL14 only. See its
190 individual dispositions and `modules/mod-ascension-compat/docs/venomancer-completion.md`.
Preserve the pending Pyromancer, Cultist and Sun Cleric packages and current native-file launcher.

- Distinguish an obsolete zero-flag proc field from a functioning native effect: Venomancy Expert
  already supplies its five-percent discount. Do not add a second one.
- Resource acquisition and spell replacement can have different gates. Fang's Brood grant uses
  specialization 92143; Widow's Kiss 807600 owns only the third-cast replacement. A new gain row
  also requires updating the constexpr array size. Kiss's native Fang modifiers need an exact
  selector extension, retaining the original eligibility checks and replacement mask.
- Saved periodic dummy fields must load before a periodic effect reads them. Reconstruct phase
  from elapsed duration, retain finite charges and carry original crit/percentage snapshots when
  spreading a HoT. Scope this correction to the intended spell family.
- Sepsis represents nominal future damage, including remaining Withering growth. It must retain
  target mitigation, unlike an echo of already resolved damage; dispel and death do not release it.
- An invisible creature display is not evidence of mushroom appearance. The native Putrid Mushroom
  visual supplies the actual asset. Likewise 800389's raw 25622 is BloodElfFemale, not a Nerubian;
  preserve base forms until a trustworthy augmented model is identified.
- Regeneration must pay all current-stack costs before healing. Barbed Stinger's global HIT phase
  may have no target and must not consume the action before the target HIT embeds or rips it.
- Native /Zs and fixture checks are separate from linking, startup registration, and live gameplay.
  State which checks ran; a lint refresh is not a regression run. Routine work needs no phase archive.


## Tinker completion, 2026-09-10

The pending Tinker policy is `modules/mod-ascension-compat/docs/tinker-completion.md`; individual
dispositions are in `runtime/validation/tinker-completion-20260910/findings.md`. Source reconstruction
does not establish linked runtime behavior or official parity. Nanobot Reconstruction scaling remains
unresolved; keep its current first/higher-rank coefficients until evidence establishes a stat term.

- Removing a temporarily learned channel spell can cancel its live aura. Spending Gear Grind's proc
  must preserve the replacement until the channel finishes. Test the native removal relationship.
- A zero-mask aura 271 cannot implement the intended owned Fire vulnerability. Napalm/Oil Pylon use
  explicit owner-scoped routing; do not leak a victim modifier to foreign casters or add it twice.
- Five new permanent pets require the exact custom-AI selector exception; the general PetAI path
  otherwise wins. Owner-based stat formulas also require a scoped native Guardian branch to prevent
  duplicate Stamina/Intellect/armor/AP inheritance. Check the actual native hit and crit consumers.
- Persist a cross-recipient Module selection in a normal saved, non-recalculated effect amount. An
  in-memory map alone cannot reconcile a different recipient after ordinary logout/login.
- Verify the actual DB schema before accepting an isolated SQL fixture: action slots live in
  creature_template_spell with the composite CreatureID/Index key. A fixture copied from a proposed
  migration can falsely approve nonexistent columns. Check against the actual schema and correct the proposal.
  Reject conflicting or extra action slots before installation; retaining failed proposals is not a routine requirement.
- Retained Tinker proc updates use HitMask 9283 (normal/crit/block/absorb/full block). 9331 would also
  admit dodge/parry. Exercise all native hit bits and retain the scripts' positive-damage filters.
