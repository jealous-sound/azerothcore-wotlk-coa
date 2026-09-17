# Ascension race/class availability and character creation

Use this reference for changes to race/class availability or starting character data. Reinspect the effective
client and server data for the task; these facts do not establish that a particular installation is current.

## Client and server gates

- The CoA client's native `IsRaceClassValid` reads `CharBaseInfo.dbc`; the Glue wrapper's `CanCreateClass`
  controls the set of classes, not race compatibility. A Lua-only override does not prove that native selection
  or server creation accepts a pair.
- The inspected CoA client uses two byte fields per on-disk CharBaseInfo record, but eight-byte native records.
  Verify the format before editing; do not assume conventional DWORD-per-field DBC records. Preserve existing
  records, ordering, and duplicates; append only the intended missing pairs and update the record count.
- Archive extraction alone does not establish override precedence. Verify the effective loaded table and native
  predicate when doing authorized client validation. A read-only predicate probe is separate from rendered UI
  and actual character-creation testing. Glue UI race index 9 means Blood Elf, not race ID 9.
- `ObjectMgr::LoadPlayerInfo` builds server race/class availability from `playercreateinfo`. Copy the race's
  unambiguous existing start rather than an arbitrary class donor's spawn. Class and race statistics are separate
  tables; verify intended levels and class-wide starter-item gates.
- Racial skills with class-specific variants need an explicit resource/stat mapping for newly opened classes.
  Do not OR every class into every variant, which would grant mutually exclusive racials. Record unresolved
  variants separately from the availability change.
