"""Checks for the CoA chat emoji client files.

The data, texture, TOC and installer checks need only the standard library. The Lua checks run ChatEmoji.lua in a Lua
5.1 state against wow_stub.lua and need "pip install lupa"; without it they are skipped.
"""

from pathlib import Path
import struct
import tempfile
import unittest

import apply_client
import build

try:
    from lupa.lua51 import LuaRuntime
except ImportError:
    LuaRuntime = None

FRAMEXML = build.CLIENT / 'FrameXML'
STUB = build.ROOT / 'wow_stub.lua'
TESTS = build.ROOT / 'test_chat_emoji.lua'
SAMPLE_TOC = 'Util.lua\nChatFrame.xml\nFloatingChatFrame.xml\nChatConfigFrame.xml\n'


class DataTests(unittest.TestCase):
    def test_emoji_json_is_valid(self):
        entries = build.load_emoji()
        self.assertGreaterEqual(len(entries), 100)
        names = {entry['name'] for entry in entries}
        self.assertLessEqual({'melting_face', 'thumbsup', 'joy', 'skull'}, names)

    def test_generated_files_are_current(self):
        self.assertEqual(build.check(build.load_emoji()), [])

    def test_textures_are_uncompressed_32_bit_tga_with_real_alpha(self):
        for entry in build.load_emoji():
            with self.subTest(entry=entry['name']):
                data = build.texture_path(entry).read_bytes()
                id_length, color_map, image_type = data[0], data[1], data[2]
                width, height = struct.unpack('<HH', data[12:16])
                depth = data[16]
                self.assertEqual((color_map, image_type, width, height, depth), (0, 2, 64, 64, 32))
                pixels = data[18 + id_length:18 + id_length + width * height * 4]
                self.assertEqual(len(pixels), width * height * 4)
                alpha = set(pixels[3::4])
                self.assertIn(0, alpha, 'corners should be transparent')
                self.assertIn(255, alpha, 'the emoji itself should be opaque')

    def test_button_glyph_is_a_32_bit_tga_with_a_transparent_corner_and_an_opaque_middle(self):
        data = build.glyph_path().read_bytes()
        id_length, color_map, image_type = data[0], data[1], data[2]
        width, height = struct.unpack('<HH', data[12:16])
        self.assertEqual((color_map, image_type, width, height, data[16]),
                         (0, 2, build.GLYPH_SIZE, build.GLYPH_SIZE, 32))
        pixels = data[18 + id_length:18 + id_length + width * height * 4]
        self.assertEqual(len(pixels), width * height * 4)

        def alpha(x, y):
            return pixels[(y * width + x) * 4 + 3]
        self.assertEqual(alpha(0, 0), 0)
        self.assertEqual(alpha(width // 2, height // 2), 255)

    def test_check_expects_the_button_glyph_and_flags_strays(self):
        entries = [{'name': 'a1', 'codepoints': '1f600'}]
        originals = build.TEXTURE_DIR, build.DATA_LUA
        try:
            with tempfile.TemporaryDirectory() as directory:
                build.TEXTURE_DIR = Path(directory) / 'CoAEmoji'
                build.DATA_LUA = Path(directory) / 'ChatEmojiData.lua'
                build.TEXTURE_DIR.mkdir()
                build.DATA_LUA.write_text(build.render_lua(entries), encoding='utf-8', newline='\n')
                (build.TEXTURE_DIR / '1f600.tga').write_bytes(b'x')
                self.assertEqual(build.check(entries), ['missing texture button.tga'])
                (build.TEXTURE_DIR / 'button.tga').write_bytes(b'x')
                self.assertEqual(build.check(entries), [])
                (build.TEXTURE_DIR / 'stray.tga').write_bytes(b'x')
                self.assertEqual(build.check(entries), ['texture stray.tga has no emoji.json entry'])
        finally:
            build.TEXTURE_DIR, build.DATA_LUA = originals

    def test_data_file_rows_follow_emoji_json(self):
        entries = build.load_emoji()
        rendered = build.render_lua(entries)
        self.assertEqual(rendered.count('\n    {'), len(entries))
        self.assertIn('{"thumbsup", "1f44d", "+1"},', rendered)

    def test_lua_files_use_the_documented_paths(self):
        code = (FRAMEXML / 'ChatEmoji.lua').read_text(encoding='utf-8')
        self.assertIn('EMOJI_DIR = "Interface\\\\CoAEmoji\\\\"', code)
        self.assertIn('BUTTON_GLYPH = EMOJI_DIR .. "button.tga"', code)
        self.assertEqual(build.BUTTON_GLYPH, 'button.tga')
        self.assertEqual(build.TEXTURE_DIR.name, 'CoAEmoji')

    def test_build_rejects_bad_entries(self):
        original = build.EMOJI_JSON
        cases = {
            'duplicate name': '[{"name": "a1", "codepoints": "1f600"}, {"name": "a1", "codepoints": "1f601"}]',
            'alias clash': '[{"name": "a1", "codepoints": "1f600"}, '
                           '{"name": "b1", "aliases": ["a1"], "codepoints": "1f601"}]',
            'shared art': '[{"name": "a1", "codepoints": "1f600"}, {"name": "b1", "codepoints": "1f600"}]',
            'upper case': '[{"name": "Joy", "codepoints": "1f600"}]',
            'bad codepoints': '[{"name": "a1", "codepoints": "1F600"}]',
        }
        try:
            with tempfile.TemporaryDirectory() as directory:
                for label, text in cases.items():
                    with self.subTest(label):
                        build.EMOJI_JSON = Path(directory) / 'emoji.json'
                        build.EMOJI_JSON.write_text(text, encoding='utf-8')
                        with self.assertRaises(ValueError):
                            build.load_emoji()
        finally:
            build.EMOJI_JSON = original


class TocTests(unittest.TestCase):
    def test_lines_go_right_after_floating_chat_frame(self):
        self.assertEqual(apply_client.patch_toc(SAMPLE_TOC),
                         'Util.lua\nChatFrame.xml\nFloatingChatFrame.xml\nChatEmojiData.lua\nChatEmoji.lua\n'
                         'ChatConfigFrame.xml\n')

    def test_patching_twice_changes_nothing_more(self):
        once = apply_client.patch_toc(SAMPLE_TOC)
        self.assertEqual(apply_client.patch_toc(once), once)

    def test_unpatch_restores_the_original(self):
        self.assertEqual(apply_client.unpatch_toc(apply_client.patch_toc(SAMPLE_TOC)), SAMPLE_TOC)

    def test_crlf_files_stay_crlf(self):
        patched = apply_client.patch_toc(SAMPLE_TOC.replace('\n', '\r\n'))
        self.assertNotIn('\n\n', patched.replace('\r\n', ''))
        self.assertIn('ChatEmoji.lua\r\nChatConfigFrame.xml\r\n', patched)

    def test_missing_or_repeated_anchor_is_an_error(self):
        with self.assertRaises(ValueError):
            apply_client.patch_toc('Util.lua\n')
        with self.assertRaises(ValueError):
            apply_client.patch_toc('FloatingChatFrame.xml\nFloatingChatFrame.xml\n')


class InstallTests(unittest.TestCase):
    def setUp(self):
        self.directory = tempfile.TemporaryDirectory()
        self.client = Path(self.directory.name)
        (self.client / 'Data').mkdir()
        self.toc = self.client / apply_client.TOC

    def tearDown(self):
        self.directory.cleanup()

    def installed(self):
        return sorted(path.relative_to(self.client).as_posix() for path in (self.client / 'Interface').rglob('*')
                      if path.is_file()) if (self.client / 'Interface').is_dir() else []

    def test_install_copies_every_file_and_patches_the_toc(self):
        apply_client.install(self.client, SAMPLE_TOC)
        files = self.installed()
        self.assertIn('Interface/FrameXML/ChatEmoji.lua', files)
        self.assertIn('Interface/FrameXML/ChatEmojiData.lua', files)
        self.assertIn('Interface/CoAEmoji/button.tga', files)
        self.assertEqual(sum(name.startswith('Interface/CoAEmoji/') for name in files), len(build.load_emoji()) + 1)
        self.assertIn('ChatEmoji.lua', self.toc.read_text())

    def test_revert_removes_everything_when_there_was_no_loose_toc(self):
        apply_client.install(self.client, SAMPLE_TOC)
        apply_client.revert(self.client)
        self.assertEqual(self.installed(), [])
        self.assertFalse((self.client / 'Interface' / 'FrameXML').exists())
        self.assertFalse((self.client / 'Interface' / 'CoAEmoji').exists())

    def test_revert_restores_a_loose_toc_that_was_already_there(self):
        self.toc.parent.mkdir(parents=True)
        self.toc.write_text(SAMPLE_TOC)
        apply_client.install(self.client, SAMPLE_TOC)
        apply_client.revert(self.client)
        self.assertEqual(self.toc.read_text(), SAMPLE_TOC)
        self.assertEqual(self.installed(), ['Interface/FrameXML/FrameXML.toc'])

    def test_installing_twice_keeps_the_original_backup(self):
        self.toc.parent.mkdir(parents=True)
        self.toc.write_text(SAMPLE_TOC)
        apply_client.install(self.client, SAMPLE_TOC)
        apply_client.install(self.client, self.toc.read_text())
        apply_client.revert(self.client)
        self.assertEqual(self.toc.read_text(), SAMPLE_TOC)

    def test_unrelated_loose_files_survive_a_revert(self):
        other = self.client / 'Interface' / 'FrameXML' / 'DressUpFrame.lua'
        other.parent.mkdir(parents=True)
        other.write_text('keep me')
        apply_client.install(self.client, SAMPLE_TOC)
        apply_client.revert(self.client)
        self.assertEqual(other.read_text(), 'keep me')

    def test_refuses_a_folder_that_is_not_a_client(self):
        with tempfile.TemporaryDirectory() as empty:
            self.assertEqual(apply_client.main([empty]), 1)


def lua_test_names():
    lua = LuaRuntime(unpack_returned_tuples=True)
    tests = load_lua(lua)
    names = lua.eval('function(tests) local names = {} for name in pairs(tests) do names[#names + 1] = name end '
                     'table.sort(names) return names end')(tests)
    return list(names.values())


def load_lua(lua):
    run = lua.eval('function(source, name) return assert(loadstring(source, "@" .. name))() end')
    for path in (STUB, FRAMEXML / 'ChatEmojiData.lua', FRAMEXML / 'ChatEmoji.lua'):
        run(path.read_text(encoding='utf-8'), path.name)
    return run(TESTS.read_text(encoding='utf-8'), TESTS.name)


@unittest.skipIf(LuaRuntime is None, 'pip install lupa to run the Lua checks')
class LuaTests(unittest.TestCase):
    pass


def add_lua_test(name):
    def run(self):
        lua = LuaRuntime(unpack_returned_tuples=True)
        load_lua(lua)[name]()
    run.__name__ = 'test_' + name
    setattr(LuaTests, run.__name__, run)


if LuaRuntime is not None:
    for _name in lua_test_names():
        add_lua_test(_name)


if __name__ == '__main__':
    unittest.main()
