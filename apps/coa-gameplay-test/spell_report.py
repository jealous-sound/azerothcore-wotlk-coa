#!/usr/bin/env python3
"""Validate and summarize the effective spell report written by the `coa spellreport <file>` console command.

`run.py` calls `check()` on a scenario's declared artifacts; the command line validates a report kept from an
earlier run. Both check the schema and internal references and recompute every summary count from the per-spell
rows, then cross-check the report against independent sources:

- `--dbc Spell.dbc`: raw values (effective values overlaid with the listed raw differences) must equal the
  3.3.5a Spell.dbc row read at its documented field offsets.
- `--source-root <repo>`: dispatch kinds must match the SpellEffects[]/AuraEffectHandler[] source tables and
  the trigger-aura list must match SpellMgr.cpp.
- `--same-as <report>`: two reports must be identical apart from their generation time.
"""

import argparse
from collections import Counter
import json
from pathlib import Path
import re
import struct
import sys

SCHEMA = 'coa-effective-spell-report-v1'
TOTAL_SPELL_EFFECTS = 199
TOTAL_AURAS = 367
CUSTOM_CLASSES = set(range(12, 33))
MAX_SPELLMOD = 32
ROOT_SOURCES = (
    'class_spell', 'live_baseline_spell', 'live_baseline_proficiency', 'class_proficiency', 'talent_proficiency',
    'unresolved_trainer_spell', 'coa_talent', 'coa_automatic_entry', 'progression_rank', 'taught_ability',
    'talent_replacement', 'racial_skill_line', 'player_create', 'dbc_talent', 'learn_spell_effect',
    'class_skill_line', 'skill_line_class_mask',
)
SKILL_LINE_SOURCES = {'class_skill_line', 'skill_line_class_mask'}
TABLE_KINDS = {'handler', 'null', 'unused', 'missing', 'out_of_range'}
AURA_TABLE_KINDS = TABLE_KINDS | {'no_immediate'}
SILENT_KINDS = {'null', 'unused', 'missing', 'out_of_range'}
UNAPPLIED_AURA_KINDS = {'none', 'not_applied'}
SCALAR_FIELDS = ('effect', 'aura', 'base_points', 'die_sides', 'misc', 'misc_b', 'trigger', 'target_a', 'target_b')
RAW_SLOT_FIELDS = SCALAR_FIELDS + ('class_mask',)
LINK_KINDS = ('cast', 'hit', 'aura', 'remove')
EDGE_KINDS = {'trigger', 'misc', 'raw_trigger', 'raw_misc'} | set(LINK_KINDS)
TOP_KEYS = {'schema', 'server', 'generated_unix', 'max_depth', 'limits', 'dispatch', 'roots', 'summary',
            'missing_spells', 'spells'}
ROOT_KEYS = {'spell', 'exists', 'grant', 'classes', 'sources'}
RANK_KEYS = {'first', 'prev', 'next', 'last', 'rank'}
SPELL_KEYS = {'id', 'name', 'effective', 'depth', 'via', 'root_scope', 'classes', 'roots', 'proc_flags',
              'proc_chance', 'proc_entry', 'bonus_data', 'scripts', 'rank', 'linked', 'raw', 'slots'}
SLOT_KEYS = {'slot', *SCALAR_FIELDS, 'class_mask', 'effect_handler', 'aura_handler', 'trigger_aura', 'raw'}
METRIC_COUNTS = (
    'spells', 'slots', 'silent_slots', 'no_immediate_aura_slots', 'dummy_slots_without_bindings',
    'proc_aura_spells_without_proc_entry', 'proc_aura_slots_without_proc_entry',
    'trigger_aura_spells_without_proc_entry', 'spellmod_op_out_of_range_slots', 'aura112_private_selector_slots',
    'aura112_converted_selector_slots', 'rewritten_slots', 'rewritten_spells', 'spells_with_scripts',
    'spells_with_bonus_data', 'spells_with_proc_entry', 'linked_keys_with_aliases',
)
METRIC_COUNTERS = ('effect_handlers', 'aura_handlers', 'silent_by_primitive', 'rewrites', 'masking')
PER_CLASS_KEYS = ('spells', 'silent_slots', 'dummy_slots_without_bindings', 'proc_aura_spells_without_proc_entry',
                  'rewritten_spells')

