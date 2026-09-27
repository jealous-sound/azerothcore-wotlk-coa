import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile

HERE = Path(__file__).resolve().parent
SOURCE = HERE.parents[2] / 'src/server/coa'


def achievement_seed_source():
    migration = SOURCE.parents[2] / 'data/sql/updates/pending_db_world/rev_20260926_30_patch_achievements.sql'
    sql = migration.read_text(encoding='utf-8')
    code = ['std::vector<WorldPacket> BuildSeedPatches()', '{',
            '    using namespace AscensionPatchAchievements;', '    std::vector<WorldPacket> packets;']
    tables = [('patch_achievement', 'Achievement', 3, 3),
              ('patch_achievement_criteria', 'Criteria', 1, 16),
              ('patch_achievement_category', 'Category', 1, 16)]
    for table, enum, columns, strings in tables:
        match = re.search(r'INSERT INTO `' + table + r'`[^\n]+ VALUES\n(.*?);\n', sql, re.S)
        if not match:
            raise ValueError(f'Missing seed INSERT for {table}')
        for row in match[1].splitlines():
            record = re.fullmatch(r'\((\d+), (.*)\),?', row)
            if not record:
                raise ValueError(f'Invalid seed row: {row}')
            tokens = re.findall(r"NULL|'(?:''|\\.|[^'\\])*'", record[2])
            if len(tokens) != columns or ', '.join(tokens) != record[2]:
                raise ValueError(f'Invalid seed columns: {row}')
            values = []
            for token in tokens:
                if token == 'NULL':
                    values.append('std::nullopt')
                else:
                    text = token[1:-1].replace("''", "'")
                    text = re.sub(r'\\([\\nrt])', lambda m: {'\\': '\\', 'n': '\n',
                                  'r': '\r', 't': '\t'}[m[1]], text)
                    values.append('std::string(' + json.dumps(text, ensure_ascii=False) + ')')
            values += ['std::nullopt'] * (strings - columns)
            code += ['    {', f'        auto packet = BuildPatch(Table::{enum}, {record[1]}u,',
                     '            {' + ', '.join(values) + '});', '        assert(packet);',
                     '        packets.push_back(*packet);', '    }']
    return '\n'.join(code + ['    return packets;', '}', ''])


def packet_source(family):
    code = '#include <array>\n#include <string_view>\n' + (HERE / 'packet_fixture.cpp.in').read_text()
    stem = {'spell': 'AscensionPatchSpells', 'item': 'AscensionPatchItems',
            'achievement': 'AscensionPatchAchievements', 'store': 'AscensionPatchSpellStore'}[family]
    names = [stem + '.h', stem + '.cpp']
    if family == 'achievement':
        names.insert(0, 'AscensionPatchAchievementData.h')
    if family == 'store':
        names = ['AscensionPatchSpells.h', 'AscensionPatchSpells.cpp', 'AscensionPatchSpellData.h',
                 'AscensionPatchSpellStore.h', 'AscensionPatchSpellStore.cpp']
    for name in names:
        code += re.sub(r'^#include.*\n', '', (SOURCE / name).read_text(), flags=re.M)
    if family == 'achievement':
        code += achievement_seed_source()
    cases = 'packet_cases.cpp.in' if family == 'spell' else f'{family}_packet_cases.cpp.in'
    return code + (HERE / cases).read_text()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--family', choices=('all', 'spell', 'item', 'achievement', 'store'), default='all')
    family = parser.parse_args().family
    families = ('spell', 'item', 'achievement', 'store') if family == 'all' else (family,)
    msvc = os.name == 'nt' and bool(os.environ.get('VCToolsInstallDir'))
    compiler = (str(Path(os.environ['VCToolsInstallDir']) / 'bin/Hostx64/x64/cl.exe') if msvc
                else shutil.which(os.environ.get('CXX', 'c++')))
    if not compiler:
        raise RuntimeError('A C++20 compiler or an MSVC developer shell is required')
    with tempfile.TemporaryDirectory(prefix='coa-patch-packets-') as directory:
        out = Path(directory)
        for current in families:
            cpp, exe = out / (current + '.cpp'), out / (current + ('.exe' if os.name == 'nt' else ''))
            cpp.write_text(packet_source(current), encoding='utf-8')
            if msvc:
                command = [compiler, '/nologo', '/std:c++20', '/EHsc', '/W4', '/WX', '/utf-8',
                           str(cpp), '/Fe' + str(exe)]
            else:
                command = [compiler, '-std=c++20', '-Wall', '-Wextra', '-Werror', str(cpp), '-o', str(exe)]
            subprocess.run(command, cwd=out, check=True, timeout=60)
            subprocess.run([str(exe)], cwd=out, check=True, timeout=15)
            print(f'PASS: {current} packets')


if __name__ == '__main__':
    main()
