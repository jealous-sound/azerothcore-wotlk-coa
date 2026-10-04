#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
"""Generate curated, auditable nourishment profiles from the live world DB."""

from __future__ import annotations

import argparse
import collections
import csv
import dataclasses
import re
import struct
import subprocess
from pathlib import Path


TIER_VALUES = (0, 1, 2, 4, 6, 9, 13, 18, 24, 30)
TIER_LEVELS = (0, 1, 5, 15, 25, 35, 45, 55, 65, 75)

# Every template is a stock 3.3.5a Well Fed aura, so an unpatched client can
# render and cancel it. The companion addon supplies the exact dynamic text.
FAMILY_TEMPLATES = {
    "hearty": 57371,       # Strength + Stamina
    "nimble": 57367,      # Agility + Stamina
    "fortifying": 57365,  # Spirit + Stamina
    "insightful": 33263,  # Spell power + Spirit
    "keen": 57329,        # Critical rating + Stamina
    "precise": 57360,     # Hit rating + Stamina
    "energizing": 57332,  # Haste rating + Stamina
    "clear": 57334,       # MP5 + Stamina
    "banquet": 57399,     # Attack power + Spell power + Stamina
}

BAD_NAME = re.compile(
    r"(?:^|[^a-z])(?:test|deprecated|unused|placeholder|internal|debug|qa|dnd)(?:[^a-z]|$)|"
    r"zzold|zzdeprecated|do not use|monster -|npc -|copy of|quest test",
    re.IGNORECASE,
)
CONJURED = re.compile(r"conjured|mana (?:gem|agate|jade|citrine|ruby)|mage food", re.IGNORECASE)
ALCOHOL = re.compile(
    r"ale\b|beer|brew|lager|stout|porter|wine|rum\b|grog|mead|moonshine|"
    r"bourbon|brandy|liquor|spirits|gin\b|whiskey|whisky|vodka|schnapps|kungaloosh|dos ogris",
    re.IGNORECASE,
)
PET_ONLY = re.compile(r"pet food|pet treat|critter bites|mammoth treats|kibble", re.IGNORECASE)
EVENT_ITEM = re.compile(
    r"darkmoon|brewfest|pilgrim|hallow|winter veil|lunar festival|midsummer|"
    r"valentine|love is in the air|noblegarden|faire drink|graccu",
    re.IGNORECASE,
)
EVENT_ITEM_IDS = frozenset(
    list(range(16166, 16172)) +
    list(range(18632, 18636)) +
    list(range(19223, 19307)) +
    list(range(44836, 44841))
)

