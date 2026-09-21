import os
from pathlib import Path
import tempfile
import unittest
import xml.etree.ElementTree as ET

from lupa.lua51 import LuaRuntime

import patch_character_names as ui


SOURCE = Path(os.environ.get('COA_CLIENT_UI_SOURCE', 'C:/Ascension/client-reference/patch-B'))


class NamesTest(unittest.TestCase):
    def setUp(self):
        self.lua = LuaRuntime(unpack_returned_tuples=True)
        self.lua.execute('function GetLocale() return "enUS" end')
        self.lua.execute((Path(__file__).parent / 'character-names/CharacterNames.lua').read_text(encoding='utf-8'))
        self.names = self.lua.globals().CoACharacterNames

    def test_canonical_names(self):
        for name, expected in [('aRTHAS', 'Arthas'), ('aRTHAS mENETHIL', 'Arthas Menethil'),
                               ('ИВАН ГРОМОВ', 'Иван Громов'), ('ÉMILIE ÉCLAIR', 'Émilie Éclair'),
                               ('ßANDER STRAßE', 'ẞander Straße')]:
            self.assertEqual(self.names.Normalize(name), expected)

    def test_optional_second_name(self):
        self.lua.execute('''
          CharacterCreateNameEdit = { GetText = function() return " aRTHAS " end }
          CharacterCreateSecondNameEdit = { GetText = function() return "" end }
        ''')
        self.assertEqual(self.names.CreationName(), 'Arthas')
        self.lua.execute('CharacterCreateSecondNameEdit.GetText = function() return " mENETHIL " end')
        self.assertEqual(self.names.CreationName(), 'Arthas Menethil')

    def test_recipient_boundary(self):
        self.assertEqual(self.names.ExtractRecipient('Arthas hello'), ('Arthas', 'hello'))
        self.assertEqual(self.names.ExtractRecipient('"Arthas Menethil" hello there'),
                         ('Arthas Menethil', 'hello there'))
        self.assertIsNone(self.names.ExtractRecipient('"Arthas Menethil'))
        self.assertIsNone(self.names.ExtractRecipient('"Arthas Menethil"message'))
        self.assertEqual(self.names.Quote('Arthas Menethil'), '"Arthas Menethil"')
        self.assertEqual(self.names.Quote('Arthas'), 'Arthas')

    def test_creation_rejects_combined_byte_overflow_before_native_copy(self):
        self.lua.execute('''
          CHAR_NAME_TOO_LONG = "too long"
          function GlueDialog_Show(kind, text) lastError = text end
          function CreateCharacter(name, faction) submittedName, submittedFaction = name, faction end
          CharacterCreateNameEdit = { GetText = function() return "Абвгдежзийкл" end }
          CharacterCreateSecondNameEdit = { GetText = function() return "Мнопрстуфхцч" end }
        ''')
        self.names.CreateCharacter(1)
        self.assertIsNone(self.lua.globals().submittedName)
        self.assertEqual(self.lua.globals().lastError, 'too long')
        self.lua.execute('CharacterCreateSecondNameEdit.GetText = function() return "Мнопрстуфхц" end')
        self.names.CreateCharacter(1)
        self.assertEqual(self.lua.globals().submittedName, 'Абвгдежзийкл Мнопрстуфхц')
        self.assertEqual(self.lua.globals().submittedFaction, 1)


