import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[3]
MODULE = ROOT / 'src/server/coa'
INCLUDE_DIRS = [MODULE, ROOT / 'src/common', ROOT / 'src/common/Utilities', ROOT / 'src/server/shared/Packets',
                ROOT / 'src/server/game/Server', ROOT / 'src/server/game/Server/Protocol']


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

    sections = []
    for value_format in ('<i', '<B', '<f', '<f'):
        values = {}
        for _ in range(take('<I')):
            length = take('<I')
            key = data[offset:offset + length].decode('ascii')
            offset += length
            assert '\0' not in key and key not in values
            values[key] = take(value_format)
        sections.append(values)
    assert [take('<I') for _ in range(2)] == [0, 0]
    assert offset == len(data)
    return sections


def config_names():
    config = (ROOT / 'src/server/game/World/WorldConfig.h').read_text()
    config_enum = re.search(r'enum ServerConfigs\s*\{.*?\};', config, re.S)[0]
    return config_enum, re.findall(r'^\s*(\w+)\s*(?:=\s*0)?\s*[,}]', config_enum, re.M)


def write_support(out):
    config_enum, _ = config_names()
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
    buffer_source = (ROOT / 'src/server/shared/Packets/ByteBuffer.cpp').read_text()
    return '\n'.join(method(buffer_source, signature) for signature in (
        'void ByteBuffer::append(uint8 const* src, std::size_t cnt)',
        'ByteBufferPositionException::ByteBufferPositionException(',
    ))


def compile_and_run(out, main_source, packet_prefix):
    compiler = shutil.which(os.environ.get('CXX', 'c++'))
    assert compiler, 'A C++20 compiler is required'
    (out / 'main.cpp').write_text(main_source)
    command = [compiler, '-std=c++20', '-Wall', '-Wextra', '-Werror']
    command += [flag for path in [out, *INCLUDE_DIRS] for flag in ('-I', str(path))]
    command += [str(out / 'main.cpp'), str(MODULE / 'AscensionCoAConfig.cpp'), '-o', str(out / 'test')]
    subprocess.run(command, check=True)
    subprocess.run([str(out / 'test'), str(out / packet_prefix)], check=True)


def expected_rates(scale):
    _, names = config_names()
    expected = {'RATE_XP_GLOBAL': 'RATE_XP_GLOBAL', 'RATE_XP_PROFESSION': 'RATE_XP_PROFESSION'}
    for key in ('KILL KILL_TBC KILL_WOTLK QUEST QUEST_TBC QUEST_WOTLK EXPLORE ELITE DUNGEON_ELITE').split():
        expected[f'RATE_XP_{key}'] = f'RATE_XP_{key}'
    for suffix in ('GRAY GREEN YELLOW ORANGE MINING HERBALISM DISENCHANTING SKINNING FISHING '
                   'BLACKSMITHING JEWELCRAFTING ALCHEMY ENCHANTING LEATHERWORKING FIRST_AID '
                   'COOKING ENGINEERING TAILORING LOCKPICKING INSCRIPTION').split():
        expected[f'RATE_XP_PROFESSION_{suffix}_MODIFIER'] = f'RATE_XP_PROFESSION_{suffix}'
    return {key: scale * (names.index(setting) + 0.25) for key, setting in expected.items()}


def main():
    with tempfile.TemporaryDirectory(prefix='coa-config-') as directory:
        out = Path(directory)
        support = write_support(out)
        compile_and_run(out, '''#include "AscensionCoAConfig.h"
#include "World.h"
#include "WorldSession.h"
#include <cassert>
#include <fstream>
#include <sstream>
#define ASSERT(condition, ...) assert(condition)
''' + support + '''
void Write(char const* prefix, int index, WorldPacket const& packet)
{
    assert(packet.GetOpcode() == 0x58D);
    std::ofstream file(std::string(prefix) + std::to_string(index), std::ios::binary);
    file.write(reinterpret_cast<char const*>(packet.contents()), packet.size());
}

int main(int, char** argv)
{
    SendAscensionCoAConfig(nullptr);
    for (int i = 0; i != 3; ++i)
    {
        world.scale = i;
        WorldSession session;
        SendAscensionCoAConfig(&session);
        Write(argv[1], i, session.sent);
    }
    RegisterAscensionClientConfig([](AscensionClientConfig& config)
    {
        config.Integers.emplace_back("CONFIG_TEST_INTEGER", -3);
        config.Booleans.emplace_back("CONFIG_TEST_ON", true);
        config.Floats.emplace_back("CONFIG_TEST_FLOAT", 1.5f);
        config.Rates.emplace_back("RATE_TEST", 2.5f);
    });
    RegisterAscensionClientConfig([](AscensionClientConfig& config)
    {
        config.Booleans.emplace_back("CONFIG_TEST_OFF", false);
    });
    world.scale = 1;
    Write(argv[1], 3, BuildAscensionCoAConfig());
}
''', 'packet')
        for scale in range(3):
            integers, booleans, floats, rates = decode((out / f'packet{scale}').read_bytes())
            assert (integers, booleans, floats) == ({}, {}, {})
            assert rates == expected_rates(scale)
        integers, booleans, floats, rates = decode((out / 'packet3').read_bytes())
        assert integers == {'CONFIG_TEST_INTEGER': -3}
        assert booleans == {'CONFIG_TEST_ON': 1, 'CONFIG_TEST_OFF': 0}
        assert floats == {'CONFIG_TEST_FLOAT': 1.5}
        assert rates == {**expected_rates(1), 'RATE_TEST': 2.5}
    print(f'PASS: opcode, six sections, {len(expected_rates(1))} rate mappings, key framing, refreshed rates, '
          'null session, registered integer, boolean, float and rate values share one packet')


if __name__ == '__main__':
    main()
