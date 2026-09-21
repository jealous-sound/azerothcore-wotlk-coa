# Optional second character names

Characters may keep one word or use two words separated by one space. Each word follows the existing
alphabet, minimum-length and repeated-letter rules, with a maximum of 12 letters. The whole name is
limited to 25 letters including the space and 47 UTF-8 bytes because the native client stores names in
48-byte buffers. Both words are capitalized. The full name is the unique character identity; two
characters can share a first name.

Character creation has a first-name field and an optional second-name field. Rename, mail and social
dialogs accept the full name. Clicking a name, replying and recalling whisper history preserve both
words. For typed commands with trailing text, use quotes: `/w "Arthas Menethil" Hello` and
`/friend "Arthas Menethil" My note`. Existing single-word syntax remains supported. Server commands
that accept player identifiers also accept quoted full names and player links.

The server changes and pending character-database migration must accompany the client changes.
This targets our native v4 client pair. Its `Extensions.dll` already replaces the shared character-name
validator with a success result, so validation remains authoritative on the server. The interface checks
the combined UTF-8 byte length before submitting creation, avoiding native buffer truncation.

## Prepare the client candidate

Run this command from the repository root, using a new output path. The UI generator reads the
canonical client source and checks every replacement before writing any output. It emits changed MPQ
members and a reviewable diff; it does not include game-derived source files in Git.

```powershell
python apps/client-compat/patch_character_names.py --source C:/Ascension/client-reference/patch-B --output .cache/character-names/ui
```

Native inspection of the installed DLL (SHA256
`9791801053f828d1ccdab1a4c17e64852d3ebe0fa708b91fa3674d0805d15bc8`) locates the existing
hook registration at DLL RVA `0xA6ACD7`: it replaces EXE address `0x6B0F90` with DLL RVA
`0xA4AAE0`, which returns `CHAR_NAME_SUCCESS` (`0x57`). The EXE creation path still copies the
name into a 48-byte buffer. Neither binary needs a change for this feature. A stock WotLK client
without this extension has different validation and is not supported by this UI patch alone.

The generator does not install files, start the client, change profiles or connect to a service.
For an authorized installation, compose the generated UI members with the current client archive
through the normal deployment guards, and deploy the server migration with the matching server build.

## Regression checks

```powershell
python -X utf8 -m unittest discover -s apps/client-compat -p test_character_names.py
python apps/coa-gameplay-test/run.py validate apps/coa-gameplay-test/scenarios/optional-character-names.json
```

The Lua checks require Lupa with Lua 5.1 and the canonical client UI source. They compile the generated
Lua/XML callbacks and exercise recipient handling and creation byte limits.
They do not prove rendered UI layout or client/server interoperability.

The C++ `PlayerNameTest.*` tests cover normalization and server validation. The gameplay scenario
uses the real worldserver character-create handler and character loading, including single names,
shared first names, maximum-length names, cache lookups, Who filtering and quoted character commands. See the
[gameplay harness](../../coa-gameplay-test/README.md) for isolated execution and its coverage limits.

For a Windows test build using the dynamic MSVC runtime (`/MD`), configure with
`-DBUILD_TESTING=ON -Dgtest_force_shared_crt=ON` so GoogleTest uses the same runtime as the server.
