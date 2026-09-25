import os
from pathlib import Path
import shutil
import struct
import subprocess
import tempfile


ROOT = Path(__file__).resolve().parents[3]
MODULE = ROOT / 'modules/mod-coa-challenges/src'


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

    assert take('<I') == 0
    values = {}
    for _ in range(take('<I')):
        length = take('<I')
        key = data[offset:offset + length].decode('ascii')
        offset += length
        assert key not in values
        values[key] = take('<B')
    assert [take('<I') for _ in range(4)] == [0, 0, 0, 0]
    assert offset == len(data)
    return values


def main():
    compiler = shutil.which(os.environ.get('CXX', 'c++'))
    assert compiler, 'A C++20 compiler is required'
    buffer_source = (ROOT / 'src/server/shared/Packets/ByteBuffer.cpp').read_text()
    support = '\n'.join(method(buffer_source, signature) for signature in (
        'void ByteBuffer::append(uint8 const* src, std::size_t cnt)',
        'ByteBufferPositionException::ByteBufferPositionException(',
    ))
    config_source = (MODULE / 'CoA.Challenges.Config.cpp').read_text()
    core_source = (MODULE / 'CoA.Challenges.Core.cpp').read_text()
    sender = method(config_source, 'void SendConfigBatch(Player* player)')
    append_string = method(core_source, 'void AppendConfigString(WorldPacket& data, std::string const& key)')
    with tempfile.TemporaryDirectory(prefix='coa-challenge-config-') as directory:
        out = Path(directory)
        (out / 'main.cpp').write_text('''#include "WorldPacket.h"
#include "Opcodes.h"
#include <cassert>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#define ASSERT(condition, ...) assert(condition)
#define LOG_INFO(...) do {} while (0)
''' + support + '''
class WorldSession
{
public:
    WorldPacket sent;
    void SendPacket(WorldPacket const* packet) { sent = *packet; }
};
class Player
{
public:
    WorldSession session;
    WorldSession* GetSession() { return &session; }
    std::string GetName() const { return "Tester"; }
};
struct TestConfig
{
    bool challengeEnabled = true;
    template <typename T> T GetOption(char const* key, T fallback)
    {
        if (std::string(key) == "CoAChallenges.ChallengeEnabled")
            return challengeEnabled;
        if (std::string(key) == "CoAChallenges.ChallengeCreatorEnabled")
            return false;
        return fallback;
    }
};
TestConfig config;
TestConfig* sConfigMgr = &config;
namespace CoAChallenges
{
    struct GameModeDef
    {
        char const* configKey;
        char const* hiddenKey;
        char const* name;
    };
    GameModeDef GameModes[] = { { "MODE", "HIDDEN", "Mode" } };
    bool ChallengesEnabled() { return true; }
    bool GameModesEnabled() { return false; }
    bool GameModeHidden(char const*) { return false; }
''' + append_string + '\n' + sender + '''
}
int main(int, char** argv)
{
    for (int value = 0; value != 2; ++value)
    {
        config.challengeEnabled = value;
        Player player;
        CoAChallenges::SendConfigBatch(&player);
        assert(player.session.sent.GetOpcode() == 0x58D);
        std::ofstream file(std::string(argv[1]) + std::to_string(value), std::ios::binary);
        file.write(reinterpret_cast<char const*>(player.session.sent.contents()), player.session.sent.size());
    }
}
''')
        include_dirs = [out, ROOT / 'src/common', ROOT / 'src/common/Utilities',
                        ROOT / 'src/server/shared/Packets', ROOT / 'src/server/game/Server',
                        ROOT / 'src/server/game/Server/Protocol']
        command = [compiler, '-std=c++20', '-Wall', '-Wextra', '-Werror']
        command += [flag for path in include_dirs for flag in ('-I', str(path))]
        command += [str(out / 'main.cpp'), '-o', str(out / 'test')]
        subprocess.run(command, check=True)
        subprocess.run([str(out / 'test'), str(out / 'packet')], check=True)
        for value in range(2):
            values = decode((out / f'packet{value}').read_bytes())
            assert values['CONFIG_CHALLENGE_ENABLED'] == value
            assert values['CONFIG_CHALLENGE_CREATOR_ENABLED'] == 0
    print('PASS: Challenges config packet has all six sections and the tab flags follow config')


if __name__ == '__main__':
    main()
