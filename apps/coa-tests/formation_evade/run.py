import os
from pathlib import Path
import runpy
import shutil
import subprocess
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
method = runpy.run_path(str(HERE.parent / 'client_compat/run.py'))['method']


def main():
    source = (ROOT / 'src/server/game/Entities/Creature/CreatureGroups.cpp').read_text(encoding='utf-8')
    header = (ROOT / 'src/server/game/Entities/Creature/CreatureGroups.h').read_text(encoding='utf-8')
    flags = header[header.index('enum class GroupAIFlags'):header.index('struct FormationInfo')]
    methods = method(source, 'void CreatureGroup::MemberEvaded(')
    if 'void CreatureGroup::RespawnRemovedMembers(' in source:
        methods += '\n' + method(source, 'void CreatureGroup::RespawnRemovedMembers(')
    code = (HERE / 'harness.cpp').read_text(encoding='utf-8')
    code = code.replace('// NATIVE_FLAGS', flags).replace('// NATIVE_METHODS', methods)
    compiler = shutil.which(os.environ.get('CXX', 'cl.exe' if os.name == 'nt' else 'c++'))
    assert compiler, 'A C++20 compiler is required'
    with tempfile.TemporaryDirectory(prefix='coa-formation-evade-') as directory:
        out = Path(directory)
        path = out / 'harness.cpp'
        path.write_text(code, encoding='utf-8')
        executable = out / ('harness.exe' if os.name == 'nt' else 'harness')
        if Path(compiler).stem.lower() == 'cl':
            flags = ['/nologo', '/std:c++20', '/EHsc', '/W4', '/WX', str(path), '/Fe' + str(executable)]
        else:
            flags = ['-std=c++20', '-Wall', '-Wextra', '-Werror', str(path), '-o', str(executable)]
        subprocess.run([compiler, *flags], cwd=out, check=True, timeout=120)
        subprocess.run([str(executable)], check=True, timeout=30)


if __name__ == '__main__':
    main()
