import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[3]
MODULE = ROOT / 'src/server/coa'


def method(source, signature):
    start = source.index(signature)
    end = source.index('{', start) + 1
    depth = 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]


def decode(data):
    offset = 0

    def take(fmt):
        nonlocal offset
        result = struct.unpack_from(fmt, data, offset)[0]
        offset += struct.calcsize(fmt)
        return result

    assert [take('<I') for _ in range(3)] == [0, 0, 0]
    rates = {}
    for _ in range(take('<I')):
        length = take('<I')
        key = data[offset:offset + length].decode('ascii')
        offset += length
        assert '\0' not in key and key not in rates
        rates[key] = take('<f')
    assert [take('<I') for _ in range(2)] == [0, 0]
    assert offset == len(data)
    return rates


def main():
    compiler = shutil.which(os.environ.get('CXX', 'c++'))
    assert compiler, 'A C++20 compiler is required'
    config = (ROOT / 'src/server/game/World/WorldConfig.h').read_text()
    config_enum = re.search(r'enum ServerConfigs\s*\{.*?\};', config, re.S)[0]
    config_names = re.findall(r'^\s*(\w+)\s*(?:=\s*0)?\s*[,}]', config_enum, re.M)
    expected = {'RATE_XP_GLOBAL': 'RATE_XP_GLOBAL', 'RATE_XP_PROFESSION': 'RATE_XP_PROFESSION'}
    for key in ('KILL KILL_TBC KILL_WOTLK QUEST QUEST_TBC QUEST_WOTLK EXPLORE ELITE DUNGEON_ELITE').split():
        expected[f'RATE_XP_{key}'] = f'RATE_XP_{key}'
    for suffix in ('GRAY GREEN YELLOW ORANGE MINING HERBALISM DISENCHANTING SKINNING FISHING '
                   'BLACKSMITHING JEWELCRAFTING ALCHEMY ENCHANTING LEATHERWORKING FIRST_AID '
                   'COOKING ENGINEERING TAILORING LOCKPICKING INSCRIPTION').split():
        expected[f'RATE_XP_PROFESSION_{suffix}_MODIFIER'] = f'RATE_XP_PROFESSION_{suffix}'
    buffer_source = (ROOT / 'src/server/shared/Packets/ByteBuffer.cpp').read_text()
    support = '\n'.join(method(buffer_source, signature) for signature in (
        'void ByteBuffer::append(uint8 const* src, std::size_t cnt)',
        'ByteBufferPositionException::ByteBufferPositionException(',
    ))
    with tempfile.TemporaryDirectory(prefix='coa-config-') as directory:
        out = Path(directory)
        (out / 'World.h').write_text('#pragma once\n#include "Define.h"\n' + config_enum + '''
struct TestWorld
{
    float scale = 1;
    float getRate(ServerConfigs setting) const { return scale * (float(setting) + 0.25f); }
};
inline TestWorld world;
inline TestWorld* sWorld = &world;
''')
        (out / 'WorldSession.h').write_text('''#pragma once
#include "WorldPacket.h"
class WorldSession
{
public:
    WorldPacket sent;
    void SendPacket(WorldPacket const* packet) { sent = *packet; }
};
''')
        (out / 'main.cpp').write_text('''#include "AscensionCoAConfig.h"
#include "World.h"
#include "WorldSession.h"
#include <cassert>
#include <fstream>
#include <sstream>
#define ASSERT(condition, ...) assert(condition)
''' + support + '''
int main(int, char** argv)
{
    SendAscensionCoAXpConfig(nullptr);
    for (int i = 0; i != 3; ++i)
    {
        world.scale = i;
        WorldSession session;
        SendAscensionCoAXpConfig(&session);
        assert(session.sent.GetOpcode() == 0x58D);
        std::ofstream file(std::string(argv[1]) + std::to_string(i), std::ios::binary);
        file.write(reinterpret_cast<char const*>(session.sent.contents()), session.sent.size());
    }
}
''')
        include_dirs = [out, MODULE, ROOT / 'src/common', ROOT / 'src/common/Utilities',
                        ROOT / 'src/server/shared/Packets', ROOT / 'src/server/game/Server',
                        ROOT / 'src/server/game/Server/Protocol']
        command = [compiler, '-std=c++20', '-Wall', '-Wextra', '-Werror']
        command += [flag for path in include_dirs for flag in ('-I', str(path))]
        command += [str(out / 'main.cpp'), str(MODULE / 'AscensionCoAConfig.cpp'), '-o', str(out / 'test')]
        subprocess.run(command, check=True)
        subprocess.run([str(out / 'test'), str(out / 'packet')], check=True)
        for scale in range(3):
            rates = decode((out / f'packet{scale}').read_bytes())
            assert rates == {key: scale * (config_names.index(setting) + 0.25)
                             for key, setting in expected.items()}
    print(f'PASS: opcode, six sections, {len(expected)} rate mappings, key framing, zero rates, refreshed rates, '
          'null session')


if __name__ == '__main__':
    main()
