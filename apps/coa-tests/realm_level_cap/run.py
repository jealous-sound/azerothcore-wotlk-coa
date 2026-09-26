import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[3]
MODULE = ROOT / 'src/server/coa'
SMSG_GAME_EVENT_INFO = 0x9BD
GAME_EVENT_INFO_SIZE = 52
WRATH_REALM_MAX_LEVEL = 80
CLIENT_CAP_EVENTS = [(356, None), (173, 70), (490, 60), (489, 55), (488, 45), (487, 35), (486, 25), (352, 60)]
REALM_LEVELS = [25, 35, 45, 55, 60, 65, 70, 80]


def method(source, signature):
    start = source.index(signature)
    end = source.index('{', start) + 1
    depth = 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]


def decode(data):
    assert len(data) == GAME_EVENT_INFO_SIZE
    event_id, state, start, end, active, length = struct.unpack_from('<HBQQBI', data)
    assert (state, start, end, length) == (0, 0, 0, 0)
    assert data[struct.calcsize('<HBQQBI'):] == bytes(GAME_EVENT_INFO_SIZE - struct.calcsize('<HBQQBI'))
    return event_id, bool(active)


def client_realm_max_level(active_events):
    for event_id, cap in CLIENT_CAP_EVENTS:
        if event_id in active_events:
            return WRATH_REALM_MAX_LEVEL if cap is None else min(WRATH_REALM_MAX_LEVEL, cap)
    return WRATH_REALM_MAX_LEVEL


def main():
    compiler = shutil.which(os.environ.get('CXX', 'c++'))
    assert compiler, 'A C++20 compiler is required'
    config = (ROOT / 'src/server/game/World/WorldConfig.h').read_text()
    config_enum = re.search(r'enum ServerConfigs\s*\{.*?\};', config, re.S)[0]
    buffer_source = (ROOT / 'src/server/shared/Packets/ByteBuffer.cpp').read_text()
    support = '\n'.join(method(buffer_source, signature) for signature in (
        'void ByteBuffer::append(uint8 const* src, std::size_t cnt)',
        'ByteBufferPositionException::ByteBufferPositionException(',
    ))
    levels = ', '.join(str(level) for level in REALM_LEVELS)
    with tempfile.TemporaryDirectory(prefix='coa-realm-level-cap-') as directory:
        out = Path(directory)
        (out / 'World.h').write_text('#pragma once\n#include "Define.h"\n' + config_enum + '''
struct TestWorld
{
    uint32 maxPlayerLevel = 80;
    uint32 getIntConfig(ServerConfigs setting) const { return setting == CONFIG_MAX_PLAYER_LEVEL ? maxPlayerLevel : 0; }
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
    int sentCount = 0;
    void SendPacket(WorldPacket const* packet) { sent = *packet; ++sentCount; }
};
''')
        (out / 'main.cpp').write_text('''#include "AscensionRealmLevelCap.h"
#include "World.h"
#include "WorldSession.h"
#include <cstdlib>
#include <fstream>
#include <string>
#define ASSERT(condition, ...) do { if (!(condition)) std::abort(); } while (0)
''' + support + '''
int main(int, char** argv)
{
    SendAscensionRealmLevelCap(nullptr);
    for (uint32 level : { ''' + levels + ''' })
    {
        world.maxPlayerLevel = level;
        WorldSession session;
        SendAscensionRealmLevelCap(&session);
        std::ofstream file(std::string(argv[1]) + std::to_string(level), std::ios::binary);
        if (!session.sentCount)
            continue;
        file << session.sentCount << ' ' << session.sent.GetOpcode() << '\\n';
        file.write(reinterpret_cast<char const*>(session.sent.contents()), session.sent.size());
    }
}
''')
        include_dirs = [out, MODULE, ROOT / 'src/common', ROOT / 'src/common/Utilities',
                        ROOT / 'src/server/shared/Packets', ROOT / 'src/server/game/Server',
                        ROOT / 'src/server/game/Server/Protocol']
        command = [compiler, '-std=c++20', '-Wall', '-Wextra', '-Werror']
        command += [flag for path in include_dirs for flag in ('-I', str(path))]
        command += [str(out / 'main.cpp'), str(MODULE / 'AscensionRealmLevelCap.cpp'), '-o', str(out / 'test')]
        subprocess.run(command, check=True)
        subprocess.run([str(out / 'test'), str(out / 'packet')], check=True)
        for level in REALM_LEVELS:
            data = (out / f'packet{level}').read_bytes()
            active_events = set()
            if data:
                header, payload = data.split(b'\n', 1)
                assert header == f'1 {SMSG_GAME_EVENT_INFO}'.encode()
                event_id, active = decode(payload)
                assert active
                active_events.add(event_id)
            expected = level if level in (25, 35, 45, 55, 60, 70) else WRATH_REALM_MAX_LEVEL
            assert client_realm_max_level(active_events) == expected, (level, active_events)
    print(f'PASS: opcode, {GAME_EVENT_INFO_SIZE}-byte event layout, client realm cap for levels {levels}, '
          'no packet without a client cap, null session')


if __name__ == '__main__':
    main()
