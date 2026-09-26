Achievement patches replace client memory rows without distributing a client
update. Use sparingly: client data, files on disk and server achievement rules
can disagree. These packets do not award achievements or change server criteria.
Preserve complete records and apply only deliberate, reviewed client corrections.
The Descension corrections require AscensionPatch.Tooltips = 1. Each packet type
has an independent default-off switch:

- AscensionPatch.Achievements: scoped achievement titles and descriptions (0x0936).
- AscensionPatch.AchievementCriteria: associated criterion text (0x0937).
- AscensionPatch.AchievementCategories: all 16 category names containing Ascension (0x0938).

The achievement-only patch reproduced the level-10 lock on a level-60 character.
Resending existing achievement state with `.debug send achievements` restored the
UI and displayed the patched title. Login delivery now resends achievement state
after sending any enabled achievement patches; it does not award achievements.
The automatic login sequence still needs client verification. Criteria and category
layouts remain unverified: keep their switches off when testing the achievement title.
Test each switch independently with a full client exit between tests. Server logs
identify sent opcodes; config reload changes future login delivery, not already patched clients.
Client restart discards the patches. Send success is not client acknowledgement.

Original layout investigation used Extensions.dll SHA256:
0f8d847b3adc44a963606f0cd4f7938ad6fcd6f4d87feac3131c7153bdf3bb11.
achievement_layouts.json records the reader RVAs and wire layouts.
Each packet contains one record, without a count: prefix uint32 words, strings,
then suffix uint32 words. The fields vector holds prefix and suffix consecutively.
Strings use uint32 byte lengths without NUL terminators or DBC string offsets.

Achievement: 14 words followed by title, description and reward text. Runtime
pointer placeholders at indices 4, 5 and 11 are zeroed by Encode.
Criteria: 9 words, 16 localized descriptions, then 5 words. No pointer placeholder
is transmitted. This locale count is disputed by the protocol atlas and is not client-validated.
Category: ID and parent ID, 16 localized names, then locale flags and sort order.
Both the locale count and suffix length are disputed by the protocol atlas.
These wire formats are not interchangeable with raw DBC or runtime records.

Inactive examples: fields must contain complete preserved records in wire order.
SMSG_PATCH_ACHIEVEMENT (0x0936)
Send(player, Table::Achievement, achievementFields, {title, description, reward});
SMSG_PATCH_ACHIEVEMENT_CRITERIA (0x0937)
Send(player, Table::Criteria, criteriaFields, localizedDescriptions);
SMSG_PATCH_ACHIEVEMENT_CATEGORY (0x0938)
Send(player, Table::Category, categoryFields, localizedNames);

The login corrections load text overrides from `patch_achievement`,
`patch_achievement_criteria` and `patch_achievement_category`. Complete original
records remain in `AscensionPatchAchievementData.h`, with their source hashes.
Only enabled SQL rows are sent; NULL fields preserve original text. See the README
for schema, supported IDs and restart behavior.
They rename all 336 enUS achievement titles/descriptions containing Ascension,
including 117 rune milestones in 41067-41191. The five unrelated achievements in
that range are unchanged; three IDs are absent. All numeric requirements remain intact.
The existing 41067 description/criterion correction remains intact. Achievement 41142
through 41191 descriptions are corrected to match their title amounts;
criterion 41190 text is also corrected to 10,000,000.
Associated criterion patches cover 189 records. Category patches rename all 16
category labels containing Ascension. The separate configuration gates are unchanged;
category and criterion layouts still need client verification. Quest-log text is unchanged.
The migration seeds every inspected enUS achievement title, description and category
name containing Ascension; original DBC files and server requirements remain unchanged.

Achievement numeric projection: DBC columns 0-3, two zero string placeholders,
38-42, one zero reward placeholder, 60-61. Criteria projection: columns 0-8,
16 descriptions (9-24), then 26-30, omitting locale flags at 25. Category projection:
columns 0-1, 16 names (2-17), then 18-19. Empty locales and reward text are preserved.

The current atlas entries for 0x0937 and 0x0938 describe a single transformed
length-prefixed blob instead of sixteen strings; 0x0938 has only one trailing
word. Their transform remains unresolved. Packet harnesses validate our encoder
against its declared layout, not acceptance by the client. Do not treat their
passing results as proof that these two packet layouts are correct.
