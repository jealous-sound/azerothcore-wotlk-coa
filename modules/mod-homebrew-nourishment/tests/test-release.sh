#!/usr/bin/env bash
set -euo pipefail

project_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
server_source="$project_root/src/mod_homebrew_nourishment.cpp"
addon_root="$project_root/client/HomebrewNourishment"
profile_sql="$project_root/data/sql/db-world/2026_08_08_00_homebrew_nourishment_profiles.sql"
migration_sql="$project_root/data/sql/db-characters/2026_09_24_00_homebrew_nourishment_slots.sql"
audit_tsv="$project_root/data/catalog/profile-audit.tsv"
expected_version="1.1.0"

fail()
{
    echo "FAIL: $*" >&2
    exit 1
}

for command in python3 rg unzip zip sha256sum; do
    command -v "$command" >/dev/null || fail "required command is unavailable: $command"
done

rg -q --fixed-strings 'constexpr char kProtocolVersion[] = "3";' "$server_source" ||
    fail "server protocol is not 3"
rg -q --fixed-strings 'local PROTOCOL = "3"' "$addon_root/HomebrewNourishment.lua" ||
    fail "client protocol is not 3"
[[ -f "$migration_sql" ]] ||
    fail "three-slot character migration is missing"
rg -q --fixed-strings 'std::array<ActiveNourishment, kNourishmentSlotCount>' "$server_source" ||
    fail "server does not retain three active slots"
rg -q --fixed-strings 'std::to_string(CompletionSeconds())' "$server_source" ||
    fail "server does not synchronize completion time"
rg -q --fixed-strings 'self.completionSeconds = tonumber(fields[2]' "$addon_root/HomebrewNourishment.lua" ||
    fail "addon does not consume synchronized completion time"
rg -q --fixed-strings "## Version: $expected_version" "$addon_root/HomebrewNourishment.toc" ||
    fail "TOC version is not $expected_version"

if rg -n '/opt/azerothcore|ac-worldserver|azerothcore-backup|192\.168\.|Homebrew Beta|Homebrew Azeroth' \
    "$project_root/README.md" "$project_root/src" "$project_root/client" "$project_root/conf"; then
    fail "private deployment assumptions remain in public runtime or instructions"
fi

python3 -m py_compile "$project_root/tools/generate-nourishment-profiles.py" "$project_root/tests/test_generator.py"
python3 "$project_root/tests/test_generator.py"

if command -v luajit >/dev/null; then
    HBN_ADDON="$addon_root/HomebrewNourishment.lua" luajit -e 'assert(loadfile(os.getenv("HBN_ADDON")))'
elif command -v lua >/dev/null; then
    HBN_ADDON="$addon_root/HomebrewNourishment.lua" lua -e 'assert(loadfile(os.getenv("HBN_ADDON")))'
else
    fail "Lua or LuaJIT is required to validate addon syntax"
fi

included="$(awk -F '\t' 'NR > 1 && $3 == "included" { count++ } END { print count + 0 }' "$audit_tsv")"
excluded="$(awk -F '\t' 'NR > 1 && $3 == "excluded" { count++ } END { print count + 0 }' "$audit_tsv")"
[[ "$included" == 300 ]] || fail "expected 300 included catalog items, found $included"
[[ "$excluded" == 195 ]] || fail "expected 195 excluded catalog items, found $excluded"
[[ "$(rg -c '^\([0-9]+,' "$profile_sql")" == 300 ]] || fail "baseline SQL does not contain 300 profiles"
rg -q 'CREATE TABLE IF NOT EXISTS `mod_homebrew_nourishment_profile`' "$profile_sql" || fail "profile table is missing"
rg -q 'CREATE TABLE IF NOT EXISTS `mod_homebrew_nourishment_stock_aura`' "$profile_sql" || fail "stock aura table is missing"
if rg -n 'DROP[[:space:]]+(TABLE|DATABASE)' "$profile_sql"; then
    fail "baseline SQL modifies data outside module-owned tables"
fi
unexpected_deletes="$(rg '^DELETE[[:space:]]+FROM' "$profile_sql" | rg -v '^DELETE FROM `mod_homebrew_nourishment_' || true)"
[[ -z "$unexpected_deletes" ]] || fail "baseline SQL deletes data outside module-owned tables"

bash -n "$project_root/scripts/package-addon.sh"
temporary="$(mktemp -d "${TMPDIR:-/tmp}/homebrew-nourishment-test.XXXXXX")"
trap 'rm -rf -- "$temporary"' EXIT

SOURCE_DATE_EPOCH=1786161600 "$project_root/scripts/package-addon.sh" "$temporary/one" >/dev/null
SOURCE_DATE_EPOCH=1786161600 "$project_root/scripts/package-addon.sh" "$temporary/two" >/dev/null
first="$temporary/one/HomebrewNourishment-$expected_version.zip"
second="$temporary/two/HomebrewNourishment-$expected_version.zip"
[[ "$(sha256sum "$first" | cut -d' ' -f1)" == "$(sha256sum "$second" | cut -d' ' -f1)" ]] ||
    fail "addon archives are not reproducible"

expected_files=$'HomebrewNourishment/HomebrewNourishment.lua\nHomebrewNourishment/HomebrewNourishment.toc\nHomebrewNourishment/LICENSE.txt\nHomebrewNourishment/README.md'
actual_files="$(unzip -Z1 "$first" | LC_ALL=C sort)"
[[ "$actual_files" == "$expected_files" ]] || fail "addon archive contains unexpected files"

echo "Homebrew Nourishment release checks passed."