FAMILY_PATTERNS: tuple[tuple[str, re.Pattern[str]], ...] = (
    ("energizing", re.compile(
        r"coffee|espresso|cocoa|chocolate|candy|cookie|cake|pie|brownie|cupcake|"
        r"pudding|sweet|sugar|honey|pastry|tart|donut|doughnut|ice cream",
        re.IGNORECASE,
    )),
    ("clear", re.compile(
        r"\bwater\b|\bmilk\b|juice|\btea\b|nectar|cider|lemonade|beverage|\bdrink\b|refreshment",
        re.IGNORECASE,
    )),
    ("keen", re.compile(
        r"fish|mackerel|snapper|salmon|trout|catfish|cod|tuna|lobster|clam|crab|"
        r"crawdad|eel|herring|sardine|yellowtail|squid|shrimp|shellfish|halibut|"
        r"grouper|sunfish|rockfin|sculpin|mussel|oyster|carp|bass|fillet|albacore|"
        r"bloodfin|bluefin|redgill|bonescale|goby|fin\b",
        re.IGNORECASE,
    )),
    ("hearty", re.compile(
        r"meat|steak|jerky|ham|sausage|bacon|roast|wolf|bear|venison|"
        r"pork|chicken|turkey|mammoth|rhino|talbuk|worg|burger|haunch|shank|"
        r"chop|drumstick|buzzard|crocolisk|\brat\b|stag|lamb|\begg|yolk|giblet|"
        r"tallhorn|\brib(?:s)?\b|\bboar\b",
        re.IGNORECASE,
    )),
    ("nimble", re.compile(
        r"apple|banana|melon|berry|berries|fruit|plantain|carrot|potato|pumpkin|"
        r"squash|pear|peach|orange|grape|corn|bean|yam|tuber|turnip|cabbage|"
        r"vegetable|papaya|mango|pomegranate|pineapple|kimchi|leaf\b|spineleaf",
        re.IGNORECASE,
    )),
    ("insightful", re.compile(
        r"mushroom|fungus|fungi|truffle|lichen|spore|morel|cap\b|herb|root",
        re.IGNORECASE,
    )),
    ("fortifying", re.compile(
        r"bread|cheese|cheddar|brie|loaf|roll|biscuit|muffin|cracker|grain|"
        r"rye|baguette|cornbread|flatbread|sourdough|sharp\b|bleu\b|swiss\b|"
        r"muenster|whey|pretzel",
        re.IGNORECASE,
    )),
    ("banquet", re.compile(r"banquet|feast|grand meal|platter|smorgasbord", re.IGNORECASE)),
    ("precise", re.compile(
        r"soup|stew|gumbo|chili|chowder|omelet|omelette|kabob|kebab|dumpling|"
        r"wrap|curry|bisque|gumbo|ragout|surprise|delight|medley|skewer|slop",
        re.IGNORECASE,
    )),
)

# A small curated override list resolves genuinely ambiguous prepared dishes
# and prevents substring-based classifications from deciding their identity.
FAMILY_OVERRIDES = {
    1082: "precise",      # Redridge Goulash
    4607: "insightful",   # Delicious Cave Mold
    5066: "nimble",       # Fissure Plant
    5479: "hearty",       # Crispy Lizard Tail
    7806: "energizing",   # Lollipop
    8948: "insightful",   # Dried King Bolete
    9681: "keen",         # Grilled King Crawler Legs
    12224: "hearty",      # Crispy Bat Wing
    18254: "precise",    # Runn Tum Tuber Surprise
    27859: "insightful",  # Zangar Caps
    29449: "fortifying",  # Bladespire Bagel
    29453: "insightful", # Sporeggar Mushroom
    31672: "hearty",      # Mok'Nathal Shortribs
    35948: "nimble",      # Savory Snowplum
    40202: "hearty",      # Sizzling Grizzly Flank
    42778: "banquet",     # Crusader's Rations
}


@dataclasses.dataclass(frozen=True)
class Spell:
    spell_id: int
    name: str
    tooltip: str
    effects: tuple[int, int, int]
    auras: tuple[int, int, int]
    misc: tuple[int, int, int]
    base_points: tuple[int, int, int]
    trigger_spells: tuple[int, int, int]


@dataclasses.dataclass(frozen=True)
class Item:
    entry: int
    name: str
    quality: int
    flags: int
    item_level: int
    required_level: int
    required_reputation: int
    spell_id: int
    spell_trigger: int
    bonding: int
    description: str
    start_quest: int
    area: int
    map_id: int
    holiday_id: int


@dataclasses.dataclass(frozen=True)
class Profile:
    item: Item
    source_spell: Spell
    meal_aura_spell: int
    tier: int
    grade: int
    cooked: bool
    family: str
    marker_spell: int
    amount_1: int
    amount_2: int
    amount_3: int
    minimum_level: int
    duration_seconds: int
    recovery_kind: str
    native_aura: int
    classification: str


def signed(value: int) -> int:
    return value if value < 2**31 else value - 2**32


def read_string(block: bytes, offset: int) -> str:
    if offset < 0 or offset >= len(block):
        return ""
    end = block.find(b"\0", offset)
    if end < 0:
        end = len(block)
    return block[offset:end].decode("utf-8", "replace")


