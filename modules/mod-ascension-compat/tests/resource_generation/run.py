"""Compile the actual central gain dispatch and mutation methods for issue #77."""
import argparse
import os
from pathlib import Path
import runpy
import subprocess
import struct
import tempfile

ROOT = Path(__file__).resolve().parents[4]
MODULE = ROOT / 'modules/mod-ascension-compat/src'


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--before', help='Use this Git revision of the resource table as a negative control')
    args = parser.parse_args()
    extract = runpy.run_path(str(ROOT / 'modules/mod-ascension-compat/tests/client_compat/run.py'))['method']
    service = extract((MODULE / 'AscensionCompat.cpp').read_text(), 'class AscensionResourceService')
    methods = [extract(service, signature) for signature in (
        'static bool SpellDealsDamage(', 'static bool MatchesGainRule(',
        'static bool ApplyGainRule(', 'static void ModifyAuraStacks(')]
    # Exercise the production resource-gain loops; native power and spending have separate fixtures.
    cast = extract(service, 'void OnSpellCast(')
    methods.append(cast[:cast.index('        for (AscensionCompatData::NativePowerGainRule')] + '\n}')
    hit = extract(service, 'void OnSpellHitResult(')
    methods.append(hit[:hit.index('        for (AscensionCompatData::NativePowerGainRule')] + '\n(void)changed;\n}')
    header = (subprocess.check_output(['git', 'show',
              f'{args.before}:modules/mod-ascension-compat/src/AscensionCustomResourceData.h'], cwd=ROOT, text=True)
              if args.before else (MODULE / 'AscensionCustomResourceData.h').read_text())
    code = header + Path(__file__).with_name('harness.cpp').read_text()
    code = code.replace('// NATIVE_STACK', extract(
        (ROOT / 'src/server/game/Spells/Auras/SpellAuras.cpp').read_text(), 'bool Aura::ModStackAmount('))
    raw = (ROOT.parent / 'runtime/server/data/dbc/Spell.dbc').read_bytes()
    count = struct.unpack_from('<I', raw, 4)[0]
    resources = {r[0]: r[49] for r in struct.iter_unpack('<234I', raw[20:20 + count * 936])
                 if r[0] in {800058, 803102, 500906}}
    assert resources == {800058: 6, 803102: 100, 500906: 6}, resources
    code += '\nstruct ResourceService {\n' + '\n'.join(methods) + '\n};\n'
    code += Path(__file__).with_name('cases.cpp').read_text()
    compiler = str(Path(os.environ['VCToolsInstallDir']) / 'bin/Hostx64/x64/cl.exe')
    with tempfile.TemporaryDirectory(prefix='coa-resource-generation-') as directory:
        out = Path(directory)
        cpp, exe = out / 'resources.cpp', out / 'resources.exe'
        cpp.write_text(code, encoding='utf-8')
        subprocess.run([compiler, '/nologo', '/std:c++20', '/EHsc', '/W4', '/WX', '/utf-8',
                        str(cpp), '/Fe' + str(exe)], cwd=out, check=True, timeout=60)
        subprocess.run([str(exe)], cwd=out, check=True, timeout=15)
    print('PASS: resource-gain event loops, ranks, hostile/miss/trigger gates, per-cast/per-target grants,\n      ward/caps and mitigated-to-zero damaging hits')


if __name__ == '__main__':
    main()