SPELL_EFFECT_DUMMY = 3
SPELL_AURA_DUMMY = 4
SPELL_AURA_PROC_TRIGGER_SPELL = 42
SPELL_AURA_PROC_TRIGGER_DAMAGE = 43
SPELL_AURA_ADD_FLAT_MODIFIER = 107
SPELL_AURA_ADD_PCT_MODIFIER = 108
SPELL_AURA_OVERRIDE_CLASS_SCRIPTS = 112
SPELL_AURA_PERIODIC_DUMMY = 226
SPELL_AURA_PROC_TRIGGER_SPELL_WITH_VALUE = 231
PRIVATE_SELECTORS = range(20000, 20018)
CONVERTED_SELECTORS = range(21000, 21011)

# Spell.dbc 3.3.5a: 234 four-byte fields. Offsets are field indices (see DBCStructure.h SpellEntry comments).
DBC_FIELDS = 234
DBC_PROC_FLAGS = 34
DBC_PROC_CHANCE = 35
DBC_SLOT_OFFSETS = {
    'effect': (71, 'I'), 'die_sides': (74, 'i'), 'base_points': (80, 'i'), 'target_a': (86, 'I'),
    'target_b': (89, 'I'), 'aura': (95, 'I'), 'misc': (110, 'i'), 'misc_b': (113, 'i'), 'trigger': (116, 'I'),
}
DBC_CLASS_MASK = 122


class ReportError(ValueError):
    pass


class Checker:
    def __init__(self, limit=50):
        self.errors = []
        self.limit = limit

    def require(self, condition, message):
        if not condition:
            self.errors.append(message)
        return bool(condition)

    def fail_if_errors(self):
        if self.errors:
            shown = self.errors[:self.limit]
            more = len(self.errors) - len(shown)
            raise ReportError('\n'.join(shown) + (f'\n... {more} more' if more > 0 else ''))


def is_int(value):
    return type(value) is int


def is_sorted_unique(values):
    return all(a < b for a, b in zip(values, values[1:]))


def read_report(path):
    with open(path, 'rb') as handle:
        return json.loads(handle.read().decode('utf-8'))


def slot_raw_values(slot):
    values = {field: slot[field] for field in RAW_SLOT_FIELDS}
    values.update(slot['raw'])
    return values


def new_metrics():
    return {**{key: 0 for key in METRIC_COUNTS}, **{key: Counter() for key in METRIC_COUNTERS}}


