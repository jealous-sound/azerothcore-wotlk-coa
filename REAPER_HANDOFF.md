# Reaper sweep handoff (branch `fix/reaper-sweep`)

This covers 89 open Reaper issues on jealous-sound/azerothcore-wotlk-coa (78 automated audit reports and 11
player reports). Nothing was compiled or run in game in this sandbox (see Verification).

## Summary

| Group | False positive | Real bug, fixed | Real bug, not fixed | Needs in-game check | Other |
|---|---|---|---|---|---|
| Audit reports (78) | 53 | 5 (#494, #1135, #1339, #2663, #3335) | 4 (#2669, #2684, #3096, #3493) | 16 | - |
| Player reports (11) | - | 1 fixed (#4244), 1 partial (#4005) | 3 (#4227, #5351, #5361) | 2 (#4902, #5353) | 2 already fixed in main (#1492, #4010), 2 not Reaper bugs (#4313, #5377) |

Notes on the evidence:
- **The Ascension references were unreachable.** `ascension-db.ascension-archive.workers.dev` and
  `ascension-archive.vercel.app` both returned 403 at the sandbox egress proxy (curl and WebFetch). There was
  also no client or server DBC set in the sandbox. Verdicts therefore rest on the DBC effects quoted in each
  audit report, the tooltips, and the repo's code, SQL and data tables. No ascension-db links could be checked.
- **The audit tool mislabels aura numbers.** For example, aura 79 is `MOD_DAMAGE_PERCENT_DONE` (not "school
  immunity") and aura 87 is `MOD_DAMAGE_PERCENT_TAKEN` (not "power regen"); see `SpellAuraDefines.h`. Most
  "Unknown Aura (N)" entries are ordinary generic auras (101, 112, 122, 123, 137, 220, 290, 344).
- **Reaper PROC_TRIGGER_SPELL talents ship with DBC ProcTypeMask 0.** This is documented in the
  `rev_20260920_17` header. A 42-aura talent with no `spell_proc` row therefore probably never fires; this
  is how #1135, #2684 and #3493 were flagged.

## (a) Issues

| # | Title | Verdict | Evidence | Commit |
|---|---|---|---|---|
| [#473](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/473) | Harvesting Grounds (705413) | NEEDS IN-GAME CHECK | Effect 2 is PERSISTENT_AREA_AURA (27) with aura 192 MOD_MELEE_RANGED_HASTE: the haste slow is generic. The movement slow and the pull-back to the centre are not in the reported effects, and nothing in the tree references 705413. Check whether the DBC has another effect for them. | - |
| [#494](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/494) | Soulstorm (705403) | REAL BUG | Proc row and rule already exist (`data/sql/updates/pending_db_world/rev_20260920_17_reaper_talent_procs.sql:112-123`, `AscensionReaperTalentProcs.h:38`), but the row had Chance 100 against the tooltip's 40%. Fixed in `rev_20260927_95`. | c79ca0f |
| [#495](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/495) | Life Tap (706788) | NEEDS IN-GAME CHECK | Aura 4 DUMMY with no handler: nothing references 706788. The cooldown part is probably a separate spell-mod effect. The 'heals 200% of the damage it deals' part is unimplemented as far as the tree shows, but the Spectral Warden damage spell ID needs DBC before it can be scripted. | - |
| [#1024](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/1024) | Soulforged Weaponry (561127, 561340) | FALSE POSITIVE | Aura 122 MOD_OFFHAND_DAMAGE_PCT is generic. The 8% free-Murder proc has rows for 561127 (Chance 8) and 561340 (Chance 15) in `data/sql/updates/pending_db_world/rev_20260921_20_reaper_talent_procs_two.sql:101-140`, plus the rule list in `AscensionReaperTalentProcs.h`. | - |
| [#1135](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/1135) | Soulbender (804004) | REAL BUG | PROC_TRIGGER_SPELL with no spell_proc row; Reaper proc talents ship with ProcTypeMask 0 (`rev_20260920_17` header). Added a row keyed like Chasing Death (family 36, mask0 0x4) in `rev_20260927_96`. | 419b1eb |
| [#1339](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/1339) | Dominion (803999) | REAL BUG | PROC_TRIGGER_SPELL with no row. Its event (gaining Soul Infusion) has no proc flag, so `ApplyAscensionReaperSoulInfusionGained` now casts the talent's own DBC trigger spell (`AscensionReaperTalents.cpp`). | fef4d10 |
| [#1372](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/1372) | Apparition (705389) | FALSE POSITIVE | aura 108 (ADD_PCT_MODIFIER) is a DBC spell modifier applied by the core; no script needed (+30% Spectre Stride). The stealth-detection rider would be a separate effect. | - |
| [#1640](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/1640) | Chancing Death (300550) | NEEDS IN-GAME CHECK | Aura 23 PERIODIC_TRIGGER_SPELL is generic. Whether the trigger spell enforces 'below 35% health' (aura state / caster condition) needs DBC or an in-game check. 300550 is not in the spellbook tree data, so check that it can be acquired. | - |
| [#1641](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/1641) | Cursed Rosary (300551) | FALSE POSITIVE | The audit mislabels aura 79: in 3.3.5 it is MOD_DAMAGE_PERCENT_DONE (`SpellAuraDefines.h:142`), which matches '+3% Physical and Shadow damage'. aura 79 has a generic core handler in `SpellAuraEffects.cpp`. | - |
| [#1642](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/1642) | Grim Skin (300552) | FALSE POSITIVE | The audit mislabels aura 87: it is MOD_DAMAGE_PERCENT_TAKEN (`SpellAuraDefines.h:150`), which matches '-3% damage taken'. Generic handler. | - |
| [#1875](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/1875) | Mark of Terror (500355) | FALSE POSITIVE | Aura 7 is MOD_FEAR (`SpellAuraDefines.h:70`), a generic fear. | - |
| [#1876](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/1876) | Crimson Harvest (500358) | NEEDS IN-GAME CHECK | Aura 3 PERIODIC_DAMAGE and the heal-back are generic. The rider 'Soul Infusion: increased tick rate' has no handler for 500358 in the tree. | - |
| [#1909](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/1909) | Dark Fate (504037) | FALSE POSITIVE | aura 107 (ADD_FLAT_MODIFIER) is a DBC spell modifier applied by the core; no script needed (cost/cooldown). | - |
| [#2027](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2027) | Essence Binder (560919) | FALSE POSITIVE | Proc row and rule exist: `data/sql/updates/pending_db_world/rev_20260921_20_reaper_talent_procs_two.sql:87-98`, `AscensionReaperTalentProcs.h:28`. | - |
| [#2036](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2036) | Mysterious Omen (561108) | FALSE POSITIVE | aura 107 (ADD_FLAT_MODIFIER) is a DBC spell modifier applied by the core; no script needed (range). | - |
| [#2038](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2038) | Spectre (561170) | FALSE POSITIVE | The Underwalk cooldown is a spell mod; 'Withering Touch always applies Ghost Claw' has a proc row and rule (`data/sql/updates/pending_db_world/rev_20260921_20_reaper_talent_procs_two.sql:115-126`, rule 561170 -> 573071). | - |
| [#2050](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2050) | Into The Shadow Realm (561342) | FALSE POSITIVE | aura 108 (ADD_PCT_MODIFIER) is a DBC spell modifier applied by the core; no script needed (cooldown %). | - |
| [#2149](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2149) | Soul Bane (800935, 705392) | FALSE POSITIVE | Aura 107 is a spell modifier: SPELLMOD crit chance for the aura's duration. Generic. | - |
| [#2156](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2156) | Spirit Render (570338) | FALSE POSITIVE | Aura 123 MOD_TARGET_RESISTANCE is spell penetration. aura 123 has a generic core handler in `SpellAuraEffects.cpp`. | - |
| [#2169](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2169) | The Time Has Come (572879) | FALSE POSITIVE | Converted to MOD_DAMAGE_DONE_VERSUS_AURASTATE (slowed) by `ApplyReaperTheTimeHasComeDamageContract` (`AscensionConditionalCombat.cpp:44`). | - |
| [#2170](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2170) | Shade (573038) | NEEDS IN-GAME CHECK | Aura 56 is TRANSFORM (generic); the stealth and Underwalk parts are separate effects. Nothing in the tree references 573038, so entering Underwalk is unverified (review: changed from FALSE POSITIVE). | - |
| [#2171](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2171) | Extinction (573039) | FALSE POSITIVE | Proc row and script `spell_ascension_reaper_extinction` (`rev_20260920_11_reaper_extinction_proc.sql:19-32`); 5% + 10% per soul in `AscensionCompat.cpp` (BaseChance/ChancePerSoul). | - |
| [#2172](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2172) | Behemoth (573041) | FALSE POSITIVE | Aura 137 MOD_TOTAL_STAT_PERCENTAGE (+20% Stamina) is generic. The ArP-from-Strength part would be another effect (not in the report). | - |
| [#2177](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2177) | Withering Touch (573071) | FALSE POSITIVE | Aura 101 MOD_RESISTANCE_PCT (-20% armor) is generic. The +10% magic taken is a separate effect. | - |
| [#2223](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2223) | The End Is Near (680996) | FALSE POSITIVE | Proc row and rule exist: `data/sql/updates/pending_db_world/rev_20260921_20_reaper_talent_procs_two.sql:143-154`, rule 680996. | - |
| [#2257](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2257) | Decimation (704193) | FALSE POSITIVE | Proc row and rule exist: `data/sql/updates/pending_db_world/rev_20260920_17_reaper_talent_procs.sql:70-81` (flags widened in `data/sql/updates/pending_db_world/rev_20260921_20_reaper_talent_procs_two.sql:257`). The 'every 6th cast' counting relies on the trigger spell's stacks, which nothing in the tree checks; the row fires on every Reap cast (Chance 100). Confirm in game (review: changed from FALSE POSITIVE). | - |
| [#2305](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2305) | Soulrunner (704547) | FALSE POSITIVE | aura 107 (ADD_FLAT_MODIFIER) is a DBC spell modifier applied by the core; no script needed (Scythe Rush same-target lockout). | - |
| [#2308](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2308) | Doomer (704556) | FALSE POSITIVE | aura 108 (ADD_PCT_MODIFIER) is a DBC spell modifier applied by the core; no script needed. | - |
| [#2647](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2647) | Spirit of Savagery (705387) | FALSE POSITIVE | Aura 344 is SPELL_AURA_ASCENSION_MOD_ATTACK_POWER_FLAT, handled by `HandleAscensionModAttackPowerFlat` (`SpellAuraEffects.cpp:409`). | - |
| [#2648](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2648) | Wicked Shadows (705390, 705391) | FALSE POSITIVE | aura 108 (ADD_PCT_MODIFIER) is a DBC spell modifier applied by the core; no script needed (both 705390 and 705391). | - |
| [#2649](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2649) | Soul Eater (705393) | FALSE POSITIVE | aura 107 (ADD_FLAT_MODIFIER) is a DBC spell modifier applied by the core; no script needed (Soul Shock debuff value). | - |
| [#2650](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2650) | Soul Inferno (705394) | FALSE POSITIVE | aura 107 (ADD_FLAT_MODIFIER) is a DBC spell modifier applied by the core; no script needed (Purgatory value). | - |
| [#2651](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2651) | Spectral Reaver (705396) | FALSE POSITIVE | aura 107 (ADD_FLAT_MODIFIER) is a DBC spell modifier applied by the core; no script needed (Murder chain targets). | - |
| [#2656](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2656) | Death Dealer (705401) | FALSE POSITIVE | aura 107 (ADD_FLAT_MODIFIER) is a DBC spell modifier applied by the core; no script needed (duration). | - |
| [#2659](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2659) | Harnessed Life (705406) | NEEDS IN-GAME CHECK | The aura is a spell modifier, but 'Tormented Souls has a 25% chance not to consume a stack' depends on Tormented Souls stack consumption, which is not implemented (see #5361). | - |
| [#2661](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2661) | Ravenous Thirst (705409) | FALSE POSITIVE | aura 107 (ADD_FLAT_MODIFIER) is a DBC spell modifier applied by the core; no script needed (Runic Power cost). | - |
| [#2663](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2663) | Crimson Death (705414) | REAL BUG | Proc row and rule exist (`data/sql/updates/pending_db_world/rev_20260920_17_reaper_talent_procs.sql:126-137`), but the row had Chance 100 against the tooltip's 20%, so every Slaughter repeated for free. Fixed in `rev_20260927_95`. | c79ca0f |
| [#2669](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2669) | Anima Ambusher (705424) | REAL BUG | Aura 354 is an unknown Ascension aura with a nullptr handler (`SpellAuraEffects.cpp:419`). It is not in `isTriggerAura`, and nothing references 705424. Other classes script each aura-354 talent individually. Not fixed: this needs the trigger spell and the Spectre Stride damage IDs from DBC. | - |
| [#2670](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2670) | Ghost (705426) | FALSE POSITIVE | Proc row and rule exist: `data/sql/updates/pending_db_world/rev_20260921_20_reaper_talent_procs_two.sql:185-196`, rule 705426 -> 805185. | - |
| [#2671](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2671) | Souls for the Slaughter (705428) | FALSE POSITIVE | Proc row with crit HitMask exists: `data/sql/updates/pending_db_world/rev_20260920_17_reaper_talent_procs.sql:140-151`. | - |
| [#2677](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2677) | Spiritual Reflexes (705436) | FALSE POSITIVE | Scripted: `aura_ascension_spiritual_reflexes` + spell_proc (`rev_20260914_14_mountain_and_reflexes.sql:5-12`), `Unit.cpp:4059`. | - |
| [#2678](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2678) | Mortal's End (705437) | FALSE POSITIVE | Proc row and rule exist: `data/sql/updates/pending_db_world/rev_20260921_20_reaper_talent_procs_two.sql:199-210`. The extra Extinction stack is a spell mod. | - |
| [#2682](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2682) | Ghostly Fortitude (705446) | FALSE POSITIVE | Aura 137 MOD_TOTAL_STAT_PERCENTAGE (+5% Stamina) is generic. | - |
| [#2683](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2683) | Sadism (705447) | FALSE POSITIVE | aura 108 (ADD_PCT_MODIFIER) is a DBC spell modifier applied by the core; no script needed. | - |
| [#2684](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2684) | Mind Screech (705448) | REAL BUG | PROC_TRIGGER_SPELL with no spell_proc row or script. Given the ProcTypeMask 0 pattern it probably never fires. Not fixed: the Shrieking Scythe spell IDs are not in the tree, and there is no DBC in the sandbox. | - |
| [#2685](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/2685) | Torturing Scythe (705449) | FALSE POSITIVE | aura 107 (ADD_FLAT_MODIFIER) is a DBC spell modifier applied by the core; no script needed (heal amount / next Soulrend). | - |
| [#3073](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3073) | Bountiful Harvest (706576) | FALSE POSITIVE | aura 108 (ADD_PCT_MODIFIER) is a DBC spell modifier applied by the core; no script needed. | - |
| [#3095](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3095) | Dark Soul (706784) | NEEDS IN-GAME CHECK | Aura 220 MOD_RATING_FROM_STAT is generic, but it grants rating from a stat, which is the reverse of the tooltip ('Stamina equal to ArP rating'). Check the stat sheet in game. | - |
| [#3096](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3096) | Unbound (706785) | REAL BUG | Aura 112 OVERRIDE_CLASS_SCRIPTS does nothing on its own (`HandleNoImmediateEffect`). 706785 is not in `AscensionConditionalCombatRules`, and no contract converts it. Not fixed: a conversion rule needs the exact DBC effect record (misc values, mask). | - |
| [#3097](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3097) | Well of Eternity (706793) | FALSE POSITIVE | aura 107 (ADD_FLAT_MODIFIER) is a DBC spell modifier applied by the core; no script needed (duration). | - |
| [#3098](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3098) | Empyrean Fortitude (706795) | FALSE POSITIVE | Aura 47 MOD_PARRY_PERCENT is generic. The +5 RP on parry has a row: `data/sql/updates/pending_db_world/rev_20260921_20_reaper_talent_procs_two.sql:213-224` (HitMask 32). | - |
| [#3122](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3122) | Blood Wraith (707305) | FALSE POSITIVE | aura 107 (ADD_FLAT_MODIFIER) is a DBC spell modifier applied by the core; no script needed. | - |
| [#3145](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3145) | Spirit Army (707634) | NEEDS IN-GAME CHECK | Spell mod. Whether a spell-mod on the summon count gives 2 extra Spectral Wardens depends on the summon path applying mods; check in game. | - |
| [#3161](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3161) | Blood Frenzy (707899, 803039) | FALSE POSITIVE | Scripted: `aura_ascension_reaper_blood_frenzy` (`AscensionReaperTalents.cpp`, `rev_20260921_21_reaper_blood_frenzy.sql`). | - |
| [#3164](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3164) | Soul Piercer (712484) | FALSE POSITIVE | Proc row and rule exist: `data/sql/updates/pending_db_world/rev_20260920_17_reaper_talent_procs.sql:182-193`, rule 712484. | - |
| [#3165](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3165) | Fleeting Soul (712683) | FALSE POSITIVE | aura 107 (ADD_FLAT_MODIFIER) is a DBC spell modifier applied by the core; no script needed (cooldown). | - |
| [#3334](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3334) | Spectral Waltz (803988) | NEEDS IN-GAME CHECK | Aura 4 DUMMY has no handler. The 50% reduction and pacify are probably other effects of 803988 (not in the report). | - |
| [#3335](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3335) | Harvest Time (803995) | REAL BUG | Aura 290 MOD_CRIT_PCT is generic. The Soul Infusion preserve was 100% for the whole buff (`AscensionCompat.cpp` HarvestTimePreserves) instead of 50%. Fixed. | d0c1ed9 |
| [#3337](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3337) | Ghostly Magic (803998, 807889) | FALSE POSITIVE | aura 108 (ADD_PCT_MODIFIER) is a DBC spell modifier applied by the core; no script needed (both IDs). | - |
| [#3338](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3338) | Reaper's Pact (804001) | NEEDS IN-GAME CHECK | Aura 81 SPLIT_DAMAGE_PCT is generic, but it is applied by APPLY_AURA (6), not an area aura, and nothing references 804001, so no code removes it beyond 30 yds unless another DBC effect does (review: changed from FALSE POSITIVE). | - |
| [#3425](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3425) | Spectre Strength (804671, 807886) | FALSE POSITIVE | Aura 137 MOD_TOTAL_STAT_PERCENTAGE is generic. | - |
| [#3490](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3490) | Lord of Death (805194) | NEEDS IN-GAME CHECK | A spell mod cannot express '+25% per additional target'. Nothing references 805194. | - |
| [#3492](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3492) | Ruin (805198) | FALSE POSITIVE | Scripted: `spell_ascension_reaper_ruin` (`AscensionReaperRuin.cpp`, `rev_20260920_13_reaper_ruin_and_redshade_procs.sql`). | - |
| [#3493](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3493) | Blood Binding (805200) | REAL BUG | PROC_TRIGGER_SPELL with no row or script, so it probably never fires. Not fixed: 'Scythe Rush directly after Cull' needs the Cull ID and a sequence check. | - |
| [#3558](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3558) | Shadow Wraith (805721) | FALSE POSITIVE | aura 107 (ADD_FLAT_MODIFIER) is a DBC spell modifier applied by the core; no script needed. | - |
| [#3673](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3673) | Reaver (807419) | FALSE POSITIVE | aura 108 (ADD_PCT_MODIFIER) is a DBC spell modifier applied by the core; no script needed. | - |
| [#3712](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3712) | Soul Tap (807397) | FALSE POSITIVE | ENERGIZE (30) is a generic effect. | - |
| [#3723](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3723) | Masochistic Rage (803030) | FALSE POSITIVE | Effect 183 is SPELL_EFFECT_ASCENSION_TRIGGER_SPELL_DELAYED -> `EffectAscensionTriggerSpellDelayed` (`SpellEffects.cpp:255`). | - |
| [#3724](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3724) | Death's Presence (520371) | FALSE POSITIVE | APPLY_AREA_AURA_RAID (65) with aura 79 MOD_DAMAGE_PERCENT_DONE; the non-stacking group is in `rev_20260922_27_raid_damage_percent_group.sql:14`. | - |
| [#3744](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3744) | Red Wake (801041) | NEEDS IN-GAME CHECK | SCHOOL_DAMAGE (2) is a generic effect. '5 Runic Power for each target struck' is not handled anywhere in the tree (nothing references 801041); it works only if the DBC energize effect targets the caster once per struck enemy (review: changed from FALSE POSITIVE). | - |
| [#3757](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3757) | Shrieker (801332) | FALSE POSITIVE | The audit read effect 0 as 41105, which is not a real effect ID, and the tooltip says '1 Shadow Damage'. This looks like a placeholder or NPC record, and it is not in the spellbook tree data. Nothing actionable. | - |
| [#3781](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3781) | Soulspear (803984) | FALSE POSITIVE | SCHOOL_DAMAGE is generic. Instant-with-Soul-Infusion would be a spell mod from the Infusion aura. Check the cast time in game. | - |
| [#3797](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3797) | Wailing Soul (804677) | NEEDS IN-GAME CHECK | SUMMON (28) is generic, but the possess/stun behaviour and the Underwalk interaction are not scripted. 804677 is not in the spellbook tree, so check that it can be acquired. | - |
| [#3835](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3835) | Soulshroud (805711) | NEEDS IN-GAME CHECK | Effect 174 is an unknown Ascension effect (`EffectNULL`, `SpellEffects.cpp:246`). The 40% reduction is presumably another effect (aura 87); check in game. | - |
| [#3866](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3866) | Sanguine Soldier (704555) | NEEDS IN-GAME CHECK | The report lists no DBC effects, and 704555 is referenced nowhere, not even in the spellbook tree. Check that it can be acquired first. | - |
| [#3895](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3895) | Soulsight (300565) | FALSE POSITIVE | Raid aura 65 with 290 MOD_CRIT_PCT is generic. The 15% Soul Fragment proc has a row and rule: `rev_20260921_20:17-28` (Chance 15). | - |
| [#3905](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3905) | Blood Harvester (300889) | FALSE POSITIVE | Raid aura 65 with 290 MOD_CRIT_PCT is generic. | - |
| [#3913](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/3913) | Ethereal Guard (300969) | FALSE POSITIVE | Raid aura 65 with 87 MOD_DAMAGE_PERCENT_TAKEN is generic (the audit mislabels 87). | - |
| [#1492](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/1492) | Ghost Pig | ALREADY FIXED | `rev_20260916_60_reaper_ghost_form_appearance.sql` adds creature 841213 -> display 5430 (the pig was the 16358 fallback for a missing template). Needs the world DB update applied. Close after an in-game check. | - |
| [#4005](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/4005) | Soul rend does not consume a Soul Infusion, allowing infinite casts. | REAL BUG (partial) | Soulrend 573316 is an all-soul consumer (`AscensionCompat.cpp` REAPER_ALL_SOUL_CONSUMERS, ConsumeReaperSouls). Two things made it effectively free: Harvest Time skipped consumption 100% of the time, and Soulstorm refunded a soul on 100% of hits. Both are fixed. Check whether infinite casts still happen outside Harvest Time. | d0c1ed9, c79ca0f |
| [#4010](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/4010) | Soul Harvester Talent does not give a buff. | ALREADY FIXED | `rev_20260919_31_reaper_soul_harvester_proc.sql` adds a PROC_FLAG_KILL row (Chance 100) for 804311 -> 804312. It only fires on kills that grant XP or honor. Check in game. | - |
| [#4227](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/4227) | Spectral scythe that is a sword and also stuck in the ground while hitting like a wet noodle. | REAL BUG (not fixed) | The creature 250305 from `rev_20260916_04_reaper_spectral_scythe.sql` uses display 25398 (runeblade) on purpose, because the deployed CreatureDisplayInfo has no scythe. It is a plain guardian (petType MAX_PET_TYPE), so `Guardian::InitStatsForLevel` (`Pet.cpp:1152+`) never sets its weapon damage, which gives near-zero hits. The model height needs hover/InhabitType. Needs Ascension's damage formula; see open questions. | - |
| [#4244](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/4244) | Ghost Claw not reseting | REAL BUG | Nothing reset Ghost Claw 803985. Added `aura_ascension_reaper_ghost_claw`: the cooldown resets when the aura is removed by the target's death. Review: talent 504269 replaces 803985 with 807234 (`AscensionTalentReplacementData.h:61`); the script now also covers 807234 (`rev_20260927_98`). | 7a35ae0, a43a9a7 |
| [#4313](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/4313) | patch-B.MPQ client workarounds: inventory, and the server-side levers that can retire them | NOT A REAPER BUG | This is a tracking/meta issue for the client patch-B.MPQ inventory, not a Reaper defect. No server change applies. | - |
| [#4902](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/4902) | Reaper's spell Murder has bugged sound effect | NEEDS IN-GAME CHECK (client) | Nothing on the server re-casts or interrupts Murder 500376 (the only references are resource rules and `AscensionReaperSecondary.cpp:128`). Spell sound comes from client SpellVisual/SoundEntries data, so the cause is very likely client-side. | - |
| [#5351](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/5351) | Can have any talent you want if you just path it first. | REAL BUG (not fixed, all classes) | `SetTalentRank` (`AscensionCompat.cpp:1431`) checks class, spec, rank count, level and budget, but has no tree adjacency or path model at all. `CoATalentEntry` has no connection fields, and CharacterAdvancement's link bytes are still opaque (`apps/coa-dbc/README.md`). This affects every class and needs its own task. | - |
| [#5353](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/5353) | Dreadwake doesn't work | NEEDS IN-GAME CHECK | Soul gain on Dreadwake 503531 is `FirstSuccessfulDamagingHit` by design (`AscensionClassContracts26To32Data.h:51`). No Ascension source was reachable to confirm on-cast gain. For the cone, 503531 has no `spell_cone` row, so it uses the target-type default (60 degrees, `Spell.cpp:1246`). Check against live cone and radius data. | - |
| [#5361](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/5361) | Reaper talent Tormented Souls and Soulfused Constitution not working properly | REAL BUG (not fixed) | Tormented Souls 500483 only applies buff 500481, and consumes all souls as an all-soul consumer. Only Hard Bargain references 500481 (`AscensionReaperSecondary.cpp:32`). Nothing implements per-soul stacks or consume-a-stack-to-heal on damage taken. Needs the stack and heal values and the Soulfused Constitution ID from DBC. | - |
| [#5377](https://github.com/jealous-sound/azerothcore-wotlk-coa/issues/5377) | too much achievement hiting lvl 60 | NOT A REAPER BUG | The achievement system checks REACH_LEVEL criteria against `achievement_criteria_data` (`AchievementMgr.cpp:1023`). There are no class-restriction rows for the CoA class level achievements in the tree, so every class's level-60 achievement completes. Fixing this needs the Achievement_Criteria IDs from the client DBC, plus CLASS_RACE criteria rows. | - |

## (b) Commits

| Commit | Change | Issues |
|---|---|---|
| d0c1ed9 | `fix(CoA/Reaper): Harvest Time preserves Soul Infusion on a 50% roll` (`AscensionCompat.cpp` HarvestTimePreserves) | Refs #3335, #4005 |
| c79ca0f | `fix(CoA/Reaper): Soulstorm and Crimson Death proc at their stated chance` (`rev_20260927_95_reaper_soulstorm_crimson_death_chance.sql`) | Fixes #494, #2663; Refs #4005 |
| fef4d10 | `fix(CoA/Reaper): Dominion grants its armor when Soul Infusion is gained` (`AscensionReaperTalents.cpp` CastOwnProcTrigger) | Fixes #1339 |
| 7a35ae0 | `fix(CoA/Reaper): Ghost Claw cooldown resets when its target dies` (`aura_ascension_reaper_ghost_claw` + `rev_20260927_97_reaper_ghost_claw_reset.sql`) | Fixes #4244 |
| 419b1eb | `fix(CoA/Reaper): Soulbender procs on a landed Deathchaser` (`rev_20260927_96_reaper_soulbender_proc.sql`) | Fixes #1135 |
| a43a9a7 | `fix(CoA/Reaper): Ghost Claw talent replacement also resets on a kill` (review; `rev_20260927_98_reaper_ghost_claw_replacement_reset.sql`) | Refs #4244 |
| (final) | Adds this handoff; removes `.reaper-run/` and `.claude/settings.json` | - |

## (c) Test plan

### Verification

- **Ran:** `python -B tools/verify_all.py --stages source --base origin/main`, which reported `VERIFY ALL: PASSED`
  (source 5/5; build, unit, harness and gameplay were skipped). I also ran `codestyle-cpp.py --files` on
  both C++ files and `codestyle-sql.py --files` on the three SQL files; all were clean.
- **Not run:** the build. The sandbox has no MySQL client libraries, no build tree and only 4 cores.
- **To do on the PC:** run `python -B tools/verify_all.py --base origin/main` (build, unit and harness). Then
  run the focused gameplay runs below, or reproduce them manually with GM commands on a level-80 Reaper
  (class 30) with the world DB updates applied.

### 1. Harvest Time (d0c1ed9)

1. `.learn 803995`, then set up Soul Infusion: `.aura 500363` three times, or cast until `.aura 803031` is up.
2. Cast Harvest Time, then cast a spell that requires Soul Infusion (for example Soulrend 573316) 20 times,
   re-applying 803031 (`.aura 803031`) whenever it is consumed.
3. **Expected:** about half of the casts keep Soul Infusion (803031 is still present). Before the fix, all of
   them kept it.

Note: Soulrend is an all-soul consumer, so it takes the `REAPER_ALL_SOUL_CONSUMERS` branch after the
preserve roll, which means a failed roll consumes the souls too.

### 2. Soulstorm and Crimson Death (c79ca0f)

1. Check the DB: `SELECT SpellId, ProcFlags, Chance FROM spell_proc WHERE SpellId IN (705403,705414);` should
   return `40` and `20`.
2. Soulstorm: `.learn 705403`. Cast Soulrend or Requiem about 30 times with souls available.
3. **Expected:** a Reaped Soul (500363) is refunded on about 40% of hits.
4. Crimson Death: `.learn 705414`. Cast Slaughter about 30 times.
5. **Expected:** the combat log shows a free second Slaughter on about 20% of casts. Before the fix, every
   cast repeated.

### 3. Dominion (fef4d10)

1. `.learn 803999` and confirm the passive is up (`.aura` list).
2. Gain Soul Infusion by building 3 Reaped Souls.
3. **Expected:** the talent's DBC trigger spell (the "Armor +585 for 6 sec" buff) is applied, and armor on
   the character sheet rises by 585 for 6 s.
4. Look up the trigger ID with `coa-dbc-viewer record --id 803999 --field 'Effect*'`, and confirm it fires
   only once per Infusion gain. It should not also fire from a DBC proc flag; if 803999 does ship ProcFlags,
   add a `spell_proc` row with `ProcFlags` 0 or a `DoCheckProc` false.

### 4. Ghost Claw (7a35ae0)

1. `.learn 803985` and cast it on a low-health mob, then kill the mob while it carries the Ghost Claw aura.
2. **Expected:** the Ghost Claw cooldown is cleared at once.
3. Kill a mob without Ghost Claw on it: the cooldown should stay.
4. Repeat with talent 504269 in spec 56, which replaces Ghost Claw with 807234: the 807234 cooldown should reset the same way.
5. **Caveat:** this only works if 803985 itself applies an aura to the enemy. If the debuff is a separately
   triggered spell, move the `spell_script_names` row to that ID. Check with
   `coa-dbc-viewer record --id 803985 --field 'Effect*'`.

### 5. Soulbender (419b1eb)

1. `.learn 804004`, then cast Deathchaser (805190) on a target.
2. **Expected:** the Soulbender trigger spell fires on each landed Deathchaser hit, and Runic Power gained
   per Deathchaser is noticeably higher than without the talent.
3. Confirm Deathchaser's SpellFamilyFlags[0] contains 0x4, which is what Chasing Death's row assumes
   (`rev_20260927_86_reaper_chasing_death.sql`).

## (d) Open questions and in-game checks

- **Spectral Scythe (#4227).** The guardian 250305 needs a damage model. It is a plain guardian, so
  `Guardian::InitStatsForLevel` gives it no weapon damage. The options are:
  - an `OnPlayerBeforeGuardianInitStatsForLevel` hook, like `AscensionStormbringerPet.cpp:81`, that sets
    `petType = SUMMON_PET` or explicit damage from the owner's weapon or AP;
  - `creature_template_movement` Flight/hover so it floats at chest height.

  Pick the numbers from Ascension data.
- **Tormented Souls (#5361) and Harnessed Life (#2659).** These need per-soul stacks on 500481, a
  consume-a-stack heal on direct damage taken, and the Soulfused Constitution ID and values. Souls are
  removed in `ConsumeReaperSouls` at cast time, so the stack count has to be captured before that call.
- **Unscripted procs: Mind Screech (705448), Blood Binding (805200), Anima Ambusher (705424, aura 354).**
  These need Shrieking Scythe, Cull and Spectre Stride IDs and trigger spells from DBC.
- **Unbound (706785).** This needs an `AscensionConditionalCombatRules` entry (misc values and mask from
  the DBC record).
- **Talent path bypass (#5351).** Every class is affected. It needs the CharacterAdvancement node links
  decoded, plus a dependency check in `SetTalentRank` for both learning and unlearning.
- **Class level achievements (#5377).** Add `achievement_criteria_data` CLASS_RACE rows for each CoA class
  level-achievement criterion. The IDs come from the client Achievement_Criteria.dbc.
- **Dreadwake (#5353).** Confirm whether live generates a soul without a hit, and the cone width (there is
  no `spell_cone` row, so it uses the 60-degree default).
- **Items tagged NEEDS IN-GAME CHECK in the table.** For each one, dump the full DBC record
  (`coa-dbc-viewer record --id <id> --field 'Effect*' --with-links`). The audit reports only list one effect
  per spell, so a missing rider may already be carried by another effect.
- **Items not in the spellbook tree data.** 300550-300552, 500355, 500358, 704547, 704555, 705387,
  705390/1, 705393/4/6, 705446-9, 800935, 801041, 801332, 803984, 804001, 804677, 805200, 805711 and
  805721 are not in `SpellbookTreeSpellData.h`. Confirm how players acquire them before spending time on
  them.
- **Measurements.** Time from investigation start to the verified outcome is unknown, since no runtime
  verification was available. No regressions or flaky checks were observed (only the source stage ran),
  and no human corrections were needed.