def load_spells(path: Path) -> dict[int, Spell]:
    with path.open("rb") as source:
        magic, records, fields, record_size, string_size = struct.unpack("<4s4I", source.read(20))
        if magic != b"WDBC" or fields != 234 or record_size != 936:
            raise RuntimeError(
                f"Unsupported Spell.dbc layout: magic={magic!r}, fields={fields}, record_size={record_size}"
            )
        rows = source.read(records * record_size)
        strings = source.read(string_size)

    spells: dict[int, Spell] = {}
    for index in range(records):
        row = struct.unpack_from("<234I", rows, index * record_size)
        spell_id = row[0]
        spells[spell_id] = Spell(
            spell_id=spell_id,
            name=read_string(strings, row[136]),
            tooltip=read_string(strings, row[187]),
            effects=tuple(row[71:74]),
            auras=tuple(row[95:98]),
            misc=tuple(signed(value) for value in row[110:113]),
            base_points=tuple(signed(value) + 1 for value in row[80:83]),
            trigger_spells=tuple(row[116:119]),
        )
    return spells


def load_cooking_outputs(skill_line_ability: Path, spell_dbc: Path, spells: dict[int, Spell]) -> set[int]:
    with skill_line_ability.open("rb") as source:
        magic, records, fields, record_size, _ = struct.unpack("<4s4I", source.read(20))
        if magic != b"WDBC" or fields != 69 or record_size != 276:
            raise RuntimeError(f"Unsupported SkillLineAbility.dbc layout: {fields} fields, {record_size} bytes")
        rows = source.read(records * record_size)
    recipe_spells = set()
    for index in range(records):
        row = struct.unpack_from("<69I", rows, index * record_size)
        if row[1] == 185 and row[2] in spells:
            recipe_spells.add(row[2])

    with spell_dbc.open("rb") as source:
        magic, records, fields, record_size, _ = struct.unpack("<4s4I", source.read(20))
        if magic != b"WDBC" or fields != 234 or record_size != 936:
            raise RuntimeError(f"Unsupported Spell.dbc layout: {fields} fields, {record_size} bytes")
        rows = source.read(records * record_size)
    outputs: set[int] = set()
    for index in range(records):
        row = struct.unpack_from("<234I", rows, index * record_size)
        if row[0] not in recipe_spells:
            continue
        for effect_type, item_id in zip(row[71:74], row[107:110]):
            if effect_type == 24 and item_id:
                outputs.add(item_id)
    return outputs


def mysql_rows(
    mysql_command: str,
    database: str,
    query: str,
    defaults_extra_file: Path | None,
) -> list[list[str]]:
    command = [mysql_command]
    if defaults_extra_file:
        command.append(f"--defaults-extra-file={defaults_extra_file}")
    command.extend(("--batch", "--raw", "--skip-column-names", database, "--execute", query))
    result = subprocess.run(
        command,
        check=True,
        text=True,
        capture_output=True,
    )
    return [line.split("\t") for line in result.stdout.splitlines() if line]


def load_items(
    mysql_command: str,
    database: str,
    defaults_extra_file: Path | None,
) -> list[Item]:
    query = """
        SELECT entry, name, Quality, Flags, ItemLevel, RequiredLevel,
               RequiredReputationFaction, spellid_1, spelltrigger_1, bonding,
               description, startquest, area, Map, HolidayId
        FROM item_template
        WHERE class = 0 AND subclass = 5
        ORDER BY entry
    """
    return [
        Item(
            entry=int(row[0]), name=row[1], quality=int(row[2]), flags=int(row[3]),
            item_level=int(row[4]), required_level=int(row[5]), required_reputation=int(row[6]),
            spell_id=int(row[7]), spell_trigger=int(row[8]), bonding=int(row[9]),
            description=row[10], start_quest=int(row[11]), area=int(row[12]),
            map_id=int(row[13]), holiday_id=int(row[14]),
        )
        for row in mysql_rows(mysql_command, database, query, defaults_extra_file)
    ]


