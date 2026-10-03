CLI_DESCRIPTION = """Exercise Bloodfang Bite's Bite Wound application and its melee leech with the actual scripts."""
import argparse
import os
from pathlib import Path
import re
import runpy
import shutil
import subprocess
import sys
import tempfile

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from source_paths import git_source  # noqa: E402

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
PATH = 'src/server/coa/AscensionBloodmageBiteWound.cpp'
method = runpy.run_path(str(HERE.parent / 'client_compat/run.py'))['method']


def compile_and_run(code):
    vc_tools = os.environ.get('VCToolsInstallDir')
    compiler = (str(Path(vc_tools) / 'bin/Hostx64/x64/cl.exe') if vc_tools else
                shutil.which(os.environ.get('CXX', 'cl.exe' if os.name == 'nt' else 'c++')))
    if not compiler:
        raise RuntimeError('Enable a C++20 compiler (VS Developer PowerShell on Windows).')
    with tempfile.TemporaryDirectory(prefix='coa-bite-wound-') as directory:
        out = Path(directory)
        cpp = out / 'bite_wound.cpp'
        exe = out / ('bite_wound.exe' if os.name == 'nt' else 'bite_wound')
        cpp.write_text(code, encoding='utf-8')
        if Path(compiler).stem.lower() == 'cl':
            flags = ['/nologo', '/std:c++20', '/EHsc', '/W4', '/WX', '/utf-8', '/UNDEBUG', str(cpp), '/Fe' + str(exe)]
        else:
            flags = ['-std=c++20', '-Wall', '-Wextra', '-Werror', '-UNDEBUG', str(cpp), '-o', str(exe)]
        subprocess.run([compiler, *flags], cwd=out, check=True, timeout=120)
        subprocess.run([str(exe)], cwd=out, check=True, timeout=15)


def main():
    parser = argparse.ArgumentParser(description=CLI_DESCRIPTION)
    parser.add_argument('--source-ref', help='Read the Bite Wound scripts from a local Git ref as a control.')
    args = parser.parse_args()
    source = (git_source(['git', 'show', f'{args.source_ref}:{PATH}'], cwd=ROOT).decode('utf-8')
              if args.source_ref else (ROOT / PATH).read_text(encoding='utf-8'))
    source = re.sub(r'^#include.*\n', '', source, flags=re.M)
    defines = (ROOT / 'src/server/shared/SharedDefines.h').read_text(encoding='utf-8')
    enums = '\n'.join(method(defines, 'enum ' + name + '\n') + ';' for name in ('Classes', 'SpellMissInfo'))
    code = (HERE / 'harness.cpp').read_text(encoding='utf-8')
    compile_and_run(code.replace('// ENUMS', enums).replace('// SOURCE', source))
    print('PASS: Bloodfang Bite ranks apply an owned Bite Wound on a landed hit and melee leeches the passive percent')


if __name__ == '__main__':
    main()
