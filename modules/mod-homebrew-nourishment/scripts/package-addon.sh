#!/usr/bin/env bash
set -euo pipefail

project_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
addon_source="$project_root/client/HomebrewNourishment"
dist_dir="${1:-$project_root/dist}"
source_date_epoch="${SOURCE_DATE_EPOCH:-1786161600}"
version="$(sed -n 's/^## Version:[[:space:]]*//p' "$addon_source/HomebrewNourishment.toc" | tr -d '\r')"

if [[ -z "$version" ]]; then
    echo "Could not read the addon version from HomebrewNourishment.toc." >&2
    exit 1
fi

for required in HomebrewNourishment.toc HomebrewNourishment.lua README.md LICENSE.txt; do
    if [[ ! -f "$addon_source/$required" ]]; then
        echo "Missing addon file: $required" >&2
        exit 1
    fi
done

temporary="$(mktemp -d "${TMPDIR:-/tmp}/homebrew-nourishment-package.XXXXXX")"
trap 'rm -rf -- "$temporary"' EXIT

mkdir -p "$temporary/HomebrewNourishment" "$dist_dir"
for file in HomebrewNourishment.toc HomebrewNourishment.lua README.md LICENSE.txt; do
    cp "$addon_source/$file" "$temporary/HomebrewNourishment/$file"
    touch -d "@$source_date_epoch" "$temporary/HomebrewNourishment/$file"
done
touch -d "@$source_date_epoch" "$temporary/HomebrewNourishment"

archive="$dist_dir/HomebrewNourishment-$version.zip"
rm -f -- "$archive"
(
    cd "$temporary"
    LC_ALL=C find HomebrewNourishment -type f -print | LC_ALL=C sort |
        zip -X -9 "$archive" -@
)

echo "$archive"