def tier_for(item: Item, premium: bool) -> int:
    required_tier = 1
    for tier in range(2, 10):
        if item.required_level >= TIER_LEVELS[tier]:
            required_tier = tier

    item_tier = 1
    for tier, threshold in enumerate((0, 0, 15, 25, 35, 45, 55, 65, 75, 85)):
        if tier and item.item_level >= threshold:
            item_tier = tier

    tier = max(required_tier, item_tier)
    # Northrend crafted foods are true endgame food even though their item
    # requirement begins at 70 rather than the ordinary-ration rank at 75.
    if premium and item.required_level >= 70 and item.item_level >= 80:
        tier = 9
    return min(9, max(1, tier))


def native_well_fed(source: Spell, spells: dict[int, Spell]) -> Spell | None:
    for trigger in source.trigger_spells:
        candidate = spells.get(trigger)
        if candidate and "well fed" in candidate.name.lower():
            return candidate
    return None


def recovery_kind(source: Spell) -> str | None:
    name = source.name.lower()
    tooltip = source.tooltip.lower()
    aura_set = set(source.auras)
    has_health = 84 in aura_set or "health per second" in tooltip
    has_mana = 85 in aura_set or 226 in aura_set or "mana per second" in tooltip
    is_refreshment = name in {"food", "drink", "refreshment", "brain food", "holiday drink"}

    if not is_refreshment and not has_health and not has_mana:
        return None
    if 77 in source.effects:  # feast/table/gameobject creation, not something the player eats directly
        return None
    if name == "drink" or (has_mana and not has_health and name != "food"):
        return "drink"
    if has_health and has_mana:
        return "refreshment"
    return "food" if has_health or name == "food" else "drink"


def find_meal_aura(source: Spell, spells: dict[int, Spell]) -> int:
    candidates = [source]
    candidates.extend(spells[trigger] for trigger in source.trigger_spells if trigger in spells)
    for candidate in candidates:
        if 23 in candidate.auras:
            return candidate.spell_id
    for candidate in candidates:
        if set(candidate.auras) & {84, 85, 226}:
            return candidate.spell_id
    return 0


def family_from_native(aura: Spell | None) -> str | None:
    if not aura:
        return None
    pairs = set(zip(aura.auras, aura.misc))
    aura_types = set(aura.auras)
    rating_masks = {misc for effect, misc in pairs if effect == 189}
    if (99 in aura_types or 124 in aura_types) and (13 in aura_types or 135 in aura_types):
        return "banquet"
    if 0 in {misc for effect, misc in pairs if effect == 29}:
        return "hearty"
    if 1 in {misc for effect, misc in pairs if effect == 29}:
        return "nimble"
    if 917504 in rating_masks:
        return "energizing"
    if 1792 in rating_masks:
        return "keen"
    if rating_masks & {224, 8388608, 16777216}:
        return "precise"
    if 85 in aura_types:
        return "clear"
    if 13 in aura_types or 135 in aura_types:
        return "insightful"
    # Generic classic Well Fed is Stamina + Spirit regardless of the meal's
    # ingredients. Let the item name give those foods their new identity.
    return None


def native_is_supported(aura: Spell) -> bool:
    if family_from_native(aura):
        return True
    active_pairs = [(effect, misc) for effect, misc in zip(aura.auras, aura.misc) if effect]
    return bool(active_pairs) and all(effect == 29 and misc in {2, 4} for effect, misc in active_pairs)


