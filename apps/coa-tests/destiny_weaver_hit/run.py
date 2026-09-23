CLI_DESCRIPTION = """Check that a Destiny Weaver view scales creature damage by the true average-hit ratio.

Compiles the module's actual level-row, average-hit and factor code against creature_classlevelstats rows from
the base world SQL, and compares it with AzerothCore's creature melee range computed independently here.
Pass --source-ref to run the same checks against the module source of another Git ref.
"""

import argparse
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from source_paths import git_source  # noqa: E402

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
MODULE = 'modules/mod-destiny-weaver/src/'
SCALING = MODULE + 'destiny_weaver_scaling.cpp'
CASES = [
    (1, 0, 5, 40, 1.0, 2000),
    (1, 0, 5, 77, 1.0, 2000),
    (1, 0, 1, 20, 1.0, 2000),
    (8, 0, 1, 30, 1.0, 2000),
    (2, 0, 12, 45, 1.0, 1500),
    (4, 0, 20, 57, 1.5, 2400),
    (1, 1, 60, 70, 1.0, 2000),
    (2, 2, 70, 77, 1.0, 2000),
    (8, 2, 68, 77, 0.5, 1000),
    (1, 0, 40, 40, 1.0, 2000),
]
RESEARCH = {(1, 0, 5, 40): (10.86, 0.01), (1, 0, 5, 77): (30.2, 0.05)}


def method(source, signature):
    start = source.index(signature)
    end = source.index('{', start) + 1
    depth = 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]


def level_rows():
    sql = (ROOT / 'data/sql/base/db_world/creature_classlevelstats.sql').read_text(encoding='utf-8')
    number = r'(-?[0-9.]+)'
    pattern = re.compile(r'^\(' + ','.join([number] * 17) + r',', re.M)
    rows = {}
    for match in pattern.finditer(sql):
        values = match.groups()
        level, unit_class = int(values[0]), int(values[1])
        rows[level, unit_class] = {
            'health': [int(v) for v in values[2:5]], 'mana': int(values[5]), 'armor': float(values[6]),
            'attack_power': int(values[7]), 'ranged_attack_power': int(values[8]),
            'damage': [float(v) for v in values[9:12]], 'stats': [int(v) for v in values[12:17]],
        }
    assert len(rows) == 400, len(rows)
    return rows


def core_melee_range():
    creature = (ROOT / 'src/server/game/Entities/Creature/Creature.cpp').read_text(encoding='utf-8')
    select = method(creature, 'void Creature::SelectLevel(')
    assert re.search(r'float weaponBaseMinDamage = basedamage;', select)
    high = float(re.search(r'float weaponBaseMaxDamage = basedamage \* ([0-9.]+);', select)[1])
    stats = (ROOT / 'src/server/game/Entities/Unit/StatSystem.cpp').read_text(encoding='utf-8')
    minmax = method(stats, 'void Creature::CalculateMinMaxDamage(')
    divisor = float(re.search(r'\(attackPower / ([0-9.]+)f\) \* variance;', minmax)[1])
    assert 'variance = GetCreatureTemplate()->BaseVariance;' in minmax
    assert 'float basePct          = GetPctModifierValue(unitMod, BASE_PCT) * attackSpeedMulti;' in minmax
    assert 'minDamage = ((weaponMinDamage + baseValue) * dmgMultiplier * basePct + totalValue) * totalPct;' in minmax
    assert 'maxDamage = ((weaponMaxDamage + baseValue) * dmgMultiplier * basePct + totalValue) * totalPct;' in minmax
    unit = (ROOT / 'src/server/game/Entities/Unit/Unit.cpp').read_text(encoding='utf-8')
    multiplier = method(unit, 'float Unit::GetAPMultiplier(')
    assert re.search(r'if \(!normalized \|\| !IsPlayer\(\)\)\s*return float\(GetAttackTime\(attType\)\) / 1000\.0f;',
                     multiplier)
    return high, divisor


def expected_factor(rows, high, divisor, unit_class, expansion, own, view, variance, attack_time):
    def average(level):
        row = rows[level, unit_class]
        speed = attack_time / 1000.0
        attack_power = row['attack_power'] / divisor * variance
        low_hit = (row['damage'][expansion] + attack_power) * speed
        high_hit = (row['damage'][expansion] * high + attack_power) * speed
        return (low_hit + high_hit) / 2.0
    return average(view) / average(own)


