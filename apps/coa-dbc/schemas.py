from dataclasses import dataclass
from pathlib import Path
import json
import re


ROOT = Path(__file__).resolve().parents[2]
STRUCTURES = 'src/server/shared/DataStores/DBCStructure.h'
TALENTS = 'src/server/coa/AscensionCoATalentData.cpp'
CHARGES = 'src/server/coa/AscensionClassMechanics.cpp'
DBC_FIELD_BYTES = 4
SPELL_FIELD_COUNT = 234
SPELL_LAYOUT = 'apps/coa-dbc/spell-layout.json'
LOCALES = ['enUS', 'koKR', 'frFR', 'deDE', 'zhCN', 'zhTW', 'esES', 'esMX', 'ruRU']


@dataclass(frozen=True)
class Field:
    name: str
    offset: int
    kind: str = 'I'
    target: str | None = None
    enum: str | None = None
    unit: str | None = None


@dataclass(frozen=True)
class Schema:
    fields: int
    size: int
    columns: tuple
    source: str
    key: str = 'ID'
    version: str = 'coa-wdbc-v1'


def column(name, index, kind='I', target=None, enum=None, unit=None):
    return Field(name, index * DBC_FIELD_BYTES, kind, target, enum, unit)


def group(name, index, count, kind='I', target=None):
    return [column(f'{name}[{i}]', index + i, kind, target) for i in range(count)]


def localized(name, index):
    return [column(f'{name}[{LOCALES[i] if i < len(LOCALES) else "locale" + str(i)}]', index + i, 's')
            for i in range(16)]


def spell_fields_from_layout(root):
    fields_by_disk_index = {}
    for name, first, last, kind in json.loads((root / SPELL_LAYOUT).read_text(encoding='utf-8')):
        if kind not in {'I', 'i', 'f', 's'} or not 0 <= first <= last < SPELL_FIELD_COUNT:
            raise ValueError('Invalid Spell disk layout field')
        for i in range(first, last + 1):
            field_name = name if first == last else f'{name}[{i-first}]'
            if kind == 's':
                locale = LOCALES[i-first] if i-first < len(LOCALES) else f'locale{i-first}'
                field_name = f'{name}[{locale}]'
            if i in fields_by_disk_index:
                raise ValueError(f'Duplicate Spell disk layout index: {i}')
            fields_by_disk_index[i] = column(field_name, i, kind)
    validate_native_spell_layout(root, fields_by_disk_index)
    return fields_by_disk_index


def validate_native_spell_layout(root, fields):
    source = re.sub(r'/\*.*?\*/|//[^\n]*', '', (root / STRUCTURES).read_text(encoding='utf-8'), flags=re.S)
    body = source.split('struct SpellEntry\n{', 1)[1].split('\n};', 1)[0]
    constants = dict(re.findall(r'#define\s+(\w+)\s+(\d+)', source))
    declarations = re.findall(
        r'(?:std::array<([^,>]+),\s*(\w+)>|(uint32|int32|float|flag96|char const\*))\s+(\w+)\s*;', body)
    native = []
    for element, count, scalar, name in declarations:
        element = (element or scalar).strip()
        count = int(constants.get(count, count)) if count else 1
        count *= 3 if element == 'flag96' else 1
        kind = {'float': 'f', 'int32': 'i', 'char const*': 's'}.get(element, 'I')
        for i in range(count):
            suffix = LOCALES[i] if kind == 's' and i < len(LOCALES) else f'locale{i}' if kind == 's' else str(i)
            native.append((name if count == 1 else f'{name}[{suffix}]', kind))
    formats = (root / 'src/server/shared/DataStores/DBCfmt.h').read_text(encoding='utf-8')
    fmt = re.search(r'SpellEntryfmt\[\]\s*=\s*"([a-zA-Z]+)"', formats).group(1)
    missing = sorted(set(range(SPELL_FIELD_COUNT)) - fields.keys())
    if len(fmt) != SPELL_FIELD_COUNT or missing or len(fields) != SPELL_FIELD_COUNT:
        raise ValueError(f'Spell disk layout field count changed; missing fields: {missing}')
    expected = [(fields[i].name, fields[i].kind) for i, code in enumerate(fmt) if code not in 'xX']
    if native != expected:
        raise ValueError('Spell disk layout does not match native structure and format')