def classify_family(item: Item, kind: str, native: Spell | None) -> tuple[str, str]:
    native_family = family_from_native(native)
    if native_family:
        return native_family, "native-well-fed"

    if item.entry in FAMILY_OVERRIDES:
        return FAMILY_OVERRIDES[item.entry], "curated-override"

    for family, pattern in FAMILY_PATTERNS:
        if pattern.search(item.name):
            return family, "name"

    if kind == "drink":
        return "clear", "drink-fallback"

    # Keep every mapping explicit in the generated audit while distributing
    # genuinely ambiguous dishes across useful, non-banquet families.
    fallback = ("hearty", "nimble", "fortifying", "insightful", "keen", "precise", "energizing")
    return fallback[item.entry % len(fallback)], "stable-fallback"


def amounts(family: str, tier: int, premium: bool) -> tuple[int, int, int]:
    value = TIER_VALUES[tier]
    if premium:
        value = max(value + 1, round(value * 4 / 3))
    spell_power = max(1, round(value * 1.25))
    mana_five = max(1, round(value * 0.4))

    if family in {"hearty", "nimble", "fortifying", "keen", "precise", "energizing"}:
        return value, value, 0
    if family == "insightful":
        return spell_power, value, spell_power
    if family == "clear":
        return mana_five, value, 0
    if family == "banquet":
        return value * 2, value, spell_power
    raise ValueError(f"Unknown family {family}")


def exclusion_reason(item: Item, source: Spell | None, spells: dict[int, Spell]) -> str | None:
    if item.holiday_id:
        return "holiday"
    if item.start_quest:
        return "starts-quest"
    if item.bonding:
        return "binding-or-quest-item"
    if item.required_reputation:
        return "reputation-gated"
    if item.area or item.map_id not in {-1, 0}:
        return "area-or-map-restricted"
    if item.spell_trigger != 0:
        return "not-on-use"
    if not item.spell_id or not source:
        return "missing-source-spell"
    if item.quality > 3:
        return "exceptional-quality"
    if BAD_NAME.search(item.name) or BAD_NAME.search(item.description):
        return "test-or-deprecated"
    if CONJURED.search(item.name):
        return "conjured"
    if ALCOHOL.search(item.name):
        return "alcohol"
    if PET_ONLY.search(item.name):
        return "pet-only"
    if EVENT_ITEM.search(item.name) or item.entry in EVENT_ITEM_IDS:
        return "event-item"
    if recovery_kind(source) is None:
        return "not-directly-edible"
    native = native_well_fed(source, spells)
    if native and not native_is_supported(native):
        return "unsupported-native-utility"
    return None


def build_profiles(
    items: list[Item],
    spells: dict[int, Spell],
    cooked_item_ids: set[int] | None = None,
) -> tuple[list[Profile], list[tuple[Item, str]]]:
    cooked_item_ids = cooked_item_ids or set()
    profiles: list[Profile] = []
    excluded: list[tuple[Item, str]] = []
    for item in items:
        source = spells.get(item.spell_id)
        reason = exclusion_reason(item, source, spells)
        if reason:
            excluded.append((item, reason))
            continue

        assert source is not None
        kind = recovery_kind(source)
        assert kind is not None
        native = native_well_fed(source, spells)
        family, classification = classify_family(item, kind, native)
        premium = native is not None
        tier = tier_for(item, premium)
        minimum_level = item.required_level or TIER_LEVELS[tier]
        if premium and tier == 9 and item.required_level >= 70:
            minimum_level = item.required_level
        amount_1, amount_2, amount_3 = amounts(family, tier, premium)
        profiles.append(Profile(
            item=item,
            source_spell=source,
            meal_aura_spell=find_meal_aura(source, spells),
            tier=tier,
            grade=1 if premium else 0,
            cooked=kind == "food" and item.entry in cooked_item_ids,
            family=family,
            marker_spell=FAMILY_TEMPLATES[family],
            amount_1=amount_1,
            amount_2=amount_2,
            amount_3=amount_3,
            minimum_level=max(1, minimum_level),
            duration_seconds=3600 if premium else 1800,
            recovery_kind=kind,
            native_aura=native.spell_id if native else 0,
            classification=classification,
        ))
    return profiles, excluded