def harness(module_source, rows):
    creature_data = (ROOT / 'src/server/game/Entities/Creature/CreatureData.h').read_text(encoding='utf-8')
    base_stats = method(creature_data, 'struct CreatureBaseStats') + ';\n'
    level_stats = method(module_source, 'struct LevelStats') + ';\n'
    stats_at = method(module_source, 'LevelStats StatsAt(')
    hit_from = method(module_source, 'double HitFrom(')
    factor = re.search(r'view\.DamageTakenFactor = (.*?);\n', module_source, re.S)[1]
    loaded = []
    for (level, unit_class), row in sorted(rows.items()):
        loaded.append(
            f'    Load({level}, {unit_class}, {{{", ".join(map(str, row["health"]))}}}, {row["mana"]}, '
            f'{row["armor"]}f, {row["attack_power"]}, {row["ranged_attack_power"]}, '
            f'{{{", ".join(f"{v}f" for v in row["damage"])}}});')
    cases = [f'    Report({c}, {e}, {o}, {v}, {var}f, {t});' for c, e, o, v, var, t in CASES]
    return r'''#include "Define.h"
#include "destiny_weaver_view_damage.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <initializer_list>
#include <map>
#include <utility>
enum Expansions { MAX_EXPANSIONS = 3 };
struct CreatureTemplate
{
    uint32 expansion = 0;
    uint32 unit_class = 1;
    uint32 BaseAttackTime = 2000;
    float BaseVariance = 1.0f;
    float ModHealth = 1.0f;
    float ModMana = 1.0f;
    float ModArmor = 1.0f;
};
''' + base_stats + r'''
struct ObjectMgr
{
    std::map<std::pair<uint32, uint32>, CreatureBaseStats> rows;
    CreatureBaseStats const* GetCreatureBaseStats(uint8 level, uint32 unitClass) const
    {
        return &rows.at({ level, unitClass });
    }
};
ObjectMgr objectMgr;
ObjectMgr* sObjectMgr = &objectMgr;
''' + level_stats + stats_at + '\n' + hit_from + r'''
double Factor(LevelStats const& viewStats, LevelStats const& ownStats, [[maybe_unused]] CreatureTemplate const* info)
{
    struct { LevelStats Stats; } view{ viewStats };
    return ''' + factor + r''';
}
void Load(uint32 level, uint32 unitClass, std::initializer_list<uint32> health, uint32 mana, float armor,
          uint32 attackPower, uint32 rangedAttackPower, std::initializer_list<float> damage)
{
    CreatureBaseStats row{};
    std::copy(health.begin(), health.end(), row.BaseHealth);
    row.BaseMana = mana;
    row.BaseArmor = armor;
    row.AttackPower = attackPower;
    row.RangedAttackPower = rangedAttackPower;
    std::copy(damage.begin(), damage.end(), row.BaseDamage);
    objectMgr.rows[{ level, unitClass }] = row;
}
void Report(uint32 unitClass, uint32 expansion, uint8 own, uint8 view, float variance, uint32 attackTime)
{
    CreatureTemplate info;
    info.unit_class = unitClass;
    info.expansion = expansion;
    info.BaseVariance = variance;
    info.BaseAttackTime = attackTime;
    std::printf("%.9f\n", Factor(StatsAt(view, &info), StatsAt(own, &info), &info));
}
int main()
{
''' + '\n'.join(loaded) + '\n' + '\n'.join(cases) + '\n}\n'


def compile_run(code):
    compiler = shutil.which(os.environ.get('CXX', 'cl.exe' if os.name == 'nt' else 'c++'))
    assert compiler, 'Enable a C++20 compiler (VS Developer PowerShell on Windows).'
    with tempfile.TemporaryDirectory(prefix='coa-destiny-weaver-hit-') as directory:
        out = Path(directory)
        source = out / 'harness.cpp'
        source.write_text(code, encoding='utf-8')
        executable = out / ('harness.exe' if os.name == 'nt' else 'harness')
        includes = [ROOT / MODULE, ROOT / 'src/common']
        if Path(compiler).stem.lower() == 'cl':
            flags = ['/nologo', '/std:c++20', '/EHsc', '/W4', '/WX', '/utf-8', *['/I' + str(p) for p in includes],
                     str(source), '/Fe' + str(executable)]
        else:
            flags = ['-std=c++20', '-Wall', '-Wextra', '-Werror', *['-I' + str(p) for p in includes], str(source),
                     '-o', str(executable)]
        subprocess.run([compiler, *flags], cwd=out, check=True, timeout=120)
        result = subprocess.run([str(executable)], cwd=out, check=True, timeout=30, capture_output=True, text=True)
    return [float(line) for line in result.stdout.split()]


def main():
    parser = argparse.ArgumentParser(description=CLI_DESCRIPTION)
    parser.add_argument('--source-ref', help='Read the module source from a local Git ref for regression checks.')
    args = parser.parse_args()
    if args.source_ref:
        module_source = git_source(['git', 'show', f'{args.source_ref}:{SCALING}'], cwd=ROOT).decode('utf-8')
    else:
        module_source = (ROOT / SCALING).read_text(encoding='utf-8')
    rows = level_rows()
    high, divisor = core_melee_range()
    actual = compile_run(harness(module_source, rows))
    assert len(actual) == len(CASES)
    failures = []
    for case, factor in zip(CASES, actual):
        expected = expected_factor(rows, high, divisor, *case)
        if abs(factor - expected) > 1e-5 * expected:
            failures.append(f'class {case[0]} exp {case[1]} L{case[2]}->L{case[3]} variance {case[4]} '
                            f'attack {case[5]} ms: factor {factor:.4f}, true average-hit ratio {expected:.4f}')
        pinned = RESEARCH.get(case[:4])
        if pinned and abs(factor - pinned[0]) > pinned[1]:
            failures.append(f'class {case[0]} L{case[2]}->L{case[3]}: factor {factor:.4f}, research {pinned[0]}')
    assert not failures, '\n'.join(failures)
    print(f'PASS: {len(CASES)} view damage factors equal the true AzerothCore average-hit ratio '
          f'(weapon range x1..x{high:g}, AP/{divisor:g} x variance, attack time)')


if __name__ == '__main__':
    main()