def accumulate(metrics, spell):
    metrics['spells'] += 1
    bound = bool(spell['scripts'])
    raw = spell['raw'] or {}
    rewritten = False
    proc_aura = False
    trigger_aura = False
    if 'proc_flags' in raw:
        metrics['rewrites']['proc_flags'] += 1
        rewritten = True
        if raw['proc_flags'] and not spell['proc_flags']:
            metrics['masking']['proc_flags_zeroed'] += 1
    if 'proc_chance' in raw:
        metrics['rewrites']['proc_chance'] += 1
        rewritten = True
    for slot in spell['slots']:
        metrics['slots'] += 1
        effect, aura, misc = slot['effect'], slot['aura'], slot['misc']
        applied = slot['aura_handler'] not in UNAPPLIED_AURA_KINDS
        if effect:
            metrics['effect_handlers'][slot['effect_handler']] += 1
        if applied:
            metrics['aura_handlers'][slot['aura_handler']] += 1
        silent = False
        if effect and slot['effect_handler'] in SILENT_KINDS:
            metrics['silent_by_primitive'][f'effect {effect}'] += 1
            silent = True
        if applied and slot['aura_handler'] in SILENT_KINDS:
            metrics['silent_by_primitive'][f'aura {aura}'] += 1
            silent = True
        metrics['silent_slots'] += silent
        metrics['no_immediate_aura_slots'] += slot['aura_handler'] == 'no_immediate'
        dummy = effect == SPELL_EFFECT_DUMMY or (applied and aura in (SPELL_AURA_DUMMY, SPELL_AURA_PERIODIC_DUMMY))
        metrics['dummy_slots_without_bindings'] += dummy and not bound
        if applied and aura in (SPELL_AURA_PROC_TRIGGER_SPELL, SPELL_AURA_PROC_TRIGGER_DAMAGE,
                                SPELL_AURA_PROC_TRIGGER_SPELL_WITH_VALUE):
            proc_aura = True
            metrics['proc_aura_slots_without_proc_entry'] += not spell['proc_entry']
        trigger_aura = trigger_aura or (applied and slot['trigger_aura'])
        if applied and aura in (SPELL_AURA_ADD_FLAT_MODIFIER, SPELL_AURA_ADD_PCT_MODIFIER):
            metrics['spellmod_op_out_of_range_slots'] += misc < 0 or misc >= MAX_SPELLMOD
        if applied and aura == SPELL_AURA_OVERRIDE_CLASS_SCRIPTS:
            metrics['aura112_private_selector_slots'] += misc in PRIVATE_SELECTORS
            metrics['aura112_converted_selector_slots'] += misc in CONVERTED_SELECTORS
        if slot['raw']:
            metrics['rewritten_slots'] += 1
            rewritten = True
            for field in slot['raw']:
                metrics['rewrites'][field] += 1
            raw_slot = slot_raw_values(slot)
            if raw_slot['effect'] and not effect:
                metrics['masking']['effect_cleared'] += 1
            if raw_slot['effect'] and raw_slot['effect'] != SPELL_EFFECT_DUMMY and effect == SPELL_EFFECT_DUMMY:
                metrics['masking']['effect_to_dummy'] += 1
            if raw_slot['aura'] and raw_slot['aura'] != SPELL_AURA_DUMMY and aura == SPELL_AURA_DUMMY and applied:
                metrics['masking']['aura_to_dummy'] += 1
            if raw_slot['trigger'] and not slot['trigger']:
                metrics['masking']['trigger_cleared'] += 1
    metrics['proc_aura_spells_without_proc_entry'] += proc_aura and not spell['proc_entry']
    metrics['trigger_aura_spells_without_proc_entry'] += trigger_aura and not spell['proc_entry']
    metrics['rewritten_spells'] += rewritten
    metrics['spells_with_scripts'] += bound
    metrics['spells_with_bonus_data'] += spell['bonus_data']
    metrics['spells_with_proc_entry'] += spell['proc_entry']
    metrics['linked_keys_with_aliases'] += sum(bool(linked['key_aliases']) for linked in spell['linked'])


def plain(metrics):
    return {key: (dict(value) if isinstance(value, Counter) else int(value)) for key, value in metrics.items()}


def recompute_summary(report):
    effective = [spell for spell in report['spells'] if spell['effective']]
    all_metrics, grant_metrics = new_metrics(), new_metrics()
    per_class = {}
    for spell in effective:
        accumulate(all_metrics, spell)
        if spell['root_scope'] == 'grant':
            accumulate(grant_metrics, spell)
        for class_id in spell['classes']:
            accumulate(per_class.setdefault(str(class_id), new_metrics()), spell)
    by_source = Counter(source for root in report['roots'] for source in root['sources'])
    return {
        'roots': len(report['roots']),
        'missing_roots': sum(not root['exists'] for root in report['roots']),
        'roots_by_source': {source: by_source[source] for source in ROOT_SOURCES},
        'closure_spells': len(report['spells']),
        'raw_only_spells': len(report['spells']) - len(effective),
        'missing_spells': len(report['missing_spells']),
        'all': plain(all_metrics),
        'grant': plain(grant_metrics),
        'per_class': {key: {name: int(metrics[name]) for name in PER_CLASS_KEYS} for key, metrics in per_class.items()},
    }


