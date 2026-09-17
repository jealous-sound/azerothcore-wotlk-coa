"""Read-only world database checks before importing the supplied SQL."""
from decimal import Decimal, InvalidOperation
from pathlib import Path
import argparse
import json
import re
import subprocess


PACKAGE = Path(__file__).resolve().parents[1]


def same(left, right):
    if left is None or right is None:
        return left is right
    try:
        a, b = Decimal(str(left)), Decimal(str(right))
        return abs(a - b) <= max(Decimal("0.00001"), abs(b) * Decimal("0.000001"))
    except InvalidOperation:
        return str(left) == str(right)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--database", required=True)
    parser.add_argument("--defaults-extra-file", type=Path,
                        help="Local MySQL option file; credentials are not included in this package")
    parser.add_argument("--mysql", default="mysql")
    parser.add_argument("--accept-item-updates", action="store_true",
                        help="Allow replacing differing Bloodforged templates after reviewing/backing them up")
    args = parser.parse_args()
    if not re.fullmatch("[A-Za-z0-9_]+", args.database):
        parser.error("Invalid database name")
    command = [args.mysql]
    if args.defaults_extra_file:
        command.append("--defaults-extra-file=" + str(args.defaults_extra_file.resolve()))
    command += ["--batch", "--skip-column-names", "--raw", args.database]

    def query(sql):
        run = subprocess.run(command, input=sql, text=True, encoding="utf-8", capture_output=True, check=True)
        return [json.loads(line) for line in run.stdout.splitlines() if line.strip()]

    expected = json.loads((PACKAGE / "data/items.json").read_text())
    fields = list(expected[0])
    schema = query("SELECT JSON_OBJECT('name',COLUMN_NAME) FROM information_schema.columns "
                   "WHERE TABLE_SCHEMA=DATABASE() AND TABLE_NAME='item_template';")
    missing = set(fields + ["ScriptName"]) - {row["name"] for row in schema}
    if missing:
        parser.exit(1, "Incompatible item_template schema: " + ", ".join(sorted(missing)) + "\n")
    select_fields = list(dict.fromkeys(fields + ["ScriptName"]))
    pairs = ",".join("'" + name + "',`" + name + "`" for name in select_fields)
    conflicts, unsafe, existing = [], [], 0
    for offset in range(0, len(expected), 100):
        batch = expected[offset:offset + 100]
        ids = ",".join(str(int(row["entry"])) for row in batch)
        rows = query("SELECT JSON_OBJECT(" + pairs + ") FROM item_template WHERE entry IN (" + ids + ");")
        actual = {int(row["entry"]): row for row in rows}
        existing += len(actual)
        for row in batch:
            current = actual.get(int(row["entry"]))
            if current is None:
                continue
            if current["ScriptName"] or not current["name"].startswith("Bloodforged "):
                unsafe.append(int(row["entry"]))
            changed = [name for name in fields if not same(current[name], row[name])]
            if changed:
                conflicts.append({"entry": int(row["entry"]), "fields": changed})
    chest = query("SELECT JSON_OBJECT('entry',entry,'script',ScriptName) FROM gameobject_template WHERE entry=994300;")
    if chest and chest[0]["script"] != "highrisk_chest":
        parser.exit(1, "Chest ID 994300 is already used by another script/template. No changes made.\n")
    enchantments = json.loads((PACKAGE / "data/enchantments.json").read_text())
    for entry, wanted in enchantments.items():
        found = query("SELECT JSON_ARRAY(ench,chance) FROM item_enchantment_template WHERE entry=" + str(int(entry)) + " AND chance>0;")
        found = sorted((int(row[0]), float(row[1])) for row in found)
        wanted = sorted((int(row[0]), float(row[1])) for row in wanted)
        if found != wanted:
            parser.exit(1, "Incompatible base enchantment pool " + entry + "; review data/enchantments.json.\n")
    print(json.dumps({"existing_items": existing, "new_items": len(expected) - existing,
                      "conflicting_templates": conflicts, "unsafe_collisions": unsafe,
                      "database_modified": False}, indent=2))
    if unsafe or (conflicts and not args.accept_item_updates):
        parser.exit(1, "Preflight failed. Review collisions and back up affected templates before importing.\n")
    print("Preflight passed. This command did not import any SQL.")


if __name__ == "__main__":
    main()