def sql_string(value: str) -> str:
    return "'" + value.replace("\\", "\\\\").replace("'", "''") + "'"


def write_sql(path: Path, profiles: list[Profile], spells: dict[int, Spell]) -> None:
    stock_auras = sorted(
        spell.spell_id for spell in spells.values()
        if "well fed" in spell.name.lower() and 6 in spell.effects
    )
    lines = [
        "-- Generated by generate-nourishment-profiles.py; do not hand edit.",
        "CREATE TABLE IF NOT EXISTS `mod_homebrew_nourishment_profile` (",
        "  `item_id` INT UNSIGNED NOT NULL,",
        "  `item_name` VARCHAR(255) NOT NULL,",
        "  `source_spell` INT UNSIGNED NOT NULL,",
        "  `meal_aura_spell` INT UNSIGNED NOT NULL,",
        "  `tier` TINYINT UNSIGNED NOT NULL,",
        "  `grade` TINYINT UNSIGNED NOT NULL DEFAULT 0,",
        "  `cooked` TINYINT UNSIGNED NOT NULL DEFAULT 0,",
        "  `family` VARCHAR(24) NOT NULL,",
        "  `marker_spell` INT UNSIGNED NOT NULL,",
        "  `amount_1` INT NOT NULL,",
        "  `amount_2` INT NOT NULL,",
        "  `amount_3` INT NOT NULL,",
        "  `minimum_level` TINYINT UNSIGNED NOT NULL,",
        "  `duration_seconds` INT UNSIGNED NOT NULL,",
        "  `recovery_kind` VARCHAR(16) NOT NULL,",
        "  `native_well_fed_aura` INT UNSIGNED NOT NULL DEFAULT 0,",
        "  `classification` VARCHAR(32) NOT NULL,",
        "  `enabled` TINYINT UNSIGNED NOT NULL DEFAULT 1,",
        "  PRIMARY KEY (`item_id`), KEY `idx_source_spell` (`source_spell`),",
        "  KEY `idx_family_tier` (`family`, `tier`)",
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;",
        "CREATE TABLE IF NOT EXISTS `mod_homebrew_nourishment_stock_aura` (",
        "  `spell_id` INT UNSIGNED NOT NULL, PRIMARY KEY (`spell_id`)",
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;",
        "DELETE FROM `mod_homebrew_nourishment_profile`;",
        "DELETE FROM `mod_homebrew_nourishment_stock_aura`;",
    ]
    if profiles:
        lines.append(
            "INSERT INTO `mod_homebrew_nourishment_profile` "
            "(`item_id`,`item_name`,`source_spell`,`meal_aura_spell`,`tier`,`grade`,`cooked`,`family`,`marker_spell`,"
            "`amount_1`,`amount_2`,`amount_3`,`minimum_level`,`duration_seconds`,"
            "`recovery_kind`,`native_well_fed_aura`,`classification`,`enabled`) VALUES"
        )
        values = []
        for profile in profiles:
            values.append(
                "({},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},1)".format(
                    profile.item.entry, sql_string(profile.item.name), profile.source_spell.spell_id,
                    profile.meal_aura_spell,
                    profile.tier, profile.grade, int(profile.cooked), sql_string(profile.family), profile.marker_spell,
                    profile.amount_1, profile.amount_2, profile.amount_3, profile.minimum_level,
                    profile.duration_seconds, sql_string(profile.recovery_kind), profile.native_aura,
                    sql_string(profile.classification),
                )
            )
        lines.append(",\n".join(values) + ";")
    if stock_auras:
        lines.append(
            "INSERT INTO `mod_homebrew_nourishment_stock_aura` (`spell_id`) VALUES\n" +
            ",\n".join(f"({spell_id})" for spell_id in stock_auras) + ";"
        )
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def write_audit(path: Path, items: list[Item], profiles: list[Profile], excluded: list[tuple[Item, str]]) -> None:
    included = {profile.item.entry: profile for profile in profiles}
    excluded_by_id = {item.entry: reason for item, reason in excluded}
    with path.open("w", encoding="utf-8", newline="") as destination:
        writer = csv.writer(destination, delimiter="\t")
        writer.writerow((
            "item_id", "item_name", "status", "reason", "tier", "grade", "cooked", "family",
            "minimum_level", "duration_seconds", "source_spell", "meal_aura", "native_aura", "kind",
        ))
        for item in items:
            profile = included.get(item.entry)
            if profile:
                writer.writerow((
                    item.entry, item.name, "included", profile.classification, profile.tier,
                    "premium" if profile.grade else "ordinary", "yes" if profile.cooked else "no", profile.family,
                    profile.minimum_level, profile.duration_seconds, profile.source_spell.spell_id,
                    profile.meal_aura_spell, profile.native_aura, profile.recovery_kind,
                ))
            else:
                writer.writerow((item.entry, item.name, "excluded", excluded_by_id[item.entry], "", "", "", "", "", "", item.spell_id, "", "", ""))