def check_via(check, where, via, spell_id, spells):
    parent = spells.get(via['from'])
    if not check.require(parent is not None, f'{where}: via parent {via["from"]} is not in the report'):
        return
    kind = via['kind']
    if kind in LINK_KINDS:
        check.require(via['slot'] == -1 and any(
            linked['kind'] == kind and spell_id in linked['spells'] for linked in parent['linked']),
            f'{where}: parent {via["from"]} has no {kind} link to {spell_id}')
        return
    slots = [slot for slot in parent['slots'] if slot['slot'] == via['slot']]
    if not check.require(len(slots) == 1, f'{where}: parent {via["from"]} has no slot {via["slot"]}'):
        return
    slot = slots[0]
    value = {'trigger': slot['trigger'], 'misc': slot['misc']}.get(kind)
    if kind == 'raw_trigger':
        value = slot_raw_values(slot)['trigger']
    elif kind == 'raw_misc':
        value = slot_raw_values(slot)['misc']
    check.require(value == spell_id, f'{where}: parent {via["from"]} slot {via["slot"]} {kind} is not {spell_id}')


def validate(report):
    check = Checker()
    if not isinstance(report, dict) or report.keys() != TOP_KEYS:
        raise ReportError(f'Top-level keys differ from {sorted(TOP_KEYS)}')
    check.require(report['schema'] == SCHEMA, f'Unsupported schema {report["schema"]!r}')
    check.require(isinstance(report['server'], str), 'server must be a string')
    check.require(is_int(report['generated_unix']), 'generated_unix must be an integer')
    check.require(is_int(report['max_depth']) and 1 <= report['max_depth'] <= 16, 'max_depth must be 1..16')
    check.require(isinstance(report['limits'], list) and all(isinstance(x, str) for x in report['limits']),
                  'limits must be strings')
    check.fail_if_errors()

    dispatch = report['dispatch']
    check.require(isinstance(dispatch, dict) and dispatch.keys() == {'effects', 'auras', 'trigger_auras'},
                  'dispatch keys differ')
    check.fail_if_errors()
    effects, auras, trigger_auras = dispatch['effects'], dispatch['auras'], dispatch['trigger_auras']
    check.require(len(effects) == TOTAL_SPELL_EFFECTS and set(effects) <= TABLE_KINDS, 'dispatch.effects invalid')
    check.require(len(auras) == TOTAL_AURAS and set(auras) <= AURA_TABLE_KINDS, 'dispatch.auras invalid')
    check.require(all(is_int(x) and 0 <= x < TOTAL_AURAS for x in trigger_auras) and is_sorted_unique(trigger_auras),
                  'dispatch.trigger_auras invalid')
    check.fail_if_errors()
    trigger_aura_set = set(trigger_auras)

    roots = {}
    previous = 0
    for root in report['roots']:
        where = f'root {root.get("spell")}'
        if not check.require(isinstance(root, dict) and root.keys() == ROOT_KEYS, f'{where}: keys differ'):
            continue
        spell_id = root['spell']
        check.require(is_int(spell_id) and spell_id > previous, f'{where}: roots must be sorted unique ids')
        previous = spell_id if is_int(spell_id) else previous
        check.require(type(root['exists']) is bool and type(root['grant']) is bool, f'{where}: flags must be booleans')
        check.require(root['classes'] and is_sorted_unique(root['classes']) and set(root['classes']) <= CUSTOM_CLASSES,
                      f'{where}: classes must be sorted custom class ids')
        sources = root['sources']
        check.require(sources and set(sources) <= set(ROOT_SOURCES)
                      and sources == [s for s in ROOT_SOURCES if s in sources], f'{where}: invalid sources')
        direct_grant = any(s not in SKILL_LINE_SOURCES and s != 'learn_spell_effect' for s in sources)
        if direct_grant:
            check.require(root['grant'], f'{where}: a module grant source must set grant')
        if set(sources) <= SKILL_LINE_SOURCES:
            check.require(not root['grant'], f'{where}: skill-line-only roots are not grants')
        roots[spell_id] = root
    check.fail_if_errors()

    spells = {}
    previous = 0
    for spell in report['spells']:
        where = f'spell {spell.get("id")}'
        if not check.require(isinstance(spell, dict) and spell.keys() == SPELL_KEYS, f'{where}: keys differ'):
            continue
        check.require(is_int(spell['id']) and spell['id'] > previous, f'{where}: spells must be sorted unique ids')
        previous = spell['id'] if is_int(spell['id']) else previous
        spells[spell['id']] = spell
    check.fail_if_errors()

    for spell_id, spell in spells.items():
        where = f'spell {spell_id}'
        check.require(isinstance(spell['name'], str), f'{where}: name must be a string')
        check.require(type(spell['effective']) is bool, f'{where}: effective must be boolean')
        depth = spell['depth']
        check.require(is_int(depth) and 0 <= depth <= report['max_depth'], f'{where}: depth out of range')
        if depth == 0:
            check.require(spell['via'] is None and spell_id in roots and spell['effective'],
                          f'{where}: depth 0 must be an effective root without via')
        else:
            via = spell['via']
            if check.require(isinstance(via, dict) and via.keys() == {'from', 'slot', 'kind'}
                             and via['kind'] in EDGE_KINDS and is_int(via['slot']) and -1 <= via['slot'] <= 2,
                             f'{where}: invalid via'):
                check_via(check, where, via, spell_id, spells)
                if spell['effective']:
                    check.require(not via['kind'].startswith('raw_')
                                  and spells.get(via['from'], {}).get('effective', False),
                                  f'{where}: an effective spell must be reached through effective edges')
        check.require(spell['root_scope'] in ('grant', 'skill_line'), f'{where}: invalid root_scope')
        spell_roots = spell['roots']
        check.require(spell_roots and is_sorted_unique(spell_roots) and all(r in roots for r in spell_roots),
                      f'{where}: roots must be sorted known root ids')
        if all(r in roots for r in spell_roots) and spell_roots:
            classes = sorted(set().union(*(roots[r]['classes'] for r in spell_roots)))
            check.require(spell['classes'] == classes, f'{where}: classes differ from its roots')
            grant = any(roots[r]['grant'] for r in spell_roots)
            check.require(spell['root_scope'] == ('grant' if grant else 'skill_line'),
                          f'{where}: root_scope differs from its roots')
        for key in ('proc_flags', 'proc_chance'):
            check.require(is_int(spell[key]) and spell[key] >= 0, f'{where}: {key} must be a non-negative integer')
        check.require(type(spell['proc_entry']) is bool and type(spell['bonus_data']) is bool,
                      f'{where}: proc_entry and bonus_data must be booleans')
        check.require(isinstance(spell['scripts'], list) and all(isinstance(s, str) and s for s in spell['scripts']),
                      f'{where}: scripts must be names')
        rank = spell['rank']
        check.require(rank is None or (isinstance(rank, dict) and rank.keys() == RANK_KEYS
                                       and all(is_int(v) for v in rank.values())), f'{where}: invalid rank')
        for linked in spell['linked']:
            check.require(isinstance(linked, dict) and linked.keys() == {'kind', 'spells', 'key_aliases'}
                          and linked['kind'] in LINK_KINDS and linked['spells']
                          and all(is_int(x) and x != 0 for x in linked['spells'])
                          and all(is_int(x) and x > 0 for x in linked['key_aliases']), f'{where}: invalid linked row')
        raw = spell['raw']
        check.require(raw is None or (isinstance(raw, dict) and set(raw) <= {'proc_flags', 'proc_chance'}
                                      and all(is_int(v) and v != spell[k] for k, v in raw.items())),
                      f'{where}: raw spell fields must list only differing proc values')
        indices = [slot.get('slot') for slot in spell['slots'] if isinstance(slot, dict)]
        check.require(len(indices) == len(spell['slots']) and is_sorted_unique(indices)
                      and all(is_int(i) and 0 <= i <= 2 for i in indices), f'{where}: slots must be sorted 0..2')
        for slot in spell['slots']:
            check_slot(check, f'{where} slot {slot.get("slot")}', slot, effects, auras, trigger_aura_set)

    previous = 0
    for missing in report['missing_spells']:
        where = f'missing spell {missing.get("id")}'
        if not check.require(isinstance(missing, dict) and missing.keys() == {'id', 'from', 'slot', 'kind'},
                             f'{where}: keys differ'):
            continue
        check.require(is_int(missing['id']) and missing['id'] > previous and missing['id'] not in spells,
                      f'{where}: missing spells must be sorted unique ids absent from spells')
        previous = missing['id'] if is_int(missing['id']) else previous
        if missing['kind'] == 'root':
            check.require(missing['id'] in roots and not roots[missing['id']]['exists'], f'{where}: unknown root')
        elif check.require(missing['kind'] in EDGE_KINDS, f'{where}: invalid kind'):
            check_via(check, where, missing, missing['id'], spells)
    for spell_id, root in roots.items():
        check.require(root['exists'] == (spell_id in spells), f'root {spell_id}: exists flag differs from spells')
    check.fail_if_errors()

    expected = recompute_summary(report)
    if report['summary'] != expected:
        for key in expected:
            check.require(report['summary'].get(key) == expected[key],
                          f'summary.{key}: reported {json.dumps(report["summary"].get(key))}, '
                          f'recomputed {json.dumps(expected[key])}')
        check.require(report['summary'].keys() == expected.keys(), 'summary keys differ')
    check.fail_if_errors()
    return expected


