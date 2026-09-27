Ascension client spell patches: use sparingly.

The original Extensions.dll already accepts SMSG_PATCH_* messages. They replace
table rows in client memory without installing an addon, editing an MPQ, or
distributing a new client. This is useful for small presentation corrections
that cannot wait for, or cannot be delivered through, a client update.

A SMSG_PATCH is NOT a server gameplay change. It only creates a second "view" of
the spell: the files on disk, the client's patched row, and the server's
SpellInfo may all differ.
That can make the "true" properties confusing to inspect and debug, so:
- use this power SPARINGLY
- Keep patches explicit, reviewed, small and well-documented here
Note: Shared duration/radius/range rows can affect many other spells.

Verified against Extensions.dll SHA256:
0f8d847b3adc44a963606f0cd4f7938ad6fcd6f4d87feac3131c7153bdf3bb11.
All 34 requested PATCH_SPELL opcodes are represented below. The layout manifest
records handler RVAs and wire field order. A packet contains one record, no count.
Numeric fields are uint32 words in wire order, including IEEE-754 float bits.
Encode writes Prefix words, Strings, then Suffix words; fields holds both numeric
groups consecutively. Pointer placeholders in copied runtime records are zeroed.
TagTypes and ActivationOverlays use NUL-terminated strings. Other string-bearing
packets use uint32 byte lengths followed by bytes, without a trailing NUL.
ItemEnchantment has 14 prefix words, 16 localized strings, and 8 suffix words;
its first suffix word is the skipped locale flags word. Do not omit that word.
ShapeshiftForm omits the runtime name pointer: 18 numeric words, then the name.
VisualEffectName and MissileMotion put two strings between the ID and tail words.

SMSG_PATCH_SPELL takes a 680-byte runtime record (170 words), then name, rank,
description and aura tooltip. This is NOT a raw 936-byte Spell.dbc record.
The client's pointer fields are 136 (name), 138 (rank), 137 (description), 139 (tooltip).
Preserve every unrelated field. The generator retains the original tooltip POC
rows; do not reconstruct them from server SpellInfo, which omits client fields.
Existing-row updates are intended. New rows may require separate index rebuilding.
Do not overwrite the SpellCustomAttr rows managed by CoASpellbook.

SQL patch_spell text overrides are encoded at startup and sent to all classes at login.
Per-aura $w1 values still use the existing 0x0673
transport in src/server/coa. AscensionChronomancerTime.cpp supplies Renewal's outgoing
pulse amount in its unused trigger-aura amount; Protection already holds its absorb.
AscensionPatch.Tooltips defaults to 0: no login patches or tooltip amount updates.
Client restart also discards the in-memory patches. No client files are changed.

Examples are inactive. Each fields vector must contain a complete preserved wire
record of the documented size, with only the intended values edited. The string
vectors contain actual text, never DBC offsets or process pointers. See layouts.json
for the count and placement of each numeric/string group.