def validate(profiles: list[Profile], spells: dict[int, Spell]) -> None:
    if not profiles:
        raise RuntimeError("No nourishment profiles were generated")
    for family, marker in FAMILY_TEMPLATES.items():
        if marker not in spells:
            raise RuntimeError(f"Missing client-known marker spell {marker} for {family}")
    seen = set()
    for profile in profiles:
        if profile.item.entry in seen:
            raise RuntimeError(f"Duplicate item profile {profile.item.entry}")
        seen.add(profile.item.entry)
        if not 1 <= profile.tier <= 9 or profile.family not in FAMILY_TEMPLATES:
            raise RuntimeError(f"Invalid profile for item {profile.item.entry}")
        if not profile.meal_aura_spell:
            raise RuntimeError(f"No sustained meal aura found for item {profile.item.entry} ({profile.item.name})")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--spell-dbc", type=Path, required=True)
    parser.add_argument("--skill-line-ability-dbc", type=Path)
    parser.add_argument("--mysql-command", default="mysql")
    parser.add_argument("--mysql-defaults-extra-file", type=Path)
    parser.add_argument("--world-database", default="acore_world")
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--audit-output", type=Path, required=True)
    args = parser.parse_args()

    spells = load_spells(args.spell_dbc)
    skill_line_ability = args.skill_line_ability_dbc or args.spell_dbc.with_name("SkillLineAbility.dbc")
    if not skill_line_ability.exists():
        raise RuntimeError(f"Missing Cooking recipe data: {skill_line_ability}")
    cooked_item_ids = load_cooking_outputs(skill_line_ability, args.spell_dbc, spells)
    items = load_items(args.mysql_command, args.world_database, args.mysql_defaults_extra_file)
    profiles, excluded = build_profiles(items, spells, cooked_item_ids)
    validate(profiles, spells)
    write_sql(args.output, profiles, spells)
    write_audit(args.audit_output, items, profiles, excluded)

    by_tier = collections.Counter(profile.tier for profile in profiles)
    by_family = collections.Counter(profile.family for profile in profiles)
    by_grade = collections.Counter(profile.grade for profile in profiles)
    by_cooked = collections.Counter(profile.cooked for profile in profiles)
    print(f"Generated {len(profiles)} profiles; excluded {len(excluded)} of {len(items)} food/drink templates.")
    print("Tiers: " + ", ".join(f"T{tier}={by_tier[tier]}" for tier in range(1, 10)))
    print("Families: " + ", ".join(f"{family}={by_family[family]}" for family in FAMILY_TEMPLATES))
    print(f"Grades: ordinary={by_grade[0]}, premium={by_grade[1]}")
    print(f"Cooking outputs: cooked-food={by_cooked[True]}, other={by_cooked[False]}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