def check_slot(check, where, slot, effects, auras, trigger_auras):
    if not check.require(slot.keys() == SLOT_KEYS, f'{where}: keys differ'):
        return
    check.require(all(is_int(slot[field]) for field in SCALAR_FIELDS), f'{where}: scalar fields must be integers')
    check.require(isinstance(slot['class_mask'], list) and len(slot['class_mask']) == 3
                  and all(is_int(x) and x >= 0 for x in slot['class_mask']), f'{where}: invalid class_mask')
    effect, aura = slot['effect'], slot['aura']
    raw = slot['raw']
    check.require(isinstance(raw, dict) and set(raw) <= set(RAW_SLOT_FIELDS)
                  and all(value != slot[field] for field, value in raw.items()),
                  f'{where}: raw must list only differing fields')
    check.require(effect or raw.get('effect'), f'{where}: slots need an effective or raw effect')
    if effect:
        expected = effects[effect] if 0 <= effect < TOTAL_SPELL_EFFECTS else 'out_of_range'
        check.require(slot['effect_handler'] == expected, f'{where}: effect_handler differs from dispatch')
    else:
        check.require(slot['effect_handler'] == 'none', f'{where}: effect 0 must have effect_handler none')
    kind = slot['aura_handler']
    if kind == 'none':
        pass
    elif kind == 'not_applied':
        check.require(aura != 0, f'{where}: not_applied needs an aura id')
    else:
        expected = auras[aura] if 0 <= aura < TOTAL_AURAS else 'out_of_range'
        check.require(aura != 0 and kind == expected, f'{where}: aura_handler differs from dispatch')
    check.require(slot['trigger_aura'] == (aura != 0 and aura in trigger_auras), f'{where}: trigger_aura differs')