@unittest.skipUnless((SOURCE / ui.MEMBERS[0]).exists(), 'Set COA_CLIENT_UI_SOURCE to canonical patch-B')
class ActualClientCallbacksTest(NamesTest):
    @classmethod
    def setUpClass(cls):
        cls.folder = tempfile.TemporaryDirectory()
        cls.members = ui.generate(SOURCE, Path(cls.folder.name) / 'candidate')

    @classmethod
    def tearDownClass(cls):
        cls.folder.cleanup()

    def setUp(self):
        super().setUp()
        self.lua.execute('''
          strmatch, strfind, strsub, strlen = string.match, string.find, string.sub, string.len
          SLASH_WHISPER1 = "/w"
          edit = { attrs = {}, text = "", history = {} }
          function edit:SetAttribute(key, value) self.attrs[key] = value end
          function edit:GetAttribute(key) return self.attrs[key] end
          function edit:SetText(text) self.text = text end
          function edit:GetText() return self.text end
          function edit:AddHistoryLine(text) table.insert(self.history, text) end
          function ChatEdit_ChooseBoxForSend() return edit end
          function ChatEdit_GetActiveWindow() return edit end
          function ChatEdit_UpdateHeader(box) box.header = box.attrs.tellTarget end
          function GetAutoCompleteResults() return nil end
          tellTargetExtractionAutoComplete = {}
        ''')
        chat = self.members['Interface/FrameXML/ChatFrame.lua']
        for function, next_function in [('ChatFrame_SendTell', 'ChatFrame_ReplyTell'),
                                        ('ChatEdit_ExtractTellTarget', 'ChatEdit_ExtractChannel'),
                                        ('ChatEdit_AddHistory', 'ChatEdit_SendText')]:
            start = chat.index('function ' + function + '(')
            end = chat.index('\nfunction ' + next_function + '(', start)
            self.lua.execute(chat[start:end])

    def test_click_whisper_and_history_keep_full_recipient(self):
        self.lua.globals().ChatFrame_SendTell('Arthas Menethil', None)
        edit = self.lua.globals().edit
        self.assertEqual(edit.attrs.tellTarget, 'Arthas Menethil')
        self.assertEqual(edit.header, 'Arthas Menethil')
        edit.text = 'hello'
        self.lua.globals().ChatEdit_AddHistory(edit)
        self.assertEqual(edit.history[1], '/w "Arthas Menethil" hello')
        self.assertTrue(self.lua.globals().ChatEdit_ExtractTellTarget(edit, '"Arthas Menethil" hello'))
        self.assertEqual(edit.attrs.tellTarget, 'Arthas Menethil')
        self.assertEqual(edit.text, 'hello')

    def test_single_recipient_and_incomplete_quotes(self):
        edit = self.lua.globals().edit
        self.assertFalse(self.lua.globals().ChatEdit_ExtractTellTarget(edit, '"Arthas Menethil'))
        self.assertIsNone(edit.attrs.tellTarget)
        self.assertTrue(self.lua.globals().ChatEdit_ExtractTellTarget(edit, 'Arthas hello'))
        self.assertEqual(edit.attrs.tellTarget, 'Arthas')
        self.assertEqual(edit.text, 'hello')

    def test_shift_click_who_keeps_full_name(self):
        self.lua.execute(self.members['Interface/FrameXML/ItemRef.lua'])
        self.lua.execute('''
          WHO_TAG_NAME = "n-"
          function strsplit(delimiter, text) return text:match("^[^:]+") end
          function IsModifiedClick() return true end
          function StaticPopup_Visible() return nil end
          function ChatEdit_GetActiveWindow() return nil end
          function SendWho(query) whoQuery = query end
        ''')
        self.lua.globals().SetItemRef('player:Arthas Menethil:1', None, 'LeftButton', None)
        self.assertEqual(self.lua.globals().whoQuery, 'n-"Arthas Menethil"')
        self.lua.globals().SetItemRef('player:Arthas:1', None, 'LeftButton', None)
        self.assertEqual(self.lua.globals().whoQuery, 'n-Arthas')

    def test_generated_lua_and_xml_parse(self):
        compile_lua = self.lua.eval('function(source) local fn, err = loadstring(source); assert(fn, err) end')
        for path, source in self.members.items():
            with self.subTest(member=path):
                if path.endswith('.lua'):
                    compile_lua(source)
                elif path.endswith('.xml'):
                    root = ET.fromstring(source)
                    for element in root.iter():
                        if element.tag.split('}')[-1].startswith('On') and element.text:
                            compile_lua('return function(self, ...) ' + element.text + '\nend')

    def test_unknown_source_fails_closed(self):
        with self.assertRaises(ValueError):
            ui.transform('Interface/FrameXML/ChatFrame.lua', '-- unrelated client')


if __name__ == '__main__':
    unittest.main()