SMSG_PATCH_SPELL_CHARGES (0x0570)
Send(player, Table::Charges, chargesFields);
SMSG_PATCH_SPELL_CHARGES_CATEGORY (0x0571)
Send(player, Table::ChargesCategory, chargesCategoryFields);
SMSG_PATCH_SPELL_AFFECT (0x0574)
Send(player, Table::Affect, affectFields);
SMSG_PATCH_SPELL_CUSTOM_ATTR (0x05F4)
Send(player, Table::CustomAttr, customAttrFields);
SMSG_PATCH_SPELL_ADDON (0x0686)
Send(player, Table::Addon, addonFields);
SMSG_PATCH_SPELL_TAG_TYPES (0x06C0)
Send(player, Table::TagTypes, tagTypesFields, tagTypesStrings);
SMSG_PATCH_SPELL_TAGS (0x06C1)
Send(player, Table::Tags, tagsFields);
SMSG_PATCH_SPELL_SPELL_SUGGESTIONS (0x06CE)
Send(player, Table::SpellSuggestions, spellSuggestionsFields);
SMSG_PATCH_SPELL_STAT_SUGGESTIONS (0x06D2)
Send(player, Table::StatSuggestions, statSuggestionsFields);
SMSG_PATCH_SPELL_ACTIVATION_OVERLAYS (0x06F6)
Send(player, Table::ActivationOverlays, activationOverlaysFields, activationOverlaysStrings);
SMSG_PATCH_SPELL (0x092A)
Send(player, Table::Spell, spellFields, spellStrings);
SMSG_PATCH_SPELL_VISUAL (0x092C)
Send(player, Table::Visual, visualFields);
SMSG_PATCH_SPELL_VISUAL_KIT (0x092D)
Send(player, Table::VisualKit, visualKitFields);
SMSG_PATCH_SPELL_VISUAL_KIT_MODEL_ATTACH (0x092E)
Send(player, Table::VisualKitModelAttach, visualKitModelAttachFields);
SMSG_PATCH_SPELL_VISUAL_EFFECT_NAME (0x092F)
Send(player, Table::VisualEffectName, visualEffectNameFields, visualEffectNameStrings);
SMSG_PATCH_SPELL_MISSILE (0x0930)
Send(player, Table::Missile, missileFields);
SMSG_PATCH_SPELL_MISSILE_MOTION (0x0931)
Send(player, Table::MissileMotion, missileMotionFields, missileMotionStrings);
SMSG_PATCH_SPELL_ITEM_ENCHANTMENT (0x0935)
Send(player, Table::ItemEnchantment, itemEnchantmentFields, itemEnchantmentStrings);
SMSG_PATCH_SPELL_SHAPESHIFT_FORM (0x0949)
Send(player, Table::ShapeshiftForm, shapeshiftFormFields, shapeshiftFormStrings);
SMSG_PATCH_SPELL_ICON (0x094A)
Send(player, Table::Icon, iconFields, iconStrings);
SMSG_PATCH_SPELL_CATEGORY (0x094F)
Send(player, Table::Category, categoryFields);
SMSG_PATCH_SPELL_DURATION (0x0950)
Send(player, Table::Duration, durationFields);
SMSG_PATCH_SPELL_CAST_TIMES (0x0951)
Send(player, Table::CastTimes, castTimesFields);
SMSG_PATCH_SPELL_RADIUS (0x0952)
Send(player, Table::Radius, radiusFields);
SMSG_PATCH_SPELL_RANGE (0x0953)
Send(player, Table::Range, rangeFields, rangeStrings);
SMSG_PATCH_SPELL_DIFFICULTY (0x0954)
Send(player, Table::Difficulty, difficultyFields);
SMSG_PATCH_SPELL_DESCRIPTION_VARIABLES (0x0955)
Send(player, Table::DescriptionVariables, descriptionVariablesFields, descriptionVariablesStrings);
SMSG_PATCH_SPELL_EFFECT_CAMERA_SHAKES (0x0956)
Send(player, Table::EffectCameraShakes, effectCameraShakesFields);
SMSG_PATCH_SPELL_CHAIN_EFFECTS (0x0957)
Send(player, Table::ChainEffects, chainEffectsFields, chainEffectsStrings);
SMSG_PATCH_SPELL_ITEM_ENCHANTMENT_CONDITION (0x0958)
Send(player, Table::ItemEnchantmentCondition, itemEnchantmentConditionFields);
SMSG_PATCH_SPELL_VISUAL_KIT_AREA_MODEL (0x0959)
Send(player, Table::VisualKitAreaModel, visualKitAreaModelFields, visualKitAreaModelStrings);
SMSG_PATCH_SPELL_VISUAL_PRECAST_TRANSITIONS (0x095A)
Send(player, Table::VisualPrecastTransitions, visualPrecastTransitionsFields, visualPrecastTransitionsStrings);
SMSG_PATCH_SPELL_RUNE_COST (0x095F)
Send(player, Table::RuneCost, runeCostFields);
SMSG_PATCH_SPELL_FOCUS_OBJECT (0x0961)
Send(player, Table::FocusObject, focusObjectFields, focusObjectStrings);

Matching server behavior, client build, locale and rendered tooltip validation
remain the caller's responsibility. The login POC uses preserved enUS text.