def check_expected_roots(report, expectations):
    check = Checker()
    roots = {root['spell']: root for root in report['roots']}
    for expectation in expectations:
        spell_text, _, source = expectation.partition(':')
        root = roots.get(int(spell_text))
        if check.require(root is not None, f'expected root {spell_text} is absent') and source:
            check.require(source in root['sources'], f'expected root {spell_text} lacks source {source}')
    check.fail_if_errors()


def parse_handler_table(text, anchor, prefix, names):
    start = text.find(anchor)
    if start < 0:
        raise ReportError(f'Cannot find {anchor}')
    body = text[start:]
    body = body[:body.index('};')]
    rows = {}
    for match in re.finditer(r'^\s*(?:&' + prefix + r'::(\w+)|(nullptr))\s*,\s*//\s*(\d+)', body, re.M):
        handler = match.group(1) or 'nullptr'
        rows[int(match.group(3))] = names.get(handler, 'handler')
    return rows


def check_source(report, root):
    check = Checker()
    root = Path(root)
    effects = parse_handler_table(
        (root / 'src/server/game/Spells/SpellEffects.cpp').read_text(encoding='utf-8', errors='replace'),
        'pEffect SpellEffects[TOTAL_SPELL_EFFECTS]', 'Spell',
        {'EffectNULL': 'null', 'EffectUnused': 'unused', 'nullptr': 'missing'})
    auras = parse_handler_table(
        (root / 'src/server/game/Spells/Auras/SpellAuraEffects.cpp').read_text(encoding='utf-8', errors='replace'),
        'pAuraEffectHandler AuraEffectHandler[TOTAL_AURAS]', 'AuraEffect',
        {'HandleNULL': 'null', 'HandleUnused': 'unused', 'HandleNoImmediateEffect': 'no_immediate',
         'nullptr': 'missing'})
    for name, table, reported in (('effect', effects, report['dispatch']['effects']),
                                  ('aura', auras, report['dispatch']['auras'])):
        check.require(sorted(table) == list(range(len(reported))), f'source {name} table ids do not cover the report')
        for index, kind in table.items():
            if index < len(reported):
                check.require(reported[index] == kind, f'{name} {index}: report {reported[index]}, source {kind}')
    defines = (root / 'src/server/game/Spells/Auras/SpellAuraDefines.h').read_text(encoding='utf-8', errors='replace')
    values = {name: int(value) for name, value in re.findall(r'\b(SPELL_AURA_\w+)\s*=\s*(\d+)', defines)}
    manager = (root / 'src/server/game/Spells/SpellMgr.cpp').read_text(encoding='utf-8', errors='replace')
    names = re.findall(r'^\s*isTriggerAura\[(SPELL_AURA_\w+)\]\s*=\s*true\s*;', manager, re.M)
    check.require(names and all(name in values for name in names), 'cannot resolve isTriggerAura entries')
    check.require(sorted({values[name] for name in names if name in values}) == report['dispatch']['trigger_auras'],
                  'dispatch.trigger_auras differs from SpellMgr.cpp isTriggerAura')
    check.fail_if_errors()