def spell_schema(root):
    fields_by_disk_index = spell_fields_from_layout(root)
    refs = {'CastingTimeIndex': 'SpellCastTimes', 'DurationIndex': 'SpellDuration', 'RangeIndex': 'SpellRange',
            'EffectRadiusIndex': 'SpellRadius', 'EffectTriggerSpell': 'Spell', 'CasterAuraSpell': 'Spell',
            'TargetAuraSpell': 'Spell', 'ExcludeCasterAuraSpell': 'Spell', 'ExcludeTargetAuraSpell': 'Spell'}
    enums = {'Effect': 'SpellEffects', 'EffectApplyAuraName': 'AuraType', 'EffectImplicitTargetA': 'Targets',
             'EffectImplicitTargetB': 'Targets', 'SchoolMask': 'SpellSchoolMask', 'DmgClass': 'SpellDmgClass'}
    for i, field in fields_by_disk_index.items():
        base = field.name.split('[')[0]
        enum = base if base.startswith('Attributes') else enums.get(base)
        milliseconds = ('RecoveryTime', 'CategoryRecoveryTime', 'StartRecoveryTime', 'EffectAmplitude')
        unit = 'ms' if base in milliseconds else None
        fields_by_disk_index[i] = Field(field.name, field.offset, field.kind, refs.get(base), enum, unit)
    columns = tuple(fields_by_disk_index[i] for i in range(SPELL_FIELD_COUNT))
    return Schema(SPELL_FIELD_COUNT, SPELL_FIELD_COUNT * DBC_FIELD_BYTES, columns, SPELL_LAYOUT, key='Id')


def registry(root=ROOT):
    def schema(count, columns, source=STRUCTURES, size=None, key='ID'):
        return Schema(count, size or count * DBC_FIELD_BYTES, tuple(columns), source, key)

    identity = [column('ID', 0)]
    return {
        'Spell': spell_schema(root),
        'SpellDuration': schema(4, identity + [column('Duration', 1, 'i', unit='ms'),
            column('DurationPerLevel', 2, 'i', unit='ms/level'), column('MaxDuration', 3, 'i', unit='ms')]),
        'SpellRadius': schema(4, identity + [column('RadiusMin', 1, 'f', unit='yards'),
            column('RadiusPerLevel', 2, 'f', unit='yards/level'), column('RadiusMax', 3, 'f', unit='yards')]),
        'SpellRange': schema(40, identity + group('RangeMin', 1, 2, 'f') + group('RangeMax', 3, 2, 'f')
            + [column('Flags', 5)] + localized('DisplayName', 6) + localized('DisplayNameShort', 23)),
        'SpellCastTimes': schema(4, identity + [column('CastTime', 1, 'i', unit='ms')]),
        'Talent': schema(23, identity + [column('TalentTab', 1, target='TalentTab'), column('Row', 2),
            column('Col', 3)] + group('RankID', 4, 5, target='Spell')
            + [column('DependsOn', 13, target='Talent'), column('DependsOnRank', 16), column('AddToSpellBook', 19)]),
        'TalentTab': schema(24, identity + localized('Name', 1) + [column('ClassMask', 20),
            column('PetTalentMask', 21), column('TabPage', 22)]),
        'SkillLineAbility': schema(14, identity + [column('SkillLine', 1), column('Spell', 2, target='Spell'),
            column('RaceMask', 3), column('ClassMask', 4), column('MinSkillLineRank', 7),
            column('SupercededBySpell', 8, target='Spell'), column('AcquireMethod', 9),
            column('TrivialSkillLineRankHigh', 10), column('TrivialSkillLineRankLow', 11)]),
        'CharacterAdvancement': schema(179, identity + group('RequiredEntry', 2, 3, target='CharacterAdvancement')
            + group('RankSpell', 5, 5, target='Spell') + [column('AbilityEssenceCost', 14),
            column('TalentEssenceCost', 15), column('RequiredLevel', 26), column('Group', 29),
            column('ClassType', 32, target='CharacterAdvancementClassTypes'),
            column('Tab', 33, target='CharacterAdvancementTabTypes')], TALENTS, 692),
        'CharacterAdvancementClassTypes': schema(23, identity + [column('ClassID', 2),
            column('CustomClass', 4)], TALENTS),
        'CharacterAdvancementTabTypes': schema(19, identity + [column('Token', 1, 's')], TALENTS),
        'ChrSpecs': schema(65, identity + [column('ClassToken', 1, 's'), column('TabToken', 2, 's'),
            column('IdentityEntry', 28, target='CharacterAdvancement')], TALENTS),
        'SpellCharges': schema(2, [column('Spell', 0, target='Spell'),
            column('Category', 1, target='SpellChargesCategory')], CHARGES, key='Spell'),
        'SpellChargesCategory': schema(3, identity + [column('MaxCharges', 1),
            column('RecoveryTime', 2, unit='ms')], CHARGES),
        'SpellAddon': schema(23, identity + [column('Spell', 1, target='Spell')]
            + group('ClientAuraType', 20, 3), '.agents/docs/systems/ascension-spell-parity.md'),
        'SpellCustomAttr': schema(11, identity + [column('Spell', 1, target='Spell'),
            column('CustomAttributes', 2)], '.agents/docs/systems/ascension-spell-parity.md'),
    }
