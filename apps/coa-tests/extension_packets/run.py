CLI_DESCRIPTION = """Run Ascension extension packet regressions without a server or database.

Compiles the production realm-info sender against the real WorldPacket and checks
the bytes the stock Extensions.dll reads. Pass --source-ref to test another Git ref.
"""

import argparse
import os
from pathlib import Path
import runpy
import shutil
import subprocess
import sys
import tempfile

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from source_paths import git_source  # noqa: E402

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
method = runpy.run_path(str(HERE.parent / 'client_compat/run.py'))['method']


def opcodes(source):
    start = source.index('namespace {\n') + len('namespace {\n')
    return source[start:source.index('struct ExtensionOpcodeIdentity')]


def main():
    parser = argparse.ArgumentParser(description=CLI_DESCRIPTION)
    parser.add_argument('--source-ref', help='Read production code from a local Git ref for regression checks.')
    args = parser.parse_args()

    def source(name):
        if args.source_ref:
            return git_source(['git', 'show', f'{args.source_ref}:{name}'], cwd=ROOT).decode('utf-8')
        return (ROOT / name).read_text(encoding='utf-8')

    compat = source('src/server/coa/AscensionCompat.cpp')
    buffer = source('src/server/shared/Packets/ByteBuffer.cpp')
    harness = (HERE / 'harness.cpp').read_text(encoding='utf-8')
    for marker, text in [
        ('BYTE_BUFFER', '\n'.join(method(buffer, signature) for signature in (
            'void ByteBuffer::append(uint8 const* src, std::size_t cnt)',
            'ByteBufferPositionException::ByteBufferPositionException(',
        ))),
        ('OPCODES', opcodes(compat)),
        ('SEND_REALM_INFO', method(compat, 'void SendRealmInfo(WorldSession *session')),
    ]:
        harness = harness.replace('// ACTUAL_' + marker, text)

    compiler = shutil.which(os.environ.get('CXX', 'cl.exe' if os.name == 'nt' else 'c++'))
    assert compiler, 'Enable a C++20 compiler (VS Developer PowerShell on Windows).'
    includes = [ROOT / 'src/common', ROOT / 'src/common/Utilities', ROOT / 'src/server/shared/Packets',
                ROOT / 'src/server/game/Server', ROOT / 'src/server/game/Server/Protocol']
    with tempfile.TemporaryDirectory(prefix='coa-extension-packets-') as directory:
        out = Path(directory)
        cpp = out / 'harness.cpp'
        cpp.write_text(harness, encoding='utf-8')
        executable = out / ('regressions.exe' if os.name == 'nt' else 'regressions')
        if Path(compiler).stem.lower() == 'cl':
            flags = ['/nologo', '/std:c++20', '/EHsc', '/utf-8', *['/I' + str(p) for p in includes],
                     str(cpp), '/Fe' + str(executable)]
        else:
            flags = ['-std=c++20', '-Wall', '-Wextra', '-Werror', '-Wno-unused-const-variable',
                     *['-I' + str(p) for p in includes], str(cpp), '-o', str(executable)]
        subprocess.run([compiler, *flags], cwd=out, check=True)
        return subprocess.run([str(executable)], cwd=out).returncode


if __name__ == '__main__':
    sys.exit(main())
