"""Validate server DBC dependencies and stage missing rows in a separate directory."""
from pathlib import Path
import argparse
import json
import struct


PACKAGE = Path(__file__).resolve().parents[1]


def read(path):
    data = path.read_bytes()
    magic, count, fields, size, strings_size = struct.unpack_from("<4s4I", data)
    if magic != b"WDBC" or size != fields * 4 or len(data) != 20 + count * size + strings_size:
        raise ValueError("Unsupported or truncated DBC: " + path.name)
    rows = {}
    for offset in range(20, 20 + count * size, size):
        row = struct.unpack_from("<" + "I" * fields, data, offset)
        if row[0] in rows:
            raise ValueError("Duplicate ID in " + path.name)
        rows[row[0]] = row
    return fields, rows, data[20 + count * size:]


def prepare(source, output=None):
    requirements = json.loads((PACKAGE / "data/server-dbc-rows.json").read_text())
    errors, staged, counts = [], {}, {}
    for table, definition in requirements.items():
        path = source / (table + ".dbc")
        if not path.is_file():
            errors.append("Missing base table: " + path.name)
            continue
        fields, rows, original_strings = read(path)
        if fields != definition["fields"]:
            errors.append("Unsupported layout: " + path.name)
            continue
        strings = bytearray(original_strings)
        additions = 0
        for entry in definition["rows"]:
            existing = rows.get(entry["id"])
            if existing is not None:
                semantic = [value for value, kind in zip(existing, definition["format"]) if kind not in "xXds"]
                if semantic != entry["semantic"]:
                    errors.append(f"Conflicting {table} ID {entry['id']}; manual review required")
                continue
            if entry["values"] is None:
                errors.append(f"Missing baseline {table} ID {entry['id']}; install compatible base data")
                continue
            row = list(entry["values"])
            for index, value in entry["strings"].items():
                row[int(index)] = len(strings) if value else 0
                if value:
                    strings.extend(value.encode("utf-8") + b"\0")
            rows[entry["id"]] = row
            additions += 1
        counts[table] = additions
        if additions:
            staged[path.name] = struct.pack("<4s4I", b"WDBC", len(rows), fields, fields * 4, len(strings))
            staged[path.name] += b"".join(struct.pack("<" + "I" * fields, *row) for row in rows.values())
            staged[path.name] += strings
    if errors:
        raise ValueError("No files were written.\n" + "\n".join(errors))
    if output:
        if output.resolve() == source.resolve() or output.exists():
            raise ValueError("Choose a new, separate output directory")
        output.mkdir(parents=True)
        for name, data in staged.items():
            (output / name).write_bytes(data)
    return {"missing_rows": counts, "staged_tables": sorted(staged), "original_files_modified": False}


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--dbc-dir", type=Path, required=True)
    parser.add_argument("--output", type=Path, help="New directory for merged files; omit for a read-only check")
    args = parser.parse_args()
    try:
        print(json.dumps(prepare(args.dbc_dir, args.output), indent=2))
    except (ValueError, OSError) as error:
        parser.exit(1, str(error) + "\n")