def read_dbc(path):
    data = Path(path).read_bytes()
    magic, records, fields, record_size, _ = struct.unpack_from('<4s4I', data)
    if magic != b'WDBC' or fields != DBC_FIELDS or record_size != DBC_FIELDS * 4:
        raise ReportError(f'{path} is not a 3.3.5a Spell.dbc')
    rows = {}
    for index in range(records):
        offset = 20 + index * record_size
        spell_id = struct.unpack_from('<I', data, offset)[0]
        rows[spell_id] = offset
    return data, rows


def dbc_field(data, offset, field, kind):
    return struct.unpack_from('<' + kind, data, offset + field * 4)[0]


def check_dbc(report, path):
    check = Checker()
    data, rows = read_dbc(path)
    absent = 0
    for spell in report['spells']:
        where = f'spell {spell["id"]}'
        offset = rows.get(spell['id'])
        if offset is None:
            absent += 1
            continue
        if not check.require(spell['raw'] is not None, f'{where}: report has no raw row but Spell.dbc does'):
            continue
        raw = {'proc_flags': spell['proc_flags'], 'proc_chance': spell['proc_chance'], **spell['raw']}
        check.require(raw['proc_flags'] == dbc_field(data, offset, DBC_PROC_FLAGS, 'I'), f'{where}: raw proc_flags')
        check.require(raw['proc_chance'] == dbc_field(data, offset, DBC_PROC_CHANCE, 'I'), f'{where}: raw proc_chance')
        slots = {slot['slot']: slot_raw_values(slot) for slot in spell['slots']}
        for index in range(3):
            if index not in slots:
                check.require(dbc_field(data, offset, DBC_SLOT_OFFSETS['effect'][0] + index, 'I') == 0,
                              f'{where}: Spell.dbc slot {index} has an effect the report omits')
                continue
            values = slots[index]
            for field, (base, kind) in DBC_SLOT_OFFSETS.items():
                actual = dbc_field(data, offset, base + index, kind)
                check.require(values[field] == actual,
                              f'{where} slot {index}: raw {field} {values[field]} != Spell.dbc {actual}')
            mask = [dbc_field(data, offset, DBC_CLASS_MASK + 3 * index + part, 'I') for part in range(3)]
            check.require(values['class_mask'] == mask, f'{where} slot {index}: raw class_mask differs from Spell.dbc')
    check.fail_if_errors()
    return absent


