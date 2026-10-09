import os
from pathlib import Path
import re
import runpy
import shutil
import subprocess
import tempfile


HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
method = runpy.run_path(str(HERE.parent / 'client_compat/run.py'))['method']


def main():
    source = (ROOT / 'src/server/coa/AscensionCompat.cpp').read_text(encoding='utf-8')
    harness = (HERE / 'harness.cpp').read_text(encoding='utf-8')
    constants = ('APPEARANCE_CATEGORY_COUNT', 'MAX_APPEARANCE_OUTFIT_NAME_BYTES',
                 'MAX_APPEARANCE_OUTFITS', 'SMSG_SAVE_APPEARANCE_OUTFIT_RESULT',
                 'SMSG_DELETE_APPEARANCE_OUTFIT_RESULT')
    harness = harness.replace('// ACTUAL_CONSTANTS', '\n'.join(
        re.search(r'^constexpr [\w:]+ ' + name + r' = [^;]+;$', source, re.M)[0]
        for name in constants))
    for marker, signature in (
        ('SEND_RESULT', 'static void SendOutfitResult('),
        ('VALID_NAME', 'static bool ValidOutfitName('),
        ('SAVE', 'void HandleSaveOutfit('),
        ('DELETE', 'void HandleDeleteOutfit('),
    ):
        harness = harness.replace('// ACTUAL_' + marker, method(source, signature))
    compiler = shutil.which(os.environ.get('CXX', 'cl.exe' if os.name == 'nt' else 'c++'))
    assert compiler, 'Enable a C++20 compiler.'
    with tempfile.TemporaryDirectory(prefix='coa-outfit-persistence-') as directory:
        out = Path(directory)
        cpp = out / 'harness.cpp'
        cpp.write_text(harness, encoding='utf-8')
        executable = out / ('regressions.exe' if os.name == 'nt' else 'regressions')
        if Path(compiler).stem.lower() == 'cl':
            flags = ['/nologo', '/std:c++20', '/EHsc', '/utf-8', str(cpp), '/Fe' + str(executable)]
        else:
            flags = ['-std=c++20', '-Wall', '-Wextra', '-Werror', str(cpp), '-o', str(executable)]
        subprocess.run([compiler, *flags], cwd=out, check=True, timeout=60)
        subprocess.run([str(executable)], cwd=out, check=True, timeout=15)


if __name__ == '__main__':
    main()
