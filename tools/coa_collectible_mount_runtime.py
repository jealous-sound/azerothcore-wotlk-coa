#!/usr/bin/env python3
import argparse
import json
import struct
from pathlib import Path

MOUNT_CATEGORY = 0x04000000
MOUNTED_AURA = 78
PLACEHOLDER = "[PH]"
EFFECT_AURA_FIELD = 95
EFFECT_MISC_FIELD = 110
SPELL_NAME_FIELD = 136
CREATURE_MOUNT_FIELD = 1
CREATURE_NAME_FIELD = 2
CREATURE_DISPLAY_FIELD = 19
ITEM_FIELD = 1
CATEGORY_FIELD = 2
LEARNED_SPELL_FIELD = 76
ITEMSPELL_SLOTS = tuple(range(2, 37, 7))
DBC_HEADER = struct.Struct("<4sIIII")
TEMPLATE_COLUMNS = ("entry", "name", "minlevel", "maxlevel", "faction", "unit_class", "type",
                    "BaseAttackTime", "RangeAttackTime")
MODEL_COLUMNS = ("CreatureID", "Idx", "CreatureDisplayID", "DisplayScale", "Probability")


def read_table(path):
    path = Path(path)
    data = path.read_bytes()
    if len(data) < DBC_HEADER.size:
        raise ValueError(f"{path}: too small for a DBC file")
    magic, records, fields, record_size, string_size = DBC_HEADER.unpack_from(data, 0)
    if magic != b"WDBC":
        raise ValueError(f"{path}: not a WDBC file")
    base = DBC_HEADER.size
    string_start = base + records * record_size
    string_block = data[string_start:string_start + string_size]

    def resolve(offset):
        if not offset or offset >= len(string_block):
            return ""
        end = string_block.find(b"\x00", offset)
        return string_block[offset:end].decode("utf-8", "replace")

    def rows():
        words = record_size // 4
        for index in range(records):
            yield struct.unpack_from("<%dI" % words, data, base + index * record_size)

    return rows, resolve


def mounted_misc(rows):
    result = {}
    for row in rows:
        if row[EFFECT_AURA_FIELD] == MOUNTED_AURA and row[EFFECT_MISC_FIELD]:
            result[row[0]] = row[EFFECT_MISC_FIELD]
    return result


def spell_names(rows, resolve):
    return {row[0]: resolve(row[SPELL_NAME_FIELD]) for row in rows}


def collection_mounts(rows):
    items, spells = set(), set()
    for row in rows:
        if row[CATEGORY_FIELD] & MOUNT_CATEGORY and row[LEARNED_SPELL_FIELD]:
            items.add(row[ITEM_FIELD])
            spells.add(row[LEARNED_SPELL_FIELD])
    return items, spells


def taught_spells(rows, item_ids):
    result = set()
    for row in rows:
        if row[ITEM_FIELD] in item_ids:
            result.update(row[slot] for slot in ITEMSPELL_SLOTS if slot < len(row) and row[slot])
    return result


def creature_mounts(rows, resolve):
    result = {}
    for row in rows:
        if row[CREATURE_MOUNT_FIELD]:
            result[row[CREATURE_MOUNT_FIELD]] = (row[CREATURE_DISPLAY_FIELD], resolve(row[CREATURE_NAME_FIELD]))
    return result


def display_models(rows):
    return {row[0]: row[1] for row in rows}


def model_ids(rows):
    return set(row[0] for row in rows)


def select_mount_runtime(existing_entries, scripted_spells, relevant_spells, misc, names,
                         creature_map, display_model, models):
    selected = {}
    for spell in sorted(relevant_spells):
        if spell in scripted_spells:
            continue
        mount = misc.get(spell)
        if not mount or mount in existing_entries or mount in selected:
            continue
        name = names.get(spell, "")
        if PLACEHOLDER in name:
            continue
        mapping = creature_map.get(mount)
        if not mapping:
            continue
        display = mapping[0]
        model = display_model.get(display)
        if not display or not model or model not in models:
            continue
        selected[mount] = {"entry": mount, "name": name, "display": display}
    return [selected[key] for key in sorted(selected)]


def render_sql(rows):
    if not rows:
        return ""
    entries = ", ".join(str(row["entry"]) for row in rows)
    templates = ", ".join("(%d, '%s', 1, 1, 35, 1, 12, 2000, 2000)"
                          % (row["entry"], row["name"].replace("'", "''")) for row in rows)
    models = ", ".join("(%d, 0, %d, 1, 1)" % (row["entry"], row["display"]) for row in rows)
    update = ", ".join("`%s` = VALUES(`%s`)" % (column, column) for column in TEMPLATE_COLUMNS[1:])
    return "\n".join([
        "INSERT INTO `creature_template` (`%s`) VALUES" % "`, `".join(TEMPLATE_COLUMNS),
        templates + " ON DUPLICATE KEY UPDATE " + update + ";",
        "DELETE FROM `creature_template_model` WHERE `CreatureID` IN (%s);" % entries,
        "INSERT INTO `creature_template_model` (`%s`) VALUES" % "`, `".join(MODEL_COLUMNS),
        models + ";",
        "",
    ])


def load_inventory(path):
    data = json.loads(Path(path).read_text(encoding="utf-8-sig"))
    return (set(int(value) for value in data.get("creatureEntries", [])),
            set(int(value) for value in data.get("scriptedSpells", [])))


def select_from_dbc(dbc_dir, existing_entries, scripted_spells):
    dbc_dir = Path(dbc_dir)
    spell_rows, _ = read_table(dbc_dir / "Spell.dbc")
    misc = mounted_misc(spell_rows())
    spell_rows, resolve = read_table(dbc_dir / "Spell.dbc")
    names = spell_names(spell_rows(), resolve)
    vanity_rows, _ = read_table(dbc_dir / "VanityCollection.dbc")
    items, collection_spells = collection_mounts(vanity_rows())
    itemspell_rows, _ = read_table(dbc_dir / "ItemSpells.dbc")
    relevant = (collection_spells | taught_spells(itemspell_rows(), items)) & set(misc)
    creature_rows, creature_resolve = read_table(dbc_dir / "Creature.dbc")
    creature_map = creature_mounts(creature_rows(), creature_resolve)
    display_rows, _ = read_table(dbc_dir / "CreatureDisplayInfo.dbc")
    model_rows, _ = read_table(dbc_dir / "CreatureModelData.dbc")
    return select_mount_runtime(existing_entries, scripted_spells, relevant, misc, names,
                                creature_map, display_models(display_rows()), model_ids(model_rows()))


def main():
    parser = argparse.ArgumentParser(description="Generate collectible mount runtime rows from the client Creature.dbc mapping.")
    parser.add_argument("--dbc", required=True, help="Directory holding the client DBC set.")
    parser.add_argument("--inventory", required=True, help="JSON with creatureEntries and scriptedSpells.")
    parser.add_argument("--out", required=True, help="Migration file to write.")
    parser.add_argument("--check", action="store_true", help="Exit nonzero when any missing mapping is found.")
    args = parser.parse_args()
    existing_entries, scripted_spells = load_inventory(args.inventory)
    rows = select_from_dbc(args.dbc, existing_entries, scripted_spells)
    Path(args.out).write_text(render_sql(rows), encoding="utf-8", newline="\n")
    print(json.dumps(rows, indent=2))
    if args.check and rows:
        raise SystemExit(1)
    raise SystemExit(0)


if __name__ == "__main__":
    main()