def check_same(report, other):
    left = {key: value for key, value in report.items() if key != 'generated_unix'}
    right = {key: value for key, value in other.items() if key != 'generated_unix'}
    if left != right:
        differing = sorted(key for key in left.keys() | right.keys() if left.get(key) != right.get(key))
        raise ReportError(f'Reports differ in {differing}')


def format_summary(summary):
    lines = [
        f"roots {summary['roots']} (missing {summary['missing_roots']}), closure spells {summary['closure_spells']} "
        f"(raw-only {summary['raw_only_spells']}), missing spells {summary['missing_spells']}",
    ]
    for scope in ('all', 'grant'):
        metrics = summary[scope]
        counts = ', '.join(f'{key} {metrics[key]}' for key in METRIC_COUNTS)
        lines.append(f'[{scope}] {counts}')
        top = sorted(metrics['silent_by_primitive'].items(), key=lambda item: (-item[1], item[0]))[:15]
        lines.append(f'[{scope}] silent by primitive: ' + ', '.join(f'{key} {value}' for key, value in top))
        lines.append(f'[{scope}] rewrites: {json.dumps(metrics["rewrites"], sort_keys=True)}; '
                     f'masking: {json.dumps(metrics["masking"], sort_keys=True)}')
    return '\n'.join(lines)


def check(path, options, context):
    """Validate one exported report for the gameplay runner and return the evidence it records."""
    report = read_report(path)
    summary = validate(report)
    checks = ['schema', 'references', 'summary_recomputed']
    result = {'schema': report['schema'], 'server': report['server'], 'checks': checks, 'summary': summary}
    if options.get('expect_roots'):
        check_expected_roots(report, options['expect_roots'])
        checks.append('expect_roots')
    source_root = Path(context['source_root'])
    check_source(report, source_root)
    checks.append('source_tables')
    dbc = Path(context['data_dir']) / 'dbc' / 'Spell.dbc'
    if not dbc.is_file():
        raise ReportError(f'{dbc} is missing; the report cannot be checked against Spell.dbc')
    result['dbc_absent_spells'] = check_dbc(report, dbc)
    checks.append('dbc')
    if options.get('same_as'):
        check_same(report, read_report(Path(context['run_dir']) / options['same_as']))
        checks.append('same_as')
    return result


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('report', type=Path)
    parser.add_argument('--dbc', type=Path, help='Spell.dbc used by the server that wrote the report')
    parser.add_argument('--source-root', type=Path, help='Source checkout the server binary was built from')
    parser.add_argument('--same-as', type=Path, help='Second report that must match apart from generated_unix')
    parser.add_argument('--expect-root', action='append', default=[], metavar='SPELL[:SOURCE]',
                        help='Require a root spell, optionally with a source')
    parser.add_argument('--summary-json', type=Path, help='Write the validated summary to this file')
    args = parser.parse_args(argv)
    try:
        report = read_report(args.report)
        summary = validate(report)
        check_expected_roots(report, args.expect_root)
        if args.source_root:
            check_source(report, args.source_root)
        absent = check_dbc(report, args.dbc) if args.dbc else None
        if args.same_as:
            check_same(report, read_report(args.same_as))
    except (OSError, ValueError, TypeError, KeyError, AttributeError, ReportError) as error:
        print(f'Invalid effective spell report: {error}', file=sys.stderr)
        return 1
    print(f'Valid {SCHEMA} report from {report["server"]}')
    if absent is not None:
        print(f'Spell.dbc cross-check passed; {absent} report spells are not in the file (spell_dbc rows)')
    print(format_summary(summary))
    if args.summary_json:
        args.summary_json.write_text(json.dumps(summary, indent=1, sort_keys=True) + '\n', encoding='utf-8')
    return 0


if __name__ == '__main__':
    sys.exit(main())
